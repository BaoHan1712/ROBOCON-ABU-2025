

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
float M_PI = 3.141592653589793238462643;
int xl_kepbong_state = 0;
int xl_nangkep_state = 0;
int ban_bong_state = 0;

//extern vu8 update;
extern speed_temp;
extern vu8 Cam;
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

extern int test;


void robotLineRunLeft(vu8 speed);
void robotLineRunRight(vu8 speed);
//=========================================BAN BONG==============================

void Ban_bong(void) {
    //ban bong
    if (TOUCHPAD) {
        vTaskDelay(2);  // Ch?ng d?i nút
        if (TOUCHPAD) {
            state++;
            if (state > 2) state = 0;  // Reset v? 0 khi vu?t quá 2
						
            while (TOUCHPAD);  // Ch? nút nh? ra m?i cho phép nh?n ti?p
        }
    }

    // L?n nh?n 1: C?p nh?t taget_BT_Nong_Ban
    if (state == 1) {
        XI_LANH_NANG_NONG;
				vTaskDelay(3000);
        Ban = 1;
				if(final_F_chuyen > 90) final_force =89;
        Ban_1 = Ban_2 = Ban_3 = final_force;
        Ban_1_next, Ban_2_next, Ban_3_next;
    }

    // L?n nh?n 2: Ch?y quy trình b?n
    else if (state == 2) {
				XI_LANH_DAY_BONG_RA;
        vTaskDelay(10000); // Wait 1 second
        Ban_1 = Ban_2 = Ban_3 = 0;
        Ban = 0;
				XI_LANH_HA_NONG;
				vTaskDelay(5000);
				XI_LANH_DAY_BONG_VAO;
				LAZER = 0;
				state = 0;
				
		}


			
			
//			if (TOUCHPAD) {
//				XI_LANH_NANG_NONG;
//				vTaskDelay(3000);
//        Ban = 1;
//				if(final_F_chuyen > 90) final_force =89;
//        Ban_1 = Ban_2 = Ban_3 = final_force;
//        Ban_1_next, Ban_2_next, Ban_3_next;				
//        vTaskDelay(15000); // Wait for 1.5 seconds
//				XI_LANH_DAY_BONG_RA;
//        vTaskDelay(10000); // Wait 1 second
//        Ban_1 = Ban_2 = Ban_3 = 0;
//        Ban = 0;
//        XI_LANH_DAY_BONG_VAO;
//				XI_LANH_HA_NONG;
//				LAZER = 0;
//			}				




//			if (TOUCHPAD) {
//			//	XI_LANH_NANG_NONG;
//				vTaskDelay(3000);
//        Ban = 1;
//				if(final_F_chuyen > 150) final_force =100;
//        Ban_1 = Ban_2 = Ban_3 = 120;
//        Ban_1_next, Ban_2_next, Ban_3_next;}
//				
//				
//				
//				
//				
//				if(R2){
//        //vTaskDelay(15000); // Wait for 1.5 seconds
//				XI_LANH_DAY_BONG_RA;
//        vTaskDelay(10000); // Wait 1 second
//        Ban_1 = Ban_2 = Ban_3 = 0;
//        Ban = 0;
//        XI_LANH_DAY_BONG_VAO;
//				XI_LANH_HA_NONG;
//			}			
	}

void Chuyen_Bong(void)
{
if (R2&&!TOUCHPAD) {

						XI_LANH_NANG_NONG;
	          Ban = 1;
						if(final_F_chuyen > 90) force_chuyen =90;
            Ban_1 = Ban_2 = Ban_3 = force_chuyen;
						Ban_1_next, Ban_2_next, Ban_3_next;
						vTaskDelay(15000);
						XI_LANH_DAY_BONG_RA;
						vTaskDelay(10000); // Wait 1 second
						Ban_1 = Ban_2 = Ban_3 = 0;
						Ban = 0;
						XI_LANH_HA_NONG;
						vTaskDelay(5000);
						XI_LANH_DAY_BONG_VAO;
						LAZER = 0;
        }	
}

// *****************************88 nang ha bo ban *********************************


//**********************************************************************



