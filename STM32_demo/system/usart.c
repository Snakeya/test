#include "stm32f10x.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
/*
    USART:全双工、异步（双方需要约定相同的频率才能正常通信，波特率），电平:单端（通信双方需要共地）
    参数：
        波特率
        起始位：1位，固定位低电平，通信开始
        数据位：8位，低位先行，如：发送0f:00001111,则发送顺序为11110000 tips:发送时数据右移
        校验位：1位，用于验证数据的正确性，
            奇校验：再数据根据发送的数据，在数据位后补0 or 1，确保数据位 + 校验位中1的个数为奇数，如果接受时，1的个数部位奇数，则数据出错
                如果传输中两个数据出错，则无法验证数据准确性，了解CRC校验，STM32内部有CRC外设
            偶校验：同上
        停止位：1位，固定高电平，通信结束，并为下一次通信准备
    
    c8t6:
        USART1 apb2 pclk2时钟，72m
        USART2 USART3 apb1 pclk1时钟，36m
        支持dma

*/

uint8_t usart1_recv_data;
uint8_t usart2_recv_data;
uint8_t usart3_recv_data;

uint8_t usart1_recv_flag;
uint8_t usart2_recv_flag;
uint8_t usart3_recv_flag;

uint8_t usart2_recv_data_packet[4];
uint8_t usart2_send_data_packet[4];

#define USART2_PACKET_HEADER 0xFF
#define USART2_PACKET_END 0xEF
#define USART2_PACKET_DATASIZE 4//字节

void usart1_init(void)
{
    /* 使能USART1时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    /* 使能GPIOA时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /* 配置USART1 GPIO */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//接收引脚配置为浮空 or 上拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    /* 配置USART1 AF */
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    
    /* 配置USART1 */
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位字长
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//USART_StopBits
    USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //配置USART1串口的接收中断使能。

   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
    
}


void usart1_send_bytes(uint8_t byte)
{
    USART_SendData(USART1, byte);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_ClearFlag(USART1, USART_FLAG_TC);

}


void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        usart1_recv_data = USART_ReceiveData(USART1);
       usart1_recv_flag = 1;
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }

}


void usart1_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		usart1_send_bytes(Array[i]);
	}
}
/*
    获取串口1收到的数据
*/
uint8_t usart1_get_recvdata(void)
{
    return usart1_recv_data;
}

/*
    获取标志位usart1_recv_flag
    usart1_recv_flag在USR1_IRQHandler里改变
*/
uint8_t usart1_get_recvflag(void)
{
    if(usart1_recv_flag == 1)
    {
        usart1_recv_flag = 0;
        return 1;
    }
    return 0;
}

/*
	使用printf
	1.重定向fputc函数
	2.勾选use microlib
	3.头文件#include <stdio.h>
*/
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*

*/
void usart2_init(uint32_t bundrate)
{
    /* 使能USART2时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    /* 使能GPIOA时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /* 配置USART2 GPIO */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//接收引脚配置为浮空 or 上拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    /* 配置USART2 AF */
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
    
    /* 配置USART2 */
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = bundrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位字长
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//USART_StopBits
    USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void usart2_send_bytes(uint8_t byte)
{
    USART_SendData(USART2, byte);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART_ClearFlag(USART2, USART_FLAG_TC);

}


void usart2_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		usart2_send_bytes(Array[i]);
	}
}

void USART2_IRQHandler(void)
{
    static uint8_t s_usart2_rx_state = USART2_RECV_HEADER;
    static uint8_t s_pack_num = 0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        usart2_recv_data = USART_ReceiveData(USART2);
        switch (s_usart2_rx_state)
        {
        case USART2_RECV_HEADER:
            if(usart2_recv_data == USART2_PACKET_HEADER)
                s_usart2_rx_state = USART2_RECV_DATA;
            break;
        case USART2_RECV_DATA:
            usart2_recv_data_packet[s_pack_num++] = usart2_recv_data;
            if(s_pack_num >= USART2_PACKET_DATASIZE)
            {
                s_usart2_rx_state = USART2_RECV_END;
                s_pack_num = 0;
            }
            break;
        case USART2_RECV_END:
            if(usart2_recv_data == USART2_PACKET_END)
            {
                s_usart2_rx_state = USART2_RECV_HEADER;
                usart2_recv_flag = 1;
            }

        default:
            break;
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }

}

uint8_t usart2_get_recvdata(void)
{
    return usart2_recv_data;
}