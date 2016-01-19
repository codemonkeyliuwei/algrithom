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
    bool isVisited;
    Node(int _v, int _w) : v(_v), w(_w), isVisited(false){}
};

struct Graph
{
    list<Node> neighbors;
};

Graph graph[MAX_VERTEX_COUNT];
bool  visited[MAX_VERTEX_COUNT];

int dfs(int start, int end, vector<int> &path)
{
    int ret = RET_OK;

    if (start == end)
    {
        cout << "Find it!\n";
        return RET_OK;
    }

    list<Node>::iterator neighbor = graph[start].neighbors.begin();
    while (neighbor != graph[start].neighbors.end())
    {
        if (!visited[neighbor->v])
        {
            visited[neighbor->v] = true;
            path.push_back(neighbor->v);
            ret = dfs(neighbor->v, end, path);
            if (ret != RET_OK)
            {
                /* remove v from path if cannot arrive end pass v */
                path.pop_back();
            }
            else
            {
                return RET_OK;
            }
        }
        neighbor++;
    }

    return RET_NO_ROUTE;
}

void PrintInt(int value)
{
    cout << value ;
}

int main(void)
{
    vector<int> path;
    graph[0].neighbors.push_back(Node(2, 1));
    graph[0].neighbors.push_back(Node(1, 1));
    graph[1].neighbors.push_back(Node(2, 2));
    graph[1].neighbors.push_back(Node(3, 3));
    graph[3].neighbors.push_back(Node(2, 5));

    path.push_back(0);
    dfs(0, 3, path);

    for_each(path.begin(), path.end(), PrintInt);
    return 0;
}