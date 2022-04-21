#include <iostream>
#include <ctime>
#include <windows.h>
#include <process.h>

using namespace std;

const int NumberOfThreads = 6;

const int n = 1000;
const int m = 1000;
const int k = 1000;

int** matr_1 = new int* [n];
int** matr_2 = new int* [m];
int** matr_3 = new int* [n];

int q = n / NumberOfThreads,
r = n % NumberOfThreads;

int from[NumberOfThreads] = { 0 }, to[NumberOfThreads] = { 0 };

unsigned __stdcall ThreadFunctionMult(void* param)
{
    int iNum = *(reinterpret_cast<int*>(param));
    for (int i = from[iNum]; i < to[iNum]; i++)
    {
        for (int j = 0; j < k; j++)
        {
            matr_3[i][j] = 0;
            for (int z = 0; z < m; z++)
                matr_3[i][j] += matr_1[i][z] * matr_2[z][j];
        }
    }
    return 0;
}

int main()
{
    for (int i = 0; i < n; i++)
        matr_1[i] = new int[m];

    for (int i = 0; i < m; i++)
        matr_2[i] = new int[k];

    for (int i = 0; i < n; i++)
        matr_3[i] = new int[k];

    srand(time(0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            matr_1[i][j] = rand() % 3;
            //cout << matr_1[i][j] << " ";
        }
        //cout << endl;
    }

    //cout << endl;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            matr_2[i][j] = rand() % 3;
            //cout << matr_2[i][j] << " ";
        }
        //cout << endl;
    }

    //cout << endl;

    setlocale(LC_ALL, "rus");
    clock_t start, end;
    start = clock();

    HANDLE hThreads[NumberOfThreads];
    unsigned id;
    int noms[NumberOfThreads];
    for (int i = 0; i < NumberOfThreads; i++) {
        to[i] = from[i] + q + (i < r ? 1 : 0);
        noms[i] = i;
        hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunctionMult, (void*)(noms + i), 0, &id);
        if (i < NumberOfThreads - 1)
            from[i + 1] = to[i];
    }
    WaitForMultipleObjects(NumberOfThreads, hThreads, TRUE, INFINITE);
    for (int i = 0; i < NumberOfThreads; i++)
        CloseHandle(hThreads[i]);

    end = clock() - start;
    printf("Время выполнения программы: %f\n", end / (double)CLOCKS_PER_SEC);

    //for (int i = 0; i < n; i++)
    //{
        //for (int j = 0; j < n; j++)
            //cout << matr_3[i][j] << " ";
        //cout << endl;
    //}

    delete[]matr_1;
    delete[]matr_2;
    delete[]matr_3;
    system("pause");
    return 0;
}