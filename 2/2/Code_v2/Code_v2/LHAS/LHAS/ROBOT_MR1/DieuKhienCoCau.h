//*****************************8 bien toan cuc *************************************
char xl_truot_lua, xl_kep_lua_tren, xl_kep_lua_duoi, xl_nang_lua,gt_kep_bong;
int vi_tri = 0, Cbcc = 0, Cbnv = 0, Cbnn = 0,vt_xl_nang_lua = 0;
char	On_Off_Ban = 0;
int xl_ban_bong = 0;
int Nang_vong_max = 322, Nang_vong_min = 105, i;
int Nang_goc_max = 540, Nang_goc_min = 300;
int touchpad_press =0;
int laybong =0;
int en_tru = 60;
int max_speed = 10;
int speed_tay_lua = 100;
int speed_xoay = 200;

int xl_kepbong_state = 0;
int xl_nangkep_state = 0;
int ban_bong_state=0 ;

//extern vu8 update;
extern speed_temp;
extern vu8	Cam;
extern float	JTL;
int state_nang = 1;
int state_ha = 1;


int force_F;
int force_F2;
int final_force;
int final_force2;
float result;
float result2;

void robotLineRunLeft(vu8 speed);
void robotLineRunRight(vu8 speed);
//=========================================BAN BONG==============================

void Ban_bong(void){
//ban bong
if (L2&&TOUCHPAD) {
            Ban_1 = Ban_2 = Ban_3 = final_force;
						Ban_1_next, Ban_2_next, Ban_3_next;
						vTaskDelay(15000); // Wait for 1.5 seconds
						Cylinder_L = 250; // Activate the cylinder
            vTaskDelay(20000);
            Ban_1=  Ban_2 = Ban_3 = 0;
            Cylinder_L = 0;
						XI_LANH_HA_NONG;
}	
if (!L2&&TOUCHPAD) {
            Ban_1 = Ban_2 = Ban_3 = final_force2;
						Ban_1_next, Ban_2_next, Ban_3_next;
						vTaskDelay(15000); // Wait for 1.5 seconds
						Cylinder_L = 250; // Activate the cylinder
            vTaskDelay(20000);
            Ban_1=  Ban_2 = Ban_3 = 0;
            Cylinder_L = 0;
						XI_LANH_HA_NONG;
}	

			
				
	if(RJOY){
		Ban_1 =  Ban_2 =  Ban_3 = 40;
		Ban_1_back, Ban_2_back, Ban_3_back;
		vTaskDelay(20000);
		Ban_1 = 0, Ban_2 = 0, Ban_3 = 0;			
		
	}
}
void test(void)
{
if(LJOY)    Cylinder_L = 250;
else Cylinder_L = 0;
}

// *****************************88 nang ha bo ban *********************************
// void Nang_ha_bo_ban(vu8 speed)
// {
// 	if(TRIANGLE)  Cylinder_next , Cylinder = speed;
// 	else if(X)		 Cylinder_back , Cylinder = speed;
// 	else Cylinder = 2;
// }

#include <math.h>
#define OFFSET_TOLERANCE 3
#define CENTER 100
#define MAX_SPEED 0.16
#define MIN_SPEED 0.075 

float calculateSpeed(int received_offset) {
    // 1. Khai báo biến trước!
    float distanceFromCenter;
    const float slope = 0.057;                 // Độ dốc cho tanh
    float speedRatio;
    float speed;

    // 2. Tính khoảng cách tuyệt đối
    distanceFromCenter = fabsf((float)(received_offset - CENTER));

    // 3. Kiểm tra vùng chết → dừng
    if (distanceFromCenter <= OFFSET_TOLERANCE) {
        return 0.;
    }

    // 4. Tính tỷ lệ tốc độ
    speedRatio = tanhf(slope * distanceFromCenter);  // Kết quả từ 0 đến 1

    // 5. Tính toán tốc độ dựa trên MIN/MAX
		if (distanceFromCenter < 14) {
        speed = MIN_SPEED + (MAX_SPEED - MIN_SPEED) * 0.001;
    }
    else speed = MIN_SPEED + (MAX_SPEED - MIN_SPEED) * speedRatio;

    // 6. Giới hạn tốc độ tối đa (cẩn thận)
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    }

    return speed;
}

void xoaytam(int received_offset) {
    float received_speed = calculateSpeed(received_offset);

    if (received_offset >= 98 && received_offset <= 102) {
        		robotStop(0);
    } else if (received_offset < CENTER - OFFSET_TOLERANCE) {
        robotRotateFree(-received_speed, 0);  // Xoay phải (âm)
    } else if (received_offset > CENTER + OFFSET_TOLERANCE) {
        robotRotateFree(received_speed, 0);   // Xoay trái (dương)
    }
}


