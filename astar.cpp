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
    Axis(void):x(0), y(0){}
    Axis(int _x, int _y):x(_x), y(_y){}
};

enum MoveDir
{
    MOVE_UP   = 0,
    MOVE_DOWN = 1,
    MOVE_LEFT = 2,
    MOVE_RIGHT= 3,
};

enum NODE_TYPE
{
    NODE_EMPTY    = 0,
    NODE_BARRIER  = 1,
    NODE_START    = 2,
    NODE_END      = 3,
    NODE_CLOUD    = 4,
    NODE_TUNNEL   = 5,
    NODE_WORMHOLE = 6,
};

struct AStarNode
{
    int x;
    int y;
    NODE_TYPE type; 
    AStarNode* father;

    //int f;    // f = g + h
    int g;
    int h;

    bool isInOpenList;
    bool isInCloseList;

    MoveDir dir;    // when type is NODE_TUNNEL
    Axis peer;      // type is NODE_WORMHOLE

    AStarNode(void){}
    AStarNode(int _x, int _y, AStarNode* _father, int _g, int _h):
    x(_x), y(_y), father(_father), g(_g), h(_h), isInOpenList(false), isInCloseList(false){}
};

int g_map[MAP_WIDTH][MAP_HEIGHT] = {
    // _map 1, end = (4, 5)
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 1, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 4, 1, 1, 0, 1, 1, 1},
    {0, 0, 0, 4, 4, 3, 0, 0, 0, 0},
    {1, 1, 1, 0, 1, 1, 1, 1, 1, 0},
    {1, 1, 0, 0, 1, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 1, 0, 1, 0, 1, 0},
    {1, 1, 1, 0, 0, 0, 1, 0, 0, 0},
    // _map 2, end = (4, 9)
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

bool Greater(const AStarNode* a, const AStarNode* b)
{
    return ((a->g + a->h) > (b->g + b->h)) ? true : false;
}

void MapInit(int _map[][MAP_HEIGHT], AStarNode node_map[MAP_WIDTH][MAP_HEIGHT])
{
    memset(node_map, 0, sizeof(AStarNode)*MAP_WIDTH*MAP_HEIGHT);
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            node_map[i][j].type = (NODE_TYPE)_map[i][j];
            node_map[i][j].x = i;
            node_map[i][j].y = j;
        }
    }
}


void InsertStartIntoOpenList(vector<AStarNode*>& openList, AStarNode* start, AStarNode* end)
{
    start->isInOpenList = true;
    start->g = 0;
    start->h = abs(start->x - end->x) + abs(start->y - end->y);
    openList.push_back(start);
    push_heap(openList.begin(), openList.end(), Greater);
}

bool isBarrierAStarNode(AStarNode & node)
{
    return node.type == NODE_BARRIER;
}

bool isEmptyAStarNode(AStarNode & node)
{
    return node.type == NODE_EMPTY;
}

bool isCloudAStarNode(AStarNode & node)
{
    return node.type == NODE_CLOUD;
}

bool isTunnelAStarNode(AStarNode & node)
{
    return node.type == NODE_TUNNEL;
}

bool isTunnelInPassable(AStarNode & current, AStarNode & tunnelAStarNode)
{
    return true;
}

bool isTunnelOutPassable(AStarNode & tunnelAStarNode, AStarNode &to)
{
    return true;
}

#define UNREACHABLE_WEIGHT  MAP_WIDTH * MAP_HEIGHT

int GetAStarNodeWeight(AStarNode & from, AStarNode & to)
{
    // anyone -> barrier or barrier -> anyone
    if (isBarrierAStarNode(from) || isBarrierAStarNode(to))
    {
        return UNREACHABLE_WEIGHT;
    }

    // empty/cloud -> empty:1
    // empty/cloud -> cloud:2
    // empty/cloud -> tunnel:1
    // empty/cloud -> tunnel相反方向:UNREACHABLE_WEIGHT
    // empty/cloud -> wormhole: 1
    if (isEmptyAStarNode(from)
     || isCloudAStarNode(from))
    {
        switch (to.type)
        {
        case NODE_EMPTY:
        case NODE_WORMHOLE:
            return 1;
        case NODE_CLOUD:
            return 2;
        case NODE_TUNNEL:
            return isTunnelInPassable(from, to)
                ? 1 : UNREACHABLE_WEIGHT;
        }
        return UNREACHABLE_WEIGHT;
    }

    // tunnel -> tunnel next: 0
    // tunnel -> non tunnel next:UNREACHABLE_WEIGHT
    if (isTunnelAStarNode(from))
    {
        return isTunnelOutPassable(from, to) 
            ? 0 : UNREACHABLE_WEIGHT;
    }

    //TODO: log here
    return MAP_WIDTH * MAP_HEIGHT;
}

