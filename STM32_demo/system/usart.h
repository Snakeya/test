#ifndef __USART_H
#define __USART_H

extern uint8_t usart2_recv_data_packet[4];
extern uint8_t usart2_send_data_packet[4];

enum DAY
{
    USART2_RECV_HEADER=0,
    USART2_RECV_DATA,
    USART2_RECV_END
};

void usart1_init(void);
void usart1_send_bytes(uint8_t byte);
void USART1_IRQHandler(void);
void usart1_SendArray(uint8_t *Array, uint16_t Length);
uint8_t usart1_get_recvdata(void);
uint8_t usart1_get_recvflag(void);


////////////////////////////////////////////////////////////////////////

void usart2_init(uint32_t bundrate);
void usart2_send_bytes(uint8_t byte);
void usart2_SendArray(uint8_t *Array, uint16_t Length);
void USART2_IRQHandler(void);
uint8_t usart2_get_recvdata(void);



#endif
