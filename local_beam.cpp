#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <time.h>
#include <cstring>
#include <fstream>

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

    while (initialCandidate.size() < beamWidth) {
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
        if (!isOverWeight(addCandidate, items, capacity)) nextCandidate.push_back(addCandidate);
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

vector<int> localBeamSearch(const vector<Item>& items, int capacity, int beamWidth, int maxSteps) {
    int n = items.size();
    vector<Candidate> curCandidates = generateInitialCandidate(n, beamWidth, items, capacity);

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

int main() {
    srand(time(NULL));
    vector<Item> items;
    vector<int> weights;
    vector<int> values;
    vector<int> classes;

    int capacity = 101;
    int beamWidth = 5;
    int maxSteps = 1;
    int totalValue = 0;
    int classNum = 0;
    int itemNum = 0;
    int tempWeight = 0;
    int tempValue = 0;
    int tempClass = 0;

    string file_input = "INPUT_3.txt";
    ifstream input(file_input);

    input >> itemNum >> capacity >> classNum;

    for (int i = 0; i < itemNum; i++) {
        input >> tempWeight;
        weights.push_back(tempWeight);
    }
    for (int i = 0; i < itemNum; i++) {
        input >> tempValue;
        values.push_back(tempValue);
    }
    for (int i = 0; i < itemNum; i++) {
        input >> tempClass;
        classes.push_back(tempClass);
    }
    for (int i = 0; i < 35; i++) {
        Item item(weights[i], values[i], classes[i]);
        items.push_back(item);
    }
    
    // for (int i = 0; i < itemNum; i++)
    //     cout << items[i].weight << " " << items[i].value << " " << items[i].classType << endl;

    double start = clock();
    vector<int> bestKnapsack = localBeamSearch(items, capacity, beamWidth, maxSteps);
    double end = clock();
    double runtime = end - start;

    cout << "Selected items: ";
    for (size_t i = 0; i < bestKnapsack.size(); i++) {
        if (bestKnapsack[i] == 1) {
            cout << i + 1 << " ";
            totalValue += items[i].value;
        } 
    }

    cout << endl << "Execution time: " << runtime/ 1000 << endl;
    cout << totalValue;

    input.close();

    return 0;
}