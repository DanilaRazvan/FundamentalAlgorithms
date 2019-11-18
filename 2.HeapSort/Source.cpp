/*********************************************************************************

WORST CASE:
	-	for all 3 comparisons (assignments, comparisons and total operations)
		the graphs show that the bottom-up heap implementation is more
		efficient than the top-down implementation (the difference between
		bu implementation and td implementation is a big one)

AVERAGE CASE:
	-	the conclusion is aproximatively tha same as in the wors with the
		observation that in this case the difference between bu implementation 
		operations and td implementation operations is smaller than in worst case

*********************************************************************************/

#include <iostream>
#include <fstream>
#include <limits.h>
#include "Profiler.h"

#define max_size 10000

using namespace std;

Profiler pr("demo");

int assign_bu, comp_bu;
int assign_td, comp_td;

void init_global_var()
{
	assign_bu = 0;
	assign_td = 0;
	comp_bu = 0;
	comp_td = 0;
}

void mean_value()
{
	assign_bu /= 5;
	assign_td /= 5;
	comp_bu /= 5;
	comp_td /= 5;
}

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
	return (i - 1) / 2;
}

void max_heapify(int a[], int i, int len)
{
	int l = left(i);	//left child of i
	int r = right(i);	//right child of i
	int m;	//maximum elem from heap
	
	comp_bu++;		//increase comparisons for bu implementation
	if (l < len && a[l] > a[i])
	{	
		m = l;
	}
	else m = i;

	comp_bu++;		//increase comparisons for bu implementation
	if (r < len && a[r] > a[m])
	{
		m = r;
	}

	if (m != i)		//if maximum != root swap the elem and recursively call the affected subtree
	{
		swap(a[i], a[m]); 
		assign_bu += 3;		//increase assignments for bu implementation
		max_heapify(a, m, len);
	}
}


void build_max_heap_bu(int a[], int len)
{
	for (int i = len / 2 - 1; i >= 0; i--)
		max_heapify(a, i, len);
}

void heap_increase_key(int a[], int i, int key)
{
	comp_td++;		//increase comparisons for td implementation
	if (key < a[i])
	{
		perror("key smaller than current key");
		exit(2);
	}

	assign_td++;		//increase comparisons for td implementation
	a[i] = key;			//assign the value to newest element
	while (i > 0 && a[parent(i)] < a[i])	//maintain the heap properly
	{
		comp_td++;		//increase comparisons for td implementation
		swap(a[i], a[parent(i)]);
		assign_td += 3;		//increase assignments for td implementation
		i = parent(i);
	}
	comp_td++;
}

void max_heap_insert(int a[], int key, int *heap_size)
{
	(*heap_size)++;		//increase heap size
	a[*heap_size] = INT_MIN;	//prepare new element
	assign_td++;	//increase assignments for td implementation
	heap_increase_key(a, *heap_size, key);
}

void build_max_heap_td(int a[], int len)
{
	int heap_size = 0;
	for (int i = 1; i < len; i++)
	{
		max_heap_insert(a, a[i], &heap_size);
	}
}

void worstCase()
{
	pr.reset("worstCase");
	int a[max_size], v[max_size];

	FillRandomArray(a, max_size, 10, 50000, true, 1);
	for (int x = 100; x < max_size; x += 100)
	{
		init_global_var();

		cout << "\nbu " << x;
		CopyArray(v, a, x);
		build_max_heap_bu(v, x);

		cout << "\ntd " << x;
		CopyArray(v, a, x);
		build_max_heap_td(v, x);

		pr.countOperation("assign_bu", x, assign_bu);
		pr.countOperation("assign_td", x, assign_td);

		pr.countOperation("comp_bu", x, comp_bu);
		pr.countOperation("comp_td", x, comp_td);
	}

	pr.addSeries("total_bu", "assign_bu", "comp_bu");
	pr.addSeries("total_td", "assign_td", "comp_td");

	pr.createGroup("Assign_comparison", "assign_bu", "assign_td");
	pr.createGroup("Comp_comparison", "comp_bu", "comp_td");
	pr.createGroup("Total_comparison", "total_bu", "total_td");

	pr.showReport();
}

void averageCase()
{
	pr.reset("averageCase");

	int a[max_size], v[max_size];

	for (int x = 100; x < max_size; x += 100)
	{
		init_global_var();

		// generate 5 random arrays in order to calculate the average (obtain a more precise graphic)
		for (int i = 0; i < 5; i++)
		{
			FillRandomArray(a, x, 10, 50000, true, 0);

			cout << "\nbu " << x << " " << i;
			CopyArray(v, a, x);
			build_max_heap_bu(v, x);
			
			cout << "\ntd " << x << " " << i;
			CopyArray(v, a, x);
			build_max_heap_td(v, x);
		}

		mean_value();

		pr.countOperation("assign_bu", x, assign_bu);
		pr.countOperation("assign_td", x, assign_td);

		pr.countOperation("comp_bu", x, comp_bu);
		pr.countOperation("comp_td", x, comp_td);
	}

	pr.addSeries("total_bu", "assign_bu", "comp_bu");
	pr.addSeries("total_td", "assign_td", "comp_td");

	pr.createGroup("Assign_comparison", "assign_bu", "assign_td");
	pr.createGroup("Comp_comparison", "comp_bu", "comp_td");
	pr.createGroup("Total_comparison", "total_bu", "total_td");

	pr.showReport();
}

void test()
{
	int a[30], v[30];
	cout << "\nArray lenght: ";
	int len;
	cin >> len;
	
	cout << "Array:\n";
	for (int i = 0; i < len; i++)
	{
		cin >> a[i];
	}
	CopyArray(v, a, len);
	build_max_heap_bu(v, len);
	cout << "\nHeap bu:\n";
	for (int i = 0; i < len; i++)
		cout << v[i] << " ";

	CopyArray(v, a, len);
	build_max_heap_td(v, len);
	cout << "\n\nHeap td:\n";
	for (int i = 0; i < len; i++)
		cout << v[i] << " ";
	cout << "\n\n";
}

int main()
{
	int d;

	do
	{
		cout << "\n1.Test\n2.Average Case\n3.Worst Case\nPress 0 for exit\nYour decision: ";
		cin >> d;
		
		if (d == 1)
		{
			test();
		}
		else if (d == 2)
		{
			pr.reset("averageCase");
			averageCase();
		}
		else if (d == 3)
		{
			pr.reset("worstCase");
			worstCase();
		}
		else if (d == 0)
		{
			return 0;
		}
		else
		{
			cout << "Please introduce a valid option.\n";
		}
	} while ((d != 1) || (d != 2) || (d != 3));
}