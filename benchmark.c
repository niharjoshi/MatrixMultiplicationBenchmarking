// Created by Nihar Joshi

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/time.h"

#define MATRIX_SIZE_MIN 16
#define MATRIX_SIZE_MAX 4096
#define BLOCK_SIZE_MIN 4
#define BLOCK_SIZE_MAX 512

int** createMatrix(int size){
    int **matrix = (int**)malloc(size * sizeof(int*));
    for(int i = 0; i < size; i++)
    {
        matrix[i] = (int*)malloc(size * sizeof(int));
    }
    return matrix;
}

void initializeMatrix(int **matrix, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            matrix[i][j] = 1;
        }
    }
}

void deleteMatrix(int **matrix, int size)
{
    for(int i = 0; i < size; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void originalMultiplication(int **matrix1, int **matrix2, int **matrix3, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            for(int k = 0; k < size; k++)
            {
                matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

void blockedMultiplication(int **matrix1, int **matrix2, int **matrix3, int size, int block)
{
    for(int i = 0; i < size; i += block) {
        for(int j = 0; j < size; j += block) {
            for(int k = 0; k < size; k += block) {
                for(int x = 0; x < (i + block); x++) {
                    for(int y = 0; y < (j + block); y++) {
                        for(int z = 0; z < (k + block); z++) {
                            matrix3[x][y] += matrix1[x][z] * matrix2[z][y];
                        }
                    }
                }
            }
        }
    }
}

void trial1(int size)
{
    printf("Matrix size of %d x %d, original multiplication:\n", size, size);
    int **matrix1 = createMatrix(size);
    initializeMatrix(matrix1, size);
    int **matrix2 = createMatrix(size);
    initializeMatrix(matrix2, size);
    int **matrix3 = createMatrix(size);
    clock_t time_capture = clock();
    originalMultiplication(matrix1, matrix2, matrix3, size);
    time_capture = clock() - time_capture;
    double execution_time = ((double)time_capture)/CLOCKS_PER_SEC;
    printf("Execution time for trial 1: %f seconds\n", execution_time);
    deleteMatrix(matrix1, size);
    deleteMatrix(matrix2, size);
    deleteMatrix(matrix3, size);
}

void trial2()
{
    for(int i = MATRIX_SIZE_MIN; i <= MATRIX_SIZE_MAX; i *= 2)
    {
        trial1(i);
    }
}

void trial3(int size, int block)
{
    printf("Matrix size of %d x %d, block of size %d, blocked multiplication:\n", size, size, block);
    int **matrix1 = createMatrix(size);
    initializeMatrix(matrix1, size);
    int **matrix2 = createMatrix(size);
    initializeMatrix(matrix2, size);
    int **matrix3 = createMatrix(size);
    clock_t time_capture = clock();
    blockedMultiplication(matrix1, matrix2, matrix3, size, block);
    time_capture = clock() - time_capture;
    double execution_time = ((double)time_capture)/CLOCKS_PER_SEC;
    printf("Execution time for trial 1: %f seconds\n", execution_time);
    deleteMatrix(matrix1, size);
    deleteMatrix(matrix2, size);
    deleteMatrix(matrix3, size);
}

void trial4(int size, int block)
{
    trial3(size, block);
}

int main(int argc, char* argv[])
{
    if(strcmp(argv[1], "1") == 0)
    {
        printf("Beginning trial 1:\n");
        trial1(MATRIX_SIZE_MIN * 64);
    }
    else if(strcmp(argv[1], "2") == 0)
    {
        printf("Beginning trial 2:\n");
        trial2();
    }
    else if(strcmp(argv[1], "3") == 0)
    {
        printf("Beginning trial 3:\n");
        for(int i = MATRIX_SIZE_MIN; i <= MATRIX_SIZE_MAX; i *= 2)
        {
            trial3(i, BLOCK_SIZE_MIN * 2);
        }
    }
    else if(strcmp(argv[1], "4") == 0)
    {
        printf("Beginning trial 4:\n");
        for(int i = BLOCK_SIZE_MIN; i <= BLOCK_SIZE_MAX; i *= 2)
        {
            trial4(MATRIX_SIZE_MAX / 2, i);
        }
    }
    return 0;
}
