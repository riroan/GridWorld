#pragma once
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define DISCOUNT 0.9
#define EPSILON_DISCOUNT 0.999
#define EPSILON_MIN 0.1

class Q
{
public:
	double arr[4];

	Q()
	{
		for (int i = 0; i < 4; i++)
			arr[i] = 0.0;
	}

	double& operator[](const int& i)
	{
		return arr[i];
	}

	double getMax()
	{
		double ret = arr[0];
		for (int i = 1; i < 4; i++)
			if (ret < arr[i])
				ret = arr[i];
		return ret;
	}

	int getMaxIx()
	{
		double max = arr[0];
		int ix = 0;
		for (int i = 1; i < 4; i++)
			if (max < arr[i])
			{
				max = arr[i];
				ix = i;
			}
		return ix;
	}
};

class Environment
{
public:
	int row;
	int col;
	int state;
	int win_cnt;
	Q ** score;
	int ** board;
	double epsilon;
	int num_obstacle;

	Environment(const int& _row, const int& _col, const int& _num_obstacle);

	void printQ();
	void draw();
	void move();
	void update(const int& action);
	int getNextState(const int& action);
	int getRandomAction();
	int getAction();
	bool isValid(const int& action);
	bool isOver();
};