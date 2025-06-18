#include <stdlib.h>
#include <math.h>

float GIA_TOC; 
float luong_giac_tt,luong_giac_pt,luong_giac_ts,luong_giac_ps;
const  float radian= 3.141592653589793238462643/180;

////bien chua so nguyen
int TOC_DO_XOAY,BIT_XOAY,GOC_LECH;
int HUONG,TOC_DO,MOTOR_LOCK,MAT_ROBOT,CHAY_VA_XOAY,CHAY_GIA_TOC=0; 
int trai_truoc1,phai_truoc1,trai_sau1,phai_sau1,TOC_DO_DAT,ROBOT_STOP;

//////////////////////xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//////////////////////////


void RUN_3_BANH(int _mat_robot,int _huong,int _toc_do_chay,float _gia_toc,int _chay_va_xoay)		
		{				BIT_XOAY		=	0;
						HUONG				=	_huong;
						GIA_TOC			=	_gia_toc;
						ROBOT_STOP	=	0;
						MOTOR_LOCK	=	0;	
						CHAY_GIA_TOC=	10;
						TOC_DO_DAT 	= _toc_do_chay;		
						MAT_ROBOT	 	= _mat_robot;	
						CHAY_VA_XOAY=	_chay_va_xoay;
						TOC_DO			=	2;
			
						luong_giac_tt=(float)(sin(((450 -HUONG)/10)*radian));
						luong_giac_ts=(float)(sin(((1350-HUONG)/10)*radian));
						luong_giac_ps=(float)(sin(((2250-HUONG)/10)*radian));
						luong_giac_pt=(float)(sin(((3150-HUONG)/10)*radian));	
			
}	

int Ban_1, Ban_2, Ban_3;

void monitor_R1_button() {
    while (1) {
        Ban_1 = Ban_2 = Ban_3 = 120 * L1; // Set to 120 if L1 is pressed, otherwise 0
    }
}
