//**********************************SAN DO*************************************
int gtri_laze_trai = 5;
//void Xuat_phat_do(void)
//{
//	RESET_ENCODER();

//	robotRunAngle(-1350, 60, 0, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 6000)
//		{
//			if (ENCODER_TONG() > 3000)
//				XI_LANH_TONG_ON;
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	robotRunAngle(-900, 80, -10, -0.2);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 15000)
//		{
//			if (ENCODER_TONG() > 5000)
//				vt_xl_nang_lua = 1;
//				xl_nang_lua = 0;
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	robotRunAngle(-850, 120, 5, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 45000)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	robotRunAngle(-850, 30, 5, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 50000)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//		while (lazeTraiValue > 80)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	XI_LANH_TRUOTGAPLUA_ON;

//	robotRunAngle(-650, 20, 5, 0.1);
//	for (i = 0; i < 50; i++)
//		while (CB_NHAN_LUA == 1 || CB_NHAN_LUA1 == 1)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	vTaskDelay(500);
//	robotStop(0);

//	robotRunAngle(0, 15, 0, 0.1); // 15

//	vTaskDelay(3000);

//	robotStop(0);
//	while (START)
//	{
//		robotGamePadControl();
//		vTaskDelay(1);
//	}

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

//	RESET_ENCODER();

//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 42000)
//		{
//			Bam_thanh_laser_trai_1800(100, -5, 82, 3, 100, 20); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//		
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 55000)
//		{
//			Bam_thanh_laser_trai_1800(40, -10, 82, 1, 80, 20); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	
//	vt_xl_nang_lua = 2;
//	xl_nang_lua = 0;
//		
//	for (i = 0; i < 50; i++)
//	{
//		while (lazeSauValue > 89)
//		{
//			Bam_thanh_laser_trai_1800(17, -10, 82, 1, 80, 20); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//		vTaskDelay(1);
//	}

//	robotStop(0);
////	vTaskDelay(1500);
//	while (START)
//	{
//		robotGamePadControl();
//		vTaskDelay(1);
//	}
//	XI_LANH_KEPLUATREN_OFF;
//	vTaskDelay(1500);

//	RESET_ENCODER();

//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 10000)
//		{
//			if (ENCODER_TONG() > 4000)
//				XI_LANH_TRUOTGAPLUA_OFF;
//			Bam_thanh_laser_trai_1800(40, -5, 82, 1, 80, 20); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 14500)
//		{												 // if(ENCODER_TONG() > 2000)		XI_LANH_TRUOTGAPLUA_OFF;
//			Bam_thanh_laser_trai_1800(17, -5, 82, 1, 80, 20); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//	{
//		while (lazeSauValue > 39)
//		{												   // if(ENCODER_TONG() > 2000)		XI_LANH_TRUOTGAPLUA_OFF;
//			Bam_thanh_laser_trai_1800(12, -5, 82, 1, 80, 20); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//		vTaskDelay(1);
//	}
//	robotStop(0);
////	vTaskDelay(1500);
//	while (START)
//	{
//		robotGamePadControl();
//		vTaskDelay(1);
//	}

//	XI_LANH_KEPLUADUOI_OFF;
//	vTaskDelay(1500);

//	//***************************************** ve lay lua lan 2 ************************************************************

//	RESET_ENCODER();
//	robotRunAngle(1000, 60, 0, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 5500)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	robotRunAngle(900, 60, 0, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 10000)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	robotStop(10);
//	//---------------------------------------lay lan 2-------------------------------------------------------------
//	RESET_ENCODER();
//	robotRunAngle(0, 60, 15, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 63000)
//		{
//			if (ENCODER_TONG() > 40000)
//				vt_xl_nang_lua = 1;
//				xl_nang_lua = 0;
//			Bam_thanh_laser_trai(100, 0, 153, 10 ,100);
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 65000)
//		{
//			Bam_thanh_laser_trai(60, 0, 153, 4, 100);
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//		while (CB_CHAM_THANH_LUA1 == 1)
//		{
//			Bam_thanh_laser_trai(20, 0, 153, 3, 80);
//			;
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//		while (CB_CHAM_THANH_LUA == 1)
//		{
//			Bam_thanh_laser_trai(17, 0, 153, 2, 80);
//			;
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	vTaskDelay(1500);
//	robotStop(0);
//	
//	RESET_ENCODER();
//	robotRunAngle(800, 15, 0, 0.1);

