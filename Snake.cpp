#include "Snake.h"

//Coordinate
Coordinate::Coordinate():x(0),y(0),vistited(false){}
Coordinate::Coordinate(int x, int y):x(x),y(y),vistited(false){}
bool Coordinate::operator==(const Coordinate&other){
	return (x == other.x&&y == other.y);
}
bool Coordinate::operator<(const Coordinate&other)const{
	if (x == other.x)
		return y < other.y; 
	else 
		return x < other.x;
	
}
//Node
Node::Node():pos(0,0){
	sharp = ' ';
	next = NULL;
}
Node::Node(const Coordinate&pos, char*sharp,Node *ptr){
	this->set(pos, sharp, ptr);
}
void Node::set(const Coordinate&pos, char*sharp, Node *ptr){
	this->pos = pos;
	this->sharp = *sharp;
	this->next = next;
}


//Head

Head::Head(const Coordinate&pos, char*sharp, int direction) : Node(pos, sharp, NULL){
	this->direction = direction;
}



//Snake

Snake::Snake(int n,Board*ptr){
	this->head = new Head(Coordinate(3,0), "#", Right);
	Node * temp = head;
	while (--n){
		temp->next = new Node(Coordinate(temp->pos.x - 1, temp->pos.y), "*", NULL);
		temp = temp->next;
	}
	tail = temp;
	score = 0;
	this->board = ptr;//TODO
}
Snake::Snake(std::ifstream &file){
	/*int s; file >> s;
	this->score = s; 
	head = new Head()
	int direct; file >> direct;
	int x, y;
	while (file >> x >> y){
	}*/
	//TODO

}

void Snake::turn(int direction){
	int currDirect = this->head->direction;
	if ((currDirect == Up && direction == Down)||
		(currDirect == Down && direction == Up)||
		(currDirect == Left &&direction == Right)||
		(currDirect == Right &&direction==Left))
		return;

	this->head->direction = direction;
}
void Snake::run(){
	switch (head->direction){
	case Up:
		checkThenGo(Coordinate(head->pos.x, head->pos.y - 1));
		break;
	case Right:
		checkThenGo(Coordinate(head->pos.x + 1, head->pos.y));
		break;
	case Down:
		checkThenGo(Coordinate(head->pos.x, head->pos.y + 1));
		break;
	case Left:
		checkThenGo(Coordinate(head->pos.x - 1, head->pos.y));
		break;
	default:
		break;
	}
	 
}
void Snake::run(const Coordinate&pos){
	checkThenGo(pos);
}

Head * Snake::getHead()const{
	return head;
}

int Snake::getScore(){
	return score;
}
Snake::~Snake(){
	Node *temp = head;
	while (temp){
		Node * t = temp->next;
		delete temp;
		temp = t;
	}
	board = NULL;
}

//Snake private
void Snake::addNode(const Coordinate&pos){
	Head *temp = new Head(pos, "#", head->direction);
	head->set(head->pos, "*", head->next);
	temp->next = head;
	head = temp;
}
void Snake::move(Node *head){
	int t_x = head->pos.x,
		t_y = head->pos.y;
	while (head->next){
		//移动到蛇头的下一个节点
		head = head->next;
		int t_t_x = head->pos.x,
			t_t_y = head->pos.y;
		head->pos.x = t_x;
		head->pos.y = t_y;
		t_x = t_t_x;
		t_y = t_t_y;
	}
}
void Snake::checkThenGo(const Coordinate&pos){
	int	flag = board->isBlock(pos);
	//-1 碰到边界或者碰到自己
	if (flag == -1)throw new std::string("Game Over");
	//1 遇到食物
	else if (flag == 1){
		board->generateFood();
		score++;
		addNode(pos);
	}
	// 0 正常行进
	else {
		move(head);
		head->pos = pos;
	}
}






//Board
Board::Board(int x, int y){
	border = '.';
	food = '$';
	space = ' ';

	this->row = y;
	this->column = x;
	//board 是一个x*y 的 node 数组;
	board = new Node[x*y];
	//给board的每个node 坐标赋值
	for (int i = 0; i < row; i++){
		for (int j = 0; j < column; j++){
			board[i*column + j].pos.x = i;
			board[i*column + j].pos.y = j;
		}
	}

	generateFood();
	drawFood();
}
//display显示每个node的sharp
void Board::display(){
	for (int i = -1; i < row+1; i++){
		for (int j = -1; j < column+1; j++){
			if (i == -1 || i == row || j == -1 || j == column)
				std::cout << border;
			else
				std::cout << board[i*column + j].sharp;
		}
		std::cout << std::endl;
	}
}
//检测当前点是否合格
int Board::isBlock(const Coordinate&pos){
	//-1 游戏结束 1 获取食物，0 正常前进
	if (pos.x >= column || pos.x < 0 || pos.y >= row || pos.y < 0)
		return -1;
	if (getPos(pos) == food){
		return 1;
	}
	if (getPos(pos) != space)
		return -1;
	return 0;
}

int Board::getRow()const{
	return row;
}
int Board::getCloum() const
{
	return column;
}

void Board::drawSnake(Node*head){
	cleanBoard();
	while (head){
		setPos(head->pos,head->sharp);
		head = head->next;
	}
	drawFood();
}
Board::~Board(){
	if (board){
		delete[] board;
		board = NULL;
	}
}
vector<Coordinate> Board::getNeighbors(const Coordinate&current){
	vector<Coordinate> re;
	int x = current.x, y = current.y;
	Coordinate	up	 (x, y - 1),
				right(x + 1, y),
				down (x, y + 1),
				left (x - 1, y);

	if (isBlock(up) != -1)re.push_back(up);
	if (isBlock(right) != -1)re.push_back(right);
	if (isBlock(down) != -1)re.push_back(down);
	if (isBlock(left) != -1)re.push_back(left);
	return re;
}
Coordinate Board::getFoodPos(){
	return foodPos;
}
//Board private
char Board::getPos(const Coordinate&pos){
	return board[pos.y*column+pos.x].sharp;
}
void Board::setPos(const Coordinate&pos,const char ch){
	board[pos.y*column+pos.x].sharp = ch;
}
//创建一个食物
void Board::generateFood(){
	Coordinate temp(0,0);
	//循环知道生成的食物没有和蛇身体重合
	do{
		temp = Coordinate(rand() % column, rand() % row);
	} while (isBlock(temp) == -1);

	foodPos = temp;
	//foodPos = Coordinate(4, 1);
}

void Board::drawFood(){
	setPos(foodPos,food);
}

void Board::cleanBoard(){
	for (int i = 0; i < column*row; i++){
		board[i].sharp = space;
	}
}