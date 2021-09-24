#include<stdio.h>
#include<stdlib.h>

int complete_node = 15;

struct node
{
	char data;
	struct node *rightchild;
	struct node *leftchild;
};

struct node* newnode(char data)
{
	struct node* temp;
	temp = (node*)malloc(sizeof(struct node));
	temp-> data = data;
	temp-> leftchild = NULL;
	temp-> rightchild = NULL;
	return(temp);
};

void preorder(struct node* root)
{
	if (root != NULL)
	{
		printf("%c", root->data);
		preorder(root->leftchild);
		preorder(root->rightchild);
	}
}

void inorder(struct node* root)
{
	if (root != NULL)
	{
		preorder(root->leftchild);
		printf("%c", root->data);
		preorder(root->rightchild);
	}
}
void postorder(struct node* root)
{
	if (root != NULL)
	{
		preorder(root->leftchild);
		preorder(root->rightchild);
		printf("%c", root->data);
	}
}

void main()
{
	struct node* temp;
	temp = newnode('D');

	temp->leftchild = newnode('A');
	temp->rightchild = newnode('F');
	temp->leftchild->leftchild = newnode('E');
	temp->leftchild->rightchild = newnode('B');

	preorder(temp);
	printf("\n");
	postorder(temp);
	printf("\n");
	inorder(temp);
	printf("\n");
}











