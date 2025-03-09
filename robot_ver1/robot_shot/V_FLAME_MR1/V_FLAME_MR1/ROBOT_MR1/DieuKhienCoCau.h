//*****************************8 bien toan cuc *************************************
#include "stm32f4xx_it.h"
char xl_truot_lua, xl_kep_lua_tren, xl_kep_lua_duoi, xl_nang_lua,gt_kep_bong;
char vi_tri = 0, Cbcc = 0, Cbnv = 0, Cbnn = 0,vt_xl_nang_lua = 0;
char	On_Off_Ban = 0;

int Nang_vong_max = 322, Nang_vong_min = 105, i;
int Nang_goc_max = 540, Nang_goc_min = 300;

int en_tru = 60;
int max_speed = 10;
int speed_temp = 0;
int speed_tay_lua = 100;
int speed_xoay = 200;

int xl_kepbong_state = 0;
int xl_nangkep_state = 0;
int ban_bong_state=0 ;
int xoay_state = 0;

float temp;
float result;  
float result_lidar;

int final_result;
int final_result_lidar;
int calculated_force;
int calculated_distance;

extern int force ;
extern int degree_F;

extern vu8 update;
extern char shooting; 
extern vu8	Cam;
extern float	JTA;


extern float offsettam;
int distanceFromCenter;
int hs_goc;
int hs_goc_lidar;
void robotLineRunLeft(vu8 speed);
void robotLineRunRight(vu8 speed);