//#######################################################################
void autorun(int rx_position) {
    switch(rx_position) {
        case 5:
            // Đi thẳng
            robotRun(-450, 50);
            break;

        case 2:
            // Rẽ trái
            robotRun(-1350, 50);
            break;

        case 3:
            // Rẽ phải
            robotRun(450, 50);
            break;

        case 4:
            // Dừng robot
            robotStop(0);
            break;
    }
}


#define MOCCHUAN 4090  
#define LUC_MOC 106
#define GOC_MOC 600

#define LUC_MAX1 250
#define LUC_MIN1 50

#define GOC_MAX1 600    
#define GOC_MIN1 550    

#define LUC_BETA 1.1  // H? s? phi tuy?n cho luc ban
#define GOC_ALPHA 0.7

int calculated_force;
int calculated_distance;
float result_Cam;

void tinhlucban_depthcam(float received_distance) {
    float gocBanCam = 0;
    float lucBanCam = 0;
    // T�nh l?c b?n v?i c�ng th?c phi tuy?n
    lucBanCam = LUC_MOC * pow((received_distance / MOCCHUAN), LUC_BETA);

    // Gi?i h?n l?c b?n trong kho?ng cho ph�p
    if (lucBanCam > LUC_MAX1) {
        lucBanCam = LUC_MAX1;
    } else if (lucBanCam < LUC_MIN1) {
        lucBanCam = LUC_MIN1;
    }
    // T�nh g�c b?n v?i h? s? hi?u ch?nh v� kho?ng d?ng
    lucBanCam = GOC_MOC + ((received_distance - MOCCHUAN) / MOCCHUAN) * (GOC_MAX1 - GOC_MIN1) * GOC_ALPHA;

    // Gi?i h?n g�c b?n trong kho?ng cho ph�p
    if (lucBanCam > GOC_MAX1) {
        lucBanCam = GOC_MAX1;
    } else if (lucBanCam < GOC_MIN1) {
        lucBanCam = GOC_MIN1;
    }
    calculated_force = lucBanCam;
    calculated_distance = lucBanCam;
}



#define MOCCU 2500       
#define LUC_TAI_MOC 140


#define LUC_MAX 250
#define LUC_MIN 50

 void lucbanlazer(float received_distance) {
    float gocBan = 0;
    float lucBan = 0;
	  float corrected_distance = sqrt((received_distance * received_distance));
    lucBan = LUC_TAI_MOC * (corrected_distance / MOCCU);

    if (lucBan > LUC_MAX) {
        lucBan = LUC_MAX;
    } else if (lucBan < LUC_MIN) {
        lucBan = LUC_MIN;
    } 
		
    force_F = lucBan;
		final_force = force_F;
	
		
	if (force_F >= 0 && force_F <= 99) {
			result = 12;}
	if (force_F >= 100 && force_F <= 112) {
			result = 11.4;}
	
	if (force_F >= 113 && force_F <= 124) {
			result = 11;}
	if (force_F >= 125 && force_F <= 134) {
			result = 10.5 ;}
	if (force_F >= 135 && force_F <= 148) {
    result = 10.3;}
	if (force_F >= 149 && force_F <= 154) {
    result = 9.8;}
	
	if (force_F >= 155 && force_F <= 170) {
			result = 9.75;}
	if (force_F >= 171 && force_F <= 180) {
			result = 9.65;}
	
	if (force_F >= 181 && force_F <= 199 ){
			result = 9.4;}
	if (force_F >= 200 && force_F <= 250) {
			result = 9.4;}
		

		final_force = force_F * (result)/10 ;
		
	}

	
	
#define MOCCU2 210       
#define LUC_TAI_MOC2 110


#define LUC_MAX2 250
#define LUC_MIN2 50

 void lucbanlazer2(float lazeTraiValue) {
    float gocBan = 0;
    float lucBan = 0;
	  float corrected_distance = sqrt((lazeTraiValue * lazeTraiValue));
    lucBan = LUC_TAI_MOC2 * (corrected_distance / MOCCU2);

    if (lucBan > LUC_MAX2) {
        lucBan = LUC_MAX2;
    } else if (lucBan < LUC_MIN2) {
        lucBan = LUC_MIN2;
    }
		
    force_F2 = lucBan;	
		
		final_force2 = force_F2;
		
		
		
		
if (force_F2 >= 60 && force_F2 <= 350) {
//    result2 = 11.2 - ((force_F2 - 60) * 1.2);
	    result2 = 9.9;
}


		
		final_force2= force_F2 * (result2)/10 ;
		
	}

