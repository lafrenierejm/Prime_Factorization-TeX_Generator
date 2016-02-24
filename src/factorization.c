#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Define node used to store factors */
typedef struct factorNode
{
	unsigned long prime;		/* Value of prime */
	unsigned int exponent;		/* Factor's exponent */
	struct factorNode* next;	/* Pointer to next node */
} factorNodeT;

/*
 * Function prototypes
 */
/* Factor the key */
void factorKey(unsigned long, FILE *);
/* Create a new node */
factorNodeT * makeNode(unsigned long, unsigned int, factorNodeT *);
/* Delete a list of nodes */
void freeList(factorNodeT *);
/* Print the current list of factors */
void printFactors(factorNodeT *, unsigned long, FILE *);

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
				argv[0]);
		exit(1);	/* Exit with code 1 */
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
		fprintf(outputFile, "\\(1\\) is not a prime number.\n");
		exit(EXIT_SUCCESS);
	}

	/* Start the file output */
	fprintf(outputFile, "\\begin{align*}\n%lu ",
			key);

	/* Factor the key */
	factorKey(key, outputFile);

	// Finish the file output and close the file
	fprintf(outputFile, "\\end{align*}\n");
	fclose(outputFile);

	/* Exit successfully */
	exit(EXIT_SUCCESS);
}

/*
 * factorKey
 * Find the prime factorization of the given key value
 * Write factors to the output file as they are found
 */
void
factorKey(unsigned long key, FILE *outputFile)
{
	unsigned long posFactor;	/* Possible factors */
	unsigned int exponent;		/* Number of occurrences of factor */
	factorNodeT *head = NULL;	/* First node in list */
	factorNodeT *tail = NULL;	/* Last node in list */

	/* Factor the key while it
	 * contains prime factors and
	 * is greater than or equal to square of factor
	 */
	posFactor = 2;	/* Start at first prime */
	do
	{
		/* Reset exponent */
		exponent = 0;

		/* See if posFactor is an actual factor */
		while ((key % posFactor == 0) && (key > 0))
		{
			exponent++;			/* Increment exponent with each factor */
			key /= posFactor;	/* Divide key by current factor */
		}

		/* If there were any multiples of the factor, create a node */
		if (exponent > 0)
		{
			printf("Factor = %lu\n", posFactor);
			printf("Exponent = %u\n", exponent);
			tail = makeNode(posFactor, exponent, tail);	/* Create the new node */
			printf("Node created successfully\n");
			/* Set head to the new node if it was previously empty */
			if (NULL == head)
			{
				printf("Reassigning the head node\n");
				head = tail;
			}
			/* Print the factors */
			printFactors(head, key, outputFile);
		}

		/* Find the next possible factor */
		if (posFactor == 2)
			posFactor = 3;	/* If posFactor was previously 2, increase to 3 */
		else
			posFactor += 2;	/* Else increase by 2 */
	} while (key > 1 && (posFactor * posFactor <= key));

	/*
	 * If the key has not yet been fully factored,
	 * it must be prime itself
	 */
	if (key != 1)
	{
		tail = makeNode(key, 1, tail);	/* Create the new node */
		key = 1;	/* Set key to 1 to mark finished */

		/* Set head to the new node if it was previously empty */
		if (NULL == head)
			head = tail;

		/* Print the factors */
		printFactors(head, key, outputFile);
	}

	/* Free the linked list */
	printf("Now freeing list\n");
	freeList(head);

	/* Return void to caller */
	return;
}

/*
 * makeNode
 * Create a new node
 */
factorNodeT *
makeNode(unsigned long pr, unsigned int exp, factorNodeT *prevNode)
{
	factorNodeT *newNode = (factorNodeT *)malloc(sizeof(factorNodeT));

	/* Test for failed allocation of newNode */
	if (NULL == newNode)
	{
		perror("Creation of new node failed");
		return NULL;
	}

	/* Assign values */
	newNode->prime = pr;
	newNode->exponent = exp;
	newNode->next = NULL;	/* Point to NULL */

	/* Point forward to this node */
	if (NULL != prevNode)
		prevNode->next = newNode;

	/* Return newNode to caller */
	printf("Returning from makeNode\n");
	return newNode;
}

/*
 * freeList
 * Free a list of nodes */
void
freeList(factorNodeT *head)
{
	factorNodeT *curNode;

	/* Free nodes starting at beginning of list */
	curNode = head;
	while (NULL != curNode)
	{
		head = curNode;
		curNode = curNode->next;	/* Advance to next node */
		free(head);
	}

	/* Return void to caller */
	return;
}

/*
 * printFactors
 * Print the current list of factors */
void
printFactors(factorNodeT *head, unsigned long key, FILE *outputFile)
{
	factorNodeT *curFactor;		/* Current factor to print */
	bool isFirstFactor;			/* Whether or not curFactor is first factor */

	/* Print the start of the new line */
	fprintf(outputFile, "&= ");

	curFactor = head;	/* Start at head */
	isFirstFactor = true;
	while (NULL != curFactor)
	{
		/* Start by multiplying if not first factor */
		if (!isFirstFactor)
			fprintf(outputFile, " \\times{}");

		/* Print the current factor */
		fprintf(outputFile, " %lu^{%u}",
				curFactor->prime,
				curFactor->exponent);

		/* Have at least one factor now */
		isFirstFactor = false;

		/* Get the next node */
		curFactor = curFactor->next;
	}

	/* Print key if it is not fully factored */
	if (key != 1)
	{
		/* Start by multiplying if not first factor */
		if (!isFirstFactor)
			fprintf(outputFile, " \\times{}");

		/* Print the key */
		fprintf(outputFile, " %lu",
				key);
	}

	/* End the current line in outputFile */
	if (key != 1)
		fprintf(outputFile, "\\\\");
	fprintf(outputFile, "\n");

	/* Return void to caller */
	return;
}
