#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void printArray(int array[], int arrayLength);
void fillArray(int array[], int arrayLength);

int* findMinimum(int array[], int arrayLength) {
  int* min = &array[0];

  for (int i = 1; i < arrayLength; i++) {
    // сравнение значений текущего и минимального эл-та
    if (array[i] < *min) {
      // если значение текущего эл-та меньше минимального, 
      // то присваиваем указателю ссылку на новое минимальное значение
      min = &array[i];
    }
  }

  return min;
}

void sort(int array[], int arrayLength) {
  for (int i = 0; i < arrayLength - 1; i++) {
    int* min = findMinimum(array + i, arrayLength - i);
    
    int tmp = array[i];
    array[i] = *min;
    *min = tmp;
  }
}

// 1. Функция находит минимальный элемент массива и возвращает указатель на него. 
// С использованием этой функции реализовать сортировку выбором.
int main() {
  int array[10] = {0};
  int arrayLength = sizeof(array)/sizeof(array[0]);

  fillArray(array, arrayLength);
  printArray(array, arrayLength);

  sort(array, arrayLength);
  
  printArray(array, arrayLength);

  return 0;
}

void printArray(int array[], int arrayLength) {
  for (int i = 0; i < arrayLength; i++) {
    printf("%3d ", array[i]);
  }
  printf("\n");
}

// Заполняет массив случайными числами от 0 до 100
void fillArray(int array[], int arrayLength) {
  srand(time(NULL));
  for (int i = 0; i < arrayLength; i++) {
    array[i] = rand() % 100;
  }
}
