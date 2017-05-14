#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include <omp.h>
#include<iostream>
#include<fstream>
#include<iomanip>
#include <chrono>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

class SSumSmartSolver {
    int *p, *w, *x, C, obj, xsz, maxf;
public:
    void read(char *file_name);
    void solve();
    void output();

};

void SSumSmartSolver::read(char *file_name) {
    try {
        ifstream g;
        g.open(file_name);
        if (!g) {
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }
        g >> obj;
        g >> C;
        xsz = obj + 1;
        w = new int [xsz];
        p = new int [xsz];
        int t = 1;
        while (!g.eof()) {
            g >> p[t];
            g >> w[t];
            t++;
            if (t > obj)
                break;
        }
        maxf = 0;
        g.close();
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate coefficients " << ba.what() << "\n";
        exit(-1);
    }
}

void SSumSmartSolver::solve() {
    int i, j;
    try {
        const int size = C + 1;
        int* h = new int[size];
        x = new int[xsz];

        std::fill_n(h, size, 0);

        h[0] = 1;

        auto printh = [&] {
            std::cout << "h = ";
            for (int i = 0; i < size; i++) {
                std::cout << " " << h[i] << " ";
            }
            std::cout << "\n";
        };

#if COUNT
        long long int cnt = 0;
#endif        
        int i = 1;
        while ((i <= obj) && (h[C] == 0)) {
            const int wi = w[i];
            for (int c = C; c >= wi; c--) {
#if COUNT
                cnt++;
#endif
                if ((h[c] == 0) && (h[c - wi] != 0)) {
                    h[c] = i;
                }
            }
            i++;
        }

        std::fill_n(x, xsz, 0);
        int c = C;
        while (c > 0) {
            if (h[c] != 0) {
                maxf = c;
                break;
            } else
                c--;
        }
        while (c > 0) {
            const int j = h[c];
            x[j] = 1;
            c -= w[j];
        }
#if COUNT        
        std::cout << "Number of iterations = " << cnt << "\n";
#endif        
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate working arrays " << ba.what() << "\n";
        exit(-1);
    }
}

void SSumSmartSolver::output() {
#if 0    
    for (int i = 1; i <= obj; i++) {
        std::cout << x[i] << " ";
    }
#endif    
    std::cout << "\n";
    std::cout << "\n\nThe maximum weight is = " << maxf << "\n";
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
