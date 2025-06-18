

//*****************************8 bien toan cuc *************************************
char xl_truot_lua, xl_kep_lua_tren, xl_kep_lua_duoi, xl_nang_lua, gt_kep_bong;
int vi_tri = 0, Cbcc = 0, Cbnv = 0, Cbnn = 0, vt_xl_nang_lua = 0;
char On_Off_Ban = 0;
int xl_ban_bong = 0;
int Nang_vong_max = 322, Nang_vong_min = 105, i;
int Nang_goc_max = 540, Nang_goc_min = 300;
int touchpad_press = 0;
int laybong = 0;
int en_tru = 60;
int max_speed = 10;
int speed_tay_lua = 100;
int speed_xoay = 200;
//float M_PI = 3.141592653589793238462643;
int xl_kepbong_state = 0;
int xl_nangkep_state = 0;
int ban_bong_state = 0;

//extern vu8 update;
extern speed_temp;

extern float JTL;

int final_force;
float result;
int force_F;
int degree;

int final_F_chuyen;
int force_chuyen;
float result_chuyen;

int Ban = 0;
int state_nang = 1;
int state_ha = 1;
int state_giu = 1;
int free_spin = 0;

int state = 0;
int state_re = 0;
int state_chuyen = 0;
int track = 0;

// tính canh
extern float DC;
extern float goc_bcd;


int lazeTruocValue_Ban;

void robotLineRunLeft(vu8 speed);
void robotLineRunRight(vu8 speed);
//=========================================BAN BONG==============================


//#include <math.h>
//#include <stdio.h>


//float tinh_DC(float BC, float CT) {
//    float EB = 55.0f;
//    float ED = 35.0f;
//    float BN = 310.0f;

//    float DB = sqrtf(EB * EB + ED * ED);
//    float BT = sqrtf(BC * BC + CT * CT);

//    // Tính góc NBT 
//    float ratio_nbt = BN / BT;
//    float goc_nbt = acosf(ratio_nbt) * (180.0f / M_PI);

//    // Tính góc CBT 
//    float ratio_cbt = BC / BT;
//    float goc_cbt = cosf(ratio_cbt) * (180.0f / M_PI);

//    // Tính góc EBD 
//    float ratio_ebd = EB / DB;
//    float goc_ebd = acosf(ratio_ebd) * (180.0f / M_PI);

//    // Tính góc CBD
//    float goc_cbd = 180.0f - (goc_nbt + goc_cbt + goc_ebd);
//    float goc_cbd_rad = goc_cbd * (M_PI / 180.0f);

//    // Tính DC
//    float DC = sqrtf(DB * DB + BC * BC - 2.0f * DB * BC * cosf(goc_cbd_rad));

//    DC = floorf(DC * 100.0f + 0.5f) / 100.0f;

//    return DC;
//}
//#define DEGREES(radian) ((radian) * (180.0f / M_PI))

//float tinh_goc_bcd(float BC, float CT) {
//    float EB = 55.0f;
//    float ED = 35.0f;
//    float BN = 310.0f;

//    float DB = sqrtf(EB * EB + ED * ED);
//    float BT = sqrtf(BC * BC + CT * CT);

//    // Tính góc NBT 
//    float ratio_nbt = BN / BT;
//    float goc_nbt = acosf(ratio_nbt) * (180.0f / M_PI);

//    // Tính góc CBT 
//    float ratio_cbt = BC / BT;
//    float goc_cbt = cosf(ratio_cbt) * (180.0f / M_PI);

//    // Tính góc EBD 
//    float ratio_ebd = EB / DB;
//    float goc_ebd = acosf(ratio_ebd) * (180.0f / M_PI);

//    // Tính góc CBD
//    float goc_cbd = 180.0f - (goc_nbt + goc_cbt + goc_ebd);
//    
//    // Chuy?n góc CBD sang radian
//    float goc_cbd_rad = goc_cbd * (M_PI / 180.0f);

//    // Tính DC (n?u c?n thi?t, nhung không dùng ? dây)
//    float DC = sqrtf(DB * DB + BC * BC - 2.0f * DB * BC * cosf(goc_cbd_rad));

//    // Tính góc BCD theo công th?c dã cho
//    float goc_bcd = DEGREES(asin(sinf(goc_cbd_rad) * DB / DC));

//    return goc_bcd;
//}

////============================= Hàm quét 2 canh=====================================

//#include <math.h>
//#include <stdio.h>

//float tinh_CD(float a, float c, float b, float h) { 
//    float term1;
//    float sqrt_term1;
//    float term2;
//    float denominator;
//    float inner_sqrt;
//    float CD;

//    term1 = 2.0f * c * c + 2.0f * b * b - a * a;
//    term2 = (c * c - b * b) * (c * c - b * b);
//    denominator = a * a * term1;

//    if (term1 <= 0 || denominator == 0.0f) {
//        return -1.0f; // D? li?u không h?p l?
//    }

