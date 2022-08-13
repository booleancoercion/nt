#include "nt.h"

static void swapl(long *p, long *q) {
	long tmp = *p;
	*p = *q;
	*q = tmp;
} 

egcd_t extended_gcd(long a, long b) {
	int swap = a > b;
	if(swap) swapl(&a, &b);

	long a0, a1, a2, b0, b1, b2;
	a0 = 0;
	a1 = 1;
	b0 = 1;
	b1 = 0;

	while(a > 0) {
		long q = b / a;
		long r = b % a;

		a2 = a0 - q * a1;
		b2 = b0 - q * b1;
		
		b = a;
		a = r;

		a0 = a1;
		a1 = a2;
		b0 = b1;
		b1 = b2;
	}
	
	egcd_t out = {.gcd = b};
    if(swap) {
		out.x = b0;
		out.y = a0;
	} else {
		out.x = a0;
		out.y = b0;
	}

	return out;
}

long rem_euclid(long x, long mod) {
	long rem = x % mod;
	if(rem < 0) {
		return rem + mod;
	} else {
		return rem;
	}
}

long mod_inverse(long x, long mod) {
	x = rem_euclid(x, mod);
	egcd_t out = extended_gcd(x, mod);

	if(out.gcd > 1) {
		return -1;
	} else {
		return rem_euclid(out.x, mod);
	}
}
