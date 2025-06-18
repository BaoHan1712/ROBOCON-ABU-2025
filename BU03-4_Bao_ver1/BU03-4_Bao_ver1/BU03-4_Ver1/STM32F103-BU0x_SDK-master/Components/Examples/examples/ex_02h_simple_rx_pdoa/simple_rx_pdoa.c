/*! ----------------------------------------------------------------------------
 *  @file    simple_rx_pdoa.c
 *  @brief   This examples prints the PDOA value to the virtual COM.
 *           The transmitter should be simple_tx_pdoa.c
 *           See note 3 regarding calibration and offset
 *
 * @attention
 *
 * Copyright 2019 - 2020 (c) Decawave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author Decawave
 */

#include <deca_device_api.h>
#include <deca_regs.h>
#include <deca_vals.h>
#include <shared_defines.h>
#include <shared_functions.h>
#include <example_selection.h>
#include "uwb.h"

#if defined(TEST_SIMPLE_RX_PDOA)

#define WINDOW_SIZE 1000

int giaTriCB = 0;
int bienTichLuy = 0;
int demLocNhieu = 0;

static void rx_ok_cb(const dwt_cb_data_t *cb_data);
static void rx_err_cb(const dwt_cb_data_t *cb_data);

/* Example application name and version to display on LCD screen. */
#define APP_NAME "PDOA example"

/* Default communication configuration. We use default non-STS DW mode. see note 2*/
static dwt_config_t config = {
        5,               /* ÐÅµÀºÅ. Channel number. */
        DWT_PLEN_128,     /* Preamble length. Used in TX only. */
        DWT_PAC8,        /* Preamble acquisition chunk size. Used in RX only. */
        9,               /* TxÇ°µ¼Âë. TX preamble code. Used in TX only. */
        9,               /* RxÇ°µ¼Âë. RX preamble code. Used in RX only. */
        1,               /* Ö¡·Ö¸ô·ûÄ£Ê½. 0 to use standard 8 symbol SFD, 1 to use non-standard 8 symbol, 2 for non-standard 16 symbol SFD and 3 for 4z 8 symbol SDF type */
        DWT_BR_6M8,      /* Êý¾ÝËÙÂÊ. Data rate. */
        DWT_PHRMODE_STD, /* ÎïÀí²ãÍ·Ä£Ê½. PHY header mode. */
        DWT_PHRRATE_STD, /* ÎïÀí²ãÍ·ËÙÂÊ. PHY header rate. */
        (129 + 8 - 8),    /* Ö¡·Ö¸ô·û³¬Ê±. SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
        (DWT_STS_MODE_1 | DWT_STS_MODE_SDC), /* STSÄ£Ê½. STS enabled */
        DWT_STS_LEN_64, /* STS³¤¶È. Cipher length see allowed values in Enum dwt_sts_lengths_e */
        DWT_PDOA_M3      /* PDOA mode 3 */
};


int16_t   pdoa_val=0;
uint8_t   pdoa_message_data[40];//Will hold the data to send to the virtual COM
#define WINDOW_SIZE 30
int pdoa_window[WINDOW_SIZE] = {0};
int pdoa_index = 0;

