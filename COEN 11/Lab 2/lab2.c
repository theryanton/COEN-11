/* Ryan Ton
 * COEN 11
 * Lab 2
 * Wednesday 5:15pm
 */

#include <stdio.h>
#include <string.h>

#define SIZE 5
#define LENGTH 20

int counter = 0;
int groups[SIZE];
char names[SIZE][LENGTH];

void insert();
void delete();
void list();

int main()
{
	int input;
	printf("Welcome to the Surf Lesson Scheduler!\n\n");

	while (1)
	{
		printf("(1) Schedule appointment.\n");
		printf("(2) Remove appointment.\n");
		printf("(3) List schedule.\n");
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
			default:
				printf("Command not valid. Input 1, 2, 3, or 0.\n");
				break;
			return 0;
		}
	}
}

void insert()
{
	int i, inputsize;
	char inputname[LENGTH];

	if (counter == SIZE)
	{
		printf("Sorry, no more appointments available.\n");
	}
	else
	{
		printf("Enter a name:\n");
		scanf("%s", inputname);
		printf("Enter group size:\n");
		scanf("%d", &inputsize);

		for (i=0; i<SIZE; i++)
		{
			if (names[i][0] == '\0')
			{
				strcpy (names[i], inputname);
				groups[i] = inputsize;
				counter++;
				printf("An appointment has been scheduled for %s for %d people.\n", inputname, groups[i]);
				break;
			}
		}
	}
	return;
}

void delete()
{
	int i, j, inputsize, hasGroups = 0;

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
			if (inputsize >= groups[i])
			{
				printf("%s's appointment with a group size of %d has been removed.\n", names[i], groups[i]);
				inputsize -= groups[i];
				hasGroups = 1;

				for (j=i; j<counter; j++)
				{
					strcpy (names[j], names[j+1]);
					groups[j] = groups[j+1];
				}
				i--;
				counter--;
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
			if (names[i][0] == '\0')
			{
				printf("free\n");
			}
			else
			{
				printf("%s	%d\n", names[i], groups[i]);
			}
		}
	}
	return;
}
