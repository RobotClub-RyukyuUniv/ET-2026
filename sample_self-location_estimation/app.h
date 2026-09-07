#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MAIN_PRIORITY  5
#define INIT_PRIORITY  6	

#ifndef STACK_SIZE
#define STACK_SIZE      4096
#endif /* STACK_SIZE */

#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
