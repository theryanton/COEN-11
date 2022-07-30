/* Ryan Ton
 * COEN 11
 * Lab 5
 * Wednesday 5:15pm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 5
#define LENGTH 20
#define NODE struct node

struct node 
{
	int groups;
	char names[LENGTH];
	NODE *next;
};
NODE *head = (NODE *)NULL;
NODE *tail = (NODE *)NULL;

void insert();
int check_duplicate(char *);
void delete();
void list();
void search_size();

int main()
{
	int input;
	printf("Welcome to the Surf Lesson Scheduler!\n\n");

	while (1)
	{
		printf("(1) Schedule appointment.\n");
		printf("(2) Remove appointment.\n");
		printf("(3) List schedule.\n");
		printf("(4) Search using group sizes.\n");
		printf("(0) Exit.\n\n");
		scanf("%d", &input);

		switch (input)
		{
			case 0:
				return 1;
			case 1:
				insert();
				break;
			case 2:
				delete();
				break;
			case 3:
				list();
				break;
			case 4:
				search_size();
				break;
			default:
				printf("Command not valid. Input 1, 2, 3, 4, or 0.\n");
				break;
			return 0;
		}
	}
}

void insert()
{
	int x, inputsize;
	char inputname[LENGTH];
	NODE *temp;

	printf("Enter a name:\n");
	scanf("%s", inputname);
	x = check_duplicate(inputname);

	if (x)
	{
		printf("There's already an appointment under %s.\n", inputname);
	}
	else
	{
		printf("Enter group size:\n");
		scanf("%d", &inputsize);
		temp = (NODE *) malloc (sizeof (NODE));
		if (head == NULL)
		{
			head = tail = temp;
		}
		else
		{
			tail->next = temp;
		}
		strcpy (temp->names, inputname);
		temp->groups = inputsize;
		temp->next = NULL;
		tail=temp;
		printf("An appointment has been scheduled for %s for %d people.\n", inputname, inputsize);
	}
	return;
}

int check_duplicate (char inputname[LENGTH])
{
	NODE *temp;
	temp = head;

	while (temp != NULL)
	{
		if (strcmp (inputname, temp->names) == 0)
		{
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

void delete()
{
	int inputsize, hasGroups = 0;
	NODE *p, *q;
	p = q = head;

	if (head == NULL)
	{
		printf("The schedule is currently empty.\n");
		return;
	}
	else
	{
		printf("Enter the group size you want to remove:\n");
		scanf("%d", &inputsize);
		while (p != NULL)
		{
			if (inputsize >= p->groups)
			{
				printf("%s's appointment with %d people has been removed.\n", p->names, p->groups);
				inputsize -= p->groups;
				if (p->next == NULL)
				{ 
					head = NULL;
					tail = NULL;
					free (p);
					return;
				}
				else	if (p == head)
				{
					head = p->next;
					free (q);
					q = head;
					p = head;
				}
				else if (p == tail)
				{
					tail = q;
					q->next = NULL;
					free (p);
					return;
				}
				else
				{
					q->next = p->next;
					free (p);
					p = q->next;
				}
			}
			else
			{
				q = p;
				p = p->next;
			}
		}
	}
	return;
}

void list()
{
	NODE *p;
	p = head;

	if (p == NULL)
	{
		printf("The schedule is currently empty.\n");
	}
	else
	{
		while (p != NULL)
		{
			printf("Name: %s Group Size: %d\n", p->names, p->groups);
			p = p->next;
		}
	}
	return;
}

void search_size()
{
	int inputsize, hasGroups = 0;
	NODE *p;
	p = head;

	if (head == NULL)
	{
		printf("The schedule is currently empty.\n");
	}
	else
	{
		printf("Enter a group size you want to search for:\n");
		scanf("%d", &inputsize);

		while (p != NULL)
		{
			if (p->groups <= inputsize)
			{
				hasGroups = 1;
				printf("Name: %s Group Size: %d\n", p->names, p->groups);
			}
			p = p->next;
		}
		if (hasGroups != 1)
		{
			printf("None found.\n");
		}
	}
	return;
}
