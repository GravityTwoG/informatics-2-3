#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printString(char* string);
int expandStringSize(char* string, int newSize);
void deletePartOfString(char string[], int start, int end);
void addPartOfString(char string[], int start, int end, char targetString[]);

int findNextCharStart(char symbol, int from, char string[]);
int findRepeatedFragment(char string[], int from, int* start, int* end);

//6. Найти в строке два одинаковых фрагмента (не включающих в себя пробелы) длиной более 5 символов,
//   скопировать их в выходную строку и удалить. Повторять этот процесс, пока такие фрагменты находятся.
//   Остаток строки добавить в выходную.
int main() {
  char string[] = "Lorem loreet, ipsum dolor sit amet,#startregion consectetur adipiscing loreet, #pragma endregion, adipiscing el";
  
  int outputStringLength = 10;
  int outputStringContains = 0;
  // Создаем указатель на первый элемент массива символов длиной outputStringLength + 1
  char* outputString = (char*)calloc(outputStringLength + 1, 1);
  if (!outputString) return -1;
  // В строке должен быть символ окончания строки
  outputString[0] = '\0';

  int from = 0;
  while (1) {
    int start = -1;
    int end = -1;
    // Находим в строке повторяющийся фрагмент
    int res = findRepeatedFragment(string, from, &start, &end);
    if (res == -1) break;
    if (start != -1 && end != -1) {
      int repeatedFragmentLength = end - start + 1;
      // Увеличиваем размер выходной строки если необходимо
      if (outputStringContains + repeatedFragmentLength > outputStringLength) {
        int newSize = outputStringContains + repeatedFragmentLength > outputStringLength*2
          ? (outputStringContains + repeatedFragmentLength)*2
          : outputStringLength*2;
        int resizeRes = expandStringSize(outputString, outputStringLength*2);
        if (resizeRes == -1) return -1;
        outputStringLength *= 2;
      }

      // Копируем фрагмент в выходную строку 
      addPartOfString(string, start, end, outputString);
      outputStringContains += repeatedFragmentLength;

      // Удаляем фрагмент из строки 
      deletePartOfString(string, start, end);
      printString(outputString);
      printf("\n");
    }
    from = res;
  }  

  int stringLength = strlen(string);
  if (outputStringContains + stringLength > outputStringLength) {
    int res = expandStringSize(outputString, outputStringContains + stringLength);
    if (res == -1) return -1;
  }

  // Остаток строки добавляем в выходную
  strcat(outputString, string);
  // addPartOfString(string, 0, strlen(string), outputString);
  printf("Result:\n");
  printString(outputString);

  // free(outputString);
  return 0;
}

void printString(char* string) {
  int cursor = 0;
  while (string[cursor] != '\0') {
    printf("%c", string[cursor]);
    cursor++;
  }
}

// Увеличивает размер строки до newSize
int expandStringSize(char* string, int newSize) {
  char* tmp = (char*)realloc(string,  newSize);
  if (!tmp) return -1;
  string = tmp;
  int stringLength = strlen(string);
  for (int i = stringLength+1; i < newSize; i++) {
    string[i] = ' ';
  }
  return 0;
}

void addPartOfString(char from[], int start, int end, char to[]) {
  int partLength = end - start + 1;
  int toLength = strlen(to);
  for (int i = start; i <= end; i++) {
    to[toLength++] = from[i];
  }
  to[toLength] = '\0';
}

// Удаляет часть строки
void deletePartOfString(char string[], int start, int end) {
  int cursor = 0;
  while (string[end + cursor + 1] != '\0') {
    string[start + cursor] = string[end + cursor + 1];
    cursor++;
  }
  string[start + cursor + 1] = '\0';
}

// Возвращает начальный индекс символа в строке, после индекса from
int findNextCharStart(char symbol, int from, char string[]) {
  int cursor = from + 1;
  while (1) {
    if (string[cursor] == '\0') return -1;
    if (string[cursor] == symbol) return cursor;
    cursor++;
  }
}

// Находит совпадающие фрагменты в строке, меняет входные переменные start и end
// на индексы первого фрагмента
// Если в строке нет фрагментов длиной более 5 символов, возвращает -1
int findRepeatedFragment(char string[], int from, int* start, int* end) {
  int fragmentStart = 0;
  int fragmentEnd = 0;
  int fragmentLength = 0;
  int cursor = from;
  // Ищем фрагмент длиной более 5 в строке
  while (1) {
    if (string[cursor] == '\0') {
      if (fragmentLength > 5) {
        fragmentEnd = cursor - 1;
        break;
      }
      // Если достигнут конец строки и не найден фрагмент длиной больше 5
      return -1;
    }

    if (string[cursor] != ' ') {
      if (fragmentLength == 0) fragmentStart = cursor;
      fragmentEnd = cursor;
      fragmentLength++;
    } else if (fragmentLength > 5) {
      break;
    } else {
      fragmentStart = 0;
      fragmentLength = 0;
      fragmentEnd = 0;
    } 
    
    cursor++;
  }
  // Первый символ фрагмента
  char firstSymbol = string[fragmentStart];

  // Находим индекс похожего символа в строке
  int nextFragmentStart = findNextCharStart(firstSymbol, fragmentEnd, string);
  // Если после фрагмента больше не осталось других фрагментов
  if (nextFragmentStart == -1) {
    return -1;
  }

  // Ищем совпадающие фрагменты
  while (1) {
    // Если фрагментов дальше нет, то прерываем цикл и выполнение функции
    if (nextFragmentStart == -1) {
      fragmentLength--;
      if (fragmentLength > 5) {
        fragmentStart++;
        firstSymbol = string[fragmentStart];
        nextFragmentStart = findNextCharStart(firstSymbol, fragmentEnd, string);
        continue;
      }
      return fragmentEnd;
    }
    int nextFragmentEnd = 0;
    int nextFragmentLength = 0;

    int cursor = 0;
    int keepChecking = 1;
    while (keepChecking) {
      nextFragmentLength++;
      if (string[fragmentStart + cursor] == string[nextFragmentStart + cursor]) {
        if (fragmentLength == nextFragmentLength) {
          nextFragmentEnd = nextFragmentStart + cursor;          
          *start = fragmentStart;
          *end = fragmentEnd;
          return fragmentEnd;
        }
      } else {
        nextFragmentStart = findNextCharStart(firstSymbol, nextFragmentStart+cursor, string);
        keepChecking = 0;
      }
      cursor++;
    }
  }

  return -1;
}