void nangnong(void)
 {
 // len
	if (!R2&&TRIANGLE){												state_giu = 0,	state_nang = 1, state_ha = 0, target_Nang_Ha = Min_Nang_Ha;
																						Ban_1 = Ban_2 = Ban_3 = 0;
																						state_re = 0;
																						LAZER = 50;
																						XI_LANH_NANG_NONG;
	
	}//165
	else if(bientronangbongValue <= Min_Nang_Ha + 3 &&  target_Nang_Ha == Min_Nang_Ha && state_giu == 0 )    Ban = 1;
	 
	 
	 // xuong
	if (!R2&&X)	
	{ 	  
	 
				target_Nang_Ha = Max_Nang_Ha ;
		//out state_giu 
				state_giu = 1;
				Ban = 0;
	 }
 else if(state_ha == 0&& target_Nang_Ha ==	Max_Nang_Ha  && bientronangbongValue > Max_Nang_Ha - 10 ) 
			{
				state_nang = 0;
				state_ha = 1;
			 }	 
 }
 
 
 
void Re_bong(void)
{
//						XI_LANH_HA_NONG;
//	          vTaskDelay(5000);
//            Ban_1 =26, Ban_2 =220, Ban_3 =26;
//						Ban_1_next, Ban_2_next, Ban_3_next;
//            vTaskDelay(12000);
//						XI_LANH_DAY_BONG_RA;
//	          vTaskDelay(5000);
//						XI_LANH_NANG_NONG;
//						//vTaskDelay(5000);
//	          Ban_1=  Ban_2 = Ban_3 = 0;
//						XI_LANH_DAY_BONG_VAO;
//						vTaskDelay(10000);
//						LAZER = 50;
//						Ban_1 =  Ban_2 =  Ban_3 = 10;
//						Ban_1_back, Ban_2_back, Ban_3_back;
//						for(i=0;i<100;i++)	while(CB_NHAN_BONG_BO_BAN==1) {if(wantExit())	break; vTaskDelay(1);};
//						vTaskDelay(3000);
//						XI_LANH_NANG_NONG;
//						Ban_1 = Ban_2 = Ban_3 = 0;

						
    if (L2) {
        vTaskDelay(50);  
        if (L2) {
            state_re++;
            if (state_re > 2) state_re = 0;  
            while (L2);  
        }
    }

   
    if (state_re == 1&&CB_NHAN_BONG_BO_BAN == 0) {
			XI_LANH_HA_NONG;
			vTaskDelay(5000);
		
     // Ban_1 =24, Ban_2 =190, Ban_3 =24;
			//Ban_1 =21, Ban_2 =220, Ban_3 =21;
			Ban_1 =23, Ban_2 =220, Ban_3 =23;
			Ban_1_next, Ban_2_next, Ban_3_next;	
    }

    // L?n nh?n 2: Ch?y quy trình b?n
    else if (state_re == 2) {

			XI_LANH_DAY_BONG_RA;
      vTaskDelay(5000);
//			XI_LANH_NANG_NONG;
						//vTaskDelay(5000);
	    Ban_1=  Ban_2 = Ban_3 = 0;
			XI_LANH_DAY_BONG_VAO;
			vTaskDelay(10000);
			LAZER = 50;
			Ban_1 =  Ban_2 =  Ban_3 = 10;
			Ban_1_back, Ban_2_back, Ban_3_back;
			for(i=0;i<100;i++)	while(CB_NHAN_BONG_BO_BAN==1) {if(wantExit())	break; vTaskDelay(1);};
			vTaskDelay(3000);
			XI_LANH_NANG_NONG;
			Ban_1 = Ban_2 = Ban_3 = 0;

      state_re = 0;  // Reset v? tr?ng thái ban d?u
										
				}
			}
 

 
 
/////////// lazer******************88
int tru_kc = 0;

void calculator_dis(float lazeTruocValue, int received_offset) {
    int cal_dis = 0;

        cal_dis = (lazeTruocValue - (received_offset));  
    
    tru_kc = cal_dis;
}

//#define MOCCU 206       
//#define LUC_TAI_MOC 37
//#define LUC_MAX 250
//#define LUC_MIN 20

//#define SMOOTHING_FACTOR 0.1  

//float filtered_lazeValue = 0;

//// H? s? h?i quy mu (suy ra d? l?c t?i MOCCU là dúng)
//#define A 19 
//#define B (log((float)LUC_TAI_MOC / A) / MOCCU)

