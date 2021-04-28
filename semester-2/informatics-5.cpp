#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Result unsigned long int

int isGood(Result number) {
  if (number % 3 != 0) return 0;
  if (number % 7 != 0) return 0;

  int sum = 0;
  unsigned int n = number;
  while (n > 0) {
    sum += n % 10;
    n = n / 10;
  }

  if (sum % 3 != 0) return 0;
  if (sum % 7 != 0) return 0;
  
  return 1;
}

Result findMinimum(int maxDigitsAmount, int digit, Result number) {
  if (digit > maxDigitsAmount) return -1;
  
  Result branch3 = number*10 + 3; 
  if (isGood(branch3)) return branch3;

  Result branch7 = number*10 + 7; 
  if (isGood(branch7)) return branch7;

  Result res = findMinimum(maxDigitsAmount, digit + 1, branch7);
  Result res2 = findMinimum(maxDigitsAmount, digit + 1, branch3);
  if (res2 == -1) return res;

  if (res <= res2) return res;
  return res2;
}

// 1. Тройки и семерки. Какое наименьшее число обладает тем свойством, что оно записывается только с помощью цифр 3 и 7 и что как оно, так и сумма его цифр делятся на 3 и 7? Например, 7 733 733 делится без остатка на 3 и на 7, но сумма его цифр (33) на 3 делится, а на 7 нет, поэтому оно не может служить решением задачи.

// Наименьшим возможным числом будет 3 333 377 733. Оно делится на 3 и на 7, и тем же свойством обладает сумма его цифр (42). Число должно содержать по крайней мере 3 семерки и 7 троек, причем семерки следует перенести как можно дальше вправо
int main() {
  printf("\nResult: %u\n", findMinimum(10, 1, 0));
  return 0;
}
