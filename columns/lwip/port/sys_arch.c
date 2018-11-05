#include "debug.h"
#include "def.h"
#include "sys.h"
#include "mem.h"
#include "sys_arch.h"

 
typedef OS_STK Task_Stack_Type[LWIP_STK_SIZE];      
__align(8) Task_Stack_Type LWIP_TASK_STK[LWIP_TASK_MAX];

static u32_t NullValue;
static void * const pvNullPointer = (void *)&NullValue;
static OS_MEM *pMBoxQMem;
static char pcQueueMemoryPool[MAX_QUEUES * sizeof(TMBOXQ_DESCR) + MEM_ALIGNMENT - 1];



/*-----------------------------------------------------------------------------------*/
/*
  Initialize sys arch
*/
void sys_init(void)
{
	u8_t ucErr;
        
    // 为消息队列划分存储空间
    pMBoxQMem = OSMemCreate( 
                            (void*)((u32_t)((u32_t)pcQueueMemoryPool + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT - 1)), 
                            MAX_QUEUES,
                            sizeof(TMBOXQ_DESCR), 
                            &ucErr 
                           );
    LWIP_ASSERT("OSMemCreate ", ucErr == OS_ERR_NONE);
}

/*-----------------------------------------------------------------------------------*/
/*
  Creates and returns a new semaphore. The "count" argument specifies
  the initial state of the semaphore. TBD finish and test
*/
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
    *sem = OSSemCreate((u16_t)count);
    LWIP_ASSERT("OSSemCreate ", *sem != NULL );
    return (ERR_OK);
}

/*-----------------------------------------------------------------------------------*/
/*
  Deallocates a semaphore
*/
void sys_sem_free(sys_sem_t *sem)
{
    u8_t ucErr;

    assert(*sem != NULL);

    (void)OSSemDel((OS_EVENT *)*sem, OS_DEL_ALWAYS, &ucErr );
    LWIP_ASSERT( "OSSemDel ", ucErr == OS_ERR_NONE );
}

/*-----------------------------------------------------------------------------------*/
/*
  Signals a semaphore
*/
void sys_sem_signal(sys_sem_t *sem)
{
    assert(*sem != NULL);
    
    OSSemPost((OS_EVENT *)*sem);
    
    // May be called when a connection is already reset, should not check...
    // LWIP_ASSERT( "OSSemPost ", ucErr == OS_ERR_NONE );
}

/*-----------------------------------------------------------------------------------*/
/*
  Blocks the thread while waiting for the semaphore to be
  signaled. If the "timeout" argument is non-zero, the thread should
  only be blocked for the specified time (measured in
  milliseconds).

  If the timeout argument is non-zero, the return value is the number of
  milliseconds spent waiting for the semaphore to be signaled. If the
  semaphore wasn't signaled within the specified time, the return value is
  SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
  (i.e., it was already signaled), the function may return zero.

  Notice that lwIP implements a function with a similar name,
  sys_sem_wait(), that uses the sys_arch_sem_wait() function.
*/
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    u8_t ucErr;
    u32_t ucos_timeout, timeout_new;

    assert(*sem != NULL);
    
    /* timeout 单位以ms计，要转换 timeout 单位以TICK计 */
    if (timeout != 0)
    {
        ucos_timeout = (timeout * OS_TICKS_PER_SEC) / 1000; // convert to timetick
        if(ucos_timeout < 1)
        {
            ucos_timeout = 1;
        }
        else if(ucos_timeout > 65536)                       // uC/OS only support u16_t pend
        {
            ucos_timeout = 65535;       
        }
    }
    else 
    {
        ucos_timeout = 0;
    }

    timeout = OSTimeGet();              // 记录起始时间
    
    OSSemPend((OS_EVENT *)*sem, (u16_t)ucos_timeout, (u8_t *)&ucErr);
    if (ucErr == OS_ERR_TIMEOUT)
    {
        timeout = SYS_ARCH_TIMEOUT;	    // only when timeout!
    }
    else if (ucErr == OS_ERR_NONE)
    {    
        timeout_new = OSTimeGet();      // 记录终止时间
        if (timeout_new >= timeout) 
        {
            timeout_new = timeout_new - timeout;
        }
        else 
        {
            timeout_new = 0xffffffff - timeout + timeout_new;
        }

        timeout = (timeout_new * 1000 / OS_TICKS_PER_SEC + 1); //convert to milisecond 为什么加1？
    }
    else
    {
        //for pbuf_free, may be called from an ISR
        LWIP_ASSERT( "OSSemPend ", ucErr != OS_ERR_EVENT_TYPE );
        LWIP_ASSERT( "OSSemPend ", ucErr != OS_ERR_PEVENT_NULL );
        LWIP_ASSERT( "OSSemPend ", ucErr != OS_ERR_PEND_ISR );
    }
    
    return timeout;
}

