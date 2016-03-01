#include <ctype.h>	/* isprint () */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>	/* boolean data type */
#include <unistd.h>	/* getopt () */

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
/* Print the proper use of the program */
void printUse (char *);
/* Factor the key */
void factorKey (unsigned long, bool);
/* Create a new node */
factorNodeT * makeNode (unsigned long, unsigned int, factorNodeT *);
/* Delete a list of nodes */
void freeList (factorNodeT *);
/* Print the current list of factors */
void printFactors (factorNodeT *, unsigned long, bool);

/*
 * main
 * Main method
 */
int
main (int argc, char **argv)
{
	bool flagSteps = false;	/* true if user wants step-by-step */

	/* Get command options */
	opterr = 0;
	printf("optind = %i\n", optind);
	int arg = getopt (argc, argv, "s:");
	while (arg != -1)
	{
		switch (arg)
		{
			case 's':
				flagSteps = true;
				break;
			default:
				fprintf (stderr, "Argument found: %i.\n", optopt);
				printUse (argv[0]);
				exit (1);
		}
		arg = getopt (argc, argv, "s:");
	}

	printf("arg = %i; optind = %i\n", argc, optind);
	/* Process remaining, non-option arguments */
	if (optind ==1 && optind >= argc)	/* If not exactly one arguments */
	{
		printUse (argv[0]);
		exit (1);
	}
	/* Set key value */
	unsigned long key = strtoul (argv[argc - 1], NULL, 10);
	fprintf (stdout, "Key = %lu\n", key);
	if (key == 1)
	{
		exit (EXIT_SUCCESS);
	}

	/* Start the output */
	fprintf (stdout, "\\begin{align*}\n%lu%%\n",
			key);

	/* Factor the key */
	factorKey (key, flagSteps);

	/* End the output */
	fprintf (stdout, "\\end{align*}\n");

	/* Exit successfully */
	exit(EXIT_SUCCESS);
}

/*
 * printUse
 * Print the proper use of the program
 */
void
printUse (char *exe_path)
{
	/* Print usage message */
	fprintf (stdout, "Usage: %s [-s] KEY\n",
			exe_path);

	/* Return void to caller */
	return;
}


/*
 * factorKey
 * Find the prime factorization of the given key value
 * Write factors to the output file as they are found
 */
void
factorKey (unsigned long key, bool showSteps)
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
			tail = makeNode(posFactor, exponent, tail);	/* Create the new node */
			/* Set head to the new node if it was previously empty */
			if (NULL == head)
				head = tail;
			/* Print the factors */
			printFactors(head, key, showSteps);
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
		printFactors(head, key, showSteps);
	}

	/* Free the linked list */
	freeList(head);

	/* Return void to caller */
	return;
}

/*
 * makeNode
 * Create a new node
 */
factorNodeT *
makeNode (unsigned long pr, unsigned int exp, factorNodeT *prevNode)
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
printFactors (factorNodeT *head, unsigned long key, bool showSteps)
{
	/* If not fully factored and not showSteps, return without printing */
	if ((key != 1) && !showSteps)
		return;

	factorNodeT *curFactor;		/* Current factor to print */
	bool isFirstFactor;			/* Whether or not curFactor is first factor */

	/* Print the start of the new line */
	printf("\t&= ");

	curFactor = head;	/* Start at head */
	isFirstFactor = true;
	while (NULL != curFactor)
	{
		/* Start by multiplying if not first factor */
		if (!isFirstFactor)
			printf(" \\times{}");

		/* Print the current factor */
		printf(" %lu^{%u}",
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
			printf(" \\times{}");

		/* Print the key */
		printf(" %lu",
				key);
	}

	/* End the current line in outputFile */
	printf("\\\\\n");

	/* Return void to caller */
	return;
}
