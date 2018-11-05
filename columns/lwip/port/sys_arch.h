#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include "cc.h" 


/*-----------------macros-----------------------------------------------------*/
#define LWIP_TASK_START_PRIO    2
#define LWIP_TASK_END_PRIO      2

#define LWIP_TASK_MAX	        (LWIP_TASK_END_PRIO - LWIP_TASK_START_PRIO + 1)  //max number of lwip tasks (TCPIP) 
                                                                                 //note LWIP TASK start with 1
		
#define LWIP_START_PRIO	        LWIP_TASK_START_PRIO	    //first prio of lwip tasks in uC/OS-II	
#define LWIP_STK_SIZE	        400

#define MAX_QUEUES              20	                        // the number of mailboxes
#define MAX_QUEUE_ENTRIES       8	                        // the max size of each mailbox

#define SYS_MBOX_NULL           (void *)0
#define SYS_SEM_NULL            (void *)0

#define LWIP_COMPAT_MUTEX       1


#define sys_sem_valid(psem)         ((*(sys_sem_t *)(psem))? 1 : 0)
#define sys_sem_set_invalid(psem)   do{ \
                                        *(sys_sem_t *)(psem) = (sys_sem_t)0; \
                                    }while(0)

/*-------------critical region protection (depends on uC/OS-II setting)-------*/

#define SYS_ARCH_DECL_PROTECT(lev)	OS_CPU_SR lev
#define SYS_ARCH_PROTECT(lev)		OS_CRITICAL_SECTION_ENTER(lev)
#define SYS_ARCH_UNPROTECT(lev)		OS_CRITICAL_SECTION_EXIT(lev)


/*-----------------type define------------------------------------------------*/

/* struct of LwIP mailbox */
typedef struct {
    OS_FLAG_TYPE    *pQ;
    void            *pvQEntries[MAX_QUEUE_ENTRIES];

} TMBOXQ_DESCR, *PMBOXQ_DESCR;

typedef OS_SEMAPHORE_TYPE   sys_sem_t;         // type of semiphores
typedef OS_MUTEX_TYPE       sys_mutex_t;       // type of mutex semiphores
typedef TMBOXQ_DESCR       *sys_mbox_t;        // type of mailboxes
typedef u8_t                sys_thread_t;       // type of id of the new thread
typedef u8_t                sys_prot_t;

/*-----------------global variables-------------------------------------------*/

//extern OS_STK LWIP_TASK_STK[LWIP_TASK_MAX][LWIP_STK_SIZE];

#endif /* __SYS_RTXC_H__ */
