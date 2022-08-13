#include "bigint.h"
#include <string.h>

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
	
	bic *tmp = realloc(p->chunks, cap);
	if(!tmp) return 1;

	p->chunks = tmp;

	// sign-extend the new memory
	bic sext = bi_sign(p) ? (bic)-1 : 0;
	memset(p->chunks + p->cap, sext, cap - p->cap);
	
	p->cap = cap;
	return 0;
}

int bi_sign(bigint *const p) {
	return p->chunks[p->cap - 1] >> (8*sizeof(bic) - 1);
}

#define max(a,b) \
	({ __typeof__(a) _a = (a); \
	   __typeof__(b) _b = (b); \
	   _a > _b ? _a : _b; })

#define min(a,b) \
	({ __typeof__(a) _a = (a); \
	   __typeof__(b) _b = (b); \
	   _a < _b ? _a : _b; })

int bi_add_assign(bigint *base, bigint *const arg) {
	size_t blen = base->len;
	size_t arlen = arg->len;

	if(bi_ensure_size(base, 1+max(blen, arlen)))
		return 1;

	int carry = 0;
	size_t i;
	for(i = 0; i < arlen; i++) {
		bic sum = base->chunks[i]
				+ arg->chunks[i]
				+ carry;
		carry = sum < arg->chunks[i] + carry;

		base->chunks[i] = sum;
	}

	bic phantom = bi_sign(arg) ? (bic)-1 : 0;
	
	// TODO: figure out while loop and
	// recalculate length
	
	return 0;
}

int bi_add_assign(bigint *base, bigint *const arg) {
	if(base->sign == arg->sign) {
		bi_add_assign_raw(base, arg);
	} else {
		bi_sub_assign_raw(base, arg);
	}
}
