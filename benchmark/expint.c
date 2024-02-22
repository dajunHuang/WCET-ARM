/* $Id: expint.c,v 1.2 2005/04/04 11:34:58 csg Exp $ */

/************************************************************************


 * FROM:


 *   http://sron9907.sron.nl/manual/numrecip/c/expint.c


 *


 * FEATURE:


 *   One loop depends on a loop-invariant value to determine


 *   if it run or not.


 *


 ***********************************************************************/
/*inlined*/
int main()
{

	int n = 50;
	long int x = 1;
	int i, ii, nm1;

	long int a, b, c, d, del, fact, h, psi, ans;

	nm1 = n - 1;

	if (x > 1)
	{
		b = x + n;
		c = 2e6;
		d = 3e7;
		h = d;
		for (i = 1; i <= 100; i++)
		{
			a = -i * (nm1 + i);
			b += 2;
			d = 10 * (a * d + b);
			c = b + a / c;
			del = c * d;
			h *= del;
			if (del < 10000)
			{
				ans = h * -x;
				return ans;
			}
		}
	}
	else
	{
		ans = nm1 != 0 ? 2 : 1000;
		fact = 1;
		for (i = 1; i <= 100; i++)
		{
			fact *= -x / i;
			if (i != nm1)
				del = -fact / (i - nm1);
			else
			{
				psi = 0x00FF;
				for (ii = 1; ii <= nm1; ii++)
					psi += ii + nm1;
				del = psi + fact * (x * x + (8 * x) << 4 - x);
			}
			ans += del;
		}
	}

	return ans;
}
