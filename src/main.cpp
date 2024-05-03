#include <iostream>
#include <vector>
#include <list>
#include <set>

using namespace std;

class edgeCell{
public:
    int index;
    vector<edgeCell*> children;
    edgeCell* parent;
    
    edgeCell(int _index, edgeCell* _parent){
        index = _index;
        parent = _parent;
    }
};
bool inSet(set<int> Set, int num){
    return Set.find(num)!=Set.end();
}
void BFS(set<int> &H, set<int> &S, set<int> &NhS, vector<bool> &paired, set<int> &chosenEdges, vector<vector<int>> &N, vector<vector<int>>& Connected, int n, int m, int current){
    list<int> Q;
    Q.push_back(current);
    
    list<edgeCell*> nextRoots;
    edgeCell* root = new edgeCell(current, nullptr);
    edgeCell* v = root;
    
    while(!Q.empty()){
        current = Q.front();
        Q.pop_front();
        S.insert(current);
        
        if(!nextRoots.empty()){
            root = nextRoots.front();
            nextRoots.pop_front();
        }
        
        for(auto &j: H){
            if(Connected[j][0]==current && !inSet(NhS, Connected[j][1])){
                NhS.insert(Connected[j][1]);
                v = new edgeCell(j,root);
                root->children.push_back(v);
                nextRoots.push_back(v);
                
                if(!paired[Connected[j][1]]){
                    paired[Connected[j][1]] = true;
                    while(paired[Connected[v->index][0]]){
                        for(auto &k: chosenEdges){
                            if(Connected[k][0] == Connected[v->index][0]){
                                chosenEdges.erase(k);
                                chosenEdges.insert(v->index);
                                break;
                            }
                        }
                        v = v->parent;
                    }
                    chosenEdges.insert(v->index);
                    paired[Connected[v->index][0]] = true;

                    Q.clear();
                    break;
                }
                for(auto &k: chosenEdges){
                    if(Connected[k][1] == Connected[j][1] && !inSet(S,Connected[k][0])){
                        Q.push_back(Connected[k][0]);
                        break;
                    }
                }
            }
        }
    }
}

bool perfectMatch(set<int> &H, set<int> &chosenEdges, set<int> &S, set<int> &NhS, vector<vector<int>> &N, vector<vector<int>>& Connected, int n, int m){
    vector<bool> paired;
    paired.resize(2*n, false);
    chosenEdges.clear();
    //pareamento guloso
    for(auto &j: H){
        if(!paired[Connected[j][1]] && !paired[Connected[j][0]]){
            paired[Connected[j][0]] = true;
            paired[Connected[j][1]] = true;
            chosenEdges.insert(j);
        }
    }
    
    for(int i = 0; i < 2*n; i++){
        if(!paired[i]){
            S.clear();
            NhS.clear();
            BFS(H,S,NhS,paired,chosenEdges,N,Connected,n,m, i);
            if(!paired[i])
                return false;
        }
    }
    return true;
}


int main() {
    
    int n,m;
    cin >> n >> m;
    vector<vector<int>> N(2*n, vector<int>(m));
    vector<int> c(m);
    
    for(int i = 0; i<2*n; i++){
        for(int j = 0; j<m; j++){
            cin >> N[i][j];
        }
    }
    
    for(int j = 0; j<m; j++)
        cin >> c[j];
    
    float min = *min_element(c.begin(),c.end());
    vector<float> y;
    y.resize(2*n, min/2);
    
    vector<vector<int>> Connected(m, vector<int>(2));
    
    for(int j = 0; j<m; j++){
        for(int i = 0; i<2*n; i++){
            if(i<n && N[i][j]==1){
                Connected[j][0] = i;
                i = n-1;
            }
            else if(N[i][j] == 1){
                Connected[j][1] = i;
                break;
            }
        }
    }
    
    set<int> H;//Edges
    set<int> S;//Vertices
    set<int> NgS;
    set<int> NhS;
    set<int> pairEdges;
    
    float epsilon = -1;
    bool solution;
    
    while(1){
        //Redefine H(step 2)
        H.clear();
        for(int j = 0; j < m; j++){
            if(y[Connected[j][0]] + y[Connected[j][1]] == c[j])
                H.insert(j);
        }
        //(step 3)
        S.clear();
        NhS.clear();
        pairEdges.clear();
        if(perfectMatch(H, pairEdges, S, NhS, N, Connected, n, m)){
            solution = true;
            break;
        }
        //Find Neighbors of S in G and check size(step 5)
        NgS.clear();
        for(auto& i: S){
            for(int j = 0; j < m; j++){
                if(N[i][j] == 1)
                    NgS.insert(Connected[j][1]);
            }
        }
        if(NgS.size()<S.size()){
            solution = false;
            break;
        }
        //Find epsilon(step 6)
        epsilon = -1;
        for(int j = 0; j < m; j++){
            if(!inSet(H,j) && (epsilon == -1 || epsilon > c[j]-y[Connected[j][0]]-y[Connected[j][1]])){
                if(inSet(S,Connected[j][0]) && inSet(NgS,Connected[j][1]))
                    epsilon = c[j]-y[Connected[j][0]]-y[Connected[j][1]];
            }
        }
        for(auto &i: NhS)
            y[i]-=epsilon;
        for(auto &i: S)
            y[i]+=epsilon;
    }
    
    if(solution){
        float sum = 0;
        for(auto &j : y){
            sum += j;
        }
        cout << sum << endl;
        for(int j = 0; j < m; j++){
            if(inSet(pairEdges,j))
                cout << 1 << " ";
            else
                cout << 0 << " ";
        }
        cout << endl;
        for(auto &j : y){
            cout << j << " ";
        }
    }
    else{
        cout << -1 << endl;
        for(int i = 0; i < n; i++){
            if(inSet(S,i))
                cout << 1 << " ";
            else
                cout << 0 << " ";
        }
        cout << endl;
        for(int i = n; i < 2*n; i++){
            if(inSet(NgS,i))
                cout << 1 << " ";
            else
                cout << 0 << " ";
        }
        cout << endl;
    }
    
    return 0;
}