// char CB_line_trai, CB_line_phai, Nho_trai_truoc, Nho_trai_sau, Nho_phai_truoc, Nho_phai_truoc;
void robotGamePadControl(void)
{

	int tocdo = 15 , td_x = 5;
	float tocdoXoay = 0;
	int rJoy_LR = (RJOY_LR >= 30 && RJOY_LR <= 180) ? 1 : 0;
	//int R2_Track = (R2 >= 2 && R2 <= 250) ? 1 : 0;
	
   calculate_angle();
	 
	

	if (!UP && !DOWN && !RIGHT && !LEFT && rJoy_LR && JTA > 15000&& !R2)
	{				  // Khong dieu khien
		robotStop(0); // truot banh

		return;
	}
//	
	/*_____________________________//___________________________*/

	/********************** TOC DO ****************************/
	/*_____________________________//___________________________*/
	//if (L2)
		//tocdo = 100, tocdoXoay = 0.7;
	//else
//		tocdo = 20+GP_BTN[6]/1.5, tocdoXoay = 0.1 + GP_BTN[6]/500;

		
		if(L1) 	tocdo = 250, tocdoXoay = 0.7;
		else		tocdo = 60, tocdoXoay = 0.2;


	//--------- chay cac huong -----------

	if (UP && !DOWN && !RIGHT && !LEFT)
		robotRun(-300, tocdo);
	else if (!UP && DOWN && !RIGHT && !LEFT)
		robotRun(1500, tocdo);
	else if (!UP && !DOWN && RIGHT && !LEFT)
		robotRun(600, tocdo);
	else if (!UP && !DOWN && !RIGHT && LEFT)
		robotRun(-1200, tocdo);

	else if (UP && !DOWN && RIGHT && !LEFT)
		robotRun(300, tocdo);
	else if (UP && !DOWN && !RIGHT && LEFT)
		robotRun(-900, tocdo);
	else if (!UP && DOWN && RIGHT && !LEFT)
		robotRun(900, tocdo);
	else if (!UP && DOWN && !RIGHT && LEFT)
		robotRun(-1500, tocdo);
	else if	(JTA < 15000)	robotRun(JTA, tocdo);

	//-------------- Dang chay va Khong chay nua, chi xoay ----------------
	if (!UP && !DOWN && !RIGHT  && !LEFT && rJoy_LR && JTA > 15000 && robotIsRun()&& !R2)
	robotStop(0);

	//-------------- Xoay ----------------	
	if (rJoy_LR && !R2)
	robotRotateStop();
	
	else if ((RJOY_LR < 20))
	{
		if (robotIsRun())
			robotRotateFree(-tocdoXoay, 0);
		else
			robotRotateFree(-tocdoXoay, 0); 
	}
	else if ((RJOY_LR > 180) )
	{
		if (robotIsRun())
			robotRotateFree(tocdoXoay, 0);
		else
			robotRotateFree(tocdoXoay, 0);
	}
	else
		robotRotateStop();
}

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
	if (lazeSauCouter++ < 50)
	{
		lazeSau_SUM += cam_bien_laze_sau;
	}
	else
	{
		lazeSauValue = lazeSau_SUM / 50;
		lazeSauCouter = 0;
		lazeSau_SUM = 0;
	}
	//-------------------------------------------
	if (lazeTruocCouter++ < 50)
	{
		lazeTruoc_SUM += cam_bien_laze_truoc;
	}
	else
	{
		lazeTruocValue = lazeTruoc_SUM / 50;
		lazeTruocCouter = 0;
		lazeTruoc_SUM = 0;
	}
	//------------------------------
	if (lazePhaiCouter++ < 50)
	{
		lazePhai_SUM += cam_bien_laze_phai;
	}
	else
	{
		lazePhaiValue = lazePhai_SUM / 50;
		lazePhaiCouter = 0;
		lazePhai_SUM = 0;
	}
	//-----------------------------------------
	if (lazeTraiCouter++ < 50)
	{
		lazeTrai_SUM += cam_bien_laze_trai;
	}
	else
	{
		lazeTraiValue = lazeTrai_SUM / 50;
		lazeTraiCouter = 0;
		lazeTrai_SUM = 0;
	}
	//-----------------TINH TOAN BIEN TRO---------------

	if (bientromamxoayCouter++ < 50)
	{
		bientromamxoay_SUM += bien_tro_nong_ban;
	}
	else
	{
		BienTroNongBanValue = bientromamxoay_SUM / 50;
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

//********************* DUA BONG CHO ROBOT*************************
void Lay_bong(void)
{
	static bool re = false;
	if(!R1 && X)	
	{
			XI_LANH_RE_RA;
			if(!CB_XI_LANH_TRUOT_DAY_RA)	XI_LANH_KEPBONG_MO;
		  re = true;
	}
	else if (re)
	{
			XI_LANH_KEPBONG_DONG;
			//vTaskDelay(3000);
			XI_LANH_RE_VAO;
			re = false;
	}
	
}
//********************* Re Bong *************************
void Re_Bong(void) 
{
	int i;
	{
		  taget_BT_Xoay = 475;
			XI_LANH_RE_RA;
			//vTaskDelay(15000);
			for(i=0;i<100;i++)	while(CB_XI_LANH_TRUOT_DAY_RA==1) {if(wantExit())	break; vTaskDelay(1);};
			vTaskDelay(1500);
			XI_LANH_DAYBONG_XUONG;
			vTaskDelay(500);
		
			XI_LANH_KEPBONG_MO;
			//XI_LANH_NANGHAKEP_XUONG;
			for(i=0;i<100;i++)	while(CB_NHAN_BONG==1) {if(wantExit())	break; vTaskDelay(1);};
			for(i=0;i<100;i++)	while(CB_NHAN_BONG==0) {if(wantExit())	break; vTaskDelay(1);};
		
			//if(CB_RE_BONG==0) XI_LANH_DAYBONG_LEN;
			vTaskDelay(1000);
			XI_LANH_DAYBONG_LEN;
			vTaskDelay(1500);
		
			for(i=0;i<100;i++)	while(CB_NHAN_BONG==1) {if(wantExit())	break; vTaskDelay(1);};
			vTaskDelay(500);
			XI_LANH_KEPBONG_DONG;
			//XI_LANH_NANGHAKEP_LEN;
			
			vTaskDelay(2000);
			XI_LANH_RE_VAO;
			for(i=0;i<100;i++)	while(CB_XI_LANH_TRUOT_DAY_VAO==1) {if(wantExit())	break; vTaskDelay(1);};
	
	}
}					  

//*********************** �UA B�NG V�O PHAN BAN ***************************
void dua_bong(void)
{
	int i;
	  taget_BT_Xoay = 475;
		//vTaskDelay(3000);
	  while(BienTroNongBanValue > 480 )		{if(wantExit())	break; vTaskDelay(1);}
		XI_LANH_KEPBONG_MO;
		vTaskDelay(5000);
		Ban_1 =  Ban_2 =  Ban_3 = 10	;
		Ban_1_back, Ban_2_back, Ban_3_back;
		vTaskDelay(5000);
		taget_BT_Xoay = 570;
		XI_LANH_KEPBONG_DONG;
	
		for(i=0;i<100;i++)	while(CB_NHAN_BONG_BO_BAN==1) {if(wantExit())	break; vTaskDelay(1);};
		vTaskDelay(3000);
		Ban_1 = 0, Ban_2 = 0, Ban_3 = 0;
}

void Day_re()
{

		if(O&&!R1){
			XI_LANH_RE_RA;
		}
		else if (OPTIONS)
		{

			XI_LANH_RE_VAO;
			Ban_1 = Ban_2 = Ban_3 = 0;

		
		}
}

void tra_bong(void)
{
	if(LJOY&&CB_NHAN_BONG_BO_BAN==0){
										taget_BT_Xoay = 600;
	            			Ban_1 = Ban_2 = Ban_3 = 30;
										Ban_1_next, Ban_2_next, Ban_3_next;
										vTaskDelay(10000);
										XI_LANH_DAY_BONG_RA;
										vTaskDelay(10000);
										Ban_1 = Ban_2 = Ban_3 = 0;
										XI_LANH_DAY_BONG_VAO;
		
	}
}


void nhabong()
	{  
	if(RJOY){
	 int i;
		Ban_1 =  Ban_2 =  Ban_3 = 20	;
		Ban_1_back, Ban_2_back, Ban_3_back;

		for(i=0;i<100;i++)	while(CB_NHAN_BONG_BO_BAN==1) {if(wantExit())	break; vTaskDelay(1);};
		
		
		vTaskDelay(3000);
		Ban_1 = 0, Ban_2 = 0, Ban_3 = 0;
	
	}}
//******************** ban bong *************************
void ban_bong_giua(void)
{
										robotStop(10);
										Ban_1 = Ban_2 = Ban_3 = 125;
										Ban_1_next, Ban_2_next, Ban_3_next;
										vTaskDelay(15000);
										XI_LANH_DAY_BONG_RA;
										vTaskDelay(7000);
  
										Ban_1 = Ban_2 = Ban_3 = 0;
										XI_LANH_DAY_BONG_VAO;
}




void calculate_force(int force, int calculated_force) {  
		final_result_lidar = calculated_force;
		final_result =  force;
	  
	
//**************************He so cua lazer************************************
						if (force == 73) result = 14.65,hs_goc = degree_F - 20;
			else if (force == 74) result = 14.7, hs_goc = degree_F - 20;
			else if (force == 75) result = 14.75, hs_goc = degree_F - 20;
			else if (force == 76) result = 14.85, hs_goc = degree_F-20;
			else if (force == 77) result = 14.9, hs_goc = degree_F-20;
			else if (force == 78) result = 14.95, hs_goc = degree_F-20;
			else if (force == 79) result = 15.05, hs_goc = degree_F-20;
			else if (force == 80) result = 15.1, hs_goc = degree_F-20;
			else if (force == 81) result = 15.15, hs_goc = degree_F - 20;
			else if (force == 82) result = 15.2, hs_goc = degree_F - 20;
			else if (force == 83) result = 15.25, hs_goc = degree_F - 20;
			else if (force == 84) result = 15.3, hs_goc = degree_F - 20;
			else if (force == 85) result = 15.35, hs_goc = degree_F - 20;
			else if (force == 86) result = 15.4, hs_goc = degree_F - 20;
			else if (force == 87) result = 15.45, hs_goc = degree_F - 20;
			else if (force == 88) result = 15.5, hs_goc = degree_F - 20;
			else if (force == 89) result = 15.55, hs_goc = degree_F - 20;
			else if (force == 90) result = 15.6, hs_goc = degree_F - 20;
			else if (force == 91) result = 15.65, hs_goc = degree_F - 20;
			else if (force == 92) result = 15.75, hs_goc = degree_F - 20;
			else if (force == 93) result = 15.85, hs_goc = degree_F - 20;
			else if (force == 94) result = 15.95, hs_goc = degree_F - 20;
			else if (force == 95) result = 16.05, hs_goc = degree_F - 17;
			else if (force == 96) result = 16.15, hs_goc = degree_F - 17;
			else if (force == 97) result = 17.0, hs_goc = degree_F - 17;
			else if (force == 98) result = 17.0, hs_goc = degree_F - 17;
			else if (force == 99) result = 17.2, hs_goc = degree_F - 17;
			else if (force == 100) result = 17.2, hs_goc = degree_F - 17;
			else if (force == 101) result = 17.4, hs_goc = degree_F - 17;
			else if (force == 102) result = 17.4, hs_goc = degree_F - 17;
			else if (force == 103) result = 17.6, hs_goc = degree_F - 17;
			else if (force == 104) result = 17.6, hs_goc = degree_F - 17;
			else if (force == 105) result = 17.8, hs_goc = degree_F - 17;
			else if (force == 106) result = 17.8, hs_goc = degree_F - 17;
			else if (force == 107) result = 18.0, hs_goc = degree_F - 19;
			else if (force == 108) result = 18.0, hs_goc = degree_F - 19;
			else if (force == 109) result = 17.55, hs_goc = degree_F - 19;
			else if (force == 110) result = 17.60, hs_goc = degree_F - 19;
			else if (force == 111) result = 17.65, hs_goc = degree_F - 19;
			else if (force == 112) result = 17.70, hs_goc = degree_F - 19;
			else if (force == 113) result = 17.75, hs_goc = degree_F - 19;
			else if (force == 114) result = 17.80, hs_goc = degree_F - 19;
			else if (force == 115) result = 17.85, hs_goc = degree_F - 19;
			else if (force == 116) result = 17.90, hs_goc = degree_F - 19;
			else if (force == 117) result = 17.95, hs_goc = degree_F - 19;
			else if (force == 118) result = 18.0, hs_goc = degree_F - 19;
			else if (force == 119) result = 18.0, hs_goc = degree_F - 19;
			else if (force == 120) result = 18.1, hs_goc = degree_F - 19;




			
//**************************He so cua lidar************************************


		// Truòng hop tù 150 dén 230 thì tang dèu 0.05
		if (calculated_force >= 150 && calculated_force <= 153) {
    result_lidar = 9.9 - (calculated_force - 150) * 0.22,hs_goc_lidar = calculated_distance-5;
 }	else if (calculated_force >= 154 && calculated_force <= 156) 
		result_lidar = 9.9 - (calculated_force - 150) * 0.112,hs_goc_lidar = calculated_distance-5; 
		else if (calculated_force >= 157 && calculated_force <= 160) 
		result_lidar = 10.2 - (calculated_force - 150) * 0.077,hs_goc_lidar = calculated_distance-5;
		else if (calculated_force >= 161 && calculated_force <= 162) 
		result_lidar = 10.2 - (calculated_force - 150) * 0.065,hs_goc_lidar = calculated_distance-5;
		else if (calculated_force >= 163 && calculated_force <= 165) 
		result_lidar = 10.2 - (calculated_force - 150) * 0.061,hs_goc_lidar = calculated_distance-5;
		else if (calculated_force >= 170 && calculated_force <= 174) 
		 result_lidar = 9.4,taget_BT_Xoay = calculated_distance-5;
		
		else if (calculated_force >= 177 && calculated_force <= 178) 
		result_lidar = 9.205, taget_BT_Xoay = calculated_distance - 5;
		else if (calculated_force >= 180 && calculated_force <= 181) 
		result_lidar = 9.28;
		else if (calculated_force >= 182 && calculated_force <= 184) 
		result_lidar = 9.26;
//		else if (calculated_force >= 185 && calculated_force <= 186) 
//		result_lidar = 9.236;
		else if (calculated_force >= 187 && calculated_force <= 188) 
		result_lidar = 9.22;
		else if (calculated_force >= 189 && calculated_force <= 192) 
		result_lidar = 9.18;
		else if (calculated_force >= 193 && calculated_force <= 195) 
		result_lidar = 9.17;
		else if (calculated_force >= 196 && calculated_force <= 199) 
		result_lidar = 8.6;
		
		
else {
    // Các điều kiện khác từ 123 ến 149
		switch (calculated_force) {
				case 99: result_lidar = 11.30, hs_goc_lidar = calculated_distance - 40; break;
				case 100: result_lidar = 11.35, hs_goc_lidar = calculated_distance - 40; break;
				case 101: result_lidar = 11.40, hs_goc_lidar = calculated_distance - 40; break;
				case 102: result_lidar = 11.45, hs_goc_lidar = calculated_distance - 40; break;
				case 103: result_lidar = 11.5, hs_goc_lidar = calculated_distance - 40; break;
				case 104: result_lidar = 11.55, hs_goc_lidar = calculated_distance - 40; break;
				case 105: result_lidar = 11.6, hs_goc_lidar = calculated_distance - 40; break;
				case 106: result_lidar = 11.65, hs_goc_lidar = calculated_distance - 40; break;
				case 107: result_lidar = 11.7, hs_goc_lidar = calculated_distance - 40; break;
				case 108: result_lidar = 11.75, hs_goc_lidar = calculated_distance - 40; break;
				case 109: result_lidar = 10.95, hs_goc_lidar = calculated_distance - 40; break;
			
			 //0.3
				case 110: result_lidar = 10.92, hs_goc_lidar = calculated_distance - 40; break;
				case 111: result_lidar = 10.89, hs_goc_lidar = calculated_distance - 40; break;
				case 112: result_lidar = 10.86, hs_goc_lidar = calculated_distance - 40; break;
				case 113: result_lidar = 10.83, hs_goc_lidar = calculated_distance - 40; break;
				case 114: result_lidar = 10.8, hs_goc_lidar = calculated_distance - 40; break;
				case 115: result_lidar = 10.89, hs_goc_lidar = calculated_distance - 40; break;
				case 116: result_lidar = 10.86, hs_goc_lidar = calculated_distance - 40; break;
				case 117: result_lidar = 10.83, hs_goc_lidar = calculated_distance - 40; break;
				case 118: result_lidar = 10.8, hs_goc_lidar = calculated_distance - 40; break;
				case 119: result_lidar = 10.77, hs_goc_lidar = calculated_distance - 40; break;
				case 120: result_lidar = 10.74, hs_goc_lidar = calculated_distance - 40; break;
				case 121: result_lidar = 10.71, hs_goc_lidar = calculated_distance - 40; break;
				case 122: result_lidar = 10.68, hs_goc_lidar = calculated_distance - 40; break;
				case 123: result_lidar = 10.65, hs_goc_lidar = calculated_distance - 40; break;
				case 124: result_lidar = 10.62, hs_goc_lidar = calculated_distance - 40; break;
				case 125: result_lidar = 10.59, hs_goc_lidar = calculated_distance - 40; break;
				case 126: result_lidar = 10.59, hs_goc_lidar = calculated_distance - 40; break;
				case 127: result_lidar = 10.53, hs_goc_lidar = calculated_distance - 40; break;
				case 128: result_lidar = 10.5, hs_goc_lidar = calculated_distance - 40; break;
				case 129: result_lidar = 10.47, hs_goc_lidar = calculated_distance - 40; break;
				case 130: result_lidar = 10.44, hs_goc_lidar = calculated_distance - 40; break;
				
				//0.2
				case 131: result_lidar = 10.41, hs_goc_lidar = calculated_distance - 40; break;
				case 132: result_lidar = 10.38, hs_goc_lidar = calculated_distance - 40; break;
				case 133: result_lidar = 10.36, hs_goc_lidar = calculated_distance - 40; break;
				case 134: result_lidar = 10.34, hs_goc_lidar = calculated_distance - 40; break;
				case 135: result_lidar = 10.32, hs_goc_lidar = calculated_distance - 40; break;
				case 136: result_lidar = 10.30, hs_goc_lidar = calculated_distance - 40; break;
				case 137: result_lidar = 10.28, hs_goc_lidar = calculated_distance - 40; break;
				case 138: result_lidar = 10.26, hs_goc_lidar = calculated_distance - 40; break;
				case 139: result_lidar = 10.24, hs_goc_lidar = calculated_distance - 40; break;
				case 140: result_lidar = 10.22, hs_goc_lidar = calculated_distance - 40; break;
				//0.1
				
				case 141: result_lidar = 11, hs_goc_lidar = calculated_distance - 23; break;
				case 142: result_lidar = 11.05, hs_goc_lidar = calculated_distance - 23; break;
				case 143: result_lidar = 11.1, hs_goc_lidar = calculated_distance - 23; break;
				case 144: result_lidar = 10, hs_goc_lidar = calculated_distance - 7; break;
				case 145: result_lidar = 9.95, hs_goc_lidar = calculated_distance - 7; break;
				case 146: result_lidar = 9.4, hs_goc_lidar = calculated_distance - 7; break;
				case 147: result_lidar = 9.37, hs_goc_lidar = calculated_distance - 7; break;
				case 148: result_lidar = 9.34, hs_goc_lidar = calculated_distance - 7; break;
				case 149: result_lidar = 9.3, hs_goc_lidar = calculated_distance - 7; break;
				case 166: result_lidar = 9.05, hs_goc_lidar = calculated_distance - 5; break;
				case 167: result_lidar = 9.05, hs_goc_lidar = calculated_distance - 5; break;
				case 168: result_lidar = 9.21, hs_goc_lidar = calculated_distance - 5; break;
				case 169: result_lidar = 9.205, hs_goc_lidar = calculated_distance - 5; break;
				
				// dang chinh luc gan het pin //
				case 170: result_lidar = 9.4, hs_goc_lidar = calculated_distance - 5; break;
				case 171: result_lidar = 9.365, hs_goc_lidar = calculated_distance - 5; break;
				case 172: result_lidar = 9.345, hs_goc_lidar = calculated_distance - 5; break;
				case 173: result_lidar = 9.33, hs_goc_lidar = calculated_distance - 5; break;
				case 174: result_lidar = 9.31, hs_goc_lidar = calculated_distance - 5; break;
				// ///////////////
				
				case 175: result_lidar = 9.25, hs_goc_lidar = calculated_distance - 5; break;
				case 176: result_lidar = 9.2105, hs_goc_lidar = calculated_distance - 6; break;
				case 179: result_lidar = 9.23, hs_goc_lidar = calculated_distance - 5; break;
				case 185: result_lidar = 9.36, hs_goc_lidar = calculated_distance - 20; break;
				case 186: result_lidar = 9.46, hs_goc_lidar = calculated_distance - 20; break;
				case 187: result_lidar = 9.56, hs_goc_lidar = calculated_distance - 20; break;
				case 188: result_lidar = 9.66, hs_goc_lidar = calculated_distance - 20; break;
				case 189: result_lidar = 9.76, hs_goc_lidar = calculated_distance - 20; break;
				case 190: result_lidar = 9.86, hs_goc_lidar = calculated_distance - 20; break;
				case 202: result_lidar = 9.08, hs_goc_lidar = calculated_distance - 5; break;
				case 203: result_lidar = 9.06, hs_goc_lidar = calculated_distance - 5; break;
				case 204: result_lidar = 9.04, hs_goc_lidar = calculated_distance - 5; break;
				case 205: result_lidar = 9.02, hs_goc_lidar = calculated_distance - 5; break;
				case 206: result_lidar = 8.9, hs_goc_lidar = calculated_distance - 5; break;
				case 207: result_lidar = 8.7, hs_goc_lidar = calculated_distance - 5; break;
				case 208: result_lidar = 8.4, hs_goc_lidar = calculated_distance - 5; break;

				
        default: result_lidar = 0; break; // Giá trị mặc định
    }
}

    final_result = force * (result)/10 ;
		final_result_lidar =calculated_force * (result_lidar)/10;
//		final_result_lidar =calculated_force ;
		 
		if (CB_NHAN_BONG_BO_BAN == 0 && shooting == 0) {
			if (L2) {
					taget_BT_Xoay = hs_goc;
					if(hs_goc<=571) XI_LANH_RE_RA;
    } else if (R2) {
        taget_BT_Xoay = hs_goc_lidar;
			if(hs_goc_lidar<=571) XI_LANH_RE_RA;
			else if (hs_goc_lidar == 0)   hs_goc_lidar= 600;
				

			
    }
}
					
}

void bantheokhoangcach()
{
	//lazer
if (L2 && CB_NHAN_BONG_BO_BAN == 0) {
        if (TOUCHPAD) { 
            shooting = 1; 
            robotStop(10); 
            Ban_1s = Ban_2 = Ban_3 = final_result;
						Ban_1_next, Ban_2_next, Ban_3_next;
            while (TOUCHPAD) {
                vTaskDelay(100); 
            }
            XI_LANH_DAY_BONG_RA;
            vTaskDelay(15000);
            Ban_1 = Ban_2 = Ban_3 = 0;
            XI_LANH_DAY_BONG_VAO;
            shooting = 0; 
        }
    }
//lidar
if (CB_NHAN_BONG_BO_BAN == 0) {
        if (TOUCHPAD) { 
            shooting = 1; 
            robotStop(10); 
            Ban_1 = Ban_2 = Ban_3 = final_result_lidar;
						Ban_1_next, Ban_2_next, Ban_3_next;
            while (TOUCHPAD) {
                vTaskDelay(100); 
            }
            XI_LANH_DAY_BONG_RA;
            vTaskDelay(15000);
            Ban_1 = Ban_2 = Ban_3 = 0;
            XI_LANH_DAY_BONG_VAO;
            shooting = 0; 
        }
    }		
}


//***************************xoay mam bang tay**************
#include <math.h> 

//////// Chỉnh thông số độ lệch /////////////
#define OFFSET_TOLERANCE 1
#define CENTER 100
#define MAX_SPEED 0.2
#define MIN_SPEED 0.14
#define Kp 0.02  // Hệ số P
#define Ki 0.0005 // Hệ số I
#define Kd 0.001  // Hệ số D

float prev_error = 0;
float integral = 0;

float pid_control(int received_offset) {
    float error = received_offset - CENTER;
    integral += error;
    float derivative = error - prev_error;
    prev_error = error;

    // Tính toán tốc độ dựa trên PID
    float speed = Kp * error + Ki * integral + Kd * derivative;

    // Giới hạn tốc độ trong khoảng MIN_SPEED đến MAX_SPEED
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    if (speed < MIN_SPEED && speed > 0) speed = MIN_SPEED;
    if (speed < -MAX_SPEED) speed = -MAX_SPEED;
    if (speed > -MIN_SPEED && speed < 0) speed = -MIN_SPEED;

    return speed;
}

void xoaytam(int received_offset) { 
    float received_speed = pid_control(received_offset);

    if (R2) {
        if (abs(received_offset - CENTER) <= OFFSET_TOLERANCE) {
            robotStop(0);  // Dừng khi đến tâm
        } 
        else if (received_offset < CENTER - OFFSET_TOLERANCE) {
            robotRotateFree(-received_speed, 0);  // Xoay phải
        } 
        else if (received_offset > CENTER + OFFSET_TOLERANCE) {
            robotRotateFree(received_speed, 0);   // Xoay trái
        }
    }
}


void LED_CHECK(int received_offset) {

		    if (received_offset < CENTER - OFFSET_TOLERANCE -2) {
						LED_TAM_LECH;
        } else if (received_offset > CENTER + OFFSET_TOLERANCE +2) {
						LED_TAM_LECH;
        } else {
            LED_TAM_CHUAN;	
        }
			}

static int last_state_kep = 0; 
static int last_state_nang = 0; 
//void xl_nang_kep_bong()
//{
////   if (L2 && last_state_nang == 0) { 
////        if (xl_nangkep_state == 0) {
////					  XI_LANH_DAY_BONG_RA;
////        xl_nangkep_state = 1;
////        } else {
////					  XI_LANH_DAY_BONG_VAO;
////        xl_nangkep_state = 0;
////        }
////    }
////    last_state_nang = L2; 
//		
//		
//		if (TOUCHPAD && last_state_kep == 0) { 
//        if (xl_kepbong_state == 0) {
//					

//            XI_LANH_KEP_BONG_ON;
//            xl_kepbong_state = 1;
//        } else {
//            XI_LANH_KEP_BONG_OFF;
//            xl_kepbong_state = 0;
//        }
//    }
//    last_state_kep = TOUCHPAD; // C?p nh?t tr?ng th�i tru?c d�
//}
#define MOCCU 699       
#define LUC_TAI_MOC 115
#define GOC_TAI_MOC 610 //415

#define LUC_MAX 250
#define LUC_MIN 50

#define GOC_MAX 650    //450
#define GOC_MIN 550    //350

#define LASER_HEIGHT_DIFF 80

#define LASER_ANGLE 0.2    

void tinhLucBan(float lazeTraiValue) {
    float gocBan = 0;
    float lucBan = 0;
    float corrected_distance = sqrt((lazeTraiValue * lazeTraiValue) + (LASER_HEIGHT_DIFF * LASER_HEIGHT_DIFF));
    
    lucBan = LUC_TAI_MOC * (corrected_distance / MOCCU);

    if (lucBan > LUC_MAX) {
        lucBan = LUC_MAX;
    } else if (lucBan < LUC_MIN) {
        lucBan = LUC_MIN;
    }

//    if (corrected_distance >= 460) {
//        lucBan += 10;
//    }
    gocBan = GOC_TAI_MOC + ((corrected_distance - MOCCU) / MOCCU) * (GOC_MAX - GOC_MIN);

    if (gocBan > GOC_MAX) {
        gocBan = GOC_MAX;
    } else if (gocBan < GOC_MIN) {
        gocBan = GOC_MIN;
    }

    force = lucBan;
    degree_F = gocBan;
}


#define MOCCHUAN 4090  
#define LUC_MOC 106
#define GOC_MOC 600

#define LUC_MAX1 250
#define LUC_MIN1 50

#define GOC_MAX1 600    
#define GOC_MIN1 550    

#define LUC_BETA 1.1  // Hệ số phi tuyến cho lực bắn
#define GOC_ALPHA 0.7  // Hệ số hiệu chỉnh góc bắn

void tinhlucban_lidar(float received_distance) {
    float gocBanlidar = 0;
    float lucBanlidar = 0;
    // Tính lực bắn với công thức phi tuyến
    lucBanlidar = LUC_MOC * pow((received_distance / MOCCHUAN), LUC_BETA);

    // Giới hạn lực bắn trong khoảng cho phép
    if (lucBanlidar > LUC_MAX1) {
        lucBanlidar = LUC_MAX1;
    } else if (lucBanlidar < LUC_MIN1) {
        lucBanlidar = LUC_MIN1;
    }
    // Tính góc bắn với hệ số hiệu chỉnh và khoảng động
    gocBanlidar = GOC_MOC + ((received_distance - MOCCHUAN) / MOCCHUAN) * (GOC_MAX1 - GOC_MIN1) * GOC_ALPHA;

    // Giới hạn góc bắn trong khoảng cho phép
    if (gocBanlidar > GOC_MAX1) {
        gocBanlidar = GOC_MAX1;
    } else if (gocBanlidar < GOC_MIN1) {
        gocBanlidar = GOC_MIN1;
    }
    calculated_force = lucBanlidar;
    calculated_distance = gocBanlidar;
}


//-----------------------------------------RESET CAC CO CAU----------------------------------------------------
//---------------------------------------NANG LUA---------------------------------------------------------------
//void giu_Tay_Nang_Lua()
//{
//	if(xl_nang_lua == 0 ){
//		XI_LANH_NANG_LUA_XUONG_ON;
//		XI_LANH_NANG_LUA_LEN_OFF;
//	}
//	else if (xl_nang_lua == 1) {
//		XI_LANH_NANG_LUA_XUONG_OFF;
//		XI_LANH_NANG_LUA_LEN_ON;
//	}
//	else{
//		XI_LANH_NANG_LUA_XUONG_OFF;
//		XI_LANH_NANG_LUA_LEN_OFF;
//	}
//	
//	if (CB_XL_NANG_LUA_1 == 0 && vt_xl_nang_lua == 1)
//	{
//		xl_nang_lua = 2;
//	}
//	if (CB_XL_NANG_LUA_2 == 0 && vt_xl_nang_lua == 2)
//	{
//		xl_nang_lua = 2;
//	}
//	if (CB_XL_NANG_LUA_3 == 0 && vt_xl_nang_lua == 3)
//	{
//		xl_nang_lua = 2;
//	}
//	if (CB_XL_NANG_LUA_4 == 0 && vt_xl_nang_lua == 4)
//	{
//		xl_nang_lua = 2;
//	}
//}

//void Nang_ha_tay_lua(void)
//{
//	
//	int count = 0;
//	while (1)
//	{
//		for (i = 0; i < 50; i++)
//		{
//			if (!L2)
//			{
//				count++;
//			}
//		}

//		if (count == 1)
//		{
//			vt_xl_nang_lua = 3;
//			xl_nang_lua = 0;
//		}

//		if (count == 2)
//		{
//			vt_xl_nang_lua = 2;
//			xl_nang_lua = 1;
//		}
//		if (count == 3)
//		{
//			vt_xl_nang_lua = 3;
//			xl_nang_lua = 1;
//		}
//		if (count == 4)
//		{
//			vt_xl_nang_lua = 4;
//			xl_nang_lua = 1;
//		}
//		if (count == 5)
//		{
//			vt_xl_nang_lua = 1;
//			xl_nang_lua = 0;
//		}
//		if (wantExit() == 1)
//			break;
//		
//	}
//}
//-----------------------------------------RESET MAM XOAY-----------------------------------------------------
void Giu_Nong_Ban()
{
//		if(BienTroNongBanValue < taget_BT_Xoay - 5 ){
//			Cylinder = 0;
//			return;
//		}

	if (taget_BT_Xoay < Min_BT_Xoay)
		taget_BT_Xoay = Min_BT_Xoay;
	if (taget_BT_Xoay > Max_BT_Xoay)
		taget_BT_Xoay = Max_BT_Xoay;

	if (abs(BienTroNongBanValue - taget_BT_Xoay) > 1)
	{
		if (BienTroNongBanValue > taget_BT_Xoay)
			Cylinder_back;
		else
			Cylinder_next;

		speed_temp = abs(BienTroNongBanValue - taget_BT_Xoay)*5;
		if (speed_temp > 250) // 50
			speed_temp = 250;
		if (speed_temp < 50) 
			speed_temp = 50;

		Cylinder = speed_temp;
	}
	else
		Cylinder = 0;
}

void Nang_Ha_Nong_Ban(void)
{
	if (R1 && X && (taget_BT_Xoay < Max_BT_Xoay))
		taget_BT_Xoay++, vTaskDelay(300);
	else if (R1 && TRIANGLE && (taget_BT_Xoay > Min_BT_Xoay))
		taget_BT_Xoay--, vTaskDelay(400);
}

////---------------------------------------GIU TAY BONG---------------------------------------------------------------
//void Giu_tay_lay_bong(void)
//{
//	//	if(bientronangbongValue < Min_BT_Nang_Bong - 12 || bientronangluaValue > Max_BT_Nang_Bong + 12)  {Mor_bong = 2; return;}
//	//	if(abs(bientronangbongValue - taget_BT_Nang_Bong) > 3){
//	//		if(bientronangbongValue > taget_BT_Nang_Bong) Mor_bong_vao;
//	//		else Mor_bong_ra;
//	//
//	//		speed_temp = abs(bientronangbongValue - taget_BT_Nang_Bong)*10;
//	//
//	//		if(speed_temp > 50) speed_temp = 50;
//	//		if(speed_temp < 10) speed_temp = 10;
//	//	  Mor_bong = speed_temp;
//	//	}
//	//	else Mor_bong = 2;
//	if (taget_BT_Nang_Bong < Min_BT_Nang_Bong)
//		taget_BT_Nang_Bong = Min_BT_Nang_Bong;
//	if (taget_BT_Nang_Bong > Max_BT_Nang_Bong)
//		taget_BT_Nang_Bong = Max_BT_Nang_Bong;

//	if (bientronangbongValue < Min_BT_Nang_Bong)
//	{
//		Mor_bong_ra;
//		Mor_bong = 10;
//		return;
//	}
//	if (bientronangbongValue > Max_BT_Nang_Bong)
//	{
//		Mor_bong_vao;
//		Mor_bong = 10;
//		return;
//	}

//	if (abs(bientronangbongValue - taget_BT_Nang_Bong) > 7)
//	{
//		if (bientronangbongValue > taget_BT_Nang_Bong)
//			Mor_bong_vao;
//		else
//			Mor_bong_ra;

//		speed_temp = abs(bientronangbongValue - taget_BT_Nang_Bong) * 8;//8
//		if (speed_temp > 220) // 50
//			speed_temp = 220;
//		if (speed_temp < 20)
//			speed_temp = 20;

//		Mor_bong = speed_temp;
//	}
//	else
//		Mor_bong = 2;
//}

//void Ra_vao_lay_bong(void)
//{
//	if ((RJOY_UD > 250) && (taget_BT_Nang_Bong < Max_BT_Nang_Bong))
//		taget_BT_Nang_Bong++, vTaskDelay(100);
//	else if ((RJOY_UD < 5) && (taget_BT_Nang_Bong > Min_BT_Nang_Bong))
//		taget_BT_Nang_Bong--, vTaskDelay(100);
//}

//// void bam_Thanh_Lazer_Phai(int speed,int angle_fix,int AngleHead,int lazer_stable_phai,int num_change_stable){
////		if(lazePhaiValue - lazer_stable_phai > num_change_stable){
////			robotRunAngle(0 + angle_fix,speed,AngleHead,-0.1);
////		}
////		else if(lazePhaiValue - lazer_stable_phai < num_change_stable){
////			robotRunAngle(0 - angle_fix,speed,AngleHead,-0.1);
////		}
////		else robotRunAngle(0,speed,AngleHead,0.1);
//// }



//void bam_Thanh_Lazer_Truoc(int speed, int angle_fix, int AngleHead, int lazer_stable_truoc, int num_change_stable)
//{
//	if (lazeTruocValue - lazer_stable_truoc > num_change_stable)
//	{
//		robotRunAngle(900 + angle_fix, speed, AngleHead, 0.2);
//	}
//	else if (lazeTruocValue - lazer_stable_truoc < num_change_stable)
//	{
//		robotRunAngle(900 - angle_fix, speed, AngleHead, 0.2);
//	}
//	else
//		robotRunAngle(900, speed, AngleHead, 0.2);
//}
//void bam_Thanh_Lazer_Truoc_san2do(int speed, int AngleHead, int lazer_stable_truoc, int num_change_stable, int fix_max)
//{
//	int fix;

//	fix = abs(lazeTruocValue - lazer_stable_truoc) * 12;

//	if (fix > fix_max)
//		fix = fix_max;

//	if (lazeTruocValue - lazer_stable_truoc > num_change_stable)
//	{
//		robotRunAngle(-900 - fix, speed, AngleHead, 0.1);
//	}
//	else if (lazeTruocValue - lazer_stable_truoc < - num_change_stable)
//	{
//		robotRunAngle(-900 + fix, speed, AngleHead, 0.1);
//	}
//	else
//		robotRunAngle(-900, speed, AngleHead, 0.1);
//}


//void bam_Thanh_Lazer_Truoc_xanh(int speed, int angle_fix, int AngleHead, int lazer_stable_truoc, int num_change_stable)
//{
//	if (lazeSauValue - lazer_stable_truoc > num_change_stable)
//	{
//		robotRunAngle(-900 - angle_fix, speed, AngleHead, 0.3);
//	}
//	else if (lazeSauValue - lazer_stable_truoc < num_change_stable)
//	{
//		robotRunAngle(-900 + angle_fix, speed, AngleHead, 0.3);
//	}
//	else
//		robotRunAngle(-900, speed, AngleHead, 0.3);
//}
////-------------------------------------bam laze moi-----------------------------------------------
//void bam_Thanh_Lazer_Truoc_do(int speed, int AngleHead, int lazer_stable_truoc, int num_change_stable, int fix_max)
//{
//	int fix;

//	fix = abs(lazeTruocValue - lazer_stable_truoc) * 12;

//	if (fix > fix_max)
//		fix = fix_max;

//	if (lazeTruocValue - lazer_stable_truoc > num_change_stable)
//	{
//		robotRunAngle(900 + fix, speed, AngleHead, 0.1);
//	}
//	else if (lazeTruocValue - lazer_stable_truoc < - num_change_stable)
//	{
//		robotRunAngle(900 - fix, speed, AngleHead, 0.1);
//	}
//	else
//		robotRunAngle(900, speed, AngleHead, 0.1);
//}
//void Bam_thanh_laser_phai(int speed, int AngleHead, int lazer_stable_phai, int num_change_stable, int fix_max)
//{
//	int fix;

//	fix = abs(lazePhaiValue - lazer_stable_phai) * 12;

//	if (fix > fix_max)
//		fix = fix_max;

//	if (lazePhaiValue - lazer_stable_phai > num_change_stable)
//	{
//		robotRunAngle(0 + fix, speed, AngleHead, 0.1);
//	}
//	else if (lazePhaiValue - lazer_stable_phai < - num_change_stable)
//	{
//		robotRunAngle(0 - fix, speed, AngleHead, 0.1);
//	}
//	else
//		robotRunAngle(0, speed, AngleHead, 0.1);
//}

//void Bam_thanh_laser_trai(int speed, int AngleHead, int lazer_stable_trai, int num_change_stable, int fix_max)
//{
//	int fix;

//	fix = abs(lazeTraiValue - lazer_stable_trai) * 12;

//	if (fix > fix_max)
//		fix = fix_max;

//	if (lazeTraiValue - lazer_stable_trai > num_change_stable)
//	{
//		robotRunAngle(0 - fix, speed, AngleHead, 0.1);
//	}
//	else if (lazeTraiValue - lazer_stable_trai < - num_change_stable)
//	{
//		robotRunAngle(0 + fix, speed, AngleHead, 0.1);
//	}
//	else
//		robotRunAngle(0, speed, AngleHead, 0.1);
//}

////-------------------------------------bam laze moi-----------------------------------------------
//void Bam_thanh_laser_phai_1800(int speed, int AngleHead, int lazer_stable_phai, int num_change_stable, int fix_max,int limit)
//{
//	int fix;

//	fix = absI(lazePhaiValue - lazer_stable_phai) * 10;

//	if (fix > fix_max)
//		fix = fix_max;
//	if (lazePhaiValue > limit)
//	{
//		if (lazePhaiValue - lazer_stable_phai > num_change_stable)
//		{
//			robotRunAngle(1800 - fix, speed, AngleHead, 0.2);
//		}
//		else if (lazePhaiValue - lazer_stable_phai < - num_change_stable)
//		{
//			robotRunAngle(-1800 + fix, speed, AngleHead, 0.2);
//		}
//		else
//			robotRunAngle(1800, speed, AngleHead, 0.2);
//	}
//	else	robotRunAngle(1800, speed, AngleHead, 0.2);
//}

//void Bam_thanh_laser_trai_1800(int speed, int AngleHead, int lazer_stable_trai, int num_change_stable, int fix_max, int limit)
//{
//	int fix = absI((lazeTraiValue - lazer_stable_trai) * 10);

//	if (fix > fix_max)	fix = fix_max;

//	if (lazeTraiValue > limit)
//	{
//		if (lazeTraiValue - lazer_stable_trai > num_change_stable)
//		{
//			robotRunAngle(-1800 + fix, speed, AngleHead, 0.2);
//		}
//		else if (lazeTraiValue - lazer_stable_trai < -num_change_stable)
//		{
//			robotRunAngle(1800 - fix, speed, AngleHead, 0.2);
//		}
//		else
//			robotRunAngle(1800, speed, AngleHead, 0.2);
//	}
//	else
//			robotRunAngle(1800, speed, AngleHead, 0.2);

////	if(fix < num_change_stable)
////		robotRunAngle(0, speed, AngleHead, 0.2);
////	else if (lazeTraiValue > lazer_stable_trai)
////		robotRunAngle(fix, speed, AngleHead, 0.2);
////	else 
////		robotRunAngle(-fix, speed, AngleHead, 0.2);

//}

////void Bam_thanh_laser_trai(int speed, int AngleHead, int lazer_stable_trai, int num_change_stable)
////{
////	int fix = absI((lazeTraiValue - lazer_stable_trai) * 15);

////	if (fix > 600) fix = 600;

////	if(fix < num_change_stable)
////		robotRunAngle(0, speed, AngleHead, 0.1);
////	else if (lazeTraiValue > lazer_stable_trai)
////		robotRunAngle(fix, speed, AngleHead, 0.1);
////	else 
////		robotRunAngle(-fix, speed, AngleHead, 0.1);

////}



//// -----------------------------
////int go_To_Trai(int speed, int laser_phai)
////{

////	for (i = 0; i < 50; i++)
////		while (lazePhaiValue < laser_phai)
////		{
////			robotLineRunLeft(speed);
////			if (wantExit() == 1)
////				break;
////			vTaskDelay(1);
////		}
////	robotStop(0);
////}
////int go_To_Phai(int speed, int laser_phai)
////{

////	for (i = 0; i < 50; i++)
////		while (lazePhaiValue > laser_phai)
////		{
////			robotLineRunRight(speed);
////			if (wantExit() == 1)
////				break;
////			vTaskDelay(1);
////		}
////	robotStop(0);
////}
////------------------------------------

//// Robot chay den vi tri bong
//int mapSan2Do[] = {305, 255, 205, 153, 105, 53};

//void chuyen_Bong_do(void)
//{
//	int speed = 0;
//	int speed_max = 70;
//	int speed_mid = 50;
//	int speed_min = 20;
//	int gap = 1;
//	if (vi_tri_bong >= 0 && vi_tri_bong <= 5)
//	{
//		while (absI(lazePhaiValue - mapSan2Do[vi_tri_bong]) > gap)
//		{
//			// tu chinh toc do
//			speed = absI(mapSan2Do[vi_tri_bong] - lazePhaiValue);
//			
//			if(speed > speed_max)	speed = speed_max;
//			else if(speed < speed_min)	speed = speed_min;
//			else speed = speed;
////			if (speed >= speed_max)
////			{
////				speed = speed_max;
////			}
////			else if (speed > speed_min){
////				speed = speed_mid;
////			}
////			else if (speed < speed_min)
////			{
////				speed = speed_min;
////			}

//			// tu bam lazer
//			if (lazePhaiValue < mapSan2Do[vi_tri_bong] + gap)
//			{
//				robotLineRunLeft(speed);
//			}
//			else if (lazePhaiValue > mapSan2Do[vi_tri_bong] - gap)
//			{
//				robotLineRunRight(speed);
//			}
//			else
//			{
//				robotStop(10);
//			}
//			if (wantExit())
//				break;
//		}
//	}
//	return;
//}
////------------------------chuyen_Bong_Xanh----------------------------
//int mapSan2Xanh[] = {304, 254, 206, 155, 105, 55};
//void chuyen_Bong_xanh(void)
//{
//	int speed = 0;
//	int speed_max = 60;
//	int speed_mid = 40;
//	int speed_min = 20;
//	int gap = 1;
//	if (vi_tri_bong >= 0 && vi_tri_bong <= 5)
//	{
//		while (absI(lazeTraiValue - mapSan2Xanh[vi_tri_bong]) > gap)
//		{
//			// tu chinh toc do
//			speed = absI(mapSan2Xanh[vi_tri_bong] - lazeTraiValue);
//			
//			if(speed > speed_max)	speed = speed_max;
//			else if(speed < speed_min)	speed = speed_min;
//			else speed = speed;
////			if (speed >= speed_max)
////			{
////				speed = speed_max;
////			}
////			else if (speed > speed_min){
////				speed = speed_mid;
////			}
////			else if (speed < speed_min)
////			{
////				speed = speed_min;
////			}

//			// tu bam lazer
//			if (lazeTraiValue < mapSan2Xanh[vi_tri_bong] + gap)
//			{
//				robotLineRunRight(speed);
//			}
//			else if (lazeTraiValue > mapSan2Xanh[vi_tri_bong] - gap)
//			{
//				robotLineRunLeft(speed);
//			}
//			else
//			{
//				robotStop(100);
//			}
//			if (wantExit())
//				break;
//		}
//	}
//	return;
//}
//void test_cylinder(void)
//{
//	//***********************************************************************************
//	if (!L2)
//	{
//		XI_LANH_KEP_BONG_OFF;
//	}
//	else
//	{
//		XI_LANH_KEP_BONG_ON;
//	}
//}

//void GAP_BONG_TRONG(void)
//{
//	//***************ban trong thoc********************
//	if (!TRIANGLE && R1)
//	{
//		update = 0;
//		XI_LANH_TONG_ON;
//		XI_LANH_DAY_BONG_VAO;
//		for (i = 0; i < 50; i++){
//			while (CB_XL_DAY_BONG_VAO == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		XI_LANH_LAY_BONG_LEN;
//		for (i = 0; i < 50; i++){
//			while (CB_TAY_GAP_BONG_DUOI == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		XI_LANH_KEP_BONG_OFF;
//		taget_BT_Nang_Bong = BT_Bat_Bong_giua ;
//		for (i = 0; i < 50; i++)
//			while (bientronangbongValue < BT_Bat_Bong_giua - 8 )
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			};
//		vTaskDelay(5000);
//		XI_LANH_LAY_BONG_XUONG;
//		for (i = 0; i < 50; i++){
//			while (CB_TAY_GAP_BONG_TREN == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//		vTaskDelay(4);
//		}
//		XI_LANH_KEP_BONG_ON;
//		vTaskDelay(3000);
//			
//		Ban_Bong_Thoc();
//		XI_LANH_LAY_BONG_LEN;
//		if (CHON_SAN == 1)
//		{
//			Quay_Mam_Gamepad_Xanh();
//		}
//		else
//		{
//			Quay_Mam_Gamepad();
//		}
//		for (i = 0; i < 50; i++){
//			while (CB_TAY_GAP_BONG_DUOI == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		taget_BT_Nang_Bong = BT_Nang_Tha_Bong;
//		XI_LANH_DAY_BONG_VAO;
//		for (i = 0; i < 50; i++)
//			while (bientronangbongValue < BT_Nang_Tha_Bong - 5)
//			{
//				vTaskDelay(1);
//			};
//		while (absI(BienTroMamXoayValue - taget_BT_Xoay) > 1);
//		XI_LANH_KEP_BONG_OFF;
//		vTaskDelay(10000);
//		while (RJOY)
//			vTaskDelay(1);
//		Mor_BanDuoi = 0;
//		Mor_BanTren = 0;
//		// vTaskDelay(1500);
//		taget_BT_Xoay = goc_ban;
//		taget_BT_Nang_Bong = BT_Bat_Bong_giua;

