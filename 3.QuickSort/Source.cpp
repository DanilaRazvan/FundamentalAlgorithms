/***********************************************************************************
	We can observe that QuickSort has a smaller multiplicative
	constant than HeapSorton the average case.
	
	It can be concluded that building the heap and constructing the sorted 
	array from the heap is generally more expensive in terms of operations made than 
	using the partition based strategy in quicksort.

	On Worst Case, quicksort picks the maximum or the minimum at 
	each step so there are n^2 comparisons, just like selection sort

	The best case of quicksort is when the pivot is around the average 
	of the values in the array. In this case, the division factor is 2 
	and the algorithm takes maximum advantage of the divide et impera technique

***********************************************************************************/

#include <iostream>
#include "Profiler.h"

#pragma warning(disable: 4996) 

#define max_size 10000

using namespace std;

Profiler pr("demo");

//global variables for counting operations
int A_hs, C_hs;
int A_qs, C_qs;

void init_global_var()
{
	A_hs = 0;
	A_qs = 0;
	C_hs = 0;
	C_qs = 0;
}

void mean_value()
{
	A_hs /= 5;
	A_qs /= 5;
	C_hs /= 5;
	C_qs /= 5;
}

int left(int i)		//left child of i
{
	return 2 * i + 1;
}

int right(int i)	//right child of i
{
	return 2 * i + 2;
}

int parent(int i)	//parent of i
{
	return (i - 1) / 2;
}

void max_heapify(int a[], int i, int len)
{
	int l = left(i);	//left child of i
	int r = right(i);	//right child of i
	int m;	//maximum elem from heap

	C_hs++;	//increment comparisons for heapSort
	if (l < len && a[l] > a[i])
	{
		m = l;
	}
	else m = i;

	C_hs++;	//increment comparisons for heapSort
	if (r < len && a[r] > a[m])
	{
		m = r;
	}

	if (m != i)		//if maximum != root swap the elem and recursively call the affected subtree
	{
		swap(a[i], a[m]);
		A_hs += 3;	//increment assignments for heapSort
		max_heapify(a, m, len);
	}
}

void build_max_heap_bu(int a[], int len)
{
	for (int i = len / 2 - 1; i >= 0; i--)
		max_heapify(a, i, len);
}

void heapSort(int a[], int len)
{
	build_max_heap_bu(a, len);
	for (int i = len - 1; i > 0; i--)
	{
		swap(a[0], a[i]);
		A_hs += 3;		//increment assignmetns for heapSort
		len--;
		max_heapify(a, 0, len);
	}
}

int partition(int a[], int start, int end)
{
	int x = a[end];
	A_qs++;		//increment assignmetns for quickSort
	int index = start - 1;
	for (int i = start; i < end; i++)
	{
		C_qs++;	//increment comparisons for quickSort
		if (a[i] <= x)
		{
			index++;
			swap(a[i], a[index]);
			A_qs += 3;	////increment assignmetns for quickSort
		}
	}
	swap(a[end], a[index + 1]);
	A_qs += 3;	////increment assignmetns for quickSort
	return index + 1;
}

int randpartition(int a[], int start, int end)
{
	int i = rand() % (start - end) + start;
	swap(a[i], a[end]);
	return partition(a, start, end);
}

void quickSort(int a[], int start, int end)
{
	if (start < end)
	{
		int p = partition(a, start, end);
		quickSort(a, start, p - 1);
		quickSort(a, p + 1, end);
	}
}

int randSelect(int a[], int k, int start, int end)
{
	if (start >= end)
	{
		return a[start];
	}

	int p = randpartition(a, start, end);
	int rel_pos = p - start + 1;
	if (rel_pos == k)
	{
		return a[p];
	}
	else if (rel_pos > k)
	{
		return randSelect(a, k, start, p - 1);
	}
	else    //rel_pos < k
	{
		return randSelect(a, k - rel_pos, p + 1, end);
	}
}

int partitionBest(int a[], int start, int end)
{
	int x = a[(end - start) / 2 + start];
	swap(a[(end - start) / 2 + start], a[end]);
	A_qs++;		//increment assignmetns for quickSort
	int index = start - 1;
	for (int i = start; i < end; i++)
	{
		C_qs++;	////increment comparisons for quickSort
		if (a[i] <= x)
		{
			index++;
			swap(a[i], a[index]);
			A_qs += 3;	////increment assignmetns for quickSort
		}
	}
	swap(a[end], a[index + 1]);
	A_qs += 3;	////increment assignmetns for quickSort
	return index + 1;
}

void quickSortBest(int a[], int start, int end)
{
	if (start < end)
	{
		int p = partitionBest(a, start, end);
		quickSortBest(a, start, p - 1);
		quickSortBest(a, p + 1, end);
	}
}

