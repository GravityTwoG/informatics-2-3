#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct TreeNode {
  char letter = ' ';
  int amount = 0;
  unsigned char bit = 0b0;

  TreeNode* left = NULL;
  TreeNode* right = NULL;
};

void printTree(TreeNode *root, int level);
void printTable(TreeNode* root, unsigned char code, unsigned char bits);
void freeTree(TreeNode *root);
TreeNode* createTreeNode(char letter, unsigned char bit, int amount);
TreeNode* isInCharsList(TreeNode** chars, int charsAmount, char letter);
int compareTreeNodes(const void* a, const void* b);

TreeNode** buildCharactersList(char* string, int* charsAmount) {
  TreeNode** chars = (TreeNode**)malloc(sizeof(TreeNode)*26);
  if (chars == NULL) return NULL;

  for (int i = 0; i < strlen(string); i++) {
    TreeNode *treeNode = isInCharsList(chars, *charsAmount, string[i]);
    
    if (treeNode != NULL) {
      treeNode->amount++;
      continue;
    }

    TreeNode* newTreeNode = createTreeNode(string[i], 0, 1);
    if (newTreeNode == NULL) return NULL;
    chars[*charsAmount] = newTreeNode;
    (*charsAmount)++;
  }

  return chars;
}

TreeNode* buildHaffmanTree(TreeNode** chars, int charsAmount) {
  if (charsAmount == 0) return NULL;
  if (charsAmount == 1) return *chars;

  // Выбираем две минимальных вершины
  TreeNode* min1 = chars[charsAmount-1];
  TreeNode* min2 = chars[charsAmount-2];
  min2->bit = 0b0;
  min1->bit = 0b1;

  // Создаем родительскую вершину
  TreeNode* root1 = createTreeNode(' ', ' ', min1->amount + min2->amount);
  root1->left = min2;
  root1->right = min1;

  chars[charsAmount - 1] = NULL;
  chars[charsAmount - 2] = root1;

  TreeNode* root = buildHaffmanTree(chars, charsAmount-1);

  if (root == NULL) return root1;
  return root;
}

// Программа должна содержать функцию  обхода дерева с выводом  его  содержимого,  функцию  добавления  вершины дерева  (ввод),  а  также  указанную  в  варианте  функцию.
// 12.  Код Хаффмана, учитывающий частоты появления символов, строится следующим образом. Для каждого символа подсчитывается частота его появления и создается вершина двоичного дерева. Затем из множества вершин выбираются две с минимальными частотами появления и создается новая - с суммарной частотой, к которой выбранные подключаются как правое и левое поддерево. Созданная вершина включается в исходное множество, а выбранные - удаляются. Затем процесс повторяется до тех пор, пока не останется единственная вершина. Код каждого символа - это последовательность движения к его вершине от корня (левое поддерево - 0, правое - 1). Функция строит код Хаффмана для символов заданной строки.
int main() {
  char string[] = "Hello";
  
  int charsAmount = 0;
  // Строим массив TreeNode с символами и кол-вом их вхождений  
  TreeNode** chars = buildCharactersList(string, &charsAmount);
  if (chars == NULL) return -1;
  // Сортировка по убыванию
  qsort(chars, charsAmount, sizeof(TreeNode*), compareTreeNodes);

  TreeNode* root = buildHaffmanTree(chars, charsAmount);
  
  printf("\n");
  printTree(root, 0);
  printf("\n");
  printTable(root, 0b0, 0);

  free(chars);
  freeTree(root);
  return 0;
}

int compareTreeNodes(const void* a, const void* b) {
  const TreeNode* t1 = *(TreeNode**)a;
  const TreeNode* t2 = *(TreeNode**)b;
  return t1->amount <= t2->amount;
}

TreeNode* createTreeNode(char letter, unsigned char bit, int amount) {
  TreeNode* treeNode = (TreeNode*)malloc(sizeof(TreeNode));

  if (treeNode != NULL) {
    treeNode->letter = letter;
    treeNode->bit = bit;
    treeNode->amount = amount;
    treeNode->left = NULL;
    treeNode->right = NULL;
  }

  return treeNode;
}

TreeNode* isInCharsList(TreeNode** chars, int charsAmount, char letter) {
  for (int i = 0; i < charsAmount; i++) {
    if (chars[i]->letter == letter) return chars[i];
  }
  return NULL;
}

void freeTree(TreeNode *root) {
  if (root->left) free(root->left);
  if (root->right) free(root->right);
  free(root);
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

void printTable(TreeNode* root, unsigned char code, unsigned char bits) {
  if (root->bit == 0 || root->bit == 1) {
    code = (code << 1) + root->bit;
  }
  // Если конечная вершина, выводим всю информацию
  if (root->left == NULL && root->right == NULL) {
    printf("| '%c' | ", root->letter);
    printBinary(code, bits);
    printf("\n");
    return;
  }
  
  if (root->left != NULL) {
    printTable(root->left, code, bits+1);
  }
  if (root->right != NULL) {
    printTable(root->right, code, bits+1);
  }
}

void printTabs(int n) {
  for (int i = 0; i < 2*n-1; i++) {
    printf("         ");
  }
}

void printTree(TreeNode *root, int level) {
  if (root->left == NULL && root->right == NULL) {
    printf("'%c' [%d]  %2d\n", root->letter, root->bit, root->amount);
    return;
  }
  if (root->bit == 0 || root->bit == 1) {
    printf(" [%d]  %2d ", root->bit, root->amount);
  } else {
    printf("     %2d  ", root->amount);
  }
  printf(" ------> ");

  if (root->right != NULL) {
    printTree(root->right, level+1);
  }
  if (root->left != NULL) {
    printTabs(level+1);
    printf(" \\-----> ");
    printTree(root->left, level+1);
  }
}