//    inner_sqrt = 1.0f - (term2 / denominator);
//    if (inner_sqrt < 0.0f) {
//        return -1.0f; // Can b?c hai s? âm
//    }

//    sqrt_term1 = sqrtf(term1);
//    CD = sqrtf(0.25f * term1 + h * h - h * sqrt_term1 * sqrtf(inner_sqrt));

//    return CD;
//}



//=========================================================
 ////hàm ban goc trai
//void ban_auto_1canh(int lazeTruocValue, int lazePhaiValue){
//int BC = lazeTruocValue + 35;
//int CT = lazePhaiValue - 25;
//	

//if (lazeTruocValue > 400 && lazeTruocValue < 510) {
//        BC += 8;}
//else if (lazeTruocValue > 510 && lazeTruocValue < 750) {
//        BC += 24;}

//DC = tinh_DC(BC,CT);
////if (lazePhaiValue < 10) { DC += 5;}

//goc_bcd = tinh_goc_bcd(BC,CT);
//if (lazeTruocValue > 500 && lazeTruocValue < 580) {
//        goc_bcd += 1.2;}
//}

//// hàm ban 2 bên góc
//void ban_auto_1canh(int lazeTruocValue, int lazeTraiValue, int lazePhaiValue){
//int BC = lazeTruocValue + 50;
//int CT_Trai = lazeTraiValue - 25;
//int CT_Phai = lazePhaiValue - 15;
//int CT_o_giua_1 =  CT_Phai - CT_Trai;
//int CT_o_giua_2 = CT_Trai-CT_Phai;
//int lazeduoi = 0;
//	
//// nêu ra giua  ben trai thì giam luc lai
//if (CT_o_giua_1 > 0) {lazeduoi = CT_Trai;}
//else if (CT_o_giua_1 < 235&&CT_o_giua_1 > 10 ) {
//BC -= 5;

//}	
//// ban ben trai
//else if (CT_o_giua_2> 0 ) {
//lazeduoi = CT_Phai;
//BC += 23;
//}
////else {BC -= 5;}

//if (lazeTruocValue > 500 && lazeTruocValue < 527) {
//        BC += 16;}
//else if (lazeTruocValue >= 527 && lazeTruocValue < 750) {
//        BC += 22;}
//DC = tinh_DC(BC,CT_Trai);
//goc_bcd = tinh_goc_bcd(BC,CT_Trai);
//if (lazeTruocValue > 500 && lazeTruocValue < 550) {
//        goc_bcd += 1.5;}
//if (CT_Phai > CT_Trai) {
//    goc_bcd = fabs(goc_bcd);  // góc duong
//} else if (CT_Trai > CT_Phai) {
//    goc_bcd = -fabs(goc_bcd); // góc âm
//}
//}



void Ban_tru_5(void){
						if(CB_NHAN_BONG_BO_BAN == 0)
						{
							lazeTruocValue_Ban = min_laze_Ban -5 ;
							if (lazeTruocValue_Ban < 590) {
							Ban_1 = final_force + 30  ;
							Ban_2 = final_force - 30;
							Ban_3 = final_force -30; // power_shot , testluc
							Ban_1_next, Ban_2_next, Ban_3_next;
							
							}
							else {
							Ban_1 = final_force + 20  ;
							Ban_2 = final_force - 20;
							Ban_3 = final_force - 20; // power_shot , testluc
							Ban_1_next, Ban_2_next, Ban_3_next;
							}
	

						}
	}		
	

void Ban_tru_10(void){
						if(CB_NHAN_BONG_BO_BAN == 0)
				
						{
							lazeTruocValue_Ban = min_laze_Ban - 10 ;
							if (lazeTruocValue_Ban < 590) {
							Ban_1 = final_force + 30  ;
							Ban_2 = final_force - 30;
							Ban_3 = final_force -30; // power_shot , testluc
							Ban_1_next, Ban_2_next, Ban_3_next;
							
							}
							else {
							Ban_1 = final_force + 20  ;
							Ban_2 = final_force - 20 ;
							Ban_3 = final_force - 20 ; // power_shot , testluc
							Ban_1_next, Ban_2_next, Ban_3_next;
							}
	

						}
	}			


void Ban_cong_5(void){
						if(CB_NHAN_BONG_BO_BAN == 0)
						{
							lazeTruocValue_Ban = min_laze_Ban + 5 ;
							if (lazeTruocValue_Ban < 590) {
							Ban_1 = final_force + 30  ;
							Ban_2 = final_force - 30;
							Ban_3 = final_force -30; // power_shot , testluc
							Ban_1_next, Ban_2_next, Ban_3_next;
							
							}
							else {
							Ban_1 = final_force + 20  ;
							Ban_2 = final_force - 20 ;
							Ban_3 = final_force -20; // power_shot , testluc
							Ban_1_next, Ban_2_next, Ban_3_next;
							}
	

						}
	}		
