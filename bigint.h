#include <stdlib.h>
#include <stdint.h>

typedef uint32_t bic; // Big Int Chunk

typedef struct {
	size_t len;
	size_t cap;
	bic *chunks;
} bigint;

bigint *bi_init(void);
void bi_free(bigint *p);

// Return the sign bit of p. (0 if positive,
// 1 otherwise)
int bi_sign(bigint *const p);

// Calculate p's length (number of chunks containing
// actual information) and update p's len field
void bi_calc_len(bigint *p);

// Ensure the BI's capacity is at least the given one.
// If it isn't, the BI is expanded.
// Returns a nonzero value on allocation failure.
int bi_ensure_size(bigint *p, size_t cap);

// Add the second argument to the first argument.
// Returns a nonzero value on allocation failure.
int bi_add_assign(bigint *base, bigint *const arg);

// Adds 1 to p.
// Returns a nonzero value on allocation failure.
int bi_add_one(bigint *p);

// Computes the arithmetic negation of p.
void bi_negate(bigint *p);

// Print the hexadecimal representation of p
void bi_print_hex(bigint *const p);
