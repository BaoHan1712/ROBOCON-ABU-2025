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
float M_PI = 3.141592653589793238462643;
int xl_kepbong_state = 0;
int xl_nangkep_state = 0;
int ban_bong_state=0 ;

//extern vu8 update;
extern speed_temp;
extern vu8	Cam;
extern float	JTL;

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
int track = 0 ;

extern int test;
void robotLineRunLeft(vu8 speed);
void robotLineRunRight(vu8 speed);
//=========================================BAN BONG==============================

void Ban_bong(void){
//ban bong
if (TOUCHPAD) {
    vTaskDelay(50);  // Chống dội nút
    if (TOUCHPAD) {
        state++;
        if (state > 2) state = 0;  // Reset về 0 khi vượt quá 2
        while (TOUCHPAD);  // Chờ nút nhả ra mới cho phép nhấn tiếp
    }
}

// Lần nhấn 1: Cập nhật taget_BT_Nong_Ban
if (state == 1) {
		taget_BT_Nong_Ban = 520;
}

// Lần nhấn 2: Chạy quy trình bắn
else if (state == 2) {
    CamRotate = 2;
    Ban = 1;
    Ban_1 = Ban_2 = Ban_3 = final_force;
		Ban_1_next, Ban_2_next, Ban_3_next;

    vTaskDelay(15000); // Wait for 1.5 seconds
			Cylinder_ban_ON;
    Cylinder_ban = 250; // Activate the cylinder

    vTaskDelay(10000); // Wait 1 second
    Ban_1 = Ban_2 = Ban_3 = 0;
    Ban = 0;
    Cylinder_ban = 0;

    state = 0;  // Reset về trạng thái ban đầu
}


				
						
	}

void Chuyen_Bong(void)
{
if (L1&&!TOUCHPAD) {
						CamRotate = 2;
						taget_BT_Nong_Ban = 520;
	          Ban = 1;
            Ban_1 = Ban_2 = Ban_3 = final_F_chuyen;
						Ban_1_next, Ban_2_next, Ban_3_next;
						vTaskDelay(15000); // Wait for 1.5 seconds
						Cylinder_ban_ON;
						Cylinder_ban = 250; // Activate the cylinder
            vTaskDelay(10000);
            Ban_1=  Ban_2 = Ban_3 = 0;
						Ban = 0 ;
            Cylinder_ban = 0;

        }	



}

// *****************************88 nang ha bo ban *********************************


//**********************************************************************
void banxoay(vu8 speed)
{	
	if (SQUARE)							  CamRotate = speed, CamRotate_back;
	else if (O) 	            CamRotate = speed, CamRotate_next;
	else 											CamRotate = 2;

}


void nangnong(void)
 {
 // len
	if (!R2&&TRIANGLE)												state_giu = 0,	state_nang = 1, state_ha = 0, target_Nang_Ha = 940;//165
	else if(bientronangbongValue >= 936 &&  target_Nang_Ha == 940 && state_giu == 0 )    Ban = 1;
	 
	 
	 // xuong
	if (!R2&&X)	
	{ 	  
	 
				target_Nang_Ha = 50 ;
		//out state_giu 
				state_giu = 1;
				Ban = 0;
	 }
 else if(state_ha == 0&& target_Nang_Ha ==50  && bientronangbongValue < 60) 
			{
				state_nang = 0;
				state_ha = 1;
			 }	 
 }
 
