////=================== Sân xanh xoáy===================


//#define MOCCU 398  // 404
//#define LUC_TAI_MOC 113
//#define LUC_TAI_MOC_XOAY 117.5


//#define LUC_MAX 250
//#define LUC_MIN 20
// 
//#define SMOOTHING_FACTOR 0.1

//float filtered_lazeValue = 0;

//// He so hoi quy mu (suy ra dc luc toi MOCCU là dúng)
//#define A 48    // 32  // 45 o NB
//#define A_XOAY 50  // 32  // 45 o NB
//#define B (log((float)LUC_TAI_MOC / A) / MOCCU)
//#define B_XOAY (log((float)LUC_TAI_MOC_XOAY / A_XOAY) / MOCCU)
//	
//int lazeTruocValue_Ban;
//void lucbanlazerXanh(float lazeTruocValue_Ban, int received_offset) {
//    float gocBan = 0;
//    float lucBan = 0;
//    float corrected_distance;

//    // Làm m?n tín hi?u do kho?ng cách
//   // filtered_lazeValue = (SMOOTHING_FACTOR * lazeTruocValue) + ((1 - SMOOTHING_FACTOR) * filtered_lazeValue);
//			filtered_lazeValue = (SMOOTHING_FACTOR * (lazeTruocValue_Ban - (received_offset)) + ((1 - SMOOTHING_FACTOR) * filtered_lazeValue));

//    // Tính kho?ng cách chu?n
//    corrected_distance = sqrt((filtered_lazeValue * filtered_lazeValue));

//    if (lazeTruocValue_Ban < 590) {
//    lucBan = A_XOAY * exp(B_XOAY * corrected_distance);}
//		else {lucBan = A * exp(B * corrected_distance);}
//		
//		if (lazeTruocValue_Ban >= 607 && lazeTruocValue_Ban < 613  ) {lucBan -= 2.2;}
//		else if (lazeTruocValue_Ban >= 613 && lazeTruocValue_Ban < 620  ) {lucBan -= 3.2;}
//		else if (lazeTruocValue_Ban >= 620 && lazeTruocValue_Ban < 630  ) {lucBan -= 5;}
//		else if (lazeTruocValue_Ban >= 630 && lazeTruocValue_Ban < 640  ) {lucBan -= 5;}
//		else if (lazeTruocValue_Ban >= 640 && lazeTruocValue_Ban < 650  ) {lucBan -= 8.5;}
//		else if (lazeTruocValue_Ban >= 650 && lazeTruocValue_Ban < 665 ) {lucBan -= 10;}
//		else if (lazeTruocValue_Ban >= 665 && lazeTruocValue_Ban < 680 ) {lucBan -= 13;}
//		else if (lazeTruocValue_Ban >= 680 && lazeTruocValue_Ban < 750 ) {lucBan -= 16;}

//		
//    if (lucBan > LUC_MAX) {
//        lucBan = LUC_MAX;
//    } else if (lucBan < LUC_MIN) {
//        lucBan = LUC_MIN;
//    }

//    force_F = lucBan;    
//    final_force = force_F;
//	
//		}
//=================== Sân xanh xoáy===================


#define MOCCU 398  // 404
#define LUC_TAI_MOC 113
#define LUC_TAI_MOC_XOAY 117.5


#define LUC_MAX 250
#define LUC_MIN 20
 
#define SMOOTHING_FACTOR 0.1

float filtered_lazeValue = 0;

// He so hoi quy mu (suy ra dc luc toi MOCCU là dúng)
#define A 48    // 32  // 45 o NB
#define A_XOAY 50  // 32  // 45 o NB
#define B (log((float)LUC_TAI_MOC / A) / MOCCU)
#define B_XOAY (log((float)LUC_TAI_MOC_XOAY / A_XOAY) / MOCCU)
	
int lazeTruocValue_Ban;
void lucbanlazerXanh(float lazeTruocValue_Ban, int received_offset) {
    float lucBan_Xoay = 0;
    float lucBan = 0;
    float corrected_distance;

    // Làm m?n tín hi?u do kho?ng cách
   // filtered_lazeValue = (SMOOTHING_FACTOR * lazeTruocValue) + ((1 - SMOOTHING_FACTOR) * filtered_lazeValue);
			filtered_lazeValue = (SMOOTHING_FACTOR * (lazeTruocValue_Ban - (received_offset)) + ((1 - SMOOTHING_FACTOR) * filtered_lazeValue));

    // Tính kho?ng cách chu?n
    corrected_distance = sqrt((filtered_lazeValue * filtered_lazeValue));

    
    lucBan_Xoay = A_XOAY * exp(B_XOAY * corrected_distance);
		//lucBan = A * exp(B * corrected_distance);
//		
//		if (lazeTruocValue_Ban >= 607 && lazeTruocValue_Ban < 613  ) {lucBan -= 2.2;}
//		else if (lazeTruocValue_Ban >= 613 && lazeTruocValue_Ban < 620  ) {lucBan -= 3.2;}
//		else if (lazeTruocValue_Ban >= 620 && lazeTruocValue_Ban < 630  ) {lucBan -= 5;}
//		else if (lazeTruocValue_Ban >= 630 && lazeTruocValue_Ban < 640  ) {lucBan -= 5;}
//		else if (lazeTruocValue_Ban >= 640 && lazeTruocValue_Ban < 650  ) {lucBan -= 8.5;}
//		else if (lazeTruocValue_Ban >= 650 && lazeTruocValue_Ban < 665 ) {lucBan -= 10;}
//		else if (lazeTruocValue_Ban >= 665 && lazeTruocValue_Ban < 680 ) {lucBan -= 13;}
//		else if (lazeTruocValue_Ban >= 680 && lazeTruocValue_Ban < 750 ) {lucBan -= 16;}

		
    if (lucBan_Xoay > LUC_MAX) {
        lucBan_Xoay = LUC_MAX;
    } else if (lucBan_Xoay < LUC_MIN) {
        lucBan_Xoay = LUC_MIN;
    }

    force_F = lucBan;    
    final_force = force_F;
	
		}
