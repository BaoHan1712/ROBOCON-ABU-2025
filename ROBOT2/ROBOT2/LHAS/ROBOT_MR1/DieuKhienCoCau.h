

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


int Luc_CHUYENLASER;

int Ban = 0;
int state_nang = 1;
int state_ha = 1;
int state_giu = 1;
int free_spin = 0;

int state = 0;
int state_re = 0;
int state_chuyen = 0;
int track = 0;


float AC = 0;
float BC = 0;

int state_luc = 0;
int lazeTruocValue_Ban;
int lazeTruocValue_CHUYENLASER;

void robotLineRunLeft(vu8 speed);
void robotLineRunRight(vu8 speed);
//=========================================BAN BONG==============================
#define MOCCUCHUYENLASER 206       
#define LUC_TAI_MOCCHUYENLASER 37
#define LUC_MAXCHUYENLASER 250
#define LUC_MINCHUYENLASER 20

#define SMOOTHING_FACTORCHUYENLASER 0.1  

float filtered_lazeValueCHUYENLASER = 0;

// H? s? h?i quy mu (suy ra d? l?c t?i MOCCU là dúng)
#define A_CHUYENLASER 20 
#define B_CHUYENLASER (log((float)LUC_TAI_MOCCHUYENLASER / A_CHUYENLASER) / MOCCUCHUYENLASER)

void lucbanlazer(float lazeTruocValue_CHUYENLASER) {

    float lucChuyen = 0;
    float corrected_distance;

    // Làm m?n tín hi?u do kho?ng cách
   // filtered_lazeValueCHUYENLASER = (SMOOTHING_FACTORCHUYENLASER * lazeTruocValue_CHUYENLASER) + ((1 - SMOOTHING_FACTORCHUYENLASER) * filtered_lazeValueCHUYENLASER);
			filtered_lazeValueCHUYENLASER = (SMOOTHING_FACTORCHUYENLASER * (lazeTruocValue_CHUYENLASER) + ((1 - SMOOTHING_FACTORCHUYENLASER) * filtered_lazeValueCHUYENLASER));

    // Tính kho?ng cách chu?n
    corrected_distance = sqrt((filtered_lazeValueCHUYENLASER * filtered_lazeValueCHUYENLASER));

    // Áp d?ng công th?c h?i quy s? mu
    lucChuyen = A_CHUYENLASER * exp(B_CHUYENLASER * corrected_distance);

    if (lucChuyen > LUC_MAXCHUYENLASER) {
        lucChuyen = LUC_MAXCHUYENLASER;
    } else if (lucChuyen < LUC_MINCHUYENLASER) {
        lucChuyen = LUC_MINCHUYENLASER;
    }
    Luc_CHUYENLASER = lucChuyen;    
	
		}




//// khoang cách 
//#include <math.h>
//#include <stdio.h>




//void dis_DC(float lazeTruocValue ) {
//// Khai báo các bi?n c?n dùng trong phép tính
//float part1 = 0.0f;
//float part2 = 0.0f;
//float part3 = 0.0f;
//float part4 = 0.0f;
//float part5 = 0.0f;

//float AC = 0;
//float BC = 0;
//float DC = 0;	
//float AB = 35.0;
//float ED = 35.0;
//	
//	
//	
//	if (L2) {
//		vTaskDelay(50);  
//		if (L2) {
//				state_luc++;
//				if (state_luc > 2) state_luc = 0;  
//				while (L2) vTaskDelay(1);  
//		}
//}
//	
//if (state_luc == 1) {
//    AC = lazeTruocValue;
//    state_luc++;
//}
//if (state_luc == 2) {
//    BC = lazeTruocValue;
//    state_luc = 0;
//}
//	
//    // Tính DC
//    part1 = 0.25f * (2 * AC * AC + 2 * BC * BC - AB * AB);
//    part2 = ED * ED;
//    part3 = sqrtf(2 * AC * AC + 2 * BC * BC - AB * AB);
//    part4 = 1.0f - powf((AC * AC - BC * BC), 2) / (AB * AB * (2 * AC * AC + 2 * BC * BC - AB * AB));
//    part5 = ED * part3 * sqrtf(part4);

//    DC = sqrtf(part1 + part2 - part5);

//}




void nhan_l1(float lazerTruocValue){

	if (L2) {
		vTaskDelay(50);  
		if (L2) {
				state_luc++;
				if (state_luc > 2) state_luc = 0;  
				while (L2) vTaskDelay(1);  
		}
}
	
if (state_luc == 1) {
    AC = lazeTruocValue;
    state_luc++;
}
if (state_luc == 2) {
    BC = lazeTruocValue;
    state_luc = 0;
}

}