//	for (i = 0; i < 50; i++)
//		while (CB_NHAN_LUA == 1 || CB_NHAN_LUA1 == 1)
//		{
//			XI_LANH_TRUOTGAPLUA_ON;
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	vTaskDelay(500);
//	
//	robotStop(0);

//	robotRun(0, 15); // 15
//	vTaskDelay(2500);
//	robotStop(0);
////	vTaskDelay(1000);
//	while (START)
//	{
//		robotGamePadControl();
//		vTaskDelay(1);
//	}

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

//	RESET_ENCODER();
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 42000)
//		{
//			Bam_thanh_laser_trai_1800(100, -10, 182, 2, 120, 100); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 55000)
//		{
//			Bam_thanh_laser_trai_1800(40, -10, 182, 1, 100, 100); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	vt_xl_nang_lua = 2;
//	xl_nang_lua = 0;
//	for (i = 0; i < 50; i++)
//	{
//		while (lazeSauValue > 89)
//		{
//			Bam_thanh_laser_trai_1800(17, -10, 182, 1, 80, 100); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//		vTaskDelay(1);
//	}

//	robotStop(0);
////	vTaskDelay(1500);
//	while (START)
//	{
//		robotGamePadControl();
//		vTaskDelay(1);
//	}
//	XI_LANH_KEPLUATREN_OFF;
//	vTaskDelay(1500);

//	RESET_ENCODER();

//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 10000)
//		{
//			if (ENCODER_TONG() > 4000)
//				XI_LANH_TRUOTGAPLUA_OFF;
//			Bam_thanh_laser_trai_1800(30,-5, 183, 2, 100, 100); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 14500)
//		{												  // if(ENCODER_TONG() > 2000)		XI_LANH_TRUOTGAPLUA_OFF;
//			Bam_thanh_laser_trai_1800(17, -5, 183, 1, 80, 100); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//	{
//		while (lazeSauValue > 39)
//		{												  // if(ENCODER_TONG() > 2000)		XI_LANH_TRUOTGAPLUA_OFF;
//			Bam_thanh_laser_trai_1800(12, -5, 183, 1, 80, 100); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//		vTaskDelay(1);
//	}
//	robotStop(0);
////	vTaskDelay(1500);
//	while (START)
//	{
//		robotGamePadControl();
//		vTaskDelay(1);
//	}   

//	XI_LANH_KEPLUADUOI_OFF;
//	vTaskDelay(1500);

//	////***************************************** ve lay lua lan 3 ************************************************************
//	RESET_ENCODER();
//	robotRunAngle(1000, 60, 0, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 5500)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	robotRunAngle(900, 60, 0, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 10000)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	robotStop(10);
//	//---------------------------------------lay lan 3-------------------------------------------------------------
//	RESET_ENCODER();
//	robotRunAngle(0, 60, 15, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 63000)
//		{
//			if (ENCODER_TONG() > 40000)
//				vt_xl_nang_lua = 1;
//				xl_nang_lua = 0;
//			Bam_thanh_laser_trai(100, 0, 253, 10, 100);
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 65000)
//		{
//			Bam_thanh_laser_trai(60, 0, 253, 4, 100);
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	for (i = 0; i < 50; i++)
//		while (CB_CHAM_THANH_LUA1 == 1)
//		{
//			Bam_thanh_laser_trai(20, 0, 253, 3, 80);
//			;
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	
//	for (i = 0; i < 50; i++)
//		while (CB_CHAM_THANH_LUA == 1)
//		{
//			Bam_thanh_laser_trai(15, 0, 253, 2, 80);
//			;
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	vTaskDelay(1500);
//	robotStop(0);
//	RESET_ENCODER();
//	robotRunAngle(800, 15, 0, 0.1);

//	for (i = 0; i < 50; i++)
//		while (CB_NHAN_LUA == 1 || CB_NHAN_LUA1 == 1)
//		{
//			XI_LANH_TRUOTGAPLUA_ON;
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	vTaskDelay(500);

//	robotStop(0);
//	robotRun(0, 15); // 15
//	vTaskDelay(2500);
//	robotStop(0);
////	vTaskDelay(1000);
//	while (START)
//	{
//		robotGamePadControl();
//		vTaskDelay(1);
//	}
//	XI_LANH_KEPLUADUOI_ON, XI_LANH_KEPLUATREN_ON;
//	vTaskDelay(2000);

