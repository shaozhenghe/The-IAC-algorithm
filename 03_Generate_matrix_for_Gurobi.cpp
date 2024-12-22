
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <gmpxx.h>
#define MAX_PATH 1000
#define N (1 << 16) + 100
using namespace std;
int n = 16;
int temp_1_S1[N], temp_2_S1[N], cnt_of_S1;
int walsh1[N], approximate_f[N], f2[N], walsh2[N], temp_1_walsh[N], temp_2_walsh[N], pos_in_P[N], cnt_of_P, Hnf[N][1000], vispos_in_P[1000];
int Length_part_walsh;
void FFT_Walsh_to_f(int *f, int *walsh, int nn) // Change Walsh spectrum to simple table with FWT
{
	// for(int i=0; i<(1<<nn); i++) f[i] = 1-2*f[i];
	for (int i = 0; i < (1 << nn); i++)
		f[i] = walsh[i];

	for (int i = 1; i < nn + 1; i++)
	{
		for (int h = 0; h < (1 << (nn - i)); h++)
		{
			for (int j = 0; j < (1 << (i - 1)); j++)
			{
				int t1 = f[j + h * (1 << i)] + f[j + (1 << (i - 1)) + h * (1 << i)];
				int t2 = f[j + h * (1 << i)] - f[j + (1 << (i - 1)) + h * (1 << i)];
				f[j + h * (1 << i)] = t1;
				f[j + (1 << (i - 1)) + h * (1 << i)] = t2;
			}
		}
	}
}
void FFT_f_to_Walsh(int f[], int walsh[], int nn) // Change truth table to Walsh spectrum with FWT
{
	for (int i = 0; i < (1 << nn); i++)
		walsh[i] = f[i];
	for (int i = 0; i < (1 << nn); i++)
		walsh[i] = 1 - 2 * walsh[i];
	for (int i = 1; i < nn + 1; i++)
	{
		for (int h = 0; h < (1 << (nn - i)); h++)
		{
			for (int j = 0; j < (1 << (i - 1)); j++)
			{
				int t1 = walsh[j + h * (1 << i)] + walsh[j + (1 << (i - 1)) + h * (1 << i)];
				int t2 = walsh[j + h * (1 << i)] - walsh[j + (1 << (i - 1)) + h * (1 << i)];
				walsh[j + h * (1 << i)] = t1;
				walsh[j + (1 << (i - 1)) + h * (1 << i)] = t2;
			}
		}
	}
}
void walsh_to_f(int f[], int walsh[]) // Change Walsh spectrum to truth table and use sign funciton
{
	FFT_Walsh_to_f(f, walsh, n);
	float b;
	int d1, d_1;
	int c;
	for (int i = 0; i < (1 << n); i++)
	{
		// sign function
		b = f[i] * 1.0 / (1 << n);
		d1 = abs(b - 1);
		d_1 = abs(b + 1);
		if (d1 < d_1)
			c = 0;
		else
			c = 1;
		if (abs(b) < 0.3)
		{
			int t = rand() % 3;
			if (t == 0)
				c = 1 - c;
		}
		f[i] = c;
	}
}
void f_to_walsh(int f[], int walsh[]) // Change truth table to Walsh spectrum
{
	FFT_f_to_Walsh(f, walsh, n);
}

int oneNumInBinary(int n) // count the number of 1 in the Binary of n
{
	int cnt = 0;
	while (n)
	{
		n = n & (n - 1);
		cnt++;
	}
	return cnt;
}

void init()
{
	char Initial_path[MAX_PATH];
	getcwd(Initial_path, MAX_PATH);
	FILE *part_Walsh_spectrum_file = fopen(strcat(Initial_path, "/Problem/Challenge/16/16.1.txt"), "r");
	getcwd(Initial_path, MAX_PATH); // Reset Variables
	FILE *P_file = fopen(strcat(Initial_path, "/Intermediate_parameters/Challenge/16.1/Set_P.txt"), "r");
	getcwd(Initial_path, MAX_PATH); // Reset Variables
	FILE *One_approximate_solution_file = fopen(strcat(Initial_path, "/Intermediate_parameters/Challenge/16.1/One_approximate_solution.txt"), "r");
	int m, Length_part_walsh;
	getcwd(Initial_path, MAX_PATH);//Reset Variables
	FILE *S1_file=fopen(strcat(Initial_path,"/Intermediate_parameters/Challenge/16.1/16.1_S1.txt"), "r");
	getcwd(Initial_path, MAX_PATH);//Reset Variables
	FILE *Hnf_S1_P1_file = fopen(strcat(Initial_path,"/Intermediate_parameters/Challenge/16.1/16.1A.txt"), "w");
	fscanf(part_Walsh_spectrum_file, "%d", &Length_part_walsh);
	//printf("%d",Length_part_walsh) ;
	int a, b;
	char c;
	memset(walsh1, 0, sizeof(walsh1));
	memset(approximate_f, 0, sizeof(approximate_f));
	memset(walsh2, 0, sizeof(walsh2));
	memset(f2, 0, sizeof(f2));

	for (int i = 1; i <= Length_part_walsh; i++)
	{
		fscanf(part_Walsh_spectrum_file, "%d,%d", &a, &b);
		walsh1[a] = b;
		temp_1_walsh[a] = b;
		temp_2_walsh[i] = a;
	}
	for (int i = 0; i < (1 << n); i++)
	{
		fscanf(One_approximate_solution_file, "%c", &c);
		approximate_f[i] = int(c - '0');
	}
	fscanf(P_file, "%d", &cnt_of_P);
	for (int i = 0; i < cnt_of_P; i++)
	{
		fscanf(P_file, "%d", &a);
		pos_in_P[i] = a;
	}
	fscanf(S1_file, "%d", &cnt_of_S1);
	for (int i = 1; i <= cnt_of_S1; i++)
	{
		fscanf(S1_file, "%d %d", &a, &b);
		temp_1_S1[a] = b;
		temp_2_S1[i] = a;
	}
	int column, row, res, value_at_one_pos;
	for (int i = 1; i <= cnt_of_S1; i++)
	{
		row = temp_2_S1[i];
		for (int j = 0; j < cnt_of_P; j++) // Print the submatrix of Hnf.We use -2Hnf actually.It is the same as Hnf
		{
			column = pos_in_P[j];
			res = -2;
			value_at_one_pos = oneNumInBinary(column & row);
			value_at_one_pos += approximate_f[column];
			if (value_at_one_pos % 2 == 1)
				res = 2;
			Hnf[i][j] = res;
			fprintf(Hnf_S1_P1_file, "%d ", Hnf[i][j]);
		}
		fprintf(Hnf_S1_P1_file, "\n");
	}
	cout<<"Hnf ready"<<endl;
}

int main()
{

	init();
}
