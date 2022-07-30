/* Ryan Ton
 * COEN 11
 * Lab 9
 * Wednesday 5:15pm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

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
pthread_mutex_t lock;
struct waitlist list[4] = {{NULL,NULL},{NULL,NULL},{NULL,NULL},{NULL,NULL}};

void read_file(char *fname);
void save_file(char *fname);
void insert(char *inputname, int inputsize);
int check_duplicate(char *inputname);
void delete();
void show();
void search_size();
void clear_all(NODE *p);
void reverse_list(int i, NODE *p);
void reverse_word(char *list);
void reverse_names(NODE *p);
void *auto_save(void *arg);
void read_auto_saved(char *fname);

int main(int argc, char *argv[])
{
	NODE *p;
	int i, x, userinput, inputsize;
	char inputname[LENGTH];
	pthread_t thread;
	printf("Welcome to the Surf Lesson Scheduler!\n");

	if(argc == 1)
	{
		printf("There's no data currently found.\n");
	}
	else
	{
		pthread_mutex_init(&lock, NULL);
		pthread_create(&thread, NULL, auto_save, (void *) argv[2]);
		read_file(argv[1]);
	}
	while (1)
	{
		printf("\n(1) Schedule appointment.\n");
		printf("(2) Remove appointment.\n");
		printf("(3) List schedule by range.\n");
		printf("(4) Search using group sizes.\n");
		printf("(5) Show each list backwards.\n");
		printf("(6) Show each name backwards.\n");
		printf("(7) Read waitlist from binary file.\n");
		printf("(0) Exit.\n\n");
		scanf("%d", &userinput);

		switch (userinput)
		{
			case 0: // End program, freeing all nodes
				if (argc == 1)
				{
					pthread_mutex_lock(&lock);
					pthread_cancel(thread);
					pthread_mutex_unlock(&lock);
					save_file("init");
					printf("Successfully saved waitlist.\n");
				}
				else // Update file
				{
					pthread_mutex_lock(&lock);
					pthread_cancel(thread);
					pthread_mutex_unlock(&lock);
					save_file(argv[1]);
					printf("Successfully updated waitlist.\n");
				}
				for (i=0; i<4; i++)
				{
					p = list[i].head;
					clear_all (p); // Frees head node, which starts chain reaction, freeing the rest in the waitlist
				}
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
					insert (inputname, inputsize);
				}
				break;
			case 2: // Removes specified appointments from list according to size
				pthread_mutex_lock(&lock);
				delete();
				pthread_mutex_unlock(&lock);
				break;
			case 3: // Lists entire waitlist sorted by range
				show();
				break;
			case 4: // Searches schedule for appointments with group sizes less or equal to user input
				search_size();
				break;
			case 5: // Reverses the order of entire waitlist
				printf("Range 1: Group sizes of 1-2.\n");
				printf("Range 2: Group sizes of 3-4.\n");
				printf("Range 3: Group sizes of 5-6.\n");
				printf("Range 4: Group sizes larger than 6.\n\n");
				printf("Range:\tName:\tGroup Size:\n-------------------------\n");
				for (i=0; i<4; i++) // Iterates through all lists
				{
					p = list[i].head;
					reverse_list (i, p);
				}
				break;
			case 6: // Reverses the order of spelling for all names in waitlist
				printf("Name:\n");
				printf("----------\n");
				for (i=0; i<4; i++) // Iterates through all lists
				{
					p = list[i].head;
					reverse_names (p);
				}
				break;
			case 7: // Reads autosaved binary file and displays contents
				read_auto_saved(argv[2]);
				break;
			default:
				printf("Command not valid. Input 1, 2, 3, 4, 5, 6, 7 or 0.\n");
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
	fprintf(outputfile,"Name:\tGroup Size:\n--------------\n"); // Printing labels for each category
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

	pthread_mutex_lock(&lock);
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
	pthread_mutex_unlock(&lock);
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
						free (p);
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

void clear_all(NODE *p)
{
	if (p == NULL)
	{
		return;
	}
	clear_all(p->next);
	free (p);
}

void reverse_list (int j, NODE *p)
{
	if (p == NULL)
	{
		return;
	}
	reverse_list (j, p->next);
	printf("%d\t%s\t%d\n", j+1, p->names, p->groups);
}

void reverse_word (char *list) // Function used to reverse each name in each list
{
	if (*list == '\0')
	{
		return;
	}
	reverse_word (list+1);
	printf("%c", *list);
}

void reverse_names (NODE *p) // Function is passed reversed names from previous function and used to display them
{
	if (p == NULL)
	{
		return;
	}
	reverse_word (p->names);
	printf("\n");
	reverse_names (p->next);
	return;
}

void *auto_save (void *arg)
{
	FILE *outputfile;
	NODE *p;
	int i;

	while(1)
	{
		sleep(15);
		if((outputfile = fopen((char*) arg, "wb")) == NULL) // Checks if file is corrupt or does not exist
		{
			printf("Error loading autosave file, try again.\n");
		}
		else
		{
			pthread_mutex_lock(&lock);
			for (i=0; i<4; i++)
			{
				p = list[i].head;
				while (p != NULL)
				{
					fwrite(p, sizeof(NODE), 1, outputfile);
					p = p->next;
				}
			}
			printf("Autosaved!\n");
			pthread_mutex_unlock(&lock);
		}
		fclose(outputfile);
	}
}

void read_auto_saved(char *fname)
{
	FILE *inputfile;
	NODE p;

	pthread_mutex_lock(&lock);
	if((inputfile = fopen(fname, "rb")) == NULL) // Checks if file is corrupt or does not exist
	{
		printf("Error loading binary file, try again.\n");
		pthread_mutex_unlock(&lock);
		return;
	}
	pthread_mutex_unlock(&lock);
	while (fread(&p, sizeof(NODE), 1, inputfile) == 1)
	{
		printf("Name: %s Group Size: %d\n", p.names, p.groups);
	}
	fclose(inputfile);
	pthread_mutex_unlock(&lock);
	return;
}
