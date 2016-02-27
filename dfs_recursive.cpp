#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

#define RET_OK          0
#define RET_NO_ROUTE    -1

/* max vertex count in graph */
#define MAX_VERTEX_COUNT 5

struct Node
{
    int v;
    int w;
    Node(int _v, int _w) : v(_v), w(_w){}
};

void PrintInteger(int value)
{
    cout << value;
}

struct Graph
{
    list<Node> neighbors;
};

Graph graph[MAX_VERTEX_COUNT];
bool  visited[MAX_VERTEX_COUNT];

//最短路径
vector<int> g_Path;

void UpdatePath(vector<int> & path)
{
    if (g_Path.empty() || path.size() < g_Path.size())
    {
        g_Path.clear();
        g_Path.insert(g_Path.begin(), path.begin(),path.end());
        return;
    }
}

int dfs(int start, int end, vector<int> &path)
{
    int ret = RET_OK;

    if (start == end)
    {
        cout << "Find path : ";
        for_each(path.begin(), path.end(), PrintInteger);
        cout << endl;
        UpdatePath(path);
        return RET_OK;
    }

    if (graph[start].neighbors.empty())
    {
        return RET_NO_ROUTE;
    }

    list<Node>::iterator neighbor = graph[start].neighbors.begin();
    while (neighbor != graph[start].neighbors.end())
    {
        if (!visited[neighbor->v])
        {
            visited[neighbor->v] = true;
            path.push_back(neighbor->v);
            dfs(neighbor->v, end, path);
            //遍历所有节点
            path.pop_back();
            visited[neighbor->v] = false;
        }
        neighbor++;
    }

    return RET_NO_ROUTE;
}

int main(void)
{
    graph[0].neighbors.push_back(Node(1, 1));
    graph[0].neighbors.push_back(Node(2, 1));
    graph[1].neighbors.push_back(Node(2, 2));
    graph[1].neighbors.push_back(Node(3, 3));
    graph[3].neighbors.push_back(Node(2, 5));
    graph[3].neighbors.push_back(Node(5, 1));
    graph[2].neighbors.push_back(Node(5, 3));
    graph[2].neighbors.push_back(Node(0, 3));

    int start = 0;
    int end   = 5;

    vector<int> path;
    visited[start] = true;
    path.push_back(start);
    dfs(start, end, path);
    if (g_Path.size() > 1)
    {
        cout << "Shortest path " << start << " -> " << end << " : ";
        for_each(g_Path.begin(), g_Path.end(), PrintInteger);
        cout << endl;
    }
    else
    {
        cout << "No route " << start << " -> " << end << endl;
    }

    return 0;
}