//=========================== KICH BAN =============================
void Kich_Ban(){
	
	if (TOUCHPAD)
	{
//				XI_LANH_DAY_BONG_RA;
//        vTaskDelay(10000); // Wait 1 second
//        Ban_1 = Ban_2 = Ban_3 = 0;
//        Ban = 0;
//				XI_LANH_HA_NONG;
//				vTaskDelay(5000);
//				XI_LANH_DAY_BONG_VAO;
//				LAZER = 0;
//				state = 0;
		
		
		Target_BT_Shoot  = 475;
		vTaskDelay(10000);
		Ban_1 = Ban_2 = Ban_3 = 0 ;
		Target_BT_Shoot = 215;
		LAZER = 0;
		state = 0;


}
}

void reset_lazer_data(void);
//=========================== NANG HA =============================
void Nang_ha(void){
 // len
	if (!R1&&TRIANGLE){
	LAZER = 50;
	target_BT_Xoay = 645;
	while	(abs(bientronangluaValue - target_BT_Xoay) > 3) vTaskDelay(1);
	state_giu = 0,	state_nang = 1, state_ha = 0, target_Nang_Ha = Min_Nang_Ha;

	LAZER = 50;		
}
	else if(bientronangbongValue <= Min_Nang_Ha + 3 &&  target_Nang_Ha == Min_Nang_Ha && state_giu == 0 )    Ban = 1;
	 
	 
	 // xuong
	if (!R1&&X)	
	{ 	
		target_BT_Xoay = 645;	
		// xuông thi reset data lazer quét
		//reset_lazer_data();
		
	 	while	(abs(bientronangluaValue - target_BT_Xoay) > 3) vTaskDelay(1);
		target_Nang_Ha = Max_Nang_Ha ;
		state_giu = 1;
		Ban = 0;
	}
	 
 else if(state_ha == 0&& target_Nang_Ha ==	Max_Nang_Ha  && bientronangbongValue > Max_Nang_Ha - 10 ) 
			{
				state_nang = 0;
				state_ha = 1;
			 }	 
 }
 
 
 


int tru_kc = 0;
void calculator_dis(float lazeTruocValue, int received_offset) {
    int cal_dis = 0;

        cal_dis = (lazeTruocValue - (received_offset));  
    
    tru_kc = cal_dis;
}

//=========================== TINH GOC =============================		
void calculate_angle(){
    float angle_deg;
    float angle_rad = atan2f((float)LJOY_LR - 127.5f, (float)LJOY_UD - 127.5f); 
    angle_deg = angle_rad * (180.0f / M_PI);

    if (angle_deg < 0) {
        angle_deg += 360.0f; 
    }
    if (LJOY_UD == 126 && LJOY_LR == 127) { 
        angle_deg = 20000;
    }

    
    if (angle_deg <= 180.0f) {
        angle_deg = angle_deg * 10; 
    } else {
        angle_deg = (angle_deg - 360.0f) * 10; 
    }

    JTA = angle_deg - 450; 
}

//=========================== NUOT BONG =============================
void nuot_bong(void){				
	if(RJOY){
	 int i;
		Ban_1 =  Ban_2 =  Ban_3 = 10;
		Ban_1_back, Ban_2_back, Ban_3_back;

	  for(i=0;i<100;i++)	while(CB_NHAN_BONG_BO_BAN==1) {if(wantExit())	break; vTaskDelay(1);};
		vTaskDelay(3000);
		LAZER = 50;
		Ban_1 = Ban_2 = Ban_3 = 0;
	}
	}	


//// Khai báo bi?n toàn c?c d? luu tr? th?i gian
//u32 system_tick = 0;

//// C?u hình SysTick d? tang m?i mili giây


//// Hàm d? l?y th?i gian hi?n t?i t? SysTick
//u32 GetSysTick(void)
//{
//    return system_tick;
//}	

//u32 matTrai_time = 0;
//u32 matPhai_time = 0;


//void duoiTheoRobot(void)
//{
//    static int  tocdo = 50;
//    vu8 test = 30;
//    u32 now = GetSysTick();  // S? d?ng GetSysTick d? l?y th?i gian hi?n t?i tính b?ng millisecond

//    u32 duration;

//    // N?u c? 3 c?m bi?n d?u không phát hi?n d?ch
//    if (CB_TRAI == 0 && CB_GIUA == 0 && CB_PHAI == 0)
//    {
//        robotStop(25);
//        matTrai_time = 0;
//        matPhai_time = 0;
////        tocdo = 0;
//				test = 0;
//        return;
//    }

//    // N?u c?m bi?n gi?a phát hi?n d?ch, reset th?i gian và t?c d?
//    if (CB_GIUA)
//    {
//        //tocdo = 10;
//        matTrai_time = 0;
//        matPhai_time = 0;
//        return;
//    }

//    // N?u c?m bi?n trái m?t d?ch và c?m bi?n ph?i v?n còn
//    if (!CB_TRAI && CB_PHAI)
//    {
//        if (matTrai_time == 0) matTrai_time = now;

