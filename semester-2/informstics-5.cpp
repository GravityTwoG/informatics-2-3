#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int* toDigits(int length) {
    int i, n;
    for (i = 0, n = length; n != 0; i++, n = n / 10);
    int k = i--; // Количество цифр
    int *array = (int*)malloc(k * sizeof(int));
    for (n = length; n != 0; i--, n = n / 10)
        array[i] = n % 10; // Запись остатков в обратном порядке
    return array;
}

int isGood(unsigned int number) {
  printf("%u\n", number);
  if (number % 3 != 0) return 0;
  if (number % 7 != 0) return 0;
  
  return 1;
}

unsigned int findMinimum(int maxDigitsAmount, int digit, unsigned int number) {
  if (digit > maxDigitsAmount) return -1;
  
  unsigned int branch1 = number*10 + 3; 
  if (isGood(branch1)) return branch1;

  unsigned int branch2 = number*10 + 7; 
  if (isGood(branch2)) return branch2;


  unsigned int res = findMinimum(maxDigitsAmount, digit + 1, branch1);
  if (res != -1) return res;

  res = findMinimum(maxDigitsAmount, digit + 1, branch2);
  return res;
}


// 1. Тройки и семерки. Какое наименьшее число обладает тем свойством, что оно записывается только с помощью цифр 3 и 7 и что как оно, так и сумма его цифр делятся на 3 и 7? Например, 7 733 733 делится без остатка на 3 и на 7, но сумма его цифр (33) на 3 делится, а на 7 нет, поэтому оно не может служить решением задачи.
int main() {
  printf("Result: %d\n", findMinimum(12, 1, 0));
  return 0;
}
