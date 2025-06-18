#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ROWS 121
#define MAX_COLS 30

#define MAX_SEGMENTS 50 // Gi?i h?n t?i da 50 do?n
#define GAP_THRESHOLD 5 // Ngu?ng kho?ng tr?ng (tru?c d?y l? N)

#define PI 3.14159265358979323846
#define M_PI 3.14159265358979323846

int segment_starts[MAX_SEGMENTS];  // M?ng luu ch? s? b?t d?u c?a c?c do?n
int segment_ends[MAX_SEGMENTS];    // M?ng luu ch? s? k?t th?c c?a c?c do?n

int arrLaser[ROWS][MAX_COLS];
int arrLaserNextIndex[ROWS];
int nonZeroArr[MAX_COLS];
int rowMeans[ROWS];
int segment_count;
int first_segment_min = 0, first_segment_min_index = -1;
int last_segment_min = 0, last_segment_min_index = -1;	


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
    int min_value, min_index; // Gi? tr? nh? nh?t v? index c?a do?n d?u
    int i, j, c, leftIndex=0, rightIndex=0;
    for (i = 0; i < ROWS; i++) {
        rowMeans[i] = removeOutliersAndCalculateMedian(arrLaser[i]);
    }
		
    segmentArray();
		
	for ( i = 0; i < segment_count; i++) {
        min_value = 15000; // Kh?i t?o v?i ph?n t? d?u ti?n
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
}                                               


int is_valid_acos(double value) {
    return (value >= -1.0 && value <= 1.0);
}

int calculate_triangle(double AC, double BC, int *DC, int *rimCenter) {
    // Giá tr? m?c d?nh	
    double AB = 1800.0;
    double ED = 380.0;
    // Bi?n hình h?c
    double DB, angle_ABD, angle_ABC, angle_DBC, angle_ACB, angle_BCD, cos_ABC, cos_ACB, cos_BCD;
		
    DB = sqrt(ED * ED + (AB / 2.0) * (AB / 2.0));
    angle_ABD = atan(2.0 * ED / AB);
    cos_ABC = (AB * AB + BC * BC - AC * AC) / (2.0 * AB * BC);
    if (!is_valid_acos(cos_ABC)) return 1;
    angle_ABC = acos(cos_ABC);
    angle_DBC = angle_ABC - angle_ABD;
    if (angle_DBC <= 0.0) return 1;
    *DC = (int)sqrt(DB * DB + BC * BC - 2.0 * DB * BC * cos(angle_DBC));
    cos_ACB = (AC * AC + BC * BC - AB * AB) / (2.0 * AC * BC);
    if (!is_valid_acos(cos_ACB)) return 1;
    angle_ACB = acos(cos_ACB);
    cos_BCD = ((*DC) * (*DC) + BC * BC - DB * DB) / (2.0 * (*DC) * BC);
    if (!is_valid_acos(cos_BCD)) return 1;
    angle_BCD = acos(cos_BCD);
    //---------------------------------------------------------
   
    *rimCenter  = (int)(last_segment_min_index - (angle_BCD / angle_ACB)*(last_segment_min_index - first_segment_min_index));

    return 0;
}