//        duration = now - matTrai_time;
//        tocdo = 10 + duration / 2;  // Tang t?c d? theo th?i gian m?t tín hi?u

//        if (tocdo > 50) tocdo = 50;

//        robotRun(-900, test);  // Ðu?i bên trái
//    }
//    // N?u c?m bi?n ph?i m?t d?ch và c?m bi?n trái v?n còn
//    else if (!CB_PHAI && CB_TRAI)
//    {
//        if (matPhai_time == 0) matPhai_time = now;

//        duration = now - matPhai_time;
//        tocdo = 10 + duration / 2;

//        if (tocdo > 20) tocdo = 20;

//        robotRun(900, test);  // Ðu?i bên ph?i
//    }
//    else
//    {
//        matTrai_time = 0;
//        matPhai_time = 0;
//    }
//}
////====================================================================================
void QueoCua(int tocdo, int HuongChay, int DoDaiEncoder)
{
	RESET_ENCODER_CHAY(); 
	for(i=0;i<100;i++) while(((HuongChay > 0 && lazePhaiValue > 10) || (HuongChay < 0 && lazeTraiValue > 10)) && ENCODER_XOAY() < DoDaiEncoder) {
		robotRunAngle(HuongChay * 900 - ENCODER_XOAY()*900/DoDaiEncoder, tocdo, 0, 0);		
		if(wantExit()) break;
		vTaskDelay(1);
	};
	
}
void vuotQuaRobot(void)
{
	int k = 3000;
	int KhoangCachDung = 5000;
	int KhoangCachDaTien = 0;
	int HuongChay = 0;
	
	RESET_ENCODER_CHAY(); 
	robotRunAngle(-900, 50, 0, 0);
	for(i=0;i<100;i++) while(lazeTraiValue > 50 && ENCODER_XOAY() < 5000) {
		//if(CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ) break;
		if(wantExit()) break;
		vTaskDelay(1);
	};
	
	QueoCua(50, -1, 5000);
	
	RESET_ENCODER_CHAY(); 
	for(i=0;i<100;i++) while(lazeTraiValue > 50 && ENCODER_XOAY() < 3000) {
		robotRunAngle(-1700, 50, 0, 0);
		//if(CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ) break;
		if(wantExit()) break;
		vTaskDelay(1);
	};

	
//	
//	robotRunAngle(900, 25, 0, 0);
//	for(i=0;i<100;i++) while(lazePhaiValue > 100) {
//		//if(CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ) break;
//		if(wantExit()) break;
//		vTaskDelay(1);
//	};

//	robotStop(0);
//	vTaskDelay(1000);

//	RESET_ENCODER_CHAY(); 
//	robotRunAngle(0, 80, 0, 0);
//	for(i=0;i<100;i++) while(ENCODER_XOAY() < KhoangCachDung) {
//		if(CB_TRAI == 0 || CB_GIUA == 0 || CB_PHAI == 0 ) break;
//		if(wantExit()) break;
//		vTaskDelay(1);
//	};

	robotStop(0);
	vTaskDelay(1000);

}
////====================================================================================
//void vuotQuaRobot_XXX(void)
//{
//	int KhoangCachDung = 5000;
//	int KhoangCachDaTien = 0;
//	
//	while(KhoangCachDaTien < KhoangCachDung){
//		if(wantExit()) break;
//		
//		if(CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ){
//			RESET_ENCODER_CHAY(); 
//			robotRunAngle(0, 60, 0, 0);
//			for(i=0;i<100;i++) while(ENCODER_XOAY() < KhoangCachDung) {
//				if(CB_TRAI == 0 || CB_GIUA == 0 || CB_PHAI == 0 ) break;
//				if(wantExit()) break;
//				vTaskDelay(1);
//			};
//			KhoangCachDaTien = KhoangCachDaTien + ENCODER_XOAY();
//			robotStop(0);
//			vTaskDelay(1000);
//		}
//		
//		if(KhoangCachDaTien > KhoangCachDung) break;
//		
//		if(lazePhaiValue > lazeTraiValue)
//		//if(((CB_TRAI == 0 || CB_GIUA == 0) && CB_PHAI == 1 && lazePhaiValue > 150) || lazePhaiValue > lazeTraiValue)
//		//if(((CB_TRAI == 0 || CB_GIUA == 1) && CB_PHAI == 1 && lazePhaiValue > 100) || (CB_TRAI == 0 && CB_GIUA == 0 && CB_PHAI == 0 && lazePhaiValue > lazeTraiValue))
//		{
//			robotStop(0);	
//			robotRunAngle(900, 80, 0, 0);
//			for(i=0;i<100;i++) while(lazePhaiValue > 70) {
//				if(CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ) break;
//				if(wantExit()) break;
//				vTaskDelay(1);
//			};
//			
//			RESET_ENCODER_CHAY(); 
//			robotRunAngle(900, 80, 0, 0);
//			for(i=0;i<100;i++) while(lazePhaiValue > 70 && ENCODER_XOAY() < 300) {
//				if(wantExit()) break;
//				vTaskDelay(1);
//			};

