#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

long long max_value = 0, sum_weight = 0, sum_value = 0, n, wgt, cls;

void print(long long n, long long &sum_weight, long long &sum_value, long long *cls_check, long long *state, long long *save_state) {
    for (int i = 1; i <= cls; i++)
        if (!cls_check[i])
            return;

    if (sum_value > max_value) {
        max_value = sum_value;

        for (int i = 0; i < n; i++)
            save_state[i] = state[i];
    }
}

void Try(long long i, long long n, long long &sum_weight, long long &sum_value, long long *wgts, long long *vls, long long *clss, long long *cls_check, long long *state, long long *save_state) {
    //do not get the item
    state[i] = 0;
    if (i < n - 1)
        Try(i + 1, n, sum_weight, sum_value, wgts, vls, clss, cls_check, state, save_state);
    else   
        print(n, sum_weight, sum_value, cls_check, state, save_state);

    //get the item
    if (sum_weight + wgts[i] > wgt)
        return;

    state[i] = 1;
    cls_check[clss[i]]++;
    sum_weight += wgts[i];
    sum_value += vls[i];

    if (i < n - 1)
        Try(i + 1, n, sum_weight, sum_value, wgts, vls, clss, cls_check, state, save_state);
    else
        print(n, sum_weight, sum_value, cls_check, state, save_state);
    
    cls_check[clss[i]]--;
    sum_weight -= wgts[i];
    sum_value -= vls[i];
}

int main() {
    for (int test = 1; test <= 10; test++) {
        string inpFileName = "INPUT_" + to_string(test) + ".txt";
        ifstream inpFile(inpFileName);
        string line;

        inpFile >> wgt >> cls;
    
        long long *wgts = new long long[50000];
        long long *vls = new long long[50000];
        long long *clss = new long long[50000];

        getline(inpFile, line);
        getline(inpFile, line);
        n = 0;
        long long num;
        istringstream iss(line);

        while (iss >> num) 
            wgts[n++] = num;
        
        for (int i = 0; i < n; i++)
            inpFile >> vls[i];

        for (int i = 0; i < n; i++)
            inpFile >> clss[i];

        long long *cls_check = new long long[cls + 1];
        long long *state = new long long[50000];
        long long *save_state = new long long[50000];
        for (int i = 0; i <= cls; i++) {
            cls_check[i] = 0;
            state[i] = 0;
            save_state[i] = 0;
        }

        clock_t start, end;
        max_value = 0, sum_weight = 0, sum_value = 0;

        start = clock();
        Try(0, n, sum_weight, sum_value, wgts, vls, clss, cls_check, state, save_state);
        end = clock();

        string outFileName = "OUTPUT_" + to_string(test) + ".txt";
        ofstream outFile(outFileName);

        cout << "- Test " << test << ":\n";
        cout << "Successful!\n";
        cout << "Execution time: " << (double) (end - start) / CLOCKS_PER_SEC << " second(s)\n\n" << endl;

        if (max_value == 0) outFile << "No solution";
        else {
            outFile << max_value << endl;
            for (int i = 0; i < n; i++)
                outFile << save_state[i] << " ";
        }

        delete [] wgts;
        delete [] vls;
        delete [] clss;
        delete [] cls_check;
        delete [] state;
        delete [] save_state;

        inpFile.close();
        outFile.close();
    }

    return 0;
}