#pragma once
struct Node
{
    double g;
    double h;
    double f;

    POINT pos;
    POINT parent;
};
class PriorityQueue
{
private:
    vector<Node> arr;
public:
    PriorityQueue();
    ~PriorityQueue();
    void Push(Node node);
    Node Pop();
    int GetCount();
    
};

