#include"Environment.h"
#include<iostream>
#include<windows.h>
#include<ctime>

enum COLOR {
	BLACK,
	BLUE,
	GREEN,
	YELLOW_GREEN,
	RED,
	DARK_RED,
	YELLOW,
	WHITE,
	GRAY,
	LIGHT_BLUE,
	LIGHT_GREEN,
	LIGHT_YELLOW_GREEN,
	LIGHT_RED,
	LIGHT_DARK_RED,
	LIGHT_YELLOW,
	DARK_WHITE
};

Environment::Environment(const int& _row, const int& _col, const int& _num_obstacle)
	:state(0), row(_row), col(_col), epsilon(1.0), win_cnt(0), num_obstacle(_num_obstacle)
{
	srand(static_cast<unsigned int>(time(NULL)));
	board = new int*[row];
	score = new Q*[row];
	for (int i = 0; i < row; i++)
	{
		board[i] = new int[col];
		score[i] = new Q[col];
		for (int j = 0; j < col; j++)
			board[i][j] = 0;
	}
	// Goal
	board[row - 1][col - 1] = 1;
	// make hole
	for (int i = 0; i < num_obstacle; i++)
	{
		int temp = rand() % (row*col - 2) + 1;
		if (board[temp / row][temp%row] != -1)
			board[temp / row][temp%row] = -1;
		else
			i--;
	}

	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void Environment::move()
{
	int action;
tryAgain:
	if (static_cast<double>(rand()) / RAND_MAX < epsilon)
		// explore
		action = getRandomAction();
	else
		// exploit
		action = getAction();

	if (isValid(action))
	{
		update(action);
		state = getNextState(action);
		if (isOver())
			state = 0;
	}
	else
		goto tryAgain;
	if (epsilon > EPSILON_MIN)
		epsilon *= EPSILON_DISCOUNT;
	system("cls");
	draw();
	Sleep(100);
}

int Environment::getAction()
{
	int cnt = 0;
	double max = score[state/row][state%row].getMax();
	int arr[4];
	for (int i = 0; i < 4; i++)
		if (max == score[state/row][state%row][i])
			arr[cnt++] = i;
	return arr[rand() % cnt];
}

void Environment::draw()
{
	for (int j = 0; j < col; j++)
		printf("бсбс");
	printf("бс\n");
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (i*row + j == state)
			{
				printf("бс");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_YELLOW);
				printf("б┌");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_WHITE);
			}
			else if (board[i][j] == 1)
			{
				printf("бс");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_GREEN);
				printf("бс");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_WHITE);
			}
			else if (board[i][j] == -1)
			{
				printf("бс");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_RED);
				printf("бс");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_WHITE);
			}
			else
				printf("бс  ");
		}
		printf("бс\n");
		for (int j = 0; j < col; j++)
			printf("бсбс");
		printf("бс\n");
	}
	//printQ();
	printf("e = %.3lf\n", epsilon);
	printf("win count : %d\n", win_cnt);
}

// if you want to check current status
void Environment::printQ()
{
	for (int i = 0; i < row * col; i++)
		printf("state %d : %.2lf %.2lf %.2lf %.2lf\n", i, score[i/row][i%row][0], score[i / row][i%row][1], score[i / row][i%row][2], score[i / row][i%row][3]);
}

int Environment::getRandomAction()
{
	return rand() % 4;
}

bool Environment::isValid(const int& action)
{
	switch (action)
	{
	case UP:
		if (state / row <= 0)
			return false;
		break;
	case RIGHT:
		if (state%row >= col - 1)
			return false;
		break;
	case DOWN:
		if (state / row >= row - 1)
			return false;
		break;
	case LEFT:
		if (state%row <= 0)
			return false;
		break;
	}
	return true;
}

bool Environment::isOver()
{
	if (board[state / row][state%row])
	{
		if (board[state / row][state%row] > 0)
			win_cnt++;
		return true;
	}
	return false;
}

void Environment::update(const int& action)
{
	int nextState = getNextState(action);
	score[state / row][state%row][action] = static_cast<double>(board[nextState / row][nextState%row]) + DISCOUNT * score[nextState / row][nextState%row].getMax();
}

int Environment::getNextState(const int& action)
{
	switch (action)
	{
	case UP:
		return state - row;
	case RIGHT:
		return state + 1;
	case DOWN:
		return state + row;
	case LEFT:
		return state - 1;
	}
}