#ifndef sys_sem_valid
int sys_sem_valid(sys_sem_t *sem)
{
    if (*sem)
    {
        return 1;
    }
    return 0;
}
#endif


#ifndef sys_sem_set_invalid
void sys_sem_set_invalid(sys_sem_t *sem)
{
    *sem = NULL;
}
#endif

/*-----------------------------------------------------------------------------------*/
/*
  Creates an empty mailbox.
*/
err_t sys_mbox_new(sys_mbox_t *mbox, int size)  // prarmeter "size" can be ignored in your implementation.
{
    u8_t     ucErr;
    
    (*mbox) = OSMemGet(pMBoxQMem, &ucErr);
    LWIP_ASSERT("OSMemGet ", ucErr == OS_ERR_NONE);
    if (ucErr == OS_ERR_NONE) 
    {
        size = MAX_QUEUE_ENTRIES;

        (*mbox)->pQ = OSQCreate((*mbox)->pvQEntries, size); 
        LWIP_ASSERT("OSQCreate ", (*mbox)->pQ != NULL);
        
        if ((*mbox)->pQ != NULL)
        {
            return (ERR_OK); 
        }
        else
        {
            ucErr = OSMemPut(pMBoxQMem, (*mbox));
        }
    }
    
    *mbox = SYS_MBOX_NULL;
    return (ERR_MEM);
}

/*-----------------------------------------------------------------------------------*/
/*
  Deallocates a mailbox
*/
void sys_mbox_free(sys_mbox_t *mbox)
{
    u8_t ucErr;
    OS_Q_DATA ucQstate;
    
    LWIP_ASSERT("sys_mbox_free ", sys_mbox_valid(mbox));      
    
    ucErr = OSQQuery((*mbox)->pQ, &ucQstate);
    LWIP_ASSERT( "OSQQuery ", ucErr == OS_ERR_NONE );
#ifdef DEBUG
    while (ucErr != OS_ERR_NONE);
    while (ucQstate.OSNMsgs);
#endif    
    OSQFlush((*mbox)->pQ);    //clear OSQ EVENT
    (void)OSQDel((*mbox)->pQ, OS_DEL_NO_PEND, &ucErr);     //del OSQ EVENT
    LWIP_ASSERT( "OSQDel ", ucErr == OS_ERR_NONE );
    
    ucErr = OSMemPut(pMBoxQMem, (*mbox));                 //put mem back to mem queue
    LWIP_ASSERT("OSMemPut ", ucErr == OS_ERR_NONE);  

    *mbox = SYS_MBOX_NULL;
}

/*-----------------------------------------------------------------------------------*/
/*
  Posts the "msg" to the mailbox.
*/
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{   
    u8_t i = 0;

    LWIP_ASSERT("sys_mbox_post ", sys_mbox_valid(mbox)); 
    
    if (msg == NULL) 
    {
        msg = pvNullPointer;        //0xffffffff以和NULL区分开，接收消息时就能区分邮箱空还是发送的消息是空指针
    }
    
    while ((i < 10) && (OSQPost((*mbox)->pQ, msg) != OS_ERR_NONE))
    {
    	i++;              //if full, try 10 times
    	OSTimeDly(2);
    }
}

/*-----------------------------------------------------------------------------------*/
/* 
  Try to post the "msg" to the mailbox.
*/
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    LWIP_ASSERT("sys_mbox_trypost ", sys_mbox_valid(mbox)); 
    
    if (msg == NULL ) 
    {
        msg = pvNullPointer;
    }
    
    if (OSQPost((*mbox)->pQ, msg) != OS_ERR_NONE)
    {
        return (ERR_MEM);
    }

    return (ERR_OK);
}

