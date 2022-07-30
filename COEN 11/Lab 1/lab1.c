/* Ryan Ton
 * COEN 11
 * Lab 1
 * Wednesday 5:15pm
 */

#include <stdio.h>

int main();
int divide();

int main()
{
	srand((int)time(NULL));
	int i, divisor, quotient, x, score = 0;

	for (i=0; i<10; i++)
	{
		divisor = rand()%12 + 1;
		quotient = rand()%13;
		x = divide (divisor, quotient);

		if (x == 1)
		{
			score++;
		}
	}
	printf("Your score is %d out of 10.\n", score);
	return 0;
}

int divide (int divisor, int quotient)
{
	int answer;

	printf("%d / %d = ?\n", divisor*quotient, divisor);
	scanf("%d", &answer);

	if (answer == quotient)
	{
		printf("Correct!\n");
		return 1;
	}
	else
	{
		printf("Incorrect.\n");
		return 0;
	}
}