void Re_bong(void)
{
						taget_BT_Nong_Ban = 577;
						while(bientronangluaValue >= 580 && bientronangluaValue <= 591) 		{if(wantExit())	break; vTaskDelay(1);}
						vTaskDelay(3000);
						CamRotate = 2;
            Ban_1 =190, Ban_2 =10 , Ban_3 =190;
						Ban_1_next, Ban_2_next, Ban_3_next;
            vTaskDelay(15000);
						Cylinder_ban_ON;
						Cylinder_ban = 250;
	          vTaskDelay(5000);
	          Ban_1=  Ban_2 = Ban_3 = 0;
						Cylinder_ban = 0;
						vTaskDelay(5000);
						Cylinder_ban_OFF;
						Cylinder_ban = 250;
						Ban_1 =  Ban_2 =  Ban_3 = 15;
						Ban_1_back, Ban_2_back, Ban_3_back;
						for(i=0;i<100;i++)	while(CB_NHAN_BONG_BO_BAN==1) {if(wantExit())	break; vTaskDelay(1);};
						vTaskDelay(3000);
						Ban_1 = Ban_2 = Ban_3 = 0;
						taget_BT_Nong_Ban = 520;
						target_BT_Xoay = 507;
						
				
				}

 

 
 
/////////// lazer******************88

#define MOCCU 401       
#define LUC_TAI_MOC 117
#define LUC_MAX 250
#define LUC_MIN 50


#define SMOOTHING_FACTOR 0.1  

float filtered_lazeValue = 0;

void lucbanlazer(float lazeTruocValue) {
    float gocBan = 0;
    float lucBan = 0;
    float corrected_distance;  // Khai b�o bi?n ? d�y tru?c c�c c�u l?nh th?c thi

    filtered_lazeValue = (SMOOTHING_FACTOR * lazeTruocValue) + ((1 - SMOOTHING_FACTOR) * filtered_lazeValue);

    corrected_distance = sqrt((filtered_lazeValue * filtered_lazeValue));  // S? d?ng bi?n sau khi khai b�o
    lucBan = LUC_TAI_MOC * (corrected_distance / MOCCU);

    if (lucBan > LUC_MAX) {
        lucBan = LUC_MAX;
    } else if (lucBan < LUC_MIN) {
        lucBan = LUC_MIN;
    }

    force_F = lucBan;    
    final_force = force_F;
	

			 if (force_F == 49) result = 10; 
			else if (force_F == 50) result = 10; 
			else if (force_F == 51) result = 10; 
			else if (force_F == 52) result = 10; 
			else if (force_F == 51) result = 10; 
			else if (force_F == 52) result = 10; 
			 if (force_F >= 85 && force_F <= 89) { 
        result = 9.90; // Gi? nguy�n 10
    }  
				if (force_F >= 90 && force_F <= 94) { 
        result = 9.8; // Gi? nguy�n 10
    }  
				if (force_F >= 95 && force_F <= 97) { 
        result = 9.75; // Gi? nguy�n 10
    }  
				if (force_F >= 98 && force_F <= 103) { 
        result = 9.70; // Gi? nguy�n 10
    }
				final_force = force_F * (result)/10 ;		
		}

		
		
		
		
#define MOCCU_CHUYEN 2500       
#define LUC_TAI_MOC_CHUYEN 80
#define LUC_MAX_CHUYEN 250
#define LUC_MIN_CHUYEN 50

#define SMOOTHING_FACTOR_CHUYEN 0.1  

float filtered_distance_chuyen = 0;

void lucchuyenbong(float received_distance) {
    float gocChuyen = 0;
    float lucChuyen = 0;
    float corrected_distance;  

    filtered_distance_chuyen = (SMOOTHING_FACTOR_CHUYEN * received_distance) + ((1 - SMOOTHING_FACTOR_CHUYEN) * filtered_distance_chuyen);

    corrected_distance = sqrt((filtered_distance_chuyen * filtered_distance_chuyen));  
    lucChuyen = LUC_TAI_MOC_CHUYEN * (corrected_distance / MOCCU_CHUYEN);

    if (lucChuyen > LUC_MAX_CHUYEN) {
        lucChuyen = LUC_MAX_CHUYEN;
    } else if (lucChuyen < LUC_MIN_CHUYEN) {
        lucChuyen = LUC_MIN_CHUYEN;
    }


    force_chuyen = lucChuyen;    
    final_F_chuyen = force_chuyen;

    if (force_chuyen == 50) result_chuyen = 10;
			
		final_F_chuyen = force_chuyen * (result_chuyen)/10 ;		
}
		
		
#include <math.h>
#define OFFSET_TOLERANCE 2
#define CENTER 100
#define MAX_SPEED 0.16
#define MIN_SPEED 0.075 

