#include "bigint.h"
#include <string.h>
#include <stdio.h>

bigint *bi_init(void) {
	bigint *p = malloc(sizeof(bigint));
	if(!p) return NULL;

	p->len = 0;
	p->chunks = calloc(4, sizeof(bic));
	if(!p->chunks) {
		free(p);
		return NULL;
	}

	p->cap = 4;
	return p;
}

void bi_free(bigint *p) {
	free(p->chunks);
	free(p);
}

static size_t next_pow2(size_t x) {
	return x <= 1 ? 1 : 1 << (8*sizeof(size_t) - __builtin_clzl(x - 1));
}

int bi_ensure_size(bigint *p, size_t cap) {
	cap = next_pow2(cap);
	if(cap <= p->cap) return 0;
	
	bic *tmp = realloc(p->chunks, cap * sizeof(bic));
	if(!tmp) return 1;

	p->chunks = tmp;

	// sign-extend the new memory
	unsigned char sext = bi_sign(p)
			? (unsigned char)-1
			: 0;

	memset(
		p->chunks + p->cap,
		sext,
		(cap - p->cap) * sizeof(bic)
	);
	
	p->cap = cap;
	return 0;
}

int bi_sign(bigint *const p) {
	return p->chunks[p->cap - 1] >> (8*sizeof(bic) - 1);
}

void bi_calc_len(bigint *p) {
	bic dummy = bi_sign(p) ? (bic)-1 : 0;

	size_t i;
	for(i = 0; i < p->cap; i++) {
		if(p->chunks[p->cap - i - 1] != dummy) break;
	}

	p->len = p->cap - i;
}

#define max(a,b) \
	({ __typeof__(a) _a = (a); \
	   __typeof__(b) _b = (b); \
	   _a > _b ? _a : _b; })

#define min(a,b) \
	({ __typeof__(a) _a = (a); \
	   __typeof__(b) _b = (b); \
	   _a < _b ? _a : _b; })

// carry must be either 0 or 1
static void add_assign_chunk_with_carry(bic *base, bic arg, bic *carry) {
	*base += arg;
	bic tcarry = *base < arg;
	*base += *carry;
   	*carry = tcarry | (*base < *carry);
}

int bi_add_assign(bigint *base, bigint *const arg) {
	size_t blen = base->len;
	size_t arlen = arg->len;

	if(bi_ensure_size(base, 1+max(blen, arlen)))
		return 1;

	bic carry = 0;
	size_t i;
	for(i = 0; i < arlen; i++)
		add_assign_chunk_with_carry(
			&base->chunks[i],
			arg->chunks[i],
			&carry
		);

	bic phantom = bi_sign(arg) ? (bic)-1 : 0;
	
	// TODO: optimize this! running the loop
	// is unnecessary sometimes
	for(; (phantom || carry) && i < base-> cap; i++)
		add_assign_chunk_with_carry(
			&base->chunks[i],
			phantom,
			&carry
		);
	// last carry is ignored - we have at least
	// one buffer chunk as is verified by the
	// size guarantee
	
	bi_calc_len(base);
	
	return 0;
}

// Add 1 to p without ensuring its size is enough
// to hold the result. I use this to avoid unnecessary
// allocations.
static void add_one_unchecked(bigint *p) {
	bic carry = 1;
	for(size_t i = 0; i < p->cap && carry; i++)
		add_assign_chunk_with_carry(
			&p->chunks[i],
			0,
			&carry
		);

	bi_calc_len(p);
}

int bi_add_one(bigint *p) {
	if(bi_ensure_size(p, p->len + 1))
		return 1;

	add_one_unchecked(p);
	return 0;
}

void bi_negate(bigint *p) {
	for(size_t i = 0; i < p->cap; i++) {
		p->chunks[i] = ~(p->chunks[i]);
	}

	add_one_unchecked(p);
}

void bi_print_hex(bigint *const p) {
	printf("0x");
	for(size_t i = 0; i < p->cap; i++) {
		printf("%.8X", p->chunks[p->cap - i - 1]);
		if(i + 1 < p->cap) printf("_");
	}
}

