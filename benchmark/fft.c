#define DEBUG 0
#if DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

// #include <math.h>
// #define MAXSIZE 256
#define MAXSIZE 128
#define MAXWAVES 8
#define DDC_PI (3.14159265358979323846)

float RealIn[MAXSIZE];
float ImagIn[MAXSIZE];
float RealOut[MAXSIZE];
float ImagOut[MAXSIZE];
float ar[3], ai[3];
// float coeff[MAXWAVES];
// float amp[MAXWAVES];
/*
inline unsigned NumberOfBitsNeeded ( unsigned PowerOfTwo )
{
    return 8;//log(MAXWAVES) = log(200) = 8
}

inline unsigned ReverseBits ( unsigned index, unsigned NumBits )
{
    unsigned i, rev;

        for ( i=rev=0; i < NumBits; i++ )
        {
            rev = (rev << 1) | (index & 1);
                index >>= 1;
        }

        return rev;
}
inline float sin(float a) {return a;}
inline float cos(float a) {return a;}

*/

int main(int argc, char *argv[])
{
    unsigned i, j;
    int invfft = 0;
    float coeff, amp;

    // dummy init
    for (i = 0; i < MAXSIZE; i++)
    {
        coeff = ((i + 39) * 91) % 1000;
        amp = ((i + 71) * 83) % 1000;
        RealIn[i] = coeff * amp;
        ImagIn[i] = 0;
    }

    /* regular*/
    // fft_float (MAXSIZE,invfft,RealIn,ImagIn,RealOut,ImagOut);
    // void fft_float (
    /*assume NumSamples is power of 2*/
    {
        unsigned NumSamples;
        int InverseTransform;
        unsigned NumBits; /* Number of bits needed to store indices */
        unsigned i, j, k, n;
        unsigned BlockSize, BlockEnd;

        float angle_numerator = 2.0 * DDC_PI;
        float tr, ti; /* temp real, temp imaginary */

        NumSamples = MAXSIZE;
        InverseTransform = invfft;
        if (InverseTransform)
            angle_numerator = -angle_numerator;
        //  NumBits = NumberOfBitsNeeded ( NumSamples );
        NumBits = 7;
        for (i = 0; i < NumSamples; i++)
        {
            {
                unsigned index = i;
                unsigned i, rev;

                for (i = rev = 0; i < NumBits; i++)
                {
                    rev = (rev << 1) | (index & 1);
                    index >>= 1;
                }
                j = rev;
            }
            //  j = ReverseBits ( i, NumBits );
            RealOut[j] = RealIn[i];
            ImagOut[j] = ImagIn[i];
        }

        /*
         **   Do the FFT itself...
         */

        BlockEnd = 1;
        for (BlockSize = 2; BlockSize <= NumSamples; BlockSize <<= 1)
        {
            float delta_angle = angle_numerator / (float)BlockSize;
            float sm2 = (-2 * delta_angle);
            float sm1 = (-delta_angle);
            float cm2 = (-2 * delta_angle);
            float cm1 = (-delta_angle);
            float w = 2 * cm1;
            float temp;

            for (i = 0; i < NumSamples; i += BlockSize)
            {
                ar[2] = cm2;
                ar[1] = cm1;

                ai[2] = sm2;
                ai[1] = sm1;

                for (j = i, n = 0; n < BlockEnd; j++, n++)
                {
                    ar[0] = w * ar[1] - ar[2];
                    ar[2] = ar[1];
                    ar[1] = ar[0];

                    ai[0] = w * ai[1] - ai[2];
                    ai[2] = ai[1];
                    ai[1] = ai[0];

                    k = j + BlockEnd;
                    tr = ar[0] * RealOut[k] - ai[0] * ImagOut[k];
                    ti = ar[0] * ImagOut[k] + ai[0] * RealOut[k];

                    RealOut[k] = RealOut[j] - tr;
                    ImagOut[k] = ImagOut[j] - ti;

                    RealOut[j] += tr;
                    ImagOut[j] += ti;
                }
            }

            BlockEnd = BlockSize;
        }

        /*forward FFT only*/
    }
}
