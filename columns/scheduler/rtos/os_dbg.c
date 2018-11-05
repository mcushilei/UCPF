
#include ".\source\os.h"

/*********************************************************************************************************
*                                             DEBUG DATA
*********************************************************************************************************/

UINT16  const  OSDebugEn           = OS_DEBUG_EN;               //!< Debug constants are defined below  

#if OS_DEBUG_EN > 0u

UINT32  const  OSEndiannessTest    = 0x00000001u;               //!< Variable to test CPU endianness    

UINT16  const  OSEventEn           = OS_EVENT_EN;
UINT16  const  OSEventMax          = OS_MAX_EVENTS;             //!< Number of event control blocks     
#if (OS_EVENT_EN) && (OS_MAX_EVENTS > 0u)
UINT16  const  OSEventSize         = sizeof(OS_EVENT);          //!< Size in Bytes of OS_EVENT          
#else
UINT16  const  OSEventSize         = 0u;
#endif

UINT16  const  OSFlagEn            = OS_FLAG_EN;
UINT16  const  OSFlagMax           = OS_MAX_FLAGS;
#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
UINT16  const  OSFlagSize          = sizeof(OS_FLAG);           //!< Size in Bytes of OS_FLAG       
UINT16  const  OSFlagNodeSize      = sizeof(OS_WAIT_NODE);      //!< Size in Bytes of OS_WAIT_NODE      
#else
UINT16  const  OSFlagSize          = 0u;
UINT16  const  OSFlagNodeSize      = 0u;
#endif

UINT16  const  OSLowestPrio        = OS_MAX_PRIO_LEVELS;

UINT16  const  OSMutexEn           = OS_MUTEX_EN;

UINT16  const  OSPtrSize           = sizeof(void *);            //!< Size in Bytes of a pointer         

UINT16  const  OSRdyTblSize        = OS_BITMAP_TBL_SIZE;        //!< Number of bytes in the ready table 

UINT16  const  OSSemEn             = OS_SEM_EN;

UINT16  const  OSStkWidth          = sizeof(OS_STK);            //!< Size in Bytes of a stack entry     

UINT16  const  OSTaskIdleStkSize   = OS_TASK_IDLE_STK_SIZE;
UINT16  const  OSTaskProfileEn     = OS_TASK_PROFILE_EN;
UINT16  const  OSTaskMax           = OS_MAX_TASKS + OS_N_SYS_TASKS; //!< Total max. number of tasks     
UINT16  const  OSTaskStatEn        = OS_STAT_EN;
UINT16  const  OSTaskStatStkSize   = OS_TASK_STAT_STK_SIZE;
UINT16  const  OSTaskStatStkChkEn  = OS_STAT_TASK_STK_CHK_EN;
UINT16  const  OSTaskSwHookEn      = OS_TASK_SW_HOOK_EN;

UINT16  const  OSTCBPrioTblMax     = OS_MAX_PRIO_LEVELS;       //!< Number of entries in osPrioTCBTbl[]
UINT16  const  OSTCBSize           = sizeof(OS_TCB);            //!< Size in Bytes of OS_TCB            

UINT16  const  OSTicksPerSec       = OS_TICKS_PER_SEC;
UINT16  const  OSTimeTickHookEn    = OS_TIME_TICK_HOOK_EN;

UINT16  const  OSVersionNbr        = OS_VERSION;

#endif

/*********************************************************************************************************
*                                             DEBUG DATA
*                            TOTAL DATA SPACE (in RAM) USED BY OS
*********************************************************************************************************/
#if OS_DEBUG_EN > 0u

UINT32  const  OSDataSize =
#if (OS_EVENT_EN > 0u) && (OS_MAX_EVENTS > 0u)
                          + sizeof(osEventFreeList)
                          + sizeof(osEventFreeTbl)
#endif
#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
                          + sizeof(osFlagFreeTbl)
                          + sizeof(osFlagFreeList)
#endif
                          + sizeof(osTCBFreeList)
                          + sizeof(osTCBFreeTbl)
                          + sizeof(osTaskList)
                          + sizeof(osRdyGrp)
                          + sizeof(osRdyTbl)
                          + sizeof(osPrioTCBTbl)
                          + sizeof(osTCBCur)
                          + sizeof(osTCBHighRdy)
                          + sizeof(osIntNesting)
                          + sizeof(osLockNesting)
                          + sizeof(osRunning)
#if OS_STAT_EN > 0u
                          + sizeof(osCtxSwCtr)
                          + sizeof(osIdleCtrMax)
                          + sizeof(osCPUUsage)
                          + sizeof(osStatRunning)
                          + sizeof(osTaskStatStk)
#endif
                          + sizeof(osIdleCtr)
                          + sizeof(osTaskIdleStk)
                          ;
#endif

/*********************************************************************************************************
*                                        OS DEBUG INITIALIZATION
*
* Description: This function is used to make sure that debug variables that are unused in the application
*              are not optimized away.  This function might not be necessary for all compilers.  In this
*              case, you should simply DELETE the code in this function while still leaving the declaration
*              of the function itself.
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : (1) This code doesn't do anything, it simply prevents the compiler from optimizing out
*                  the 'const' variables which are declared in this file.
*              (2) You may decide to 'compile out' the code (by using #if 0/#endif) INSIDE the function 
*                  if your compiler DOES NOT optimize out the 'const' variables above.
*********************************************************************************************************/

#if OS_DEBUG_EN > 0u
void  OSDebugInit (void)
{
    void  const  *ptemp;


    ptemp = (void const *)&OSDebugEn;

    ptemp = (void const *)&OSEndiannessTest;

    ptemp = (void const *)&OSEventMax;
    ptemp = (void const *)&OSEventEn;
    ptemp = (void const *)&OSEventSize;

    ptemp = (void const *)&OSFlagEn;
    ptemp = (void const *)&OSFlagSize;
    ptemp = (void const *)&OSFlagNodeSize;
    ptemp = (void const *)&OSFlagMax;

    ptemp = (void const *)&OSLowestPrio;

    ptemp = (void const *)&OSMutexEn;

    ptemp = (void const *)&OSPtrSize;

    ptemp = (void const *)&OSRdyTblSize;

    ptemp = (void const *)&OSSemEn;

    ptemp = (void const *)&OSStkWidth;

    ptemp = (void const *)&OSTaskIdleStkSize;
    ptemp = (void const *)&OSTaskProfileEn;
    ptemp = (void const *)&OSTaskMax;
    ptemp = (void const *)&OSTaskStatEn;
    ptemp = (void const *)&OSTaskStatStkSize;
    ptemp = (void const *)&OSTaskStatStkChkEn;
    ptemp = (void const *)&OSTaskSwHookEn;

    ptemp = (void const *)&OSTCBPrioTblMax;
    ptemp = (void const *)&OSTCBSize;

    ptemp = (void const *)&OSTicksPerSec;
    ptemp = (void const *)&OSTimeTickHookEn;

    ptemp = (void const *)&OSVersionNbr;

    ptemp = (void const *)&OSDataSize;

    ptemp = ptemp;                             //!< Prevent compiler warning for 'ptemp' not being used!
}
#endif
	 	   	  		 			 	    		   		 		 	 	 			 	    		   	 			 	  	 		 				 		  			 		 					 	  	  		      		  	   		      		  	 		 	      		   		 		  	 		 	      		  		  		  