void InsertIntoOpenList(AStarNode node_map[MAP_WIDTH][MAP_HEIGHT], 
                        vector<AStarNode*>& openList, 
                        Axis node,
                        AStarNode* current, 
                        AStarNode* end)
{
    /* node is barrier */
    if (NODE_BARRIER == node_map[node.x][node.y].type)
    {
        return;
    }

    /* node has already been add into close list */
    if (node_map[node.x][node.y].isInCloseList)
    {
        return;
    }

    int weight = GetAStarNodeWeight(*current, node_map[node.x][node.y]);
    //int weight = 1;

    /* node has already been add into open list */
    if (node_map[node.x][node.y].isInOpenList)
    {
        /* if the path form current node to  node is much shorter, update g of node */
        if (node_map[node.x][node.y].g > current->g + weight)
        {
            node_map[node.x][node.y].g      = current->g + weight;
            node_map[node.x][node.y].father = current;

            /* adjust the the heap of open list */
            make_heap(openList.begin(), openList.end(), Greater);
        }
        return;
    }

    /* if node is not in open list, add node into it */
    node_map[node.x][node.y].g = current->g + weight;
    node_map[node.x][node.y].h = abs(node.x - end->x) + abs(node.y - end->y);
    node_map[node.x][node.y].father = current;
    node_map[node.x][node.y].isInOpenList = true;
    openList.push_back(&node_map[node.x][node.y]);
    push_heap(openList.begin(), openList.end(), Greater);
}

void SearchNeighbors(AStarNode _map[][MAP_HEIGHT],
                     vector<AStarNode*>& openList, 
                     AStarNode* current, 
                     AStarNode* end)
{
    int x = current->x;
    int y = current->y;

    //right
    if ((x + 1) >= 0 && (x + 1) < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
    {
        InsertIntoOpenList(_map, openList, Axis(x+1, y), current, end);
    } 

    //left
    if ((x - 1) >= 0 && (x - 1) < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
    {
        InsertIntoOpenList(_map, openList, Axis(x-1, y), current, end);
    }

    //down
    if ((y + 1) >= 0 && (y + 1) < MAP_HEIGHT && x >= 0 && x < MAP_WIDTH)
    {
        InsertIntoOpenList(_map, openList, Axis(x, y+1), current, end);
    }

    //up
    if ((y - 1) >= 0 && (y - 1) < MAP_HEIGHT && x >= 0 && x < MAP_WIDTH)
    {
        InsertIntoOpenList(_map, openList, Axis(x, y-1), current, end);
    }
}

AStarNode* GetCurrentAStarNode(vector<AStarNode*>& openList, vector<AStarNode*>& closeList)
{
    pop_heap(openList.begin(), openList.end(), Greater);
    AStarNode* current = openList.back();
    openList.pop_back();
    current->isInOpenList  = false;
    current->isInCloseList = true;
    closeList.push_back(current);
    //cout << current->g + current->h << "\t" << current->g << "\t" << current->h << endl;
    return current;
}

// Search path by A-Star
int FindPath(int _map[][MAP_HEIGHT], Axis& start, Axis& end, list<Axis>& path)
{
    //open list and close list
    vector<AStarNode*> openList;
    vector<AStarNode*> closeList;
    AStarNode node_map[MAP_WIDTH][MAP_HEIGHT];

    AStarNode* current;
    AStarNode* endAStarNode;

    if (start.x == end.x && start.y == end.y)
    {
        cout << "Start is same with end.\n";
        return -1;
    }

    /* initalize _map */
    MapInit(_map, node_map);
    endAStarNode = &node_map[end.x][end.y];

    /* add start node into open list */
    InsertStartIntoOpenList(openList, &node_map[start.x][start.y], endAStarNode);

    /* begin to find path */
    while (true)
    {
        /* choose node whose f is smallest as the current node */
        current = GetCurrentAStarNode(openList, closeList);
        if (current->x == endAStarNode->x && current->y == endAStarNode->y)
        {
            cout << "Find path success!:)\n";
            break;
        }

        /* add the current node's neighbors into open list */
        SearchNeighbors(node_map, openList, current, endAStarNode);

        /* open list is empty that means we can't find path */
        if (openList.empty())
        {
            cout << "Cannot find path!:(\n";
            return -1;
        }
    }

    /* backtrack from end to start */
    current = endAStarNode;
    while (current)
    {
        path.push_front(Axis(current->x, current->y));
        current = current->father;
    }

    return 0;
}

void _mapPrint(void)
{
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            cout << g_map[i][j] << " ";
        }
        cout << endl;
    }
}

int main(void)
{
    Axis start(0, 0);
    Axis end(4, 5);   // end for _map 1
    //Axis end(4, 9);     // end for _map 2
    list<Axis> path;

    _mapPrint();

    int ret = FindPath(g_map, start, end, path);
    if (ret != 0)
    {
        return -1;
    }

    list<Axis>::iterator it  = path.begin();
    it++;
    for (; it != path.end(); it++)
    {
        cout << "(" << it->x << "," << it->y << ")";
        g_map[it->x][it->y] = 8;
    }
    cout << endl;

    _mapPrint();
    
    return 0;
}