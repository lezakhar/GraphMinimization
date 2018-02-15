#include <iostream>
#include <cstdlib>
#include <fstream>
#include <map>
#include <vector>
using namespace std;


//структура ребра(вес и следующий узел)
struct t_node{
    double weight;
    int nodeb;
};


    map <int, bool> used;
    map <int, bool> used_vertex;
    map <int , vector<t_node> > g;// наш граф(это ассоциативный массив ключи(инты) это у него вершины из которых стрелка, а значения массив узлов, где значения массива это вес ребра и следующий узел, ну короче получается список смежности)
    vector<int> keys;//массив вершин
    double cur_weight;//текущий вес
    int cur_edge_before;
    int cur_edge;
    int cur_vertex;
    int cur_node;//текущая вершина
    int coun;//колиество ребер который алгоритм(поиск в глубину) обошёл
    int first_point;//стартовая вершина (нужна чтобы найти цикл)
    bool cycle = false;//флаг цикла


        //заполнение графа
    void input_graph(){
        ifstream fin("test3.txt");
        int point1 = 0;
        int point2 = 0;
        cur_vertex = 0;
        double weight = 0;
        t_node node;
        while(!fin.eof()){
            fin >> point1;//вершина из которой
            fin >> point2;//вершина в которую
            if(g[point1].size() == 0){
                keys.push_back(point1);//все вершины
            }
            fin >> weight;//вес
            node.nodeb = point2;//вершина в которую
            node.weight = weight;//вес ребра
            g[point1].push_back(node);//добавляем ребро в ассоциативный массив
            if(!used_vertex[point1]){
                cur_vertex++;
                used_vertex[point1] = true;
            }
            if(!used_vertex[point2]){
                cur_vertex++;
                used_vertex[point2] = true;
            }
            cur_weight+=weight;//общий вес
            cur_edge_before++;
        }
        cout << "coun of vertex(original) = " << cur_vertex << endl;
        cout << "coun of edges(original) = " << cur_edge_before << endl;
        cout << "coun of weight(original) = " << cur_weight << endl;//это для проверки веса
    }

    //проверка
    void check(){
        cur_weight = 0;
        cur_edge = 0;
        int coun_vertex = 0;
        for (unsigned int i=0; i<keys.size(); ++i){
            used[keys[i]] = false;
                for(unsigned int j = 0; j < g[keys[i]].size(); ++j)
                    used[g[keys[i]][j].nodeb] = false;
        }
        for(unsigned int i = 0; i < keys.size(); ++i){
                if(g[keys[i]].size() != 0){
                    cout << "degree of vertex: " << keys[i] << " = " << g[keys[i]].size() << endl;
                    for(unsigned int j = 0; j < g[keys[i]].size(); ++j){
                        if(!used[keys[i]]){
                            coun_vertex++;
                        }
                        used[keys[i]] = true;
                        if(!used[g[keys[i]][j].nodeb]){
                            coun_vertex++;
                        }
                        used[g[keys[i]][j].nodeb] = true;
                        cout << keys[i] << " - > ";
                        cout << g[keys[i]][j].nodeb << " ";
                        cout << g[keys[i]][j].weight << endl;
                        cur_weight+=g[keys[i]][j].weight;
                        ++cur_edge;
                    }
                }
        }
        cout << "coun of vertex(transformed) = " << coun_vertex << endl;
        cout << "coun of edges(transformed) = " << cur_edge << endl;
        cout << "coun of weight(transformed) = " << cur_weight << endl;
    }



    //основная часть минимизирующей функции (модифицированный поиск в глубину)
    void dfs(int v) {
        if ((g[v].size() > 1)&&(coun != 0))
            return;
        used[v] = true;
        for (size_t i=0; i<g[v].size(); ++i) {
            int to = g[v][i].nodeb;
            coun++;
            if (! used[to]){
                cur_weight += g[v][i].weight;
                cur_node = g[v][i].nodeb;
                dfs (to);
                if(!cycle){
                    if(coun == 1){
                        if(g[v].size() == 0)
                            used[v] = false;
                        g[v][i].weight = cur_weight;
                        g[v][i].nodeb = cur_node;
                        cur_weight = 0;
                    }else{
                        g[v].erase(g[v].begin() + i);
                    }
                }else{
                    cur_node = v;
                    cur_weight-=g[v][i].weight;
                }
            }
            coun--;
            if(first_point == to){
                cycle = true;
            }else{
                cycle = false;
            }
        }
    }

    //функция которая минимизирует(тоесть все линейные участки преобразует в ребро, с соответствующим весом)
    void min_graph() {
        for (unsigned int i=0; i<keys.size(); ++i)
            used[keys[i]] = false;
        for (unsigned int i=0; i<keys.size(); ++i){
            if (! used[keys[i]]) {
                cur_weight = 0;
                coun = 0;
                first_point = keys[i];
                dfs (first_point);//рекурсия
            }
        }
    }

int main(){
    input_graph();
    min_graph();
    check();
    return 0;
}