#include <math.h>
#include <stdio.h>

#define AB 35.0f
#define ED 35.0f

float calculate_DC(float AC, float BC) {
    float part1 = 0.25f * (2 * AC * AC + 2 * BC * BC - AB * AB);
    float part2 = ED * ED;
    float part3 = sqrtf(2 * AC * AC + 2 * BC * BC - AB * AB);
    float part4 = 1.0f - powf((AC * AC - BC * BC), 2) / (AB * AB * (2 * AC * AC + 2 * BC * BC - AB * AB));
    float part5 = ED * part3 * sqrtf(part4);

    float DC = sqrtf(part1 + part2 - part5);
    return DC;
}

float calculate_angle_DC(float AC, float BC, float DC) {
    float numerator;
    float denominator;
    float cos_angle;
    float angle;

    numerator = AC * AC + DC * DC - BC * BC;
    denominator = 2 * AC * DC;

    if (denominator == 0.0f) return 0.0f;

    cos_angle = numerator / denominator;
    if (cos_angle > 1.0f) cos_angle = 1.0f;
    if (cos_angle < -1.0f) cos_angle = -1.0f;

    angle = acosf(cos_angle) * (180.0f / M_PI) / 10.0f;
    return angle;
}


		
		



void Ban_bong(void) {
	
			if(CB_NHAN_BONG_BO_BAN == 0)
			{
				lazeTruocValue_Ban = lazeTruocValue;
				Ban_1 = Ban_2 = Ban_3 = final_force; // power_shot , testluc
				Ban_1_next, Ban_2_next, Ban_3_next;
			}	
	}

void Ban_bong_Goc_Cheo_1(void){
						if(CB_NHAN_BONG_BO_BAN == 0)
						{
							lazeTruocValue_Ban = lazeTruocValue-5;
							Ban_1 = Ban_2 = Ban_3 = final_force ; // power_shot , testluc
							Ban_1_next, Ban_2_next, Ban_3_next;
						}
	}		

void Ban_bong_Goc_Cheo_2(void){
						if(CB_NHAN_BONG_BO_BAN == 0)
						{
							lazeTruocValue_Ban = lazeTruocValue-10;
							Ban_1 = Ban_2 = Ban_3 = final_force  ; // power_shot , testluc
							Ban_1_next, Ban_2_next, Ban_3_next;
						}
	}			
void Ban_bong_Goc_Cheo_3(void){
						if(CB_NHAN_BONG_BO_BAN == 0)
						{
							lazeTruocValue_Ban = lazeTruocValue-15;
						  Ban_1 = Ban_2 = Ban_3 = final_force  ; // power_shot , testluc
							Ban_1_next, Ban_2_next, Ban_3_next;
						}
	}		
	
	


