// #include "cmsis_os.h"

// 19h42 18 03 2023

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "LID_HMI.h"
#include "doc_tay_game.h"
#include "config.h"
#include "Tinh_Khoang_Cach.h"
#include "3SwerveWheelsControler.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "DieuKhienCoCau.h"
 //#include "ROBOTRUN.h"
#include "BasicFunction.h"
#include "San_Xanh.h"
#include "San_Do.h"

static void taskGyro(void *pvParameters)
{
	while (1)
	{
		//-------------------------------------------------------------

		//-----------------------------------------------------------------
		HMI_RUN_LOOP(20);
		USART_Cmd(USART3, ENABLE);
		vTaskDelay(15);
	}
}

static void taskDieuKhienCoCau1(void *pvParameters)
{

	while (1)
	{

	if(Ban == 0)	Giu_nang_ha();
	else					nang_ha = 8;
		
	Xoay_Nong_Ban();
	Shoot();
		vTaskDelay(3);
	}
}

static void taskDieuKhienCoCau(void *pvParameters)
{

	while (1)
		
	{	
		if(SHARE)		robotGamePadControl(35, 200);
		else				robotStop(0);

		
		Angle_0h();
		Angle_4h();
		Angle_8h();
		
		vTaskDelay(3);
	}
}
static void taskRobotAnalytics(void *pvParameters)
{
	while (1)
	{
		ADCValue_Control();
		Curent_angle_4_wheel();
		robotAnalytics();

		vTaskDelay(4);
	}
}
static void taskMain(void *pvParameters)
{
	Config_out_mode(); // khai b�o ngo ra cua mot chan bat ky
	Config_in_mode();  // khai bao ngo v�o cua mot chan bat ky
	// Config_pwm_time_t8();//cai dat timer8 o che do dieu xung
	//Config_pwm_time_t4(); // cai dat timer4 o che do dieu xung
	// Config_pwm_time_t9();//cai dat timer9 o che do RC SEVOR
	Config_encoder_timer2_timer3(); // doc encoder  timer 2, timer 3, timer 5 ,timer 9
	Config_encoder_timer1();
	Config_encoder_timer5();
	Config_encoder_timer4();
	// ngat_ngoai();			//chuy�n dung de doc sieu am ket hop timer7, hoac co the dung lam nut nh�n
	Config_ADC1_DMA(); // su dung  khi doc tin hieu laze hay cac t�n hieu ADC<3,3v
	// Config_int_time6();		//phuc vu cho chuong trinh hoat dong song song voi he thong v?i chu ky 1ms
	Config_int_time7();	  // su dung de doc sieu am, tang giam bien, ch�ng nhieu, v� c�c chuong tr�nh hoat dong khong song song voi he thong
	UART1_DMA_RX(115200); // usart giao tiep voi laban
	UART2_DMA_TX(115200); /// DIEU KHIEN DONG CO
	UART3_DMA_RX(115200); // usart giao tiep de doc gamepad
	UART4_DMA_RX(115200);	//SU DUNG DE GIAO TIEP MACH DO LAI
	UART5_DMA_TX(921600); // GIAO TIEP MAN HINH HMI
	// if (SysTick_Config(SystemCoreClock / 1000))while (1);// 1ms truyen du lieu usart den cac slever
	UART6_DMA_RX(115200);
	
	// reset lai laban
	robotResetIMU();
//	
//////////	//---- reset he thong ve vi tri ban dau
//	while(!Home_wheel_RL_Out() | !Home_wheel_RR_Out() | !Home_wheel_FR_Out() | !Home_wheel_FL_Out());
//	while(!Home_wheel_RL() | !Home_wheel_RR() | !Home_wheel_FR() | !Home_wheel_FL());



while(!Home_wheel_0h_Out() | !Home_wheel_4h_Out() | !Home_wheel_8h_Out());
while(!Home_wheel_0h()| !Home_wheel_4h() | !Home_wheel_8h());

//	while( !Home_wheel_4h_Out());
//	while(!Home_wheel_4h());

	Vi_tri = 0;
	RESET_ENCODER(); 

	//-----------------------------------
	xTaskCreate(taskRobotAnalytics, (signed char *)"taskRobotAnalytics", 256, NULL, 0, NULL);
	xTaskCreate(taskDieuKhienCoCau, (signed char *)"taskDieuKhienCoCau", 256, NULL, 0, NULL);
	xTaskCreate(taskDieuKhienCoCau1, (signed char *)"taskDieuKhienCoCau1", 256, NULL, 0, NULL);
	
	state_nang = 0;
	// mam xoay
	target_BT_Xoay ;
	// Ban
	Target_BT_Shoot = 215;
//	if(bientronangluaValue > 650) Nongban = 0;
//	if(bientronangluaValue < 520) Nongban = 0;
//	


	while (1)
	{
		while(SHARE) 
		{	
			//robotGamePadControl(35, 120);
	
//			if(L1) vuotQuaRobot();//duoiTheoRobot();	

		 
		reset();		
		if(CHON_SAN == 0){
		lucbanlazerDo(lazeTruocValue_Ban);
		
		
		}
		else{
		lucbanlazerXanh(lazeTruocValue_Ban,received_offset);
		//lucbanlazerXanh_KX(min_laze_Ban_KX, received_offset);
		

		}	
		
		if	(bientronangbongValue <= 600) 
		{		
				nuot_bong();		
				Xoay_nong();		
				
				if (R2) quy_trinh_do_lazer();
				if(R1 && TRIANGLE )		Ban_tu_dong();
				if(R1 && SQUARE )			Ban_tru_5(); 
				if(R1 && X )					Ban_tru_10(); 
					//	if(R1 && X) 				calculateOverallMean();
				if(R1 && O )					Ban_cong_5();
		}
		else if( bientronangbongValue >= 800) 
		{		
			
				//if(L1 && !R2 && !R1 && !L2) testrun();
				//if (!L1 && R2 && !R1 && !L2) test_lui();
				nuot_bong();
				reset_lazer_data();
		}
		PENALTY();
		Kich_Ban();
		Nang_ha();	
		tru_luc();
		cong_luc();
	}
		robotStop(10);
	}
}


int main(void)
{
	xTaskCreate(taskMain, (signed char *)"taskMain", 256, NULL, 0, NULL);
	xTaskCreate(taskGyro, (signed char *)"taskGyro", 256, NULL, 0, NULL);

	vTaskStartScheduler(); // lenh nay cho phep cac tac vu da nhiem hoat dong.
}
