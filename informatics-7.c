#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sort(int array[], int arrayLength, int outputArray[]);
int isSorted(int array[], int arrayLength);
void printArray(int array[], int arrayLength);
void fillArray(int array[], int arrayLength);

// 5. Сортировка c предварительным выбором. Из массива путем однократного просмотра выбирается последовательность подряд расположенных элементов, находящихся в порядке возрастания, переносятся в выходной массив и заменяются во входном на -1. Затем оставшиеся элементы включаются в полученную упорядоченную последовательность методом погружения. Проверить эффективность на массивах, содержащих значения, возрастающие «с большой вероятностью».
int main() {
  int array[10] = {0};
  int arrayLength = sizeof(array)/sizeof(array[0]);
  fillArray(array, arrayLength);

  printArray(array, arrayLength);
  int* outputArray = (int *)calloc(arrayLength, sizeof(int));
  sort(array, arrayLength, outputArray);
  printArray(outputArray, arrayLength);
  if (isSorted(outputArray, arrayLength)) {
    printf("Sorted\n");
  } else {
    printf("Not sorted\n");
  }

  return 0;
}

//переставляет местами 2 элемента.
void swap(int* p1, int* p2) {
	int temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

void sort(int array[], int arrayLength, int outputArray[]) {
  int start = -1;
  int end = -1;
  int outputLength = 0;
  for (int i = 1; i < arrayLength; i++) {
    if (array[i] > array[i-1]) {
      start = i-1;
      outputArray[outputLength++] = array[i-1];
      array[i-1] = -1;
      for (int j = i+1; j < arrayLength; j++) {
        if (array[j] > array[j-1]) {
          end = j;
          outputArray[outputLength++] = array[j-1];
          array[j-1] = -1;
        } else {
          outputArray[outputLength++] = array[j-1];
          array[j-1] = -1; 
          break;
        }
      }
      break;
    }
  }
  //перенос оставшихся элементов 
  int i;
  for (i = 0; i < arrayLength; i++) {
  	if (array[i] != -1) {
  		outputArray[outputLength++] = array[i];
  		array[i] = -1;
  	}
  }
	// вставка погружением/ проверка выходного массива и престановка элементов в порядке возрастания
	for ( int i = 1; i < outputLength; i++) {
    // Пока не достигли " дна" или меньшего себя
    for ( int k = i; k !=0 && outputArray[k] < outputArray[k-1]; k--) {
      swap(&outputArray[k], &outputArray[k-1]);
    }
  }
}

void printArray(int array[], int arrayLength) {
  for (int i = 0; i < arrayLength; i++) {
    printf("%3d ", array[i]);
  }
  printf("\n");
}

void fillArray(int array[], int arrayLength) {
  srand(time(NULL));
  for (int i = 0; i < arrayLength; i++) {
    int r = rand(); 
    array[i] = rand() % 100;
  }
}

int isSorted(int array[], int arrayLength) {
  for (int i = 1; i < arrayLength; i++) {
    if (array[i-1] > array[i]) return 0;
  }

  return 1;
}

