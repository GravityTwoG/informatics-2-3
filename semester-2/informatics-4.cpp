#include <stdlib.h>
#include <stdio.h>
#include <time.h>


//1. Объединение двух односвязных списков.
struct Node {
  int value;
  Node* next;
};


void freeList(Node* list) {
  Node* node = list;
  while (node != NULL) {
    Node* tmp = node;
    node = node->next;
    free(tmp);
  }
}

Node* createList(int nodesAmount) {
  Node* nodes = NULL;
  Node* node = (Node*)malloc(sizeof(Node));
  if (node == NULL) {
    return NULL;
  }
  node->value = 0;
  node->next = NULL;

  nodes = node;
  Node* prevNode = nodes;
  for (int i = 1; i < nodesAmount; i++) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
      freeList(nodes);
      return NULL;
    }
    newNode->value = 0;
    newNode->next = NULL;

    prevNode->next = newNode;
    prevNode = newNode;
  }
  return nodes;
}

void fillList(Node* nodes, int nodesAmount) {
  char* values = (char*)malloc(nodesAmount * 2 * sizeof(char));
  if (values == NULL) return;
  for (int i = 0; i < nodesAmount * 2; i++) {
    values[i] = 0;
  }

  Node *currentNode = nodes;
  int newValue = 0;
  while (1) {
    do {
      newValue = rand() % (nodesAmount * 2);
    } while (values[newValue] == 1);
    values[newValue] = 1;
    currentNode->value = newValue;
    if (currentNode->next == NULL) break;
    currentNode = currentNode->next;
  }

  free(values);
}

void printList(Node* list) {
  Node* currentNode = list;
  printf(" %3d  ", currentNode->value);
  while (currentNode->next != NULL) {
    currentNode = currentNode->next;
    printf(" %3d  ", currentNode->value);
  }
  printf("\n");
}

int isInList(Node* list, Node* node) {
  Node* currentNode = list;
  while (1) {
    if (currentNode->value == node->value) return 1;
    if (currentNode->next == NULL) return 0;
    currentNode = currentNode->next;
  }
  return 0;
}

// Добавляет элементы из списка 2 в список 1, если их нет в первом списке
void mergeLists(Node** list1, Node** list2) {
  // Находим последний элемент из первого списка
  if (list1 == NULL) return;
  if (list2 == NULL) return;
  Node* tail1 = *list1;
  while (1) {
    if (tail1->next == NULL) break;
    tail1 = tail1->next;
  }

  Node* currentNode = *list2;
  *list2 = NULL;
  Node* tail2 = NULL;
  while (1) {
    // Если элемент с таким значением уже есть в первом списке
    if (isInList(*list1, currentNode)) {
      // Добавляем этот элемент в список 2
      if (tail2 == NULL) {
        *list2 = currentNode;
        tail2 = *list2;
      } else {
        tail2->next = currentNode;
        tail2 = currentNode;
      }
      
      if (currentNode->next == NULL) break;
      currentNode = currentNode->next;
      
      tail2->next = NULL;
    } else {
      // Иначе добавляем этот элемент в конец первого списка
      tail1->next = currentNode;
      tail1 = currentNode;
      
      if (currentNode->next == NULL) break;
      currentNode = currentNode->next;

      tail1->next = NULL;
    }
  }
}

int main() {
  srand(time(NULL));
  int nodesAmount = 5;
  Node* list1 = createList(nodesAmount);
  if (list1 == NULL) return -1;

  Node* list2 = createList(nodesAmount);
  if (list2 == NULL) { 
    freeList(list1);
    return -1; 
  }
  fillList(list1, nodesAmount);
  fillList(list2, nodesAmount);

  printf("List 1: ");
  printList(list1);
  printf("List 2: ");
  Node* currentNode = list2;
  while (1) {
    if (isInList(list1, currentNode)) {
      printf(" %3d* ", currentNode->value);
    } else {
      printf(" %3d  ", currentNode->value);
    }
    if (currentNode->next == NULL) break;
    currentNode = currentNode->next;
  }
  printf("\n\n");

  mergeLists(&list1, &list2);
  printf("List 1: ");
  printList(list1);
  printf("List 2: ");
  printList(list2);
  
  freeList(list1);
  freeList(list2);
  return 0;
}