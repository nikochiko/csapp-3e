#include <assert.h>
#include <stdio.h>

#define INT_MIN 0x80000000
#define INT_MAX 0x7FFFFFFF

typedef enum { NEG, ZERO, POS, OTHER } range_t;

range_t find_range_truth(float x)
{
	int result;
	if (x < 0)
	  result = NEG;
	else if (x == 0)
	  result = ZERO;
	else if (x > 0)
	  result = POS;
	else
	    result = OTHER;

    return result;
}

range_t find_range(float x)
{
    __asm__(
        "vxorps %xmm1, %xmm1, %xmm1\n"
        "vucomiss %xmm1, %xmm0\n"
        "jp .L1\n"
        "ja .L2\n"
        "jb .L3\n"
        "movl $1, %eax\n"
        "jmp .done\n"
        ".L1:\n"
        "movl $3, %eax\n"
        "jmp .done\n"
        ".L2:\n"
        "movl $2, %eax\n"
        "jmp .done\n"
        ".L3:\n"
        "movl $0, %eax\n"
        ".done:\n"
    );
}

char* getRangeName(range_t r) {
    switch (r) {
        case NEG: return "NEG";
        case POS: return "POS";
        case ZERO: return "ZERO";
        case OTHER: return "OTHER";
        default: return "not right...";
    }
}

int main() {
    int been_here = 0;
    for (int i = INT_MIN; i <= INT_MAX; i++) {
        if (i == INT_MIN && ++been_here == 2) {
            // if we're at INT_MIN for the second time, just break
            break;
        }

        float f = * (float *) (&i);

        // find_range_truth(f)
        if (i % (1 << 22) == 0)
            printf("range(%f) = %s\n", f, getRangeName(find_range(f)));

        assert(find_range(f) == find_range_truth(f));
    }
}
