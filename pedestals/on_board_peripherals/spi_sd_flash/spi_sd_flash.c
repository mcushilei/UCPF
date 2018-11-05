/*******************************************************************************
 *  Copyright(C)2016 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
#define SD_FLASH_BLOCK_SIZE         (512u)   /* Block Size in Bytes */

/** 
  * @brief  Mask for Card Status R1 (CSR Register) 
  */
#define SD_CSR_ADDR_OUT_OF_RANGE        (0x80000000u)
#define SD_CSR_ADDR_MISALIGNED          (0x40000000u)
#define SD_CSR_BLOCK_LEN_ERR            (0x20000000u)
#define SD_CSR_ERASE_SEQ_ERR            (0x10000000u)
#define SD_CSR_BAD_ERASE_PARAM          (0x08000000u)
#define SD_CSR_WRITE_PROT_VIOLATION     (0x04000000u)
#define SD_CSR_CARD_IS_LOCKED           (0x02000000u)
#define SD_CSR_LOCK_UNLOCK_FAILED       (0x01000000u)
#define SD_CSR_COM_CRC_FAILED           (0x00800000u)
#define SD_CSR_ILLEGAL_CMD              (0x00400000u)
#define SD_CSR_CARD_ECC_FAILED          (0x00200000u)
#define SD_CSR_CC_ERROR                 (0x00100000u)
#define SD_CSR_GENERAL_UNKNOWN_ERROR    (0x00080000u)
#define SD_CSR_STREAM_READ_UNDERRUN     (0x00040000u)
#define SD_CSR_STREAM_WRITE_OVERRUN     (0x00020000u)
#define SD_CSR_CID_CSD_OVERWRIETE       (0x00010000u)
#define SD_CSR_WP_ERASE_SKIP            (0x00008000u)
#define SD_CSR_CARD_ECC_DISABLED        (0x00004000u)
#define SD_CSR_ERASE_RESET              (0x00002000u)
#define SD_CSR_READY_FOR_DATA           (0x00000100u)
#define SD_CSR_APP_CMD                  (0x00000020u)
#define SD_CSR_AKE_SEQ_ERROR            (0x00000008u)
#define SD_CSR_ERRORBITS                (0xFDFFE008u)

#define SD_R1_IDLE_STATE_MSK            (0x01u)
#define SD_R1_ERASE_RESET_MSK           (0x02u)
#define SD_R1_ILLEGAL_CMD_MSK           (0x04u)
#define SD_R1_CRC_ERROR_MSK             (0x08u)
#define SD_R1_ERASE_SEQ_ERROR_MSK       (0x10u)
#define SD_R1_ADDRESS_ERROR_MSK         (0x20u)
#define SD_R1_PARAM_ERROR_MSK           (0x40u)

/** 
  * @brief  Masks for R6 Response 
  */
#define SD_R6_GENERAL_UNKNOWN_ERROR     (0x00002000u)
#define SD_R6_ILLEGAL_CMD               (0x00004000u)
#define SD_R6_COM_CRC_FAILED            (0x00008000u)

/** 
  * @brief arg used by ACMD41
  */
#define SD_ACMD41_HOST_CAPACITY_SUPPORT_MSK (1u << 30)       // host is support SDHC or SDXC
#define SD_ACMD41_ARGUMENT                  (SD_ACMD41_HOST_CAPACITY_SUPPORT_MSK)

/*! 
 *! \brief arg used by CMD8
 */
/*!< Argument: - [31:12]: Reserved (shall be set to '0')
               - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V), other value are resverd.
               - [7:0]: Check Pattern (recommended 0xAA) */
#define SD_CHECK_PATTERN                (0xAAu)       // CHECK PATTERN
#define SD_SUPPLY_VOLTAGE               (0x01u)       // 2.7-3.6V
#define SD_CMD8_SUPPLY_VOLTAGE_BIT0     8
#define SD_CMD8_ARGUMENT                ((SD_SUPPLY_VOLTAGE << 8) | SD_CHECK_PATTERN)

/*! 
 *! \brief SD OCR register mask
 */
#define SD_OCR_VDD_BIT0                 15
#define SD_OCR_VDD_MSK                  MASK(23, 15)
#define SD_OCR_CCS_MSK                  (1u << 30)
#define SD_OCR_POWER_UP_MSK             (1u << 31)


