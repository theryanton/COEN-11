/* Ryan Ton
 * COEN 11
 * Lab 3
 * Wednesday 5:15pm
 */

#include <stdio.h>
#include <string.h>

#define SIZE 5
#define LENGTH 20

typedef struct entry 
{
	int groups;
	char names[LENGTH];
} ENTRY;
ENTRY waitlist [SIZE];
// To access waitlist[i].group //
int counter = 0;

void insert();
int check_duplicate(char inputname[LENGTH]);
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
	int i, x, inputsize;
	char inputname[LENGTH];
	struct entry *p;
	p = waitlist;

	if (counter >= SIZE)
	{
		printf("Sorry, no more appointments available.\n");
	}
	else
	{
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

			for (i=0; i<SIZE; i++)
			{
				if (p[i].names[0] == '\0')
				{
					strcpy (p[i].names, inputname);
					p[i].groups = inputsize;
					counter++;
					printf("An appointment has been scheduled for %s for %d people.\n", inputname, p[i].groups);
					break;
				}
			}
		}
	}
	return;
}

int check_duplicate (char inputname[LENGTH])
{
	int i;
	struct entry *p;
	p = waitlist;

	for (i=0; i<counter; i++)
	{
		if (strcmp (inputname, p[i].names) == 0)
		{
			return 1;
		}
	}
	return 0;
}

void delete()
{
	int i, j, inputsize, hasGroups = 0;
	struct entry *p;
	p = waitlist;

	if (counter == 0)
	{
		printf("The schedule is currently empty.\n");
	}
	else
	{
		printf("Enter the group size you want to remove:\n");
		scanf("%d", &inputsize);

		for (i=0; i<counter; i++)
		{
			if (inputsize >= p[i].groups)
			{
				printf("%s's appointment with a group size of %d has been removed.\n", p[i].names, p[i].groups);
				inputsize -= p[i].groups;
				hasGroups = 1;

				for (j=i; j<counter-1; j++)
				{
					strcpy (p[j].names, p[j+1].names);
					p[j].groups = p[j+1].groups;
				}
				counter--;
				p[counter].names[0] = '\0';
				i--;
			}
		}
		if (hasGroups == 0)
		{
			printf("Entered group size does not fit any group on waitlist.\n");
		}
	}
	return;
}

void list()
{
	int i;
	struct entry *p;
	p = waitlist;

	if (counter == 0)
	{
		printf("The schedule is currently empty.\n");
	}
	else
	{
		printf("Name	Group Size\n");
		printf("-----------------\n");
		
		for (i=0; i<SIZE; i++)
		{
			if (p[i].names[0] == '\0')
			{
				printf("free\n");
			}
			else
			{
				printf("%s	%d\n", p[i].names, p[i].groups);
			}
		}
	}
	return;
}

void search_size()
{
	int i, inputsize, hasGroups = 0;
	struct entry *p;
	p = waitlist;

	if (counter == 0)
	{
		printf("The schedule is currently empty.\n");
	}
	else
	{
		printf("Enter a group size you want to search for:\n");
		scanf("%d", &inputsize);

		for (i=0; i<counter; i++)
		{
			if (p[i].groups <= inputsize)
			{
				hasGroups = 1;
				printf("%s	%d\n", p[i].names, p[i].groups);
			}
		}
		if (hasGroups != 1)
		{
			printf("None found.\n");
		}
	}
	return;
}
