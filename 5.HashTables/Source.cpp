/*
	Danila Razvan - group 30424

	The effort for hit is smaller than the effort for miss

	The complexity is O(1)
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Profiler.h"

#pragma warning(disable:4996);

using namespace std;

Profiler p("Hash Tables");

const int n = 10007;
int c1 = 1;
int c2 = 1;

struct entry
{
	int id;
	char name[30];
};

//hashfunction
int h(int key, int i, int n)
{
	return (key + c1 * i + c2 * i*i) % n;
}

int hashInsert(int key, int n, entry t[])
{
	int i = 0;
	int j;

	do
	{
		j = h(key, i, n);
		if (t[j].id == NULL)
		{
			t[j].id = key;		//if slot empty insert key
			return j;
		}
		else i++;
	} while (i != n);

	//if not empty slot return -1
	return -1;
}

int hashSearch(int key, int n, entry t[], Operation op)
{
	int i = 0;
	int j;

	do
	{
		j = h(key, i, n);
		op.count();
		if (t[j].id == key)
		{
			return j;	//if found return element
		}
		i++;
	} while (i != n && t[j].id != NULL);

	//if not found return -1
	return -1;
}

//set all cells from hashTable to NULL
void initHashTable(entry t[])
{
	for (int i = 0; i < n; i++)
	{
		t[i].id = NULL;
	}
}

void solve(entry t[], double fillFactor, Operation f, Operation nf, int &maxf, int &maxnf)
{
	for (int k = 0; k < 5; k++)
	{
		initHashTable(t);

		int hit[n], miss[1500];
		FillRandomArray(hit, n*fillFactor, 10, 30000, true, 0);
		FillRandomArray(miss, 1500, 30001, 50000, true, 0);

		//insert elements in hashTable
		for (int i = 0; i < fillFactor*n; i++)
		{
				hashInsert(hit[i], n, t);
		}

		maxf = 0;
		maxnf = 0;

		for (int m = 0; m < 1500; m++)
		{
			int tempFound = f.get();
			int tempNotFound = nf.get();

			hashSearch(t[m].id, n, t, f);
			hashSearch(miss[m], n, t, nf);

			//get maximum effort foar both hit and miss
			if (f.get() - tempFound > maxf)
			{
				maxf = f.get() - tempFound;
			}

			if (nf.get() - tempNotFound > maxf)
			{
				maxnf = nf.get() - tempNotFound;
			}
		}
	}
}

void task()
{
	entry t[n];
	double fillFactor[] = { 0.80, 0.85, 0.90, 0.95, 0.99 };

	int maxf[] = { 0, 0, 0, 0, 0 };		//max effort for hit for each fillfactor
	int maxnf[] = { 0, 0, 0, 0, 0 };	//max effort for miss for each fillfactor


	//operations for each fillfactor
	Operation found0 = p.createOperation("0.80 F", n);
	Operation found1 = p.createOperation("0.85 F", n);
	Operation found2 = p.createOperation("0.90 F", n);
	Operation found3 = p.createOperation("0.95 F", n);
	Operation found4 = p.createOperation("0.99 F", n);

	Operation nfound0 = p.createOperation("0.80 nF", n);
	Operation nfound1 = p.createOperation("0.85 nF", n);
	Operation nfound2 = p.createOperation("0.90 nF", n);
	Operation nfound3 = p.createOperation("0.95 nF", n);
	Operation nfound4 = p.createOperation("0.99 nF", n);

	//solve each case and divide the values (averageCase) for the table
	solve(t, fillFactor[0], found0, nfound0, maxf[0], maxnf[0]);
	solve(t, fillFactor[1], found1, nfound1, maxf[1], maxnf[1]);
	solve(t, fillFactor[2], found2, nfound2, maxf[2], maxnf[2]);
	solve(t, fillFactor[3], found3, nfound3, maxf[3], maxnf[3]);
	solve(t, fillFactor[4], found4, nfound4, maxf[4], maxnf[4]);

	p.divideValues("0.80 F", 5);
	p.divideValues("0.85 F", 5);
	p.divideValues("0.90 F", 5);
	p.divideValues("0.95 F", 5);
	p.divideValues("0.99 F", 5);
	p.divideValues("0.80 nF", 5);
	p.divideValues("0.85 nF", 5);
	p.divideValues("0.90 nF", 5);
	p.divideValues("0.95 nF", 5);
	p.divideValues("0.99 nF", 5);

	//print the table
	FILE *fout = fopen("hash.txt", "w");

	fprintf(fout, "FILL       AVG_E_F      MAX_E_F       AVG_E_NF        MAX_E_NF\n\n");
	fprintf(fout, "%.2f        %.2f           %d            %.2f             %d\n",
		fillFactor[0], found0.get() / double(1500), maxf[0], nfound0.get() / double(1500), maxnf[0]);
	fprintf(fout, "%.2f        %.2f           %d            %.2f             %d\n",
		fillFactor[1], found1.get() / double(1500), maxf[1], nfound1.get() / double(1500), maxnf[1]);
	fprintf(fout, "%.2f        %.2f           %d            %.2f             %d\n",
		fillFactor[2], found2.get() / double(1500), maxf[2], nfound2.get() / double(1500), maxnf[2]);
	fprintf(fout, "%.2f        %.2f           %d            %.2f             %d\n",
		fillFactor[3], found3.get() / double(1500), maxf[3], nfound3.get() / double(1500), maxnf[3]);
	fprintf(fout, "%.2f        %.2f           %d            %.2f             %d\n",
		fillFactor[4], found4.get() / double(1500), maxf[4], nfound4.get() / double(1500), maxnf[4]);

	fclose(fout);
}

void demo()
{
	//DEMO
	entry demo[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	int demoSize = 7;

	cout << "4 Elements for insertion:\n";
	entry one, two, three, four;

	cout << "1ST ELEMENT:\n";
	cout << "id: ";		cin >> one.id;
	cout << "name: ";	cin >> one.name;

	cout << "\n2ND ELEMENT:\n";
	cout << "id: ";		cin >> two.id;
	cout << "name: ";	cin >> two.name;

	cout << "\n3RD ELEMENT:\n";
	cout << "id: ";		cin >> three.id;
	cout << "name: ";	cin >> three.name;

	cout << "\n4TH ELEMENT:\n";
	cout << "id: ";		cin >> four.id;
	cout << "name: ";	cin >> four.name;

	hashInsert(one.id, demoSize, demo); //insert into table
	hashInsert(two.id, demoSize, demo);
	hashInsert(three.id, demoSize, demo);
	hashInsert(four.id, demoSize, demo);

	cout << "\n1 element which is not in the HashTable for testing the search process:\n";
	entry notF;
	cout << "Element id: ";
	cin >> notF.id;
	cout << "Element name: ";
	cin >> notF.name;
	cout << "\n";

	cout << "\n\nTABLE:\n";
	for (int i = 0; i < demoSize; i++)
	{
		cout << demo[i].id << " ";
	}
	cout << "\n\n";

	Operation op = p.createOperation("demo", 2);

	if (hashSearch(one.id, demoSize, demo, op) != -1)
	{
		printf("Element %s found!\n", one.name);
	}
	else printf("Element %s not found!\n", one.name);

	if (hashSearch(two.id, demoSize, demo, op) != -1)
	{
		printf("Element %s found!\n", two.name);
	}
	else printf("Element %s not found!\n", two.name);

	if (hashSearch(three.id, demoSize, demo, op) != -1)
	{
		printf("Element %s found!\n", three.name);
	}
	else printf("Element %s not found!\n", three.name);

	if (hashSearch(four.id, demoSize, demo, op) != -1)
	{
		printf("Element %s found!\n", four.name);
	}
	else printf("Element %s not found!\n", four.name);

	if (hashSearch(notF.id, demoSize, demo, op) != -1)
	{
		printf("Element %s found!\n", notF.name);
	}
	else printf("Element %s not found!\n", notF.name);
}

int main()
{
	int op;
	do
	{
		cout << "\n1. DEMO\n2. TASK\n\n Press 0 to exit\n\nOption: ";
		cin >> op;
		if (op == 0)
		{
			return 0;
		}
		else if (op == 1)
		{
			demo();
		}
		else if (op == 2)
		{
			task();
		}
	} while (op != 0 || op != 1 || op != 2);
}