/** 
  * @brief SD Commands  Index 
  */
#define SD_CMD_GO_IDLE_STATE                       (0)
#define SD_CMD_SEND_OP_COND                        (1)
#define SD_CMD_ALL_SEND_CID                        (2)
#define SD_CMD_SEND_RELATIVE_ADDR                  (3)
#define SD_CMD_SET_DSR                             (4)
#define SD_CMD_SDIO_SEN_OP_COND                    (5)
#define SD_CMD_SWITCH_FUNC                         (6)
#define SD_CMD_SEL_DESEL_CARD                      (7)
#define SD_CMD_SEND_IF_COND                        (8)
#define SD_CMD_SEND_CSD                            (9)
#define SD_CMD_SEND_CID                            (10)
#define SD_CMD_VOLTAGE_SWITCH                      (11)
#define SD_CMD_STOP_TRANSMISSION                   (12)
#define SD_CMD_SEND_STATUS                         (13)
#define SD_CMD_HS_BUSTEST_READ                     (14)
#define SD_CMD_GO_INACTIVE_STATE                   (15)
#define SD_CMD_SET_BLOCKLEN                        (16)
#define SD_CMD_READ_SINGLE_BLOCK                   (17)
#define SD_CMD_READ_MULT_BLOCK                     (18)
#define SD_CMD_HS_BUSTEST_WRITE                    (19)
#define SD_CMD_SPEED_CLASS_CONTROL                 (20)
#define SD_CMD_SET_BLOCK_COUNT                     (23)
#define SD_CMD_WRITE_SINGLE_BLOCK                  (24)
#define SD_CMD_WRITE_MULT_BLOCK                    (25)
#define SD_CMD_PROG_CID                            (26) /*!< reserved for manufacturers */
#define SD_CMD_PROG_CSD                            (27)
#define SD_CMD_SET_WRITE_PROT                      (28)
#define SD_CMD_CLR_WRITE_PROT                      (29)
#define SD_CMD_SEND_WRITE_PROT                     (30)
#define SD_CMD_ERASE_WR_BLK_START_ADDR             (32)
#define SD_CMD_ERASE_WR_BLK_END_ADDR               (33)
#define SD_CMD_ERASE_GRP_START                     (35)
#define SD_CMD_ERASE_GRP_END                       (36)
#define SD_CMD_ERASE                               (38)
#define SD_CMD_FAST_IO                             (39)
#define SD_CMD_GO_IRQ_STATE                        (40)
#define SD_CMD_LOCK_UNLOCK                         (42)
#define SD_CMD_APP_CMD                             (55)
#define SD_CMD_GEN_CMD                             (56)
#define SD_CMD_READ_OCR                            (58)
#define SD_CMD_CRC_ON_OFF                          (59)
#define SD_CMD_NO_CMD                              (64)

/** 
  * @brief Following commands are SD Card Specific commands.
  *        SDIO_APP_CMD(CMD55) should be sent before sending these commands. 
  */
#define SD_ACMD_SET_BUSWIDTH                    (6)
#define SD_ACMD_STAUS                           (13)
#define SD_ACMD_SECURE_READ_MULTIPLE_BLOCK      (18)
#define SD_ACMD_SEND_NUM_WRITE_BLOCKS           (22)
#define SD_ACMD_SET_WR_BLK_ERASE_COUNT          (23)
#define SD_ACMD_SECURE_WRITE_MULTIPLE_BLOCK     (25)
#define SD_ACMD_SECURE_ERASE                    (38)
#define SD_ACMD_SEND_OP_COND                    (41)
#define SD_ACMD_SET_CLR_CARD_DETECT             (42)
#define SD_ACMD_GET_MKB                         (43)
#define SD_ACMD_GET_MID                         (44)
#define SD_ACMD_SET_CER_RN1                     (45)
#define SD_ACMD_GET_CER_RN2                     (46)
#define SD_ACMD_SET_CER_RES2                    (47)
#define SD_ACMD_GET_CER_RES1                    (48)
#define SD_ACMD_SECURE_WRITE_MKB                (48)
#define SD_ACMD_SECURE_CHANGE_AREA              (49)
#define SD_ACMD_SEND_SCR                        (51)
#define SD_ACMD_SDIO_RW_DIRECT                  (52)
#define SD_ACMD_SDIO_RW_EXTENDED                (53)
      
