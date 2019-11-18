/******************************************************************************************

1. Best Case - the array is increasingly sorted
	
	- assignments:
			* bubbleSort performs the fewest assignments - 0
			
			* insertionSort performs fewer assignments than selectionSort

	- comparisons:
			* selectionSort performs more comparisons than bubble and insertionSort,
			which do about the same number of comparisons

	- total operations:
			* overall, the most efficient sorting technique is bubbleSort because
			the number of assignments is 0 and it performs the least number of comparisons;
			it is followed by insertionSort and then selectionSort


2. Worst Case - the array is decreasingly sorted
	
	- assignments:
			* selectionSort performs way less assignments in this case, so it is more 
			efficient than the other sorting techniques; it is followed by insertionSort
			and bubbleSort

	- comparisons:
			* bubbleSort is the least efficient sorting technique compared to the other
			two techniques, which perform aproximatively the same number of comparisons

	- total operations:
			* summing all operations for each sorting technique shows that
			the most efficient sorting technique is selectionSort


3. Average Case - the array is unsorted

			* for the average case, insertion and selectionSort do aproximatively
			the same number of operations (insertionSort is more efficient, performing
			about 600 operations less than selectionSort) compared with bubbleSort,
			which does way more operations 
			
			!!! (strong argument for not using bubbleSort anymore =D)  !!!

******************************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Profiler.h"

#define MAX_SIZE 2000

using namespace std;

Profiler profiler("bestCase");


// Global variables for counting total operations for each sorting technique
int bs_cmp, bs_asg;
int is_cmp, is_asg;
int ss_cmp, ss_asg;

void bubbleSort(int a[], int len)
{
	bool sw = true;
	while (sw)		// no swappes are done means the array is sorted
	{
		sw = false;
		for (int j = 0; j < len - 1; j++)
		{
			profiler.countOperation("nb_of_comparisons_bs", len);  bs_cmp++;		// increase nb of comparisons
			if (a[j] > a[j + 1])
			{
				swap(a[j], a[j + 1]);		// 3 assignments
				profiler.countOperation("nb_of_assignments_bs", len, 3);    bs_asg += 3;	// increase with 3 the nb of assignments
				sw = true;
			}
		}
	}
}

void insertionSort(int a[], int len)
{
	for (int i = 1; i < len; i++)
	{
		int key = a[i];
		profiler.countOperation("nb_of_assignments_is", len); is_asg++;		// increase nb of assignments
		int j = i - 1;
		while (j >= 0 && a[j] > key)
		{
			profiler.countOperation("nb_of_comparisons_is", len); is_cmp++;		// increase nb of comparisons (the last one is not counted)
			a[j + 1] = a[j];
			profiler.countOperation("nb_of_assignments_is", len);  is_asg++;	// increase nb of assignments
			j--;
		}
		profiler.countOperation("nb_of_comparisons_is", len); is_cmp++;		// increase nb of comparisons (last comparison from while)

		a[j + 1] = key;
		profiler.countOperation("nb_of_assignments_is", len);  is_asg++;	// increase nb of assignments
	}
}

void selectionSort(int a[], int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		int aux = i;
		for (int j = i + 1; j < len; j++)
		{
			profiler.countOperation("nb_of_comparisons_ss", len);  ss_cmp++;		// increase nb of comparisons
			if (a[j] < a[aux])
				aux = j;
		}
		swap(a[i], a[aux]);		// 3 assignments
		profiler.countOperation("nb_of_assignments_ss", len, 3);  ss_asg += 3;		// increase nb of assignments with 3
	}
}

void test()
{
	int a[MAX_SIZE], v[MAX_SIZE];

beta:
	int len;
	cout << "Introduce array length: ";
	cin >> len;
	

	cout << "\n Array:\n";
	for (int i = 0; i < len; i++)
		cin >> a[i];

	CopyArray(v, a, len);
	cout << "\nTesting bubbleSort...\n";

//	FillRandomArray(a, len, 10, 100, false, 0);

//	cout << "\nArray:\n";
//	for (int i = 0; i < len; i++)
//	{
//		cout << a[i] << " ";
//	}
	bubbleSort(v, len);
	cout << "\nSorted array:\n";
	for (int i = 0; i < len; i++)
	{
		cout << v[i] << " ";
	}

	cout << "\n\nTesting inserionSort...\n";
//	cout << "Introduce array length: ";
//	cin >> len;

//	FillRandomArray(a, len, 10, 100, false, 0);

	CopyArray(v, a, len);
	cout << "\nArray:\n";
	for (int i = 0; i < len; i++)
	{
		cout << v[i] << " ";
	}
	insertionSort(v, len);
	cout << "\nSorted array:\n";
	for (int i = 0; i < len; i++)
	{
		cout << v[i] << " ";
	}

	cout << "\n\nTesting selectionSort...\n";
//	cout << "Introduce array length: ";

//	FillRandomArray(a, len, 10, 100, false, 0);
	CopyArray(v, a, len);
	cout << "\nArray:\n";
	for (int i = 0; i < len; i++)
	{
		cout << v[i] << " ";
	}
	selectionSort(v, len);
	cout << "\nSorted array:\n";
	for (int i = 0; i < len; i++)
	{
		cout << v[i] << " ";
	}

	cout << "\n\nDo you want to test again?  'y' - yes/'n' - no \n";
	char s;
	cin >> s;
	if (s == 'y')
		goto beta;
	else return;
}

void bestCase()		// the array is already sorted
{
	// generate values in increasingly order in array a; work with array v

	int a[MAX_SIZE], v[MAX_SIZE];

	FillRandomArray(a, MAX_SIZE, 10, 50000, true, 1);

	for (int x = 100; x < MAX_SIZE; x += 100)
	{
		CopyArray(v, a, x);
		cout << x << " bubble" << endl;
		bubbleSort(v, x);

		CopyArray(v, a, x);
		cout << x << " insertion" << endl;
		insertionSort(v, x);

		CopyArray(v, a, x);
		cout << x << " selection" << endl;
		selectionSort(v, x);
	}

	// add nb of assignments and nb of comparisons to obtain total nb of operations
	profiler.addSeries("is_op", "nb_of_assignments_is", "nb_of_comparisons_is");
	profiler.addSeries("ss_op", "nb_of_assignments_ss", "nb_of_comparisons_ss");

	// generate graphics
	profiler.createGroup("assign comparison", "nb_of_assignments_is", "nb_of_assignments_ss");
	profiler.createGroup("comp comparison", "nb_of_comparisons_bs", "nb_of_comparisons_is", "nb_of_comparisons_ss");
	profiler.createGroup("total op comparison", "nb_of_comparisons_bs", "is_op", "ss_op");

	profiler.createGroup("total op comparison bs is", "nb_of_comparisons_bs", "is_op");

	// show results
	profiler.showReport();
}

void worstCase()		// the array is reverse sorted
{
	// generate values in decreasingly order in array a; work with array v

	int a[MAX_SIZE], v[MAX_SIZE];

	profiler.reset("worstCase");

	FillRandomArray(a, MAX_SIZE, 10, 50000, true, 2);

	for (int x = 100; x < MAX_SIZE; x += 100)
	{
		CopyArray(v, a, x);
		cout << x << " bubble" << endl;
		bubbleSort(v, x);

		CopyArray(v, a, x);
		cout << x << " insertion" << endl;
		insertionSort(v, x);

		CopyArray(v, a, x);
		cout << x << " selection" << endl;
		selectionSort(v, x);
	}

	// obtain total nb of operations
	profiler.addSeries("bs_op", "nb_of_assignments_bs", "nb_of_comparisons_bs");
	profiler.addSeries("is_op", "nb_of_assignments_is", "nb_of_comparisons_is");
	profiler.addSeries("ss_op", "nb_of_assignments_ss", "nb_of_comparisons_ss");

	// generate graphics
	profiler.createGroup("assign comparison", "nb_of_assignments_bs", "nb_of_assignments_is", "nb_of_assignments_ss");
	profiler.createGroup("comp comparison", "nb_of_comparisons_bs", "nb_of_comparisons_is", "nb_of_comparisons_ss");
	profiler.createGroup("total op comparison", "bs_op", "is_op", "ss_op");

	profiler.createGroup("comp comparison bs is", "nb_of_comparisons_bs", "nb_of_comparisons_is");
	profiler.createGroup("nb_of_assignments_ss", "nb_of_assignments_ss");
	// show results
	profiler.showReport();
}

void averageCase()			// the array contains randomly generated numbers
{
	// generate random values in array a; work with array v
	int a[MAX_SIZE], v[MAX_SIZE];

	profiler.reset("averageCase");

	for (int x = 100; x < MAX_SIZE; x += 100)
	{
		// global variables for counting the operations done by each sorting technique
		bs_cmp = 0;
		bs_asg = 0;

		is_cmp = 0;
		is_asg = 0;

		ss_cmp = 0;
		ss_asg = 0;

		int total_bs = 0;
		int total_is = 0;
		int total_ss = 0;

		// generate 5 random arrays in order to calculate the average (obtain a more precise graphic)
		for (int i = 0; i < 5; i++)
		{
			FillRandomArray(a, x, 10, 50000, true, 0);

			CopyArray(v, a, x);
			cout << x << " bubble" << endl;
			bubbleSort(v, x);

			CopyArray(v, a, x);
			cout << x << " insertion" << endl;
			insertionSort(v, x);

			CopyArray(v, a, x);
			cout << x << " selection" << endl;
			selectionSort(v, x);
		}

		// calculate mean value
		bs_cmp /= 5;
		bs_asg /= 5;

		is_cmp /= 5;
		is_asg /= 5;

		ss_cmp /= 5;
		ss_asg /= 5;

		//	total operations
		total_bs += bs_cmp + bs_asg;
		total_is += is_cmp + is_asg;
		total_ss += ss_cmp + ss_asg;

		// associate an operation for each variable in ordear to create the graphics
		profiler.countOperation("bs_comp", x, bs_cmp);
		profiler.countOperation("bs_assign", x, bs_asg);
		profiler.countOperation("bs_total", x, total_bs);

		profiler.countOperation("is_comp", x, is_cmp);
		profiler.countOperation("is_assign", x, is_asg);
		profiler.countOperation("is_total", x, total_is);

		profiler.countOperation("ss_comp", x, ss_cmp);
		profiler.countOperation("ss_assign", x, ss_asg);
		profiler.countOperation("ss_total", x, total_ss);
	}

	// generate graphics
	profiler.createGroup("assign comparison", "bs_assign", "is_assign", "ss_assign");
	profiler.createGroup("comp comparison", "bs_comp", "is_comp", "ss_comp");
	profiler.createGroup("total op comparison", "bs_total", "is_total", "ss_total");

	profiler.divideValues("nb_of_assignments_bs", 5);
	profiler.divideValues("nb_of_assignments_is", 5);
	profiler.divideValues("nb_of_assignments_ss", 5);
	profiler.divideValues("nb_of_comparisons_bs", 5);
	profiler.divideValues("nb_of_comparisons_is", 5);
	profiler.divideValues("nb_of_comparisons_ss", 5);

	// show results
	profiler.showReport();
}

int main()
{
	int cmd;

alpha:
	cout << "\n1. Average Case\n2. Best Case\n3. Worst Case\n4. Test\n\nOption: ";
	cin >> cmd;
	switch (cmd)
	{
		case 1:
		{
			averageCase();
			break;
		}
		
		case 2:
		{
			bestCase();
			break;
		}
		
		case 3:
		{
			worstCase();
			break;
		}

		case 4:
		{
			test();
			break;
		}

		default:
		{
			cout << "\nplease enter a valid command.\n";
			goto alpha;
		}
	}

	return 0;
}
