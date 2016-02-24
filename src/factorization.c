#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Function prototypes
 */
/* Factor the key */
void factorKey(unsigned long, FILE *);

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
	fprintf(outputFile, "\\(%lu =",
			key);

	/* Factor the key */
	factorKey(key, outputFile);

	// Write the end of the TeX math environment
	fprintf(outputFile, "\\)");

	/* Finish and close the file */
	fprintf(outputFile, "\n");
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
	unsigned int exponent;
	bool isFirstFactor = true;		/* If false then write \times{} to file */
	unsigned long posFactor = 2;	/* Start possible factor at 2 */

	/* Factor the key while it
	 * contains prime factors and
	 * is greater than or equal to square of factor
	 * */
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
		/* If there were any factors, write them to outputFile */
		if (exponent > 0)
		{
			/* Write \times{} if not first factor */
			if (!isFirstFactor)
				fprintf(outputFile, " \\times{}");
			/* Write factor */
			fprintf(outputFile, " %lu^{%u}",
					posFactor,
					exponent);
			/* Now at least one factor */
			isFirstFactor = false;
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
		if (!isFirstFactor)
			fprintf(outputFile, " \\times{}");
		/* Write factor */
		fprintf(outputFile, " %lu^{1}",
				key);
	}

	/* Return void to caller */
	return;
}