#define SD_SPI_DATA_RESP_TOKEN_STATUS_MSK           (0x1Fu)
#define SD_SPI_DATA_RESP_TOKEN_STATUS_ACCEPTED      (0x05u)
#define SD_SPI_DATA_RESP_TOKEN_STATUS_CRC_ERROR     (0x0Bu)
#define SD_SPI_DATA_RESP_TOKEN_STATUS_WRITE_ERROR   (0x0Du)

#define SD_CMD_FORMAT_START_AND_TRANS_BIT_MSK       (0x40u)
#define SD_CMD_FORMAT_END_BIT_MSK                   (0x01u)
/*============================ MACROFIED FUNCTIONS ===========================*/
#define SD_TIME_SET(__N)            s_wTimer = __N
#define SD_TIME_IS_OVERFLOW()       (0 == (--s_wTimer))

	
#define __SD_CMD_ITEM(__N, __VALUE)     SD_CMD_CMD##__N = __N,
#define __SD_ACMD_ITEM(__N, __VALUE)    SD_CMD_ACMD##__N = __N,

      
      
/*============================ TYPES =========================================*/
typedef enum {
    MREPEAT(80, __SD_CMD_ITEM, 0)
} em_sd_cmd_t;

typedef enum {
    MREPEAT(80, __SD_ACMD_ITEM, 0)
} em_sd_acmd_t;

/*! 
 *! @brief  SDIO specific error defines  
 */   
typedef enum {
    SD_ERR_NONE                         = 0, 
    SD_ERR_CMD_RSP_CRC_FAIL,                /*!< Command response received (but CRC check failed) */
    SD_ERR_CMD_RSP_TIMEOUT,                 /*!< Command response timeout */
    SD_ERR_DATA_CRC_FAIL,                   /*!< Data block sent/received (CRC check Failed) */
    SD_ERR_DATA_TIMEOUT,                    /*!< Data time out */
    SD_ERR_TX_UNDERRUN,                     /*!< Transmit FIFO under-run */
    SD_ERR_RX_OVERRUN,                      /*!< Receive FIFO over-run */
    SD_ERR_START_BIT_ERR,                   /*!< Start bit not detected on all data signals in wide bus mode */
    SD_ERR_CMD_OUT_OF_RANGE,                /*!< CMD's argument was out of range.*/
    SD_ERR_ADDR_MISALIGNED,                 /*!< Misaligned address */
    SD_ERR_BLOCK_LEN_ERR,                   /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
    SD_ERR_ERASE_SEQ_ERR,                   /*!< An error in the sequence of erase command occurs.*/
    SD_ERR_BAD_ERASE_PARAM,                 /*!< An Invalid selection for erase groups */
    SD_ERR_WRITE_PROT_VIOLATION,            /*!< Attempt to program a write protect block */
    SD_ERR_LOCK_UNLOCK_FAILED,              /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
    SD_ERR_CMD_ILLEGAL,                     /*!< Command is not legal for the card state */
    SD_ERR_CARD_ECC_FAILED,                 /*!< Card internal ECC was applied but failed to correct the data */
    SD_ERR_CARD_INTERNAL_ERROR,             /*!< Internal card controller error */
    SD_ERR_STREAM_READ_UNDERRUN,            /*!< The card could not sustain data transfer in stream read operation. */
    SD_ERR_STREAM_WRITE_OVERRUN,            /*!< The card could not sustain data programming in stream mode */
    SD_ERR_CID_CSD_OVERWRITE,               /*!< CID/CSD overwrite error */
    SD_ERR_WP_ERASE_SKIP,                   /*!< only partial address space was erased */
    SD_ERR_CARD_ECC_DISABLED,               /*!< Command has been executed without using internal ECC */
    SD_ERR_ERASE_RESET,                     /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
    SD_ERR_AKE_SEQ_ERROR,                   /*!< Error in sequence of authentication. */
    SD_ERR_INVALID_VOLTRANGE,               
    SD_ERR_ADDR_OUT_OF_RANGE,               
    SD_ERR_SWITCH_ERROR,                    
    SD_ERR_SDIO_DISABLED,                   
    SD_ERR_SDIO_FUNCTION_BUSY,              
    SD_ERR_SDIO_FUNCTION_FAILED,            
    SD_ERR_SDIO_UNKNOWN_FUNCTION,           
} em_sd_error_t;

