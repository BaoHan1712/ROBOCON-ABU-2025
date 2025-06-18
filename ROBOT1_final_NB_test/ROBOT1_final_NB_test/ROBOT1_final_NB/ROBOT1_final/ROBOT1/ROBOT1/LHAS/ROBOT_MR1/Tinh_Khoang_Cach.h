#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ROWS 105
#define MAX_COLS 40

#define MAX_SEGMENTS 50 // Gi?i h?n t?i da 50 do?n
#define GAP_THRESHOLD 5 // Ngu?ng kho?ng tr?ng (tru?c d?y l? N)

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

int segment_starts[MAX_SEGMENTS];  // M?ng luu ch? s? b?t d?u c?a c?c do?n
int segment_ends[MAX_SEGMENTS];    // M?ng luu ch? s? k?t th?c c?a c?c do?n

int arrLaser[ROWS][MAX_COLS];
int arrLaserNextIndex[ROWS];
int nonZeroArr[MAX_COLS];
int rowMeans[ROWS];
int segment_count;

int widthBackboard = 180;
int distanceBackboardToRimCenter = 38;


//================================================================================
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void quicksort(int arr[], int low, int high) {
	int pi, pivot = arr[high];
	int j, i = low - 1;
	if (low < high) {
		for (j = low; j < high; j++) {
			if (arr[j] <= pivot) {
				i++;
				swap(&arr[i], &arr[j]);
			}
		}
		swap(&arr[i + 1], &arr[high]);
		pi = i + 1; 
		
		quicksort(arr, low, pi - 1);
		quicksort(arr, pi + 1, high);
	}
}

void initStaticArray() {
    int i, j;
    for (i = 0; i < ROWS; i++) {
        arrLaserNextIndex[i] = 0; 
        for (j = 0; j < MAX_COLS; j++) {
            arrLaser[i][j] = 0;
        }
    }
}

void addElement(int rowIndex, int value) {
	if (rowIndex < 0 || rowIndex >= ROWS) return;
	if (value == 0) return;
	arrLaser[rowIndex][arrLaserNextIndex[rowIndex]] = value;
	arrLaserNextIndex[rowIndex] = (arrLaserNextIndex[rowIndex] + 1) % MAX_COLS;
}

int removeOutliersAndCalculateMedian(int arr[]) {
	int nonZeroCount;
	int Q1, Q3, IQR, lowerBound, upperBound;
	int validCount;
	int i;

	nonZeroCount = 0;
	for (i = 0; i < MAX_COLS; i++) {
		if (arr[i] != 0) {
			nonZeroArr[nonZeroCount++] = arr[i];
		}
	}

	if (nonZeroCount == 0) {
		return 0;
	}

	quicksort(nonZeroArr, 0, nonZeroCount-1);

	Q1 = nonZeroArr[(int)nonZeroCount / 4];
	Q3 = nonZeroArr[(int)(3 * nonZeroCount) / 4];
	IQR = Q3 - Q1;
	lowerBound = Q1 - (int)(1.5 * IQR);
	upperBound = Q3 + (int)(1.5 * IQR);

	// Move valid elements to the front of nonZeroArr
	for (i = 0, validCount = 0; i < nonZeroCount; i++) {
		if (nonZeroArr[i] >= lowerBound && nonZeroArr[i] <= upperBound) {
			nonZeroArr[validCount++] = nonZeroArr[i];
		}
	}

	if (validCount == 0) return 0;

	if (validCount % 2 == 0) return (int)(nonZeroArr[validCount / 2 - 1] + nonZeroArr[validCount / 2]) / 2;
	else return nonZeroArr[validCount / 2];
}

void segmentArray() {
		int i, last_non_zero;
    int zero_count = 0; 
    int start = 0;      

		segment_count = 0;
	
    for ( i= 0; i < ROWS; i++) {
        if (rowMeans[i] == 0) {
            zero_count++; 
        } else {
            if (zero_count > GAP_THRESHOLD && segment_count < MAX_SEGMENTS) {
                if (start < i - zero_count) {
                    segment_starts[segment_count] = start;
                    segment_ends[segment_count] = i - zero_count - 1;
                    segment_count++;
                }
                start = i; 
            }
            zero_count = 0; 
        }
    }
   
    if (start < ROWS && segment_count < MAX_SEGMENTS) {
        last_non_zero = start;
        for (i = start; i < ROWS; i++) {
            if (rowMeans[i] != 0) {
                last_non_zero = i; 
            }
        }
        if (last_non_zero >= start) {
            segment_starts[segment_count] = start;
            segment_ends[segment_count] = last_non_zero;
            segment_count++;
        }
    }
}