//**********************************************************************








//void banxoay(void)
//{	int tocdo;
//	if(L2) tocdo = 20;
//	else	tocdo = 7;
//	
//	 if (SQUARE)													  Mor_CamRotate = tocdo, Mor_CamRotate_next;
//	 else if (O) 	                          Mor_CamRotate = tocdo, Mor_CamRotate_back;
//	 else	                                        Mor_CamRotate = 2;
//}


//void nangcam(void)
// {

//	if (!R1&&TRIANGLE)													state_nang = 1, state_ha = 0, target_Nang_Ha = 165;
//	if (!R1&&X)
//	{ 					 
//				target_Nang_Ha = 610 ;
//	 }
//	 else if(state_ha == 0&& target_Nang_Ha == 610 && BienTroNongBanValue > 595) 
//			{
//				state_nang = 0;
//				state_ha = 1;
//			 }	 
// }

//void Nang_Ha_Nong_Ban(void)
//{
//	if (R1 && X && (taget_BT_Xoay < Max_BT_Xoay))
//		taget_BT_Xoay++, vTaskDelay(300);
//	else if (R1 && TRIANGLE && (taget_BT_Xoay > Min_BT_Xoay))
//		taget_BT_Xoay--, vTaskDelay(400);
//}


//====================================================================================


// char CB_line_trai, CB_line_phai, Nho_trai_truoc, Nho_trai_sau, Nho_phai_truoc, Nho_phai_truoc;
//void robotGamePadControl(vu8 speedMin, vu8 speedMax)
//{
//	int tocdo = 20, td_x = 5;
//	float tocdoXoay = 0;
//	int rJoy_LR = (RJOY_LR >= 5 && RJOY_LR <= 250) ? 1 : 0;
//	int lJoy_LR = (LJOY_LR >= 5 && LJOY_LR <= 250) ? 1 : 0;

//	if (UP && DOWN && RIGHT && LEFT && rJoy_LR)
//	{				  // Khong dieu khien
//		robotStop(0); // truot banh

//		return;
//	}
//	/*_____________________________//___________________________*/

//	/********************** TOC DO ****************************/
//	/*_____________________________//___________________________*/
//	if (!L1)
//		tocdo = speedMax, tocdoXoay = 0.5;
//	else
//		tocdo = speedMin, tocdoXoay = 0.3;

//	//--------- chay cac huong -----------
//	if (!UP && DOWN && RIGHT && LEFT && L2)
//		robotRun(0, tocdo);
//	else if (UP && !DOWN && RIGHT && LEFT && L2)
//		robotRun(1800, tocdo);
//	else if (UP && DOWN && !RIGHT && LEFT && L2)
//		robotRun(900, tocdo);
//	else if (UP && DOWN && RIGHT && !LEFT && L2)
//		robotRun(-900, tocdo);

//	else if (!UP && DOWN && !RIGHT && LEFT && L2)
//		robotRun(450, tocdo);
//	else if (!UP && DOWN && RIGHT && !LEFT && L2)
//		robotRun(-450, tocdo);
//	else if (UP && !DOWN && !RIGHT && LEFT && L2)
//		robotRun(1350, tocdo);
//	else if (UP && !DOWN && RIGHT && !LEFT && L2)
//		robotRun(-1350, tocdo);

//	//-------------- Dang chay va Khong chay nua, chi xoay ----------------
//	if (UP && DOWN && RIGHT && LEFT && !rJoy_LR && lJoy_LR && robotIsRun())
//		robotStop(0);

//	//-------------- Xoay ----------------
//	if (rJoy_LR)
//		robotRotateStop();
//	else if (RJOY_LR < 5) 
//	{
//		if (robotIsRun())
//			robotRotateFree(tocdoXoay, 0);
//		else
//			robotRotateFree(tocdoXoay, 0);
//	}
//	else if (RJOY_LR > 250)
//	{
//		if (robotIsRun())
//			robotRotateFree(-tocdoXoay, 0);
//		else
//			robotRotateFree(-tocdoXoay, 0);
//		//			robotLineRunLeft(20);//
//	}

//	else
//		robotRotateStop();
//}

