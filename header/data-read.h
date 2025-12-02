#ifndef DATA_READ_H
#define DATA_READ_H
#include <stdbool.h>
int** ladeDaten(const char* filename);
void getMinMaxVal(int **matrix, int *maxVal, int *minVal);
void writeData(const char* dateiname);
bool compareData(const char* datei1, const char* datei2);
#endif