float calculateSpeed(int received_offset) {
    float distanceFromCenter;
    const float slope = 0.057;                 
    float speedRatio;
    float speed;

    // 2. Tính khoảng cách tuyệt đối
    distanceFromCenter = fabsf((float)(received_offset - CENTER));

    // 3. Kiểm tra vùng chết → dừng
    if (distanceFromCenter <= OFFSET_TOLERANCE) {
        return 1;
    }

    // 4. Tính tỷ lệ tốc độ
    speedRatio = tanhf(slope * distanceFromCenter); 

    // 5. Tính toán tốc độ dựa trên MIN/MAX
		if (distanceFromCenter < 15) {
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
        robotRotateFree(-received_speed, 1); 
    } else if (received_offset > CENTER + OFFSET_TOLERANCE) {
        robotRotateFree(received_speed, 1);  
    }
}

		
void calculate_angle() {
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

void nuot_bong(void)
{

// nuot bong				
	if(RJOY){
	 int i;
		Ban_1 =  Ban_2 =  Ban_3 = 10;
		Ban_1_back, Ban_2_back, Ban_3_back;

	  for(i=0;i<100;i++)	while(CB_NHAN_BONG_BO_BAN==1) {if(wantExit())	break; vTaskDelay(1);};
		vTaskDelay(3000);
		Cylinder_ban_OFF, Cylinder_ban = 250;
		Ban_1 = Ban_2 = Ban_3 = 0;
		taget_BT_Nong_Ban = 520;
	
	}
	


	}

	
	
	
	
//nuot truot
//if (R1) {
//        if (R1) { 
//            Ban_1 = Ban_2 = Ban_3 = 120;
//						Ban_1_next, Ban_2_next, Ban_3_next;
//            while (R1) {
//                vTaskDelay(100); 
//            }
//						
//            Ban_1 = Ban_2 = Ban_3 = 30;
//						Ban_1_back, Ban_2_back, Ban_3_back;
//						for(i=0;i<100;i++)	while(CB_NHAN_BONG_BO_BAN==1) {if(wantExit())	break; vTaskDelay(1);};
//						vTaskDelay(3000);
//						Cylinder_nang = 250;
//						Ban_1 = 0, Ban_2 = 0, Ban_3 = 0;

//        }
//    }	




void reset(void){
 if(OPTIONS) {
    // Reset các giá trị ban đầu
    Ban_1 = Ban_2 = Ban_3 = 0;
		track = 0;
	 	Cylinder_ban_ON,  Cylinder_ban = 0;
    target_BT_Xoay = 507;
     if (target_BT_Xoay < Min_BT_Xoay)
         target_BT_Xoay = Min_BT_Xoay;
     if (target_BT_Xoay > Max_BT_Xoay)
         target_BT_Xoay = Max_BT_Xoay;
		 
    // Kiểm tra và điều khiển động cơ xoay về vị trí 480
    if (abs(BienTroXoayValue - target_BT_Xoay) > 3) {
        if (BienTroXoayValue > target_BT_Xoay) {
            CamRotate_back;
            CamRotate = 20; 
        }
        else {
            CamRotate_next;
            CamRotate = 20;
        }
    }
    else {
        CamRotate = 2;
    }
    
    // Kiểm tra giới hạn an toàn
    if(BienTroXoayValue > 620) CamRotate = 0;
    if(BienTroXoayValue < 250) CamRotate = 0;
 }
}
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
		bientromamxoay_SUM += bien_tro_xoay_nong;
	}
	else
	{
		BienTroXoayValue = bientromamxoay_SUM / 10;
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



//void Nang_Ha_Nong_Ban_abs(void)
//{
//	if (R1 && X && (taget_BT_Nong_Ban < Max_BT_Nong_Ban))
//		taget_BT_Nong_Ban++, vTaskDelay(300);
//	else if (R1 && TRIANGLE && (taget_BT_Nong_Ban > Min_BT_Nong_Ban))
//		taget_BT_Nong_Ban--, vTaskDelay(300);
//}

////*******************************************************************


///////NONG BAN
void Nong_Ban()
{
	if (taget_BT_Nong_Ban < Min_BT_Nong_Ban )
		taget_BT_Nong_Ban = Min_BT_Nong_Ban;
	if (taget_BT_Nong_Ban > Max_BT_Nong_Ban)
		taget_BT_Nong_Ban = Max_BT_Nong_Ban;

	if (abs(bientronangluaValue - taget_BT_Nong_Ban) > 5)
	{
		if (bientronangluaValue > taget_BT_Nong_Ban)  Nongban_next;
		else                                       Nongban_back;

		speed_temp = abs(bientronangluaValue - taget_BT_Nong_Ban);
		if (speed_temp > 250) // 50
			speed_temp = 250;	
		if (speed_temp < 250) 							
			speed_temp = 250;
		
		Nongban = speed_temp;
	}
	else
		Nongban = 2;
//	
	if(bientronangluaValue > 650) Nongban = 0;
	if(bientronangluaValue < 500) Nongban = 0;
}


//////////NANG HA

void Giu_nang_ha()
{
	
	if (target_Nang_Ha < Min_Nang_Ha)
		target_Nang_Ha = Min_Nang_Ha;
	if (target_Nang_Ha > Max_Nang_Ha)
		target_Nang_Ha = Max_Nang_Ha;

	if (abs(bientronangbongValue - target_Nang_Ha) > 3)
	{
		if (bientronangbongValue > target_Nang_Ha)	nang_back;
		else																			nang_next;

		speed_temp = abs(bientronangbongValue - target_Nang_Ha)*2;
		if (speed_temp > 250)
				speed_temp = 250;
		
		if (speed_temp < 20)
				speed_temp = 20;
		

		
		
		if (bientronangbongValue < 70) speed_temp = 20;
		else if (bientronangbongValue < 100) speed_temp = 50;
					
		nang_ha = speed_temp;
	}
	else nang_ha = 2;
	if(state_nang == 0)  nang_ha = 0;
	
	
	}


void Nang_Ha_Nong_Ban(void)
{
	if (R2 && X && (target_Nang_Ha < Max_Nang_Ha))
		target_Nang_Ha++, vTaskDelay(300);
	else if (R2 && TRIANGLE && (target_Nang_Ha > Min_Nang_Ha))
		target_Nang_Ha--, vTaskDelay(400);
}

///////XOAY nONG
// void Xoay_Nong_Ban()
// {
//     if (target_BT_Xoay < Min_BT_Xoay)
//         target_BT_Xoay = Min_BT_Xoay;
//     if (target_BT_Xoay > Max_BT_Xoay)
//         target_BT_Xoay = Max_BT_Xoay;

//     if (abs(BienTroXoayValue - target_BT_Xoay) > 3)
//     {
//         if (BienTroXoayValue > target_BT_Xoay)  CamRotate_back;
//         else                                     CamRotate_next;

//         speed_temp = abs(BienTroXoayValue - target_BT_Xoay)*10;
//         if (speed_temp > 10)                             
//             speed_temp = 10;
        
//         CamRotate = speed_temp;
//     }
//     else
//         CamRotate = 2;

		

//     if(BienTroXoayValue > 620) CamRotate = 0;
//     if(BienTroXoayValue < 250) CamRotate = 0;

// }

void Xoay_nong (void)
{
	if (O && (target_BT_Xoay < Max_BT_Xoay))
		target_BT_Xoay++, vTaskDelay(750);
	else if (SQUARE && (target_BT_Xoay > Min_BT_Xoay))
		target_BT_Xoay --, vTaskDelay(750);
}


