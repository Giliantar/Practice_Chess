#ifndef ARM_FIX_H
#define ARM_FIX_H

#ifdef __APPLE__
#include <pthread.h>

// Для _yield (с одним подчёркиванием)
#ifndef _yield
#define _yield() pthread_yield_np()
#endif

// Для __yield (с двумя подчёркиваниями) — это то, что нужно
#ifndef __yield
#define __yield() pthread_yield_np()
#endif

#ifndef yield
#define yield() pthread_yield_np()
#endif

#endif

#endif // ARM_FIX_H