#include "inc.h"
extern void NVIC_Configuration(IRQn_Type IRQn,uint32_t Preemption,uint32_t Sub);
extern void UART_Configuration(UART_TypeDef* UARTx);
extern uint8_t UART_Read(UART_TypeDef* UARTx);
extern void UART_Send(UART_TypeDef* UARTx,uint8_t ch);
extern void DMA_Config_P2M(void);
extern void DMA_ChannelCmd(void);
extern void LLI_Init(void);
extern void Time_config(uint32_t ms);
extern void ringBufferInit(RingBufferTypeDef * buf);
extern void ringBufferPut(RingBufferTypeDef * buf, uint8_t ch);
extern uint8_t ringBufferGet(RingBufferTypeDef * buf);		//输出
extern void GPIO_Config(void);
void system_set(void);
LLI llilist[MULTIBLOCK_BLOCKCOUNTER];
uint8_t dst_Buf0[MULTIBLOCK_BLOCKSIZE] = {0};
extern uint16_t __kfifo_put(kfifo_t *fifo, uint16_t len);
extern uint16_t __kfifo_get(kfifo_t *fifo, uint8_t *buffer, uint16_t len);


//uint8_t uart1TxBuf[1 << 8];
kfifo_t uart0RxFifo ={	.buffer = dst_Buf0,
						.size = sizeof(dst_Buf0),
						.in = 0,
						.out = 0
					};

uint8_t led=1;


int main(void)
{
    system_set();//设置时钟，使能时钟
    NVIC_Configuration(TIM0_0_IRQn,1,1);
    Time_config(7000);

    GPIO_Config();
    GPIOB->OEN &= (~(1 << 2));
    led = 0;
    while (1)
    {       
       if(led==0)
        GPIOB->BSRR |= 1 << ( 2 + 16 );//低电平
       else
         GPIOB->BSRR |= 1 << 2;        //高电平
    }
    
    return 0;
}


void system_set(void)//门控，软复位，时钟频率设定
{
    SYSCTRL->CG_CTRL1 |= 0X04310003;
    SYSCTRL->SOFT_RST1 |= 0x00300003;
    SYSCTRL->FREQ_SEL |=  0x200F395A;//取PLL时钟192MHZ
}



