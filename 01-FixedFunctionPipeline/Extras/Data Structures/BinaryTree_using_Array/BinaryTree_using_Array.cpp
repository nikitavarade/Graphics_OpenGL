#include<stdio.h>
int complete_node = 15;
char tree[] = { '\0','D','A','F','E','B','R','T','G','Q','\0','\0','V','\0','J','L' };

int leftchild(int index)
{
	if (tree[index] != '\0' && (2 * index) <= complete_node)
	{
		return(2 * index);
	}
	return -1;
}
int rightchild(int index)
{
	if (tree[index] != '\0' && (2 * index)+1 <= complete_node)
	{
		return(2 * index)+1;
	}
	return -1;
}
void postorder(int index)
{
	if (index > 0 && tree[index] != '\0')
	{
		postorder(leftchild(index));
		postorder(rightchild(index));
		printf("%c ", tree[index]);
	}
}

void inorder(int index)
{
	if (index > 0 && tree[index] != '\0')
	{
		inorder(leftchild(index));
		printf("%c ", tree[index]);
		inorder(rightchild(index));

	}
}

void preorder(int index)
{
	if (index > 0 && tree[index] != '\0')
	{
		printf("%c ", tree[index]);
		preorder(leftchild(index));
		preorder(rightchild(index));

	}
}

int main(void)
{
	printf("Preorder: ");
	preorder(1);
	printf("\nPostorder: ");
	postorder(1);
	printf("\n Inorder: ");
	inorder(1);
	printf("\n");
	return 0;
}