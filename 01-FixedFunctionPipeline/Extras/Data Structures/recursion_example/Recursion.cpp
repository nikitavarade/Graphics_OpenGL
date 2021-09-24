#include<stdio.h>
void tail_factorial(int, int);
int nontail_factorial(int);
int num, fact = 1, ct = 1;

int main()
{
	printf("Enter Number whose Factorial you want to find...\n");
	scanf_s("%d", &num);

	tail_factorial(num, fact);

	printf("Non-tail Fcatorial %d", nontail_factorial(num));

	return 0;
}
//when recursive call is last thing

void tail_factorial(int num, int fact)
{
	fact = fact * num;
	if (num == 1)
	{
		printf("Tail Factorial is %d\n", fact);
		return;

	}
	else
	{
		tail_factorial(num - 1, fact);
	}
}

//recursive call is not the last thing
int nontail_factorial(int x)
{
	if (x > 0)
	{
		return(x * nontail_factorial(x - 1));
	}
	return 1;
}