#include "Profiler.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#define max_size 10000

using namespace std;

Profiler profiler("DynamicStatistics");

int del_op;
int sel_op;

struct node
{
	int key;
	int size;
	node* right;
	node* left;
	node* parent;
};

node* newNode(int data)
{
	node* p = (node*)malloc(sizeof(node));
	p->key = data;
	p->left = NULL;
	p->right = NULL;
	p->parent = NULL;
	p->size = 1;

	return p;
}

node* buildBST(int arr[], int start, int end, node* parent)
{
	if (start <= end)
	{
		// Get the middle element and make it root
		int mid = (start + end) / 2;
		node *root = newNode(arr[mid]);
		root->parent = parent;

		root->left = buildBST(arr, start, mid - 1, root);
		root->right = buildBST(arr, mid + 1, end, root);

		// post-order set sizes
		if (root->left != NULL)
		{
			root->size = root->size + root->left->size;
		}

		if (root->right != NULL)
		{
			root->size = root->size + root->right->size;
		}

		return root;
	}
	else
	{
		return NULL;
	}
}

node* OS_Select(node* root, int i) 
{
	int r;

	sel_op++;
	if (root->left == NULL)
	{
		r = 1;
	}
	else
	{
		r = root->left->size + 1;
	}
	
	sel_op++;
	if (i == r)
	{
		return root;
	}
	else
	{
		sel_op++;
		if (i < r)
		{
			//search in left subtree
			return OS_Select(root->left, i);
		}
		else //i > r
		{
			//search in right subtree
			return OS_Select(root->right, i - r);
		}
	}
}

node* minValue(node* n)
{
	//min value from right subtree
	node *curr = n;

	while (curr->left != NULL)
	{
		curr = curr->left;
	}

	return curr;
}

node* succesor(node* root)
{
	if (root->right != NULL)
	{
		return minValue(root->right);
	}

	node* n = root->parent;
	while (n != NULL && root == n->right)
	{
		root = n;
		n = root->parent;
	}
	return n;
}

void setSizes(node* root)
{
	node* n = root->parent;
	while (n != NULL)
	{
		del_op++;
		n->size--;
		n = n->parent;
	}
}

void cpyInf(node* a, node* b)
{
	a->key = b->key;
}

node* OS_Delete(node **root, node* z)
{
	node* y; //node which we physically delete
	node* x; 

	del_op += 2;
	if (z->left == NULL || z->right == NULL)
	{
		del_op++;
		y = z;
	}
	else
	{
		del_op++;
		y = succesor(z);
	}

	del_op++;
	if (y->left != NULL)
	{
		del_op++;
		x = y->left;
	}
	else
	{
		del_op++;
		x = y->right;
	}

	del_op++;
	if (x != NULL)
	{
		del_op++;
		x->parent = y->parent;
	}
	
	if (y->parent == NULL)
	{
		del_op++;
		*root = x;
	}
	else
	{
		del_op++;
		if (y == y->parent->left)
		{
			del_op++;
			y->parent->left = x;
		}
		else
		{
			del_op++;
			y->parent->right = x;
		}
	}
	
	setSizes(y);

	return y;
}

//pretty print
void inorder(node *root, int level)
{
	if (root != NULL)
	{
		inorder(root->right, level + 1);
		for (int i = 0; i <= level; i++)
			cout << "    ";
		cout << root->key << "-" << root->size << "\n";
		inorder(root->left, level + 1);
	}
}

void demo()
{
	srand(time(NULL));
	//DEMO
	
	int n;
	cout << "Array Lenght: ";
	cin >> n;

	int *a = (int*)malloc(n * sizeof(int));
	FillRandomArray(a, n, 1, n, true, 1);

	//build BST
	node *root = buildBST(a, 0, n - 1, NULL);

	inorder(root, 0);

	node* y;
	node* z;

//	while( root != NULL ) {
		cout << "\n\n";
//		int random = rand() % (root->size)+1;

		//delete 5
		cout << "\nPOSITION: " << 4 << "\n";
		z = OS_Select(root, 4);
		cout << "element on position " << 4 << " is: " << z->key << "\n";
		cout << "Delete: " << z->key << "\n";
		y = OS_Delete(&root, z);

		cpyInf(z, y);
		free(y);

		inorder(root, 0);

		//delete 4
		cout << "\nPOSITION: " << 4 << "\n";
		z = OS_Select(root, 4);
		cout << "element on position " << 4 << " is: " << z->key << "\n";
		cout << "Delete: " << z->key << "\n";
		y = OS_Delete(&root, z);

		cpyInf(z, y);
		free(y);

		inorder(root, 0);

		//delete root
		cout << "\nPOSITION: " << 4 << "\n";
		z = OS_Select(root, 4);
		cout << "element on position " << 4 << " is: " << z->key << "\n";
		cout << "Delete: " << z->key << "\n";
		y = OS_Delete(&root, z);

		cpyInf(z, y);
		free(y);

		inorder(root, 0);
//	}
}

void initVar()
{
	//init global variables
	del_op = 0;
	sel_op = 0;
}

void mean(int n)
{
	del_op /= 5;
	sel_op /= 5;
}

void task()
{
	srand(time(NULL));
	
	int arr[max_size];

	for (int n = 100; n < max_size; n += 100)
	{
		initVar();
		for (int i = 0; i < 5; i++)
		{
			cout << "\n" << n << " " << i;
			FillRandomArray(arr, n, 1, n, true, 1);
			node* root = buildBST(arr, 0, n - 1, NULL);
			int elem = n;
			for (int k = 0; k < n; k++)
			{
				int random = rand() % elem + 1;
				node* z = OS_Select(root, random);
				node* y = OS_Delete(&root, z);
				cpyInf(z, y);
				free(y);
				elem--;
			}
		}

		mean(n);

		profiler.countOperation("OS_SELECT", n, sel_op);
		profiler.countOperation("OS_DELETE", n, del_op);
	}

	profiler.showReport();
}

int main()
{
	int d;
	do
	{
		cout << "\n1. DEMO\n2. TASK\n\nPRESS 0 TO EXIT\n\nOption: ";
		cin >> d;

		if (d == 0)
		{
			cout << "Program terminated!";
			return 0;
		}
		else if (d == 1)
		{
			demo();
		}
		else if (d == 2)
		{
			task();
		}
	} while (d != 0 || d != 1 || d != 2);
}