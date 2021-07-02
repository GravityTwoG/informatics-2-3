#include <iostream>
#include <cmath>
using namespace std;

struct list {
	int val;
	list* next;

	void push(list*& head, int val) {
		list* newListItem = new list;
		newListItem->next = NULL;
		newListItem->val = val;

		if (head == NULL) {
			head = newListItem;
		} else {
			list* temp = head;
			for (; temp->next != NULL; temp = temp->next); // получение последнего эл-та списка
			temp->next = newListItem;
		}
	}
};

list** init(list* mas, int mas_size, int N) {
	int list_size = ceil((float)mas_size / N);

	list** result = new list* [N+1];
	list* currentNode = mas;
	
	for (int i = 0; i < N && currentNode != NULL; i++) {
		result[i] = NULL;
		
		for (
			int j = 0; 
			j < list_size && currentNode != NULL; 
			j++, currentNode = currentNode->next) {
			
			result[i]->push(result[i], currentNode->val);
		}
	}	
	result[N] = NULL;

	return result;
}

void show(list** temp) {
	cout << "Lists:" << endl;
	
	for (int i = 0; temp[i] != NULL; i++) {
		cout << i+1 << ": ";
		
		for (
			list* currentNode = temp[i]; 
			currentNode != NULL; 
			currentNode = currentNode->next
			) {
			cout << currentNode->val << " ";		
		}

		cout << endl << endl;
	}
}

int main() {
	int Array[] = { 3,5,1,9,3,2,4,7,5,6,4,0,6,6,3,4,2,2,4,3,2,3,5,4,3,2,7 };
	int mas_size = sizeof(Array) / sizeof(Array[0]);
	
	// Преобразование массива в односвязный список
	list* mas = new list;
	list* currentNode = mas;
	currentNode->val = Array[0];
  for (int i = 1; i < mas_size; i++) {  
		list* listItem = new list;
		listItem->next = NULL;
		listItem->val = Array[i];
		
		currentNode->next = listItem;
		currentNode = listItem;
  }

	cout << "Array: " << endl;
	for (list* temp = mas; temp != NULL; temp = temp->next) {
		cout << temp->val << " ";
	}
	cout << endl << endl;

	list** lists = init(mas, mas_size, 3);
	show(lists);

	delete mas;
	for (int i = 0; lists[i] != NULL; i++) {
		delete lists[i];
	}
	delete lists;

	system("pause");
	return 0;
}