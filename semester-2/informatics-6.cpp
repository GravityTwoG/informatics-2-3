#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
  int values[2];
  TreeNode* left = NULL;
  TreeNode* right = NULL;
};

void printTreeNode(TreeNode *treeNode) {
  printf("[%-2d %2d]", treeNode->values[0], treeNode->values[1]);
}

void printTabs(int n) {
  for (int i = 0; i < 2*n-1; i++) {
    printf("       ");
  }
}

void printTree(TreeNode *root, int level) {
  char hasLeft = 0;
  char hasRight = 0;

  // printTabs(level);
  printTreeNode(root);
  printf(" ----> ");

  if (root->right != NULL) {
    printTree(root->right, level+1);
  }
  if (root->left != NULL) {
    printf("\n");
    printTabs(level+1);
    printf(" \\---> ");
    printTree(root->left, level+1);
    printf("\n");
  }
}

TreeNode* createTreeNode(int a, int b) {
  TreeNode* treeNode = (TreeNode*)malloc(sizeof(TreeNode));

  if (treeNode != NULL) {
    treeNode->values[0] = a;
    treeNode->values[1] = b;
    treeNode->left = NULL;
    treeNode->right = NULL;
  }

  return treeNode;
}

// Программа должна содержать функцию  обхода дерева с выводом  его  содержимого,  функцию  добавления  вершины дерева  (ввод),  а  также  указанную  в  варианте  функцию.
// 3.Вершина дерева содержит два целых числа и три  указателя на  поддеревья.  Данные  в  дереве  упорядочены.   Написать функцию включения нового значения в  дерево  с  сохранением упорядоченности.
int main() {
  TreeNode* root = createTreeNode(0, 1);

  if  (root == NULL) return -1;

  TreeNode* branch1 = createTreeNode(0, 0);
  root->left = branch1;
  TreeNode* branch2 = createTreeNode(3, 5);
  root->right = branch2;

  TreeNode* branch22 = createTreeNode(7, 9);
  branch2->right = branch22;
  TreeNode* branch222 = createTreeNode(6, 8);
  branch2->left = branch222;

  printTree(root, 0);
  return 0;
}