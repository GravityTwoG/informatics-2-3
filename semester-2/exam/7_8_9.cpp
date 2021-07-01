#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Замечания по выполняемым операциям. Объединение - результат содержит элементы
// из двух исходных структур данных (СД), элемент, присутствующий в обеих СД,
// включается в одном экземпляре. Пересечение - результат содержит элементы,
// одновременно присутствующие в обеих структурах данных. Разность - результат
// содержит элементы из первой СД, которые отсутствуют во второй.

struct ListNode {
  int value;
  ListNode* prev;
  ListNode* next;
};

void printList(ListNode* list);
void printListR(ListNode* list);
ListNode* createList(int nodesAmount);
void fillList(ListNode* nodes, int nodesAmount);
void freeList(ListNode* list);
int isInList(ListNode* list, ListNode* node);
ListNode* getLastNode(ListNode* head);
ListNode* makeCopy(ListNode* head);

// 1. Объединение двух циклических двусвязных списков.
ListNode* mergeLists(ListNode* list1, ListNode* list2) {
  if (list1 == NULL) return makeCopy(list2);
  if (list2 == NULL) return makeCopy(list1);

  ListNode* merged = makeCopy(list1);
  ListNode* tail = getLastNode(merged);
  ListNode* currentNode = list2;
  while (1) {
    // Если элемента с таким значением нет в первом списке
    if (!isInList(list1, currentNode)) {
      // добавляем этот элемент в список
      ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
      if (newNode == NULL) {
        freeList(merged);
        return NULL;
      }

      newNode->value = currentNode->value;
      newNode->prev = tail;
      newNode->next = NULL;
      
      if (tail == NULL) {
        merged = newNode;
        tail = newNode;
      }
      tail->next = newNode;
      tail = tail->next;
    }
    if (currentNode->next == NULL || currentNode->next == list2) break;
    currentNode = currentNode->next;
  }

  return merged;
}

// 2. Пересечение двух циклических двусвязных списков.
// Возвращает новый список состоящий из элементов, которые присутствуют в обоих списках одновременно
ListNode* intersect(ListNode* list1, ListNode* list2) {
  if (list1 == NULL || list2 == NULL) return NULL;
  ListNode* intersectionHead = NULL;

  ListNode* currentNode = list2;
  ListNode* intersectionTail = NULL;
  while (1) {
    // Если элемент с таким значением есть в первом списке
    if (isInList(list1, currentNode)) {
      // Добавляем этот элемент в список
      ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
      if (newNode == NULL) {
        freeList(intersectionHead);
        return NULL;
      }
      newNode->value = currentNode->value;
      newNode->next = NULL;

      if (intersectionHead == NULL) {
        intersectionHead = newNode;
        intersectionTail = intersectionHead;
      } else {
        intersectionTail->next = newNode;
        intersectionTail = intersectionTail->next;
      }
    } 
    if (currentNode->next == NULL || currentNode->next == list2) break;
    currentNode = currentNode->next;
  }

  return intersectionHead;
}

// 3. Разность двух циклических двусвязных списков.
ListNode* subtract(ListNode* list1, ListNode* list2) {
  if (list1 == NULL) return NULL;
  if (list2 == NULL) return makeCopy(list1);
  ListNode* subtractionHead = NULL;

  ListNode* currentNode = list1;
  ListNode* subtractionTail = NULL;
  while (1) {
    // Если элемента с таким значением нет во втором списке
    if (!isInList(list2, currentNode)) {
      // Добавляем этот элемент в список
      ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
      if (newNode == NULL) {
        freeList(subtractionHead);
        return NULL;
      }
      newNode->value = currentNode->value;
      newNode->next = NULL;

      if (subtractionHead == NULL) {
        subtractionHead = newNode;
        subtractionTail = subtractionHead;
      } else {
        subtractionTail->next = newNode;
        subtractionTail = subtractionTail->next;
      }
    }
    if (currentNode->next == NULL || currentNode->next == list1) break;
    currentNode = currentNode->next;
  }

  return subtractionHead;
}

ListNode* findMax(ListNode* head) {
  if (head == NULL) return NULL;
  ListNode* max = head;
  ListNode* currentNode = max->next;
  while (currentNode != NULL) {
    if (currentNode->value > max->value) {
      max = currentNode;
    }
    currentNode = currentNode->next;
  }
  return max;
}

