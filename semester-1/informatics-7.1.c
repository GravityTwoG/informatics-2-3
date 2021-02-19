#include <stdio.h>
#include <time.h>
#include <math.h>

void sort(int n);//функция сортировки
#define N 10
int enter[N];//Входной массив int exit[10]
int ex[N];// Выходной массив

void sort(int n)
{
	int amax, t = 0, i, j, u=0, m=0, s=0; //максимальный элемент, номер макс элемента
		amax = enter[0]; //записываем нулевой элемент как максимальный
		for (i = 0; i < n; i++, u++) //u - счетчик количества шагов цикла
		{
		if (enter[i] > amax) //если значение больше максимального 
		{
	    amax = enter[i]; //заменяем максимальное значение
		ex[t++] = amax;//переносим элемент во второй массив
		m++; //количество перемещенных элементов
		enter[i] = -1; //соответсвенно в первыый массив заносим - 1
		}
		}
		for (i = 0; i < n; i++)
			//проверка оставшихся элементов и перенос их.
		{
			if (enter[i] != -1)
			{
				ex[t] = enter[i];
				for (int k = t; k != 0 && ex[k] < ex[k - 1]; k--, u++) {
					amax = ex[k]; ex[k] = ex[k - 1]; ex[k - 1] = amax; s++;
				}
				t++;
			}
		}
	printf("%d ", s); //количество погружений
	printf("%d ", m); //количество перемещенных
	printf("%d \n\n", u); //количество шагов цикла
}

int main()
{
	int k, i, n=N;
	for (i = 0; i < n; i++) enter[i] = rand() % 1000;
	printf("Neotsortirovaniy massiv\n");
	for (k = 0; k < n; k++) //обычный цикл с десятью итерациями (нужен для вывода входного массива)
		printf("%d ", enter[k]); //вывод элементов входного массива
		printf("\n\n");
	sort(n);// вызов функции сортировки
	printf("Otsortirovaniy massiv \n");
	for (int i = 0; i < n; i++) //почти такой же цикл для вывода выходного массива
		printf("%d ", ex[i]); //соответственно вывод
		printf("\n\n");


}