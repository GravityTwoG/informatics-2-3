#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#define MAX_NUMBER_SIZE 15
#define MAX_NUMBER_SIZE_S "14"
#define MAX_BRAND_SIZE 35
#define MAX_BRAND_SIZE_S "34"
#define MAX_MILEAGE_DIGITS 9
#define MAX_LINE_LENGTH MAX_NUMBER_SIZE + MAX_BRAND_SIZE + MAX_MILEAGE_DIGITS + 9  

#define DATA_PATH "cars.txt"
#define DATA_TEMP_PATH "temp.txt"
#define DATA_FORMAT_IN "\"%[^\"]\", \"%[^\"]\", %d"
#define DATA_FORMAT_OUT "\"%s\", \"%s\", %d\n"

// Определить структурированный тип и набор  функций для работы с таблицей записей, реализованной в массиве структур. В перечень функций входят:
// · ввод записи таблицы с клавиатуры;
// · загрузка и сохранение таблицы в текстовом файле;
// · просмотр таблицы;
// · сортировка таблицы в порядке возрастания заданного поля;
// · поиск в таблице элемента с заданным значением поля или с наиболее близким к нему по значению;
// · удаление записи;
// · изменение (редактирование) записи;
// · вычисление с проверкой и использованием всех pfgbctq по заданному условию и формуле (например, общая сумма на всех счетах).
// 10. Регистрационный номер автомобиля, марка, пробег.


void readString(char string[], int length);
void readInt(int *number);
int readNumber(char number[]);
int readBrand(char brand[]);
int readMileage(unsigned int *mileage);

struct Car {
  char number[MAX_NUMBER_SIZE];
  char brand[MAX_BRAND_SIZE];
  unsigned int mileage;
};

struct CarsTable {
  Car* cars = NULL;
  int carsAmount = 0;
  int maxAmount = 0;

  int findCarIndex(char number[]);
  void addCar(Car *car);
  void editCar(Car *car, char number[]);
  void removeCar(char number[]);
  void init();
  void destruct();
  void writeToFile();
  void sortCars(char direction);
};

void CarsTable::init() {
  FILE* file = fopen(DATA_PATH, "ab+");
  if (file == NULL) {
    puts("Не удалось открыть файл\n");
    exit(1);
  }

  char line[MAX_LINE_LENGTH];
  fseek(file, 0, SEEK_SET);
  fgets(line, MAX_LINE_LENGTH, file);
  int amount = 0;
  if (!sscanf(line, "%d", &amount)) {
    return;
  }

  maxAmount = amount > 10 ? (int)(amount * 1.1) : 10;
  cars = (Car*)malloc(maxAmount * sizeof(Car));
  if (cars == NULL) {
    exit(1);
  }

  fgets(line, MAX_LINE_LENGTH, file);
  while (!feof(file) && carsAmount < amount) {
    Car car;
    if (sscanf(line, DATA_FORMAT_IN, car.number, car.brand, &car.mileage)) {
      cars[carsAmount] = car;
      carsAmount++;
    }
    fgets(line, MAX_LINE_LENGTH, file);
  }
  fclose(file);
};

void CarsTable::destruct() {
  free(cars);
}
void CarsTable::writeToFile() {
  FILE* tmpFile = fopen(DATA_TEMP_PATH, "w+");
  if (tmpFile == NULL) return;

  fseek(tmpFile, 0, SEEK_SET);
  fprintf(tmpFile, "%d\n", carsAmount);

  for (int i = 0; i <= carsAmount - 1; i++) {
    Car currentCar = cars[i];
    fprintf(tmpFile, DATA_FORMAT_OUT, currentCar.number, currentCar.brand, currentCar.mileage);
  }
  fclose(tmpFile);
  remove(DATA_PATH);
  int res = rename(DATA_TEMP_PATH, DATA_PATH);
  if (res == -1) {
    puts("Ошибка при работе с файлами");
    exit(1);
  }
}

void CarsTable::addCar(Car *car) {
  if (carsAmount + 1 > maxAmount) {
    maxAmount = (int)(maxAmount * 1.1);
    Car *ptr = (Car*)realloc(cars, maxAmount * sizeof(Car));
    if (ptr== NULL) {
      exit(1);
    }
    cars = ptr;
  }

  FILE* file = fopen(DATA_PATH, "r+");
  if (file == NULL) {
    puts("Не удалось открыть файл\n");
    exit(1);
  }

  fseek(file, 0, SEEK_SET);
  int res = fprintf(file, "%d\n", carsAmount + 1);
  if (res < 0) {
    puts("Ошибка при записи в файл\n");
    return;
  }
  fseek(file, 0, SEEK_END);
  res = fprintf(file, DATA_FORMAT_OUT, car->number, car->brand, car->mileage);
  if (res < 0) {
    puts("Ошибка при записи в файл\n");
    return;
  }
  fclose(file);

  cars[carsAmount] = *car;
  carsAmount++;
};