/** 
  * @brief Supported SD Memory Card's type 
  */
typedef enum {
    SD_UNKNOW_CARD_TYPE = 0,
    SD_V1_CARD,
    SD_V2_0_SC_CARD,
    SD_V2_0_HC_CARD,
    SD_SDIO_CARD,
    SD_SDIO_COMBO_CARD,
} em_sd_device_type;

/*! 
 *! @brief SD Card information 
 */
typedef struct {
    uint32_t wCardCapacity;     /*!< Card Capacity */
    uint32_t wCardMaxBlockSize; /*!< Card Block Size */
    uint16_t hwRCA;
    uint8_t  chCardType;
    uint8_t  chCID[16];
    uint8_t  chCSD[16];
    uint8_t  chSCR[8];
} sd_card_properties_t;


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static volatile uint32_t s_wTimer = 0;
static sd_card_properties_t s_tCardProperties;

/*============================ PROTOTYPES ====================================*/
extern void     sd_spi_write_byte(uint8_t Data);
extern uint8_t  sd_spi_read_byte(void);
extern void     sd_spi_cs_clr(void);
extern void     sd_spi_cs_set(void);
extern uint8_t  sd_crc7_calculator(uint8_t chCRCValue, uint8_t chData);
extern uint8_t  sd_crc16_calculator(uint16_t hwCRCValue, uint8_t chData);

/*============================ IMPLEMENTATION ================================*/
static bool spi_sd_wait_busy(void)
{
    SD_TIME_SET(1000);
    do {
        if (sd_spi_read_byte() == 0xFF) {
            return TRUE;
        }
    } while (!SD_TIME_IS_OVERFLOW());

    return FALSE;
}

static uint32_t spi_sd_send_cmd(uint8_t cmd, uint32_t arg, uint8_t *pchResp, uint32_t len) 
{
    uint32_t wR1, i, wRes;
    uint8_t crc_end;

    //! Wait until the card is ready to receive cmd.
    if (!spi_sd_wait_busy()) {
        return 0x81;
    }

    //! Calculate CRC.
    cmd |= SD_CMD_FORMAT_START_AND_TRANS_BIT_MSK;
    crc_end = sd_crc7_calculator(0, cmd);
    crc_end = sd_crc7_calculator(crc_end, arg >> 24);
    crc_end = sd_crc7_calculator(crc_end, arg >> 16);
    crc_end = sd_crc7_calculator(crc_end, arg >> 8);
    crc_end = sd_crc7_calculator(crc_end, arg);
    crc_end |= SD_CMD_FORMAT_END_BIT_MSK; //!< CRC7 + End bit.

    //! Send command.
    sd_spi_write_byte(cmd);
    sd_spi_write_byte(arg >> 24);
    sd_spi_write_byte(arg >> 16);
    sd_spi_write_byte(arg >> 8);
    sd_spi_write_byte(arg);
    sd_spi_write_byte(crc_end); /* Valid or dummy CRC plus stop bit */

    //! The command response time is 0 to 8 bytes for SDC, 1 to 8 bytes for MMC. */
    for (i = 8; i; i--) {
        wR1 = sd_spi_read_byte();
        if (wR1 != 0xFF) {
            break;   /* received valid response */
        }
    }
    if (i == 0) { 
        return 0x82; /* command response time out error */
    } else {
        wRes = wR1;
    }

    /* Read remaining bytes after R1 response */
    if (pchResp && len) {
        do {
            *pchResp++ = sd_spi_read_byte ();
        } while (--len);
    }
    
    return wRes;
}

static uint8_t spi_sd_send_acmd(uint8_t cmd, uint32_t arg, uint8_t *buf, uint32_t len)
{
    uint8_t wR1;

	wR1 = spi_sd_send_cmd(SD_CMD_CMD55, 0, NULL, 0);
	if (wR1 & (~SD_R1_IDLE_STATE_MSK)) {
       return wR1;
    }
    
    wR1 = spi_sd_send_cmd(cmd, arg, buf, len);
    
    return wR1;
}

