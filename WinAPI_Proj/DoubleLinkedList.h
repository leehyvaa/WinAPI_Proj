#pragma once
#include "pch.h"
using namespace std;

struct Node
{
	POINT* data;
	Node* _next;
	Node* _prev;
};

class CList
{
private:

	Node* head;
	int count;


public:
	CList() { head = nullptr;  count = 0; }
	~CList();
	void Push_Back(POINT* _data);
	void Pop_Back();
	void Delete(int _index);
	void Insert(int, POINT* _data);
	void Clear(Node* node);

	void PrintAll();
	void PrintReverse();
};


//인덱스가 -1인 인덱스 찾기


inline CList::~CList()
{
	Clear(head);

}

inline void CList::Clear(Node* node)
{

	if (node->_next == head)
	{
		delete node;
		return;
	}
	Clear(node->_next);
	delete node;
}





inline void CList::Push_Back(POINT* _data)
{
	Node* nNode = new Node;
	nNode->data = _data;
	nNode->_next = nullptr;

	if (head == nullptr)
	{
		head = nNode;
		head->_next = nNode;
		head->_prev = nNode;
	}
	else
	{
		nNode->_next = head;
		nNode->_prev = head->_prev;
		head->_prev->_next = nNode;
		head->_prev = nNode;

	}

	count++;
}

inline void CList::Delete(int _index)
{
	if (head == nullptr)
		return;
	if (count < _index)
		return;

	//꼬리를 삭제할때
	if (_index == count - 1)
	{
		Pop_Back();
		return;
	}



	//헤드삭제할때
	if (_index == 0)
	{
		Node* temp = head;
		head = head->_next;
		temp->_prev->_next = head;
		temp->_next->_prev = head;

		delete temp;
	}
	else
	{
		Node* prev = head;

		for (int i = 0; i < _index - 1; i++)
		{
			prev = prev->_next;
		}
		Node* dNode = prev->_next;

		prev->_next = dNode->_next;
		dNode->_next->_prev = prev;
		delete dNode;
	}

	count--;
}


inline void CList::Pop_Back()
{


	if (count != 0)
	{
		Node* dNode = head->_prev;
		dNode->_prev->_next = head;
		head->_prev = dNode->_prev;

		delete dNode;

	}

	count--;
}


inline void CList::Insert(int _index, POINT* _data)
{
	if (_index > count)
		return;

	Node* nNode = new Node;
	nNode->data = _data;

	if (_index == 0)
	{
		nNode->_next = head;
		nNode->_prev = head->_prev;
		head->_prev->_next = nNode;
		head->_prev = nNode;
		head = nNode;
	}
	else if (_index == count)
	{
		Push_Back(_data);
		return;
	}
	else
	{
		Node* prev = head;

		for (int i = 0; i < _index - 1; i++)
		{
			prev = prev->_next;
		}


		nNode->_next = prev->_next;
		nNode->_prev = prev;
		prev->_next->_prev = nNode;
		prev->_next = nNode;

	}

	count++;
}



inline void CList::PrintAll()
{
	Node* temp = head;

	while (1)
	{
		cout << temp->data->x << " " << temp->data->y << endl;

		if (temp == head->_prev)
			break;
		temp = temp->_next;

	}

}



inline void CList::PrintReverse()
{
	Node* temp = head;

	while (1)
	{
		cout << temp->data->x << " " << temp->data->y << endl;

		if (temp == head->_next)
			break;
		temp = temp->_prev;

	}

}