//	vt_xl_nang_lua = 3;
//	xl_nang_lua = 1;
//	
//	for (i = 0; i < 50; i++)
//		while (CB_XL_NANG_LUA_3 == 1)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	RESET_ENCODER();
//	
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 42000)
//		{
//			Bam_thanh_laser_trai_1800(100, -8, 283, 3, 100, 120); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 55000)
//		{
//			Bam_thanh_laser_trai_1800(40, -20, 283, 1, 80, 120); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	vt_xl_nang_lua = 2;
//	xl_nang_lua = 0;
//	for (i = 0; i < 50; i++)
//	{
//		while (lazeSauValue > 89)
//		{
//			Bam_thanh_laser_trai_1800(17, -20, 283, 1, 80, 200); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//		vTaskDelay(1);
//	}

//	robotStop(0);
////	vTaskDelay(1500);
//	while (START)
//	{
//		robotGamePadControl();
//		vTaskDelay(1);
//	}
//	XI_LANH_KEPLUATREN_OFF;
//	vTaskDelay(1500);

//	RESET_ENCODER();

//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 10000)
//		{
//			if (ENCODER_TONG() > 4000)
//				XI_LANH_TRUOTGAPLUA_OFF;
//			Bam_thanh_laser_trai_1800(30, -12, 283, 2, 100, 200); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 14500)
//		{												  
//			Bam_thanh_laser_trai_1800(17, -10 , 284, 1, 80, 200); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//	{
//		while (lazeSauValue > 39)
//		{												 
//			Bam_thanh_laser_trai_1800(12, -10, 284, 1, 80, 200); //-5
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//		vTaskDelay(1);
//	}
//	robotStop(0);
////	vTaskDelay(1500);
//	while (START)
//	{
//		robotGamePadControl();
//		vTaskDelay(1);
//	}

//	XI_LANH_KEPLUADUOI_OFF;
//	vTaskDelay(1500);

//	RESET_ENCODER();
//	robotRunAngle(950, 60, 15, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 4500)
//		{
//			if (ENCODER_TONG() > 3000)
//			XI_LANH_KEP_BONG_OFF;
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	vt_xl_nang_lua = 4;
//	xl_nang_lua = 1;
//	
//	for (i = 0; i < 50; i++)
//		while (CB_XL_NANG_LUA_4 == 1)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	RESET_ENCODER();

//	robotRun(450 - _robotIMUAngle, 60);
//	robotRotate(-1800, -0.6, 0);
//	while (_robotIMUAngle > -1820)
//	{
//		if (wantExit() == 1)
//			break;
//		vTaskDelay(1);
//	}

//	robotRunAngle(900, 40, -1800, -0.3);
//	for (i = 0; i < 50; i++)
//		while (lazeTraiValue > gtri_laze_trai)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	robotStop(0);
//	Vi_tri = 1;
//}

//void chay_san_2_do(void)
//{

//	//------len doc-----------
//	RESET_ENCODER();
//	robotRunAngle(1800, 120, -1800, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 68000)
//		{
//			speed_xoay = 50;  
//			taget_BT_Xoay = goc_ban;
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	
//	robotRunAngle(1800, 60, -1800, 0.1);
//	for (i = 0; i < 50; i++)
//		while (lazeTruocValue > 180)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	robotStop(0);

//	RESET_ENCODER();
//	robotRunAngle(-1350, 40, -1850, 0.1);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 10000)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//		while (lazeTruocValue > 92)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	RESET_ENCODER();
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 10000)
//		{
//			bam_Thanh_Lazer_Truoc_san2do(30, -1850, 87, 2, 120);
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}

//	for (i = 0; i < 50; i++)
//		while (lazePhaiValue > mapSan2Do[0])
//		{
//			bam_Thanh_Lazer_Truoc_san2do(30, -1850, 87, 2, 120);
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	robotStop(1);
//	speed_xoay = 200; 
//}
//void retry_san2_do(void)
//{
//	RESET_ENCODER();
//	robotRunAngle(1550, 80, 0, 0.2);
//	for (i = 0; i < 50; i++)
//		while (ENCODER_TONG() < 2000)
//		{
//			//if (ENCODER_TONG() > 2000)
//			XI_LANH_TONG_ON;
//			XI_LANH_KEP_BONG_OFF;
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	
//	robotRotate(-1800, -0.6, 0);
//	while (_robotIMUAngle > -1800)
//	{
//		if (wantExit() == 1)
//			break;
//		vTaskDelay(1);
//	}

//	robotRunAngle(1350, 30, -1800, -0.3);
//	for (i = 0; i < 50; i++)
//		while (lazeTraiValue > gtri_laze_trai)
//		{
//			if (wantExit() == 1)
//				break;
//			vTaskDelay(1);
//		}
//	robotStop(50);
//	chay_san_2_do();
//	//	robotStop(0);
//}