void printArray(int a[], int len)
{
	cout << "\n";
	for (int i = 0; i < len; i++)
		cout << a[i] << " ";
}

void bestCase()	
{
	pr.reset("bestCase");

	int a[max_size], v[max_size];

	FillRandomArray(a, max_size, 10, 50000, true, 1);

	cout << "\n\n";
	for (int x = 100; x < max_size; x += 100)
	{
		init_global_var();

		cout << "\nquick " << x;
		CopyArray(v, a, x);
		quickSortBest(v, 0, x - 1);

		pr.countOperation("assign_QS", x, A_qs);
		pr.countOperation("comp_QS", x, C_qs);
	}
	cout << "\n";

	//obtain total operations
	pr.addSeries("total_QS", "assign_QS", "comp_QS");

	//create graphs
	pr.createGroup("ASSIGN", "assign_QS");
	pr.createGroup("COMP", "comp_QS");
	pr.createGroup("TOTAL", "total_QS");

	//show conclusions
	pr.showReport();
}

void worstCase()
{
	pr.reset("worstCase");

	int a[max_size], v[max_size];

	FillRandomArray(a, max_size, 10, 50000, true, 1);

	cout << "\n\n";
	for (int x = 100; x < max_size; x += 100)
	{
		init_global_var();

		cout << "\nquick " << x;
		CopyArray(v, a, x);
		quickSort(v, 0, x - 1);

		pr.countOperation("assign_QS", x, A_qs);
		pr.countOperation("comp_QS", x, C_qs);
	}
	cout << "\n";

	//obtain total operations
	pr.addSeries("total_QS", "assign_QS", "comp_QS");

	//create graphs
	pr.createGroup("ASSIGN", "assign_QS");
	pr.createGroup("COMP", "comp_QS");
	pr.createGroup("TOTAL", "total_QS");

	//show conclusions
	pr.showReport();
}

void averageCase()
{
	pr.reset("averageCase");

	int a[max_size], v[max_size];

	cout << "\n\n";
	for (int x = 100; x < max_size; x += 100)
	{
		init_global_var();

		// generate 5 random arrays in order to calculate the average (obtain a more precise graphic)
		for (int i = 0; i < 5; i++)
		{
			FillRandomArray(a, x, 10, 50000, true, 0);

			cout << "\nheap " << x;
			CopyArray(v, a, x);
			heapSort(v, x);

			cout << "\nquick " << x;
			CopyArray(v, a, x);
			quickSort(v, 0, x - 1);
		}

		mean_value();

		pr.countOperation("assign_HP", x, A_hs);
		pr.countOperation("comp_HP", x, C_hs);

		pr.countOperation("assign_QS", x, A_qs);
		pr.countOperation("comp_QS", x, C_qs);
	}
	cout << "\n";

	//obtain total operations
	pr.addSeries("total_HS", "assign_HP", "comp_HP");
	pr.addSeries("total_QS", "assign_QS", "comp_QS");

	//create graphs
	pr.createGroup("ASSIGN Average", "assign_HP", "assign_QS");
	pr.createGroup("COMP Average", "comp_HP", "comp_QS");
	pr.createGroup("TOTAL Average", "total_HS", "total_QS");

	//show conclusions
	pr.showReport();
}

void test()
{
	int a[20],v[20];
	int len;
	cout << "Array lenght: ";
	cin >> len;
	cout << "Array:\n";
	for (int i = 0; i < len; i++)
		cin >> a[i];

	CopyArray(v, a, len);
	cout << "\nHEAPSORT";
	heapSort(v, len);
	printArray(v, len);

	CopyArray(v, a, len);
	cout << "\n\nQUICKSORT";
	quickSort(v, 0, len - 1);
	printArray(v, len);

	CopyArray(v, a, len);
	cout << "\n\nQUICKSORT BEST";
	quickSortBest(v, 0, len - 1);
	printArray(v, len);

	cout << "\n\nQUICKSELECT";
	cout << "\nIndex in the sorted array: ";
	int k;
	cin >> k;
	cout << "\nIn the sorted array, on index " << k << " is the element: " << randSelect(a, k, 0, len - 1) << "\n\n";
}

int main()
{
	int d;
	do 
	{
		cout << "\n1.Test\n2.bestCase\n3.worstCase\n4.averageCase\n\nPress 0 to exit\n\nOption: ";
		cin >> d;
		if (d == 0)
		{
			return 0;
		}
		else if (d == 1)
		{
			test();
		}
		else if (d == 2)
		{
			bestCase();
		}
		else if (d == 3)
		{
			worstCase();
		}
		else if (d == 4)
		{
			averageCase();
		}
		else
		{
			cout << "\nPlease introduce a valid option\n";
		}
	} while ((d != 0) || (d != 1) || (d != 2) || (d != 3) || (d != 4));
}