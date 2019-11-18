#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

const int n = 10;

//multiwayTree node
struct node_v2 {
	int key;
	node_v2* parent;
	int nbChildren;
	node_v2** children;
};

//binaryTree node
struct node_v3 {
	int key;
	node_v3* left;
	node_v3* right;
}; 

//create new node for multiwayTree
node_v2* newNodeV2(int key) {
	node_v2* n = (node_v2*)malloc(sizeof(node_v2));
	n->parent = NULL;
	n->nbChildren = 0;
	n->key = key;
	n->children = new node_v2*[5];

	return n;
}
 
//create new node for binaryTree
node_v3* newNodeV3(int key) {
	node_v3* n = (node_v3*)malloc(sizeof(node_v3));
	n->key = key;
	n->left = n->right = NULL;

	return n;
}

//"insert" node into multiwayTree
void createNode(int* arr, int i, node_v2* created[], node_v2** root) {
	
	//check if node is already created
	if (created[i] != NULL)
		return;

	created[i] = newNodeV2(i);

	if (arr[i] == 0) {
		*root = created[i];
		return;
	}

	//create node's parent
	if (created[arr[i]] == NULL)
		createNode(arr, arr[i], created, root);

	//set pointers
	node_v2* p = created[arr[i]];
	created[i]->parent = p;

	p->children[p->nbChildren] = created[i];
	p->nbChildren++;
}

//function to construct multiwayTree from parent vector
node_v2* parentToMultiway(int* arr) {
	node_v2* root = NULL;

	//to avoid O(n^2) complexity and reach O(n) we need additional space
	//in order to memorize the created nodes and not to search every time
	node_v2* created[n];
	for (int i = 0; i < n; i++) {
		created[i] = NULL;
	}

	for (int i = 1; i <= n; i++) {
		createNode(arr, i, created, &root);
	}

	return root;
}

//function to construct binaryTree from multiwayTree
node_v3* multiwayToBinary(node_v2* root, int c) {
	
	node_v3* node = newNodeV3(root->key);
	
	if (root->nbChildren > 0) {
		node->left = multiwayToBinary(root->children[0], 0);
	}
	else {
		node->left = NULL;
	}

	if (root->parent != NULL && c < root->parent->nbChildren - 1) {
		node->right = multiwayToBinary(root->parent->children[c + 1], c + 1);
	}
	else {
		node->right = NULL;
	}

	return node;
}

void printMultiwayTree(node_v2* root) {
	cout << "node " << root->key << " has children: ";
	for (int i = 0; i < root->nbChildren; i++) {
		cout << root->children[i]->key << " ";
	}
	cout << endl;
	for (int i = 0; i < root->nbChildren; i++) {
		if (root->children[i]->nbChildren != 0) {
			printMultiwayTree(root->children[i]);
		}
	}
}

void inorder(node_v3* root, int level) {
	if (root != NULL)
	{
		inorder(root->right, level + 1);
		for (int i = 0; i <= level; i++)
			cout << "    ";
		cout << root->key << "\n";
		inorder(root->left, level + 1);
	}
}

int main() {
	int arr[n];
	for (int i = 1; i < n; i++)
	{
		cout << "parent of elem " << i << " : ";
		cin >> arr[i];
	}

	cout << "\nMultiwayTree:\n";
	node_v2* mRoot = parentToMultiway(arr);
	printMultiwayTree(mRoot);
	
	cout << "\nBinaryTree:\n\n";
	node_v3* bRoot = multiwayToBinary(mRoot, 0);
	inorder(bRoot, 0);

	int d;
	cin >> d;
	if (d == 0) {
		return 0;
	}
}
