#include <iostream>
#include <conio.h>
#include <windows.h>

#include <thread>

using namespace std;

bool gameOver;
const int height = 20;
const int width = 20;

int x, y, fruitX, fruitY, score;

int tailX[100], tailY[100], ntail;

enum class eDirection {Stop=0, UP, DOWN, LEFT, RIGHT};
eDirection dir = eDirection::Stop;
eDirection LastDir = eDirection::Stop; //check the last dir so you don't move backward

void Setup()
{
	gameOver = false;
	dir = eDirection::Stop;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
}
void Draw()
{
	system("CLS");

	// create the boards left
	for (int i = 0; i < width; i++)
	{
		cout << "#";
	}cout << endl;

	// create the board width signs along the height
	for (int i = 0; i < height; i++)
	{
		for (int u = 0; u < width; u++)
		{
			if (u == 0 || u == width - 1)
			{
				cout << "#";
			}
			else if (i == y && u == x)
			{
				cout << "O";
			}
			else if (i == fruitY && u == fruitX)
			{
				cout << "F";
			}
			else
			{

				bool print = false;

				for(int k = 0; k < ntail; k++)
				{
					if (i == tailY[k] && u == tailX[k])
					{
						cout << "o";
						print = true;
					}
				}
				if (!print) { cout << " "; }
			}
		}cout << endl;
	}

	// bottum side
	for (int i = 0; i < height; i++)
	{
		cout << "#";
	}cout << endl;
	cout << endl;
	cout << "Score" << score << endl;
}
void Input()
{
	while (!gameOver)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case 'w':
				if (LastDir != eDirection::DOWN)
				{
					dir = eDirection::UP;
				}
				break;
			case 'a':
				if (LastDir != eDirection::RIGHT)
				{
					dir = eDirection::LEFT;
				}
				break;
			case 's':
				if (LastDir != eDirection::UP)
				{
					dir = eDirection::DOWN;
				}
				break;
			case 'd':
				if (LastDir != eDirection::LEFT)
				{
					dir = eDirection::RIGHT;
				
				}
				break;
			default:
				break;
			}
		}

		LastDir = dir;
		Sleep(1);

	}
}
void Logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	tailX[0] = x;
	tailY[0] = y;
	int prev2X, prev2Y;

	// chain the snake toghether
	for(int i=1; i < ntail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;

		prevX = prev2X;
		prevY = prev2Y;
	}

	switch (dir)
	{
	case eDirection::Stop:
		break;
	case eDirection::UP:
		y--;
		break;
	case eDirection::DOWN:
		y++;
		break;
	case eDirection::LEFT:
		x--;
		break;
	case eDirection::RIGHT:
		x++;
		break;
	default:
		break;
	}

	// hit the wall
	if(x < 0 || x > width  || y < 0 || y > height)
	{
		gameOver = true;
	}

	// game over if touching tail
	for (int i = 0; i < ntail; i++)
	{
		if (x == tailX[i] && y == tailY[i])
		{
			gameOver = true;
		}
	}

	// eat spawn new fruit
	if(x==fruitX && y==fruitY)
	{
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		ntail++;
	}
}

int main()
{
	Setup();

	thread InputThread(Input);

	while (!gameOver)
	{
		Logic();
		Draw();
		//Input();
		Sleep(100);
	}

	InputThread.detach();
	return 0;
}