//		update = 1;
//	}
//	// --------------------------------ban trong lep-----------
//	if (!TRIANGLE && !R1)
//	{
//		update = 0;
//		XI_LANH_TONG_ON;
//		XI_LANH_DAY_BONG_VAO;
//		for (i = 0; i < 50; i++){
//			while (CB_XL_DAY_BONG_VAO == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		XI_LANH_LAY_BONG_LEN;
//		for (i = 0; i < 50; i++)
//			while (CB_TAY_GAP_BONG_DUOI == 1){
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		XI_LANH_KEP_BONG_OFF;
//		taget_BT_Nang_Bong = BT_Bat_Bong_giua ;
//		for (i = 0; i < 50; i++)
//			while (bientronangbongValue < BT_Bat_Bong_giua - 8)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			};
//		vTaskDelay(5000);
//		XI_LANH_LAY_BONG_XUONG;
//		for (i = 0; i < 50; i++)
//			while (CB_TAY_GAP_BONG_TREN == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//		XI_LANH_KEP_BONG_ON;
//		vTaskDelay(3000);
//			
//		Ban_Bong_Lep();
//		XI_LANH_LAY_BONG_LEN;
//		if (CHON_SAN == 1)
//		{
//			Quay_Mam_Gamepad_Tim_Xanh();
//		}
//		else
//		{
//			Quay_Mam_Gamepad_Tim_Do();
//		}
//		for (i = 0; i < 50; i++){
//			while (CB_TAY_GAP_BONG_DUOI == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		taget_BT_Nang_Bong = BT_Nang_Tha_Bong;
//		XI_LANH_DAY_BONG_VAO;

