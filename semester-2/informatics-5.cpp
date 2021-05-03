#include <stdio.h>

typedef unsigned long long int Result;

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

int digitsAmount(Result number) {
  int digits = 0;

  while (number > 0) {
    number = number / 10;
    digits++;
  }
  return digits;
}

Result findMinimum(int maxDigitsAmount, int digit, Result number) {
  if (digit > maxDigitsAmount) return 0;
  
  Result branch3 = number*10 + 3; 
  if (isGood(branch3)) return branch3;

  Result branch7 = number*10 + 7; 
  if (isGood(branch7)) return branch7;

  Result res1 = findMinimum(maxDigitsAmount, digit + 1, branch7);
  if (res1 != 0) {
    int res1Digits = digitsAmount(res1);
    Result res2 = findMinimum(res1Digits, digit + 1, branch3);

    if (res2 == 0) return res1;
    if (res1 <= res2) return res1;
    return res2;
  }

  Result res2 = findMinimum(maxDigitsAmount, digit + 1, branch3);
  return res2;
}

// 1. Тройки и семерки. Какое наименьшее число обладает тем свойством, что оно записывается только с помощью цифр 3 и 7 и что как оно, так и сумма его цифр делятся на 3 и 7? Например, 7 733 733 делится без остатка на 3 и на 7, но сумма его цифр (33) на 3 делится, а на 7 нет, поэтому оно не может служить решением задачи.
int main() {
  Result res = findMinimum(10, 1, 0);
  printf("\nResult = %u\n", res);
  return 0;
}
