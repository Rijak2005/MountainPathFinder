// read data from the target data file and store it in a 2D Array in the heap
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../header/cmdline.h"
#include "../header/data-read.h"
#include "../header/graphics.h"
#define INSGESAMT 403200
//403.200 ints mit 480 rows and 844 cols

int** ladeDaten(const char* filename){ //has to be int**, because it returns a 2D Matrix (2 times malloc)
FILE *file = fopen(filename,"r");
if(!file){
    perror("Datei konnte nicht geöffnet werden\n");
    exit(1);
}
int** bergdaten = malloc(GRIDROWS * sizeof(int*)); //column = Spalte
if(bergdaten == NULL){
    printf("Speicher nicht alokiert bei zeilen");
    fclose(file);
    return NULL;
}

// printf("Starting the reading with ROWS = %d and COLS = %d", GRIDROWS,GRIDCOLS);

for (int i = 0; i < GRIDROWS; i++) { //rows = Zeile 
    bergdaten[i] = malloc(GRIDCOLS * sizeof(int));
    if (bergdaten[i] == NULL) {
        perror("Error for malloc");
        // free the already allocated memory
        for (int m = 0; m < i; m++) {
            free(bergdaten[m]);
        }
        free(bergdaten);
        fclose(file);
        return NULL;
    }
    for (int j = 0; j < GRIDCOLS; j++) {
        if (fscanf(file, "%d", &bergdaten[i][j]) != 1) {
            fprintf(stderr, "Fehler beim Einlesen der Matrix bei [%d][%d]\n", i, j);
            // free malloc
            for (int k = 0; k <= i; k++) {
                if (bergdaten[k] != NULL) {
                    free(bergdaten[k]);
                    bergdaten[k] = NULL;
                }
            }
            free(bergdaten);
            fclose(file);
            return NULL; 
        }
    }
}


fclose(file);
return bergdaten;
}

 

void getMinMaxVal(int** matrix, int *maxVal, int *minVal) {
    *minVal = matrix[0][0];
    *maxVal = matrix[0][0];

    for (int i = 0; i < GRIDROWS; i++) {
        for (int j = 0; j < GRIDCOLS; j++) {
            if (matrix[i][j] < *minVal) *minVal = matrix[i][j];
            if (matrix[i][j] > *maxVal) *maxVal = matrix[i][j];
        }
    }
}
//this methode writes in a new file(build/src/ausgabe.dat) 
void writeData(const char* dateiname) {
    FILE* datei = fopen(dateiname, "w");
    if (!datei) {
        printf("error opening file");
        return;
    }

    for (int i = 0; i < GRIDROWS; i++) {
        for (int j = 0; j < GRIDCOLS; j++) {
            fprintf(datei, "%d", bergdaten[i][j]);
            if (j < GRIDCOLS - 1) {
                fputc(' ', datei); //writes " " between heightnumbers 
            }
        }
        fputc('\n', datei); // newline after every column
    }

    fclose(datei);
}

bool compareData(const char* datei1, const char* datei2) {
    FILE* f1 = fopen(datei1, "r");
    FILE* f2 = fopen(datei2, "r");

    if (!f1 || !f2) {
        printf("error opening file");
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return false;
    }
    //comparing both files for differences 
    int zahl1, zahl2;
    for (int i = 0; i < GRIDROWS; i++) {
        for (int j = 0; j < GRIDCOLS; j++) {
            if (fscanf(f1, "%d", &zahl1) != 1 || fscanf(f2, "%d", &zahl2) != 1) {
                printf("error during reading or unexpected fileend at [%d][%d]\n", i, j);
                fclose(f1);
                fclose(f2);
                return false;
            }

            if (zahl1 != zahl2) {
                printf("difference at row %d, column %d: %d != %d\n", i, j, zahl1, zahl2);
                fclose(f1);
                fclose(f2);
                return false;
            }
        }
    }

    //check if there are still numbers 
    if (fscanf(f1, "%d", &zahl1) == 1 || fscanf(f2, "%d", &zahl2) == 1) {
        printf("one file contains more data than the other.\n");
        fclose(f1);
        fclose(f2);
        return false;
    }

    fclose(f1);
    fclose(f2);
    return true;
}