//////-------------------------------------------------------------
void ADCValue_Control(void)
{
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
		bientromamxoay_SUM += bien_tro_nong_ban;
	}
	else
	{
		BienTroNongBanValue = bientromamxoay_SUM / 10;
		bientromamxoayCouter = 0;
		bientromamxoay_SUM = 0;
	}
	//-----------------------------------
	if (bientronangluaCouter++ < 50)
	{
		bientronanglua_SUM += bien_tro_xoay_nong;
	}
	else
	{
		BienTroXoayValue = bientronanglua_SUM / 50;
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


 
 

//	if (taget_BT_Nong_Ban < Min_BT_Nong_Ban)
//		taget_BT_Nong_Ban = Min_BT_Nong_Ban;
//	if (taget_BT_Nong_Ban > Max_BT_Nong_Ban)
//		taget_BT_Nong_Ban = Max_BT_Nong_Ban;

//	if (abs(BienTroNongBanValue - taget_BT_Nong_Ban) > 1)
//	{
//		if (BienTroNongBanValue > taget_BT_Nong_Ban)
//			Cylinder_back;
//		else
//			Cylinder_next;

//		speed_temp = abs(BienTroNongBanValue - taget_BT_Nong_Ban)*10;
//		if (speed_temp > 250) // 50
//			speed_temp = 250;
//		if (speed_temp < 50) 
//			speed_temp = 50;

//		Cylinder = speed_temp;
//	}
//	else
//		Cylinder = 0;
//}

//void Xoay_Nong_Ban()
//{
//	if (target_BT_Xoay < Min_BT_Xoay )
//		target_BT_Xoay = Min_BT_Xoay;
//	if (target_BT_Xoay > Max_BT_Xoay)
//		target_BT_Xoay = Max_BT_Xoay;

//	if (abs(BienTroXoayValue - target_BT_Xoay) > 2)
//	{
//		if (BienTroXoayValue > target_BT_Xoay)
//			Mor_CamRotate_back;
//		else
//			Mor_CamRotate_next;

//		speed_temp = abs(BienTroXoayValue - target_BT_Xoay);
//		if (speed_temp > 70) // 50
//			speed_temp = 70;
//		if (speed_temp < 8) 							
//			speed_temp = 8;
//		Mor_CamRotate = speed_temp;
//	}
//	else
//		Mor_CamRotate = 0;
//}

//void Nang_Ha_Nong_Ban_abs(void)
//{
//	if (R1 && X && (taget_BT_Nong_Ban < Max_BT_Nong_Ban))
//		taget_BT_Nong_Ban++, vTaskDelay(300);
//	else if (R1 && TRIANGLE && (taget_BT_Nong_Ban > Min_BT_Nong_Ban))
//		taget_BT_Nong_Ban--, vTaskDelay(300);
//}

////***************************** xoay nong ban bang tay **************************************
////void Xoay_nong_ban(vu8 speed)
////{
////	if(R1 && SQUARE)					Mor_CamRotate = speed, Mor_CamRotate_next;
////	else	if(R1 && O)					Mor_CamRotate = speed, Mor_CamRotate_back;
////	else											Mor_CamRotate = 2;
////}
//void Xoay_nong (void)
//{
//	if (R1 && O && (target_BT_Xoay < Max_BT_Xoay))
//		target_BT_Xoay++, vTaskDelay(500);
//	else if (R1 && SQUARE && (target_BT_Xoay > Min_BT_Xoay))
//		target_BT_Xoay--, vTaskDelay(500);
//}


//void Giu_nang_ha()
//{
////		if(BienTroNongBanValue < taget_BT_Xoay - 5 ){
////			Cylinder = 0;
////			return;
////		}
//	
//	if (target_Nang_Ha < Min_Nang_Ha)
//		target_Nang_Ha = Min_Nang_Ha;
//	if (target_Nang_Ha > Max_Nang_Ha)
//		target_Nang_Ha = Max_Nang_Ha;

//	if (abs(BienTroNongBanValue - target_Nang_Ha) > 5)
//	{
//		if (BienTroNongBanValue > target_Nang_Ha)	nang_next;
//		else																			nang_back;

//		speed_temp = abs(BienTroNongBanValue - target_Nang_Ha)*2;
//		if (speed_temp > 250)
//				speed_temp = 250;
//		if (speed_temp < 30)
//				speed_temp = 30;
//	
//		nang_ha = speed_temp;
//	}
//	else nang_ha = 2;
//	if(state_nang == 0)  nang_ha = 0;
//	}



//void Nang_Ha_Nong_Ban(void)
//{
//	if (R1 && X && (target_Nang_Ha < Max_Nang_Ha))
//		target_Nang_Ha++, vTaskDelay(300);
//	else if (R1 && TRIANGLE && (target_Nang_Ha > Min_Nang_Ha))
//		target_Nang_Ha--, vTaskDelay(400);
//}