//void lucbanlazer(float lazeTruocValue, int received_offset) {
//    float gocBan = 0;
//    float lucBan = 0;
//    float corrected_distance;

//    // Làm m?n tín hi?u do kho?ng cách
//   // filtered_lazeValue = (SMOOTHING_FACTOR * lazeTruocValue) + ((1 - SMOOTHING_FACTOR) * filtered_lazeValue);
//			filtered_lazeValue = (SMOOTHING_FACTOR * (lazeTruocValue - (received_offset)) + ((1 - SMOOTHING_FACTOR) * filtered_lazeValue));

//    // Tính kho?ng cách chu?n
//    corrected_distance = sqrt((filtered_lazeValue * filtered_lazeValue));

//    // Áp d?ng công th?c h?i quy s? mu
//    lucBan = A * exp(B * corrected_distance);

//    if (lucBan > LUC_MAX) {
//        lucBan = LUC_MAX;
//    } else if (lucBan < LUC_MIN) {
//        lucBan = LUC_MIN;
//    }


//    force_F = lucBan;    
//    final_force = force_F;
//		}

void Luc_co_dinh(void)
{
//						taget_BT_Nong_Ban = 540;
//	          Ban = 1;
//            Ban_1 = Ban_2 = Ban_3 = 100;
//						Ban_1_next, Ban_2_next, Ban_3_next;
//						vTaskDelay(15000); // Wait for 1.5 seconds
//						Cylinder_Chan = 0;
//						vTaskDelay(3000);
//						Cylinder_ban_ON;
//						Cylinder_ban = 250; // Activate the cylinder
//            vTaskDelay(10000);
//            Ban_1=  Ban_2 = Ban_3 = 0;
//						Ban = 0 ;
//            Cylinder_ban = 0;
//						Cylinder_Chan = 0;
//	XI_LANH_DAY_BONG_RA;
	
}
		
		
		
//#define MOCCU_CHUYEN 2500       
//#define LUC_TAI_MOC_CHUYEN 51
//#define LUC_MAX_CHUYEN 250
//#define LUC_MIN_CHUYEN 0

//#define SMOOTHING_FACTOR_CHUYEN 0.1  

//float filtered_distance_chuyen = 0;

//void lucchuyenbong(int received_distance) {
//    float gocChuyen = 0;
//    float lucChuyen = 0;
//    float corrected_distance;  

//    filtered_distance_chuyen = (SMOOTHING_FACTOR_CHUYEN * received_distance) + ((1 - SMOOTHING_FACTOR_CHUYEN) * filtered_distance_chuyen);

//    corrected_distance = sqrt((filtered_distance_chuyen * filtered_distance_chuyen));  
//    lucChuyen = LUC_TAI_MOC_CHUYEN * (corrected_distance / MOCCU_CHUYEN);

//    if (lucChuyen > LUC_MAX_CHUYEN) {
//        lucChuyen = LUC_MAX_CHUYEN;
//    } else if (lucChuyen < LUC_MIN_CHUYEN) {
//        lucChuyen = LUC_MIN_CHUYEN;
//    }


//    force_chuyen = lucChuyen;    
//    final_F_chuyen = force_chuyen;

//    if (force_chuyen == 50) result_chuyen = 10;
////		
////		
////		
////		
////			 if (force_chuyen >= 63 && force_chuyen <= 67) { 
////        result_chuyen = 10.4; // Gi? nguy?n 10
////    }  		
////			 if (force_F >= 68 && force_chuyen <= 72) { 
////        result_chuyen = 10.4; // Gi? nguy?n 10
////    }  		
////			 if (force_F >= 73 && force_chuyen <= 77) { 
////        result_chuyen = 10.6; // Gi? nguy?n 10

////		}		
////			 if (force_F >= 78 && force_chuyen <= 82) { 
////        result_chuyen = 10.6; // Gi? nguy?n 10

////		}		
////			 if (force_F >= 83 && force_chuyen <= 87) { 
////        result_chuyen = 10.8; // Gi? nguy?n 10

////		}		
////			 if (force_F >= 88 && force_chuyen <= 92) { 
////        result_chuyen = 10.8; // Gi? nguy?n 10

////		}	 if (force_F >= 93 && force_chuyen <= 97) { 
////        result_chuyen = 11; // Gi? nguy?n 10

