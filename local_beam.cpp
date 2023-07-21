#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <time.h>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

struct Item {
    int weight;
    int value;
    int classType;
    Item(int weight, int value, int classType) {
        this->weight = weight;
        this->value = value;
        this->classType = classType;
    }
};

struct Candidate {
    vector<int> knapsack;
    int value;
    vector<int> classCounts;
};

bool isOverWeight(const Candidate& candidate, const vector<Item>& items, int capacity) {
    int totalWeight = 0;

    for (size_t i = 0; i < candidate.knapsack.size(); i++) 
        if (candidate.knapsack[i] == 1) totalWeight += items[i].weight;
    
    if (totalWeight <= capacity)
        return false;

    return true;
}

bool isEqualCandidate(Candidate addCandidate, vector<Candidate> nextCandidate) {
    for (int i = 0; i < nextCandidate.size(); i++) {
        for (int j = 0; j < nextCandidate[i].knapsack.size(); j++)
            if (addCandidate.knapsack[j] != nextCandidate[i].knapsack[j])
                return false;
    }
    
    return true;
}

vector<Candidate> generateInitialCandidate(int n, int beamWidth, const vector<Item>& items, int capacity) {
    vector<Candidate> initialCandidate;

   for (int i = 0; i < beamWidth; i++) {
        vector<int> knapsack(n , 0);
        for (int j  = 0; j < n; j++) {
            knapsack[j] = rand() % 2; // Random binary selection
        }
        Candidate addCandidate;
        addCandidate.knapsack = knapsack;
        addCandidate.value = 0;
        addCandidate.classCounts = vector<int>(); 
        if (!isOverWeight(addCandidate, items, capacity)) initialCandidate.push_back(addCandidate);        
    }

    return initialCandidate;
}

vector<Candidate> generateNextCandidate(const Candidate& candidate, int n, int beamWidth, const vector<Item>& items, int capacity) {
    vector<Candidate> nextCandidate;
    vector<int> indexUsed;

    for (int i = 0; i < beamWidth; i++) {
        vector<int> nextKnapsack = candidate.knapsack;
        nextKnapsack[i] = 1 - nextKnapsack[i]; //Flip the selected item
        Candidate addCandidate;
        addCandidate.knapsack = nextKnapsack;
        addCandidate.value = 0;
        addCandidate.classCounts = vector<int>(); 
        nextCandidate.push_back(addCandidate);
    }

    return nextCandidate;
}

vector<Candidate> scoreCandidate(const vector<Candidate>& candidates, const vector<Item>& items, int capacity) {
    vector<Candidate> scoredCandidate;

    for (const Candidate& candidate : candidates) {
        int totalWeight = 0;
        int totalValue = 0;
        vector<int> classCounts;

        for (size_t i = 0; i < candidate.knapsack.size(); i++) {
            if (candidate.knapsack[i] == 1) {
                totalWeight += items[i].weight;
                totalValue += items[i].value;

                if (items[i].classType >= classCounts.size()) {
                    classCounts.resize(items[i].classType + 1, 0);
                }
            }
        }

        if (totalWeight <= capacity) {
            Candidate addCandidate;
            addCandidate.knapsack = candidate.knapsack;
            addCandidate.value = totalValue;
            addCandidate.classCounts = classCounts; 
            scoredCandidate.push_back(addCandidate);
        }
    }

    return scoredCandidate;
}

bool Comparator(Candidate a, Candidate b) {
    return a.value > b.value;
}

vector<int> localBeamSearch(const vector<Item>& items, int capacity, int beamWidth, int maxSteps, string file_output, int test) {
    int n = items.size();
    vector<Candidate> curCandidates = generateInitialCandidate(n, beamWidth, items, capacity);

    if (curCandidates.empty()) {
        ofstream output(file_output);
        output << 0 << endl;
        for (int i = 0; i < n; i++) output << 0 << " ";
        output.close();
        exit(1);
    }

    for (int step = 0; step < maxSteps; step++) {
        vector<Candidate> nextCandidate;
        for (const Candidate& candidate : curCandidates) {
            vector<Candidate> expandedCandidates = generateNextCandidate(candidate, n, beamWidth, items, capacity);
            if (nextCandidate.empty()) nextCandidate.insert(nextCandidate.end(), expandedCandidates.begin(), expandedCandidates.end());
            else {
                for (int i = 0; i < expandedCandidates.size(); i++)
                    if (!isEqualCandidate(expandedCandidates[i], nextCandidate))
                        nextCandidate.push_back(expandedCandidates[i]);
            }
        }

        vector<Candidate> scoredCandidate = scoreCandidate(nextCandidate, items, capacity);
        sort(scoredCandidate.begin(), scoredCandidate.end(), Comparator);

        curCandidates.assign(scoredCandidate.begin(), scoredCandidate.end() + beamWidth);
    }
    
    return curCandidates[0].knapsack;
}

void read_input(int test, vector<int>& weights, vector<int>& values, vector<int>& classes, int &capacity, int &classNum, int &itemNum) {
    string file_input = "INPUT_" + to_string(test) + ".txt";
    ifstream input(file_input);

    int tempWeight = 0;
    int tempValue = 0;
    int tempClass = 0;

    input >> capacity >> classNum;
    itemNum = 0;
    input.ignore();

    string line;
    getline(input, line);
    istringstream ss(line);
    int num;
    while(ss >> num) {
        tempWeight = num;
        weights.push_back(tempWeight);
        itemNum++;      
    }

    for(int i = 0; i < itemNum; i++) {
        input >> tempValue;
        values.push_back(tempValue);
    }
    for(int i = 0; i < itemNum; i++) {
        input >> tempClass;
        classes.push_back(tempClass);
    }
    
    input.close();
}

int main() {
    srand(time(NULL));

    for (int test = 1; test <= 10; test++) {
        vector<Item> items;
        vector<int> weights;
        vector<int> values;
        vector<int> classes;

        int totalValue = 0;
        int capacity = 101;
        int beamWidth = 10;
        int maxSteps = 1;
        int classNum = 0;
        int itemNum = 0;

        read_input(test, weights, values, classes, capacity, classNum, itemNum);
 
        for (int i = 0; i < itemNum; i++) {
            Item item(weights[i], values[i], classes[i]);
            items.push_back(item);
        }
        
        // for (int i = 0; i < itemNum; i++)
        //     cout << items[i].weight << " " << items[i].value << " " << items[i].classType << endl;

        string file_output = "OUTPUT_" + to_string(test) + ".txt";
        ofstream output(file_output);

        double start = clock();
        vector<int> bestKnapsack = localBeamSearch(items, capacity, beamWidth, maxSteps, file_output, test);
        double end = clock();
        double runtime = end - start;

        for (int i = 0; i < bestKnapsack.size(); i++) {
            if (bestKnapsack[i] == 1) {
                totalValue += items[i].value;
            } 
        }

        output << totalValue << endl;
        for (int i = 0; i < bestKnapsack.size(); i++) {
            output << bestKnapsack[i] << " ";
        }

        cout << endl << "Execution time: " << runtime/ 1000 << endl;
       
        output.close();
    }
    return 0;
}