#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>	// For boolean data type

// Define node for linked list
struct node {
	unsigned long value;	// Holds data of this element
	struct node *next;
};

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
	int exponent;
	bool didFactor;	// Set to true if a prime factor was found
	struct node *currentNode;

	fprintf(outputFile, "1^{1}");
	currentNode = head;
	do {
		// Reset variables
		didFactor = false;
		exponent = 1;

		// Loop while still looking for factor and still in list
		while(!didFactor && currentNode->value != 0) {
			// See if value in currentNode divides key
			if(key % currentNode->value == 0) {
				fprintf(outputFile, "*%lu", currentNode->value);	// Output new factor
				while(key % currentNode->value == 0) {
					exponent++;			// Increment exponent of factor
					key /= currentNode->value;	// Divide key by currentNode
				}
				fprintf(outputFile, "^{%i}", exponent - 1);	// Output power of factor
				didFactor = true;			// Proceed to next factorization loop
			}	
			currentNode = currentNode->next;		// Advance to next node in list
		}
	}while(didFactor && key > 1);	
}

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
