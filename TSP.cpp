/*
Genetic Algorithm for Traveling Salesman Problem (TSP)
Crossover - (Source/Destination swapped between Parents for Children)
Mutation - (Swapping any 2 cities in between the Source/Destination)
*/
#include<bits/stdc++.h>
using namespace std;
//Storing the city to city distances and (population + children) globally
vector<int> CityD[100000];
vector<vector<int>> rgsolns;
vector<vector<int>> children;
//Crossover function
void crossover(int n){
    srand(time(0));
    map<int,int> pused;
    int chd = 0;
    if(n%2){
        int ind = rand()%n;
        children.push_back(rgsolns[ind]);
        pused[ind] = 1;
        chd++;
    }
    while(chd<n){
        int i = rand()%n, j = rand()%n;
        if(!pused[i]&&!pused[j]&&i!=j){
            int st1 = rgsolns[i][0];
            int st2 = rgsolns[j][0];
            vector<int> chd1;
            vector<int> chd2;
            for(int z=0;z<=n;z++){
                if(z==0 || z==n){
                    chd1.push_back(st2);
                    chd2.push_back(st1);
                }
                else{
                    if(rgsolns[i][z]==st2){
                        chd1.push_back(st1);
                    }
                    else{
                        chd1.push_back(rgsolns[i][z]);
                    }
                    if(rgsolns[j][z]==st1){
                        chd2.push_back(st2);
                    }
                    else{
                        chd2.push_back(rgsolns[j][z]);
                    }
                }
            }
            children.push_back(chd1);
            children.push_back(chd2);
            chd+=2;
        }
    }
    return;
}
//Mutation function (5% chance)
void mutation(int n){
    srand(time(0));
    double chk = 0.05*2*n;
    int mut = (int)chk;
    map<int,int> mp;
    map<int,int> mc;
    if(mut>=1){
        int v = 0;
        while(v<mut){
            int i = (rand()%(n-1)) + 1, j = (rand()%(n-1)) + 1, poc = rand()%2, ind = rand()%n;
            if(poc){
                if(!mp[ind]){
                    int sw = rgsolns[ind][i];
                    rgsolns[ind][i] = rgsolns[ind][j];
                    rgsolns[ind][j] = sw;
                    mp[ind] = 1;
                    v++;
                }
            }
            else{
                if(!mc[ind]){
                    int sw = children[ind][i];
                    children[ind][i] = children[ind][j];
                    children[ind][j] = sw;
                    mc[ind] = 1;
                    v++;
                }
            }
        }
    }
    return;
}
//Generating new population to send into the next iteration via fitness (i.e distance)
vector<vector<int>> newpopulation(int n){
    vector<vector<int>> np;
    int full = 0;
    int inf = 999999999;
    map<int,int> usedP;
    map<int,int> usedC;
    while(full < n){
        int i1,i2,minD1 = inf, minD2 = inf;
        for(int i=0;i<n;i++){
            int dist = 0;
            if(!usedP[i]){
                for(int j=0;j<n;j++){
                    dist+=CityD[rgsolns[i][j]][rgsolns[i][j+1]];
                }
                if(dist<minD1){
                    minD1 = dist;
                    i1 = i;
                }
            }
        }
        for(int i=0;i<n;i++){
            int dist = 0;
            if(!usedC[i]){
                for(int j=0;j<n;j++){
                    dist+=CityD[children[i][j]][children[i][j+1]];
                }
                if(dist<minD2){
                    minD2 = dist;
                    i2 = i;
                }
            }
        }
        if(minD1<minD2){
            np.push_back(rgsolns[i1]);
            usedP[i1] = 1;
        }
        else{
            np.push_back(children[i2]);
            usedC[i2] = 1;
        }
        full++;
    }
    return np;
}
//random permutation of city order to start the iteration and we generate 'n' permutations to begin with
void rgp(int n){
    srand(time(0));
    int i = 0;
    while(i<n){
        vector<int> soln;
        map<int,int> used;
        int f = rand()%n;
        soln.push_back(f);
        used[f] = 1;
        int k = 1;
        while(k<n){
            int num = rand()%n;
            if(used[num]){
                continue;
            }
            else{
                k++;
                soln.push_back(num);
                used[num] = 1;
            }
        }
        soln.push_back(f);
        rgsolns.push_back(soln);
        i++;
    }
    return;
}
int main(){
    //User input for distance between cities
    int n;
    cout << "Enter the number of cities: ";
    cin >> n;
    for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(i==j){
                    CityD[i].push_back(0);
                }
                else{
                    int d;
                    cout << "Enter distance between City" << i + 1 << " and City" << j + 1 <<": ";
                    cin >> d;
                    CityD[i].push_back(d);
                }
            }
    }
    //Sending the population into 400 iterations to get the fittest population with least distance (sub-optimal answer)
    rgp(n);
    for(int i=0;i<400;i++){
        children.clear();
        crossover(n);
        mutation(n);
        vector<vector<int>> popu = newpopulation(n);
        rgsolns.clear();
        rgsolns = popu;
    }
    //Printing the answer from the algorithm
    cout << "The optimal path via the GA is from cities in order: ";
    for(int i=0;i<n+1;i++){
        cout << rgsolns[0][i] + 1 << " ";
    }
    int ansD = 0;
    for(int i=0;i<n;i++){
        ansD+=CityD[rgsolns[0][i]][rgsolns[0][i+1]];
    }
    cout << "\nThe optimal distance is: " << ansD << "\n";
    return 0;
}
