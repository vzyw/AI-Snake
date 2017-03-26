#pragma once
#pragma execution_character_set("utf-8")
#include <iostream>
#include<windows.h>
#include<time.h>
#include<conio.h>
#include<string>
using namespace std;

#include "Snake.h"

int main(){
	//srand(time(0));
	int gamespeed = 1;
	int timeover,start;
	char direction = Left;
	//60*15的板子
	Board board(30,15);
	Snake snake(3, &board);
	
	int n = 3;
	while (true)
	{

		Astart aStart(snake.getHead()->pos, board.getFoodPos(), &board);
		vector<Coordinate> path = aStart.getPath();
		if (!path.size())
			break;
		for (Coordinate i : path){

			snake.run(i);
			board.drawSnake(snake.getHead());
			board.display();

			Sleep(gamespeed);
			system("cls");
		}
		//system("cls");
	}

	
}