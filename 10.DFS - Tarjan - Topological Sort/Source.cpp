#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Profiler.h"

using namespace std;

Profiler profiler("DFS");

int op;
int t;
int step = 0;

struct ListNode {
	int data;
	ListNode* next;
};

ListNode* newListNode(int data) {
	ListNode* n = (ListNode*)malloc(sizeof(ListNode));
	n->data = data;
	n->next = NULL;

	return n;
}

struct List {
	ListNode* first;
	ListNode* last;
};

List* initList() {
	List* l = (List*)malloc(sizeof(List));

	l->first = l->last = NULL;
	return l;
}

void frontInsert(List* l, int data) {
	ListNode* n = newListNode(data);

	n->next = l->first;
	l->first = n;
}

void printList(List* l) {
	
	ListNode* w = l->first;
	while (w) {
		cout << w->data << " ";
		w = w->next;
	}
}



struct StackNode
{
	int data;
	StackNode* next;
};

struct Stack {
	StackNode* first;
	StackNode* last;
};

StackNode* newNode(int data)
{
	StackNode* stackNode = (StackNode*) malloc(sizeof(StackNode));
	stackNode->data = data;
	stackNode->next = NULL;
	return stackNode;
}

Stack* initStack() {
	Stack* s = (Stack*)malloc(sizeof(Stack));
	s->first = s->last = NULL;

	return s;
}

int isEmpty(Stack *s)
{
	if (s->first == NULL) {
		return 1;
	}
	return 0;
}

void push(Stack* s, int data)
{
	StackNode* stackNode = newNode(data);

	if (s->first == NULL) {
		s->first = stackNode;
		s->last = stackNode;
		return;
	}

	stackNode->next = s->first;
	s->first = stackNode;

	//	printf("%d pushed to stack\n", data);
}

int pop(Stack* s)
{
	if (isEmpty(s))
		return NULL;
	StackNode* temp = s->first;
	s->first = s->first->next;
	int popped = temp->data;
	free(temp);

	return popped;
}

int top(Stack* s)
{
	if (isEmpty(s))
		return NULL;
	return s->first->data;
}



struct adjListNode {
	int dest;
	adjListNode* next;
};