//		for (i = 0; i < 50; i++){
//			while (bientronangbongValue < BT_Nang_Tha_Bong - 8)
//			{
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		while (absI(BienTroMamXoayValue - taget_BT_Xoay) > 1);
//		XI_LANH_KEP_BONG_OFF;
//		vTaskDelay(10000);
//		while (RJOY)
//		vTaskDelay(1);
//		Mor_BanDuoi = 0;
//		Mor_BanTren = 0;
//		// vTaskDelay(1500);
//		taget_BT_Xoay = goc_ban;
//		taget_BT_Nang_Bong = BT_Bat_Bong_giua;

//		update = 1;
//	}
//}
////----------------------------------------------LAY BONG NGOAI---------------------------------------
//void GAP_BONG_NGOAI(void)
//{
//	// --------------------------------ban Ngoai thoc-----------
//	if (!X && R1 )
//	{
//		update = 0;
//		XI_LANH_DAY_BONG_VAO;
//		XI_LANH_TONG_ON;
//		XI_LANH_LAY_BONG_LEN;
//		for (i = 0; i < 50; i++){
//			while (CB_TAY_GAP_BONG_DUOI == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		XI_LANH_KEP_BONG_ON;
//		taget_BT_Nang_Bong = BT_Bat_Bong_sau;
//		XI_LANH_DAY_BONG_RA;
//		for (i = 0; i < 50; i++){
//			while (CB_XL_DAY_BONG_RA == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		for (i = 0; i < 50; i++)
//			while (bientronangbongValue < BT_Bat_Bong_sau - 5)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			};
//		vTaskDelay(3000);
//		XI_LANH_KEP_BONG_OFF;
//		XI_LANH_LAY_BONG_XUONG;
//		for (i = 0; i < 50; i++){
//			while (CB_TAY_GAP_BONG_TREN == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		XI_LANH_KEP_BONG_ON;
//		vTaskDelay(3000);
//		XI_LANH_LAY_BONG_LEN;
//		
//		for (i = 0; i < 50; i++){
//			while (CB_TAY_GAP_BONG_DUOI == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		if (CHON_SAN == 1)
//		{
//			Quay_Mam_Gamepad_Xanh();
//		}
//		else
//		{
//			Quay_Mam_Gamepad();
//		}
//		taget_BT_Nang_Bong = BT_Nang_Tha_Bong;
//		Ban_Bong_Thoc();
//		XI_LANH_DAY_BONG_VAO;
//		for (i = 0; i < 50; i++){
//			while (CB_XL_DAY_BONG_VAO == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		// tu quay mam
//		
//		for (i = 0; i < 50; i++)
//			while (bientronangbongValue < BT_Nang_Tha_Bong - 8)
//			{
//				vTaskDelay(1);
//			};
//		while (absI(BienTroMamXoayValue - taget_BT_Xoay) > 1);
//		XI_LANH_KEP_BONG_OFF;
//		vTaskDelay(10000);
//		while (RJOY)
//			vTaskDelay(1);
//		Mor_BanDuoi = 0;
//		Mor_BanTren = 0;
//		taget_BT_Xoay = goc_ban;
//		taget_BT_Nang_Bong = BT_Bat_Bong_giua;
//		update = 1;
//	}
//	// --------------------------------ban ngoai lep-----------
//	if (!X && !R1)
//	{
//		update = 0;
//		XI_LANH_DAY_BONG_VAO;
//		XI_LANH_TONG_ON;
//		XI_LANH_LAY_BONG_LEN;
//		for (i = 0; i < 50; i++){
//			while (CB_TAY_GAP_BONG_DUOI == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		XI_LANH_KEP_BONG_ON;
//		taget_BT_Nang_Bong = BT_Bat_Bong_sau;
//		XI_LANH_DAY_BONG_RA;
//		for (i = 0; i < 50; i++){
//			while (CB_XL_DAY_BONG_RA == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		for (i = 0; i < 50; i++)
//			while (bientronangbongValue < BT_Bat_Bong_sau - 5)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			};
//		vTaskDelay(3000);
//		XI_LANH_KEP_BONG_OFF;
//		XI_LANH_LAY_BONG_XUONG;
//		for (i = 0; i < 50; i++){
//			while (CB_TAY_GAP_BONG_TREN == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		XI_LANH_KEP_BONG_ON;
//		vTaskDelay(3000);
//		XI_LANH_LAY_BONG_LEN;
//		
//		for (i = 0; i < 50; i++){
//			while (CB_TAY_GAP_BONG_DUOI == 1)
//			{
//				if (wantExit() == 1)
//					break;
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		XI_LANH_DAY_BONG_VAO;	
//		if (CHON_SAN == 1)
//		{
//			Quay_Mam_Gamepad_Tim_Xanh();
//		}
//		else
//		{
//			Quay_Mam_Gamepad_Tim_Do();
//		}
//		
//		taget_BT_Nang_Bong = BT_Nang_Tha_Bong;
//		Ban_Bong_Lep();
//		
//		for (i = 0; i < 50; i++){
//			while (CB_XL_DAY_BONG_VAO == 1)
//			{
//				vTaskDelay(1);
//			}
//			vTaskDelay(4);
//		}
//		// tu quay mam
//		
//		for (i = 0; i < 50; i++)
//			while (bientronangbongValue < BT_Nang_Tha_Bong - 8)
//			{
//				vTaskDelay(1);
//			};
//		while (absI(BienTroMamXoayValue - taget_BT_Xoay) > 1);
//		XI_LANH_KEP_BONG_OFF;
//		vTaskDelay(10000);
//		while (RJOY)
//			vTaskDelay(1);
//		Mor_BanDuoi = 0;
//		Mor_BanTren = 0;
//		taget_BT_Xoay = goc_ban;
//		taget_BT_Nang_Bong = BT_Bat_Bong_giua;
//		update = 1;
//	}
//}

