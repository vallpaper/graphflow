/* 
 * Dinic's algorithm
 */

#include <cstdlib>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <deque>

using namespace std;

/*struct for edge*/
struct edge {
    /*capacity*/
    unsigned int flow;
    /*blocking flow[flow - used flow]*/
    int block;
};

/*struct for vertex*/
struct nod {
    /*distance from source*/
    unsigned int distance;
    /*flag for flood fill*/
    char visited; /*'f' - fresh, 'o' - open, 'c' - closed in floodFillWidth*/
};

struct graph {
    nod* vertex;
    edge** matrix;
    int n;
};

void deleteGraph(graph * g1) {
    for (int i = 0; i < g1->n; i++) {
        delete[] g1->matrix[i];
    }
    delete[] g1->matrix;
    delete[] g1->vertex;
    delete g1;
}

//void printGraph(graph * g1) {
//    cout << g1->n << endl;
//
//    for (int i = 0; i < g1->n; i++) {
//        for (int j = 0; j < g1->n; j++) {
//            cout << g1->matrix[i][j].block << "/" << g1->matrix[i][j].flow << " ";
//        }
//        cout << endl;
//    }
//}

graph* readMatrix(fstream & file) {

    graph * g1 = new graph();
    /*load size of matrix for function*/
    file >> g1->n;

    if (file.fail()) {
        cout << "Read error." << endl;
        exit(0);
    }

    int temporary = 0;
    /*allocation of graph*/
    g1->vertex = new nod[g1->n];
    g1->matrix = new edge*[g1->n];
    for (int i = 0; i < g1->n; i++) {
        g1->matrix[i] = new edge[g1->n];
    }

    for (int i = 0; i < g1->n; i++) {
        /*vertex first init, sets distance to intMax*/
        g1->vertex[i].distance = (unsigned int) - 1;
        /*'a' = invalid value*/
        g1->vertex[i].visited = 'a';
        /*set rows in matrix of edges*/
        for (int j = 0; j < g1->n; j++) {
            file >> temporary;
            if (file.fail()) {
                cout << "Read error." << endl;
                exit(0);
            }
            g1->matrix[i][j].block = 0;
            g1->matrix[i][j].flow = temporary;
        }
    }
    return g1;
}

/*flag = {w,d} where w = width first, d = depth first*/
int floodFillWidth(graph * g1, int source, int target) {
    deque<int> q;
    int i;
    for (i = 0; i < g1->n; i++) {
        g1->vertex[i].visited = 'f';
        g1->vertex[i].distance = (unsigned int) - 1;
    }
    g1->vertex[source].distance = 0;
    g1->vertex[source].visited = 'o';
    /*source to deque*/
    q.push_back(source);
    while (!q.empty()) {
        i = q.front();
        q.pop_front();
        /*used vertex set to close*/
        g1->vertex[i].visited = 'c';
        for (int j = 0; j < g1->n; j++) {
            /*edge missing or flow is full*/
            if (g1->matrix[i][j].flow - g1->matrix[i][j].block == 0) continue;
            /*if vertex has cycle*/
            if (g1->vertex[j].visited != 'f') continue;
            g1->vertex[j].visited = 'o';
            /*distance [y.dist->x.dist + 1]*/
            g1->vertex[j].distance = g1->vertex[i].distance + 1;
            /*put new open vertex to deque*/
            q.push_back(j);
            /*if we reach the end*/
            if (j == target) return g1->vertex[j].distance;
        }
        if (i == target) return g1->vertex[i].distance;
    }
    /*if we don't have flow*/
    return 0;
}

graph* createGraph(int n) {
    nod* vertex = new nod[n];
    edge **matrix = new edge*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new edge[n];
    }

    /*final structure*/
    graph * g1 = new graph();
    g1->matrix = matrix;
    g1->vertex = vertex;
    g1->n = n;

    for (int i = 0; i < n; i++) {
        /*vertex first init, sets distance to intMax*/
        vertex[i].distance = (unsigned int) - 1;
        /*'a' = invalid value*/
        vertex[i].visited = 'a';
        for (int j = 0; j < n; j++) {
            /*found new edge*/
            matrix[i][j].block = 0;
            matrix[i][j].flow = 0;
        }
    }
    return g1;
}

void clearGraph(graph*g2, int target) {
    bool flag;
    /*temporary array of flow*/
    int * flowSum = new int[g2->n];
    deque<int> q;
    int i, j;
    for (i = 0; i < g2->n; i++) {
        flowSum[i] = 0;
        for (j = 0; j < g2->n; j++) {
            /*sum of all out flows */
            if (g2->vertex[i].distance < g2->vertex[j].distance) {
                flowSum[i] += g2->matrix[i][j].flow;
            } else {
                /*delete edge*/
                g2->matrix[i][j].flow = 0;
            }
        }/*not target, without flow*/
        if (flowSum[i] == 0 && i != target) q.push_back(i);
    }
    
    /*dead end vertexes*/
    while (!q.empty()) {
        j = q.front();
        q.pop_front();
        /*find and clean dead vertexes if they are not target*/
        for (i = 0; i < g2->n; i++) {
            flag = (g2->matrix[i][j].flow != 0);
            flowSum[i] = flowSum[i] - g2->matrix[i][j].flow;
            g2->matrix[i][j].flow = 0;
            /*check if new dead vertexes created*/
            if (flowSum[i] == 0 && i != target && flag) {
                q.push_back(i);
            }
        }
    }
    delete[] flowSum;
}