//			RESET_ENCODER_CHAY(); 
//			robotRunAngle(450, 30, 0, 0);
//			for(i=0;i<100;i++) while(lazePhaiValue > 70 && ENCODER_XOAY() < 700) {
//				if(CB_PHAI == 0 ) break;
//				if(wantExit()) break;
//				vTaskDelay(1);
//			};

////			RESET_ENCODER_CHAY(); 
////			robotRunAngle(450, 60, 0, 0);
////			for(i=0;i<100;i++) while(lazePhaiValue > 70 && ENCODER_XOAY() < 1000) {
////				if(CB_PHAI == 0 ) break;
////				if(wantExit()) break;
////				vTaskDelay(1);
////			};

//			KhoangCachDaTien = KhoangCachDaTien + ENCODER_XOAY()/1.4;		
//		}
//		else{
//			robotStop(0);
//			robotRunAngle(-900, 80, 0, 0);
//			for(i=0;i<100;i++) while(lazeTraiValue > 70) {
//				if(CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ) break;
//				if(wantExit()) break;
//				vTaskDelay(1);
//			};
//			
//			RESET_ENCODER_CHAY(); 
//			robotRunAngle(-900, 80, 0, 0);
//			for(i=0;i<100;i++) while(lazeTraiValue > 70 && ENCODER_XOAY() < 300) {
//				if(wantExit()) break;
//				vTaskDelay(1);
//			};

//			RESET_ENCODER_CHAY(); 
//			robotRunAngle(-450, 30, 0, 0);
//			for(i=0;i<100;i++) while(lazeTraiValue > 70 && ENCODER_XOAY() < 700) {
//				if(CB_PHAI == 0 ) break;
//				if(wantExit()) break;
//				vTaskDelay(1);
//			};
//			
////			RESET_ENCODER_CHAY(); 
////			robotRunAngle(-450, 60, 0, 0);
////			for(i=0;i<100;i++) while(lazeTraiValue > 70 && ENCODER_XOAY() < 1000) {
////				if(CB_PHAI == 0 ) break;
////				if(wantExit()) break;
////				vTaskDelay(1);
////			};

//			KhoangCachDaTien = KhoangCachDaTien + ENCODER_XOAY()/1.4;
//		}
//	}
//	
//	robotStop(0);
//	vTaskDelay(1000);

//}

//=========================== ADC =================================
void ADCValue_Control(void){
	// khai bao lazer
	static unsigned int lazeSauCouter = 0, lazeSau_SUM = 0;
	static unsigned int lazeTruocCouter = 0, lazeTruoc_SUM = 0;
	static unsigned int lazePhaiCouter = 0, lazePhai_SUM = 0;
	static unsigned int lazeTraiCouter = 0, lazeTrai_SUM = 0;

	// Bien tro vi tri co cau
	static unsigned int bientromamxoayCouter = 0, bientromamxoay_SUM = 0;
	static unsigned int bientronangluaCouter = 0, bientronanglua_SUM = 0;
	static unsigned int bientronangbongCouter = 0, bientronangbong_SUM = 0;
	////----------------TINH TOAN LAZER---------------
	if (lazeSauCouter++ < 100)
	{
		lazeSau_SUM += cam_bien_laze_sau;
	}
	else
	{
		lazeSauValue = lazeSau_SUM / 100;
		lazeSauCouter = 0;
		lazeSau_SUM = 0;
	}
	//-------------------------------------------
	if (lazeTruocCouter++ < 100)
	{
		lazeTruoc_SUM += cam_bien_laze_truoc;
	}
	else
	{
		lazeTruocValue = lazeTruoc_SUM / 100;
		lazeTruocCouter = 0;
		lazeTruoc_SUM = 0;
	}
	//------------------------------
	if (lazePhaiCouter++ < 100)
	{
		lazePhai_SUM += cam_bien_laze_phai;
	}
	else
	{
		lazePhaiValue = lazePhai_SUM / 100;
		lazePhaiCouter = 0;
		lazePhai_SUM = 0;
	}
	//-----------------------------------------
	if (lazeTraiCouter++ < 120)
	{
		lazeTrai_SUM += cam_bien_laze_trai;
	}
	else
	{
		lazeTraiValue = lazeTrai_SUM / 120;
		lazeTraiCouter = 0;
		lazeTrai_SUM = 0;
	}
	//-----------------TINH TOAN BIEN TRO---------------

	if (bientromamxoayCouter++ < 10)
	{
		bientromamxoay_SUM += bien_tro_xoay_nong;
	}
	else
	{
		BienTroBanValue = bientromamxoay_SUM / 10;
		bientromamxoayCouter = 0;
		bientromamxoay_SUM = 0;
	}
	//-----------------------------------
	if (bientronangluaCouter++ < 50)
	{
		bientronanglua_SUM += bien_tro_nang_lua;
	}
	else
	{
		bientronangluaValue = bientronanglua_SUM / 50;
		bientronangluaCouter = 0;
		bientronanglua_SUM = 0;
	}
	//-----------------------------
	if (bientronangbongCouter++ < 50)
	{
		bientronangbong_SUM += bien_tro_nang_bong;
	}
	else
	{
		bientronangbongValue = bientronangbong_SUM / 50;
		bientronangbongCouter = 0;
		bientronangbong_SUM = 0;
	}
}


