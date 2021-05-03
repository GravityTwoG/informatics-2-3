#include <stdio.h>

struct Vertex {
  int values[2];
  Vertex* left = NULL;
  Vertex* right = NULL;
};

void printVertex(Vertex *vertex) {
  printf("%d  %d", vertex->values[0], vertex->values[1]);
}

void printTree(Vertex *root) {
  char hasLeft = 0;
  char hasRight = 0;
  if (root->left != NULL) hasLeft = 1;
  if (root->right != NULL) hasRight = 1;
  
  printf("   ");
  printVertex(root);
  
  if (hasLeft && hasRight) {
    printf("\n    /\\");
    printf("\n   /  \\\n");
    printVertex(root->left);
    printf("   ");
    printVertex(root->right);
  }
  if (hasLeft && !hasRight) {
    printf("\n    /  ");
    printf("\n   /    \n");
    printVertex(root->left);
    printf("   ");
  }
  if (!hasLeft && hasRight) {
    printf("\n     \\");
    printf("\n      \\\n");
    printf("      ");
    printVertex(root->right);
  }
}

// Программа должна содержать функцию  обхода дерева с выводом  его  содержимого,  функцию  добавления  вершины дерева  (ввод),  а  также  указанную  в  варианте  функцию.
// 3.Вершина дерева содержит два целых числа и три  указателя на  поддеревья.  Данные  в  дереве  упорядочены.   Написать функцию включения нового значения в  дерево  с  сохранением упорядоченности.
int main() {
  Vertex root;
  root.values[0] = 0;
  root.values[1] = 1;

  Vertex branch1;
  branch1.values[0] = 0;
  branch1.values[1] = 0;
  root.left = &branch1;
  
  Vertex branch2;
  branch2.values[0] = 3;
  branch2.values[1] = 5;
  root.right = &branch2;

  printTree(&root);
  return 0;
}