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
    void solveVector();
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
        x = new int[xsz];
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

void SSumSmartSolver::solveVector() {
    int i, j;
    try {
        const int size = C + 1;
        int* a = new int[size];
        int* rweights = new int[xsz];
        std::vector<int> nzr = {0};
        std::fill_n(a, size, 0);
        std::fill_n(x, xsz, 0);
        rweights[obj + 1] = 0;
        for (int i = obj; i >= 1; i--) {
            rweights[i] = rweights[i + 1] + w[i];
        }

        auto printa = [&] {
            std::cout << "a = ";
            for (int i = 0; i < size; i++) {
                std::cout << " " << a[i] << " ";
            }
            std::cout << "\n";
        };

        auto printnzr = [&] {
            std::cout << "nzr = ";
            for (auto o : nzr) {
                std::cout << " " << o << " ";
            }
            std::cout << "\n";
        };
        int bestF = 0;
        for (int i = 1; i <= obj; i++) {
            std::vector<int> tmp;
            for (auto I = nzr.begin(); I != nzr.end();) {
                const int cc = *I;
                const int u = cc + rweights[i];
                if (u <= C) {
                    if (u > bestF) {
                        int nw = cc;
                        for (int j = i; j <= obj; j++) {
                            nw += w[j];
                            a[nw] = j;
                        }
                        bestF = u;
                    }
                    I = nzr.erase(I);
                } else {
                    const int cw = cc + w[i];
                    if (cw <= C) {
                        if (a[cw] == 0) {
                            a[cw] = i;
                            if (bestF < cw)
                                bestF = cw;
                            tmp.push_back(cw);
                        }
                    }
                    I++;
                }
            }
            nzr.insert(nzr.end(), tmp.begin(), tmp.end());
            if (bestF == C)
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
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate working arrays " << ba.what() << "\n";
        exit(-1);
    }
}

void SSumSmartSolver::solve() {
    int i, j;
    try {
        const int size = C + 1;
        int* a = new int[size];
        int* rweights = new int[xsz];
        int* nzr = new int[size];
        int nzrb = 1;
        nzr[0] = 0;
        std::fill_n(a, size, 0);
        std::fill_n(x, xsz, 0);
        rweights[obj + 1] = 0;

        for (int i = obj; i >= 1; i--) {
            rweights[i] = rweights[i + 1] + w[i];
        }

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

        int bestF = 0;
        int mones = 0;
        const int purgeTresh = 64;

        auto purge = [&] {
            if (mones > purgeTresh) {
                int whead = 0, rhead = 0;
                while ((rhead < nzrb) && (nzr[whead] != -1))
                    rhead++;
                whead = rhead;
                while (rhead < nzrb) {
                    if (nzr[rhead] == -1) {
                        rhead++;
                    } else {
                        nzr[whead] = nzr[rhead];
                        rhead++;
                        whead++;
                    }
                }
                nzrb = whead;
                mones = 0;
            }
        };

        for (int i = 1; i <= obj; i++) {
            int b = nzrb;
            for (int j = 0; j < nzrb; j++) {
                const int cc = nzr[j];
                if(cc == -1)
                    continue;
                const int u = cc + rweights[i];
                if (u <= C) {
                    if (u > bestF) {
                        bestF = u;
                    }
                    int nw = cc;
                    for (int k = i; k <= obj; k++) {
                        nw += w[k];
                        if (a[nw] == 0)
                            a[nw] = k;
                    }
                    nzr[j] = -1;
                } else {
                    const int cw = cc + w[i];
                    if (cw <= C) {
                        if (a[cw] == 0) {
                            a[cw] = i;
                            if (cw > bestF)
                                bestF = cw;
                            nzr[b] = cw;
                            b++;
                        }
                    }
                }
            }
            nzrb = b;
            //purge();
            if (bestF == C)
                break;
            //printa();
            //printnzr();
        }

        std::cout << "mones = " << mones << "\n";
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
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate working arrays " << ba.what() << "\n";
        exit(-1);
    }
}

void SSumSmartSolver::output() {
    /*
    for (int i = 0; i < obj; i++) {
        std::cout << x[i] << " ";
    }
    std::cout << "\n";
     */
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
