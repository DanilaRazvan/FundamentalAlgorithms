#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Profiler.h"

using namespace std;

Profiler profiler("BFS");

int op; //variable to count operations



struct qNode {
	int key;
	qNode* next;
};

struct queue {
	qNode* front;
	qNode* rear;
};

qNode* newQueueNode(int k) {
	qNode* temp = (qNode*)malloc(sizeof(qNode));
	temp->key = k;
	temp->next = NULL;

	return temp;
}

queue* initQueue() {
	queue* q = (queue*)malloc(sizeof(queue));
	q->front = q->rear = NULL;

	return q;
}

void enqueue(queue* q, int k) {
	qNode* temp = newQueueNode(k);

	if (q->rear == NULL) {
		q->front = q->rear = temp;
		return;
	}

	q->rear->next = temp;
	q->rear = temp;
}

qNode* dequeue(queue* q) {
	if (q->front == NULL)
		return NULL;

	qNode* temp = q->front;

	return temp;
}

void freeQueueFirst(queue* q) {
	if (q->front != NULL) {
		qNode* temp = q->front;
		q->front = q->front->next;
		if (q->front == NULL) {
			q->rear = NULL;
		}
		free(temp);
	}
}

bool isEmpty(queue* q) {
	if (q->front == NULL) {
		return true;
	}
	return false;
}



struct adjListNode {
	int dest;
	adjListNode* next;
};

struct adjList {
	int d;
	int p;
	int color;
	adjListNode* head;
};

struct graph {
	int V;
	adjList* arr;
};

adjListNode* newAdjListNode(int dest) {
	adjListNode* n = (adjListNode*)malloc(sizeof(adjListNode));
	n->dest = dest;
	n->next = NULL;

	return n;
}

graph* createGraph(int V) {
	graph* g = (graph*)malloc(sizeof(graph));
	g->V = V;

	g->arr = (adjList*)malloc((V + 1) * sizeof(adjList));

	for (int i = 1; i <= V; i++) {
		g->arr[i].head = NULL;
		g->arr[i].color = 0;
		g->arr[i].d = INT_MAX;
		g->arr[i].p = 0;
	}

	return g;
}

void addEdge(graph* g, int src, int dest) {
	adjListNode* n = newAdjListNode(dest);
	n->next = g->arr[src].head;
	g->arr[src].head = n;

	n = newAdjListNode(src);
	n->next = g->arr[dest].head;
	g->arr[dest].head = n;
}



struct NodeV1
{
	int key;
	int k;
	NodeV1** children;
};

NodeV1* newNode1(int key, int c)
{
	NodeV1* p = (NodeV1*)malloc(sizeof(NodeV1));
	p->key = key;
	p->children = (NodeV1**)malloc(c * sizeof(NodeV1*));
	p->k = 0;
	return p;
}

void createMultiwayTree(int p[], int i, NodeV1* created[], NodeV1* root[], int count[], int &k)
{
	if (created[i] != NULL)
		return;
	created[i] = newNode1(i, count[i]);
	if (p[i] == 0)
	{
		root[k] = created[i];
		k++;
		return;
	}
	if (created[p[i]] == NULL)
		createMultiwayTree(p, p[i], created, root, count, k);

	NodeV1* parent = created[p[i]];
	parent->children[parent->k++] = created[i];
}

NodeV1** parentToMultiway(int p[], int n, int &nr)
{
	int* count = (int*)calloc(n + 1, sizeof(int));
	NodeV1** created = (NodeV1**)malloc((n + 1) * sizeof(NodeV1*));

	for (int i = 1; i <= n; i++)
	{
		if (p[i] > 0)
			count[p[i]]++;
		else
			nr++;
		created[i] = NULL;
	}

	NodeV1** root = (NodeV1**)calloc(nr, sizeof(NodeV1*));
	int k = 0;

	for (int i = 1; i <= n; i++)
	{
		createMultiwayTree(p, i, created, root, count, k);
	}

	return root;
}

