#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>

#include "Profiler.h"

using namespace std;

Profiler profiler("Disjoint Sets");

int n;
int m;	//variable for counting "make" operations
int u;	//variable for counting "union" operations
int f;	//variable for counting "find" operations

void initVar() {
	m = 0;
	u = 0;
	f = 0;
}

struct node {
	int key;
	int rank;
	node* parent;
};

struct edge {
	int weight;
	node* src;
	node* dest;
};

struct graph {
	int V;	//number of verteces
	int E;	//number of edges
	edge* edges;
	node** nodes;
};

graph* createGraph(int V, int E) {
	graph* g = (graph*)malloc(sizeof(graph));
	g->E = E;
	g->V = V;
	g->nodes = (node**)malloc((V + 1) * sizeof(node));
	g->edges = (edge*)malloc(E * sizeof(edge));

	return g;
}

node* make_set(int x) {
	node* p = (node*)malloc(sizeof(node));
	p->key = x;
	p->rank = 0;
	p->parent = p;
	m += 3;	//count operations
	return p;
}

node* find_set(node* x) {
	f += 2;	//count operations
	if (x != x->parent) {
		x->parent = find_set(x->parent);
	}

	return x->parent;
}

void link(node* x, node* y) {
	u++;	//count operations
	if (x->rank > y->rank) {
		y->parent = x;
		u++;	//count operations
	}
	else {
		x->parent = y;
		u += 2;	//count operations
		if (x->rank == y->rank) {
			u++;
			y->rank++;
		}
	}
}

void unite(node* x, node* y) {
	link(find_set(x), find_set(y));
}

int partition(edge a[], int start, int end) {
	int x = a[end].weight;
	int index = start - 1;
	for (int i = start; i < end; i++) {
		if (a[i].weight <= x) {
			index++;
			swap(a[i], a[index]);
		}
	}
	swap(a[end], a[index + 1]);
	return index + 1;
}

int randpartition(edge a[], int start, int end) {
	int i = rand() % (end - start) + start;
	swap(a[i], a[end]);
	return partition(a, start, end);
}

void quickSort(edge a[], int start, int end) {
	if (start < end) {
		int p = randpartition(a, start, end);
		quickSort(a, start, p - 1);
		quickSort(a, p + 1, end);
	}
}

graph* mst;
int mstSize;

void kruskal(graph* g) {
	mstSize = 0;

	mst = createGraph(g->V, g->V - 1);
	
	for (int v = 1; v <= g->V; v++) {
		mst->nodes[v] = make_set(g->nodes[v]->key);
	}
	
	//sort edges
	quickSort(g->edges, 0, g->E - 1);

	//for each edge in ascending order
	for (int i = 0; i < g->V; i++) {

		node* s = find_set(g->edges[i].src);
		node* d = find_set(g->edges[i].dest);

		//if the source and destination vertices have the same parent,
		//the edge closes a loop
		if (s != d) {
			mst->edges = (edge*)realloc(mst->edges, (mstSize + 1) * sizeof(edge)); //dynamic memory allocation
			mst->edges[mstSize++] = g->edges[i];
			unite(s, d);
		}
	}
}

bool isUnique(edge e[], int E, node* s, node* d) {
	for (int i = 0; i < E; i++) {
		if ((e[i].src == s && e[i].dest == d) || (e[i].src == d && e[i].dest == s) || s == d)
			return false;
	}
	return true;
}

int main() {

	/*************************************************************************
	                                  DEMO
	**************************************************************************/

	cout << "Number of nodes: ";
	cin >> n;
	node** nodes = (node**)malloc(n * sizeof(node));

	cout << "BEFORE UNION\n";
	for (int i = 0; i < n; i++) {
		nodes[i] = make_set(i);
		cout << nodes[i]->key << " -> " << find_set(nodes[i])->key << endl;
	}

	cout << "\nAFTER UNION\n";
	unite(nodes[0], nodes[2]);
	unite(nodes[3], nodes[2]);
	unite(nodes[2], nodes[9]);
	unite(nodes[4], nodes[8]);
	unite(nodes[9], nodes[1]);

	for (int i = 0; i < n; i++) {
		cout << nodes[i]->key << " -> " << find_set(nodes[i])->key << endl;
	}

	cout << "\nKRUSKAL\n";

	int V = 5;
	int E = 7;

	graph* g = createGraph(V, E);

	for (int v = 1; v <= V; v++) {
		g->nodes[v] = make_set(v);
	}

	g->edges[0].src = g->nodes[1];
	g->edges[0].dest = g->nodes[2];
	g->edges[0].weight = 10;

	g->edges[1].src = g->nodes[1];
	g->edges[1].dest = g->nodes[5];
	g->edges[1].weight = 3;

	g->edges[2].src = g->nodes[1];
	g->edges[2].dest = g->nodes[4];
	g->edges[2].weight = 5;

	g->edges[3].src = g->nodes[2];
	g->edges[3].dest = g->nodes[3];
	g->edges[3].weight = 9;

	g->edges[4].src = g->nodes[2];
	g->edges[4].dest = g->nodes[4];
	g->edges[4].weight = 12;

	g->edges[5].src = g->nodes[3];
	g->edges[5].dest = g->nodes[4];
	g->edges[5].weight = 2;

	g->edges[6].src = g->nodes[4];
	g->edges[6].dest = g->nodes[5];
	g->edges[6].weight = 7;

	cout << "\nInitial graph:\n";
	for (int i = 0; i < E; i++) {
		cout << g->edges[i].src->key << "---" << g->edges[i].dest->key << "; weight: " << g->edges[i].weight << "\n";
	}

	kruskal(g);

	cout << "\nMST:\n";
	for (int i = 0; i < mstSize; i++) {
		cout << mst->edges[i].src->key << "---" << mst->edges[i].dest->key << "; weight: " << mst->edges[i].weight << "\n";
	}

	/*****************************************************************************
										TASK
	******************************************************************************/
	/*
	int *weights;

	for (n = 100; n < 10000; n += 100) {
		graph* gr = createGraph(n, n * 4);
		
		weights = (int*)malloc(4*n*sizeof(int));
		FillRandomArray(weights, 4*n, 1, 50000, false, 0);
		cout << n << endl;

		initVar();

		for (int i = 1; i <= n; i++) {
			gr->nodes[i] = make_set(i);
		}

		for (int i = 0; i < 4 * n; i++) {
			node* s;
			node* d;
			do {
				s = gr->nodes[rand() % n + 1];
				d = gr->nodes[rand() % n + 1];
			} while (!isUnique(gr->edges, i, s, d));

			gr->edges[i].src = s;
			gr->edges[i].dest = d;
			gr->edges[i].weight = weights[i];
		}

		kruskal(gr);

		int total = m + u + f;

		profiler.countOperation("make", n, m);
		profiler.countOperation("union", n, u);
		profiler.countOperation("find", n, f);
		profiler.countOperation("total", n, total);
	}

	profiler.createGroup("Disjoint Sets", "make", "union", "find");
	profiler.showReport();
	*/
	int d;
	cin >> d;
	
	return 0;
}