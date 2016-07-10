#include <pmmintrin.h>

int
main(int argc, char *argv[])
{
	float a[4] __attribute__ ((aligned(16)));
	float b[4] __attribute__ ((aligned(16)));
	__m128 vec1, vec2;
	float rv;
	a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f; a[3] = 4.0f;
	b[0] = 1.0f; b[1] = 2.0f; b[2] = 3.0f; b[3] = 4.0f;
	vec1 = _mm_load_ps(a);
	vec2 = _mm_load_ps(b);
	vec1 = _mm_mul_ps(vec1, vec2);
	vec1 = _mm_hadd_ps(vec1, vec1);
	vec1 = _mm_hadd_ps(vec1, vec1);
	_mm_store_ss(&rv, vec1);
	return (0);
}

