#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <chrono>
#include <random>
#include <math.h>
using namespace std;

class Instances_generator_ss {
    int *p, *w, capacity;
    FILE *a, *b, *c, *d, *e;
public:
    Instances_generator_ss(int number_of_items);
    void pthree_instances(int number_of_items, int num_instances);
    void psix_instances(int number_of_items, int num_instances);
    void evenodd_instances(int number_of_items, int num_instances);
    void evenoddvar_instances(int number_of_items, int num_instances);
    void avis_instances(int number_of_items, int num_instances);
    void somatoth_instances(int number_of_items, int num_instances);
    void f2_instances(int number_of_items, int num_instances);
    void printPair(FILE* f, int *p, int *w, int N, int C);
    string file_name(string method, int N, int C, int num_instance);
};

Instances_generator_ss::Instances_generator_ss(int number_of_items) {
    mkdir("inputs_ss", 0777);
    p = (int *) malloc(number_of_items * sizeof (int));
    if (p == NULL) {
        cerr << "Error : Your size is too much.\n";
        exit(1);
    }
    w = (int *) malloc(number_of_items * sizeof (int));
    if (w == NULL) {
        cerr << "Error : Your size is too much.\n";
        exit(1);
    }
}

void Instances_generator_ss::printPair(FILE* f, int *p, int *w, int N, int C) {
    fprintf(f, "%d\t%d\n", N, C);
    for (int i = 0; i < N; i++)
        fprintf(f, "%d\t%d\n", p[i], w[i]);
}

string Instances_generator_ss::file_name(string method, int N, int C, int num_instance) {
    ostringstream os;
    os << "inputs_ss/";
    os << method << "/";
    os << method << "_" << N << "_";
    if (num_instance < 10)os << "0" << "0" << num_instance;
    if (num_instance >= 10 && num_instance < 100)os << "0" << num_instance;
    if (num_instance >= 100)os << num_instance;
    os << "_" << C;
    return os.str();
}

void Instances_generator_ss::pthree_instances(int number_of_items, int num_instances) {
    string M = "pthree_instances";
    mkdir("inputs_ss/pthree_instances", 0777);
    // obtain a seed from the system clock:
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    // seeds the random number engine, the mersenne_twister_engine
    std::mt19937 generator(seed);
    // set a distribution range (1 - 1000)
    std::uniform_int_distribution<int> distribution(1, 1000);
    for (int j = 0; j < num_instances; j++) {
        for (int i = 0; i < number_of_items; i++) {
            p[i] = w[i] = distribution(generator);
        }
        capacity = floor(number_of_items * (1000 / 4));
        a = fopen(file_name(M, number_of_items, capacity, j + 1).c_str(), "w");
        printPair(a, p, w, number_of_items, capacity);
        fclose(a);
    }
}

void Instances_generator_ss::psix_instances(int number_of_items, int num_instances) {
    string M = "psix_instances";
    mkdir("inputs_ss/psix_instances", 0777);
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(1, 1000000);
    for (int j = 0; j < num_instances; j++) {
        for (int i = 0; i < number_of_items; i++)
            p[i] = w[i] = distribution(generator);
        capacity = floor(number_of_items * (1000000 / 4));
        b = fopen(file_name(M, number_of_items, capacity, j + 1).c_str(), "w");
        printPair(b, p, w, number_of_items, capacity);
        fclose(b);
    }
}

void Instances_generator_ss::evenodd_instances(int number_of_items, int num_instances) {
	int rand_num = 1;
    string M = "evenodd_instances";
    mkdir("inputs_ss/evenodd_instances", 0777);
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(1, 1000);
    for (int j = 0; j < num_instances; j++) {
        for (int i = 0; i < number_of_items; i++){
        	while(rand_num%2 > 0)
            	rand_num = distribution(generator);
            p[i] = w[i] = rand_num;
            rand_num = 1;
        }
        capacity = 2 * floor(number_of_items * (1000 / 8)) + 1;
        c = fopen(file_name(M, number_of_items, capacity, j + 1).c_str(), "w");
        printPair(c, p, w, number_of_items, capacity);
        fclose(c);
    }
}