int median_filter(int new_val)
{
    pdoa_window[pdoa_index++] = new_val;
    if (pdoa_index >= WINDOW_SIZE) pdoa_index = 0;

    int temp[WINDOW_SIZE];
    memcpy(temp, pdoa_window, sizeof(temp));

    // S?p x?p m?ng t?m d? l?y giá tr? trung v?
    for (int i = 0; i < WINDOW_SIZE - 1; i++) {
        for (int j = i + 1; j < WINDOW_SIZE; j++) {
            if (temp[i] > temp[j]) {
                int t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
        }
    }

    return temp[WINDOW_SIZE / 2];
}


/**
 * Application entry point.
 */
int simple_rx_pdoa(void)
{

    int16_t   last_pdoa_val=0;

    /* ´®¿ÚÊä³öÓ¦ÓÃÃû³Æ. Sends application name to test_run_info function. */
    _dbg_printf((unsigned char *)APP_NAME);

    /* ÅäÖÃSPI¿ìËÙÂÊ. Configure SPI rate, DW IC supports up to 38 MHz */
    port_set_dw_ic_spi_fastrate();

    /* Ó²¸´Î»DW3000Ä£¿é. Reset DW IC */
    reset_DWIC(); /* Target specific drive of RSTn line into DW IC low for a period. */

    Sleep(2); // Time needed for DW3000 to start up (transition from INIT_RC to IDLE_RC

    /* ¼ì²éDW3000Ä£¿éÊÇ·ñ´¦ÓÚIDLE_RC */
    while (!dwt_checkidlerc()) /* Need to make sure DW IC is in IDLE_RC before proceeding */
    { };

    /* ³õÊ¼»¯DW3000Ä£¿é */
    if (dwt_initialise(DWT_DW_IDLE) == DWT_ERROR)
    {
        _dbg_printf((unsigned char *)"INIT FAILED");
        while (1)
        { };
    }

    /* ÅäÖÃDW3000ÐÅµÀ²ÎÊý. Configure DW3000. */
    if(dwt_configure(&config)) /* if the dwt_configure returns DWT_ERROR either the PLL or RX calibration has failed the host should reset the device */
    {
        _dbg_printf((unsigned char *)"CONFIG FAILED     ");
        while (1)
        { };
    }

    /* ×¢²árx»Øµ÷º¯Êý. Register RX call-back. */
    dwt_setcallbacks(NULL, rx_ok_cb, rx_err_cb, rx_err_cb, NULL, NULL);

    /* Ê¹ÄÜÖÐ¶Ï. Enable wanted interrupts (RX good frames and RX errors). */
    dwt_setinterrupt(SYS_ENABLE_LO_RXFCG_ENABLE_BIT_MASK | SYS_STATUS_ALL_RX_ERR, 0, DWT_ENABLE_INT);

    /* Çå³ýÖÐ¶Ï. Clearing the SPI ready interrupt*/
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RCINIT_BIT_MASK | SYS_STATUS_SPIRDY_BIT_MASK);

    /* Install DW IC IRQ handler. */
//    port_set_dwic_isr(dwt_isr);

    /* Á¢¼´¿ªÆô½ÓÊÜ. Activate reception immediately. See NOTE 1 below. */
    dwt_rxenable(DWT_START_RX_IMMEDIATE);

    port_EnableEXT_IRQ();

    _dbg_printf("PDoA³õÊ¼»¯³É¹¦\n");

    /*loop forever receiving frames*/
//    while (1)
//    {
//        if (last_pdoa_val!=pdoa_val)
//        {
//						uint8_t buf[32];
//						uint8_t bufss[32];
////						int pdoa_deg = 0;
//            last_pdoa_val=pdoa_val;
//						bienTichLuy+=last_pdoa_val;
//					
//						if(demLocNhieu++%10 == 0){
//							giaTriCB = bienTichLuy/10;
//							bienTichLuy = 0;
//						}
//            //sprintf((char *)&pdoa_message_data,"\n %d",last_pdoa_val);
//            _dbg_printf((unsigned char *)&pdoa_message_data);
//						
//						sprintf (buf, "%d",giaTriCB);
//						//sprintf(epkieu, "Tung ngu ne %d",5);
////						OLED_ShowStr (0, 0, "Goc ne:", 2);
////						OLED_ShowStr (0, 2, buf, 2);
//					
//						// Ð?m b?o xóa ph?n còn du c?a chu?i tru?c b?ng cách padding kho?ng tr?ng
//						int len = strlen(buf);
//						for (int i = len; i < 10; i++) {  // 10 là s? ký t? t?i da b?n mu?n hi?n th?
//								buf[i] = ' ';
//						}
//						
//						buf[10] = '\0';  // k?t thúc chu?i
//						
//						sprintf (bufss, "demLocNhieu %d",demLocNhieu%100);
//						OLED_ShowStr(0, 0, bufss, 2);
//						OLED_ShowStr(0, 2, buf, 2);
//						
////						OLED_ShowStr (0, 2, "     ", 2);
//        }

//    }
		while (1)
		{
				if (last_pdoa_val != pdoa_val)
				{
						last_pdoa_val = pdoa_val;

						int filtered_val = median_filter(last_pdoa_val);

						char buf[32], bufss[32];
						sprintf(buf, "%d", filtered_val);

						int len = strlen(buf);
						for (int i = len; i < 10; i++) {
								buf[i] = ' ';
						}
						buf[10] = '\0';

						OLED_ShowStr(0, 0, "Filtered:", 2);
						OLED_ShowStr(0, 2, buf, 2);
				}
		}
    return DWT_SUCCESS;
}