int findFlow(graph * g2, int routeDist, int source, int target) {
    /*part of final flow*/
    int acc = 0;
    unsigned int flow;
    deque<int> q;
    /*break occur*/
    bool broken;
    /*when we need back step from dead end vertex*/
    bool up = false;
    /*k = route length*/
    int i, j, k;
    
    while (1) {
        up = false;
        flow = (unsigned int) - 1;
        /*find route and minimal flow on it*/
        for (i = source, k = 0; i != target && k < routeDist; k++) {
            broken = false;
            j = 0;
            if (up) {
                //          cout << "up:  " << q.size() << " ";
                /*throw flow*/
                q.pop_back();
                /*end of edge, relocate edge to next vertex*/
                j = q.back() + 1;
                q.pop_back();
                /*start of edge*/
                i = q.back();
                q.pop_back();
                /*route minimal flow from start to i*/
                flow = q.back();
                up = false;
                //        cout << i << " " << j << " " << flow << endl;
            }
         
            /*search minimum reserve and find out route*/
            for (; j < g2->n; j++) {
                /*find first route*/
                if (g2->matrix[i][j].flow - g2->matrix[i][j].block > 0) {
                    /*if flow - block is greater than flow, flow =flow*/
                    flow = (flow < g2->matrix[i][j].flow - g2->matrix[i][j].block) ?
                            flow : g2->matrix[i][j].flow - g2->matrix[i][j].block;
                    //          cout << "down:" << q.size() << " ";
                    q.push_back(i);
                    q.push_back(j);
                    q.push_back(flow);
                    //        cout << i << " " << j << " " << flow << endl;
                    i = j;
                    broken = true;
                    break;
                }
            }
            if (!broken) {
                /*no ore route*/
                if (q.empty()) break;
                /* k-2 because up and increment*/
                k -= 2;
                up = true;
            }
        }
        
        /*we used route*/
        if (q.empty()) break;
        if (flow == (unsigned int) - 1) break;
        /*route from source to target*/
        while (!q.empty()) {
            i = q.front();
            q.pop_front();
            j = q.front();
            q.pop_front();
            q.pop_front();
            /*use minimal flow on route*/
            g2->matrix[i][j].block += flow;
        }
        acc += flow;
    }
    return acc;
}

void copyFlow(graph * g1, graph * g2) {
    for (int i = 0; i < g1->n; i++) {
        for (int j = 0; j < g1->n; j++) {
            if (g2->matrix[i][j].block > 0 && g1->matrix[j][i].block > 0) {
                if (g2->matrix[i][j].block < g1->matrix[j][i].block) {
                    g1->matrix[j][i].block -= g2->matrix[i][j].block;
                } else {
                    g1->matrix[i][j].block = g1->matrix[i][j].block
                            + g2->matrix[i][j].block - g1->matrix[j][i].block;
                    g1->matrix[j][i].block = 0;
                }
            } else{
                g1->matrix[i][j].block += g2->matrix[i][j].block;
            }
        }
    }
}

void remakeGraph(graph * g1, graph * g2) {
    for (int i = 0; i < g1->n; i++) {
        g2->vertex[i].distance = g1->vertex[i].distance;
        g2->vertex[i].visited = g1->vertex[i].visited;
        for (int j = 0; j < g1->n; j++) {
            /*flow limit -> flow [x,y] =+ flow[x,y] - block[x,y] + block[y,x]  */
            g2->matrix[i][j].flow = g1->matrix[i][j].flow - g1->matrix[i][j].block + g1->matrix[j][i].block;
            g2->matrix[i][j].block = 0;
        }
    }
}

void dinicAlgorithm(fstream & file) {

    /*max flow*/
    int maxFlow = 0;
    /*flow of last loop*/
    int flow = 0;
    /*source target distance*/
    int routeDist = 0;
    /*source and target init, sets target after read N*/
    int source = 0, target = 0;

    /*graph from file*/
    graph * g1;
    /*read from file*/
    g1 = readMatrix(file);
    /*set level graph*/
    graph * g2 = createGraph(g1->n);
    /*set target to n-1*/
    target = g1->n - 1;

    //printGraph(g1);

    do {
        flow = 0;
        /*copy from g1 to g2*/
        remakeGraph(g1, g2);
        /*search route distance from source to target*/
        routeDist = floodFillWidth(g2, source, target);
        /*clear dead end vertexes and [x->y, x<-y] edges */
        clearGraph(g2, target);

        //printGraph(g2);

        /*find parts of final flow for routeDist*/
        flow = findFlow(g2, routeDist, source, target);
        
        maxFlow += flow;
        
        copyFlow(g1, g2);
        //printGraph(g1);
    } while (flow != 0);

    cout << "flow = " << maxFlow << endl;
    deleteGraph(g1);
    deleteGraph(g2);
}

int main(int argc, char** argv) {

    fstream file;

    if (argc != 2) {
        cout << "Invalid number of arguments." << endl;
        return 0;
    }

    file.open(argv[1], std::fstream::in);
    if (file.fail()) {
        cout << "Unable to read file." << endl;
        return 0;
    }
    cout << "File detected." << endl;
    /*jump to alhorithm*/
    dinicAlgorithm(file);
    file.close();

    return 0;
}
