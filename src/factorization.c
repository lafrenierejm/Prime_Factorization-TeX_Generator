#include <stdio.h>	// I/O
#include <stdlib.h>
#include <stdbool.h>	// bool data type

/* Define prime for linked list */
typedef struct prime
{
	unsigned long value;	// Holds prime's data
	struct prime *next;
} primeT;

/* Function prototypes */
// Create new prime
primeT * makeNode(unsigned long);
// Free list
void freeMemory(primeT *);
// Factor the key
void factorKey(unsigned long, FILE *);
// Find the next prime number
unsigned long findNextPrime(primeT *, unsigned long);

/*
 * main
 * Main method
 */
int
main()
{
	/* Variables */
	unsigned long key;	// Value to factorize
	primeT *head;		// Points to first prime in list
	FILE *outputFile = fopen("Output.tex", "w");

	/* Get user input for value of key */
	printf("Enter a positive integer: ");
	scanf("%lu", &key);

	head = makeNode(2);		// Allocate memory for head prime
	head->next = makeNode(0);	// Allocate memory for end prime
	makeListOfPrimes(head, key);	// Create list of primes which are possible factors of key
	outputFactorization(head, key, outputFile);

	return(EXIT_SUCCESS);	// Exit successfully
}

/*
 * makeNode
 * Create new node
 */
primeT *
makeNode(unsigned long num)
{
	/* malloc a new primeT instance */
	primeT *newNode = (primeT*)malloc(sizeof(primeT));

	/* Test for successful allocation of newPrime */
	if (newNode == NULL)
	{
		printf("primeT creation failed\n.");
		return NULL;
	}

	/* Assign initial values to node */
	newNode->value = num;	// Passed value
	newNode->next = NULL;	// Point to NULL

	/* Return the new primeT */
	return newNode;
}

/*
 * freeMemory
 * Clean up linked list
 */
void
freeMemory(primeT *head)
{
	/* Create primeT pointer at second prime */
	primeT *ptr = head->next;

	/* Free memory from head to tail */
	while (ptr->value != 0)
	{
		free(head);		// Free head
		head = ptr;		// Advance head
		ptr = ptr->next;	// Advance ptr through linked list
	}
	head = ptr;	// Head is now the tail
	free(head);	// Free final node

	/* Return void */
	return;
}

/*
 * factorKey
 * Find the prime factorization of the given key value
 * Write factors to the output file as they are found
 */
void
factorKey(unsigned long key,
		FILE *outputFile)
{
	primeT *curNode = makeNode(2);	// Smallest prime number is 2
	primeT *head = curNode;		// Set head to curNode
	unsigned int exponent = 0;
	bool isFirstFactor = true;	// If true then write \cdot{} to file

	/* Divide by 2 */
	while ((key % 2 == 0) && (key > 0))
	{
		exponent++;	// Increment exponent with each factor
		key = key/2;	// Divide key by curNode->value
	}
	/* If there were any factors, write them to outputFile */
	if (exponent > 0)
	{
		fprintf(outputFile, " 2^{%u}",
				exponent
		       );
		/* Have at least one factor */
		isFirstFactor = false;
	}

	/* Continue factoring while key is positive */
	while (key != 0)
	{
		exponent = 0;	// Reset exponent
		curNode = nextPrime(head, curNode, key);	// Find next prime

		/* Divide by the found prime */
		while ((key % curNode->value == 0) && (key > 0))
		{
			exponent++;	// Increment exponent with each factor
			key = key/curNode->value;	// Divide key by current prime
		}
		/* If there were any factors, write them to outputFile */
		if (exponent > 0)
		{
			/* Write \cdot{} if not first factor */
			if (!isFirstFactor)
				fprintf(outputFile, " \\cdot{}");
			/* Write factor */
			fprintf(outputFile, " %lu%{%u}",
					curNode->value,
					exponent
			       );
			/* Have at least one factor */
			isFirstFactor = false;
		}
	}

	/* Return void */
	return;
}

/*
 * nextPrime
 * Find the next prime number givena a list of primes starting at 2
 */
nodeT *
nextPrime(nodeT *head,
		nodeT *tail,
		unsigned long key)
{
	curPrime = tail->value;	// Current prime is at end of list
	unsigned long tmp;

	/* Find first test value for curPrime */
	if (curPrime == 2)
		tmp = 3;
	else
		tmp = curPrime + 2;

	/* Test tmp for primeness while tmp <= key */
	while (tmp <= key)
	{
		nodeT *position = head;	// Set position to beginning of list
		isPrime = true;		// Assume that tmp is prime
		while (isPrime && loopPosition->value != 0)
		{
			if (tmp % loopPosition->value == 0)
				isPrime = false;	// tmp is not prime
			else
				loopPosition = loopPosition->next;	// Go to next prime
		}
		if (isPrime)
		{
			currentPosition->value = tmp;		// Set last prime to this prime
			currentPosition->next = makeNode(0);	// Add new end prime
			currentPosition = currentPosition->next;	// Move to that prime
		}
		tmp += 2;	// Increment tmp by 2
	}
}