int CarsTable::findCarIndex(char number[]) {
  for (int i = 0; i < carsAmount; i++) {
    if (strcmp(number, cars[i].number) == 0) return i;
  }
  return -1;
};

void CarsTable::editCar(Car *car, char number[]) {
  int carIndex = findCarIndex(number);
  if (carIndex == -1) return;

  strcpy(cars[carIndex].number, car->number);
  strcpy(cars[carIndex].brand, car->brand);
  cars[carIndex].mileage = car->mileage;

  writeToFile();
};

void CarsTable::removeCar(char number[]) {
  int carIndex = findCarIndex(number);
  if (carIndex == -1) return;

  for (int i = carIndex; i < carsAmount - 1; i++) {
    cars[i] = cars[i + 1];
  }
  carsAmount--;
  writeToFile();
};

int compareCars(const void* s1, const void* s2) {
  Car* c1 = (Car*)s1;
  Car* c2 = (Car*)s2;
  return c1->mileage >= c2->mileage;
}
int compareCarsReverse(const void* s1, const void* s2) {
  Car* c1 = (Car*)s1;
  Car* c2 = (Car*)s2;
  return c1->mileage <= c2->mileage;
}

void CarsTable::sortCars(char direction) {
  if (carsAmount == 0) return;
  if (direction == 1) {
    qsort(cars, carsAmount, sizeof(Car), compareCars);
  }
  else if (direction == 0) {
    qsort(cars, carsAmount, sizeof(Car), compareCarsReverse);
  }
  writeToFile();
}





void createCar(CarsTable* carsTable) {
  char number[MAX_NUMBER_SIZE];
  char brand[MAX_BRAND_SIZE];
  unsigned int mileage;

  printf("Введите номер (%d символа(ов)): ", MAX_NUMBER_SIZE - 1);
  int res = readNumber(number);
  if (res == -1) {
    getchar();
    return;
  }

  if (carsTable->findCarIndex(number) != -1) {
    puts("Автомобиль с таким номером уже есть\n");
    getchar();
    return;
  }

  printf("Введите марку: (%d символа(ов)): ", MAX_BRAND_SIZE - 1);
  res = readBrand(brand);
  if (res == -1) {
    getchar();
    return;
  }

  printf("Введите пробег [0-%d]: ", int(pow(10, MAX_MILEAGE_DIGITS) - 1));
  res = readMileage(&mileage);
  if (res == -1) { 
    getchar();
    return; 
  }

  Car car;
  strcpy(car.number, number);
  strcpy(car.brand, brand);
  car.mileage = mileage;

  carsTable->addCar(&car);
};

void deleteCar(CarsTable* carsTable) {
  char number[MAX_NUMBER_SIZE];

  puts("Введите номер удаляемого автомобиля: ");
  int res = readNumber(number);
  if (res == -1) { 
    puts("Автомобиль не найден");
    getchar();
    return; 
  }
  carsTable->removeCar(number);
}

void editCar(CarsTable* carsTable) {
  char number[MAX_NUMBER_SIZE];
  char brand[MAX_BRAND_SIZE];
  unsigned int mileage;

  puts("Введите номер редактируемого автомобиля: ");
  int res = readNumber(number);
  if (res == -1) return;
  int carIndex = carsTable->findCarIndex(number);
  if (carIndex == -1) {
    puts("Автомобиль не найден");
    getchar();
    return;
  }

  strcpy(brand, carsTable->cars[carIndex].brand);
  mileage = carsTable->cars[carIndex].mileage;

  while (1) {
    puts("Что Вы хотите изменить?");
    printf("| 0: Отмена | 1: Номер | 2: Марка | 3: Пробег | 4: Сохранить |\n");

    int input = -1;
    readInt(&input);

    if (input == 1) {
      printf("Введите номер (%d символов): ", MAX_NUMBER_SIZE - 1);
      int res = readNumber(number);
      if (res == -1) return;
      if (carsTable->findCarIndex(number) != -1) {
        puts("Автомобиль с таким номером уже есть\n");
        return;
      }
    } else if (input == 2) {
      printf("Введите марку: (%d символов): ", MAX_BRAND_SIZE - 1);
      res = readBrand(brand);
      if (res == -1) return;
    } else if (input == 3) {
      printf("Введите пробег [0-999999999]: ");
      res = readMileage(&mileage);
      if (res == -1) return;
    } else if (input == 4) {
      break;
    } else if (input == 0) {
      return;
    }
  }

  Car car;
  strcpy(car.number, number);
  strcpy(car.brand, brand);
  car.mileage = mileage;

  carsTable->editCar(&car, carsTable->cars[carIndex].number);
}

