#include <stdio.h>
#include <windows.h>

int toDigit(int A[], int a);
int reverseInt(int number);
void task6();
void task16();
int task11(int input[],int output[], int inputLength);
void task19();

int main() {
    task6();
    // task16();
    system("pause");
}

int toDigit(int array[], int length) {
    int i, n;

    for (i = 0, n = length; n != 0; i++, n = n / 10);

    int k = i--; // Количество цифр

    for (n = length; n != 0; i--, n = n / 10)
        array[i] = n % 10; // Запись остатков в обратном порядке с конца

    return k;
}

// Переворачивает число.
int reverseInt(int number) {
    int result = 0;
    for (; number != 0; number /= 10) {
        int lastDigit = number % 10;
        result = result * 10 + lastDigit;         
    }

    return result; 
}
// Возвращает 0, если число составное, 1, если число простое.
int isPrime(int number) {
    int n = 2; // делитель
    while (number % n != 0) {
        n++;
        if (n == number) return 1;
    }
    
    return 0;
}

//6. Прогулка по улице. Мегамозг живет и работает на одной улице. Из дома до работы он ходит пешком. Однажды он сосчитал сумму номеров домов, которые он проходил мимо (включая номер дома и номер здания, где работает). В сумме он получил 297. Назовите номера домов, в которых живет и работает Мегамозг, с учетом того, что между домом и работой Мегамозга не менее 8 домов (не включая дом и работу) и дома он считал только по одной стороне улицы. Расположение домов на улицах стандартное, в номерах домов нет индексов-букв.
void task6() {
    int sum;
    int homeAddress;
    int workAddress;

    // Мегамозг живёт на улице с нечётными номерами,
    // так как сумма номеров нечётная, а дома он считал только по одной стороне улицы.
    // Перебираем каждое нечётное значение дома от 1 до Z, почему Z надо обосновать  
    // с количеством домов, которые он проходил мимо от 8 до X, почему X надо обосновать
    for (homeAddress = 1; homeAddress < 297; homeAddress +=2) {
        int startWithNewHomeAddress = 0;
        // Считаем сумму при текущем начальном доме и разном кол-ве домов.
        for (int otherHousesAmount = 8; otherHousesAmount < 100 && !startWithNewHomeAddress; otherHousesAmount++) {
            printf("Way: (%d) ", homeAddress);
            sum = homeAddress;
            int house;
            // Сумма номеров домов, которые Мегамозг прошёл мимо
            int otherHouses = 0;
            // Добавляем все номера домов между homeAddress и workAddress
            for (house = homeAddress+2; house < homeAddress+otherHousesAmount*2+1; house+=2) {
                sum += house;
                otherHouses += house;
                printf("%d ", house);
            }

            workAddress = house;
            printf("(%d). ", workAddress);
            sum += workAddress;
            printf("Sum: %d ", sum);
            printf("otherHousesAmount: %d ", otherHousesAmount);

            if (sum == 297) {
                printf("Answer is found: ");
                printf("Home: №%d ", homeAddress);
                printf("Work: №%d\n\n", workAddress);
            }
            // Прерываем вычисления, если при таком homeAddress и 8 домами
            // сумма превышает нужную.
            // При дальнейшем увеличении homeAddress и кол-ва домов, ответ мы всё равно не найдём
            if (otherHousesAmount == 8 && sum > 297) {
                printf("Stopping computations. Sum of houses bigger than needed\n");
                return;
            }
            if (sum > 297) {
                printf("Wrong way\n\n");
                // Переходим на следующий homeAddress, т.к при текущем homeAddress
                // и количестве домов сумма получается слишком большой
                startWithNewHomeAddress = 1;
            }
            printf("\n");
        }
    }
}

//16. Числовой ребус: Найдите числа, зашифрованные словами КУБ и БУК, если известно, что число КУБ - действительно является кубом некоторого числа, а БУК - простое число.
void task16() {
    for (int i = 5; i < 10; i++) {
        int digits[3] = {0};
        toDigit(digits ,i*i*i); // Раскладываем число в массив цифр
        
        // В числе не должно быть одинаковых цифр
        if (digits[0] == digits[1] || 
            digits[0] == digits[2] ||
            digits[1] == digits[2] ) {
                continue;
        }

        int reversedCube = reverseInt(i*i*i); // Переворачиваем число
        if (isPrime(reversedCube)) { // Проверяем простое ли число получилось
            printf("%d %d\n", i*i*i, reversedCube);
            printf("Answer is: %d\n", i*i*i);
        }
    }
}

//11. Найти в массиве все последовательности возрастающих элементов и копировать в выходной массив.
// int A[10] = {0,1,2,3,4,4,4,6,4};
// int B[10];
// int bLength = task11(A,B,10);
// for (int i = 0; i < bLength; i++) printf("%d ", B[i]);
int task11(int input[],int output[], int inputLength) {
    int outputLength = 0;

    for (int i = 1; i < inputLength; i++) {
        int start = -1;
        int end = -1;
        if (input[i] > input[i - 1]) { // вычисление начала и конца возрастающей последовательности
            start = i - 1;
            int k;
            for (k = i; k < inputLength && input[k] > input[k-1]; k++);
            end = k;
            i = end; // при след. итерации i > конца возрастающей последовательности
        }
        if (start != -1 && end != -1) { // копирование возрастающей последовательности
            for (int j = start; j < end; j++) output[outputLength++] = input[j];
        }

    }

    return outputLength;
}

// 19. ОДИН + ОДИН = МНОГО
void task19() {
    int vv;                   // Проверяемое число

    int A1[10], B1[10];       // Разложение по цифрам ОДИН и МНОГО

    int M[10];                // Массив признаков = цифра БЫЛА

    for (vv = 5000; vv <= 9999; vv++) {     // Перебор диапазона 5-значных чисел

        toDigit(A1, vv);      // Разложение ОДИН

        for (int j = 0; j < 10; j++) M[j] = 0;      // Очистить массив признаков

        int retr = 0;         // Признак - повторение цифры

        for (int j = 0; j < 4; j++) {
            int k = A1[j];    // Очередная цифра

            if (M[k] != 0) {  // Цифра уже была

                retr = 1; break;

            }

            else M[k] = 1;    // Отметить, что была

        }

        if (retr) continue;   // Повторение цифр

        toDigit(B1, 2 * vv);  // Разложение МНОГО

        if (B1[2] != B1[4]) continue;// Нет двух букв О

        if (M[B1[0]]) continue;// цифра на месте М уже была в ОДИН
        if (M[B1[3]]) continue;// цифра на месте Г уже была в ОДИН

        if (A1[0] != B1[2]) continue;  // буква О должна быть в (О)ДИН и МН(О)ГО

        if (A1[0] != B1[4]) continue;  // буква О должна быть в (О)ДИН и МНОГ(О)

        if (A1[3] != B1[1]) continue;  // буква Н должна быть в ОДИН и МНОГО

        if (B1[0] == B1[1]) continue;  // цифры на месте М и Н - д.б. разные
        if (B1[0] == B1[2]) continue;  // цифры на месте М и О - д.б. разные
        if (B1[0] == B1[3]) continue;  // цифры на месте М и Г - д.б. разные
        if (B1[1] == B1[2]) continue;  // цифры на месте Н и О - д.б. разные
        if (B1[2] == B1[3]) continue;  // цифры на месте О и Г - д.б. разные


        printf("%d+%d=%d\n", vv, vv, 2 * vv);
    }
}