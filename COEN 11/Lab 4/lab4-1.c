/* Ryan Ton
 * COEN 11
 * Lab 4
 * Wednesday 5:15pm
 */

#include <stdio.h>
#include <string.h>

#define SIZE 5
#define LENGTH 20

typedef union extra_info
{
	char phone[LENGTH];
	int youngest;
	float average;
} EXTRA_INFO; // To access union: waitlist[i].info. //

typedef struct entry 
{
	int groups, oldest;
	char names[LENGTH];
	EXTRA_INFO info;
} ENTRY;
ENTRY waitlist [SIZE]; // To access struct: waitlist[i]. //
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
	int i, x, inputsize, inputage;
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
			printf("Oldest age in the group:\n");
			scanf("%d", &inputage);
			if (inputage < 18)
			{
				printf("Parent or guardian's phone number:\n");
				scanf("%s", p[counter].info.phone);
			}
			else if (inputage > 65)
			{
				printf("Youngest age in the group:\n");
				scanf("%d", &p[counter].info.youngest);
			}
			else
			{
				printf("Average age of the group:\n");
				scanf("%f", &p[counter].info.average);
			}
			for (i=0; i<SIZE; i++)
			{
				if (p[i].names[0] == '\0')
				{
					strcpy (p[i].names, inputname);
					p[i].groups = inputsize;
					p[i].oldest = inputage;
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
				printf("%s's appointment for %d people has been removed.\n", p[i].names, p[i].groups);
				inputsize -= p[i].groups;
				hasGroups = 1;
				for (j=i; j<counter-1; j++)
				{
					strcpy (p[j].names, p[j+1].names);
					p[j].groups = p[j+1].groups;
					p[j].oldest = p[j+1].oldest;
					p[j].info = p[j+1].info;
				}
				counter--;
				p[counter].names[0] = '\0';
				p[counter].groups = '\0';
				p[counter].oldest = '\0';
				p[counter].info.phone[0] = '\0';
				p[counter].info.youngest = '\0';
				p[counter].info.average = '\0';
				i--; // Decrements counter and i, erases all the categories within the last element of array //
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
		for (i=0; i<SIZE; i++)
		{
			if (p[i].oldest < 18)
			{
				printf("Name: %s, Group size: %d, Oldest age: %d, Parent or guardian's phone number: %s\n", p[i].names, p[i].groups, p[i].oldest, p[i].info.phone);
			}
			else if (p[i].oldest > 65)
			{
				printf("Name: %s, Group size: %d, Oldest age: %d, Youngest age: %d\n", p[i].names, p[i].groups, p[i].oldest, p[i].info.youngest);
			}
			else
			{
				printf("Name: %s, Group size: %d, Oldest age: %d, Average age: %f\n", p[i].names, p[i].groups, p[i].oldest, p[i].info.average);
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
				if (p[i].oldest < 18)
				{
					printf("Name: %s, Group size: %d, Oldest age: %d, Parent or guardian's phone number: %s\n", p[i].names, p[i].groups, p[i].oldest, p[i].info.phone);
				}
				else if (p[i].oldest > 65)
				{
					printf("Name: %s, Group size: %d, Oldest age: %d, Youngest age: %d\n", p[i].names, p[i].groups, p[i].oldest, p[i].info.youngest);
				}
				else
				{
					printf("Name: %s, Group size: %d, Oldest age: %d, Average age: %f\n", p[i].names, p[i].groups, p[i].oldest, p[i].info.average);
				}
			}
		}
		if (hasGroups != 1)
		{
			printf("None found.\n");
		}
	}
	return;
}
