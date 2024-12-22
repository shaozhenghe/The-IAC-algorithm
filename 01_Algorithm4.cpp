#include <bits/stdc++.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <gmpxx.h>
#define MAX_PATH 1000
#define N (1 << 16) + 100
#define iternum 1000 // iteration rounds
using namespace std;
int n = 16, vis[N]={0};
int walsh[N]={0}, f[N]={0}, f0[N]={0}, temp_1[N]={0}/*spectrum value of S*/ , temp_2[N]={0}/*spectrum position of S*/, visf[N]={0}, P[N]={0},P_set[N]={0};  //P_set is the positions and P is a temp vairble to find P_set. 


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

void output_real_solutions(FILE *fp, int f[])
{
	for (int i = 0; i < (1 << n); i++)
	{
		fprintf(fp, "%d", f[i]);
	}
	fprintf(fp, "\n");
}

void init()
{
	char Initial_path[MAX_PATH];
    getcwd(Initial_path, MAX_PATH);
	FILE *part_Walsh_spectrum_file = fopen(strcat(Initial_path,"/Problem/Challenge/16/16.1.txt"), "r");
	getcwd(Initial_path, MAX_PATH);//Reset Variables
	FILE *P_file = fopen(strcat(Initial_path,"/Intermediate_parameters/Challenge/16.1/Set_P.txt"), "w");
	getcwd(Initial_path, MAX_PATH); // Reset Variables
	FILE *solutions_file = fopen(strcat(Initial_path, "/Results/Challenge/16.1/Final_solutions.txt"), "w");
	getcwd(Initial_path, MAX_PATH);//Reset Variables
	FILE *One_approximate_solution_file = fopen(strcat(Initial_path,"/Intermediate_parameters/Challenge/16.1/One_approximate_solution.txt"),"w");
	int m, Length_part_walsh;
	fscanf(part_Walsh_spectrum_file, "%d", &Length_part_walsh);
	//printf("test=%d",Length_part_walsh);
	int a, b;
	mpz_t square_sum_of_difference;	
	mpz_t cnt0;//lower bound of approximate solutions
	mpz_t const0; // 0
	mpz_init(cnt0);
	mpz_init(square_sum_of_difference);
	mpz_init(const0);
	mpz_set_ui(cnt0,(1<<n));    // You can reset cnt0 according to the result of |P|
	memset(walsh, 0, sizeof(walsh));
	memset(f, 0, sizeof(f));
	memset(f0, 0, sizeof(f0)); // f0 is the first approximate solution
	for (int i = 1; i <= Length_part_walsh; i++)
	{
		fscanf(part_Walsh_spectrum_file, "%d,%d", &a, &b);

		walsh[a] = b;
		temp_1[a] = b;
		temp_2[i] = a;
	}

	gmp_printf("cnt0:%Zd\n",cnt0);

	walsh_to_f(f, walsh);
	f_to_walsh(f, walsh);

	int location = 0;
	int cnt_of_P=0;

	int flag = 0;

	for (int t = 1; t <= 10; t++) // use different t to get enough approximate solutions, you can change the range as you need
	{
		memset(walsh, 0, sizeof(walsh));
		int difference=0;
		for (int iter = 1; iter <= iternum; iter++)
		{
			for (int j = 1; j <= int(Length_part_walsh / t); j++)
			{
				int abadon = rand() % (1 << n); // forgetting strategy
				walsh[abadon] = 0;
			}
			for (int j = 1; j <= Length_part_walsh; j++) // correct with the part-Walsh spectrum
			{
				location = temp_2[j];
				walsh[location] = temp_1[location];
			}
			walsh_to_f(f, walsh);
			f_to_walsh(f, walsh);
			if (iter % 1 == 0) // Always True
			{
				mpz_set_ui(square_sum_of_difference,0); // square sum of correct points in the approximate solution
				for (int j = 1; j <= Length_part_walsh; j++)
				{
					location = temp_2[j];
					difference=walsh[location] - temp_1[location]; //One points satisfying S is found
					mpz_add_ui(square_sum_of_difference,square_sum_of_difference,abs(difference)*abs(difference));
				}
				//cout<< square_sum_of_difference<<endl;

				if (mpz_cmp(square_sum_of_difference,cnt0)<=0)
				{

					if (flag == 0) // the first approximate solution
					{
						if(mpz_cmp(square_sum_of_difference,const0)==0)
						{
							output_real_solutions(solutions_file, f);
						}
						
						for (int i = 0; i < (1 << n); i++)
						{
							f0[i] = f[i];
							fprintf(One_approximate_solution_file, "%d", f0[i]);
						}
					}
					else
					{
						for (int i = 0; i < (1 << n); i++) // compare the location with the first one
						{

							if (f[i] != f0[i])
							{

								if (P[i] == 0)
								{
									if(mpz_cmp(square_sum_of_difference,const0)==0)
									{
										output_real_solutions(solutions_file, f);
									}
									P_set[cnt_of_P]=i;
									cnt_of_P++;
									//fprintf(P_file, "%d\n", i);
								}
								P[i]++;
							}
						}
					}
					flag++;
					if (iter%100==0)
					{
						cout << " t: " << t << " \titer: " << iter << endl;
						cout << "\n" << endl;
						gmp_printf("Square Sum of Difference:%Zd",square_sum_of_difference);
						cout << "\n" << endl;
						cout << "|P|: " << cnt_of_P << endl;
						cout << "\n" << endl;
					}
				}
			}
		}
	}
	mpz_clear(square_sum_of_difference);
	mpz_clear(cnt0);
	cout << "find the number of P:" << cnt_of_P << endl;
	fprintf(P_file, "%d\n", cnt_of_P);
	for(int i=0;i<cnt_of_P;i++)
	{
		fprintf(P_file, "%d\n", P_set[i]);
	}
}

int main()
{

	init();
}