#include <limits.h>

#define MAX_SIZE 20


int lazer_data[MAX_SIZE];
int lazer_data_count = 0;
int lazer_data_save_count = 0;

extern int min_laze_Ban;
extern int full_data;

void add_lazer_data(int lazeTruocValue)
{
    int i;
    int exists = 0;
    int minv;

    // Chi luu giá tri hop le
    if (lazeTruocValue <= 10 || lazeTruocValue > 755) {
        return;
    }

    // Kiem tra trùng lap
    for (i = 0; i < lazer_data_count; i++) {
        if (lazer_data[i] == lazeTruocValue) {
            exists = 1;
            break;
        }
    }

    if (exists) {
        return;
    }

    // xu ly buffer full
    if (lazer_data_count < MAX_SIZE) {
        lazer_data[lazer_data_count++] = lazeTruocValue;
    } else {
        minv = lazer_data[0];
        for (i = 1; i < lazer_data_count; i++) {
            if (lazer_data[i] < minv) {
                minv = lazer_data[i];
            }
        }
        full_data = minv;

        lazer_data[0] = lazeTruocValue;
        lazer_data_count = 1;
    }

    // Tìm giá tri moi trong mang
    minv = lazer_data[0];
    for (i = 1; i < lazer_data_count; i++) {
        if (lazer_data[i] < minv) {
            minv = lazer_data[i];
        }
    }

    min_laze_Ban = minv;
    lazer_data_save_count++;
}


// ham reset data lazer
void reset_lazer_data(void)
{	
	 int i;
		for (i = 0; i < MAX_SIZE; i++) {
        lazer_data[i] = 0;
    }
	
    // Reset các bien d?m
    lazer_data_count = 0;
    lazer_data_save_count = 0;

    // Reset giá tri min và max
    min_laze_Ban = 0;
}






//=========================== NANG HA =============================
void Giu_nang_ha(){
	
	if (target_Nang_Ha < Min_Nang_Ha)
		target_Nang_Ha = Min_Nang_Ha;
	if (target_Nang_Ha > Max_Nang_Ha)
		target_Nang_Ha = Max_Nang_Ha;

	if (abs(bientronangbongValue - target_Nang_Ha) > 3)
	{
		if (bientronangbongValue > target_Nang_Ha)	nang_back;
		else																				nang_next;

		speed_temp = abs(bientronangbongValue - target_Nang_Ha)*5;
		
		if(state_giu==1){
		if (speed_temp > 120)
				speed_temp = 120;
		 else if(bientronangbongValue>= 880) 	 speed_temp = 50;	
		}


		else {
		if (speed_temp > 200)
		speed_temp = 200;
		
		if(bientronangbongValue >= 800 )speed_temp = 80; 
		if(bientronangbongValue >= 750 )speed_temp = 160; 
		
		if(bientronangbongValue <= 450 )speed_temp = 80;
	
		}
		
		nang_ha = speed_temp;
	}
	else nang_ha = 8;
	if(state_nang == 0)  nang_ha = 0;
	}


void Nang_Ha_Nong_Ban(void){
	if (R2 && X && (target_Nang_Ha < Max_Nang_Ha))
		target_Nang_Ha++, vTaskDelay(300);
	else if (R2 && TRIANGLE && (target_Nang_Ha > Min_Nang_Ha))
		target_Nang_Ha--, vTaskDelay(400);
}

//=========================== MAM XOAY =============================
void Xoay_Nong_Ban(){
	if (target_BT_Xoay < Min_BT_Xoay )
		target_BT_Xoay = Min_BT_Xoay;
	if (target_BT_Xoay > Max_BT_Xoay)
		target_BT_Xoay = Max_BT_Xoay;

	if (abs(bientronangluaValue - target_BT_Xoay) > 3)
	{
		if (bientronangluaValue > target_BT_Xoay)  CamRotate_back;
		else                                       CamRotate_next;

		speed_temp = abs(bientronangluaValue - target_BT_Xoay)*2;

		if (speed_temp > 250) // 50
			speed_temp = 250;	
		if (speed_temp > 30) 							
			speed_temp = 30;
		
//		if (speed_temp > 17) 							
//			speed_temp = 17;
		
		CamRotate = speed_temp;
	}
	else
		CamRotate = 2;
//	
	if(bientronangluaValue > 1000) CamRotate = 0;
	if(bientronangluaValue < 50) CamRotate = 0;
}
void Xoay_nong (void){
	int speed;
	if(L1)	speed = 100;
	else		speed = 250;
	if (O&&!R1 && (target_BT_Xoay < Max_BT_Xoay))
		target_BT_Xoay++, vTaskDelay(speed);
	else if (SQUARE &&!R1 && (target_BT_Xoay > Min_BT_Xoay))
		target_BT_Xoay --, vTaskDelay(speed);
}