void Instances_generator_ss::evenoddvar_instances(int number_of_items, int num_instances) {
    int rand_num = 1, D, sum_w = 0;
    cout<<"Please input range = ";
    cin>>D;
    string M = "evenoddvar_instances";
    mkdir("inputs_ss/evenoddvar_instances", 0777);
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(1, D);
    for (int j = 0; j < num_instances; j++) {
        for (int i = 0; i < number_of_items; i++){
            while(rand_num%2 > 0)
                rand_num = distribution(generator);
            p[i] = w[i] = rand_num;
            sum_w+=w[i];
            rand_num = 1;
        }
        if(j < num_instances/3)capacity = (2 * floor(sum_w / 8)) + 1;
        if(j >= num_instances/3 && j < (num_instances/3)*2)capacity = (2 * floor(sum_w / 4)) + 1;
        if(j >= (num_instances/3)*2)capacity = (2 * floor(sum_w / 2)) - 1;
        c = fopen(file_name(M, number_of_items, capacity, j + 1).c_str(), "w");
        printPair(c, p, w, number_of_items, capacity);
        fclose(c);
        sum_w = 0;
    }
}

void Instances_generator_ss::avis_instances(int number_of_items, int num_instances) {
    string M = "avis_instances";
    mkdir("inputs_ss/avis_instances", 0777);
    for (int i = 0; i < number_of_items; i++)
        p[i] = w[i] = number_of_items * (number_of_items + 1) + i + 1;
    capacity = (number_of_items * (number_of_items + 1)) * floor((number_of_items - 1) / 2) + (number_of_items * (number_of_items - 1) / 2);
    d = fopen(file_name(M, number_of_items, capacity, 1).c_str(), "w");
    printPair(d, p, w, number_of_items, capacity);
    fclose(d);
}

void Instances_generator_ss::somatoth_instances(int number_of_items, int num_instances) {
    int w1 = 0, w2 = 0, count=0;
    vector<int> a;
    vector<int> b;
    string M = "somatoth_instances";
    mkdir("inputs_ss/somatoth_instances", 0777);
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(2, number_of_items);
    for (int j = 0; j < num_instances; j++) {
    	while(count != 1){
    		count = 0;
        	w1 = distribution(generator);
            w2 = distribution(generator);
            capacity = (w1 - 1) * (w2 - 1) - 1;
            if((capacity / w1) < (number_of_items / 2) && (capacity / w2) < (number_of_items / 2)){
            	for(int k=1; k<=w1; k++){
            		if(w1%k == 0)
            			a.push_back (k);
           		}
            	for(int l=1; l<=w2; l++){
            		if(w2%l == 0)
            			b.push_back (l);
            	}
            	for(int i=0; i<a.size(); i++)
            		for(int j=0; j<b.size(); j++)
            			if(a[i]==b[j])count++;
            	a.clear();
            	b.clear();
            }
        }
        for (int i = 0; i < number_of_items; i++) {
            if (i % 2 == 0)
                p[i] = w[i] = ceil((i + 1) / 2.) * w1;
            else
                p[i] = w[i] = ceil((i + 1) / 2.) * w2;
        }
        //cout<<"("<<w1<<", "<<w2<<")"<<endl;
        e = fopen(file_name(M, number_of_items, capacity, j + 1).c_str(), "w");
        printPair(e, p, w, number_of_items, capacity);
        fclose(e);
        count = 0;
    }
}

