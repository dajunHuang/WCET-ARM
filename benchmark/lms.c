/* MDH WCET BENCHMARK SUITE. File version $Id: lms.c,v 1.1 2005/11/11 10:16:18 ael01 Exp $ */

/*************************************************************************/
/*                                                                       */
/*   SNU-RT Benchmark Suite for Worst Case Timing Analysis               */
/*   =====================================================               */
/*                              Collected and Modified by S.-S. Lim      */
/*                                           sslim@archi.snu.ac.kr       */
/*                                         Real-Time Research Group      */
/*                                        Seoul National University      */
/*                                                                       */
/*                                                                       */
/*        < Features > - restrictions for our experimental environment   */
/*                                                                       */
/*          1. Completely structured.                                    */
/*               - There are no unconditional jumps.                     */
/*               - There are no exit from loop bodies.                   */
/*                 (There are no 'break' or 'return' in loop bodies)     */
/*          2. No 'switch' statements.                                   */
/*          3. No 'do..while' statements.                                */
/*          4. Expressions are restricted.                               */
/*               - There are no multiple expressions joined by 'or',     */
/*                'and' operations.                                      */
/*          5. No library calls.                                         */
/*               - All the functions needed are implemented in the       */
/*                 source file.                                          */
/*                                                                       */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/*  FILE: lms.c                                                          */
/*  SOURCE : C Algorithms for Real-Time DSP by P. M. Embree              */
/*                                                                       */
/*  DESCRIPTION :                                                        */
/*                                                                       */
/*     An LMS adaptive signal enhancement. The input signal is a sine    */
/*     wave with added white noise.                                      */
/*     The detailed description is in the program source code.           */
/*                                                                       */
/*  REMARK :                                                             */
/*                                                                       */
/*  EXECUTION TIME :                                                     */
/*                                                                       */
/*                                                                       */
/*************************************************************************/

/*
 * Changes: JG 2005/12/23: Changed type of main to int Inserted prototypes.
 * Removed unused variables.
 * Indented program.
 */

#define RAND_MAX 32768
#define PI 3.14159265358979323846

// int             rand();
// float  lms(float x, float d, float *b, int l,float mu, float alpha);
/* function prototypes for fft and filter functions */
// static float    gaussian(void);

#define N 1024
#define L 20 /* filter order, (length L+1) */

/* set convergence parameter */
float mu = 0.01;
float d[N], b[21];
float px[51]; /* max L = 50 */

// inline float sqrt(float n) {return 1.0;}
int main()
{
    // float           lms(float, float, float *, int, float, float);
    float signal_amp, noise_amp, arg, x;
    int k;

    /* create signal plus noise */
    signal_amp = 1.414213562;
    noise_amp = 0.2 * 3.464101615;
    arg = 2.0 * PI / 20.0;
    for (k = 0; k < N; k++)
    {
        // d[k] = signal_amp * sin(arg * k) + noise_amp * gaussian();
        d[k] = 1;
    }

    /* scale based on L */
    mu = 2.0 * mu / (L + 1);

    x = 0.0;
    for (k = 0; k < N; k++)
    {
        // lms(x, d[k], b, L, mu, 0.01);
        // lms(float x, float di, float *b, int l,float mu, float alpha)
        float di, alpha;
        int l;
        int ll;
        float e, mu_e, y;
        float sigma = 2.0; /* start at 2 and update internally */

        di = d[k];
        l = L;
        alpha = 0.01;
        px[0] = x;

        /* calculate filter output */
        y = b[0] * px[0];
        for (ll = 1; ll <= l; ll++)
            y = y + b[ll] * px[ll];

        /* error signal */
        e = di - y;

        /* update sigma */
        sigma = alpha * (px[0] * px[0]) + (1 - alpha) * sigma;
        mu_e = mu * e * sigma;

        /* update coefficients */
        for (ll = 0; ll <= l; ll++)
            b[ll] = b[ll] + mu_e * px[ll];
        /* update history */
        for (ll = l; ll >= 1; ll--)
            px[ll] = px[ll - 1];

        // return (y);
        /* delay x one sample */
        x = d[k];
    }
    return 0;
}
