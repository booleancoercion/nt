#include "nt.h"
#include <stdio.h>

int main(void) {
	long a = 25421;
	long b = 8492;
	egcd_t res = extended_gcd(a, b);
	printf("egcd test: gcd(%ld, %ld) = %ld\n", a, b, res.gcd);
	printf("%ld = %ld * a + %ld * b\n", res.gcd, res.x, res.y);
	
	a = a / res.gcd;
	b = b / res.gcd;

	printf("\na = %ld, b = %ld\n", a, b);
	long ainv = mod_inverse(a, b);
   	printf("a * %ld === 1 (mod b)\n", ainv);

	return 0;
}
