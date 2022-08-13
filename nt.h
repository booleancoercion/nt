#ifndef NT_C
#define NT_C

typedef struct egcd {
	long gcd, x, y;
} egcd_t;

/* Caller must ensure that a, b are non-negative, and that
   at least one of them is nonzero. */
egcd_t extended_gcd(long a, long b);

/* Caller must ensure that mod > 0. */
long rem_euclid(long x, long mod);

/* Caller must ensure that mod > 0. */
long mod_inverse(long x, long mod);

#endif /* NT_C */