bool spi_sd_card_detect(void)
{
    uint8_t chRes[4];
    uint32_t wR1;
    uint32_t wOCR = 0;
    word_t tWord;
    
    s_tCardProperties.chCardType = SD_UNKNOW_CARD_TYPE;
    
//    /* Before reset, Send at least 74 clocks at low frequency 
//    (between 100kHz and 400kHz) with CS high and DI (MISO) high. */
//    for (uint32_t i = 0; i < 10; i++) {
//        sd_spi_write_byte(0xFF);
//    }
    
    //! The CS signal must be kept low during a transaction
    sd_spi_cs_clr();
    
    wR1 = spi_sd_send_cmd(SD_CMD_CMD0, 0, NULL, 0);
    if (wR1 != SD_R1_IDLE_STATE_MSK) {
        goto __CARD_DETECT_FAULT_EXIT;
    }
    
    wR1 = spi_sd_send_cmd(SD_CMD_CMD8, SD_CMD8_ARGUMENT, chRes, 4);
    if (wR1 & 0x80) {
        goto __CARD_DETECT_FAULT_EXIT;
    }
    
    //! analyzing card operating condition
    if (wR1 & SD_R1_ILLEGAL_CMD_MSK) {  /* It's Ver1.x SD card or MMC card */
        
        wR1 = spi_sd_send_cmd(SD_CMD_CMD58, 0, chRes, 4);
        if (wR1 & SD_R1_ILLEGAL_CMD_MSK) {
            goto __CARD_DETECT_FAULT_EXIT;
        }
        tWord.Byte0 = chRes[0];
        tWord.Byte1 = chRes[1];
        tWord.Byte2 = chRes[2];
        tWord.Byte3 = chRes[3];
        wOCR = tWord.Value;
        //! get card operate valtage if need.

        //! Check if it is SD card
        SD_TIME_SET(1000);
        do {
            wR1 = spi_sd_send_acmd (SD_CMD_ACMD41, 0, NULL, 0);
            if (wR1 == 0x00) {
                break;
            } else if (wR1 & (~SD_R1_IDLE_STATE_MSK)) {
                //! not a SD card or error happen.
                goto __CARD_DETECT_FAULT_EXIT;
            }
        } while (!SD_TIME_IS_OVERFLOW());
        s_tCardProperties.chCardType = SD_V1_CARD;
            
    } else {                              /* It's V2.0 or later SD card */
        if ((chRes[2] & 0x0f) != SD_SUPPLY_VOLTAGE || chRes[3] != SD_CHECK_PATTERN) {
            goto __CARD_DETECT_FAULT_EXIT;
        }

        wR1 = spi_sd_send_cmd(SD_CMD_CMD58, 0, chRes, 4);
        tWord.Byte0 = chRes[3];
        tWord.Byte1 = chRes[2];
        tWord.Byte2 = chRes[1];
        tWord.Byte3 = chRes[0];
        wOCR = tWord.Value;
        //! get card operate valtage if need.
        
        SD_TIME_SET(1000);
        //! The card is SD V2 and can work at voltage range of 2.7 to 3.6V
        do {
            wR1 = spi_sd_send_acmd (SD_CMD_ACMD41, SD_ACMD41_ARGUMENT, NULL, 0);
            if (wR1 == 0x00) {
                break;
            } else if (wR1 & (~SD_R1_IDLE_STATE_MSK)) {
                goto __CARD_DETECT_FAULT_EXIT;
            }
        } while (!SD_TIME_IS_OVERFLOW());
        
        wR1 = spi_sd_send_cmd(SD_CMD_CMD58, 0, chRes, 4);
        tWord.Byte0 = chRes[3];
        tWord.Byte1 = chRes[2];
        tWord.Byte2 = chRes[1];
        tWord.Byte3 = chRes[0];
        wOCR = tWord.Value;
        s_tCardProperties.chCardType = (wOCR & SD_OCR_CCS_MSK) ? SD_V2_0_HC_CARD : SD_V2_0_SC_CARD;
        
    }
    
    /* For SDHC or SDXC, block length is fixed to 512 bytes, for others,
    the block length is set to 512 manually. */
    if (s_tCardProperties.chCardType == SD_V1_CARD
    ||  s_tCardProperties.chCardType == SD_V2_0_SC_CARD ) {
        wR1 = spi_sd_send_cmd(SD_CMD_CMD16, SD_FLASH_BLOCK_SIZE, NULL, 0);
        if (wR1) {
            s_tCardProperties.chCardType = SD_UNKNOW_CARD_TYPE;
            goto __CARD_DETECT_FAULT_EXIT;
        }
    }
    
    sd_spi_cs_set();
    return true;

__CARD_DETECT_FAULT_EXIT:
    sd_spi_cs_set();
    return false;
}

