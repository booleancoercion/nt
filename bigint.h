#include <stdlib.h>
#include <stdint.h>

typedef uint32_t bic; // Big Int Chunk

typedef struct {
	int sign;
	size_t len;
	size_t cap;
	bic *chunks;
} bigint;

bigint *bi_init(void);
void bi_free(bigint *p);

// Return the sign bit of p. (0 if positive,
// 1 otherwise)
int bi_sign(bigint *const p);

// Ensure the BI's capacity is at least the given one.
// If it isn't, the BI is expanded.
// Returns a nonzero value on allocation failure.
int bi_ensure_size(bigint *p, size_t cap);

// Add the second argument to the first argument.
// Returns a nonzero value on allocation failure.
int bi_add_assign(bigint *base, bigint *const arg);
