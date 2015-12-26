#include "random.h"

float ran0 (long *idum) {
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
