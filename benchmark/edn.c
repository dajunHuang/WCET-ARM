/*
 * MDH WCET BENCHMARK SUITE. File version $Id: edn.c,v 1.1 2005/11/11
 * 10:14:10 ael01 Exp $
 */

/************************************************************************
 *	Simple vector multiply				*
 ************************************************************************/

/*
 * Changes: JG 2005/12/22: Inserted prototypes, changed type of main to int
 * etc. Added parenthesis in expressions in jpegdct. Removed unused variable
 * dx. Changed int to long to avoid problems when compiling to 16 bit target
 * Indented program.
 * JG 2006-01-27: Removed code in codebook
 */

#define N 2048
#define ORDER 50

int main(void)
{
    /*
     * Declared as memory variable so it doesn't get optimized out
     */

    // vec_mpy1(short y[], const short x[], short scaler)
    // vec_mpy1(a, b, c);
    short a[N];
    short b[N];
    long int output[N];
    short c = 0x3;
    long int d = 0xAAAA;
    int e[1] = {0xEEEE};
    {
        long int i;

        for (i = 0; i < N; i++)
            a[i] += ((c * b[i]) >> 15);
    }
    // long int mac(const short *a, const short *b, long int sqr, long int *sum);
    // c = mac(a, b, (long int) c, (long int *) output);
    {
        long int i;
        long int dotp = *output;

        for (i = 0; i < N; i++)
        {
            dotp += b[i] * a[i];
            c += b[i] * b[i];
        }

        *output = dotp;
    }

    // void fir(const short array1[], const short coeff[], long int output[]);
    // fir(a, b, output);
    {
        long int i, j, sum;

        for (i = 0; i < N - ORDER; i++)
        {
            sum = 0;
            for (j = 0; j < ORDER; j++)
            {
                sum += a[i + j] * b[i + j];
            }
            output[i] = sum >> 15;
        }
    }
    // void fir_no_red_ld(const short x[], const short h[], long int y[]);
    // fir_no_red_ld(a, b, output);
    {
        long int i, j;
        long int sum0, sum1;
        short x0, x1, h0, h1;
        for (j = 0; j < N; j += 2)
        {
            sum0 = 0;
            sum1 = 0;
            x0 = a[j];
            for (i = 0; i < 32; i += 2)
            {
                x1 = a[j + i + 1];
                h0 = b[i];
                sum0 += x0 * h0;
                sum1 += x1 * h0;
                x0 = a[j + i + 2];
                h1 = b[i + 1];
                sum0 += x1 * h1;
                sum1 += x0 * h1;
            }
            output[j] = sum0 >> 15;
            output[j + 1] = sum1 >> 15;
        }
    }
    // long int latsynth(short b[], const short k[], long int n, long int f)
    // d = latsynth(a, b, N, d);
    {
        long int i;
        int n = N;

        d -= a[n - 1] * b[n - 1];
        for (i = n - 2; i >= 0; i--)
        {
            d -= a[i] * b[i];
            a[i + 1] = a[i] + ((b[i] * (d >> 16)) >> 16);
        }
        a[0] = d >> 16;
    }

    // void iir1(const short *coefs, const short *input, long int *optr, long int *state)
    // iir1(a, b, &output[100], output);
    {
        long int x;
        long int t;
        long int n;
        short *coefs = a;
        short *input = b;
        long int *optr = &output[100];
        long int *state = output;
        x = input[0];
        for (n = 0; n < 512; n++)
        {
            t = x + ((coefs[2] * state[0] + coefs[3] * state[1]) >> 15);
            x = t + ((coefs[0] * state[0] + coefs[1] * state[1]) >> 15);
            state[1] = state[0];
            state[0] = t;
            coefs += 4; /* point to next filter coefs  */
            state += 2; /* point to next filter states */
        }
        *optr++ = x;
    }

// e[0] = codebook(d, 1, 17, e[0], d, a, c, 1);

// jpegdct(a, b);
// void jpegdct(short *d, short *r)
#if 0
    {
        long int        t[12];
        short           i, j, k, m, n, p;
        short *d = a;
        short *r = b;
        for (k = 1, m = 0, n = 13, p = 8; k <= 8; k += 7, m += 3, n += 3, p -= 7, d -= 64) {
            for (i = 0; i < 8; i++, d += p) {
                for (j = 0; j < 4; j++) {
                    t[j] = d[k * j] + d[k * (7 - j)];
                    t[7 - j] = d[k * j] - d[k * (7 - j)];
                }
                t[8] = t[0] + t[3];
                t[9] = t[0] - t[3];
                t[10] = t[1] + t[2];
                t[11] = t[1] - t[2];
                d[0] = (t[8] + t[10]) >> m;
                d[4 * k] = (t[8] - t[10]) >> m;
                t[8] = (short) (t[11] + t[9]) * r[10];
                d[2 * k] = t[8] + (short) ((t[9] * r[9]) >> n);
                d[6 * k] = t[8] + (short) ((t[11] * r[11]) >> n);
                t[0] = (short) (t[4] + t[7]) * r[2];
                t[1] = (short) (t[5] + t[6]) * r[0];
                t[2] = t[4] + t[6];
                t[3] = t[5] + t[7];
                t[8] = (short) (t[2] + t[3]) * r[8];
                t[2] = (short) t[2] * r[1] + t[8];
                t[3] = (short) t[3] * r[3] + t[8];
                d[7 * k] = (short) (t[4] * r[4] + t[0] + t[2]) >> n;
                d[5 * k] = (short) (t[5] * r[6] + t[1] + t[3]) >> n;
                d[3 * k] = (short) (t[6] * r[5] + t[1] + t[2]) >> n;
                d[1 * k] = (short) (t[7] * r[7] + t[0] + t[3]) >> n;
            }
        }
    }
#endif
}
