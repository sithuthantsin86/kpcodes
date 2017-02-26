#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <chrono>
#include <random>
#include <vector>
using namespace std;
class Instances_generator {
    int *p, *w, sum, min, capacity, C, alpha_size;
    FILE *a, *b, *c, *d, *e, *f, *g;
    vector<float> alpha;
public:
    Instances_generator(int number_of_items, int a_size);
    void Uncorrelated_data_instances(int number_of_items, int R, int num_instances);
    void Weekly_correlated_instances(int number_of_items, int R, int num_instances);
    void Strongly_correlated_instances(int number_of_items, int R, int num_instances);
    void Inverse_strongly_correlated_instances(int number_of_items, int R, int num_instances);
    void Almost_strongly_correlated_instances(int number_of_items, int R, int num_instances);
    void Subset_sum_instances(int number_of_items, int R, int num_instances);
    void Uncorrelated_instances_with_similar_weights(int number_of_items, int R, int num_instances);
    void printPair(FILE* f, int *p, int *w, int N, int C);
    string file_name(string method, int N, int R, int C, int num_instance);
};

Instances_generator::Instances_generator(int number_of_items, int a_size){
    sum = 0;
    min = 0;
    capacity = 0;
    C = 0;
    alpha_size = a_size;
    mkdir("inputs", 0777);
    for(int i=0; i<alpha_size; i++){
    	float temp=float(i+1)/float(alpha_size+1);
    	alpha.push_back (temp);
    }
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

void Instances_generator::printPair(FILE* f, int *p, int *w, int N, int C) {
    fprintf(f, "%d\t%d\n", N, C);
    for (int i = 0; i < N; i++)
        fprintf(f, "%d\t%d\n", p[i], w[i]);
}

string Instances_generator::file_name(string method, int N, int R, int C, int num_instance) {
    ostringstream os;
    os << "inputs/";
    os << method << "/";
    os << method << "_" << N << "_" << R << "_";
    if (num_instance < 10)os << "0" << "0" << num_instance;
    if (num_instance >= 10 && num_instance < 100)os << "0" << num_instance;
    if (num_instance >= 100)os << num_instance;
    os << "_" << C;
    return os.str();
}

void Instances_generator::Uncorrelated_data_instances(int number_of_items, int R, int num_instances) {
    string M = "Uncorrelated_data_instances";
    mkdir("inputs/Uncorrelated_data_instances", 0777);
    // obtain a seed from the system clock:
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    // seeds the random number engine, the mersenne_twister_engine
    std::mt19937 generator(seed);
    // set a distribution range (1 - R)
    std::uniform_int_distribution<int> distribution(1, R);
    for (int j = 0; j < num_instances * alpha_size; j++) {
        p[0] = distribution(generator);
        w[0] = distribution(generator);
        sum = sum + w[0];
        min = w[0];
        for (int i = 1; i < number_of_items; i++) {
            p[i] = distribution(generator);
            w[i] = distribution(generator);
            sum = sum + w[i];
            if (min > w[i])min = w[i];
        }
        C = alpha[j / num_instances] * sum;
        if (C >= min)capacity = C;
        else {
            capacity = min;
        }
        a = fopen(file_name(M, number_of_items, R, capacity, j + 1).c_str(), "w");
        printPair(a, p, w, number_of_items, capacity);
        fclose(a);
        sum = 0;
        min = 0;
    }
}

void Instances_generator::Weekly_correlated_instances(int number_of_items, int R, int num_instances) {
    string M = "Weekly_correlated_instances";
    mkdir("inputs/Weekly_correlated_instances", 0777);
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution1(1, R);
    for (int j = 0; j < num_instances * alpha_size; j++) {
        w[0] = distribution1(generator);
        std::uniform_int_distribution<int> distribution2(w[0]-(R / 10), w[0]+(R / 10));
        p[0] = distribution2(generator);
        sum = sum + w[0];
        min = w[0];
        for (int i = 1; i < number_of_items; i++) {
            w[i] = distribution1(generator);
            std::uniform_int_distribution<int> distribution2(w[i]-(R / 10), w[i]+(R / 10));
            p[i] = distribution2(generator);
            sum = sum + w[i];
            if (min > w[i])min = w[i];
        }
        C = alpha[j / num_instances] * sum;
        if (C >= min)capacity = C;
        else {
            capacity = min;
        }
        b = fopen(file_name(M, number_of_items, R, capacity, j + 1).c_str(), "w");
        printPair(b, p, w, number_of_items, capacity);
        fclose(b);
        sum = 0;
        min = 0;
    }
}

void Instances_generator::Strongly_correlated_instances(int number_of_items, int R, int num_instances) {
    string M = "Strongly_correlated_instances";
    mkdir("inputs/Strongly_correlated_instances", 0777);
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(1, R);
    for (int j = 0; j < num_instances * alpha_size; j++) {
        w[0] = distribution(generator);
        p[0] = w[0]+(R / 10);
        sum = sum + w[0];
        min = w[0];
        for (int i = 1; i < number_of_items; i++) {
            w[i] = distribution(generator);
            p[i] = w[i]+(R / 10);
            sum = sum + w[i];
            if (min > w[i])min = w[i];
        }
        C = alpha[j / num_instances] * sum;
        if (C >= min)capacity = C;
        else {
            capacity = min;
        }
        c = fopen(file_name(M, number_of_items, R, capacity, j + 1).c_str(), "w");
        printPair(c, p, w, number_of_items, capacity);
        fclose(c);
        sum = 0;
        min = 0;
    }
}

void Instances_generator::Inverse_strongly_correlated_instances(int number_of_items, int R, int num_instances) {
    string M = "Inverse_strongly_correlated_instances";
    mkdir("inputs/Inverse_strongly_correlated_instances", 0777);
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(1, R);
    for (int j = 0; j < num_instances * alpha_size; j++) {
        p[0] = distribution(generator);
        w[0] = p[0]+(R / 10);
        sum = sum + w[0];
        min = w[0];
        for (int i = 1; i < number_of_items; i++) {
            p[i] = distribution(generator);
            w[i] = p[i]+(R / 10);
            sum = sum + w[i];
            if (min > w[i])min = w[i];
        }
        C = alpha[j / num_instances] * sum;
        if (C >= min)capacity = C;
        else {
            capacity = min;
        }
        d = fopen(file_name(M, number_of_items, R, capacity, j + 1).c_str(), "w");
        printPair(d, p, w, number_of_items, capacity);
        fclose(d);
        sum = 0;
        min = 0;
    }
}

void Instances_generator::Almost_strongly_correlated_instances(int number_of_items, int R, int num_instances) {
    string M = "Almost_strongly_correlated_instances";
    mkdir("inputs/Almost_strongly_correlated_instances", 0777);
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution1(1, R);
    for (int j = 0; j < num_instances * alpha_size; j++) {
        w[0] = distribution1(generator);
        std::uniform_int_distribution<int> distribution2(w[0]+(R / 10)-(R / 500), w[0]+(R / 10)+(R / 500));
        p[0] = distribution2(generator);
        sum = sum + w[0];
        min = w[0];
        for (int i = 1; i < number_of_items; i++) {
            w[i] = distribution1(generator);
            std::uniform_int_distribution<int> distribution2(w[i]+(R / 10)-(R / 500), w[i]+(R / 10)+(R / 500));
            p[i] = distribution2(generator);
            sum = sum + w[i];
            if (min > w[i])min = w[i];
        }
        C = alpha[j / num_instances] * sum;
        if (C >= min)capacity = C;
        else {
            capacity = min;
        }
        e = fopen(file_name(M, number_of_items, R, capacity, j + 1).c_str(), "w");
        printPair(e, p, w, number_of_items, capacity);
        fclose(e);
        sum = 0;
        min = 0;
    }
}

void Instances_generator::Subset_sum_instances(int number_of_items, int R, int num_instances) {
    string M = "Subset_sum_instances";
    mkdir("inputs/Subset_sum_instances", 0777);
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(1, R);
    for (int j = 0; j < num_instances * alpha_size; j++) {
        p[0] = w[0] = distribution(generator);
        sum = sum + w[0];
        min = w[0];
        for (int i = 1; i < number_of_items; i++) {
            p[i] = w[i] = distribution(generator);
            sum = sum + w[i];
            if (min > w[i])min = w[i];
        }
        C = alpha[j / num_instances] * sum;
        if (C >= min)capacity = C;
        else {
            capacity = min;
        }
        f = fopen(file_name(M, number_of_items, R, capacity, j + 1).c_str(), "w");
        printPair(f, p, w, number_of_items, capacity);
        fclose(f);
        sum = 0;
        min = 0;
    }
}

void Instances_generator::Uncorrelated_instances_with_similar_weights(int number_of_items, int R, int num_instances) {
    string M = "Uncorrelated_instances_with_similar_weights";
    mkdir("inputs/Uncorrelated_instances_with_similar_weights", 0777);
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution1(1, 1000);
    std::uniform_int_distribution<int> distribution2(100000, 100100);
    for (int j = 0; j < num_instances * alpha_size; j++) {
        p[0] = distribution1(generator);
        w[0] = distribution1(generator);
        sum = sum + w[0];
        min = w[0];
        for (int i = 1; i < number_of_items; i++) {
            p[i] = distribution1(generator);
            w[i] = distribution1(generator);
            sum = sum + w[i];
            if (min > w[i])min = w[i];
        }
        C = alpha[j / num_instances] * sum;
        if (C >= min)capacity = C;
        else {
            capacity = min;
        }
        g = fopen(file_name(M, number_of_items, R, capacity, j + 1).c_str(), "w");
        printPair(g, p, w, number_of_items, capacity);
        fclose(g);
        sum = 0;
        min = 0;
    }
}

int main(int argc, char* argv[]) {
    string method;
    struct stat st;
    int num_instances, number_of_items, R, a_size;
    if (argc >= 2) {
        method = argv[1];
        if (argc == 6) {
            number_of_items = atoi(argv[2]);
            R = atoi(argv[3]);
            num_instances = atoi(argv[4]);
            a_size = atoi(argv[5]);
        }
    } else {
        fprintf(stderr, "Usage: %s <method> <number of items> <range> <How many instances for each alpha value> <Alpha size>\n---Methods---\nUC = Uncorrelated_data_instances\nWC = Weekly_correlated_instances\nSC = Strongly_correlated_instances\nISC = Inverse_strongly_correlated_instances\nASC = Almost_strongly_correlated_instances\nSS = Subset_sum_instances\nUCS = Uncorrelated_instances_with_similar_weights\n", argv[0]);
        exit(-1);
    }
    Instances_generator IG(number_of_items, a_size);
    if (method == "UC" || method == "ALL") {
        if (stat("inputs/Uncorrelated_data_instances", &st) == 0)system("rm -r inputs/Uncorrelated_data_instances");
        IG.Uncorrelated_data_instances(number_of_items, R, num_instances);
    }
    if (method == "WC" || method == "ALL") {
        if (stat("inputs/Weekly_correlated_instances", &st) == 0)system("rm -r inputs/Weekly_correlated_instances");
        IG.Weekly_correlated_instances(number_of_items, R, num_instances);
    }
    if (method == "SC" || method == "ALL") {
        if (stat("inputs/Strongly_correlated_instances", &st) == 0)system("rm -r inputs/Strongly_correlated_instances");
        IG.Strongly_correlated_instances(number_of_items, R, num_instances);
    }
    if (method == "ISC" || method == "ALL") {
        if (stat("inputs/Inverse_strongly_correlated_instances", &st) == 0)system("rm -r inputs/Inverse_strongly_correlated_instances");
        IG.Inverse_strongly_correlated_instances(number_of_items, R, num_instances);
    }
    if (method == "ASC" || method == "ALL") {
        if (stat("inputs/Almost_strongly_correlated_instances", &st) == 0)system("rm -r inputs/Almost_strongly_correlated_instances");
        IG.Almost_strongly_correlated_instances(number_of_items, R, num_instances);
    }
    if (method == "SS" || method == "ALL") {
        if (stat("inputs/Subset_sum_instances", &st) == 0)system("rm -r inputs/Subset_sum_instances");
        IG.Subset_sum_instances(number_of_items, R, num_instances);
    }
    if (method == "UCS" || method == "ALL") {
        if (stat("inputs/Uncorrelated_instances_with_similar_weights", &st) == 0)system("rm -r inputs/Uncorrelated_instances_with_similar_weights");
        IG.Uncorrelated_instances_with_similar_weights(number_of_items, R, num_instances);
    }
    return 0;
}
