#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int findFragment(char string[], int from, int* start, int* end);


void freeArray(char** words, int wordsAmount) {
  for (int i = 0; i < wordsAmount; i++) {
    free(*(words + i));
  }
  free(words);
}

// Функция получает строку текста и возвращает динамический массив указателей на слова. Каждое слово копируется в отдельный массив в динамической памяти.
char** splitString(char* string, int* wordsAmount) {
  *wordsAmount = 0;
  
  int wordPointers = 10;
  char** words = (char**)malloc(wordPointers * sizeof(char*));
  if (words == NULL) return NULL;
  
  int start = 0;
  int end = 0;
  int cursor = 0;
  while (findFragment(string, cursor, &start, &end) != -1) {
    char* word = (char*)malloc((end - start + 2) * sizeof(char));
    if (word == NULL) {
      free(words);
      return NULL;
    }
    
    strncpy(word, string + start, end - start + 1);
    word[end - start + 1] = '\0';
    
    if (wordPointers < *wordsAmount + 1) {
      words = (char**)realloc(words, wordPointers*2);
      wordPointers *= 2;
      if (words == NULL) {
        freeArray(words, *wordsAmount);
        return NULL;
      }
    }

    *(words + *wordsAmount) = word;
    (*wordsAmount)++;
    cursor = end + 1;
  }

  return words;
}

int main() {
  int wordsAmount = 0;
  char string[] = "Hello world! This is C.";
  printf("%s\n\n", string);
  char** words = splitString((string), &wordsAmount);
  if (words == NULL) return -1;
  
  for (int i = 0; i < wordsAmount; i++) {
    printf("%s\n", words[i]);
  }

  freeArray(words, wordsAmount);  
  return 0;
}

char chars[] = { ' ', '!', '?', ',', '.' };
int isPartOfWord(char chr) {
  for (int i = 0; i < sizeof(chars); i++) {
    if (chr == chars[i]) return 0;
  }
  return 1;
}

// Находит фрагмент в строке
int findFragment(char string[], int from, int* start, int* end) {
  int fragmentStart = -1;
  int fragmentEnd = -1; 
  int fragmentLength = -1;
  int cursor = from;
  // итерируемся по строке, пока не дойдём до её конца
  while (string[cursor] != '\0') {
    char currentSymbol = string[cursor];
    if (!isPartOfWord(currentSymbol) || currentSymbol == '\n') {
      if (fragmentLength > 0) {
        break;
      } else {
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
  if (fragmentLength > 0) {
    *start = fragmentStart;
    *end = fragmentEnd;
    return 0;
  }
  return -1;
}