static bool spi_sd_read_data(uint8_t *buf, uint32_t len)
{
    uint8_t chByte;
    uint32_t i;
    uint16_t hwCRC = 0;

    //! Read data token (0xFE)
	SD_TIME_SET(1000);
	do {							
		chByte = sd_spi_read_byte();
        if (chByte == 0xFE) {
            break;
        }
	} while (!SD_TIME_IS_OVERFLOW());
	if (chByte != 0xFE) {
        return false;	/* data read timeout */
    }

    //! Read data block
    for (i = 0; i < len; i++) {
        chByte = sd_spi_read_byte();
        buf[i] = chByte;
        hwCRC = sd_crc16_calculator(hwCRC, chByte);
    }

    //! 2 bytes CRC 
    chByte = sd_spi_read_byte();
    hwCRC = sd_crc16_calculator(hwCRC, chByte);
    chByte = sd_spi_read_byte();
    hwCRC = sd_crc16_calculator(hwCRC, chByte);
    //! hwCRC shuld be 0.

    return true;
}

static bool spi_sd_write_data(const uint8_t *buf, uint8_t tkn, uint32_t len) 
{
    uint32_t i;
    
    /* Send Start Block Token */
    sd_spi_write_byte(tkn);

    /* Send data block */
    for (i = 0; i < len; i++) {
        sd_spi_write_byte(buf[i]);
    }

    /* Send 2 bytes dummy CRC */
    sd_spi_write_byte(0xFF);
    sd_spi_write_byte(0xFF);

    /* Read data response to check if the data block has been accepted. */
    if ((sd_spi_read_byte() & SD_SPI_DATA_RESP_TOKEN_STATUS_MSK)
    != SD_SPI_DATA_RESP_TOKEN_STATUS_ACCEPTED) {
        return false; /* write error */
    }

    return true;    /* write time out */
}

bool spi_sd_get_card_info(void)
{
    uint32_t wR1 = 0;
    uint8_t chBuf[16];
    
    /* Read CID */
    sd_spi_cs_clr();
    wR1 = spi_sd_send_cmd(SD_CMD_SEND_CID, 0, NULL, 0);
    if (0 != wR1) {
        sd_spi_cs_set();
        return false;
    }
    if (!spi_sd_read_data(chBuf, 16)) {
        sd_spi_cs_set();
        return false;
    }
    sd_spi_cs_set();
    //! paser CID here.

    /* Read CSD */
    sd_spi_cs_clr();
    wR1 = spi_sd_send_cmd(SD_CMD_SEND_CSD, 0, NULL, 0);
    if (0 != wR1) {
        sd_spi_cs_set();
        return false;
    }
    if (!spi_sd_read_data(chBuf, 16)) {
        sd_spi_cs_set();
        return false;
    }
    sd_spi_cs_set();
    //! paser CSD here.

    return true;
}

bool spi_sd_read_blocks(uint32_t block, uint8_t *buf, uint32_t cnt)
{
    uint32_t wR1 = 0;
    bool bRetVal = false;
    
    //! Convert sector-based address to byte-based address for non SDHC.
    if (SD_V2_0_HC_CARD != s_tCardProperties.chCardType) {
        block <<= 9;
    }

    //! The CS signal must be kept low during a transaction.
    sd_spi_cs_clr();
    
    if (cnt > 1) {  /* Read multiple block */
        wR1 = spi_sd_send_cmd(SD_CMD_READ_MULT_BLOCK, block, NULL, 0);
		if (0 == wR1) {           
			do {
				if (!spi_sd_read_data(buf, SD_FLASH_BLOCK_SIZE)) {
                    break;
                }
				buf += SD_FLASH_BLOCK_SIZE;
			} while (--cnt);

			/* Stop transmission */
            spi_sd_send_cmd(SD_CMD_STOP_TRANSMISSION, 0, NULL, 0);				

            /* Wait for the card is ready */
            if (spi_sd_wait_busy() && cnt == 0) {
                bRetVal = true;
            }
        }
    } else {    /* Read single block */
        wR1 = spi_sd_send_cmd(SD_CMD_READ_SINGLE_BLOCK, block, NULL, 0);
        if (0 == wR1) {
            if (spi_sd_read_data(buf, SD_FLASH_BLOCK_SIZE)) {
                bRetVal = true;
            }
        }
    }

    sd_spi_cs_set();
    return bRetVal;
}