////********************************* doc cam bien line **************************************8
//void Doc_cb_line(void)
//{
//	if (!CB_Line_P1 && CB_Line_P2 && CB_Line_P3 && CB_Line_P4)
//		CB_line_trai = 3;
//	else if (!CB_Line_P1 && !CB_Line_P2 && CB_Line_P3 && CB_Line_P4)
//		CB_line_trai = 2;
//	else if (CB_Line_P1 && !CB_Line_P2 && CB_Line_P3 && CB_Line_P4)
//		CB_line_trai = 1;

//	else if (CB_Line_P1 && !CB_Line_P2 && !CB_Line_P3 && CB_Line_P4)
//		CB_line_trai = 0;

//	else if (CB_Line_P1 && CB_Line_P2 && !CB_Line_P3 && CB_Line_P4)
//		CB_line_trai = -1;
//	else if (CB_Line_P1 && CB_Line_P2 && !CB_Line_P3 && !CB_Line_P4)
//		CB_line_trai = -2;
//	else if (CB_Line_P1 && CB_Line_P2 && CB_Line_P3 && !CB_Line_P4)
//		CB_line_trai = -3;

//	if (!CB_Line_T1 && CB_Line_T2 && CB_Line_T3 && CB_Line_T4)
//		CB_line_phai = 3;
//	else if (!CB_Line_T1 && !CB_Line_T2 && CB_Line_T3 && CB_Line_T4)
//		CB_line_phai = 2;
//	else if (CB_Line_T1 && !CB_Line_T2 && CB_Line_T3 && CB_Line_T4)
//		CB_line_phai = 1;
//	else if (CB_Line_T1 && !CB_Line_T2 && !CB_Line_T3 && CB_Line_T4)
//		CB_line_phai = 0;
//	else if (CB_Line_T1 && CB_Line_T2 && !CB_Line_T3 && CB_Line_T4)
//		CB_line_phai = -1;
//	else if (CB_Line_T1 && CB_Line_T2 && !CB_Line_T3 && !CB_Line_T4)
//		CB_line_phai = -2;
//	else if (CB_Line_T1 && CB_Line_T2 && CB_Line_T3 && !CB_Line_T4)
//		CB_line_phai = -3;
//}