void Instances_generator_ss::f2_instances(int number_of_items, int num_instances) {
    int w1 = 0, w2 = 0, count=0, E = 0, v = 0, phi = 0;
    cout<<"Please define the value of E for F2 = ";
    cin>>E;
    cout<<"Please define the value of v for F2 = ";
    cin>>v;
    cout<<"Please define the value of phi for F2 = ";
    cin>>phi;
    vector<int> a;
    vector<int> b;
    string M = "f2_instances";
    mkdir("inputs_ss/f2_instances", 0777);
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(2, pow(10, E));
    for (int j = 0; j < num_instances; j++) {
        while(count != 1){
            count = 0;
            w1 = distribution(generator);
            w2 = distribution(generator);
            capacity = (w1 - 1) * (w2 - 1) - 1;
            if((phi - v) * pow(10, E) <= capacity && capacity <= phi * pow(10, E)){
                for(int k=1; k<=w1; k++){
                    if(w1%k == 0)
                        a.push_back (k);
                }
                for(int l=1; l<=w2; l++){
                    if(w2%l == 0)
                        b.push_back (l);
                }
                for(int i=0; i<a.size(); i++)
                    for(int j=0; j<b.size(); j++)
                        if(a[i]==b[j])count++;
                a.clear();
                b.clear();
            }
        }
        for (int i = 1; i <= number_of_items / 2; i++) {
            if (i % 2 == 0)
                p[i-1] = w[i-1] = fmod((1 + (i - 1)), floor(capacity / w2)) * w2;
            else
                p[i-1] = w[i-1] = fmod((1 + (i - 1)), floor(capacity / w1)) * w1;
        }
        cout<<"("<<w1<<", "<<w2<<")"<<endl;
        e = fopen(file_name(M, number_of_items / 2, capacity, j + 1).c_str(), "w");
        printPair(e, p, w, number_of_items / 2, capacity);
        fclose(e);
        count = 0;
    }
}

int main(int argc, char* argv[]) {
    string method;
    struct stat st;
    int num_instances, number_of_items;
    if (argc >= 2) {
        method = argv[1];
        if (argc == 4) {
            number_of_items = atoi(argv[2]);
            num_instances = atoi(argv[3]);
        }
    } else {
        fprintf(stderr, "Usage: %s <method> <number of items> <How many instances>\n---Methods---\nP3 = pthree_instances\nP6 = psix_instances\nEO = evenodd_instances\nEOV = evenoddvar_instances\nAV = avis_instances\nST = somatoth_instances\nF2(E, v, phi) = f2_instances\n", argv[0]);
        exit(-1);
    }
    Instances_generator_ss IG(number_of_items);
    if (method == "P3" || method == "ALL") {
        if (stat("inputs_ss/pthree_instances", &st) == 0)system("rm -r inputs_ss/pthree_instances");
        IG.pthree_instances(number_of_items, num_instances);
    }
    if (method == "P6" || method == "ALL") {
        if (stat("inputs_ss/psix_instances", &st) == 0)system("rm -r inputs_ss/psix_instances");
        IG.psix_instances(number_of_items, num_instances);
    }
    if (method == "EO" || method == "ALL") {
        if (stat("inputs_ss/evenodd_instances", &st) == 0)system("rm -r inputs_ss/evenodd_instances");
        IG.evenodd_instances(number_of_items, num_instances);
    }
    if (method == "EOV" || method == "ALL") {
        if (stat("inputs_ss/evenoddvar_instances", &st) == 0)system("rm -r inputs_ss/evenoddvar_instances");
        IG.evenoddvar_instances(number_of_items, num_instances);
    }
    if (method == "AV" || method == "ALL") {
        if (stat("inputs_ss/avis_instances", &st) == 0)system("rm -r inputs_ss/avis_instances");
        IG.avis_instances(number_of_items, num_instances);
    }
    if (method == "ST" || method == "ALL") {
        if (stat("inputs_ss/somatoth_instances", &st) == 0)system("rm -r inputs_ss/somatoth_instances");
        IG.somatoth_instances(number_of_items, num_instances);
    }
    if (method == "F2" || method == "ALL") {
        if (stat("inputs_ss/f2_instances", &st) == 0)system("rm -r inputs_ss/f2_instances");
        IG.f2_instances(number_of_items, num_instances);
    }
    return 0;
}
