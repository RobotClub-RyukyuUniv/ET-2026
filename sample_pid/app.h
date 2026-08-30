#ifdef __cplusplus
extern "C" {
#endif

#include <kernel.h>
#if defined(__has_include)
#  if __has_include("kernel_cfg.h")
#    include "kernel_cfg.h"
#  endif
#else
#  include "kernel_cfg.h"
#endif

/* タスク優先度 */
#define MAIN_PRIORITY    10 /* メインタスク */
#define TRACER_PRIORITY  11 /* ライントレースタスク */

#ifndef STACK_SIZE
#define STACK_SIZE      (4096)
#endif /* STACK_SIZE */

#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void tracer_task(intptr_t exinf);
extern void tracer_cyc(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
