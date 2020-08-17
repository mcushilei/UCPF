/*******************************************************************************
 *  Copyright(C)2015-2018 by Dreistein<mcu_shilei@hotmail.com>                *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify it   *
 *  under the terms of the GNU Lesser General Public License as published     *
 *  by the Free Software Foundation; either version 3 of the License, or      *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful, but       *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU          *
 *  General Public License for more details.                                  *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this program; if not, see http://www.gnu.org/licenses/.        *
*******************************************************************************/

#ifndef __ULTILITIES_H__
#define __ULTILITIES_H__

/*============================ INCLUDES ======================================*/
#include ".\compiler\compiler.h"
#include ".\user_type.h"
#include ".\preprocessor\preprocessor.h"
#include ".\usebits.h"
#include ".\ooc.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name structure definition with pre-declaration
//! @{
#define DEF_STRUCTURE(__NAME)               \
            typedef struct __NAME __NAME;   \
            struct __NAME {

#define END_DEF_STRUCTURE(__NAME)           \
            };
//! @}

/*! \brief  change representation of a varible to another type. This is different
 *          from type conversion which change the BINARY value of varible.
 *  \note   It's assume that the destination type is memory alligned to original
 *          type.
 */
#define TYPE_CAST(__V, __T)         (*(__T *)&(__V))
            
#define SIZE_OF(__type, __member)   (sizeof( ((__type *)0)->__member ))

//! \brief  get compound type variable's address from one of its member's address.
#define CONTAINER_OF(__ptr, __type, __member) ( \
        (__type*)( (char*)(__ptr) - offsetof(__type, __member) ))


#define UBOUND(__ARRAY)             ( sizeof(__ARRAY) / sizeof(__ARRAY[0]) )
#define ARRAY_LENGTH(__ARRAY)       ( sizeof(__ARRAY) / sizeof(__ARRAY[0]) )

#define IS_POWER_OF_2(__N)  (\
        ((__N) != 0u) && (( ((__N) - 1u) & (__N) ) == 0) )

#define NEXT_POEWER_OF_2(__N, __V)          do {\
            uint32_t x = (__N);                 \
            if (IS_POWER_OF_2(x)) {             \
                __V = x;                        \
                break;                          \
            }                                   \
            x |= x >> 1;                        \
            x |= x >> 2;                        \
            x |= x >> 4;                        \
            x |= x >> 8;                        \
            x |= x >> 16;                       \
            __V = x + 1;                        \
        } while (0)

#define U32_COUNT_1S(__N, __V)  do {\
            uint32_t x = (__N);                 \
            x = (x & 0x55555555) + ((x >> 1)  & 0x55555555);\
            x = (x & 0x33333333) + ((x >> 2)  & 0x33333333);\
            x = (x & 0x0F0F0F0F) + ((x >> 4)  & 0x0F0F0F0F);\
            x = (x & 0x00FF00FF) + ((x >> 8)  & 0x00FF00FF);\
            x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);\
            __V = x;                            \
        } while (0)

#define U64_COUNT_1S(__N, __V)  do {\
            uint64_t x = (__N);                 \
            x = (x & 0x5555555555555555) + ((x >> 1)  & 0x5555555555555555);\
            x = (x & 0x3333333333333333) + ((x >> 2)  & 0x3333333333333333);\
            x = (x & 0x0F0F0F0F0F0F0F0F) + ((x >> 4)  & 0x0F0F0F0F0F0F0F0F);\
            x = (x & 0x00FF00FF00FF00FF) + ((x >> 8)  & 0x00FF00FF00FF00FF);\
            x = (x & 0x0000FFFF0000FFFF) + ((x >> 16) & 0x0000FFFF0000FFFF);\
            x = (x & 0x00000000FFFFFFFF) + ((x >> 16) & 0x00000000FFFFFFFF);\
            __V = x;                            \
        } while (0)

