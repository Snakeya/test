#ifndef __IIC_H
#define __IIC_H

void soft_iic_init(void);
void soft_iic_start(void);
void soft_iic_stop(void);
void soft_iic_sendbyte(uint8_t byte);
uint8_t soft_iic_recv_byte(void);
void soft_iic_send_ack(uint8_t ack);
uint8_t soft_iic_recv_ack(void);




#endif