#include "stdafx.h"
#include <iostream>
#include <set>
#include <queue>
#include <conio.h>
#include <iterator>
#include <string>
#include <fstream>//раб0та с фаiл0м
//#include "Header.h"
using namespace std;

class Exception {
public:
	Exception() {
		cout << "Не существующая связь, вершина" << endl;
		_getch();
	}
	~Exception() {}
};

class Node {//Класс вершин
private:
	string name;//
	set <Node*> neighbours;//соседи  вершины
	typedef set<Node*>::const_iterator node_iterator;//множество итераторов
													 //int count = 0;
	void addNeighbour(Node* neighbour)//добавление соседей
	{
		neighbours.insert(neighbour);
	}
	void removeNeighbour(Node* neighbour)//удалить
	{
		neighbours.erase(neighbour);
	}
public:
	Node() {	}
	~Node() {	}
	Node(const string& aname) : name(aname) {}

	friend class Graph;
	void addNeighbours(Node* neighbour)//добавление соседей
	{
		neighbours.insert(neighbour);
		neighbour->addNeighbour(this);
	}
	void removeNeighbours(Node* neighbour)//удалить
	{
		neighbours.erase(neighbour);
		neighbour->removeNeighbour(this);
	}

	const string& getName() const {
		return name;
	}

	void setName(string aname) {
		this->name = aname;
	}

	node_iterator nd_begin() const {
		return neighbours.begin();
	}
	node_iterator nd_end() const {
		return neighbours.end();
	}
	void printNeighbours() {
		for (node_iterator i = neighbours.begin(); i != neighbours.end(); i++)
		{
			cout << (*i)->getName() << " ";
		}
	}

	friend ostream& operator <<(ostream& out, const Node& n) {
		out << n.getName() << endl;
		return out;
	}
};

class Graph {
private:
	set <Node*> nodes;
	typedef set<Node*>::const_iterator node_iterator;//множество итераторов
	set<string> names;
	Node* GetPtrNode(string nameNode) {
		if (names.find(nameNode) == names.end()) {
			Node* node = new Node;
			node->setName(nameNode);
			addNode(node);
			return node;
		}
		else for (set<Node*>::iterator it = nodes.begin(); it != nodes.end(); it++)
			if ((*it)->getName() == nameNode)
				return *it;
	}
public:
	Graph() {}
	~Graph() {}
	void addNode(Node* node)//добавить вершину
	{
		nodes.insert(node);
		names.insert(node->getName());
	}
	void removeNode(Node* node) {//удалить вершину и связи
		//if (!node) throw Exception();
		nodes.erase(node);
		for (set<Node*>::iterator it = nodes.begin(); it != nodes.end(); it++)
			(*it)->removeNeighbours(node);
	}

	void addEdge(Node* begin, Node* end) {
		if (nodes.find(begin) == nodes.end()) return;
		if (nodes.find(end) == nodes.end()) return;
		begin->addNeighbours(end);
	//	end->addNeighbours(begin);
	}
	void removeEdge(Node* begin, Node* end) {
		begin->removeNeighbours(end);
	}
	node_iterator end() const {
		return nodes.end();
	}
	node_iterator begin() const {
		return nodes.begin();
	}

	void showGraph() {
		for (node_iterator i = nodes.begin(); i != nodes.end(); i++)
		{
			cout << (*i)->getName() << ": ";
			(*i)->printNeighbours();
			cout << endl;
		}
	}

	void readGraph(char nameText[]) {//чтение файла, запись в граф
		ifstream file(nameText);
		char begin[10], end[10];
		//проверка на сущёств0ваниё
		if (!file.is_open())
			cout << "error: file is not found" << endl;
		else{
			file >> begin;
			file >> end;
			while (file) {
				file >> begin;
				file >> end;
				Node* node[2];
				node[0] = GetPtrNode(begin);
				node[1] = GetPtrNode(end);
				addEdge(node[0], node[1]);
			}
		}
	}
};

class BFS {//поиск в ширину
private:
	const Graph& graph;
	typedef set<Node*>::const_iterator node_iterator;//множество итераторов
public:
	BFS(const Graph& agraph) : graph(agraph) {}
	bool connected(Node* begin, Node* end) {
		queue<Node*> nodes;//очередь
		nodes.push(begin);//добавление в очередь
		set <Node*> visited;//посещеные вершины
		while (!nodes.empty()) {
			Node* next = nodes.front();
			nodes.pop();
			if (end == next) return true;
			visited.insert(next);
			for (node_iterator it = next->nd_begin(); it != next->nd_end(); it++)
				if (visited.find(*it) == visited.end()) nodes.push(*it);
		}
		return false;
	}
};


void main() {
	setlocale(LC_ALL, "Russian");

	Graph g;
	g.readGraph("TestGraph.txt");
	g.showGraph();

	_getch();
}
