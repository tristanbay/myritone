#include "main.h"
#include "myritone.h"

void print_scale(scalenote* scale, unsigned len)
{
	for (unsigned i = 0; i < len; ++i) {
		if (scale[i].ji.n == 0 || scale[i].ji.d == 0 || scale[i].ed.d == 0) {
			printf("0.");
		} else if (scale[i].ed.n == scale[i].ed.d) {
			printf("%lu/%lu", scale[i].ji.n, scale[i].ji.d);
		} else {
			printf("%.6lf", log((double)scale[i].ji.n
				/ (double)scale[i].ji.d) / log(2.0) * 1200.0
				/ (double)scale[i].ed.d * (double)scale[i].ed.n);
		}
		printf("\n");
	}
}