struct adjList {
	int key;
	int f;
	int d;
	int p;
	int color;
	adjListNode* head;
	bool on_stack;
	int low_link;
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

adjList* initNode(adjList* n, int i) {
	n->key = i;
	n->head = NULL;
	n->color = 0;
	n->d = 0;
	n->f = 0;
	n->p = 0;
	n->on_stack = false;
	n->low_link = 0;

	return n;
}

graph* createGraph(int V) {
	graph* g = (graph*)malloc(sizeof(graph));
	g->V = V;

	g->arr = (adjList*)malloc((V + 1) * sizeof(adjList));

	for (int i = 1; i <= V; i++) {
		initNode(&g->arr[i], i);
	}

	return g;
}

void addEdge(graph* g, int src, int dest) {
	adjListNode* n = newAdjListNode(dest);
	n->next = g->arr[src].head;
	g->arr[src].head = n;
}



void DFSVisit(graph* g, adjList* u, bool topo, List* l) {
	t++;
	u->d = t;
	u->color = 1;
	op += 2;

	if (topo == false) {
		cout << u->key << " ";
	}

	adjListNode* v = u->head;
	op++;
	while (v) {
		op += 2;
		if (g->arr[v->dest].color == 0) {
			g->arr[v->dest].p = u->key;
			op ++;
			DFSVisit(g, &g->arr[v->dest], topo, l);
		}
		op++;
		v = v->next;
	}
	op++;
	u->color = 2;
	t++;
	u->f = t;
	op += 2;
	if (topo == true) {
		frontInsert(l, u->key);
	}
}

void DFS(graph* g, bool topo, List* l) {
	t = 0;
	for (int u = 1; u <= g->V; u++) {
		if (g->arr[u].color == 0) {
			DFSVisit(g, &g->arr[u], topo, l);
		}
	}
}



void SCC_visit(graph* g, int node, Stack* S) {

	g->arr[node].d = g->arr[node].low_link = ++step;
	push(S, node);
	g->arr[node].on_stack = true;

	adjListNode* p = g->arr[node].head;

	while (p != NULL) {
		int v = p->dest;

		if (g->arr[v].d == 0) {
			SCC_visit(g, v, S);
			g->arr[node].low_link = min(g->arr[node].low_link, g->arr[v].low_link);
		}
		else if (g->arr[v].on_stack == true) {
			g->arr[node].low_link = min(g->arr[node].low_link, g->arr[v].d);
		}
		p = p->next;
	}

	int w;
	if (g->arr[node].low_link == g->arr[node].d) {
		while (top(S) != node) {
			w = pop(S);
			printf("%d ", w);
			g->arr[w].on_stack = false;
		}
		w = pop(S);
		printf("%d \n", w);
		g->arr[w].on_stack = false;
	}
}

void SCC(graph* g) {
	
	Stack *S = initStack();

	for (int i = 1; i <= g->V; i++) {
		if (g->arr[i].d == 0) {
			SCC_visit(g, g->arr[i].key, S);
		}
	}

}



void topoSort(graph* g) {
	List* l = initList();

	for (int i = 1; i <= g->V; i++) {
		initNode(&g->arr[i], i);
	}

	addEdge(g, 6, 3);
	addEdge(g, 5, 1);
	addEdge(g, 5, 2);
	addEdge(g, 4, 2);
	addEdge(g, 2, 3);
	addEdge(g, 1, 6);

	DFS(g, true, l);

	printList(l);
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
	/*******************************************/
	/*******************DEMO********************/
	/*******************************************/
	int v = 6;

	graph* g = createGraph(v);
	
	addEdge(g, 6, 3);
	addEdge(g, 6, 1);
	addEdge(g, 5, 1);
	addEdge(g, 5, 2);
	addEdge(g, 3, 4);
	addEdge(g, 4, 2);
	addEdge(g, 2, 3);
	addEdge(g, 1, 6);

	cout << "GRAPH:\n";
	for (int i = 1; i <= g->V; i++) {
		cout << "node " << i << ": ";
		
		adjListNode* u = g->arr[i].head;
		while (u) {
			cout << " " << u->dest;
			u = u->next;
		}
		cout << endl;
	}

	cout << "\nDFS Traversal: ";
	DFS(g, false, NULL);

	cout << "\n\nDFS Tree:";
	for (int i = 1; i <= g->V; i++) {
		cout << "\n" << i << "'s parent: " << g->arr[i].p;
	}

	cout << "\n\nSCC:\n";
	for (int i = 1; i <= g->V; i++) {
		initNode(&g->arr[i], i);
	}

	addEdge(g, 6, 3);
	addEdge(g, 6, 1);
	addEdge(g, 5, 1);
	addEdge(g, 5, 2);
	addEdge(g, 3, 4);
	addEdge(g, 4, 2);
	addEdge(g, 2, 3);
	addEdge(g, 1, 6);

	SCC(g);

	cout << "\nTOPOLOGICAL SORT: ";
	topoSort(g);

	/*******************************************/
	/*******************TASK********************/
	/*******************************************/

	// FIRST
	//int E;
	//int V = 100;
	//graph* g;
	//for (E = 1000; E < 4500; E += 100) {

	//	cout << "\n 1. " << E;

	//	g = generateGraph(V, E);
	//	op = 0;
	//	int s = rand() % V + 1;
	//	cout << "\n";
	//	DFS(g, false, NULL);
	//	profiler.countOperation("Fixed V", E, op);
	//}

	//// SECOND
	//E = 4500;
	//for (V = 100; V < 200; V += 10) {

	//	cout << "\n 2. " << V;

	//	g = generateGraph(V, E);
	//	op = 0;
	//	int s = rand() % V + 1;
	//	cout << "\n";
	//	DFS(g, false, NULL);
	//	profiler.countOperation("Fixed E", V, op);
	//}

	//profiler.showReport();

	getchar();
	return 0;
}