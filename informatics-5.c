#include <stdio.h>
#include <math.h>

/*
  Для  заданного  варианта  написать функцию  вычисления суммы ряда:
  1. стандартный диапазон значений x =0.1...0.9, шаг  0.1. Убедиться, что на этом диапазоне элемент суммы ряда стремится к 0, при необходимости скорректировать диапазон;

  2. вычислить значения суммы ряда  и  контрольной функции, к которой он сходится, с  точностью  до  4 знаков после запятой;

  3. исходя из полученного значения n и формулы ряда объяснить различную скорость сходимости для разных x;

  Вариант 6: ряд: 1 + x^2/2! + x^4/4! +…+ x^2n/(2n)! функция: ch(x)
*/

unsigned long long int factorial(int number);
double sequenceElement(float x, int n);
double sequence(float x, int n);

int main() {
  int n = 2; // Кол-во элементов в ряду  

  printf("|  Sequence sum  |   ch(x)  |   Diff   |\n");
  for (float x = 0.1; x <= 0.9; x += 0.1) {
    double sequenceSum = sequence(x, n);
    printf("|  %12.4f  ", sequenceSum);
    printf("|  %5.4f  ", cosh(x));
    printf("|  %5.4f  |", abs(sequenceSum - cosh(x)));
    printf("\n");
  }

  return 0;
}

// Возвращает факториал числа
unsigned long long int factorial(int number) {
  unsigned long long int product = 1;
  while (number != 0) {
    product *= number;
    number--;
  }
  
  return product;
}

// Возвращает значение n-ного элемента ряда x^2n / (2n)!
double sequenceElement(float x, int n) {
  double xPow = pow(x, 2*n);
  unsigned long long int currentFactorial = factorial(2*n);
  return xPow/currentFactorial;
}

// Возвращает сумму ряда при данном х и n
double sequence(float x, int n) {
  double sum = 1;
  for (int i = 1; i <=n; i++) {
    double res = sequenceElement(x, i);
    sum += res;
  }

  return sum;
}