void printLine(Car *car) {
  printf(
    "| %" MAX_NUMBER_SIZE_S "s | %" MAX_BRAND_SIZE_S "s | %9d Km |\n",
    car->number, car->brand, car->mileage
  );
}

// mode
// 0 вывести всё
// 1 фильтр по номеру
// 2 фильтр по марке
// 3 фильтр по пробегу
void printTable(int mode, char* filter, int mileage, Car cars[], int carsAmount) {
  unsigned long long int totalMileage = 0;
  int filterLen = strlen(filter);

  puts("|----------------|------------------------------------|--------------|");
  puts("|   Рег. номер   |               Марка                |    Пробег    |");
  puts("|----------------|------------------------------------|--------------|");
  for (int i = 0; i < carsAmount; i++) {
    Car *car = &(cars[i]);

    if (mode == 1 && filterLen) {
      if (strstr(car->number, filter) != NULL) {
        printLine(car);
        totalMileage += car->mileage;
      }
    }
    else if (mode == 2 && filterLen) {
      if (strstr(car->brand, filter) != NULL) {
        printLine(car);
        totalMileage += car->mileage;
      }
    }
    else if (mode == 3) {
      if (car->mileage >= (unsigned int)(mileage * 0.9) 
        && car->mileage <= (unsigned int)(mileage * 1.1)) {
        printLine(car);
        totalMileage += car->mileage;
      }
    }
    else {
      printLine(car);
      totalMileage += car->mileage;
    }
  }
  puts("|----------------|------------------------------------|--------------|");
  printf("| Суммарный пробег: %45llu Km |\n", totalMileage);
  puts("|-----------------------------------------------------|--------------|");
}

void search(CarsTable* carsTable) {
  char numberFilter[MAX_NUMBER_SIZE];
  char brandFilter[MAX_BRAND_SIZE];
  unsigned int mileageFilter = 0;
  int mode = 0;

  while (1) {
    puts("Поиск по:");
    puts("1 - номеру, 2 - марке, 3 - пробегу, 0 - отменить поиск");
    readInt(&mode);

    if (mode == 1) {
      puts("Поиск по номеру:");
      readNumber(numberFilter);
      printTable(mode, numberFilter, 0, carsTable->cars, carsTable->carsAmount);
      strcpy(numberFilter, "");
    }
    else if (mode == 2) {
      puts("Поиск по марке:");
      readBrand(brandFilter);
      printTable(mode, brandFilter, 0, carsTable->cars, carsTable->carsAmount);
      strcpy(brandFilter, "");
    }
    else if (mode == 3) {
      puts("Поиск по пробегу +-10%:");
      readMileage(&mileageFilter);
      printTable(mode, brandFilter, mileageFilter, carsTable->cars, carsTable->carsAmount);
    }
    else if (mode == 0) return;
  }
}

int main() {
  SetConsoleOutputCP(1251);
  SetConsoleCP(1251);

  CarsTable carsTable;
  carsTable.init();

  int input;
  char direction = 1;
  while (1) {
    system("cls");
    printTable(0, (char*)(" "), 0, carsTable.cars, carsTable.carsAmount);
    puts("|  1: Добавить   |  2: Редактировать  |  3: Удалить   |   4: Поиск   |");
    puts("|  5: Cортировка                                      |   0: Выход   |");

    input = -1;
    readInt(&input);

    if (input == 1) {
      createCar(&carsTable);
    } else if (input == 2) {
      editCar(&carsTable);
    } else if (input == 3) {
      deleteCar(&carsTable);
    } else if (input == 4) {
      search(&carsTable);
    } else if (input == 5) {
      direction = direction == 1 ? 0 : 1;
      carsTable.sortCars(direction);
    } else if (input == 0) {
      return 0;
    }
  }
  return 0;
}

// Считывает ввод с клавиатуры и сохраняет данные в string
void readString(char string[], int length) {
  scanf_s("%[^\n]", string, length);
  // Остаются символы в потоке ввода, их нужно удалить
  scanf_s("%*[^\n]");
  scanf_s("%*c");
}

void readInt(int *number) {
  scanf_s("%d", number);
  scanf_s("%*[^\n]");
  scanf_s("%*c");
}
int readNumber(char number[]) {
  readString(number, MAX_NUMBER_SIZE);
  if (!strlen(number)) {
    puts("Некорректно введён номер\n");
    return -1;
  }
  return 0;
}

int readBrand(char brand[]) {
  readString(brand, MAX_BRAND_SIZE);
  if (!strlen(brand)) {
    puts("Некорректно введена марка\n");
    return -1;
  }
  return 0;
}

int readMileage(unsigned int* mileage) {
  int number;
  readInt(&number);
  if (number < 0 || number > (int)(pow(10, MAX_MILEAGE_DIGITS) - 1)) {
    puts("Пробег не корректен\n");
    return -1;
  }
  *mileage = (unsigned int)(number);
  return 0;
}