////		}		
////		
////				if (force_F >= 98 && force_chuyen <= 102) { 
////        result_chuyen = 11; // Gi? nguy?n 10

////		}		
////				if (force_F >= 103 && force_chuyen <= 107) { 
////        result_chuyen = 11.2; // Gi? nguy?n 10

////		}			
////				if (force_F >= 108 && force_chuyen <= 112) { 
////        result_chuyen = 11.2; // Gi? nguy?n 10

////		}
////				if (force_F >= 113 && force_chuyen <= 117) { 
////        result_chuyen = 11.4; // Gi? nguy?n 10

////		}						
////				if (force_F >= 118 && force_chuyen <= 122) { 
////        result_chuyen = 11.4; // Gi? nguy?n 10

////		}			
//		final_F_chuyen = force_chuyen * (result_chuyen)/10 ;		
//}
//		

#define MOCCU_CHUYEN        1000       
#define LUC_TAI_MOC_CHUYEN  27
#define LUC_MAX_CHUYEN      250
#define LUC_MIN_CHUYEN      20
#define SMOOTHING_FACTOR_CHUYEN  0.1f

float filtered_distance_chuyen = 0;

void lucchuyenbong(int received_distance) {
    float lucChuyen = 0;
    float corrected_distance;
    float A = 19.0f;
    float B = logf((float)LUC_TAI_MOC_CHUYEN / A) / (float)MOCCU_CHUYEN;  

    filtered_distance_chuyen = (SMOOTHING_FACTOR_CHUYEN * received_distance) +
                               ((1 - SMOOTHING_FACTOR_CHUYEN) * filtered_distance_chuyen);

    corrected_distance = filtered_distance_chuyen;

    lucChuyen = A * expf(B * corrected_distance);  // expf dùng cho float

    // Gi?i h?n l?c
    if (lucChuyen > LUC_MAX_CHUYEN) {
        lucChuyen = LUC_MAX_CHUYEN;
    } else if (lucChuyen < LUC_MIN_CHUYEN) {
        lucChuyen = LUC_MIN_CHUYEN;
    }

    force_chuyen = lucChuyen;
    final_F_chuyen = force_chuyen;
}


//#define MOCCU_CHUYEN 206       
//#define LUC_TAI_MOC_CHUYEN 20
//#define LUC_MAX_CHUYEN 250
//#define LUC_MIN_CHUYEN 20
//#define LUC_MAX 250
//#define LUC_MIN 20

//#define SMOOTHING_FACTOR 0.1  
//float filtered_chuyen = 0;
//		
//// H? s? h?i quy mu (suy ra d? l?c t?i MOCCU là dúng)
//#define A_CHUYEN 20 
//#define B_CHUYEN (log((float)LUC_TAI_MOC_CHUYEN / A_CHUYEN) / MOCCU_CHUYEN )
//	

//void lucchuyen(int received_distance) {
//		float lucBan_chuyen = 0;
//    float corrected_distance_chuyen;
//	
//	filtered_chuyen = (SMOOTHING_FACTOR * (received_distance ) + ((1 - SMOOTHING_FACTOR) * filtered_chuyen));
//	
//	corrected_distance_chuyen = sqrt((filtered_chuyen * filtered_chuyen));
//	
//	lucBan_chuyen = A_CHUYEN * exp(B_CHUYEN * corrected_distance_chuyen);
//	
//	if (lucBan_chuyen > LUC_MAX) {
//        lucBan_chuyen = LUC_MAX;
//    } else if (lucBan_chuyen < LUC_MIN) {
//        lucBan_chuyen = LUC_MIN;
//    }


//    force_chuyen = lucBan_chuyen;    
//    final_F_chuyen = force_chuyen;

//    if (force_chuyen == 50) result_chuyen = 10;
//}		






//#include <math.h>
//#define OFFSET_TOLERANCE 2
//#define CENTER 100
//#define MAX_SPEED 4
//#define MIN_SPEED 3

//float calculateSpeed(int received_offset) {
//    // 1. Khai báo bi?n tru?c!
//    float distanceFromCenter;
//    const float slope = 0.057;                 
//    float speedRatio;
//    float speed;

//    // 2. Tính kho?ng cách tuy?t d?i
//    distanceFromCenter = fabsf((float)(received_offset - CENTER));

