#include <stdio.h>
#include <stdlib.h>

int readInt(int *number) {
  int res = scanf("%d", number);
  scanf("%*[^\n]");
  scanf("%*c");
  return res;
}
int readFloat(float *number) {
  int res = scanf("%f", number);
  scanf("%*[^\n]");
  scanf("%*c");
  return res;
}

int readMatrix(char* memory, int memorySize, int pos) {
  int m = 0;
  int n = 0;
  puts("Введите количество строк:");
  readInt(&m);
  puts("Введите количество столбцов:");
  readInt(&n);

  if (m <= 0 || n <= 0) return pos;
  
  if ((m*n*sizeof(float) + 2*sizeof(int) + pos) > memorySize) {
    puts("Недостаточно места в памяти.");
    return -1;
  }
  
  int cursor = pos;
  *(int*)(memory + pos) = m;
  *(int*)(memory + pos + sizeof(int)) = n;
  cursor += 2*sizeof(int);

  float x = 0.0;
  int N = 0;

  int lastDigit = m*n % 10;
  if (lastDigit == 1) {
    printf("Введите %d число:\n", m*n);
  } else if (lastDigit == 2 || lastDigit == 3 || lastDigit == 4) {
    printf("Введите %d числа:\n", m*n);
  } else {
    printf("Введите %d чисел:\n", m*n);
  }
  while (N < m*n) {
    int res = readFloat(&x);
    if (res == 0) {
      puts("Произошла ошибка, введите число заново.");
      continue;
    }
    *(float*)(memory + cursor) = x;
    cursor += sizeof(float);
    N++;
  }
  return cursor;
}

int printMatrix(char* memory, int memorySize, int pos) {
  int cursor = pos;

  int m = *(int*)(memory+cursor);
  int n = *(int*)(memory+cursor+sizeof(int));
  cursor += 2*sizeof(int);
  
  if (m <= 0 || n <= 0) return -1;
  printf("Матрица %d x %d:\n", m, n);

  int N = 0;
  while (N < m*n) {
    printf("%f ", *(float*)(memory + cursor));
    N++;
    if (N % n == 0) printf("\n");
    cursor += sizeof(float);  
  }
  printf("\n");
  
  return cursor;
}

void printMatrices(char* memory, int memorySize) {
  int pos = 0;
  while (pos < memorySize) {
    pos = printMatrix(memory, memorySize, pos); 
    if (pos == -1) return;
  }
}

//Разработать две функции, одна из которых вводит с клавиатуры набор данных в произвольной последовательности и размещает в памяти в заданном формате. Другая функция читает эти данные и выводит на экран. Программа запрашивает и размещает в памяти несколько наборов данных при помощи первой функции, а затем читает их и выводит на экран при помощи второй. Размещение данных производить в выделенном массиве байтов с контролем его переполнения.
//1. Последовательность прямоугольных матриц вещественных чисел, предваренная двумя целыми переменными - размерностью матрицы.
int main() {
  int memorySize = 240;
  char* memory = (char*)malloc(memorySize);
  if (memory == NULL) return -1;
  int wpos = 0;

  int action = 0;
  while (1) {
    puts("Выберите действие: 0 - Выход, 1 - Ввод данных, 2 - Вывод данных.");
    readInt(&action);
    
    if (action == 0) return 0;
    if (action == 1) {
      wpos = readMatrix(memory, memorySize, wpos); 
      if (wpos == -1) return -1;
    }
    if (action == 2) printMatrices(memory, memorySize);
  }
  return 0;
}