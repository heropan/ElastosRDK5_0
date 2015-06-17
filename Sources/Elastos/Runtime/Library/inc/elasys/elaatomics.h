#ifndef _ELAATOMICS_H
#define _ELAATOMICS_H

extern "C" {

extern int atomic_cmpxchg(int old, int _new, volatile int *ptr);
extern int atomic_swap(int _new, volatile int *ptr);

extern int atomic_inc(volatile int *ptr);
extern int atomic_dec(volatile int *ptr);
extern int atomic_add(int value, volatile int* ptr);
extern int atomic_and(int value, volatile int* ptr);
extern int atomic_or(int value, volatile int *ptr);

}

#endif /* _ELAATOMICS_H */
