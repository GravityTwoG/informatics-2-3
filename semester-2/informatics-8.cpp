#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Symbol {
  char letter = ' ';
  int amount = 0;
  unsigned char code = 0b0;
  unsigned char size = 4;
};

typedef int (*CompareFunction)(void*, void*);

void sort(void** array, int arrayLength, CompareFunction compare);
int compareSymbols(void* a, void* b);
void printBinary(unsigned char number, unsigned char bits);

Symbol* createSymbol(char letter, unsigned char bit, int amount);
Symbol* findChar(Symbol** chars, int charsAmount, char letter);
Symbol** buildCharactersList(char* string, int* charsAmount);

char* pack(char string[], int length, Symbol** chars, int charsAmount);
int unpack(char* string, Symbol** chars);

//13. Первые 15 наиболее часто встречающихся символов кодируются 4-битными кодами от 0000 до 1110. Код 1111 обозначает, что следующие за ним 8 бит кодируют один из остальных символов. Разработать функции упаковки и распаковки строки с определением наиболее часто встречающихся символов и коэффициента уплотнения.
int main() {
  char string[] = "Sentence for test.";
  int length = strlen(string);
  
  // Строим массив Symbol с символами и кол-вом их вхождений в строке 
  int charsAmount = 0;
  Symbol** chars = buildCharactersList(string, &charsAmount);
  if (chars == NULL) return NULL;
  // Сортировка по убыванию
  sort((void**)chars, charsAmount, compareSymbols);
  // Присваивание кодов символам
  for (int i = 0; i < 15 && i < charsAmount; i++) {
    chars[i]->code = i & 0b00001111;
    chars[i]->size = 4;    
  }
  for (int i = 15, j = 0; i < charsAmount && j < 255; i++, j++) {
    chars[i]->code = j & 0b11111111;
    chars[i]->size = 8;    
  }

  char* memory = pack(string, length, chars, charsAmount);

  // printf("| A | # | 0b0 \n");  
  // for (int i = 0; i < charsAmount; i++) {
  //   printf("| %c | %d | ", chars[i]->letter, chars[i]->amount);
  //   printBinary(chars[i]->code, chars[i]->size);
  //   printf("\n");
  // }  

  int memorySize = unpack(memory, chars);  
    
    // printf("\n");
    // for (int i = 0; i < memorySize; i++) {
    //   printBinary((memory[i] & 0b11110000) >> 4, 4);
    //   printf(" ");
    //   printBinary(memory[i] & 0b00001111, 4);
    //   printf(" ");
    // }

  printf("\nS0: %d\n", length);
  printf("Sc: %d\n", memorySize);
  printf("K: %f\n", float(length)/memorySize);

  free(memory);
  free(chars);
  return 0;
}

char* pack(char string[], int length, Symbol** chars, int charsAmount) {
  int memorySize = length;
  char* memory = (char*)malloc(memorySize);
  if (memory == NULL) return NULL;
  
  int mi = 0;
  for (int i = 0; i < (length - length % 2); i += 2) {
    Symbol* symbol1 = findChar(chars, charsAmount, string[i]);
    Symbol* symbol2 = findChar(chars, charsAmount, string[i+1]);
    
    if (mi + 3 > memorySize) {
      char* memoryExt = (char*)realloc(memory, mi*2);
      memorySize = mi*2;
      if (memoryExt == NULL) return NULL;
      memory = memoryExt;
    }

    if (symbol1->size == 4 && symbol2->size == 4) {
      memory[mi] = (symbol1->code << 4) | (symbol2->code & 0b00001111);
      mi++;

    } else if (symbol1->size == 4 && symbol2->size == 8) {
      memory[mi] = (symbol1->code << 4) | 0b00001111;
      memory[mi+1] = symbol2->code & 0b11111111;
      mi += 2;

    } else if (symbol1->size == 8 && symbol2->size == 4) {
      memory[mi] = 0b11110000 + ((symbol1->code & 0b11110000) >> 4);
      memory[mi+1] = ((symbol1->code & 0b00001111) << 4) | symbol2->code;
      mi += 2;

    } else if (symbol1->size == 8 && symbol2->size == 8) {
      memory[mi] = 0b11110000 | ((symbol1->code & 0b11110000) >> 4);
      memory[mi+1] = ((symbol1->code & 0b00001111) << 4)| 0b00001111;
      memory[mi+2] = (symbol2->code & 0b11111111);
      mi += 3;
    }
  }
  if (length % 2 != 0) { 
    Symbol* lastSymbol = findChar(chars, charsAmount, string[length-1]);
    if (lastSymbol->size == 4) {
      memory[mi] = (lastSymbol->code << 4) | 0b00001111;
      memory[mi+1] = 0b11111111;
      mi+=2;
    } else if (lastSymbol->size == 8) {
      memory[mi] = (lastSymbol->code & 0b11111111);
      memory[mi+1] = 0b11111111;
      memory[mi+2] = 0b11111111;
      mi+=3;
    }

  } else {
    memory[mi] = 0b11111111;
    memory[mi+1] = 0b11111111;
    mi+=2;
  }

  return (char*)realloc(memory, mi);
}