void Kich_Ban(){
	
	if (!L1&&TOUCHPAD)
	{
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
}


// *****************************88 nang ha bo ban *********************************

void Nang_ha(void){
 // len
	if (!R1&&!LJOY&&!L1&&TRIANGLE || L1 && TRIANGLE || L1 && SQUARE || L1 && X ||  L1 && O )
{						state_giu = 0,	state_nang = 1, state_ha = 0, target_Nang_Ha = Min_Nang_Ha;
																						Ban_1 = Ban_2 = Ban_3 = 0;
																						state_re = 0;
																						LAZER = 50;

	
}
	
	if (!R1&&!LJOY&&L1&&!TRIANGLE&&!SQUARE&&!X&&!O){
																						state_giu = 0,	state_nang = 1, state_ha = 0, target_Nang_Ha = Min_Nang_Ha+60;
																						state_re = 0;
																						LAZER = 50;

	
	}
	else if(bientronangbongValue <= Min_Nang_Ha + 3 &&  target_Nang_Ha == Min_Nang_Ha && state_giu == 0 )    Ban = 1;
	 
	 
	 // xuong
	if (!R1&&!LJOY&&!L1&&X)	
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
    if (L2) {
        vTaskDelay(50);  
        if (L2) {
            state_re++;
            if (state_re > 2) state_re = 0;  
            while (L2) vTaskDelay(1);  
        }
    }

   
    if (state_re == 1&&CB_NHAN_BONG_BO_BAN == 0) {

//			XI_LANH_NANG_NONG;
			vTaskDelay(3000);
			//Ban_1 =10, Ban_2 =250, Ban_3 =10	
			Ban_1 =15, Ban_2 =230, Ban_3 =15;
			Ban_1_next, Ban_2_next, Ban_3_next;	
    }

    // L?n nh?n 2: Ch?y quy trình b?n
    else if (state_re == 2) {

			XI_LANH_DAY_BONG_RA;
      vTaskDelay(8000);
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


void Luc_co_dinh(void)
{
      if(CB_NHAN_BONG_BO_BAN == 0){
			lazeTruocValue_CHUYENLASER = lazeTruocValue;
			Ban_1 = Ban_2 = Ban_3 = Luc_CHUYENLASER  ; // power_shot , testluc
			Ban_1_next, Ban_2_next, Ban_3_next;
			}
}
void Luc_co_dinh_1(void)
{
      if(CB_NHAN_BONG_BO_BAN == 0){
			lazeTruocValue_CHUYENLASER = lazeTruocValue;
			Ban_1 = Ban_2 = Ban_3 = Luc_CHUYENLASER  ; // power_shot , testluc
			Ban_1_next, Ban_2_next, Ban_3_next;
			}
}		
void Luc_co_dinh_2(void)
{
      if(CB_NHAN_BONG_BO_BAN == 0){
			lazeTruocValue_CHUYENLASER = lazeTruocValue;
			Ban_1 = Ban_2 = Ban_3 = Luc_CHUYENLASER  ; // power_shot , testluc
			Ban_1_next, Ban_2_next, Ban_3_next;
			}
}		
void Luc_co_dinh_3(void)
{
      if(CB_NHAN_BONG_BO_BAN == 0){
			lazeTruocValue_CHUYENLASER = lazeTruocValue;
			Ban_1 = Ban_2 = Ban_3 = Luc_CHUYENLASER  ; // power_shot , testluc
			Ban_1_next, Ban_2_next, Ban_3_next;
			}
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
#include <math.h>  // Ð? dùng exp() và log()

#define MOCCU_CHUYEN             1000
#define LUC_TAI_MOC_CHUYEN       76
#define LUC_MAX_CHUYEN           250
#define LUC_MIN_CHUYEN           20

#define SMOOTHING_FACTOR_CHUYEN  0.5

// Tính h?ng s? h?i quy mu d?a trên m?c dã bi?t
#define A_CHUYEN                 25
#define B_CHUYEN                 (log(LUC_TAI_MOC_CHUYEN / A_CHUYEN) / MOCCU_CHUYEN)

// Bi?n toàn c?c luu tr?ng thái

void lucchuyenbong(int rx_chuyen) {
    int rx_distance = rx_chuyen;
    float corrected_distance;
	float filtered_distance_chuyen = 0;
    float lucChuyen;

    // Làm mu?t tín hi?u d?u vào
    filtered_distance_chuyen = (SMOOTHING_FACTOR_CHUYEN * rx_distance) +
                               ((1.0 - SMOOTHING_FACTOR_CHUYEN) * filtered_distance_chuyen);

    // L?y tr? tuy?t d?i kho?ng cách dã làm mu?t
    corrected_distance = fabs(filtered_distance_chuyen);

    // H?i quy s? mu d? tính l?c chuy?n
    lucChuyen = A_CHUYEN * exp(B_CHUYEN * corrected_distance);
		
		 if (rx_distance > 2000 && rx_distance < 2300) {
        lucChuyen *= 0.95;
    }
		else if (rx_distance > 2300 && rx_distance < 2700) {
        lucChuyen *= 0.9;
    }
    // N?u kho?ng cách l?n hon 3000, tang l?c thêm 10%
    else if (rx_distance > 2700 && rx_distance < 3000) {
        lucChuyen *= 0.85;
    }
		else if (rx_distance > 3000 && rx_distance < 3350) {
        lucChuyen *= 0.82;
    }
		else if (rx_distance > 3350 && rx_distance < 3600) {
        lucChuyen *= 0.79;
    }
		else if (rx_distance > 3600 && rx_distance < 4000) {
        lucChuyen *= 0.76;
    }
		else if (rx_distance > 4000 && rx_distance < 10000) {
        lucChuyen *= 0.73;
    }

    // Gi?i h?n l?c n?m trong ph?m vi
    if (lucChuyen > LUC_MAX_CHUYEN) {
        lucChuyen = LUC_MAX_CHUYEN;
    } else if (lucChuyen < LUC_MIN_CHUYEN) {
        lucChuyen = LUC_MIN_CHUYEN;
    }

    // Gán giá tr? ra các bi?n toàn c?c
    force_chuyen = lucChuyen;
    final_F_chuyen = force_chuyen;
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
void Chuyen_Bong(void)
{
    static vu8 R2_count = 0; // Bi?n d?m s? l?n nh?n R2

    if (R2 && !TOUCHPAD) {
        vTaskDelay(20); // Ch?ng d?i phím
        if (R2 && !TOUCHPAD) {
            R2_count++;
            if (R2_count > 2) R2_count = 1;  // Sau 2 l?n thì reset l?i v? 1

            while (R2); // Ch? th? nút ra

            if (R2_count == 1) {
                // L?n nh?n th? 1: ch? truy?n lên l?y d? li?u
             truyenlen();
							vTaskDelay(20);
						 lucchuyenbong(rx_chuyen);
            }
            else if (R2_count == 2) {

                vTaskDelay(1000);
                Ban = 1;

//                if (final_F_chuyen > 90) force_chuyen = 90;
                Ban_1 = Ban_2 = Ban_3 = force_chuyen;
                Ban_1_next;
                Ban_2_next;
                Ban_3_next;
            }
        }
    }
}
void Chuyen_laser (float laserTruocvalue){
	  
      if(CB_NHAN_BONG_BO_BAN == 0){
			lazeTruocValue_CHUYENLASER = lazeTruocValue;
			Ban_1 = Ban_2 = Ban_3 = Luc_CHUYENLASER  ; // power_shot , testluc
			Ban_1_next, Ban_2_next, Ban_3_next;
			}
}
	

int imu_hientai= 0;

void xoay_theo_goc(int IMU)
{
	int imu_capnhat = IMU;
    // N?u chua b?t d?u xoay và có nh?n nút
    if (L1 && !TOUCHPAD) {
        vTaskDelay(20); // Ch?ng d?i phím
        if (L1 && !TOUCHPAD) {
            while (L1); // Ch? th? nút ra
						imu_hientai = IMU + 100;
						vTaskDelay(50);   
            robotResetIMU();       
            vTaskDelay(50);       
				while (imu_capnhat < imu_hientai)
            robotRotateFree(3, 0); 
				for(i=0;i<100;i++) while(imu_capnhat > imu_hientai) {
		if(wantExit()) break;
		vTaskDelay(1);
				}   
    }

    }
}


void nuot_bong_duoi(void)
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

	}
	}

	
void nuot_bong_tren(void)
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


// Khai báo bi?n toàn c?c d? luu tr? th?i gian
u32 system_tick = 0;

// C?u hình SysTick d? tang m?i mili giây


// Hàm d? l?y th?i gian hi?n t?i t? SysTick
u32 GetSysTick(void)
{
    return system_tick;
}	

u32 matTrai_time = 0;
u32 matPhai_time = 0;


void duoiTheoRobot(void)
{
    static int  tocdo = 50;
    vu8 test = 30;
    u32 now = GetSysTick();  // S? d?ng GetSysTick d? l?y th?i gian hi?n t?i tính b?ng millisecond

    u32 duration;

    // N?u c? 3 c?m bi?n d?u không phát hi?n d?ch
    if (CB_TRAI == 0 && CB_GIUA == 0 && CB_PHAI == 0)
    {
        robotStop(25);
        matTrai_time = 0;
        matPhai_time = 0;
//        tocdo = 0;
				test = 0;
        return;
    }

    // N?u c?m bi?n gi?a phát hi?n d?ch, reset th?i gian và t?c d?
    if (CB_GIUA)
    {
        //tocdo = 10;
        matTrai_time = 0;
        matPhai_time = 0;
        return;
    }

    // N?u c?m bi?n trái m?t d?ch và c?m bi?n ph?i v?n còn
    if (!CB_TRAI && CB_PHAI)
    {
        if (matTrai_time == 0) matTrai_time = now;

        duration = now - matTrai_time;
        tocdo = 10 + duration / 2;  // Tang t?c d? theo th?i gian m?t tín hi?u

        if (tocdo > 50) tocdo = 50;

        robotRun(-900, test);  // Ðu?i bên trái
    }
    // N?u c?m bi?n ph?i m?t d?ch và c?m bi?n trái v?n còn
    else if (!CB_PHAI && CB_TRAI)
    {
        if (matPhai_time == 0) matPhai_time = now;

        duration = now - matPhai_time;
        tocdo = 10 + duration / 2;

        if (tocdo > 20) tocdo = 20;

        robotRun(900, test);  // Ðu?i bên ph?i
    }
    else
    {
        matTrai_time = 0;
        matPhai_time = 0;
    }
}
//====================================================================================
//void vuotQuaRobot_xxx(void)
//{
//	int KhoangCachDung = 5000;
//	int KhoangCachDaTien = 0;
//	int HuongChay = 0;
//	
//	robotRunAngle(900, 130, 0, 0);
//	for(i=0;i<100;i++) while(lazePhaiValue > 200) {
//		//if(CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ) break;
//		if(wantExit()) break;
//		vTaskDelay(1);
//	};
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

//	robotStop(0);
//	vTaskDelay(1000);

//}
//====================================================================================
//void vuotQuaRobot(void)
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
//====================================================================================
void vuotQuaRobot(void)
{
	int KhoangChayNgang = 3000;
	int KhoangCachDung = 3500;
	int KhoangCachDaTien = 0;
	int HuongChay = 1;

	if(CB_TRAI == 1 && (CB_GIUA == 0 || CB_PHAI == 0)) HuongChay = -1;
		
	while(KhoangCachDaTien < KhoangCachDung){
		if(wantExit()) break;
		
		if(CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ){
			RESET_ENCODER_CHAY(); 
			robotRunAngle(0, 60, 0, 0);
			for(i=0;i<100;i++) while(ENCODER_XOAY() < KhoangCachDung) {
				//if(CB_TRAI == 0 || CB_GIUA == 0 || CB_PHAI == 0 ) break;
				if(CB_GIUA == 0) break;
				if(wantExit()) break;
				vTaskDelay(1);
			};
			KhoangCachDaTien = KhoangCachDaTien + ENCODER_XOAY();
			robotStop(0);
			HuongChay = 2;
			vTaskDelay(1000);
		}
		
		if(HuongChay == 2 || KhoangCachDaTien > KhoangCachDung) break;
		
		if(HuongChay == 1){
			robotStop(0);	
			RESET_ENCODER_CHAY(); 
			robotRunAngle(900, 60, 0, 0);
			for(i=0;i<100;i++) while(ENCODER_XOAY() < KhoangChayNgang) {
				if(CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ) break;
				if(wantExit()) break;
				vTaskDelay(1);
			};

			if(ENCODER_XOAY() < KhoangChayNgang && CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ){
				RESET_ENCODER_CHAY(); 
				robotRunAngle(450, 60, 0, 0);
				for(i=0;i<100;i++) while(ENCODER_XOAY() < 2000) {
					//if(CB_PHAI == 1) break;
					if(wantExit()) break;
					vTaskDelay(1);
				};
				HuongChay = 2;
			}else{
				HuongChay = -1;			
				robotStop(20);
				vTaskDelay(4000);
				robotStop(0);
			}				
		}

		if(HuongChay == -1){
			RESET_ENCODER_CHAY(); 
			robotRunAngle(-900, 60, 0, 0);
			for(i=0;i<100;i++) while(ENCODER_XOAY() < KhoangChayNgang) {
				if(CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ) break;
				if(wantExit()) break;
				vTaskDelay(1);
			};

			if(ENCODER_XOAY() < KhoangChayNgang && CB_TRAI == 1 && CB_GIUA == 1 && CB_PHAI == 1 ){
				RESET_ENCODER_CHAY(); 
				robotRunAngle(-450, 60, 0, 0);
				for(i=0;i<100;i++) while(ENCODER_XOAY() < 2000) {
					//if(CB_TRAI == 1) break;
					if(wantExit()) break;
					vTaskDelay(1);
				};
				HuongChay = 2;
			}else{
				HuongChay = 1;
				robotStop(20);
				vTaskDelay(4000);
				robotStop(0);
			}
		}
	}
	
	robotStop(0);
	vTaskDelay(1000);
}


void Chay_giua_san (){
	 int i;

		Ban_1 =  Ban_2 =  Ban_3 = 10;
		Ban_1_back, Ban_2_back, Ban_3_back;
	  for(i=0;i<100;i++)	while(CB_NHAN_BONG_BO_BAN==1) {if(wantExit())	break; vTaskDelay(1);};
		vTaskDelay(3000);
		LAZER = 50;
		Ban_1 = Ban_2 = Ban_3 = 0;
		//vTaskDelay(3000); 
		RESET_ENCODER_CHAY(); 
		robotRunAngle(0, 70, 0, 0);
		for(i=0;i<100;i++) while(ENCODER_XOAY() < 7000) {
		if(wantExit()) break;
		vTaskDelay(1);
	};
		
		RESET_ENCODER_CHAY(); 
		robotRunAngle(450, 60, 0, 0);
		for(i=0;i<100;i++) while(ENCODER_XOAY() < 4000) {

			if(wantExit()) break;
			vTaskDelay(1);
		};
		robotStop(0);
		vTaskDelay(1000);

	
}

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
		if (speed_temp > 250)
				speed_temp = 250;
		
//	if (speed_temp > 254)
//			speed_temp = 254;
		

		
		
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




