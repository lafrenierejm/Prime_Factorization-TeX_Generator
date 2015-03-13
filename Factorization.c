#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>	// For boolean data type

// Define node for linked list
struct node {
	unsigned long value;	// Holds data of this element
	struct node *next;
};

// Function prototypes
struct node* makeNode(unsigned long num);	// Create new node
void makeListOfPrimes(struct node *head, unsigned long max); // Construct linked list of prime numbers which are possible factors of max
void outputFactorization(struct node *head, unsigned long key, FILE *outputFile);	// Write factorization to file

// Main method
main() {
	// Variables
	unsigned long key;	// Value to factorize
	struct node *head;	// Points to first node in list
	FILE *outputFile = fopen("Output.tex", "w");

	// Get user input for value of key
	printf("Enter a positive integer: ");
	scanf("%lu", &key);

	// Write prefix to file
	fprintf(outputFile, "\\( = ");
	
	head = makeNode(2);		// Allocate memory for head node
	head->next = makeNode(0);	// Allocate memory for end node
	makeListOfPrimes(head, key);	// Create list of primes which are possible factors of key
	outputFactorization(head, key, outputFile);
	
	// Write suffix to file
	fprintf(outputFile, " \\)");

	return(EXIT_SUCCESS);	// Exit successfully
}

// Create new node
struct node* makeNode(unsigned long num) {
	struct node *newNode;
	newNode = (struct node*)malloc(sizeof(struct node));

	if(newNode == NULL) {	// Insufficient memory
		printf("Node creation failed\n.");
		return NULL;
	}

	newNode->value = num;
	newNode->next = NULL;
}

// Clean up linked list
void freeMemory(struct node *head) {
	struct node *ptr;

	ptr = head->next; // Free memory from head up to tail
	while(ptr->value != 0) {
		free(head);	// Free head
		head = ptr;	// Advance head
		ptr = ptr->next;// Advance ptr through linked list
	}
	head = ptr;	// Head is tail
	free(head);	// Free final node

	return;	// Return void
}

// Construct linked list all prime numbers which are possible factors of max
void makeListOfPrimes(struct node *head, unsigned long max) {
	struct node *currentPosition;	// Will point to end of list
	struct node *loopPosition;	// Used to cycle through list
	unsigned long tmp;
	bool isPrime;

	currentPosition = head;
	// Set currentPosition to end of list
	while(currentPosition->value != 0)
		currentPosition = currentPosition->next;

	tmp = 3;	// Begin testing numbers at 3
	while(tmp < max/2) {
		loopPosition = head;	// Reset loopPosition to beginning of list
		isPrime = true;		// Assume that tmp is prime
		while(isPrime && loopPosition->value != 0) {
			if(tmp % loopPosition->value == 0)
				isPrime = false;	// tmp is not prime
			else
				loopPosition = loopPosition->next;	// Go to next node
		}
		if(isPrime) {
			currentPosition->value = tmp;		// Set last node to this prime
			currentPosition->next = makeNode(0);	// Add new end node
			currentPosition = currentPosition->next;	// Move to that node
		}
		tmp += 2;	// Increment tmp by 2
	}
}

// Output prime factorizaiton
void outputFactorization(struct node *head, unsigned long key, FILE *outputFile) {
	int exponent;			// Contains exponent of each factor
	bool firstFactor = true;	// Set to false after first factor is found
	struct node *currentNode;	// Node pointer

	currentNode = head;	// Point to beginning of list

	// If 1, output 1 as factor
	if(key == 1)
		fprintf(outputFile, "1");

	// Loop while still looking for factors and still in list
	while(key != 0 && currentNode->value != 0) {
		exponent = 1;	// Reset exponent

		// See if value in currentNode divides key
		if(key % currentNode->value == 0) {
			// If not first factor, print multiplication sign
			if(!firstFactor)
				fprintf(outputFile, "*");

			fprintf(outputFile, "%lu", currentNode->value);	// Output new factor

			while(key % currentNode->value == 0) {
				exponent++;			// Increment exponent of factor
				key /= currentNode->value;	// Divide key by currentNode
			}

			fprintf(outputFile, "^{%i}", exponent - 1);	// Output power of factor
			firstFactor = false;			// At least one factor has been found
		}	
		currentNode = currentNode->next;		// Advance to next node in list
	}	
}
