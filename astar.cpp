#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

#define MAP_WIDTH    10
#define MAP_HEIGHT   10

struct Axis
{
    int x;
    int y;
    Axis(int _x, int _y):x(_x), y(_y){}
};

enum NODE_TYPE
{
    NODE_FREE = 0,
    NODE_BARRIER,
    NODE_START,
    NODE_END,
};

struct Node
{
    int x;
    int y;
    int type; 
    Node* father;

    //int f;    // f = g + h
    int g;
    int h;

    bool isInOpenList;
    bool isInCloseList;

    Node(void){}
    Node(int _x, int _y, Node* _father, int _g, int _h):
    x(_x), y(_y), father(_father), g(_g), h(_h), isInOpenList(false), isInCloseList(false){}
};

int gMap[MAP_WIDTH][MAP_HEIGHT] = {
    // map 1, end = (4, 5)
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 1, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 3, 0, 0, 0, 0},
    {1, 1, 1, 0, 1, 1, 1, 1, 1, 0},
    {1, 1, 0, 0, 1, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 1, 0, 1, 0, 1, 0},
    {1, 1, 1, 0, 0, 0, 1, 0, 0, 0},
    // map 2, end = (4, 9)
    /*
    {2, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 1, 0, 1, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 1, 0, 1, 1},
    {0, 0, 0, 0, 1, 0, 1, 0, 1, 3},
    {1, 1, 1, 0, 1, 0, 1, 0, 1, 0},
    {1, 1, 0, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 1, 0, 1, 0},
    {1, 1, 1, 0, 0, 0, 1, 0, 1, 0},
    */
};

bool Greater(const Node* a, const Node* b)
{
    return ((a->g + a->h) > (b->g + b->h)) ? true : false;
}

void MapInit(int map[][MAP_HEIGHT], Node nodeMap[MAP_WIDTH][MAP_HEIGHT])
{
    memset(nodeMap, 0, sizeof(Node)*MAP_WIDTH*MAP_HEIGHT);
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            nodeMap[i][j].type = map[i][j];
            nodeMap[i][j].x = i;
            nodeMap[i][j].y = j;
        }
    }
}


void InsertStartIntoOpenList(vector<Node*>& openList, Node* start, Node* end)
{
    start->isInOpenList = true;
    start->g = 0;
    start->h = abs(start->x - end->x) + abs(start->y - end->y);
    openList.push_back(start);
    push_heap(openList.begin(), openList.end(), Greater);
}

void InsertIntoOpenList(Node nodeMap[MAP_WIDTH][MAP_HEIGHT], vector<Node*>& openList, 
                        Axis node, Node* current, Node* end)
{
    /* node is barrier */
    if (NODE_BARRIER == nodeMap[node.x][node.y].type)
    {
        return;
    }

    /* node has already been add into close list */
    if (nodeMap[node.x][node.y].isInCloseList)
    {
        return;
    }

    /* node has already been add into open list */
    if (nodeMap[node.x][node.y].isInOpenList)
    {
        /* if thep path form current node to  node is much shorter, update g of node */
        if (nodeMap[node.x][node.y].g > current->g + 1)
        {
            nodeMap[node.x][node.y].g      = current->g + 1;
            nodeMap[node.x][node.y].father = current;

            /* adjust the the heap of open list */
            make_heap(openList.begin(), openList.end(), Greater);
        }
        return;
    }

    /* if node is not in open list, add node into it */
    nodeMap[node.x][node.y].g = current->g + 1;
    nodeMap[node.x][node.y].h = abs(node.x - end->x) + abs(node.y - end->y);
    nodeMap[node.x][node.y].father = current;
    nodeMap[node.x][node.y].isInOpenList = true;
    openList.push_back(&nodeMap[node.x][node.y]);
    push_heap(openList.begin(), openList.end(), Greater);
}

void SearchNeighbors(Node map[][MAP_HEIGHT], vector<Node*>& openList, Node* current, Node* end)
{
    int x = current->x;
    int y = current->y;

    if ((x + 1) >= 0 && (x + 1) < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
    {
        InsertIntoOpenList(map, openList, Axis(x+1, y), current, end);
    } 

    if ((x - 1) >= 0 && (x - 1) < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
    {
        InsertIntoOpenList(map, openList, Axis(x-1, y), current, end);
    }

    if ((y + 1) >= 0 && (y + 1) < MAP_HEIGHT && x >= 0 && x < MAP_WIDTH)
    {
        InsertIntoOpenList(map, openList, Axis(x, y+1), current, end);
    }

    if ((y - 1) >= 0 && (y - 1) < MAP_HEIGHT && x >= 0 && x < MAP_WIDTH)
    {
        InsertIntoOpenList(map, openList, Axis(x, y-1), current, end);
    }
}

Node* GetCurrentNode(vector<Node*>& openList, vector<Node*>& closeList)
{
    pop_heap(openList.begin(), openList.end(), Greater);
    Node* current = openList.back();
    openList.pop_back();
    current->isInOpenList  = false;
    current->isInCloseList = true;
    closeList.push_back(current);
    //cout << current->g + current->h << "\t" << current->g << "\t" << current->h << endl;
    return current;
}

// Search path by A-Star
int FindPath(int map[][MAP_HEIGHT], Axis& start, Axis& end, list<Axis>& path)
{
    //open list and close list
    vector<Node*> openList;
    vector<Node*> closeList;
    Node nodeMap[MAP_WIDTH][MAP_HEIGHT];

    Node* current;
    Node* endNode;

    if (start.x == end.x && start.y == end.y)
    {
        cout << "Start is same with end.\n";
        return -1;
    }

    /* initalize map */
    MapInit(map, nodeMap);
    endNode = &nodeMap[end.x][end.y];

    /* add start node into open list */
    InsertStartIntoOpenList(openList, &nodeMap[start.x][start.y], endNode);

    /* begin to find path */
    while (true)
    {
        /* choose node whose f is smallest as the current node */
        current = GetCurrentNode(openList, closeList);
        if (current->x == endNode->x && current->y == endNode->y)
        {
            cout << "Find path success!:)\n";
            break;
        }

        /* add the current node's neighbors into open list */
        SearchNeighbors(nodeMap, openList, current, endNode);

        /* open list is empty that means we can't find path */
        if (openList.empty())
        {
            cout << "Cannot find path!:(\n";
            return -1;
        }
    }

    /* backtrack from end to start */
    current = endNode;
    while (current)
    {
        path.push_front(Axis(current->x, current->y));
        current = current->father;
    }

    return 0;
}

void MapPrint(void)
{
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            cout << gMap[i][j] << " ";
        }
        cout << endl;
    }
}

int main(void)
{
    Axis start(0, 0);
    Axis end(4, 5);   // end for map 1
    //Axis end(4, 9);     // end for map 2
    list<Axis> path;

    MapPrint();

    int ret = FindPath(gMap, start, end, path);
    if (ret != 0)
    {
        return -1;
    }

    list<Axis>::iterator it  = path.begin();
    it++;
    for (; it != path.end(); it++)
    {
        cout << "(" << it->x << "," << it->y << ")";
        gMap[it->x][it->y] = 8;
    }
    cout << endl;

    MapPrint();
    
    return 0;
}