//    // 3. Ki?m tra vùng ch?t ? d?ng
//    if (distanceFromCenter <= OFFSET_TOLERANCE) {
//        return 0;
//    }

//    // 4. Tính t? l? t?c d?
//    speedRatio = tanhf(slope * distanceFromCenter); 

//    // 5. Tính toán t?c d? d?a trên MIN/MAX
//		if (distanceFromCenter < 15) {
//        speed = MIN_SPEED + (MAX_SPEED - MIN_SPEED) * 0.001;
//    }
//    else speed = MIN_SPEED + (MAX_SPEED - MIN_SPEED) * speedRatio;

//    // 6. Gi?i h?n t?c d? t?i da (c?n th?n)
//    if (speed > MAX_SPEED) {
//        speed = MAX_SPEED;
//    }

//    return speed;
//}

//void xoaytam(int received_offset) {
//	
//    int received_speed; //= calculateSpeed(received_offset);
//		Ban_1 =50, Ban_2 =50 , Ban_3 =50;
//		Ban_1_next, Ban_2_next, Ban_3_next;



//	
//		if(abs(received_offset - CENTER) > 3) 
//		{	
//			received_speed = received_offset - CENTER;
//			if	(abs(received_speed) > 20)		
//				{
//					if(received_speed > 0)	RobotRotate_free(5);
//					else										RobotRotate_free(-5);
//				}
//			else												//RobotRotate_XoayTam(3);
//				{
//					if(received_speed > 0)	RobotRotate_XoayTam(4);
//					else										RobotRotate_XoayTam(-4);
//				}
//		}
//    else   		robotRotateStop();
////    } else if (received_offset < CENTER - OFFSET_TOLERANCE) {
////        RobotRotate_free(-received_speed); 
////    } else if (received_offset > CENTER + OFFSET_TOLERANCE) {
////       RobotRotate_free(received_speed); 
////    }
//}
		
		
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
		LAZER = 50;
		Ban_1 = Ban_2 = Ban_3 = 0;
		XI_LANH_NANG_NONG;
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
    // Reset các giá tr? ban d?u
    Ban_1 = Ban_2 = Ban_3 = 0;
		track = 0;
	 	LAZER = 0;
	 XI_LANH_HA_NONG;
	 state_chuyen = 0;
	 state_re = 0;
	 state = 0;
	 
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
//void Nong_Ban()
//{
//	if (taget_BT_Nong_Ban < Min_BT_Nong_Ban )
//		taget_BT_Nong_Ban = Min_BT_Nong_Ban;
//	if (taget_BT_Nong_Ban > Max_BT_Nong_Ban)
//		taget_BT_Nong_Ban = Max_BT_Nong_Ban;

//	if (abs(bientronangluaValue - taget_BT_Nong_Ban) > 2)
//	{
//		if (bientronangluaValue > taget_BT_Nong_Ban)  Nongban_next;
//		else                                       Nongban_back;

//		speed_temp = abs(bientronangluaValue - taget_BT_Nong_Ban);
//		if (speed_temp > 250) // 50
//			speed_temp = 250;	
//		if (speed_temp < 250) 							
//			speed_temp = 250;
//		
//		Nongban = speed_temp;
//	}
//	else
//		Nongban = 2;
////	
//	if(bientronangluaValue > 650) Nongban = 0;
//	if(bientronangluaValue < 500) Nongban = 0;
//}


//////////NANG HA

void Giu_nang_ha()
{
	
	if (target_Nang_Ha < Min_Nang_Ha)
		target_Nang_Ha = Min_Nang_Ha;
	if (target_Nang_Ha > Max_Nang_Ha)
		target_Nang_Ha = Max_Nang_Ha;

	if (abs(bientronangbongValue - target_Nang_Ha) > 3)
	{
		if (bientronangbongValue > target_Nang_Ha)	nang_next;
		else																				nang_back;

		speed_temp = abs(bientronangbongValue - target_Nang_Ha)*2;
//		if (speed_temp > 150)
//				speed_temp = 150;
		
	if (speed_temp > 254)
			speed_temp = 254;
		

		
		
		//if (bientronangbongValue < 60) speed_temp = 20;

					
		nang_ha = speed_temp;
	}
	else nang_ha = 5;
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




