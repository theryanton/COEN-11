/* Ryan Ton
 * COEN 11
 * Lab 7
 * Wednesday 5:15pm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 20
#define NODE struct node

struct node 
{
	int groups;
	char names[LENGTH];
	NODE *next;
};
struct waitlist
{
	NODE *head;
	NODE *tail;
};
struct waitlist list[4] = {{NULL,NULL},{NULL,NULL},{NULL,NULL},{NULL,NULL}};

void read_file(char *fname);
void save_file(char *fname);
void insert(char *inputname, int inputsize);
int check_duplicate(char *inputname);
void delete();
void show();
void search_size();
void clear_all();

int main(int argc, char *argv[])
{
	int x, input, inputsize;
	char inputname[LENGTH];
	printf("Welcome to the Surf Lesson Scheduler!\n\n");

	if(argc == 1)
	{
		printf("There's no data currently found. Initiating new waitlist.\n");
	}
	else
	{
		read_file(argv[1]);
	}
	while (1)
	{
		printf("(1) Schedule appointment.\n");
		printf("(2) Remove appointment.\n");
		printf("(3) List schedule by range.\n");
		printf("(4) Search using group sizes.\n");
		printf("(0) Exit.\n\n");
		scanf("%d", &input);

		switch (input)
		{
			case 0: // End program, freeing all nodes
				if (argc == 1)
				{
					save_file("init");
					printf("Successfully saved waitlist.\n");
				}
				else // Update file
				{
					save_file(argv[1]);
					printf("Successfully updated waitlist.\n");
				}
				clear_all();
				return 1;	
			case 1: // Inserts names into according arrays
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
				}
				insert(inputname, inputsize);
				break;
			case 2: // Removes specified appointments from list according to size
				delete();
				break;
			case 3: // Lists entire waitlist sorted by range
				show();
				break;
			case 4: // Searches schedule for appointments with group sizes less or equal to user input
				search_size();
				break;
			default:
				printf("Command not valid. Input 1, 2, 3, 4, or 0.\n");
				break;
			return 0;
		}
	}
}

void read_file(char *fname)
{
	FILE *inputfile;
	int x, inputsize;
	char inputname[LENGTH];

	if ((inputfile = fopen(fname, "r")) == NULL)
	{
		printf("Error loading waitlist, try again.\n");
		return;
	}
	fseek(inputfile,32,SEEK_SET); // 4 arrays, 8 bytes per node
	while(fscanf(inputfile,"%s%d",inputname,&inputsize) == 2)
	{
		x = check_duplicate(inputname); // Checks and skips over any duplicate names
		if (x)
		{
			continue;
		}
		insert(inputname, inputsize);
	}
	printf("Waitlist successfully loaded.\n");
	show();
	fclose(inputfile);
	return;
}

void save_file(char *fname)
{
	FILE *outputfile;
	int i;
	NODE *p;

	if ((outputfile = fopen(fname, "w")) == NULL) // Checks if file is able to be opened and read
	{
		printf("Error saving waitlist, try again.\n");
		return;
	}
	fprintf(outputfile,"Name:\tGroup Size:\n-------------\n"); // Printing labels for each category
	for (i=0; i<4; i++)
	{
		p = list[i].head;
		while (p != NULL)
		{
			fprintf(outputfile, "%s\t%d\n", p->names, p->groups);
			p = p->next;
		}
	}
	fclose(outputfile);
	return;
}
	
void insert(char *inputname, int inputsize)
{
	NODE *p;
	p = (NODE *) malloc (sizeof (NODE)); // Declaring new node for insert

	if (inputsize == 1 || inputsize == 2) // Checking each user input, and placing each of them in the according range
	{
		if (list[0].head == NULL)
		{
			list[0].head = list[0].tail = p;
		}
		else
		{
			list[0].tail->next = p;
		}
		strcpy (p->names, inputname);
		p->groups = inputsize;
		p->next = NULL;
		list[0].tail = p;
	}
	else if (inputsize == 3 || inputsize == 4)
	{
		if (list[1].head == NULL)
		{
			list[1].head = list[1].tail = p;
		}
		else
		{
			list[1].tail->next = p;
		}
		strcpy (p->names, inputname);
		p->groups = inputsize;
		p->next = NULL;
		list[1].tail = p;
	}
	else if (inputsize == 5 || inputsize == 6)
	{
		if (list[2].head == NULL)
		{
			list[2].head = list[2].tail = p;
		}
		else
		{
			list[2].tail->next = p;
		}
		strcpy (p->names, inputname);
		p->groups = inputsize;
		p->next = NULL;
		list[2].tail = p;
	}
	else if (inputsize > 6)
	{
		if (list[3].head == NULL)
		{
			list[3].head = list[3].tail = p;
		}
		else 
		{
			list[3].tail->next = p;
		}
		strcpy (p->names, inputname);
		p->groups = inputsize;
		p->next = NULL;
		list[3].tail = p;
	}
	printf("An appointment has been scheduled for %s for %d people.\n", inputname, inputsize);
	return;
}

int check_duplicate (char inputname[LENGTH])
{
	int i;
	NODE *p;
	
	for (i=0; i<4; i++) // Checks through each node within every range for a match between names
	{
		p = list[i].head;
		while (p != NULL)
		{
			if (strcmp (inputname, p->names) == 0)
			{
				return 1;
			}
			p = p->next; // Continue iterating through each node within the range until reaching the tail
		}
	}
	return 0;
}

void delete()
{
	int i, inputsize;
	NODE *p, *q;

	if (list[0].head == NULL && list[1].head == NULL && list[2].head == NULL && list[3].head == NULL)
	{
		printf("The schedule is currently empty.\n");
	}
	else
	{
		printf("Enter the group size you want to remove:\n");
		scanf("%d", &inputsize);
		for (i=0; i<inputsize; i++) // Iterate a specific range given a user input
		{
			p = q = list[i].head;
			while (p != NULL)
			{
				if (inputsize >= p->groups)
				{
					printf("%s's appointment with %d people has been removed.\n", p->names, p->groups);
					inputsize -= p->groups; // Iterating through nodes, checking if user input matches any element, shifts remaining appointments up
					if (p->next == NULL)
					{ 
						list[i].head = NULL;
						list[i].tail = NULL;
						free (p);
						break;
					}
					else	if (p == list[i].head)
					{
						list[i].head = p->next;
						free (q);
						q = list[i].head;
						p = list[i].head;
					}
					else if (p == list[i].tail)
					{
						list[i].tail = q;
						q->next = NULL;
						free (q);
						break;
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
					printf("None found.\n");
				}
			}
		}
	}
	return;
}

void show()
{
	int i;
	NODE *p;

	if (list[0].head == NULL && list[1].head == NULL && list[2].head == NULL && list[3].head == NULL)
	{
		printf("The schedule is currently empty.\n");
	}
	else
	{
		printf("Range 1: Group sizes of 1-2.\n");
		printf("Range 2: Group sizes of 3-4.\n");
		printf("Range 3: Group sizes of 5-6.\n");
		printf("Range 4: Group sizes larger than 6.\n\n");

		for (i=0; i<4; i++) // Iterate through all range lists with i
		{
			p = list[i].head;
			while (p != NULL)
			{
				printf("Range: %d Name: %s Group Size: %d\n", i+1, p->names, p->groups);
				p = p->next; // Continue iterating through each node within that range until reaching the tail
			}
		}
	}
	return;
}

void search_size()
{
	int i, inputsize, hasGroups = 0;
	NODE *p;

	if (list[0].head == NULL && list[1].head == NULL && list[2].head == NULL && list[3].head == NULL)
	{
		printf("The schedule is currently empty.\n");
	}
	else
	{
		printf("Enter a group size you want to search for:\n");
		scanf("%d", &inputsize);

		for (i=0; i<inputsize; i++)
		{
			p = list[i].head;
			while (p != NULL)
			{
				if (p->groups <= inputsize)
				{
					hasGroups = 1; // Flag denotes that there are group sizes less than or equal to user input
					printf("Name: %s Group Size: %d\n", p->names, p->groups);
				}
				p = p->next;
			}
		}
		if (hasGroups != 1)
		{
			printf("None found.\n");
		}
	}
	return;
}

void clear_all()
{
	int i;
	NODE *p, *q;

	for (i=0; i<4; i++) // Iterate through each range with i
	{
		p = q = list[i].head; // Set p and q equal to head
		while (p != NULL)
		{
			q = q->next;
			free (p);
			p = q; // Iterating through and setting each element to head, then clearing it
		}
	}
	return;
}
