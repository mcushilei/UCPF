

#include "./app_cfg.h"
#include "./at_port.h"
#include "./at_helper.h"



//uint32_t list_mux;
static char  buff_full = 0;

static uint32_t bufferSize = 0;
static uint32_t wi = 0;
static uint32_t ri = 0;
static uint32_t si = 0;



bool at_usart_init(void)
{
    at_config *at_user_conf = at_get_config();

    bufferSize = at_user_conf->user_buf_len;
    
    PM.Clock.Peripheral.Config(PCLK_UART1, PCLK_DIV_4);
    PM.Clock.Peripheral.Get(PCLK_UART1);
    PM.Power.Enable(PCONP_UART1);

    PIN_CFG(
        {2, 0, DRIVER_PIN_FUNC(2), DRIVER_PIN_MODE_PULL_UP},
        {2, 1, DRIVER_PIN_FUNC(2), DRIVER_PIN_MODE_PULL_UP},
    );
    
    
    static const uart_cfg_t uartCfg = {
        .Baudrate = 115200,
        .DataBits = DRIVER_UART_8_BITS_LENGTH,
        .StopBits = DRIVER_UART_1_STOPBITS,
        .Parity   = DRIVER_UART_NO_PARITY,
    };
    UART[1].Config(&uartCfg);
    UART[1].Enable();
    
    //! enable interrupt
    UART1_REG.IER = UART_IER_RBRIE_MSK;// | UART_IER_RXIE_MSK
    NVIC_EnableIRQ(UART1_IRQn);

    return true;
}

void at_usart_deinit(void)
{
    /* USART1 interrupt Deinit */
    NVIC_DisableIRQ(UART1_IRQn);
    
    UART[1].Disable();
    PM.Power.Disable(PCONP_UART1);
}


void at_uart_send(const char *data, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++) {
        while (false == UART[1].WriteByte(data[i]));
    }
}

void at_transmit(const char *cmd, int32_t len, bool with_line_end)
{
    at_config *at_user_conf = at_get_config();
    
    const char *line_end = at_user_conf->line_end;
    at_uart_send(cmd, len);
    if (with_line_end) {
        at_uart_send(line_end, strlen(at_user_conf->line_end));
    }
}

ISR(UART1_IRQHandler)
{
    osIntEnter();
    
    uint32_t reg;
    
    reg = (UART1_REG.IIR >> 1) & 0x07u;
        
    switch (reg) {                                          
        case 0x02:
        {
            uint8_t  byte;
            while (UART[1].ReadByte(&byte)) {
                if ((wi + 1) == ri) {
                    buff_full = 1;
                } else {
                    at.recv_buf[wi++] = byte;
                    
                    if (wi >= bufferSize) {
                        wi = 0;
                    }
                }
            }
        }
        break;
        
        case 0x06:
        {
            recv_buff recv_buf;
            uint8_t  byte;
            
            while (UART[1].ReadByte(&byte)) {
                if ((wi + 1) == ri) {
                    buff_full = 1;
                } else {
                    at.recv_buf[wi++] = byte;
                    
                    if (wi >= bufferSize) {
                        wi = 0;
                    }
                }
            }

            /* send msg to the AT task to indicate that we have data in buffer. */
            recv_buf.ori = si;
            recv_buf.end = wi;
            recv_buf.msg_type = AT_USART_RX;
            if(OS_QUEUE_WRITE(at.RecvQueue, &recv_buf, 0) == OS_ERR_NONE) {
                si = wi;
            }
        }
        break;
            
        case 0x01:
            break;
            
        case 0x03:
        {
            reg = UART1_REG.LSR;
            uint8_t trashcan;
            
            if (reg & UART_LSR_BI_MSK) {

            } else if (reg & (UART_LSR_OE_MSK | UART_LSR_PE_MSK | UART_LSR_FE_MSK | UART_LSR_RXFE_MSK)) {
            }
        }
        break;
            
        default:
            break;
    }
    
    osIntExit();
}

/* \brief   copy data from recv_buf which has been sent in queue from at_uart_irq_handler()
 */
int at_read_resp(char *buf, uint32_t ori, uint32_t end)
{
    uint32_t len = 0;
    uint32_t tmp_len = 0;

    if (NULL == buf) {
        return -1;
    }
    
    if (0 != buff_full) {
        AT_LOG("buff_full");
    }
    
    NVIC_DisableIRQ(UART1_IRQn);

    if (end == ori) {
        goto END;
    }

    if (end > ori) {
        len = end - ori;
        memcpy(buf, &at.recv_buf[ori], len);
    } else {
        tmp_len = bufferSize - ori;
        memcpy(buf,             &at.recv_buf[ori], tmp_len);
        memcpy(buf + tmp_len,   at.recv_buf,                 end);
        len = end + tmp_len;
    }

    ri = end;
    
END:
    NVIC_EnableIRQ(UART1_IRQn);
    
    return len;
}



uint32_t at_get_time(void)
{
    return (osGetSysTickCount() * OS_MS_PER_TICK)  / 1000;
}


void *at_malloc(size_t size)
{
    void *pMem = osHeapMalloc(size);
    if(NULL != pMem)
    {
        memset(pMem, 0, size);
    }
    return pMem;
}

void at_free(void *ptr)
{
    osHeapFree(ptr);
}
