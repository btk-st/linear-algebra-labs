/*
	Author: Volkov Andrey IVT12BO	
	All 3 methods for calculating the determinant are implemented.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
long int min(long int x, long int y)
{
	return (x < y) ? x : y;
}
long int gcd(long int x, long int y)
{
	while (x != 0 && y != 0)
	{
		if (x > y)
			x %= y;
		else
			y %= x;
	}
	return x + y;
}
long int lcm(long int x, long int y) //check zero before run!
{
	return x*y / gcd(x,y);
}
void swap_str(long int *a, int n, int shift, int i)
{
	int tmp;
	for (int j = shift; j < n; j++)
	{
		tmp = *(a + shift * n + j);
		*(a + shift * n + j) = *(a + i * n + j);
		*(a + i * n + j) = tmp;
	}
}
void swap(long int *x, long int *y)
{
	int tmp = *x;
	*x = *y;
	*y = tmp;
}
long int abs(long int x)
{
	return (x > 0) ? x : -x;
}
void printMatr(long int *a, int n)
{
	//printf("\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			printf("%2d ", *(a + i * n + j));
		printf("\n");
	}
	//printf("\n");

}
int maxZerosInRows(long int *a, int n, int *index) //return max zeros count in rows and changes *index
{
	int maxZ = -1,
		curZ,
		i, j;
	*index = 0;
	for (i = 0; i < n; i++)
	{
		curZ = 0;
		for (j = 0; j < n; j++)
			if (*(a + i * n + j) == 0)
				curZ++;
		if (curZ > maxZ)
		{
			*index = i;
			maxZ = curZ;
		}
	}
	return maxZ;
}
int maxZerosInColumns(long int *a, int n, int *index) //return max zeros count in columns and changes *index
{
	int maxZ = -1,
		curZ,
		i, j;
	*index = 0;
	for (j = 0; j < n; j++)
	{
		curZ = 0;
		for (i = 0; i < n; i++)
			if (*(a + i * n + j) == 0)
				curZ++;
		if (curZ > maxZ)
		{
			*index = j;
			maxZ = curZ;
		}
	}
	return maxZ;
}
void del_IRowJCol(long int *new_a, long int *a, int n, int i_del, int j_del) //delete i row and j column
{
	int i, j, p = 0;
	for (i = 0; i < n; i++)
		if (i != i_del)
			for (j = 0; j < n; j++)
				if (j != j_del)
					*(new_a + p++) = *(a + i * n + j);
}
long int calcDetLaplas(long int *old_a, int n, int firstLaunch)
{
	long int *a;
	long int det = 0;
	int rowWithMaxZeros,
		columnWithMaxZeros;
	if (firstLaunch)
		printf("\nLaplas:");
	if (n == 1)
		return *old_a;
	a = (long int*)malloc((n - 1)*(n - 1) * sizeof(long int));
	if (maxZerosInRows(old_a, n, &rowWithMaxZeros) >
		maxZerosInColumns(old_a, n, &columnWithMaxZeros))
	{
		if (firstLaunch)
		{
			printf("\nDecomposition of the determinant by %d row:", rowWithMaxZeros + 1);
		}
		for (int j = 0; j < n; j++)
			if (*(old_a + rowWithMaxZeros * n + j) != 0)
			{
				long int el, tempDet;
				el = *(old_a + rowWithMaxZeros * n + j) * (((rowWithMaxZeros + j) % 2 == 0) ? 1 : -1);
				del_IRowJCol(a, old_a, n, rowWithMaxZeros, j);
				tempDet = calcDetLaplas(a, n - 1, 0); // complementary minor
				det += tempDet * el;
				if (firstLaunch)
				{
					printf("\n%d\n", el);
					printMatr(a, n - 1);
					if (tempDet < 0)
						printf("= %d*(%d) = %d", el, tempDet, el*tempDet);
					else
						printf("= %d*%d = %d", el, tempDet, el*tempDet);
				}
			}
	}
	else
	{
		if (firstLaunch)
			printf("\nDecomposition of the determinant by %d column:", columnWithMaxZeros + 1);
		for (int i = 0; i < n; i++)
			if (*(old_a + i * n + columnWithMaxZeros) != 0)
			{
				long int el, tempDet;
				el = *(old_a + i * n + columnWithMaxZeros) * (((i + columnWithMaxZeros) % 2 == 0) ? 1 : -1);
				del_IRowJCol(a, old_a, n, i, columnWithMaxZeros);
				tempDet = calcDetLaplas(a, n - 1, 0); // complementary minor
				det += tempDet * el;
				if (firstLaunch)
				{
					printf("\n%d\n", el);
					printMatr(a, n - 1);
					if (tempDet < 0)
						printf("= %d*(%d) = %d", el, tempDet, el*tempDet);
					else
						printf("= %d*%d = %d", el, tempDet, el*tempDet);
				}
			}
	}
	if (firstLaunch)
		printf("\nDeterminant of the matrix by a recursive formula = %d\n", det);
	free(a);
	return det;
}
long int calcDetLeibniz(long int *a, int n)
{
	//johnson-trotter algorithm was used to generate permutations
	int *vector = (int*)malloc(n * sizeof(int)),
		*permutationArray = (int*)malloc(n * sizeof(int));
	int //i,
		f = 1, //for first output
		//tmp,
		sign = 0; //changes every iteration, 0 - '+', 1 - '-'
	long int
		res = 0,
		tmpRes;


	//init
	for (int i = 0; i < n; i++)
	{
		vector[i] = -1;
		permutationArray[i] = i;
	}

	printf("\nDeterminant of the matrix by the combinatorial formula: \n");
	while (1)
	{

		int id = -1;

		if (f == 0)
			printf(" %c ", (sign == 0) ? '+' : '-');
		else
			f = 0;

		tmpRes = *(a + permutationArray[0]); //1st row element

		if (tmpRes > 0)
			printf("%d", tmpRes);
		else
			printf("(%d)", tmpRes);

		//calc next multiplication
		for (int i = 1; i < n; i++)
		{
			int el = *(a + i * n + permutationArray[i]);
			if (el >= 0)
				printf("*%d", el);
			else
				printf("*(%d)", el);
			tmpRes *= el;
		}
		if (sign == 1)
			tmpRes = -tmpRes;
		res += tmpRes;

		//find new id
		for (int i = 0; i < n; i++)
			if ((((0 <= i + vector[i]) && (i + vector[i] < n))
				&& (permutationArray[i] > permutationArray[i + vector[i]]))
				&& ((id == -1) || (permutationArray[i] > permutationArray[id])))
				id = i;

		if (id == -1)
			break;

		//switch sign
		for (int i = 0; i < n; i++)
			if (permutationArray[i] > permutationArray[id])
				vector[i] = -vector[i];

		swap(&permutationArray[id], &permutationArray[id + vector[id]]);
		swap(&vector[id], &vector[id + vector[id]]);

		sign = 1 - sign;
	}
	printf(" = %d\n", res);
	free(vector);
	free(permutationArray);
	return res;
}
long int calcDetGauss(long int *a_source, int n)
{
	long int inverse_det = 1,
			 det = 1,
			 *a;
	//make copy of source matrix
	a = (long int*)malloc(n*n * sizeof(long int));
	for (int i = 0; i < n*n; i++)
		*(a + i) = *(a_source + i);

	printf("\nDeterminant by using Gauss method:\n");
	for (int shift = 0; shift < n - 1; shift++)
	{
		long int multiplier;
		int min_str = shift;

		printMatr(a, n);

		//str with min el to the top
		for (int i = shift+1; i < n; i++)
		{
			if (*(a + min_str * n + shift) == 0 ||
				(*(a + i * n + shift) != 0) && (abs(*(a + i * n + shift)) < abs(*(a + min_str * n + shift))))
				min_str = i;
		}
		swap_str(a, n, shift, min_str);

		//debug info
		if (min_str != shift)
		{
			printf("\nChanged %d row to the %d row, determinant changed sign,\n", shift + 1, min_str + 1);
			inverse_det = -inverse_det;
			printf("Now matrix =\n");
			printMatr(a, n);
		}

		//if top el == 0
		if (*(a + shift * n + shift) == 0)
			return 0;

		//first row element --> 0 
		for (int i = shift+1; i < n; i++)
		{
			if (*(a + i * n + shift) != 0)
			{
				//calc multiplier
				multiplier = lcm(abs(*(a + shift * n + shift)), abs(*(a + i * n + shift))) / abs(*(a + i * n + shift));
				if (*(a + shift * n + shift) * (*(a + i * n + shift)) < 0)
					multiplier = -multiplier;
				//debug info
				if (multiplier != 1)
				{
					printf("Multiply %d row by %d,\n", i+1, multiplier);
					inverse_det *= multiplier;
				}
				//multiply row elements
				for (int j = shift; j < n; j++)
					*(a + i * n + j) *= multiplier;
				//debug info
				multiplier = *(a + i * n + shift) / (*(a + shift * n + shift));
				printf("(%d) - %d*(%d),\n", i + 1, multiplier, shift + 1);
				//element by element subtraction
				for (int j = shift; j < n; j++)
					*(a + i * n + j) -= *(a + shift * n + j) * multiplier;
			}
		}
		printf("\n");
	}
	printMatr(a, n);

	//debug output
	printf("Determinant of the matrix by Gauss method: %d", *(a));
	for (int i = 1; i < n; i++)
	{
		int el = *(a + i * n + i);
		if (el > 0)
			printf("*%d", el);
		else
			printf("*(%d)", el);
		det *= el;
	}
	printf("/(%d) = %d", inverse_det, det/inverse_det);

	free(a);
	return det;
}
int main(int argc, char **argv)
{
	int N, i, j;
	long int *a;

	FILE *f_inp;
	f_inp = fopen(argv[1], "rt");
	fscanf(f_inp, "%d", &N);
	a = (long int*)malloc(N*N * sizeof(long int));
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			fscanf(f_inp, "%d", (a + i * N + j));
	fclose(f_inp);

	printMatr(a, N);

	calcDetLaplas(a, N, 1);

	calcDetLeibniz(a, N);
	
	calcDetGauss(a, N);

	//system("pause");
	return 0;
}