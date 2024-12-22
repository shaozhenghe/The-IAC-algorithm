#include <io.h>
#include <iostream>
#define MAX_PATH 1000
#define N (1 << 16) + 100
using namespace std;
int n = 16;
int Length_of_gurobi_output;
int walsh1[N], approximate_f[N], f2[N], walsh2[N], temp_1_walsh[N], temp_2_walsh[N], pos_in_P[N], cnt_of_P, pos_need_enumeration[10000], vispos_in_P[10000];
int cntyan2, cntlun;
int da = (1 << n), Length_part_walsh;

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
void f_to_walsh(int f[], int walsh[]) // Change truth table to Walsh spectrum
{
	FFT_f_to_Walsh(f, walsh, n);
}

void output_real_solutions(FILE *fp, int f[])
{
	for (int i = 0; i < (1 << n); i++)
	{
		fprintf(fp, "%d", f[i]);
	}
	fprintf(fp, "\n");
}

int varification_by_FWT(int approximate_f[], int Length_part_walsh, int num_for_varification)
{

	int a, b;
	int cnt = 0;

	f_to_walsh(approximate_f, walsh2);
	for (int i = 1; i <= Length_part_walsh; i++)
	{
		a = temp_2_walsh[i];
		b = temp_1_walsh[a];
		if (walsh2[a] == b)
		{

			cnt++;
		}
	}
	//cout<<cnt<<" "<<Length_part_walsh<<endl;
	if (cnt != Length_part_walsh)
		return 0;
	return 1;
}
int weight(int n, int a[])
{
	int cnt_weight = 0; // i.e. weight
	int cnt = 0;
	int temp;
	while (n != 0)
	{
		temp = n % 2;

		if (temp == 1)
		{

			a[cnt_weight] = pos_in_P[cnt];
			vispos_in_P[cnt_weight] = cnt;
			cnt_weight++;
		}
		cnt++;
		n = n / 2;
	}
	return (cnt_weight);
}
void init()
{
	char Initial_path[MAX_PATH];
	getcwd(Initial_path, MAX_PATH);
	FILE *part_Walsh_spectrum_file = fopen(strcat(Initial_path, "\\Problem\\Challenge\\16\\16.1.txt"), "r");
	getcwd(Initial_path, MAX_PATH); // Reset Variables
	FILE *P_file = fopen(strcat(Initial_path, "\\Intermediate_parameters\\Challenge\\16.1\\Set_P.txt"), "r");
	getcwd(Initial_path, MAX_PATH); // Reset Variables
	FILE *gurobi_output = fopen(strcat(Initial_path, "\\Intermediate_parameters\\Challenge\\16.1\\16.1_output.txt"), "r");
	getcwd(Initial_path, MAX_PATH); // Reset Variables
	FILE *solutions_file = fopen(strcat(Initial_path, "\\Results\\Challenge\\16.1\\Final_solutions.txt"), "w");
	getcwd(Initial_path, MAX_PATH); // Reset Variables
	FILE *One_approximate_solution_file = fopen(strcat(Initial_path, "\\Intermediate_parameters\\Challenge\\16.1\\One_approximate_solution.txt"), "r");
	fscanf(part_Walsh_spectrum_file, "%d", &Length_part_walsh);
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

	int varification_pos, alpha, res, zhi;
	int num_for_varification = 0;
	int num_of_real_solutions = 0;
	fscanf(gurobi_output, "%d", &Length_of_gurobi_output);
	cout<<"==============="<<Length_of_gurobi_output<<"iters in total===============\n";

	for (int i = 0; i < Length_of_gurobi_output; i++)
	{
		num_for_varification = 0;
		fscanf(gurobi_output, "%c", &c);   // Throw the "\n" for each row
		//cout<<"c:"<<c<<endl;
		for (int j = 0; j < cnt_of_P; j++) // For each line in the output of Gurobi, i.e. one approximate solution that pass the necessary condition
		{
			fscanf(gurobi_output, "%c", &c);

			vispos_in_P[j] = int(c - '0');

			if (vispos_in_P[j] == 1)
			{

				pos_need_enumeration[num_for_varification] = pos_in_P[j];
				num_for_varification++;
			}
		}
		for (int j = 0; j < num_for_varification; j++)
		{
			varification_pos = pos_need_enumeration[j];
			approximate_f[varification_pos] = 1 - approximate_f[varification_pos];
		}
		int varification_result = varification_by_FWT(approximate_f, Length_part_walsh, num_for_varification);
		if (i % 100 == 0)
			printf("The %d iter\n", i);
		if (varification_result == 1)
		{
			output_real_solutions(solutions_file, approximate_f);
			num_of_real_solutions++;
			printf("The %dth line finished.Find %d solutions in total.\n", i, num_of_real_solutions);
		}
		for (int j = 0; j < num_for_varification; j++) // change the approximate solution back
		{
			varification_pos = pos_need_enumeration[j];
			approximate_f[varification_pos] = 1 - approximate_f[varification_pos];
		}
	}
}

int main()
{

	init();
}
