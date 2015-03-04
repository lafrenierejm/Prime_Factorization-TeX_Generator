# Prime_Factorization-TeX_Generator
Prompts for an integer and outputs the prime factorization of that number as a single line of math-formatted TeX.

The input is stored as unsigned long in C.
The algorithm finds and stores all prime numbers up to input/2 in a linked list. The linked list is implemented as a series of struct pointers.
After generating the list of primes, those primes are sequentially compared to the input number to break it down in powers of primes.
Each factor is written to Output.tex, which will be located in the directory of the executable.
