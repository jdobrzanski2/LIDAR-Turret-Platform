/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-F07
 */

#include <xdc/std.h>

#include <ti/sysbios/family/c28/Hwi.h>
extern const ti_sysbios_family_c28_Hwi_Handle hwi0;

#include <ti/sysbios/family/c28/Hwi.h>
extern const ti_sysbios_family_c28_Hwi_Handle hwi1;

#include <ti/sysbios/knl/Swi.h>
extern const ti_sysbios_knl_Swi_Handle mySwi;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle draw_Sem;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle lock_Sem;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle draw_point;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle clear_point;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle clear_Sem;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle redraw_point;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle redraw_Sem;

extern int xdc_runtime_Startup__EXECFXN__C;

extern int xdc_runtime_Startup__RESETFXN__C;

