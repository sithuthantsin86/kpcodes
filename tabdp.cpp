#include<omp.h>
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<fstream>
#include<iomanip>
#include <chrono>
#include <c++/6/chrono>

using namespace std;

class KnapSolver {
    int *a, *p, *w, *x, max_w, obj;
public:
    void read(char *file_name);
    void solve();
    void output();

};

void KnapSolver::read(char *file_name) {
    ifstream g;
    int t = 0;
    g.open(file_name);
    if (!g) {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    g >> obj;
    g >> max_w;
    w = (int *) malloc(obj * sizeof (int));
    if (w == NULL) {
        cerr << "Error : Your size is too much.\n";
        exit(1);
    }
    p = (int *) malloc(obj * sizeof (int));
    if (p == NULL) {
        cerr << "Error : Your size is too much.\n";
        exit(1);
    }
    while (!g.eof()) {
        g >> p[t];
        g >> w[t];
        t++;
        if (t > obj)
            break;
    }
    //printf("\nTotal number of object is %d.\n\n",obj);
    // printf("The maximum capacity of the knapsack is %d.\n\n",max_w);
}

void KnapSolver::solve() {
    int i, j;
    const int M = max_w + 1;
    const int N = obj;
    a = (int *) malloc(M * N * sizeof (int));

    //#define GETA(i,j) a[(i) * N + (j)]        
#define GETA(i,j) a[(j) * M + (i)]        

    if (a == NULL) {
        cerr << "Error : Your size is too much.\n";
        exit(1);
    }
    x = (int *) malloc(obj * sizeof (int));
    if (x == NULL) {
        cerr << "Error : Your size is too much.\n";
        exit(1);
    }
    for (j = 0; j < obj; j++) {
        for (i = 0; i < max_w + 1; i++) {
            if (i < w[j]) {
                if (i == 0 || j == 0)
                    GETA(i, j) = 0;
                else
                    GETA(i, j) = GETA(i, j - 1);
            }
            if (i >= w[j]) {
                if (j == 0)
                    GETA(i, j) = p[j];
                else {
                    int k = i - w[j];

                    if (GETA(i, j - 1) > GETA(k, j - 1) + p[j])
                        GETA(i, j) = GETA(i, j - 1);
                    else
                        GETA(i, j) = GETA(k, j - 1) + p[j];
                }

            }
        }
    }
    int k = max_w;
    for (int i = obj - 1; i >= 0; i--) {
        if (i == 0) {
            if (GETA(k, 0) == 0)
                x[i] = 0;
            else
                x[i] = 1;
        } else if (GETA(k, i) != GETA(k, i - 1)) {
            x[i] = 1;
            k = k - w[i];
        } else
            x[i] = 0;
    }
}

void KnapSolver::output() {
#if 0
    for (int j = 0; j < max_w + 1; j++) {
        for (int i = 0; i < obj; i++) {
            printf("%d ", a[j * obj + i]);
        }
        printf("\n");
    }
#endif

    //cout<<"\nThe binary solution of the knapsack is = ";
    /*for(int i=0;i<max_w+1;i++)
    {
            for(int j=0;j<obj;j++)
            {
                    cout<<setw(4)<<a[i*obj+j]<<" ";
            }
            cout<<endl;
    }*/
    //for(int i=0;i<obj;i++)cout<<x[i]<<" ";
    cout << "\n\nThe maximum profit is = " << a[max_w * obj + obj - 1] << endl;
}

int main(int argc, char* argv[]) {
    KnapSolver kp;
    char* str = NULL;
    //double start=0,end=0;
    if (argc >= 2) {
        str = argv[1];
    } else {
        fprintf(stderr, "usage: %s <input_file>\n", argv[0]);
        exit(-1);
    }
    //start = omp_get_wtime();
    kp.read(str);
    auto start = std::chrono::high_resolution_clock::now();
    kp.solve();
    auto end = std::chrono::high_resolution_clock::now();
    kp.output();
    cout << "time in millisecs: " << (std::chrono::duration_cast<chrono::milliseconds>(end - start)).count() << "\n";
    //end = omp_get_wtime();
    //printf("\nRuntime = %f seconds.\n\n",end-start);
    //cout<<end-start<<"\t";
}
