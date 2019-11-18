/*
	If we fix k and increment N we can see that the graphic has a polynomial curve resembling O(n).
	Since K can be viewed as a constant and n is the variable then k can be ignored

	If we fix n and increment k then the graphic looks like a logarithmic curve 
	because N can be seen as a constant and k is the actual variable.
	The complexity of our logarithm is O(n*log(k)) so N doesn't play such an important role in modelling the curve
 */

#include <iostream>
#include <stdio.h>
#include "Profiler.h"
#include <time.h>

#define max_size 10000

using namespace std;

Profiler profiler("MERGE K SORTED LISTS");

int left(int i)
{
	return 2 * i + 1;
}

int right(int i)
{
	return 2 * i + 2;
}

int parent(int i)
{
	return i / 2;
}

struct node
{
	int key;
	node* next;
};

struct list
{
	int len;
	node *first, *last;
};

struct elem
{
	int val;
	int pos;
};


list *listVect[500], *res;
elem a[max_size], v;
int heapSize;

int A_k5, C_k5;
int A_k10, C_k10;
int A_k100, C_k100;
int A_k, C_k;

void initVar()
{
	A_k = 0; C_k = 0;
	A_k5 = 0; C_k5 = 0;
	A_k10 = 0; C_k10 = 0;
	A_k100 = 0; C_k100 = 0;
}

void initList(list *l)
{
	l->len = 0;
	l->first = l->last = NULL;
}

//insert a node at the end od the list
void add(list *l, int k)
{
	node *t = (node*)malloc(sizeof(node));
	t->key = k;
	t->next = NULL;

	if (l->last != 0)
	{
		l->last->next = t;
		l->last = t;
	}
	else
	{
		l->first = t;
		l->last = t;
	}
	l->len++;
}

//remove the forst node from the list
void remove(list *l)
{
	node *t = (node*)malloc(sizeof(node));

	if (l->first != 0)
	{
		t = l->first;
		l->first = l->first->next;
		free(t);

		if (l->first == 0)
		{
			l->last = 0;
		}
		l->len--;
	}
}

void printList(list *l)
{
	if (l->first == 0)
	{
		printf("empty list");
	}
	else
	{
		node *t = l->first;
		while (t != 0)
		{
			printf("%d ", t->key);
			t = t->next;
		}
	}
	printf("\n");
}

void initH(elem a[], int &len)
{
	len = 0;
}

void min_heapify(elem a[], int i, int len, int &assign, int &comp)
{
	int l = left(i);	//left child of i
	int r = right(i);	//right child of i
	int smallest;	//minimum elem from heap

	if (l < len && a[l].val < a[i].val)
	{
		comp++;
		smallest = l;
	}
	else smallest = i;

	if (r < len && a[r].val < a[smallest].val)
	{
		comp++;
		smallest = r;
	}

	if (smallest != i)		//if minimum != root swap the elem and recursively call the affected subtree
	{
		swap(a[i], a[smallest]);
		assign += 3;
		min_heapify(a, smallest, len, assign, comp);
	}
}

void build_min_heap_bu(elem a[], int len, int &assign, int &comp)
{
	for (int i = len / 2 - 1; i >= 0; i--)
		min_heapify(a, i, len, assign, comp);
}

//insert an element in the heap
void hPush(elem a[], int &hSize, elem x, int &assign, int &comp)
{
	hSize++;
	a[hSize - 1] = x;
	assign++;
	build_min_heap_bu(a, hSize, assign, comp);
}

//extract the root from the heap
elem hPop(elem a[], int &hSize, int &assign, int &comp)
{
	elem x;
	if (hSize > 0)
	{
		x = a[0];
		a[0] = a[hSize - 1];
		assign ++;
		hSize--;
	}
	min_heapify(a, 0, hSize, assign, comp);
	return x;
}

//main function that performs the merging of k lists
void merge(int k, int &assign, int &comp)
{
	initH(a, heapSize);

	res = (list*)malloc(sizeof(list));
	initList(res);

	//insert the first element from each list in the heap
	for (int i = 0; i < k; i++)
	{
		if (listVect[i]->first != 0)
		{
			comp++;
			v.val = listVect[i]->first->key;
			v.pos = i;
			hPush(a, heapSize, v, assign, comp);
		}
		comp++;
	}

	//while the heap is not empty, extract minimum and insert 
	//next node from the list from which the root belongs
	while (heapSize != 0)
	{
		v = hPop(a, heapSize, assign, comp);
		assign++;
		add(res, v.val); //add the heap root in the final list
		remove(listVect[v.pos]); //remove the node from the list

		if (listVect[v.pos]->first != 0)
		{
			comp++;
			v.val = listVect[v.pos]->first->key;
			hPush(a, heapSize, v, assign, comp); //insert the next element from the list
		}
		comp++;
	}
}

//generate k lists, with total number of elements equal to n
void Lists(int k, int n, int &assign, int &comp)
{
	int a[max_size];

	for (int i = 0; i < k; i++)
	{
		listVect[i] = (list*)malloc(sizeof(list));
		initList(listVect[i]);
	}

	srand(time(NULL));
	FillRandomArray(a, max_size, 10, 50000, true, 1);

	//randomly distribute the elements in the k lists
	for (int i = 0; i < n; i++)
	{
		int r = rand() % k;
		add(listVect[r], a[i]);
	}

//	for (int i = 0; i < k; i++)
//	{
//		cout << "\n" << i << ": ";
//		printList(listVect[i]);
//	}

//	cout << "\n";
	
	//merge those k lists
	merge(k, assign, comp);
	
//	cout << "Result: ";
//	printList(res);
//	cout << "\n";
}

void test()
{
	int n, k, a, c;
	cout << "Nb of elements: ";
	cin >> n;
	cout << "Nb of lists: ";
	cin >> k;
	Lists(k, n, a, c);
}

void task()
{
	for (int i = 10; i < 500; i += 10)
	{
		cout << "\nk = " << i;
		initVar();
		Lists(i, max_size, A_k, C_k);

		profiler.countOperation("T_k", i, A_k + C_k);
	}

	profiler.createGroup("Total KLists", "T_k");

	for (int n = 100; n < max_size; n += 100)
	{
		cout << "\nn = " << n;
		initVar();
		Lists(5, n, A_k5, C_k5);
		Lists(10, n, A_k10, C_k10);
		Lists(100, n, A_k100, C_k100);

		profiler.countOperation("5Lists", n, A_k5 + C_k5);
		profiler.countOperation("10Lists", n, A_k10 + C_k10);
		profiler.countOperation("100Lists", n, A_k100 + C_k100);
	}

	profiler.createGroup("5,10,100 Lists", "5Lists", "10Lists", "100Lists");
	profiler.showReport();
}

int main()
{
	int d;
	do
	{
		cout << "1. TEST\n2. GENERATE GRAPHS\n\nPRESS 0 TO EXIT\n\nOPTION: ";
		cin >> d;
		switch (d)
		{
		case 0:
		{
			cout << "\nDONE.";
			return 0;
		}
		case 1:
		{
			test();
			break;
		}
		case 2:
		{
			task();
			break;
		}
		}
	} while (d != 1 || d != 0 || d != 2);
}