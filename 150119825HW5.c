// Merve Rana Kýzýl
// 150119825
// CSE1142- Homework 5

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include<time.h> 

typedef struct node {
	char songName[25];
	int duration;
	struct node *chrono_next;
	struct node *alpha_next;
	struct node *duration_next;
	struct node *random_next;
}Node;

// Convert duration to seconds
int duration(char *duration) {

	const char *colon = ":";
	char *c = duration;
	int songDuration = 0;

	// Extract the first token
	char * token = strtok(duration, ":");
	int flag = 1;

	while(token != NULL) {
		if(flag){
			songDuration = 60*atoi(token);
			flag = 0;

		}
		else
			songDuration += atoi(token);
			token = strtok(NULL, " ");

	}

	return songDuration;
}

 void copyArray(char copy[], char original[]){
	for(int i = 0; i < 25; i++)
		copy[i] = original[i];
 }

// Insert a new node at the end
void insertNode(Node** head, int duration, char songName[]) { 
	//Allocate node
	Node* new_node = (Node*) malloc(sizeof(Node)); 
  
	Node *last = *head;
  
	// Put in duration and the song name
	new_node->duration  = duration;
	copyArray(new_node->songName, songName);
  
	// Assign NULL to the last node
	new_node->duration_next = NULL; 
  
	// Check if the linked list is empty
	if (*head == NULL) { 
	   *head = new_node; 
	   return; 
	} 
  
	//Traverse till the last node
	while (last->duration_next != NULL) 
		last = last->duration_next; 
  
	// Insert a new node
	last->duration_next = new_node; 
	return; 
} 


// Print the contents of a linked list starting from head 
void printList(Node *node) {
	int seconds;
	int min;
	int i = 1;
 
  while (node != NULL) { 
	seconds = node->duration;
	min = seconds/60;
	seconds = seconds - min*60;	
	printf("\t%d.%s %d:%d\n",i, node->songName, min, seconds);
	node = node->duration_next;
	i++;
  } 
  
  printf("\n");
}

// Write to a text file
void output(FILE *f, Node *node){	
	int seconds;
	int min;
	int i = 1;
		
	if (f == NULL) {
	    printf("Error opening file!\n");
	    exit(1);
	}
  	while (node != NULL) { 
		seconds = node->duration;
		min = seconds/60;
		seconds = seconds - min*60;
		fprintf(f, "\t%d.%s %d:%d\n", i, node->songName, min, seconds);
		node = node->duration_next;
		i++;
  }
  
  fprintf(f, "\n"); 		  
}

// Swap data of two nodes
void swap(Node *node1, Node *node2) {	
	// Swap song names
	char temp[25];
	copyArray(temp, node1->songName);
	copyArray(node1->songName, node2->songName);
	copyArray(node2->songName, temp);
	
	// Swap durations
	int temp2 = node1->duration;
	node1->duration = node2->duration;
	node2->duration = temp2;
}

// Generate a random number
int random(int num){	
	srand(time(NULL));
	int random = rand()%num + num%rand();
	return random ;	
}

// Sort the listed link according to alphabetical order, 
// duration time order and random order
 void sortList(Node *head, int alpha, int shuffle) {
	int swapped;
	Node *ptr1;
	Node *lptr = NULL;

	// Check if the linked list is empty
	if (head == NULL)
		return;

	do {
		swapped = 0;
		ptr1 = head;

		while (ptr1->duration_next != lptr) {
			
			// Sort based on the names of the songs
			if(alpha){
				
				if (strcmp(ptr1->songName, ptr1->duration_next->songName) > 0) {
						swap(ptr1, ptr1->duration_next);
						swapped = 1;
				}
				
			}
			// Sort randomly
			else if(shuffle) {
				
				if(random(ptr1->duration) > ptr1->duration_next->duration) {
					
					swap(ptr1, ptr1->duration_next);
					swapped = 1;
				}		
			}
			// Sort based on the duration times of the songs		
			else {
			
				if (ptr1->duration > ptr1->duration_next->duration) {
					swap(ptr1, ptr1->duration_next);
					swapped = 1;
				}			
				
			}
			
			ptr1 = ptr1->duration_next;
		}
		
		lptr = ptr1;
	}
	while (swapped);
}

