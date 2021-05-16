/* 2nd ASA Project
   Group 50
   Bernardo CastiÃ§o ist196845
   Hugo Rita ist 196870
   */

#include <iostream>
#include <vector>
#include <list>

#define S -1

using namespace std;

void processInput(int processes, int entries, vector<int>& ProcessorX, vector<int>& ProcessorY, vector<vector<int>>& weights,
                  vector<vector<int>>& adjacencies, vector<int>& height, vector<int>& excess, vector <vector <int>>& capacities,
                  vector <vector <int>>& flow){

    int i, X ,Y, weight;

    ProcessorX.reserve(processes);
    ProcessorY.reserve(processes);

    excess.assign(processes + 2, 0);
    height.assign(processes + 2, 0);

    for(i = 0; i < processes+2; i++) {
        flow.insert(flow.begin()+i, vector<int>());
        capacities.insert(capacities.begin()+i, vector<int>());
        capacities[i].assign(processes + 2, 0);
        flow[i].assign(processes + 2, 0);
    }
    for(i = 0; i < processes; i++){
        if (!scanf("%d %d", &X, &Y)) {
            cout << "ERROR" << endl;
        }
        weights.insert(weights.begin() + i, vector<int>());
        adjacencies.insert(adjacencies.begin() + i, vector<int>());
        ProcessorX.push_back(X);
        ProcessorY.push_back(Y);
        capacities[0][i + 1] = X;
        capacities[i + 1][processes + 1] = Y;
        weights[i].assign(processes, 0);
        adjacencies[i].push_back(S);
        adjacencies[i].push_back(processes);
    }

    for(i = 0; i < entries; i++){
        if(!scanf("%d %d %d", &X, &Y, &weight)){
            cout << "ERROR" << endl;
        }
        weights[Y-1][X-1] = weight;
        weights[X-1][Y-1] = weight;
        adjacencies[X-1].push_back(Y-1);
        adjacencies[Y-1].push_back(X-1);
        capacities[X][Y] = weight;
        capacities[Y][X] = weight;
    }
}

void InicializePreFlow(vector<int>& excess, vector<int>& height, int processes, vector<int>& ProcessorX, vector<int>& ProcessorY,
                       vector <vector <int>>& capacities, vector <vector <int>>& flow){

    int i;

    height[0] = processes + 2;

    for(i = 0; i < processes; i++){
        excess[i+1] = ProcessorX[i];
        capacities[0][i+1] -= ProcessorX[i];
        capacities[i+1][0] += ProcessorX[i];
        flow[0][i+1] += ProcessorX[i];
        excess[0] -= ProcessorX[i];
    }
}

void Push(int u, int v, vector<int>& excess, vector <vector <int>>& capacities, vector <vector <int>>& flow){
    int d;
    if(excess[u+1] >= capacities[u+1][v+1])
        d = capacities[u+1][v+1];
    else
        d = excess[u+1];
    flow[u+1][v+1] += d;
    flow[v+1][u+1] -= d;
    excess[u+1] -= d;
    excess[v+1] += d;
    capacities[u+1][v+1] -= d;
    capacities[v+1][u+1] += d;
}

void Relabel(vector<int>& height, vector<vector<int>>& adjacencies, int u, vector <vector <int>>& capacities){
    height[u+1] = height[adjacencies[u][0] + 1] + 1;
    for(int i = 0; i < (int)adjacencies[u].size()-1; i++){
        if(height[u + 1] > height[adjacencies[u][i+1]+1] && capacities[u+1][adjacencies[u][i+1]+1] > 0){
            height[u+1] = 1 + height[adjacencies[u][i+1]+1];
        }
    }
}

int VerifyHeight(int u, vector<int>& height, vector<vector<int>>& adjacencies, vector <vector <int>>& capacities){
    int result = 1;
    for(int i = 0; i < (int)adjacencies[u].size(); i++){
        if(height[u+1] > height[adjacencies[u][i]+1] && capacities[u+1][adjacencies[u][i]+1] > 0) {
            result = 0;
            break;
        }
    }
    return result;
}

void Discharge(vector<int>& excess, vector<int>& height, vector<int>& ProcessorX, vector<int>& ProcessorY,
               vector<vector<int>>& weights, vector<vector<int>>& adjacencies, int u, vector<int> current,
               vector <vector <int>>& capacities, vector <vector <int>>& flow){
    int v;
    int i = 0;

    while(excess[u+1] > 0){
        v = current[u];
        if(VerifyHeight(u, height, adjacencies, capacities)){
            Relabel(height , adjacencies, u, capacities);
            current[u] = adjacencies[u][i];
        }
        else if(capacities[u+1][v+1] > 0 && height[u+1] == height[v+1]+1){
            Push(u, v, excess, capacities, flow);
        }
        else{
            i++;
            if(i == (int)(adjacencies[u].size())){
                i = 0;
            }
            current[u] = adjacencies[u][i];
        }
    }
}

int RelableToFront(vector<int>& excess, vector<int>& height, vector<int>& ProcessorX, vector<int>& ProcessorY,
                   vector<vector<int>>& weights, int processes, vector<vector<int>>& adjacencies,
                   vector <vector <int>>& capacities, vector <vector <int>>& flow){

    vector<int> Aux;
    list<int> L;
    vector<int> current;
    int i, u, oldheight;

    InicializePreFlow(excess, height, processes, ProcessorX, ProcessorY, capacities, flow);
    for (i = 0; i < processes; i++) {
        L.push_back(i);
        current.push_back(adjacencies[i][0]);
    }

    u = L.front();

    while(u != -1){
        Aux.clear();
        oldheight = height[u+1];
        Discharge(excess, height, ProcessorX, ProcessorY, weights, adjacencies,
                  u, current, capacities, flow);
        if(height[u+1] > oldheight){
            L.remove(u);
            L.push_front(u);
        }
        for(i = 0; i < (int)L.size(); i++){
            if(excess[i+1] > 0){
                u = i;
                break;
            }
            u = -1;
        }
    }
    return excess[processes+1];
}

int Output(vector<int>& height, vector<int>& excess, int processes, vector<int>& ProcessorX, vector<int>& ProcessorY,
           vector<vector<int>>& weights, vector<vector<int>>& adjacencies, vector <vector <int>>& capacities, vector <vector <int>>& flow){

    return RelableToFront(excess, height, ProcessorX, ProcessorY, weights, processes, adjacencies, capacities, flow);
}



int main(){
    int output, processes, entries;

    vector <int> ProcessorX;
    vector <int> ProcessorY;
    vector <vector <int>> weights;
    vector <vector <int>> capacities;
    vector <vector <int>> adjacencies;
    vector <vector <int>> flow;
    vector <int> height;
    vector <int> excess;

    if(!scanf("%d %d", &processes, &entries)){
        cout << "ERROR" << endl;
    }

    processInput(processes, entries, ProcessorX, ProcessorY, weights, adjacencies, height, excess,
                 capacities, flow);

    output = Output(height, excess, processes, ProcessorX, ProcessorY, weights, adjacencies, capacities, flow);

    cout << output << endl;

    return 0;
}