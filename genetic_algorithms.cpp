#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <cstring>

using namespace std;

int W, m, n, total_fitness, size, generations;
int w[1000], v[1000], c[1000], fitness[1000];
vector<vector<int> > population;

void generate_random_population(int size) {
    vector<int> chromosome;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < n; j++) {
            int genes = rand() % 2; // genes = [0, 1]
            chromosome.push_back(genes);
        }
        population.push_back(chromosome);
        chromosome.clear();
    }
}

int calculate_fitness(vector<int> chromosome) {
    int total_weight = 0, total_value = 0;

    int mark[20]; // check select full class
    for(int i = 1; i <= m; i++) 
        mark[i] = 0;

    for(int i = 0; i < n; i++) 
        if (chromosome[i] == 1) {
            total_weight += w[i];
            total_value += v[i];
            mark[c[i]]++;
        }

    if (total_weight > W) {
        while (total_weight > W) {
            int k = rand() % n;
            while (chromosome[k] == 0)
                k = rand() % n;
            chromosome[k] = 0;
            mark[c[k]]--;
            total_weight -= w[k];
            total_value -= v[k];
        }
    }

    int num = 0;
    for(int i = 1; i <= m; i++)
        if (mark[i] > 0) num++;
    if (num < m) return 0;
    
    return total_value;
}

int select_chromosome() {
    // Roulette wheel
    double random = (rand() % total_fitness) / (total_fitness * 1.0);
    double offset = 0;
    for(int i = 0; i < size; i++) {
        double probability = fitness[i] / (total_fitness * 1.0);
        offset += probability;
        if (random < offset) return i;
    }
    return -1;
}


void crossover(int c1, int c2) {
    vector<int> parent1 = population[c1];
    vector<int> parent2 = population[c2];
    int random = rand() % n;
    for(int i = 0; i < random; i++)
        swap(parent1[i], parent2[i]);
}

void mutate(vector<int> &chromosome) {
    for(int i = 0; i < n; i++) {
        int random = rand() % 100;
        if (random == 0) 
            chromosome[i] = !chromosome[i];
    }
}

int get_best() {
    int maxf = INT_MIN;
    int pos = -1;
    for(int i = 0; i < size; i++) {
        fitness[i] = calculate_fitness(population[i]);
        if (fitness[i] > maxf) {
            maxf = fitness[i];
            pos = i;
        }
    }
    return pos;
}

int genetic_algorithms() {
    population.clear();
    
    size = 1000, generations = 10;
    generate_random_population(size);

    total_fitness = 0;
    for(int i = 0; i < size; i++) {
        fitness[i] = calculate_fitness(population[i]);
        total_fitness += fitness[i];
    }

    for(int i = 0; i < generations; i++) {
        // reproduction
        int c1 = select_chromosome();
        int c2 = select_chromosome();

        if (c1 == -1 || c2 == -1)
            continue;

        crossover(c1, c2);

        mutate(population[c1]);
        mutate(population[c2]);
    }

    int res = get_best();

    return res;
}

int main() {
    srand(time(NULL));

    for(int test = 1; test <= 10; test++) {
        // read input
        string file_input = "INPUT_" + to_string(test) + ".txt";
        ifstream input(file_input);
        
        input >> n >> W >> m;
        for(int i = 0; i < n; i++)
            input >> w[i];
        for(int i = 0; i < n; i++)
            input >> v[i];
        for(int i = 0; i < n; i++)
            input >> c[i];

        clock_t start, end;
        start = clock();

        int res = genetic_algorithms();

        end = clock();

        double time_use = (double)(end - start) / CLOCKS_PER_SEC;

        cout << "- Test " << test << ":\n";
        cout << "Successful!\n";
        cout << "Execution time: " << time_use << " second(s)\n\n";

        // write output
        string file_output = "OUTPUT_" + to_string(test) + ".txt";
        ofstream output(file_output);
        if (fitness[res] == 0) {
            output << "No solution";

            input.close();
            output.close();
            continue;
        }

        output << fitness[res] << endl;
        for(int i = 0; i < n; i++)
            output << population[res][i] << " ";

        input.close();
        output.close();
    }
    
    return 0;
}
