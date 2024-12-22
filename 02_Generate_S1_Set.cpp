#include <bits/stdc++.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <gmpxx.h>
#define MAX_PATH 1000
#define N (1<<16)+100
#define num_of_S1 200   //select about 200 points to construct S1. You can adjust according to your computing power
using namespace std;
int S1_a[N],S1_b[N],cnt,part_walsh_pos[N]={0},part_walsh_value_abs[N]={0},part_walsh_value[N]={0};

void Qsort(int arr[], int low, int high){
    if (high <= low) return;
    int i = low;
    int j = high;
    int key = arr[low];
    while (true)
    {
 
        while (arr[i] >= key)
        {
        	i++;
            if (i == high){
                break;
            }
        }
 
        while (arr[j] <= key)
        {
        	j--;
            if (j == low){
                break;
            }
        }
        if (i >= j) break;

        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    arr[low] = arr[j];
    arr[j] = key;
    Qsort(arr, low, j - 1);
    Qsort(arr, j + 1, high);
}
int main()
{
	char Initial_path[MAX_PATH];
    getcwd(Initial_path, MAX_PATH);
	int Length_part_walsh;
	FILE *part_Walsh_spectrum_file,*S1_file;
	part_Walsh_spectrum_file=fopen(strcat(Initial_path,"/Problem/Challenge/16/16.1.txt"), "r");
	getcwd(Initial_path, MAX_PATH);//Reset Variables
	S1_file=fopen(strcat(Initial_path,"/Intermediate_parameters/Challenge/16.1/16.1_S1.txt"), "w");
	fscanf(part_Walsh_spectrum_file,"%d",&Length_part_walsh);
	int a,b;
	for (int i = 0; i <Length_part_walsh ; i++)
	{
		fscanf(part_Walsh_spectrum_file,"%d,%d",&a,&b);
		part_walsh_pos[i]=a;
		part_walsh_value[i]=b;
		part_walsh_value_abs[i]=abs(b);
	}
	Qsort(part_walsh_value_abs, 0, Length_part_walsh-1);
	
	for(int i=0;i<Length_part_walsh;i++)
	{	
		if(abs(part_walsh_value[i])>=part_walsh_value_abs[num_of_S1])  //select about 200 points to construct S1
		{
			cnt++;
			S1_a[cnt]=part_walsh_pos[i];
			S1_b[cnt]=part_walsh_value[i];
		}
	}

	fprintf(S1_file,"%d\n",cnt);
	for(int i=1;i<=cnt;i++)
	{
		fprintf(S1_file,"%d %d\n",S1_a[i],S1_b[i]);
	}
	cout<<"|S1|:"<<cnt<<endl;
	fclose(part_Walsh_spectrum_file);
	fclose(S1_file);
	return 0;
}
