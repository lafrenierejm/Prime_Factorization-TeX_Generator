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
void freeList(primeT *);
// Factor the key
void factorKey(primeT *, unsigned long, FILE *);
// Find the next prime number
primeT * findNextPrime(primeT *, primeT *, unsigned long);

/*
 * main
 * Main method
 */
int
main(int argc, char *argv[])
{
	/* User should have provided key and filename arguments */
	if (argc != 3)
	{
		printf("Usage: %s key filename\n",
				argv[0]
		      );
		exit(1);	// Exit with code 1
	}

	/* Open file */
	FILE *outputFile = fopen(argv[2], "w");
	if (NULL == outputFile)
	{
		perror("An error occured while opening the file\n");
		exit(EXIT_FAILURE);
	}

	/* Set key value */
	unsigned long key = strtoul(argv[1], NULL, 10);
	if (key == 1)
	{
		printf("1 is not a prime number\n");
		exit(EXIT_SUCCESS);
	}

	primeT *head = makeNode(2);	// Start list of primes at 2
	factorKey(head, key, outputFile);	// Factor the key
	freeList(head);	// Free the linked list

	/* Finish and close the file */
	fprintf(outputFile, "\n");
	fclose(outputFile);

	/* Exit successfully */
	exit(EXIT_SUCCESS);
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
 * freeList
 * Clean up linked list
 */
void
freeList(primeT *head)
{
	primeT *ptr;

	/* Create primeT pointer at second prime */
	if (head->next)
		ptr = head->next;	// Set ptr to second node if it exists
	else
		ptr = head;		// Else set ptr to head

	/* Free memory from head to tail */
	while (ptr->next)
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
factorKey(primeT *head,
		unsigned long key,
		FILE *outputFile)
{
	primeT *curNode = head;		// Set curNode to head
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
		/* There is now at least one factor */
		isFirstFactor = false;
	}

	/* Continue factoring while key contains prime factors */
	while (key > 1)
	{
		exponent = 0;	// Reset exponent
		curNode = findNextPrime(head, curNode, key);	// Find next prime

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
			fprintf(outputFile, " %lu^{%u}",
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
primeT *
findNextPrime(primeT *head,
		primeT *tail,
		unsigned long key)
{
	unsigned long tmp;	// Values to test for primeness
	primeT *newNode;	// New node to return

	/* Find first test value for curPrime */
	if (tail->value == 2)
		tmp = 3;	// If second node in list, then 3
	else
		tmp = tail->value + 2;	// Else largest prime + 2

	/* Test tmp for primeness while tmp is not greater than key */
	while (tmp <= key)
	{
		primeT *position = head;	// Set position to beginning of list
		bool isPrime = true;	// Assume that tmp is prime

		/* Test every number in list to see if tmp is really prime */
		while (isPrime && position->next)
		{
			if (tmp % position->value == 0)
				isPrime = false;	// tmp is not prime
			else
				position = position->next;	// Go to next prime
		}
		if (isPrime && (tmp % position->value == 0))	// Check last value
			isPrime = false;	// tmp is not prime

		/* If tmp is prime then add node with its value and return */
		if (isPrime)
		{
			newNode = makeNode(tmp);	// Create new node with value tmp
			tail->next = newNode;		// Point tail to newNode
			break;	// Exit loop
		}

		/* Else increase tmp by 2 and test again */
		tmp += 2;	// Increment tmp by 2
	}

	/* Return newNode to caller */
	return newNode;
}