void printMultiwayTree(NodeV1* root, int lvl)
{
	if (root != NULL)
	{
		for (int i = 0; i < lvl; i++)
		{
			cout << " ";
		}
		lvl += 5;
		cout << root->key << endl;
		for (int i = 0; i < root->k; i++)
		{
			printMultiwayTree(root->children[i], lvl);
		}
	}

}

void BFS_util(graph* g, int s, int* parent, int demo) {

	g->arr[s].color = 1;
	g->arr[s].d = 0;
	g->arr[s].p = 0;
	op += 3;

	queue* q = initQueue();
	enqueue(q, s);	op++;

	while (!isEmpty(q)) {
		op++;

		qNode* u = dequeue(q);
		cout << u->key << " ";
		freeQueueFirst(q);	op++;

		adjListNode* v = g->arr[u->key].head;
		while (v) {
			op++;
			if (g->arr[v->dest].color == 0) {
				g->arr[v->dest].color =g->arr[u->key].d + 1;
				g->arr[v->dest].d =  + 1;
				g->arr[v->dest].p = u->key;

				if (demo == 1) {
					if (u) {
						parent[v->dest] = u->key;
					}
				}

				enqueue(q, v->dest);
				op += 4;
			}
			op += 2;
			v = v->next;
		}
		op += 2;
		g->arr[u->key].color = 2;
	}
	op++;

}

void BFS(graph* g, int s) {

	int nr = 1;
	int* parent = (int*)malloc(g->V * sizeof(int));
	for (int i = 1; i <= g->V; i++) {
		parent[i] = 0;
	}

	BFS_util(g, s, parent, 1);

	for (int u = 1; u <= g->V; u++) {
		if (g->arr[u].color == 0) {
			BFS_util(g, u, parent, 1);
			nr++;
		}
	}

	cout << "\n\n";
	for (int i = 1; i <= g->V; i++) {
		cout << "node: " << i << ", parent: " << parent[i] << "\n";
	}

	cout << "\n\n";
	NodeV1** roots = parentToMultiway(parent, g->V, nr);
	for (int i = 0; i < nr; i++)
	{
		cout << "Component " << i + 1 << endl;
		printMultiwayTree(roots[i], 0);
		cout << endl;
	}
}

bool isOk(graph* g, int s, int d) {
	adjListNode* v = g->arr[s].head;
	while (v != NULL) {
		if (v->dest == d)
			return false;
		v = v->next;
	}

	return true;
}

void generateEdge(graph* g) {
	srand(time(NULL));
	int s;
	int d;
	do {
		s = rand() % g->V + 1;
		d = rand() % g->V + 1;
	} while (s == d || !isOk(g, s, d));

	addEdge(g, s, d);
}

graph* generateGraph(int V, int E) {
	graph* g = createGraph(V);
	for (int i = 0; i < E; i++) {
		generateEdge(g);
	}

	return g;
}

int main() {

	/*********************************************************************
									DEMO
	*********************************************************************/
	
	int E;
	int V = 10;
	graph* g = createGraph(V);

	addEdge(g, 1, 2);
	addEdge(g, 1, 9);
	addEdge(g, 1, 8);
	addEdge(g, 8, 3);
	addEdge(g, 8, 7);
	addEdge(g, 3, 4);
	addEdge(g, 6, 5);
	addEdge(g, 6, 10);
	addEdge(g, 5, 10);

	cout << "BFS traversal: ";
	BFS(g, 2);

	free(g);
	/*********************************************************************
									TASK
	*********************************************************************/
	/*
	// FIRST
	int E;
	int V = 100;
	graph* g;
	for (E = 1000; E < 4500; E += 100) {
		
		cout << "\n 1. " << E;
		
		g = generateGraph(V, E);
		op = 0;
		int s = rand() % V + 1;
		BFS(g, s, 0);
		profiler.countOperation("Fixed V", E, op);
	}

	// SECOND
	E = 4500;
	for (V = 100; V < 200; V += 10) {
		
		cout << "\n 2. " << V;

		g = generateGraph(V, E);
		op = 0;
		int s = rand() % V + 1;
		BFS(g, s, 0);
		profiler.countOperation("Fixed E", V, op);
	}

	profiler.showReport();
	*/
	int d;
	cin >> d;
	return 0;
}