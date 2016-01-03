#include "random.h"
#include <stdlib.h>

float ran0 (long *idum) {
  return (float)rand() / (RAND_MAX + 1.0);
  
  long k;
  float ans;
  
  *idum ^= MASK;  // XORing with MASK allows use of zero and other simple bit patterns for idum
  k = (*idum)/IQ; 
  *idum = IA*(*idum-k*IQ)-IR*k; // Compute idum=(IA*idum) % IM without overflows
  if (*idum < 0) 
    *idum += IM;              // by Schragefs method.
  ans = AM*(*idum);           // Convert idum to a floating result.
  *idum ^= MASK;              // Unmask before return.
  return ans;
}


static uint32_t temper(uint32_t x)
{
    x ^= x>>11;
    x ^= x<<7 & 0x9D2C5680;
    x ^= x<<15 & 0xEFC60000;
    x ^= x>>18;
    return x;
}
uint32_t lcg64_temper(uint64_t *seed)
{
    *seed = 6364136223846793005ULL * *seed + 1;
    return temper(*seed >> 32);
}
