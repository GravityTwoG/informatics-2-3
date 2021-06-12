#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void fillArray(int array[], int arrayLength);

typedef int (*CompareFunction)(void*, void*);

void** findMinMax(
  void** array, 
  int arrayLength, 
  CompareFunction compare) {
  void* minMax = array[0];
  int minMaxI = 0;
  for (int i = 1; i < arrayLength; i++) {
    int res = compare(array[i], minMax);
    if (res > 0) {
      minMax = array[i];
      minMaxI = i;
    }
  }
  return array + minMaxI; 
}


void sort(void** array, int arrayLength, CompareFunction compare) {
  for (int i = 0; i < arrayLength - 1; i++) {
    void** min = findMinMax(array + i, arrayLength - i, compare);
    
    void *tmp = array[i];
    array[i] = *min;
    *min = tmp;
  }
} 

int compareInts(void *e1, void *e2) {
  int i1 = *(int*)e1;
  int i2 = *(int*)e2;
  return i1 <= i2;
}

int compareWords(void *e1, void *e2) {
  char* w1 = (char*)e1;
  char* w2 = (char*)e2;
  return strcmp(w2, w1);
}

// Преобразовать функцию сортировки с использованием массивов (b), списков (6.3), деревьев (8.4, 8.5) в итератор. Проверить его работу на двух структурах данных содержащих указатели на различные типы (например, целые и строки). Массив преобразовать в массив указателей
int main() { 
  int A[10];
  int length = sizeof(A)/sizeof(A[0]);
  fillArray(A, length);

  int **array = (int**)malloc(sizeof(int*) * length);
  for (int i = 0; i < length; i++) {
    array[i] = &A[i];
  }

  for (int i = 0; i < length; i++) printf("%d ", *array[i]);
  printf("\n");
  sort((void**)array, length, compareInts);
  for (int i = 0; i < length; i++) printf("%d ", *array[i]);
  printf("\n");

  free(array);

  char* words[] = {"hello", "world", "xyz", "abc", "abd"};  
  int wordsAmount = 5;

  for (int i = 0; i < wordsAmount; i++) {
    printf("%s, ", words[i]);
  }
  sort((void**)words, wordsAmount, compareWords);
  printf("\n");
  for (int i = 0; i < wordsAmount; i++) {
    printf("%s, ", words[i]);
  }

  return 0;
}

// Заполняет массив случайными числами от 0 до 100
void fillArray(int array[], int arrayLength) {
  srand(time(NULL));
  for (int i = 0; i < arrayLength; i++) {
    array[i] = rand() % 100;
  }
}