//void robotLineRunLeft(vu8 speed)
//{
//	if (CB_line_trai > -1 && CB_line_trai < 1 && CB_line_phai > -1 && CB_line_phai < 1)
//		robotRun(-900, speed);
//	else if (CB_line_trai <= -1)
//		robotCurve(-900 - 30 * abs(CB_line_trai), speed, -0.05 * abs(CB_line_trai));
//	else if (CB_line_trai >= 1)
//		robotCurve(-900 + 30 * abs(CB_line_trai), speed, 0.05 * abs(CB_line_trai));
//	else if (CB_line_phai <= -1)
//		robotCurve(-900 - 30 * abs(CB_line_trai), speed, -0.05 * abs(CB_line_trai));
//	else if (CB_line_phai >= 1)
//		robotCurve(-900 + 30 * abs(CB_line_trai), speed, 0.05 * abs(CB_line_trai));
//}

//void robotLineRunRight(vu8 speed)
//{
//	if (CB_line_trai > -1 && CB_line_trai < 1 && CB_line_phai > -1 && CB_line_phai < 1)
//		robotRun(900, speed);
//	else if (CB_line_trai <= -1)
//		robotCurve(900 + 30 * abs(CB_line_phai), speed, 0.03 * abs(CB_line_phai));
//	else if (CB_line_trai >= 1)
//		robotCurve(900 - 30 * abs(CB_line_phai), speed, -0.03 * abs(CB_line_phai));
//	else if (CB_line_phai <= -1)
//		robotCurve(900 + 30 * abs(CB_line_phai), speed, 0.03 * abs(CB_line_phai));
//	else if (CB_line_phai >= 1)
//		robotCurve(900 - 30 * abs(CB_line_phai), speed, -0.03 * abs(CB_line_phai));
//}

