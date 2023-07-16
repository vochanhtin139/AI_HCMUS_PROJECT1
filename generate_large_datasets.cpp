#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <fstream>
using namespace std;

int main() {
    srand(time(NULL));

    for(int i = 6; i <= 10; i++) {
        string file = "OUTPUT_" + to_string(i) + ".txt";
        ofstream output(file);

        int n, W, m;
        n = rand() % (1000 - 50 + 1) + 50;
        W = rand() % (1000 - 50 + 1) + 50;
        m = rand() % (10 - 5 + 1) + 5;
        output << n << endl << W << endl << m << endl;

        for(int i = 0; i < n; i++) {
            int w = rand() % (1000 - 50 + 1) + 50;
            output << w << " ";
        }

        output << endl;

        for(int i = 0; i < n; i++) {
            int v = rand() % (1000 - 50 + 1) + 50;
            output << v << " ";
        }

        output << endl;

        for(int i = 0; i < n; i++) {
            int c = rand() % (m - 1 + 1) + 1;
            output << c << " ";
        }

        output.close();
    }
}