void selectionSort(ListNode*& head) {
  if (head == NULL) return;
  ListNode* sortedHead = NULL;
  ListNode* currentNode = head;
  while (currentNode != NULL) {
    ListNode* maxNode = findMax(currentNode);
    if (maxNode == NULL) break;

    // Вырезаем элемент из списка
    if (maxNode->prev != NULL && maxNode->next != NULL) {
      maxNode->prev->next = maxNode->next;
      maxNode->next->prev = maxNode->prev;
      currentNode = head;
    }  else if (maxNode->prev != NULL) {
      maxNode->prev->next = NULL;
      currentNode = head;
    } else if (maxNode->next != NULL) {
      head = maxNode->next;
      head->prev = NULL;
      currentNode = head;
    } else {
      head = NULL;
      currentNode = NULL;
    }

    maxNode->prev = NULL;
    
    if (sortedHead == NULL) {
      maxNode->next = NULL;
      sortedHead = maxNode;
    } else {
      maxNode->next = sortedHead;
      sortedHead->prev = maxNode;
      sortedHead = sortedHead->prev;
    }
  }
  head = sortedHead;
}
int j = 0;
void insertNode(ListNode *&head, ListNode* newNode) { 
  newNode->prev = newNode->next = NULL;
  printf("n: %3d  \n", newNode->value);
  printList(head);

  if (head  == NULL) { // включение в пустой список
    head = newNode; 
    head->next = head;
    head->prev = head;
    return;
  }
  // поиск места включения - currentNode
  ListNode *currentNode = head->prev; 
  ListNode *prevNode = head->prev;
  char insertionType = 0; 

  while (1) { // итерация с конца к заголовку
    if (newNode->value >= currentNode->value && newNode->value < prevNode->value ) {
      insertionType = 1;
      break;
    } else if (newNode->value >= currentNode->value) {
      prevNode = currentNode;
      insertionType = 2;
      break;
    }
    if (currentNode == head) break;
    prevNode = currentNode;
    currentNode = currentNode->prev;
  }
  printf("pr: %3d  ", prevNode->value);
  printf("cr: %3d  \n", currentNode->value);
  

  if (insertionType == 0) { // включение в начало списка
    newNode->next = head;
    head->prev = newNode;
    head = newNode;
    prevNode->next = newNode;
    newNode->prev = prevNode; 
    printf("start: ");
  } else if (insertionType == 2) { // в конец
    prevNode->next = newNode;
    newNode->prev = prevNode; 
    newNode->next = head;
    head->prev = newNode;
    printf("last: ");

  } else {
    printf("med: \n");
    currentNode->next = newNode;
    newNode->prev = currentNode;
    
    prevNode->prev = newNode;       
    newNode->next = prevNode;       
    printListR(head);
    printList(head);
    exit(1);
  }
  printf("processed: \n");
  printList(head);
}

ListNode* removeNode(ListNode *&head, int n) { 
  ListNode *currentNode = head;
  for (
    ; 
    currentNode != NULL && currentNode != head && n != 0; 
    currentNode = currentNode->next, n--
  );   // Отсчитать n-ый
  if (currentNode == NULL) return NULL;  // нет элемента с таким номером  

  if (currentNode == head) { // 
    head->next = currentNode->next; 
    currentNode->next->prev = head;
    head = currentNode->next;
  } else { // удаление из середины
    currentNode->next->prev = currentNode->prev;                  
    currentNode->prev->next = currentNode->next;
  }
  return currentNode;
}

void insertionSort(ListNode*& head, int nodesAmount) {
  ListNode* sorted = NULL;
  for (int i = 0; i < nodesAmount; i++) {
    ListNode* deleted = removeNode(head, 0);
    // printf("  %d  \n", deleted->value);
    insertNode(sorted, deleted);
    // printList(sorted);
  }
  head = sorted;
}

