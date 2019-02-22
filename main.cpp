#include"Environment.h"
#define NUM_OBSTACLE 4

int main()
{
	Environment env(4, 4, NUM_OBSTACLE);	// row, col, the number of obstacle
	while (1)
		env.move();
}