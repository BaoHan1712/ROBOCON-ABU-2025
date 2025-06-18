//=================== Sân xanh xoáy===================


#define MOCCU 398  // 404
#define MOCCU_KX 585
#define LUC_TAI_MOC 164
#define LUC_TAI_MOC_XOAY 116


#define LUC_MAX 250
#define LUC_MIN 20
 
#define SMOOTHING_FACTOR 0.1

float filtered_lazeValue = 0;

// He so hoi quy mu (suy ra dc luc toi MOCCU là dúng)
#define A 62   // 32  // 45 o NB
#define A_XOAY 50  // 32  // 45 o NB
#define B (log((float) (LUC_TAI_MOC + cong_tru_luc)/ A) / MOCCU_KX)
#define B_XOAY (log((float) (LUC_TAI_MOC_XOAY + cong_tru_luc)/ A_XOAY) / MOCCU)
	
int lazeTruocValue_Ban;
void lucbanlazerXanh(float lazeTruocValue_Ban, int received_offset) {
    float gocBan = 0;
    float lucBan = 0;
    float corrected_distance;

    // Làm m?n tín hi?u do kho?ng cách
			filtered_lazeValue = (SMOOTHING_FACTOR * (lazeTruocValue_Ban - (received_offset)) + ((1 - SMOOTHING_FACTOR) * filtered_lazeValue));

    // Tính kho?ng cách chu?n
    corrected_distance = sqrt((filtered_lazeValue * filtered_lazeValue));

    if (lazeTruocValue_Ban < 590) {
    lucBan = A_XOAY * exp(B_XOAY * corrected_distance);}
		else {lucBan = A * exp(B * corrected_distance);}

    if (lucBan > LUC_MAX) {
        lucBan = LUC_MAX;
    } else if (lucBan < LUC_MIN) {
        lucBan = LUC_MIN;
    }

    force_F = lucBan;    
    final_force = force_F;
	
		}
