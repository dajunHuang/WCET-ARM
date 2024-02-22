/* MDH WCET BENCHMARK SUITE. File version $Id: matmult.c,v 1.3 2005/11/11 10:31:26 ael01 Exp $ */

/* matmult.c */
/* was mm.c! */

/*----------------------------------------------------------------------*
 * To make this program compile under our assumed embedded environment,
 * we had to make several changes:
 * - Declare all functions in ANSI style, not K&R.
 *   this includes adding return types in all cases!
 * - Declare function prototypes
 * - Disable all output
 * - Disable all UNIX-style includes
 *
 * This is a program that was developed from mm.c to matmult.c by
 * Thomas Lundqvist at Chalmers.
 *----------------------------------------------------------------------*/

/* Changes:
 * JG 2005/12/12: Indented program.
 */

#define UPPSALAWCET 1

/* ***UPPSALA WCET***:
   disable stupid UNIX includes */
#ifndef UPPSALAWCET
#include <sys/types.h>
#include <sys/times.h>
#endif

/*
 * MATRIX MULTIPLICATION BENCHMARK PROGRAM:
 * This program multiplies 2 square matrices resulting in a 3rd
 * matrix. It tests a compiler's speed in handling multidimensional
 * arrays and simple arithmetic.
 */

#define UPPERLIMIT 24

typedef int matrix[UPPERLIMIT][UPPERLIMIT];

int Seed;
matrix A, B, RA;

int main(void)
{
    // InitSeed();
    Seed = 0;
    // Test(ArrayA, ArrayB, ResultArray);
    {

        // Initialize(A);
        {

            int OuterIndex, InnerIndex;

            for (OuterIndex = 0; OuterIndex < UPPERLIMIT; OuterIndex++)
                for (InnerIndex = 0; InnerIndex < UPPERLIMIT; InnerIndex++)
                    A[OuterIndex][InnerIndex] = 1;
        }
        // Initialize(B);
        {

            matrix Array;
            int OuterIndex, InnerIndex;

            for (OuterIndex = 0; OuterIndex < UPPERLIMIT; OuterIndex++)
                for (InnerIndex = 0; InnerIndex < UPPERLIMIT; InnerIndex++)
                {
                    RA[OuterIndex][InnerIndex] = 0;
                    B[OuterIndex][InnerIndex] = 1;
                }
        }

        /* ***UPPSALA WCET***: don't print or time */
        // Multiply(A, B, Res);
        {
            register int Outer, Inner, Index;

            for (Outer = 0; Outer < UPPERLIMIT; Outer++)
                for (Inner = 0; Inner < UPPERLIMIT; Inner++)
                {
                    for (Index = 0; Index < UPPERLIMIT; Index++)
                        RA[Outer][Inner] += A[Outer][Index] * B[Index][Inner];
                }
        }
    }
    return 0;
}
