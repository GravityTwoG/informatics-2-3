#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printString(char* string);
void addPartOfString(char source[], int start, int end, char destination[]);
void deletePartOfString(char string[], int start, int end);

int findFragment(char string[], int from, int* start, int* end);
int findNextCharIndex(char symbol, int from, char string[]);
int findRepeatedFragment(char string[], int from, int* start, int* end);

//6. Найти в строке два одинаковых фрагмента (не включающих в себя пробелы) длиной более 5 символов,
//   скопировать их в выходную строку и удалить. Повторять этот процесс, пока такие фрагменты находятся.
//   Остаток строки добавить в выходную.
int main() {
  char string[] = "123456 123456Hello123456 123456World123456 123456!";
  // char string[] = "thisisstringwithsubstring";
  // char string[] = "At server side, a software repository is typically managed by source control or repository managers. Some of the repository managers allow to aggregate other repository location into one URL and provide a caching proxy. When doing continuous builds many artifacts are produced and often centrally stored, so automatically deleting the ones which are not released is important.";
  int stringLength = strlen(string);
  
  char* outputString = (char*)calloc(stringLength + 1, sizeof(char));
  if (!outputString) return -1;
  outputString[0] = '\0';

  printf("Original string: %d characters\n", stringLength);
  printString(string);
  printf("\n");

  int from = 0;
  while (1) {
    int start = -1;
    int end = -1;
    // Ищем повторяющийся фрагмент
    int res = findRepeatedFragment(string, from, &start, &end);
    // В строке нет фрагментов длиной более 5
    if (res == -1) break;
    if (start != -1 && end != -1) {
      // Копируем фрагмент в выходную строку
      addPartOfString(string, start, end, outputString);
      // Удаляем фрагмент, 
      deletePartOfString(string, start, end);
      // в изначальной строке на месте удалённого фрагмента окажутся последующие символы 
      // мы продолжим поиск, начиная с этого символа
      from = start;
      continue;
    }
    from = res;
  }  

  strcat_s(outputString, stringLength+1, string);
  printf("\nResult:\n");
  printString(outputString);
  printf("\n");

  free(outputString);
  return 0;
}

// Добавляет подстроку из source в конец строки destination
void addPartOfString(char source[], int start, int end, char destination[]) {
  int partLength = end - start + 1;
  int toLength = strlen(destination);
  for (int i = start; i <= end; i++) {
    destination[toLength++] = source[i];
  }
  destination[toLength] = '\0';
}
// Удаляет часть строки
void deletePartOfString(char string[], int start, int end) {
  int stringLength = end - start + 1;
  int cursor = start;
  do {
    string[cursor] = string[cursor + stringLength];
    cursor++;
  } while (string[cursor + stringLength] != '\0');
  string[cursor] = '\0';
}

// Возвращает индекс символа в строке, найденного после индекса from
// или -1 если такого символа нет
int findNextCharIndex(char symbol, int from, char string[]) {
  int cursor = from;
  while (1) {
    if (string[cursor] == '\0') return -1;
    if (string[cursor] == symbol) return cursor;
    cursor++;
  }
}

// Находит фрагмент длиной более пяти в строке
int findFragment(char string[], int from, int* start, int* end) {
  int fragmentStart = -1;
  int fragmentEnd = -1; 
  int fragmentLength = -1;
  int cursor = from;
  // итерируемся по строке, пока не дойдём до её конца
  while (string[cursor] != '\0') {
    char currentSymbol = string[cursor];
    if (currentSymbol == ' ' || currentSymbol == '\n') {
      if (fragmentLength > 5) {
        break;
      } else {
        fragmentStart = -1;
        fragmentEnd = -1;
        fragmentLength = -1;
        cursor++;
        continue;
      }
    }
    if (fragmentLength == -1) fragmentStart = cursor;
    fragmentEnd = cursor;
    fragmentLength = fragmentEnd - fragmentStart + 1;
    cursor++;
  }
  if (fragmentLength > 5) {
    *start = fragmentStart;
    *end = fragmentEnd;
    return 0;
  }
  return -1;
}

// Проверяет содержатся ли в строке одинаковые фрагменты длиной более пяти,
// если да, то возвращает индекс последнего символа первого фрагмента
int findEqualFragments(char string[], int f1Start, int f2Start) {
  int f2End = 0;
  int f2Length = 0;
  int firstSymbol = string[f1Start];
  int cursor = 0;
  int keepComparing = 1;
  while (1) {
    // Если символы не совпадают или встречается конец строки/пробел/перевод строки
    if (string[f1Start + cursor] != string[f2Start + cursor] ||
        string[f1Start + cursor] == ' '  || string[f2Start + cursor] == ' ' ||
        string[f1Start + cursor] == '\n' || string[f2Start + cursor] == '\n' ||
        string[f1Start + cursor] == '\0' || string[f2Start + cursor] == '\0') {
      // Если длина совпадающего фрагмента больше 5
      if (f2Length > 5) {
        // Возвращаем индекс посл. символа первого фрагмента
        return f1Start + cursor - 1;
      }
      // иначе ищем дальше
      f2Start = findNextCharIndex(firstSymbol, f2Start+cursor, string);
      f2Length = 0;
      if (f2Start == -1) return -1;
      cursor = 0;
      continue;
    }
    f2Length++; 
    cursor++;
  }

  return -1;
}

// Находит совпадающие фрагменты в строке, меняет входные переменные start и end
// на индексы первого фрагмента.
// Если в строке нет фрагментов длиной более 5 символов, возвращает -1.
// Если есть фрагмент длиной более пяти, но без повторений, то возвращает индекс последнего символа.
int findRepeatedFragment(char string[], int from, int* start, int* end) {
  int fragmentStart = 0;
  int fragmentEnd = 0;
  int fragmentLength = 0;
  // Находим фрагмент длиной более пяти
  int res = findFragment(string, from, &fragmentStart, &fragmentEnd);
  if (res == -1) return -1;

  fragmentLength = fragmentEnd - fragmentStart + 1;
  char firstSymbol = string[fragmentStart];
  // Определяем символ с которого нужно начать поиск одинаковых фрагментов
  int nextCharIndex = findNextCharIndex(firstSymbol, fragmentStart + 6, string);
  
  // Ищем повторения этого фрагмента
  while (1) {
    if (nextCharIndex == -1) {
      // Меняем firstSymbol, уменьшаем длину рассматриваемого фрагмента
      if (fragmentEnd - fragmentStart > 5) {
        fragmentLength -= 1;
        fragmentStart += 1;
        firstSymbol = string[fragmentStart];
        nextCharIndex = findNextCharIndex(firstSymbol, fragmentStart + 6, string);
        if (nextCharIndex != -1 && nextCharIndex < fragmentEnd) {
          fragmentLength = nextCharIndex - fragmentStart;
        }
        continue;
      }
      return fragmentStart + fragmentLength - 1;
    }

    // Ищем одинаковые фрагменты длиной более пяти
    int searchRes = findEqualFragments(string, fragmentStart, nextCharIndex);
    if (searchRes == -1) {
      nextCharIndex = -1;
      continue;
    } else {
      *start = fragmentStart;
      *end = searchRes;
      return searchRes;
    }
  }

  return -1;
}

void printString(char* string) {
  int cursor = 0;
  while (string[cursor] != '\0') {
    printf("%c", string[cursor]);
    cursor++;
  }
}
