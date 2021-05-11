/* 2nd ASA Project
   Group 50
   Bernardo Castiço ist196845
   Hugo Rita ist 196870
   */

#include <iostream>
#include <vector>

using namespace std;

void processInput(int processes, int entries, vector<int>& ProcessorX, vector<int>& ProcessorY, vector<vector<int>>& weights){

    int i, X ,Y, weight;

    ProcessorX.reserve(processes);
    ProcessorY.reserve(processes);

    for(i = 0; i < processes; i++){
        scanf("%d %d", &X, &Y);
        ProcessorX.push_back(X);
        ProcessorY.push_back(Y);
    }
    
    for(i = 0; i < processes; i++) {
        weights[i].assign(processes, 0);
    }
    for(i = 0; i < entries; i++){
        scanf("%d %d %d", &X, &Y, &weight);
        weights[X-1].insert(weights[X-1].begin()+(Y-1), weight);
        weights[Y-1].insert(weights[Y-1].begin()+(X-1), weight);
    }

}

int Output(){
    return 0;
}



int main(){
    int output, processes, entries;

    vector <int> ProcessorX;
    vector <int> ProcessorY;
    vector <vector <int>> weights;

    scanf("%d %d", &processes, &entries);

    for(int i = 0; i < processes; i++){
        weights.insert(weights.begin() + i, vector<int>());
    }

    processInput(processes, entries, ProcessorX, ProcessorY, weights);

    output = Output();

    cout << output << endl;
}