void calculateOverallMean() {
    int min_value, min_index, first_segment_min = 0, first_segment_min_index = -1; // Gi? tr? nh? nh?t v? index c?a do?n d?u
    int last_segment_min = 0, last_segment_min_index = -1;  // Gi? tr? nh? nh?t v? index c?a do?n cu?i
    int i, j, c, leftIndex=0, rightIndex=0;
    for (i = 0; i < ROWS; i++) {
        rowMeans[i] = removeOutliersAndCalculateMedian(arrLaser[i]);
    }
		segmentArray();
		
		for ( i = 0; i < segment_count; i++) {
        min_value = 1000; // Kh?i t?o v?i ph?n t? d?u ti?n
        min_index = -1;          // Index c?a ph?n t? nh? nh?t
        for ( j = segment_starts[i] + 1; j <= segment_ends[i]; j++) {
            if (rowMeans[j] > 0 && rowMeans[j] < min_value) {
                min_value = rowMeans[j];
                min_index = j;
            }
        }
        // Luu th?ng tin cho do?n d?u v? do?n cu?i
        if (i == 0) { // ?o?n d?u
            first_segment_min = min_value;
            first_segment_min_index = min_index;
        }
        if (i == segment_count - 1) { // ?o?n cu?i
            last_segment_min = min_value;
            last_segment_min_index = min_index;
        }
    }

		
		canh_trai = first_segment_min;
		canh_phai = last_segment_min;
		tong_bien_tro_bang = (int)(first_segment_min_index + last_segment_min_index);
		bien_tro_canh_phai = (int)last_segment_min_index;
		
		//bien_tro_tam_ro = (int)(first_segment_min_index + last_segment_min_index);//segment_count;
//		banhaicanh(canh_trai,canh_phai);
}                                               

float calculateDistance(float laserToLeft, float laserToRight)
{
    float part1;
    float sqrtPart1;
    float part2;
    float denominator;
    float innerValue;
    float distance;

    /* Tính part1 theo công th?c */
    part1 = 2.0f * laserToRight * laserToRight + 2.0f * laserToLeft * laserToLeft - widthBackboard * widthBackboard;

    /* Tính part2 theo công th?c */
    part2 = (laserToRight * laserToRight - laserToLeft * laserToLeft) * (laserToRight * laserToRight - laserToLeft * laserToLeft);

    /* M?u s? */
    denominator = widthBackboard * widthBackboard * part1;

    /* Ki?m tra d? li?u h?p l? */
    if (part1 <= 0.0f || denominator == 0.0f)
    {
        return -1.0f; /* D? li?u không h?p l? */
    }

    /* Tính giá tr? bên trong can b?c hai */
    innerValue = 1.0f - (part2 / denominator);

    /* N?u giá tr? âm ? l?i */
    if (innerValue < 0.0f)
    {
        return -1.0f; /* Can b?c hai s? âm */
    }

    /* Tính sqrt(part1) */
    sqrtPart1 = sqrtf(part1);

    /* Cu?i cùng, tính CD */
    distance = sqrtf(0.25f * part1
               + distanceBackboardToRimCenter * distanceBackboardToRimCenter
               - distanceBackboardToRimCenter * sqrtPart1 * sqrtf(innerValue));

    /* Tr? v? k?t qu? */
    return distance;
}

/* Hàm tính góc (theo d?) gi?a 2 c?nh a và b, bi?t 3 c?nh c?a tam giác */
double calculateAngleFromSides(double sideA, double sideB, double sideC)
{
    double cosTheta;
    double angleRadians;
    double angleDegrees;

    /* Tính cos(theta) b?ng d?nh lý cosine */
    cosTheta = (sideA * sideA + sideB * sideB - sideC * sideC) / (2.0 * sideA * sideB);

    /* Ép cosTheta vào [-1, 1] d? tránh l?i do sai s? */
    if (cosTheta > 1.0)
    {
        cosTheta = 1.0;
    }
    else if (cosTheta < -1.0)
    {
        cosTheta = -1.0;
    }

    /* Tính góc radian */
    angleRadians = acos(cosTheta);

    /* Ð?i sang d? */
    angleDegrees = angleRadians * (180.0 / M_PI);

    /* Tr? v? k?t qu? */
    return angleDegrees;
}

// ham tinh bt de quay ve tam ro
int calculateBTForRimCenter(int viTriBTBatDauQuet){
	float DB,goc_ACB,goc_DCB,he_so_dcb;
	int BTForRimCenter;
	// luu y : -- vi tri bien tro bat dau quet la vi tri vi tri bien tro nogn ban bat dau qyet va luu lazer vao mang-- 
	//chieu dài tu tam ro den canh phai
	DB = sqrt(widthBackboard * widthBackboard/4 + distanceBackboardToRimCenter * distanceBackboardToRimCenter);
	
	//goc_ACB = tinh_goc(AC, BC, AB);
	
	goc_ACB = calculateAngleFromSides(canh_trai, canh_phai, widthBackboard);
	
	//goc_DCB = tinh_goc(DC, BC, DB);
	goc_DCB = calculateAngleFromSides(distanceRobotToBackboard, canh_phai, DB);
	
	//tinh he so dcb/acb
	he_so_dcb = goc_DCB/goc_ACB;
	// tinh bien tro giua tam ro
	BTForRimCenter  =  viTriBTBatDauQuet + tong_bien_tro_bang - (int)(he_so_dcb*tong_bien_tro_bang);
	//if nho hon min hoac lon hon max return ve min
	if (BTForRimCenter > Max_BT_Xoay || BTForRimCenter < Min_BT_Xoay) BTForRimCenter = viTriBTBatDauQuet;
	return BTForRimCenter;
}