//=== road to ABU==


 void Shoot()
 {
     if (Target_BT_Shoot < Min_BT_Shoot)
         Target_BT_Shoot = Min_BT_Shoot;
     if (Target_BT_Shoot > Max_BT_Shoot)
         Target_BT_Shoot = Max_BT_Shoot;

     if (abs(BienTroBanValue - Target_BT_Shoot) > 4)
     {
         if (BienTroBanValue > Target_BT_Shoot)  	Mor_Shoot_next;
         else                                     Mor_Shoot_back;

         speed_temp = abs(BienTroBanValue - Target_BT_Shoot)*1.2;
			 
         if (speed_temp > 250)                             
             speed_temp = 250;
        
         Mor_Shoot = speed_temp;
				
     }
     else
         Mor_Shoot = 0;

		

//     if(BienTroBanValue > 620) Mor_Shoot = 0;
//     if(BienTroBanValue < 250) Mor_Shoot = 0;

 }


void Xoay_Theo_Goc(float goc_muon_xoay, int bientronangluaValue) {
    const int BT_GOC_0 = 550;  
    const int BT_PER_DEGREE = 15;
		int goc_da_xoay = bientronangluaValue;
		int vi_tri_lay_goc = goc_da_xoay - BT_GOC_0;

    int target = ((BT_GOC_0 + (int)(goc_muon_xoay * BT_PER_DEGREE))+ (vi_tri_lay_goc-10));

    // Gi?i h?n target trong kho?ng cho phép
    if (target > Max_BT_Xoay) target = Max_BT_Xoay;
    if (target < Min_BT_Xoay) target = Min_BT_Xoay;

    target_BT_Xoay = target;  // c?p nh?t bi?n toàn c?c m?c tiêu
	
	
			// vua xoay vua quet khoang cách
			 add_lazer_data(lazeTruocValue);
				 //addElement(40, lazeTruocValue);

}

void Ban_bong(void) {
	
			if(CB_NHAN_BONG_BO_BAN == 0)
			{
				lazeTruocValue_Ban = DC;

				Ban_1 = Ban_2 = Ban_3 = final_force; // power_shot , testluc
				Ban_1_next, Ban_2_next, Ban_3_next;
				
				
			}	
	}
// ham tu dong ban bang 2 lazer
void tudonban(void) {

Ban_bong();

Xoay_Theo_Goc(goc_bcd, bientronangluaValue);
}

//void quy_trinh_do_lazer() {

//	int i ;
//	for (i = 0; i < 1000; i++) {
//       add_lazer_data(lazeSauValue);
//    }
//    // xoay theo goc va lay gia tri

//    Xoay_Theo_Goc(1, bientronangluaValue); 
//			}


void quy_trinh_do_lazer(void) {
    Xoay_Theo_Goc(1, bientronangluaValue); 
	
}
//=========================== RESET BIEN =============================
void reset(void){
 if(OPTIONS) {
    		
    Ban_1 = Ban_2 = Ban_3 = 0;
		track = 0;
	 	LAZER = 0;
	 state_chuyen = 0;
	 state_re = 0;
	 state = 0;
	 reset_lazer_data();
}
 }

 extern int min_laze_Ban_KX;
 
// ============================ HAM TEST RUN==================================


void Ban_tu_dong(void){
						if(CB_NHAN_BONG_BO_BAN == 0)
						{
							lazeTruocValue_Ban = min_laze_Ban ;
							if (lazeTruocValue_Ban < 590) {
							Ban_1 = final_force + 30  ;
							Ban_2 = final_force - 30;
							Ban_3 = final_force -30; // power_shot , testluc
							Ban_1_next, Ban_2_next, Ban_3_next;
							
							}
							else {
							Ban_1 = final_force + 20  ;
							Ban_2 = final_force - 20 ;
							Ban_3 = final_force - 20 ; // power_shot , testluc
							Ban_1_next, Ban_2_next, Ban_3_next;
							}
	

						}
	}		

	


void nuot_bong_tu_dong(void){				

	 int i;
		Ban_1 =  Ban_2 =  Ban_3 = 10;
		Ban_1_back, Ban_2_back, Ban_3_back;

	  for(i=0;i<100;i++)	while(CB_NHAN_BONG_BO_BAN==1) {if(wantExit())	break; vTaskDelay(1);};
		vTaskDelay(3000);
		LAZER = 50;
		Ban_1 = Ban_2 = Ban_3 = 0;
		
		
		
	}