int main() {
  srand(time(NULL));
  int nodesAmount = 5;
  ListNode* list1 = createList(nodesAmount);
  if (list1 == NULL) return -1;

  ListNode* list2 = createList(nodesAmount);
  if (list2 == NULL) { 
    freeList(list1);
    return -1; 
  }
  fillList(list1, nodesAmount);
  fillList(list2, nodesAmount);

  printf("List 1: ");
  printList(list1);
  
  printf("List 2: ");
  ListNode* currentNode = list2;
  while (1) {
    if (isInList(list1, currentNode)) {
      printf(" %3d* ", currentNode->value);
    } else {
      printf(" %3d  ", currentNode->value);
    }
    if (currentNode->next == NULL || currentNode->next == list2) break;
    currentNode = currentNode->next;
  }
  printf("\n\n");

  // ListNode* mergedList = mergeLists(list1, list2);
  // printf("Union:  ");
  // printList(mergedList);
  // freeList(mergedList);

  // ListNode* intersection = intersect(list1, list2);
  // printf("Intersection:\n");
  // if (intersection != NULL) {
  //   printList(intersection);
  // } else {
  //   printf("No intersections\n");
  // }
  // freeList(intersection);
  
  // ListNode* subtraction = subtract(list1, list2);
  // printf("Subtraction:\n");
  // if (subtraction != NULL) {
  //   printList(subtraction);
  // } else {
  //   printf("Subtraction is NULL\n");
  // }
  // freeList(subtraction);

  // selectionSort(list1);
  // printf("Sorted: ");
  // printList(list1);
  
  insertionSort(list1, nodesAmount);
  printf("Sorted: ");
  printList(list1);

  freeList(list1);
  freeList(list2);

  return 0;
}

void printList(ListNode* head) {
  if (head == NULL) return;
  ListNode* currentNode = head;
  printf(" %3d  ", currentNode->value);
  while (currentNode->next != NULL && currentNode->next != head) {
    currentNode = currentNode->next;
    printf(" %3d  ", currentNode->value);
  }
  printf("\n");
}
void printListR(ListNode* head) {
  if (head == NULL) return;
  ListNode* currentNode = head;
  do {
    currentNode = currentNode->prev;
    printf(" %3d  ", currentNode->value);
  } while (currentNode != NULL && currentNode != head);
  printf("\n");
}

ListNode* getLastNode(ListNode* head) {
  if (head == NULL) return NULL;
  ListNode* tail = head;
  while (1) {
    if (tail->next == NULL || tail->next == head) break;
    tail = tail->next;
  }
  return tail;
}

int isInList(ListNode* head, ListNode* node) {
  if (head == NULL) return NULL;
  ListNode* currentNode = head;
  while (1) {
    if (currentNode->value == node->value) return 1;
    if (currentNode->next == NULL || currentNode->next == head) return 0;
    currentNode = currentNode->next;
  }
  return 0;
}

void freeList(ListNode* head) {
  ListNode* currentNode = head;
  while (currentNode != NULL && currentNode != head) {
    ListNode* tmp = currentNode;
    currentNode = currentNode->next;
    free(tmp);
  }
}

// Создаёт циклический двусвязный список необходимой длины
ListNode* createList(int nodesAmount) {
  ListNode* head = NULL;
  ListNode* node = (ListNode*)malloc(sizeof(ListNode));
  if (node == NULL) return NULL;
  node->value = 0;
  node->prev = NULL;
  node->next = NULL;

  head = node;
  ListNode* prevNode = head;
  for (int i = 1; i < nodesAmount; i++) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if (newNode == NULL) {
      freeList(head);
      return NULL;
    }

    newNode->value = 0;
    newNode->prev = prevNode;
    newNode->next = NULL;

    prevNode->next = newNode;
    prevNode = newNode;
  }
  prevNode->next = head;
  head->prev = prevNode;
  return head;
}

// Заполняет список уникальными значениями
void fillList(ListNode* head, int nodesAmount) {
  if (head == NULL) return;
  char* values = (char*)malloc(nodesAmount * 2 * sizeof(char));
  if (values == NULL) return;
  for (int i = 0; i < nodesAmount * 2; i++) {
    values[i] = 0;
  }

  ListNode *currentNode = head;
  int newValue = 0;
  while (1) {
    do {
      newValue = rand() % (nodesAmount * 2);
    } while (values[newValue] == 1);
    values[newValue] = 1;
    currentNode->value = newValue;
    if (currentNode->next == NULL || currentNode->next == head) break;
    currentNode = currentNode->next;
  }

  free(values);
}

ListNode* makeCopy(ListNode* head) {
  if (head == NULL) return NULL;
  ListNode* copyHead = NULL;

  ListNode* currentNode = head;
  ListNode* prevNode = copyHead;
  while (1) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if (newNode == NULL) {
      freeList(copyHead);
      return NULL;
    }
    newNode->value = currentNode->value;
    newNode->prev = prevNode;
    newNode->next = NULL;
    
    if (copyHead == NULL) {
      copyHead = newNode;
      prevNode = copyHead;
    } else {
      prevNode->next = newNode;
      prevNode = newNode;
    }

    if (currentNode->next == NULL || currentNode->next == head) break;
    currentNode = currentNode->next;
  }
  return copyHead;
}
