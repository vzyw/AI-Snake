#pragma once
#pragma execution_character_set("utf-8")
enum direct{ Up = 72, Right = 77, Down = 80, Left = 75 };
#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include<vector>
#include <algorithm>
using namespace std;
struct Coordinate{
	Coordinate();
	Coordinate(int x, int y);
	bool operator==(const Coordinate&other);
	bool operator<(const Coordinate&other) const;
	int x,
		y;
	bool vistited;
};
class Node{
public:
	Node();
	Node(const Coordinate&pos,char*sharp,Node *next);
	virtual void set(const Coordinate&pos, char* sharp, Node*ptr);

	Coordinate pos;
	char sharp;
	Node* next;
};

class Head :public Node{
public:
	Head();
	Head(const Coordinate&pos, char*sharp, int direction);
	int direction;
};

class Board;
class Snake{
public:
	//构建一个长度为n的蛇
	Snake(int n,Board*ptr);
	//从文件中读取蛇数据 TODO
	Snake(std::ifstream &file);
	//转方向
	void turn(int direction);
	//按照当前的方向运行
	void run();
	//根据提供的坐标运行
	void run(const Coordinate&pos);
	//数据写入文件 TODO
	void writeToFile();
	//返回分数
	int getScore();
	Head *getHead()const;

	~Snake();
private:
	//移动整个蛇身 依次把当前节点赋值给下一个节点
	void move(Node *head);
	//遇到食物则把食物当头节点添加到蛇身上去
	void addNode(const Coordinate&pos);
	//工厂函数，检测下一个节点是否是边界还是自身还是食物
	void checkThenGo(const Coordinate&pos);
	//保存头节点
	Head * head;
	//保存尾部节点 TODO
	Node * tail;
	//分数
	int score;
	//棋盘的引用
	Board *board;
};

class Board{
private:
	//各自表示边界，食物，空的地方 的字符
	char border,food,space;
	//一个棋盘
	Node *board;
	//棋盘的长宽
	int row, column;
	//食物坐标
	Coordinate foodPos;
	//画出食物
	void drawFood();
	
	//返回指定坐标的值
	char getPos(const Coordinate&pos);
	//设置指定坐标的值
	void setPos(const Coordinate&pos,const char ch);
	//对棋盘进行清理
	void cleanBoard();

public:
	//构建一个row*column 的棋盘
	Board(int row, int column);
	~Board();
	void drawSnake(Node * head);
	void display();
	int getRow()const;
	int getCloum()const;
	int isBlock(const Coordinate&pos);
	//创建一个食物
	void generateFood();

	//该方法用于寻路时获取当前位置周围的结点的数组集
	vector<Coordinate> getNeighbors(const Coordinate&current);

	Coordinate getFoodPos();
};


//Astart 寻路
class Astart{
private:
	//保存起点到终点测试过的位置
	queue<Coordinate> frontier;
	//一个棋盘；
	Board *board;
	map<Coordinate, Coordinate> cameFrom;
	Coordinate start, goal;
public:
	Astart(Coordinate& start, Coordinate& goal,Board *board){
		this->board = board;
		this->start = start;
		this->goal = goal;
		frontier.push(start);
		cameFrom[start] = Coordinate(0, 0);
		//Astar 寻路 先把起点放入队列，然后把起点pop出，再把起点的上下左右放入，然后pop出一个 作为起点  ，继续循环。
		while (!frontier.empty()){
			Coordinate current = frontier.front();
			frontier.pop();
			if (current == goal)break;//遇到目标则终止
			vector<Coordinate> neighbors = board->getNeighbors(current);
			for (Coordinate next : neighbors){
				if (cameFrom.find(next) == cameFrom.end()){
					frontier.push(next);
					cameFrom[next] = current;
				}
			}

		}
	}

	vector<Coordinate> getPath(){
		vector<Coordinate> path;
		Coordinate current = goal;
		path.push_back(current);
		if (cameFrom.find(current) == cameFrom.end())return path;
		while (!(current == start)){
			current = cameFrom[current];
			path.push_back(current);
		}
		reverse(path.begin(), path.end());
		path.erase(path.begin());
		return path;
	}
}; 