void Nang_co_dinh(void){
 // len

	LAZER = 50;
	target_BT_Xoay = 549;
	while	(abs(bientronangluaValue - target_BT_Xoay) > 2) vTaskDelay(1);
	state_giu = 0,	state_nang = 1, state_ha = 0, target_Nang_Ha = Min_Nang_Ha  ;

	LAZER = 50;		
}


//void testrun(void) {
//    int KhoangCachDung = 4800;
//    int KhoangCachDaTien = 0;

//    int i; 
//    RESET_ENCODER_CHAY(); 
//		
//    robotRunAngle(1800, 40, 0, 0); 

//    	for(i=0;i<100;i++) while(ENCODER_XOAY() < KhoangCachDung) {
//				if(wantExit()) break;
//				vTaskDelay(1);
//			};
//			

//    KhoangCachDaTien = ENCODER_XOAY(); 
//		if (KhoangCachDaTien >= KhoangCachDung ) {
//		robotStop(0);                      
//    vTaskDelay(1000);		

//		}     
//}



void testrun(void) {
    RESET_ENCODER_CHAY(); 
    robotRun(1800, 50); 

    	for(i=0;i<100;i++) while(ENCODER_XOAY() < 3200) {
				if(wantExit()) break;
				vTaskDelay(1);
			};
			
	  RESET_ENCODER_CHAY(); 
    robotRun(1800, 35); 

    	for(i=0;i<100;i++) while(ENCODER_XOAY() < 1000) {
				if(wantExit()) break;
				vTaskDelay(1);
			};		
		robotStop(0);                      
    vTaskDelay(1000);	
		Nang_co_dinh();
			

		}     


void test_lui(void) {
    int KhoangCachDung = 4800;
    int KhoangCachDaTien = 0;

    int i; 
    RESET_ENCODER_CHAY(); 
		
    robotRunAngle(0, 35, 0, 0); 

    	for(i=0;i<100;i++) while(ENCODER_XOAY() < KhoangCachDung) {
				if(wantExit()) break;
				vTaskDelay(1);
			};

    KhoangCachDaTien = ENCODER_XOAY(); 
		if (KhoangCachDaTien >= KhoangCachDung ) {
		robotStop(0);                      
    vTaskDelay(1000);		

		}     
}

/////================= Ham ban pen +++++++
void PENALTY(){
	 int LUC = 211;
	if(LJOY){
	 int i;
		Ban_1 =  Ban_2 =  Ban_3 = 15;
		Ban_1_back, Ban_2_back, Ban_3_back;

	  for(i=0;i<100;i++)	while(CB_NHAN_BONG_BO_BAN==1) {if(wantExit())	break; vTaskDelay(1);};
		LAZER = 50;
    vTaskDelay(3000);	
		Ban_1 = LUC +30;
		Ban_2 = LUC -30;
		Ban_3 = LUC -30; 
		Ban_1_next, Ban_2_next, Ban_3_next;
	}

}

//=========== Ham quet tu dong====================



void xoay_lay_khoang_cach(){
	int x, y;
	double goc_ACB, goc_DCB,DB;
	float he_so_dcb;
	int taget_bien_tro_giua_ro;
	initStaticArray();
	if(BienTroNongBanValue > 800) return;
	target_BT_Xoay = Min_BT_Xoay;
	custom_xoay = 1;
	custom_speed_xoay = 40;
//	Ban_1 = Ban_2 = Ban_3 = 13;
//	Ban_1_back, Ban_2_back, Ban_3_back;
	while (abs(bientronangluaValue - target_BT_Xoay) > 4) {
		vTaskDelay(1);
	}
	
	target_BT_Xoay = Max_BT_Xoay;
	custom_speed_xoay = 50;
	while (abs(bientronangluaValue - target_BT_Xoay) > 4) {
		vTaskDelay(1);
		if(lazeTruocValue > 150 && lazeTruocValue < 641) {
			addElement(bientronangluaValue - Min_BT_Xoay + 5,lazeTruocValue);			
		}
	}
	calculateOverallMean();
	
//	if (calculate_triangle(canh_trai, canh_phai, home_BT_xoay, &distanceRobotToBackboard, &target_BT_Xoay) != 0) {
//			//xu ly
//	}	
	
	//tính khoang cách tu robot den bang 
	distanceRobotToBackboard = calculateDistance(canh_trai,canh_phai);
	
	//tim bien tro tam ro
	target_BT_Xoay = calculateBTForRimCenter(Min_BT_Xoay);
	
	
	custom_xoay = 1;
	custom_speed_xoay = 50;
	while (abs(bientronangluaValue - target_BT_Xoay) > 4) {
		vTaskDelay(1);
	}
	custom_xoay = 0;
}
extern int cong_tru_luc ;

void tru_luc(void) {
if (L2 && SQUARE ) {
cong_tru_luc -=1;
while(L2 && SQUARE ){
vTaskDelay(1);}
}

}

void cong_luc(void) {
	if (L2 && O ) {
	cong_tru_luc +=1;
	while(L2 && O ){
vTaskDelay(1);}
}
	}