bool spi_sd_write_blocks(uint32_t block, const uint8_t *buf, uint32_t cnt)
{
    bool bRetVal = false;
    uint32_t wR1 = 0;
    uint8_t recv;
    
    if (cnt == 0u) {
        return true;
    }
    
    //! Convert sector-based address to byte-based address for non SDHC.
    if (SD_V2_0_HC_CARD != s_tCardProperties.chCardType) {
        block <<= 9;
    }

    //! The CS signal must be kept low during a transaction.
    sd_spi_cs_clr();
    
    if (cnt > 1) { /* write multiple block */
        wR1 = spi_sd_send_cmd(SD_CMD_WRITE_MULT_BLOCK, block, NULL, 0);
        if (0 == wR1) {
            for (; cnt != 0u; cnt--) {
                if (!spi_sd_write_data(buf, 0xFC, SD_FLASH_BLOCK_SIZE)) {
                    break;
                }
                
                /* Wait for wirte complete. */
                SD_TIME_SET(1000);
                do {
                    recv = sd_spi_read_byte();
                    if (recv == 0xFF) {
                        break;
                    }
                } while (!SD_TIME_IS_OVERFLOW());
                if (recv != 0xFF) {
                    break;
                }

                buf += SD_FLASH_BLOCK_SIZE;
            }

            /* Stop transmission */
            if (cnt == 0u) {
                spi_sd_send_cmd(SD_CMD_STOP_TRANSMISSION, 0, NULL, 0);
            } else {
                sd_spi_write_byte(0xFD);
            }
        
            /* Wait for complete */
            if (spi_sd_wait_busy() && cnt == 0) {
                bRetVal = true;
            }
        }
    } else { /* write single block */
        wR1 = spi_sd_send_cmd(SD_CMD_WRITE_SINGLE_BLOCK, block, NULL, 0);
        if (0 == wR1) {
            if (spi_sd_write_data(buf, 0xFE, SD_FLASH_BLOCK_SIZE)) {
                
                /* Wait for wirte complete. */
                SD_TIME_SET(8000);
                do {
                    recv = sd_spi_read_byte();
                    if (recv == 0xFF) {
                        break;
                    }
                } while (!SD_TIME_IS_OVERFLOW());
                if (recv == 0xFF) {
                    bRetVal = true;
                }
            }
        }
    }

    sd_spi_cs_set();
    
    return bRetVal;
}

bool spi_sd_erase_blocks(uint32_t block, uint32_t cnt)
{
    bool bRetVal = false;
    uint32_t wR1;
    uint32_t wStart, wEnd;
    uint8_t recv;
    
    if (0 == cnt) {
        return true;
    }
    wStart = block, wEnd = block + cnt - 1;
    
    //! Convert sector-based address to byte-based address for non SDHC.
    if (SD_V2_0_HC_CARD != s_tCardProperties.chCardType) {
        wStart *= 512;
        wEnd   *= 512;
    }
    
    //! The CS signal must be kept low during a transaction.
    sd_spi_cs_clr();
    
    do {
        wR1 = spi_sd_send_cmd(SD_CMD_ERASE_WR_BLK_START_ADDR, wStart, NULL, 0);
        if (0 != wR1) {
            break;
        }
        
        wR1 = spi_sd_send_cmd(SD_CMD_ERASE_WR_BLK_END_ADDR, wEnd, NULL, 0);
        if (0 != wR1) {
            break;
        }
        
        wR1 = spi_sd_send_cmd(SD_CMD_ERASE, wEnd, NULL, 0);
        if (0 != wR1) {
            break;
        }
        
        //! Wait for wirte complete.
        SD_TIME_SET(1000);
        do {
            recv = sd_spi_read_byte();
            if (recv == 0xFF) {
                break;
            }
        } while (!SD_TIME_IS_OVERFLOW());
        if (recv == 0xFF) {
            bRetVal = true;
        }
    } while (0);
    
    sd_spi_cs_set();
    
    return bRetVal;
}



		   
/* END OF FILE */