int unpack(char* memory, Symbol** chars) {
  int i = 0;
  while (1) {
    int i1 = (memory[i] & 0b11110000) >> 4;

    if (i1 == 0b1111) {
      int i2 = ((memory[i] & 0b00001111) << 4) | (memory[i+1] & 0b11110000) >> 4;
      i++;
      if (i2 == 0b11111111) return i + 1;
      printf("%c", chars[i2+15]->letter);
    } else {
      printf("%c", chars[i1]->letter);
    }
    
    int i2 = memory[i] & 0b00001111;
    if (i2 == 0b1111) {
      int i3 = memory[i+1] & 0b11111111;
      i++;
      if (i3 == 0b11111111) return i + 1;
      printf("%c", chars[i3+15]->letter);
    } else {
      printf("%c", chars[i2]->letter);
    }
    i++;
  }
}

Symbol* findChar(Symbol** chars, int charsAmount, char letter) {
  for (int i = 0; i < charsAmount; i++) {
    if (chars[i]->letter == letter) return chars[i];
  }
  return NULL;
}

Symbol** buildCharactersList(char* string, int* charsAmount) {
  int charsMaxAmount = 26;
  Symbol** chars = (Symbol**)malloc(sizeof(Symbol)*charsMaxAmount);
  if (chars == NULL) return NULL;

  for (int i = 0; i < strlen(string); i++) {
    Symbol *symbol = findChar(chars, *charsAmount, string[i]);
    
    if (symbol != NULL) {
      symbol->amount++;
      continue;
    }

    Symbol* newSymbol = createSymbol(string[i], 0, 1);
    if (newSymbol == NULL) return NULL;
    
    if (*charsAmount > charsMaxAmount) {
      Symbol** charsExt = (Symbol**)realloc(chars, sizeof(Symbol)*charsMaxAmount*2);
      if (charsExt == NULL) return NULL;
      charsMaxAmount *= 2;
      chars = charsExt;
    }
    chars[*charsAmount] = newSymbol;
    (*charsAmount)++;
  }

  return chars;
}

Symbol* createSymbol(char letter, unsigned char code, int amount) {
  Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));

  if (symbol != NULL) {
    symbol->letter = letter;
    symbol->code = code;
    symbol->amount = amount;
    symbol->size = 4; 
  }

  return symbol;
}

int compareSymbols(void* a, void* b) {
  const Symbol* t1 = (Symbol*)a;
  const Symbol* t2 = (Symbol*)b;
  return t1->amount >= t2->amount;
}

void printBinary(unsigned char number, unsigned char bits) {
  unsigned char reversedNumber = 0b0;
  for (int i = 0; i < bits; number =  number >> 1, i++) {
    reversedNumber = reversedNumber << 1; // Сдвиг на 1 разряд влево
    reversedNumber += number % 0b10;
  }
  for (int i = 0; i < bits; reversedNumber =  reversedNumber >> 1, i++) {
    printf("%d", reversedNumber % 0b10);
  }
}

void** findMinMax(void** array, int arrayLength, CompareFunction compare) {
  void* minMax = array[0];
  int minMaxI = 0;
  for (int i = 1; i < arrayLength; i++) {
    int res = compare(array[i], minMax);
    if (res > 0) {
      minMax = array[i];
      minMaxI = i;
    }
  }
  return array + minMaxI; 
}

void sort(void** array, int arrayLength, CompareFunction compare) {
  for (int i = 0; i < arrayLength - 1; i++) {
    void** min = findMinMax(array + i, arrayLength - i, compare);
    
    void *tmp = array[i];
    array[i] = *min;
    *min = tmp;
  }
} 