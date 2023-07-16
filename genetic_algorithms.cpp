#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int W, m, n, total_fitness, size, generations;
int w[100], v[100], c[100], fitness[100];
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

    int mark[50]; // check select full class
    for(int i = 1; i <= m; i++) 
        mark[i] = 0;

    for(int i = 0; i < n; i++) 
        if (chromosome[i] == 1) {
            total_weight += w[i];
            total_value += v[i];
            mark[c[i]] = 1;
        }

    if (total_weight > W) return 0;

    int num = 0;
    for(int i = 1; i <= m; i++)
        num += mark[i];
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

int main() {
    srand(time(NULL));

    // read input
    freopen("test.inp", "r", stdin);
    
    cin >> n >> W >> m;
    for(int i = 0; i < n; i++)
        cin >> w[i];
    for(int i = 0; i < n; i++)
        cin >> v[i];
    for(int i = 0; i < n; i++)
        cin >> c[i];

    size = 100, generations = 10;
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

    if (fitness[res] == 0) {
        cout << "No solution";
        return 0;
    }

    cout << fitness[res] << endl;
    for(int i = 0; i < n; i++)
        cout << population[res][i] << " ";
    
    return 0;
}