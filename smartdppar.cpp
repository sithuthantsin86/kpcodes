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
    void solvemt();
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
        const int nt = omp_get_num_threads();
        int* a = new int[size];
        int* nzr = new int[size];
        int nzrb = 1;
        nzr[0] = 0;
        x = new int[xsz];

        std::fill_n(a, size, 0);
        std::fill_n(x, obj, 0);

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

        for (int i = 1; i <= obj; i++) {
            int b = nzrb;
#pragma omp parallel for
            for (int j = 0; j < nzrb; j++) {
                const int cw = nzr[j] + w[i];
                if (cw <= C) {
                    if (a[cw] == 0) {
                        a[cw] = i;
#pragma omp critical 
                        {
                            nzr[b] = cw;
                            b++;
                        }
                    }
                }
            }
            nzrb = b;
            if (a[C] != 0)
                break;
            //printa();
            //printnzr();
        }

        int i = size - 1;
        while (i > 0) {
            if (a[i] != 0) {
                maxf = i;
                break;
            } else
                i--;
        }
        while (i > 0) {
            int j = a[i];
            if (j != 0) {
                x[j] = 1;
                i -= w[j];
            } else
                i--;
            //std::cout << "i = " << i << ", j = " << j << "\n";
        }
        delete [] a;
        delete [] nzr;
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate working arrays " << ba.what() << "\n";
        exit(-1);
    }
}

void SSumSmartSolver::solvemt() {
    try {
        const int size = C + 1;
        int nt;
#pragma omp parallel 
#pragma omp single
        nt = omp_get_num_threads();

        std::cout << "nt = " << nt << "\n";
        int* a = new int[size];
        int* nzr = new int[size];
        int** nzrt = new int*[nt];
        for (int i = 0; i < nt; i++) {
            nzrt[i] = new int[size];
        }
        int* nb = new int [nt];
        int* nsft = new int [nt];
        int nzrb = 1;
        nzr[0] = 0;
        x = new int[xsz];

        std::fill_n(a, size, 0);
        std::fill_n(x, obj, 0);


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

        for (int i = 1; i <= obj; i++) {
#pragma omp parallel 
            {
                const int tn = omp_get_thread_num();
                int nbloc = 0;
#pragma omp for
                for (int j = 0; j < nzrb; j++) {
                    const int cw = nzr[j] + w[i];
                    if (cw <= C) {
                        if (a[cw] == 0) {
                            a[cw] = i;
                            nzrt[tn][nbloc] = cw;
                            nbloc ++;
                        }
                    }
                }
                
                nb[tn] = nbloc;
#pragma omp barrier  

#pragma omp single 
                {
                    nsft[0] = nzrb;
                    for (int k = 1; k < nt; k++) {
                        nsft[k] = nsft[k - 1] + nb[k - 1];
                    }
                }
#pragma omp barrier                
                for (int j = 0; j < nbloc; j++) {
                    nzr[nsft[tn] + j] = nzrt[tn][j];
                }
            }
            nzrb = nsft[nt - 1] + nb[nt - 1];

            if (a[C] != 0)
                break;
            //printa();
            //printnzr();
        }

        int i = size - 1;
        while (i > 0) {
            if (a[i] != 0) {
                maxf = i;
                break;
            } else
                i--;
        }
        while (i > 0) {
            int j = a[i];
            if (j != 0) {
                x[j] = 1;
                i -= w[j];
            } else
                i--;
            //std::cout << "i = " << i << ", j = " << j << "\n";
        }
        delete [] a;
        delete [] nzr;
        for (int i = 0; i < nt; i++)
            delete nzrt[i];
        delete [] nzrt;
        delete [] nb;
        delete [] nsft;
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate working arrays " << ba.what() << "\n";
        exit(-1);
    }
}

void SSumSmartSolver::output() {
    for (int i = 0; i < obj; i++) {
        std::cout << x[i] << " ";
    }
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
    //kp.solvemt();
    auto end = std::chrono::high_resolution_clock::now();
    kp.output();
    cout << "time in millisecs: " << (std::chrono::duration_cast<chrono::milliseconds>(end - start)).count() << "\n";
    //end = omp_get_wtime();
    //printf("\nRuntime = %f seconds.\n\n",end-start);
    //cout<<end-start<<"\t";
}
