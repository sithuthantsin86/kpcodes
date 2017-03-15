#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<iostream>
#include<fstream>
#include<iomanip>
#include <chrono>
#include <vector>
#include <algorithm>
#include <c++/6/chrono>

using namespace std;

class SSumSmartSolver {
    int *p, *w, *x, max_w, obj;
public:
    void read(char *file_name);
    void solve();
    void output();

};

void SSumSmartSolver::read(char *file_name) {
    try {
        ifstream g;
        int t = 0;
        g.open(file_name);
        if (!g) {
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }
        g >> obj;
        g >> max_w;
        w = new int [obj];
        p = new int[obj];
        while (!g.eof()) {
            g >> p[t];
            g >> w[t];
            t++;
            if (t > obj)
                break;
        }
        g.close();
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate coefficients " << ba.what() << "\n";
        exit(-1);
    }
}

void SSumSmartSolver::solve() {
    int i, j;
    try {
        const int size = max_w + 1;
        int* a = new int[size];
        int* nzr = new int[size];
        int nzrb = 1;
        nzr[0] = 0;
        x = new int[obj];

        std::fill_n(a, size, 0);

        auto printa = [&] {
            std::cout << "a = ";
            for (int i = 0; i < size; i++) {
                std::cout << " " << a[i] << " ";
            }
            std::cout << "\n";
        };

        auto printnzr = [&] {
            std::cout << "nzr = ";
            for (int i = 0; i < nzrb; i++) {
                std::cout << " " << nzr[i] << " ";
            }
            std::cout << "\n";
        };

        for (int i = 0; i < obj; i++) {
            int b = nzrb;
            for (int j = 0; j < nzrb; j++) {
                const int cw = nzr[j] + w[i];
                if (cw <= max_w) {
                    if (a[cw] == 0) {
                        a[cw] = i + 1   ;
                        nzr[b] = cw;
                        b++;
                    }
                }
            }
            nzrb = b;
            printa();
            printnzr();
        }
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate working arrays " << ba.what() << "\n";
        exit(-1);
    }
}

void SSumSmartSolver::output() {
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
    //cout << "\n\nThe maximum profit is = " << a[max_w * obj + obj - 1] << endl;
}

int main(int argc, char* argv[]) {
    SSumSmartSolver kp;
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
