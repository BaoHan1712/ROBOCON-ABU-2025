#define MOCCU 206       
#define LUC_TAI_MOC 36
#define LUC_MAX 250
#define LUC_MIN 20

#define SMOOTHING_FACTOR 0.1  

float filtered_lazeValue = 0;

// H? s? h?i quy mu (suy ra d? l?c t?i MOCCU là dúng)
#define A 19 
#define B (log((float)LUC_TAI_MOC / A) / MOCCU)

void lucbanlazerXanh(float lazeTruocValue, int received_offset) {
    float gocBan = 0;
    float lucBan = 0;
    float corrected_distance;

    // Làm m?n tín hi?u do kho?ng cách
   // filtered_lazeValue = (SMOOTHING_FACTOR * lazeTruocValue) + ((1 - SMOOTHING_FACTOR) * filtered_lazeValue);
			filtered_lazeValue = (SMOOTHING_FACTOR * (lazeTruocValue - (received_offset)) + ((1 - SMOOTHING_FACTOR) * filtered_lazeValue));

    // Tính kho?ng cách chu?n
    corrected_distance = sqrt((filtered_lazeValue * filtered_lazeValue));

    // Áp d?ng công th?c h?i quy s? mu
    lucBan = A * exp(B * corrected_distance);

    if (lucBan > LUC_MAX) {
        lucBan = LUC_MAX;
    } else if (lucBan < LUC_MIN) {
        lucBan = LUC_MIN;
    }


    force_F = lucBan;    
    final_force = force_F;
	
		}