// Delete the first occurrence of song name in linked list
int deleteNode(Node **head, char* name) 
{ 
    // Store head node 
    Node* temp = *head, *prev; 
  
    // If head node itself holds the song name to be deleted 
    if (temp != NULL && strcmp(temp->songName, name) == 0) 
    { 
        *head = temp->duration_next;
        free(temp);               
        return 1; 
    } 
  
    // Search for the song name to be deleted 
    while (temp != NULL && strcmp(temp->songName, name) != 0) 
    { 
        prev = temp; 
        temp = temp->duration_next; 
    } 
  
    // If song name was not present in linked list 
    if (temp == NULL) 
		return 0; 
  
    // Unlink the node from linked list 
    prev->duration_next = temp->duration_next; 
    free(temp);  
	return 1; 
} 

void sortLists(Node* alpha_head, Node* duration_head, Node* random_head){
	
	sortList(alpha_head, 1, 0);
	sortList(duration_head, 0, 0);
	sortList(random_head, 0, 1);
}

void printLists(Node* chrono_head, Node* alpha_head, Node* duration_head, Node* random_head){
	
	printf("\nThe list in chronological order:\n");
	printList(chrono_head);	
	printf("The list in alphabetical order:\n");
	printList(alpha_head);
	printf("The list in duration-time order:\n");
	printList(duration_head);
	printf("The list in random order:\n");
	printList(random_head);
}


int main() { 
	Node* chrono_head = NULL;
	Node* duration_head = NULL;
	Node* alpha_head = NULL;
	Node* random_head = NULL;

	FILE *fp;
	char str[1000];
	char *filename = "songs.txt";
	char *pch, song[25];
	int d;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Could not open file %s", filename);
		return 1;
	}

	// Read from the given input file
	while (fgets(str, 1000, fp) != NULL) {
		
		// Split the string using strtok
		pch = strtok(str,  "	");
		while (pch != NULL) {
			
			if (pch){
							
				strcpy(song, pch);		
			}

			pch = strtok(NULL, "	");

			if (pch){

				d = duration(pch);
				insertNode(&chrono_head, d, song);
				insertNode(&duration_head, d, song);
				insertNode(&alpha_head, d, song);
				insertNode(&random_head, d, song);
								
			}

			pch = strtok(NULL, "	");
		}

		pch = strtok(NULL, "	");

	}
	
	sortLists(alpha_head, duration_head, random_head);
	printLists(chrono_head, alpha_head, duration_head, random_head);
	
	// Create a menu
	while(1) {
		
		int choice;
		printf("\nEnter your choice: \n");
		printf(" 1 to insert a song into the list.\n");
		printf(" 2 to delete a song from the list.\n");
		printf(" 3 to print the songs in the list.\n");
		printf(" 4 to print the songs to an output file.\n");
		printf(" 5 to end.\n");	
		scanf(" %d", &choice);
			
		if(choice == 1){
				
			char *songInfo =  malloc(40);
			char songname[25];
			printf("\nEnter a song name with duration:\n");
			scanf(" %[^\n]", songInfo);
			
			songInfo = strtok(songInfo, "	");
		
			if (songInfo){
							
				strcpy(songname, songInfo);		
			}

			songInfo = strtok(NULL, "	");
			
			if (songInfo){

				d = duration(songInfo);
				insertNode(&chrono_head, d, songname);
				insertNode(&duration_head, d, songname);
				insertNode(&alpha_head, d, songname);
				insertNode(&random_head, d, songname);
				sortLists(alpha_head, duration_head, random_head);
								
			}					
		}			
		else if(choice == 2){
			
			char songname[25];
			int flag = 0;	
			printf("Enter a song name:\n");
			scanf(" %[^\n]", songname);
			flag = deleteNode(&chrono_head, songname);
			deleteNode(&alpha_head, songname);
			deleteNode(&duration_head, songname);
			deleteNode(&random_head, songname);
			sortLists(alpha_head, duration_head, random_head);
			
			if(flag)
				printf("The song \"%s\" is deleted from the list!\n", songname);
			else
				printf("The song \"%s\" cannot be found.\n", songname);
					
		}
		else if(choice == 3){
			
			printLists(chrono_head, alpha_head, duration_head, random_head);	
		}
		else if(choice == 4){
			
			char filename[30];
			printf("Enter a file name:\n");
			scanf(" %[^\n]", filename);
			
			FILE *f = fopen(filename, "w");	
			fprintf(f, "The list in chronological order:\n");
			output(f, chrono_head);
			fprintf(f, "The list in alphabetical order:\n");
			output(f, alpha_head);
			fprintf(f, "The list in duration-time order:\n");
			output(f, duration_head);
			fprintf(f, "The list in random order:\n");
			output(f, random_head);
  			fclose(f);
				
		}
		else if(choice == 5)
			break;
			
		else
			printf("Invalid input");
			
	}
		
	fclose(fp);
	return 0;
} 
