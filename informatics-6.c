#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printSubstring(char string[], int start, int end) {
  for (int i = start; i <= end; i++) {
    printf("%c", string[i]);
  }
}

void printString(char* string) {
  int cursor = 0;
  while (string[cursor] != '\0') {
    printf("%c", string[cursor]);
    cursor++;
  }
}
void addPartOfString(char from[], int start, int end, char to[]) {
  int partLength = end - start + 1;
  int toLength = strlen(to);
  for (int i = start; i <= end; i++) {
    to[toLength++] = from[i];
  }
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

int findNextCharIndex(char symbol, int from, char string[]);
int findRepeatedFragment(char string[], int from, int* start, int* end);

//6. Найти в строке два одинаковых фрагмента (не включающих в себя пробелы) длиной более 5 символов,
//   скопировать их в выходную строку и удалить. Повторять этот процесс, пока такие фрагменты находятся.
//   Остаток строки добавить в выходную.
int main() {
  // char string[] = "\nBut I must explain to you how all this mistaken idea of denouncing pleasure and praising pain was born and I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master-builder of human happiness. No one rejects, dislikes, or avoids pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful. Nor again is there anyone who loves or pursues or desires to obtain pain of itself, because it is pain, but because occasionally circumstances occur in which toil and pain can procure him some great pleasure. To take a trivial example, which of us ever undertakes laborious physical exercise, except to obtain some advantage from it? But who has any right to find fault with a man who chooses to enjoy a pleasure that has no annoying consequences, or one who avoids a pain that produces no resultant pleasure?";
  char string[] = "123456 123456Hello123456 123456World123456 123456!";
  int stringLength = strlen(string);
  
  // Выделяем память для строки
  char* outputString = (char*)calloc(stringLength + 1, sizeof(char));
  if (!outputString) return -1;
  // В строке должен быть символ окончания строки
  outputString[0] = '\0';

  printf("Original string:\n");
  printString(string);
  printf("\n\n");

  int from = 0;
  while (1) {
    int start = -1;
    int end = -1;
    int res = findRepeatedFragment(string, from, &start, &end);
    if (res == -1) break;
    if (start != -1 && end != -1) {
      printf("Fragment: ");
      printSubstring(string, start, end);
      printf("\n");
      addPartOfString(string, start, end, outputString);
      deletePartOfString(string, start, end);
    }
    from = res;
  }  

  // Остаток строки добавляем в выходную
  strcat(outputString, string);
  printf("\nResult:\n");
  printString(outputString);
  printf("\n");

  free(outputString);
  return 0;
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

// находит фрагмент длиной более 5 в строке
int findFragment(char string[], int from, int* start, int* length) {
  int fragmentStart = 0;
  int fragmentLength = 0; 
  int cursor = from;
  // Ищем фрагмент длиной более 5 в строке
  while (1) {
    if (string[cursor] == '\0') {
      if (fragmentLength > 5) break;
      // Если достигнут конец строки и не найден фрагмент длиной больше 5
      return -1;
    }
    if (string[cursor] != ' ') {
      if (fragmentLength == 0) fragmentStart = cursor;
      fragmentLength++;
    } else if (fragmentLength > 5) {
      break;
    } else {
      fragmentStart = 0;
      fragmentLength = 0;
    }  
    cursor++;
  }
  *start = fragmentStart;
  *length = fragmentLength;
  return 0;
}

// Находит совпадающие фрагменты в строке, меняет входные переменные start и end
// на индексы первого фрагмента
// Если в строке нет фрагментов длиной более 5 символов, возвращает -1
int findRepeatedFragment(char string[], int from, int* start, int* end) {
  int fragmentStart = 0;
  int fragmentLength = 0;
  int res = findFragment(string, from, &fragmentStart, &fragmentLength);
  if (res == -1) return -1;

  // Первый символ фрагмента
  char firstSymbol = string[fragmentStart];
  int nextFragmentStart = findNextCharIndex(firstSymbol, fragmentStart + 6, string);
  int nextFragmentEnd = 0;
  int nextFragmentLength = 0;
  
  // Ищем совпадающие фрагменты
  while (1) {
    // Меняем firstSymbol
    if (nextFragmentStart == -1) {
      if (fragmentLength-1 > 5) {
        fragmentLength -= 1;
        fragmentStart += 1;
        firstSymbol = string[fragmentStart];
        nextFragmentStart = findNextCharIndex(firstSymbol, fragmentStart + 6, string);
        if (nextFragmentStart != -1 && nextFragmentStart < fragmentStart + fragmentLength - 1) {
          fragmentLength = nextFragmentStart - fragmentStart;
        } 
        continue;
      }
      return fragmentStart + fragmentLength - 1;
    }

    nextFragmentEnd = 0;
    nextFragmentLength = 0;

    // посимвольное сравнение
    int cursor = 0;
    int keepComparing = 1;
    while (1) {
      nextFragmentLength++;
      if (string[fragmentStart + cursor] == string[nextFragmentStart + cursor]) {
        if (fragmentLength == nextFragmentLength) {
          nextFragmentEnd = nextFragmentStart + cursor;          
          *start = fragmentStart;
          *end = fragmentStart + fragmentLength - 1;
          return fragmentStart;
        }
      } else {
        nextFragmentLength--;
        // Если длина совпадающего фрагмента больше 5
        if (nextFragmentLength > 5) {
          *start = fragmentStart;
          *end = fragmentStart + cursor - 1;
          return fragmentStart;
        }
        // Если символы не совпадают, ищем следующий подходящий символ и начинаем посимвольное сравнение сначала
        nextFragmentStart = findNextCharIndex(firstSymbol, nextFragmentStart+cursor, string);
        break;
      }

      cursor++;
    }
  }

  return -1;
}