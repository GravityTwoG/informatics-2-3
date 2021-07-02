#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

char* concatenate(char* str, ...) {
  va_list args;
  
  // Первый проход по списку аргументов, для подсчёта кол-ва символов
  va_start(args, str);
  int length = strlen(str);
  char* s = va_arg(args, char*);
  while (s != NULL) {
    length += strlen(s);
    s = va_arg(args, char*);
  }
  length += 1;
  va_end(args);

  char* string = (char*)malloc(sizeof(char) * length);
  if (string == NULL) return NULL;
  string[0] = '\0';

  // Второй проход по списку аргументов, для копирования данных в созданную строку
  va_start(args, str);
  strcat(string, str);
  s = va_arg(args, char*);
  while (s != NULL) {
    strcat(string, s);
    s = va_arg(args, char*);
  }
  va_end(args);
  string[length - 1] = '\0';

  return string;
}
// Разработать функцию с переменным числом параметров. Для извлечения параметров из списка использовать операцию преобразования типа указателя.
//2. Каждый параметр - строка, последний параметр - NULL. Функция возвращает строку в динамической памяти, содержащую объединение строк-параметров.
int main() {
  char* string = concatenate("Hello ", "world", "!", "Hello", NULL);
  printf("%s\n",string);
  free(string);
  return 0;
}