/*-----------------------------------------------------------------------------------*/
/*
  Blocks the thread until a message arrives in the mailbox, but does
  not block the thread longer than "timeout" milliseconds (similar to
  the sys_arch_sem_wait() function). The "msg" argument is a result
  parameter that is set by the function (i.e., by doing "*msg =
  ptr"). The "msg" parameter maybe NULL to indicate that the message
  should be dropped.

  The return values are the same as for the sys_arch_sem_wait() function:
  Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
  timeout.

  Note that a function with a similar name, sys_mbox_fetch(), is
  implemented by lwIP. 
*/
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
    u8_t	ucErr;
    u32_t	ucos_timeout, timeout_new;
    void	*temp;

    LWIP_ASSERT("sys_arch_mbox_fetch ", sys_mbox_valid(mbox)); 
    
    if (timeout != 0)
    {
        ucos_timeout = (timeout * OS_TICKS_PER_SEC) / 1000; // convert to timetick
        if(ucos_timeout < 1)
        {
            ucos_timeout = 1;
        }
        else if(ucos_timeout > 65536)                       // uC/OS only support u16_t pend
        {
            ucos_timeout = 65535;       
        }
    }
    else 
    {
        ucos_timeout = 0;
    }

    timeout = OSTimeGet();
    
    temp = OSQPend((*mbox)->pQ, (u16_t)ucos_timeout, &ucErr );
    if (msg != NULL)
    {
    	if (temp == pvNullPointer)
        {
            *msg = NULL;
    	}
        else
        {
            *msg = temp;
        }
    }   
    
    if (ucErr == OS_ERR_TIMEOUT) 
    {
        timeout = SYS_ARCH_TIMEOUT;
    }
    else if (ucErr == OS_ERR_NONE)
    {
        timeout_new = OSTimeGet();
        if (timeout_new > timeout) 
        {
            timeout_new = timeout_new - timeout;
        }
        else 
        {
            timeout_new = 0xffffffff - timeout + timeout_new;
        }

        timeout = timeout_new * 1000 / OS_TICKS_PER_SEC + 1; //convert to milisecond
    }
    else
    {
        LWIP_ASSERT( "OSQPend ", ucErr != OS_ERR_EVENT_TYPE );
        LWIP_ASSERT( "OSQPend ", ucErr != OS_ERR_PEVENT_NULL );
        LWIP_ASSERT( "OSQPend ", ucErr != OS_ERR_PEND_ISR );
    }

	return timeout; 
}


u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
    void	*temp;
    u8_t    ucErr;
    
    LWIP_ASSERT("sys_arch_mbox_tryfetch ", sys_mbox_valid(mbox));      
        
    temp = OSQAccept((*mbox)->pQ, &ucErr);
    if (ucErr == OS_ERR_Q_EMPTY)
    {
        return (SYS_MBOX_EMPTY);
    }
    else if (ucErr == OS_ERR_NONE)
    {
    	if (temp == pvNullPointer)
        {
            *msg = NULL;
    	}
        else
        {
            *msg = temp;
        }
    }
    else
    {   
        LWIP_ASSERT( "OSQAccept ", ucErr != OS_ERR_EVENT_TYPE );
        LWIP_ASSERT( "OSQAccept ", ucErr != OS_ERR_PEVENT_NULL );
    }

    return (ERR_OK);
}

int sys_mbox_valid(sys_mbox_t *mbox)
{
    if ((u32_t)pcQueueMemoryPool <= (u32_t)(*mbox) && (u32_t)(*mbox) < (u32_t)(&pcQueueMemoryPool + 1))
    {
        if (((u32_t)(*mbox) - (u32_t)pcQueueMemoryPool) % sizeof(TMBOXQ_DESCR) == 0)
        {
            return (1);
        }
    }
    return (0);
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    *mbox = NULL;
}

/*-----------------------------------------------------------------------------------*/
/*
  Starts a new thread with priority "prio" that will begin its execution in the
  function "thread()". The "arg" argument will be passed as an argument to the
  thread() function. The id of the new thread is returned. Both the id and
  the priority are system dependent.
*/
sys_thread_t sys_thread_new(const char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio)
{
    u8_t ucPrio = 0;
    u8_t ucErr;
    
    arg = arg;
    
    if ((prio > 0) && (prio <= LWIP_TASK_MAX))
    {
        ucPrio = (u8_t)(LWIP_START_PRIO + prio - 1);
        stacksize = LWIP_STK_SIZE;
        
#if (OS_TASK_STAT_EN == 0)
        OSTaskCreate(thread, (void *)arg, &LWIP_TASK_STK[prio - 1][stacksize - 1],ucPrio);
#else
        OSTaskCreateExt(
                        thread, 
                        (void *)arg, 
                        &LWIP_TASK_STK[prio - 1][stacksize - 1],
                        ucPrio,
                        ucPrio,
                        &LWIP_TASK_STK[prio - 1][0],
                        stacksize,
                        (void *)0,
                        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
                        );
#endif
        OSTaskNameSet(ucPrio, (u8_t *)name, &ucErr);
    }

    return ucPrio;
}