/*! ------------------------------------------------------------------------------------------------------------------
 * @fn rx_ok_cb()
 *
 * @brief Callback to process RX good frame events
 *
 * @param  cb_data  callback data
 *
 * @return  none
 */
//static void rx_ok_cb(const dwt_cb_data_t *cb_data)
//{
//    int16_t cpqual;
//    // ¼ì²éSTSÖÊÁ¿. checking STS quality see note 4
//    if(dwt_readstsquality(&cpqual))
//    {
//        /* ¶ÁÈ¡ÏàÎ»²î*/
//        pdoa_val=dwt_readpdoa();
//    }
//    dwt_rxenable(DWT_START_RX_IMMEDIATE);
//}
static void rx_ok_cb(const dwt_cb_data_t *cb_data)
{
    uint8_t rx_buffer[40];
    int16_t cpqual;

    // Ð?c d? li?u frame nh?n du?c
    if (cb_data->datalength > sizeof(rx_buffer)) return; // tránh tràn b? d?m
    dwt_readrxdata(rx_buffer, cb_data->datalength, 0);

    // Ki?m tra ID g?i
    uint8_t expected_id[8] = {'D','E','C','A','W','A','V','E'};
    int id_match = 1;
    for (int i = 0; i < 8; i++) {
        if (rx_buffer[2 + i] != expected_id[i]) {
            id_match = 0;
            break;
        }
    }

    if (id_match) {
        if (dwt_readstsquality(&cpqual)) {
            pdoa_val = dwt_readpdoa(); // ch? d?c n?u ID dúng và ch?t lu?ng t?t
        }
    }

    dwt_rxenable(DWT_START_RX_IMMEDIATE);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn rx_err_cb()
 *
 * @brief Callback to process RX error and timeout events
 *
 * @param  cb_data  callback data
 *
 * @return  none
 */
static void rx_err_cb(const dwt_cb_data_t *cb_data)
{
    dwt_rxenable(DWT_START_RX_IMMEDIATE);
}
#endif

/*****************************************************************************************************************************************************
 * NOTES:
 *
 * 1. Manual reception activation is performed here but DW IC offers several features that can be used to handle more complex scenarios or to
 *    optimise system's overall performance (e.g. timeout after a given time, automatic re-enabling of reception in case of errors, etc.).
 * 2. This is the default configuration recommended for optimum performance. An offset between the clocks of the transmitter and receiver will
 *    occur. The DW3000 can tolerate a difference of +/- 20ppm. For optimum performance an offset of +/- 5ppm is recommended.
 * 3. A natural offset will always occur between any two boards. To combat this offset the transmitter and receiver should be placed
 *    with a real PDOA of 0 degrees. When the PDOA is calculated this will return a non-zero value. This value should be subtracted from all
 *    PDOA values obtained by the receiver in order to obtain a calibrated PDOA.
 * 4. If the STS quality is poor the returned PDoA value will not be accurate and as such will not be recorded
 ****************************************************************************************************************************************************/
