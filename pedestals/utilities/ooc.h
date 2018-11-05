/***************************************************************************
 *   Copyright(C)2009-2012 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __OBJECT_ORIENTED_C_H__
#define __OBJECT_ORIENTED_C_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class definition
//! @{
#define CLASS(__NAME)           __NAME

#define DEF_CLASS(__NAME)                   \
            typedef struct __NAME __NAME;   \
            struct __NAME {

#define END_DEF_CLASS(__NAME)               \
            };

#define EXTERN_CLASS(__NAME)                \
            typedef struct __NAME __NAME;   \
            struct __NAME {

#define END_EXTERN_CLASS(__NAME)            \
            };
//! @}

//! \name interface definition
//! @{
#define DEF_INTERFACE(__NAME)               \
            typedef struct __NAME __NAME;   \
            struct __NAME {

#define END_DEF_INTERFACE(__NAME)           \
            };
//! @}


/*============================ TYPES =========================================*/
//! \name interface: u32_property_t
//! @{
DEF_INTERFACE(u32_property_t)
    bool        (*Set)(uint32_t value);
    uint32_t    (*Get)(void);
END_DEF_INTERFACE(u32_property_t)
//! @}

//! \name interface: u16_property_t
//! @{
DEF_INTERFACE(u16_property_t)
    bool        (*Set)(uint16_t value);
    uint16_t    (*Get)(void);
END_DEF_INTERFACE(u16_property_t)
//! @}

//! \name interface: u8_property_t
//! @{
DEF_INTERFACE(u8_property_t)
    bool        (*Set)(uint8_t value);
    uint8_t     (*Get)(void);
END_DEF_INTERFACE(u8_property_t)
//! @}


//! \name interface: i32_property_t
//! @{
DEF_INTERFACE(i32_property_t)
    bool        (*Set)(int32_t value);
    int32_t     (*Get)(void);
END_DEF_INTERFACE(i32_property_t)
//! @}

//! \name interface: i16_property_t
//! @{
DEF_INTERFACE(i16_property_t)
    bool        (*Set)(int16_t value);
    int16_t     (*Get)(void);
END_DEF_INTERFACE(i16_property_t)
//! @}

//! \name interface: u8_property_t
//! @{
DEF_INTERFACE(i8_property_t)
    bool        (*Set)(int8_t value);
    int8_t      (*Get)(void);
END_DEF_INTERFACE(i8_property_t)
//! @}


//! \name interface: bool_property_t
//! @{
DEF_INTERFACE(bool_property_t)
    bool        (*Set)(bool value);
    bool        (*Get)(void);
END_DEF_INTERFACE(bool_property_t)
//! @}

//! \name interface: bool_property_t
//! @{
DEF_INTERFACE(en_property_t)
    bool        (*Enable)(void);
    bool        (*Disable)(void);
END_DEF_INTERFACE(en_property_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

#endif
/* EOF */