//// tu chinh goc mam
//int mamArray[] = {520 ,540, 570, 580, 605, 630}; //515,535, 570, 595, 608, 635
//int Quay_Mam_Gamepad()
//{
//	taget_BT_Xoay = mamArray[vi_tri_bong];
//	return 1;
//}

//int mamArrayTimdo[] = {430, 455, 490, 510, 545, 570};
//int Quay_Mam_Gamepad_Tim_Do()
//{
//	taget_BT_Xoay = mamArrayTimdo[vi_tri_bong];
//	return 1;
//}
//int mamArrayXanh[] = {370, 355, 325, 305, 270 , 260};
//int Quay_Mam_Gamepad_Xanh()
//{

//	taget_BT_Xoay = mamArrayXanh[vi_tri_bong];
//	robotStop(1);
//	return 1;
//}
//int mamArrayTimXanh[] = {440, 430, 410, 370, 340, 320};
//int Quay_Mam_Gamepad_Tim_Xanh()
//{
//	taget_BT_Xoay = mamArrayTimXanh[vi_tri_bong];
//	return 1;
//}

////---------------------------------------toc do ban bong------------------
//int BanThocArray[6][2] = {{8, 220}, {8, 225}, {9, 230}, {12, 225}, {12, 235}, {12, 245}}; // -------mor tren,mor duoi/
//int Ban_Bong_Thoc()
//{
//	Mor_BanTren = BanThocArray[vi_tri_bong][0];
//	Mor_BanTren_nghich;
//	Mor_BanDuoi = BanThocArray[vi_tri_bong][1];
//	Mor_BanDuoi_nghich;
//	robotStop(1);
//	return 1;
//}
//int BanLepArray[6][2] = {{25, 165}, {25, 165}, {30, 170}, {25, 170}, {30, 170}, {30, 180}}; // -------mor tren,mor duoi/
//int Ban_Bong_Lep()
//{
//	Mor_BanTren = BanLepArray[vi_tri_bong][0];
//	Mor_BanTren_nghich;
//	Mor_BanDuoi = BanLepArray[vi_tri_bong][1];
//	Mor_BanDuoi_nghich;
//	robotStop(1);
//	return 1;
//}


//void test_gap_lua(){
//	if(!R2){
//	vt_xl_nang_lua = 1;
//	xl_nang_lua = 0;
//	for (i = 0; i < 50; i++
//		while (CB_XL_NANG_LUA_1 == 1)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	XI_LANH_TRUOTGAPLUA_ON;
//	vTaskDelay(6000);
//	XI_LANH_KEPLUADUOI_ON, XI_LANH_KEPLUATREN_ON;
//	vTaskDelay(2000);
//	vt_xl_nang_lua = 3;
//	xl_nang_lua = 1;

//	for (i = 0; i < 50; i++)
//		while (CB_XL_NANG_LUA_3 == 1)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	}
//	if(!SELECT){
//	vt_xl_nang_lua = 2;
//	xl_nang_lua = 0;
//	}
//}
