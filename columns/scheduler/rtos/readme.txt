
The building of this RTOS is trying to achieve the goal that easy the
application transplant between windows and embedded system base on MCU.
This RTOS is somewhat behaving similar to windows. Code is better structured and
easier readable than uCOS.

OS schedule: preemptive base on priority, without time slice.
Mutex: yes, priority ceiling mechanism.
Semaphore: yes, with mulitple post at once.
Event flag: yes, signature-notification mechanism.


Code Style

1) FUNCTIONS:
API:                osFlagCreate()
user implement:     OSStartHighRdy(), OSEnterCriticalSection()
internal interface: OS_TaskStackInit()
static function:    os_tcb_init()

2) MACRO FUNCTIONS:
as function.

3) TYPE: 
OS_FLAG, UINT8, UINT32

4) CONSTANT(macro and enumeration):
OS_STAT_PEND_TO

5) VARIABLES:
local variable:         nextNode， i， flag
global variable:        osReadyList， osCrentTCB
member of structure:    EventOwnerPrio, TCBNext

Some Principle
1) You should call osIntEnter() and osIntExit() at the begin and end of ISR if
you call any API of RTOS.
2) You should not call any API of RTOS in hard-real-time ISR.
