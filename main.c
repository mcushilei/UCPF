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




/*============================ INCLUDES ======================================*/
#include ".\beams\beams.h"
#include ".\applications\applications.h"
#ifdef __OS_WINDOWS__
#include ".\stdafx.h"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

/*! \note main function
 *  \param none
 *  \return none
 */
#ifdef __OS_WINDOWS__
int main(int argc, uint8_t *argv[], uint8_t *envp[])
#else
int main(void)
#endif
{
    /* initialize whole system */
    if (!framework_init()) {
        return 0;
    }

    app_main();

    return 0;
}
 
/* EOF */
