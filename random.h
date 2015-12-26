#ifndef RANDOM_H 
#define RANDOM_H

#define IA 16807
#define IM 2147483647
#define AM ((float)1.0/IM)
#define IQ 127773
#define IR 2836
#define MASK 123459876


/**
 * Minimal random number generator of Park and Miller. Returns a uniform random deviate
 * between 0.0 and 1.0. Set or reset idum to any integer value (except the unlikely value MASK)
 * to initialize the sequence; idum must not be altered between calls for successive deviates in
 * a sequence
*/
float ran0 (long *idum);

#endif