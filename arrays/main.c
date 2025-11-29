#include <stdio.h>
#define N 6

// номер 1
void print_matrix(const int arr[][N]){ // const int(*arr)[N] , const int arr[N][N]
    for (int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%d\t", arr[i][j]);
        }
        printf("\n");

    }
    printf("\n");
}

// номер 1
void fill_matrix(int arr[][N]){
    for (int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            arr[i][j] = i * N + j + 1; 
        }
    }
}

//void fill_matrix_v2(int* arr)

//номер 2
void fill_array(int arr[]){
    for (int i = 0; i < N; i++)
        arr[i] = i + 1;
}

//номер 2
void print_reverse_array(const int arr[]){
    int temp_array[N];
    for(int i = 0; i < N; i++){
        temp_array[i] = arr[N-1-i];
        printf("%d ", temp_array[i]);
    }
    printf("\n");
}

//номер 3
void fill_triangle_in_matrix(int arr[][N]){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(j < N - 1 - i)
                arr[i][j] = 0;
            else
                arr[i][j] = 1;
        }
    }
}

//номер 4
void fill_matrix_spiral(int arr[][N]){ 
    int count_iter = 0;
    int value = 1;
    int top = 0, bottom = N - 1;
    int left = 0, right = N - 1;
    // int example[3][3] = {
    //     {1, 2, 3},
    //     {8, 9, 4},
    //     {7, 6, 5}
    // };
    // int example[4][4] = {
    //     { 1,  2,  3, 4},
    //     {12, 13, 14, 5},
    //     {11, 16, 15, 6},
    //     {10,  9,  8, 7}
    // };
    while(value <= N*N){
        for(int i = left; i <= right; i++){ //направо
            arr[top][i] = value++;
        }
        top++;
        for(int i = top; i <= bottom; i++){//вниз
            arr[i][right] = value++;
        }
        right--;
        for(int i = right; i >= left; i-- ){//влево
            arr[bottom][i] = value++;
        }
        bottom--;
        for(int i = bottom; i >= top; i--){ //вверх
            arr[i][left] = value++;
        }
        left++;

        count_iter++;
    }
    printf("count_iter: %d\n", count_iter);
}

int main(){
    // номер 1
    printf("1. Вывести квадратную матрицу по заданному N:\n");
    int matrix[N][N];
    fill_matrix(&matrix[0]); //matrix
    print_matrix(&matrix[0]);
    printf("-------------------------------\n");

    //номер 2
    printf("2. Вывести заданный массив размером N в обратном порядке:\n");
    int array[N];
    fill_array(&array[0]); //array
    for(int i = 0; i < N; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
    print_reverse_array(&array[0]);
    printf("-------------------------------\n");

    //номер 3
    printf("3. Заполнить верхний треугольник матрицы 0 а нижний 1:\n");
    int array2[N][N];
    fill_triangle_in_matrix(array2);
    print_matrix(array2);
    
    printf("-------------------------------\n");

    //номер 4
    printf("4. Заполнить матрицу числами от 1 до N^2 спиралькой:\n");

    int array3[N][N];
    fill_matrix_spiral(array3);
    print_matrix(array3);

    printf("-------------------------------\n");

    return 0;
}