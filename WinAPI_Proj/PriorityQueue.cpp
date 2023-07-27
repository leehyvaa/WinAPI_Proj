#include "pch.h"
#include "PriorityQueue.h"

PriorityQueue::PriorityQueue()
	:arr{}
{
}

PriorityQueue::~PriorityQueue()
{
}

void PriorityQueue::Push(Node node)
{
	arr.push_back(node);
	int crnt= GetCount();
	int parent = (crnt - 1) / 2;
	Node temp = {};
	while (parent >= 0)
	{
		if (arr[crnt].f < arr[parent].f)
		{
			temp = arr[parent];
			arr[parent] = arr[crnt];
			arr[crnt] = temp;
			crnt = parent;
			parent = (crnt - 1) / 2;
		}
		else
			break;
	}
}

Node PriorityQueue::Pop()
{

	//assert(GetCount() <= 1);

	Node rtn = arr[0];
	arr[0] = arr[GetCount()];
	arr.pop_back();

	int crnt = 0;
	int left = crnt * 2 + 1;
	int right = crnt * 2 + 2;
	Node temp={};
	while (1)
	{
		if (left > GetCount())
			break;

		int now = crnt;

		if (arr[left].f < arr[crnt].f)
		{
			temp = arr[left];
			arr[left] = arr[crnt];
			arr[crnt] = temp;
			now = left;
		}

		if (right > GetCount())
			break;

		if (arr[right].f < arr[crnt].f)
		{
			temp = arr[right];
			arr[right] = arr[crnt];
			arr[crnt] = temp;
			now = right;
			
		}

		left = now * 2 + 1;
		right = now * 2 + 2;

		if (now == crnt)
			break;
		else
		{
			crnt = now;
		}
	}

	return rtn;
}

int PriorityQueue::GetCount()
{
	return arr.size()-1;
}