#define U32_COUNT_TRAILING_ZEROS(__N, __V)  do {\
            uint32_t x = (__N);                 \
            x = ((x - 1) | x) ^ x;              \
            x = (x & 0x55555555) + ((x >> 1)  & 0x55555555);\
            x = (x & 0x33333333) + ((x >> 2)  & 0x33333333);\
            x = (x & 0x0F0F0F0F) + ((x >> 4)  & 0x0F0F0F0F);\
            x = (x & 0x00FF00FF) + ((x >> 8)  & 0x00FF00FF);\
            x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);\
            __V = x;                            \
        } while (0)

#define U64_COUNT_TRAILING_ZEROS(__N, __V)  do {\
            uint64_t x = (__N);                 \
            x = ((x - 1) | x) ^ x;              \
            x = (x & 0x5555555555555555) + ((x >> 1)  & 0x5555555555555555);\
            x = (x & 0x3333333333333333) + ((x >> 2)  & 0x3333333333333333);\
            x = (x & 0x0F0F0F0F0F0F0F0F) + ((x >> 4)  & 0x0F0F0F0F0F0F0F0F);\
            x = (x & 0x00FF00FF00FF00FF) + ((x >> 8)  & 0x00FF00FF00FF00FF);\
            x = (x & 0x0000FFFF0000FFFF) + ((x >> 16) & 0x0000FFFF0000FFFF);\
            x = (x & 0x00000000FFFFFFFF) + ((x >> 16) & 0x00000000FFFFFFFF);\
            __V = x;                            \
        } while (0)
    
#define U32_COUNT_LEADING_ZEROS(__N, __V)   do {\
            uint32_t x = (__N);                 \
            x |= x >> 1;                        \
            x |= x >> 2;                        \
            x |= x >> 4;                        \
            x |= x >> 8;                        \
            x |= x >> 16;                       \
            x = ~x;                             \
            x = (x & 0x55555555u) + ((x >> 1)  & 0x55555555u);\
            x = (x & 0x33333333u) + ((x >> 2)  & 0x33333333u);\
            x = (x & 0x0F0F0F0Fu) + ((x >> 4)  & 0x0F0F0F0Fu);\
            x = (x & 0x00FF00FFu) + ((x >> 8)  & 0x00FF00FFu);\
            x = (x & 0x0000FFFFu) + ((x >> 16) & 0x0000FFFFu);\
            __V = x;                            \
        } while (0)

//! \brief math macros
#define MAX(__A,__B)        (((__A) > (__B)) ? (__A) : (__B))
#define MIN(__A,__B)        (((__A) < (__B)) ? (__A) : (__B))
#define ABS(__I)            (((__I) ^ ((__I) >> 31)) - ((__I) >> 31))
            
//! \brief LFSM(Little Finite State Machine) implement by 'goto' method;
#define LFSM_BEGIN(__M)             __##__M##_begin__:\
                                    {\
    
#define LFSM_END(__M)               }\
                                    __##__M##_end__:

#define LFSM_CPL(__M)               goto __##__M##_end__;

#define LFSM_STATE_BEGIN(__S, ...)  \
                                    __##__S##_ENTRY__:\
                                    {\
                                    ##__VA_ARGS__\
                                    }\
                                    __##__S##_begin__:\
                                    {

//! It just jumps at the start of this state if there is no state transfer.
#define LFSM_STATE_END(__S)         }\
                                    goto __##__S##_begin__;

#define LFSM_STATE_TRANS_TO(__S, ...)\
                                    {\
                                    ##__VA_ARGS__\
                                    }\
                                    goto __##__S##_ENTRY__;\

/* LFSM example:
    LFSM_BEGIN(test_fsm)
        uint8_t c = 4;
        LFSM_STATE_BEGIN(state_1)
            printf("\r\nc = %u", c);
            LFSM_STATE_TRANS_TO(state_2)
        LFSM_STATE_END(state_1)

        LFSM_STATE_BEGIN(state_2)
            uint8_t d = 4;
            printf("\r\nd = %u", d);
            c--;
            if (c == 0) {
                LFSM_CPL(test_fsm)
            } else {
                LFSM_STATE_TRANS_TO(state_1)
            }
        LFSM_STATE_END(state_2)
    LFSM_END(test_fsm)
*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif  //! #ifndef __TEMPLATE_H__
/* EOF */
