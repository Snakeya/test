#include "stm32f10x.h"
#include "Delay.h"

/*
    写配置寄存器，读数据寄存器
*/

/*
    两根通信线：
        时钟，提供同步时序，从机只能读取时钟线
        数据，一根，同一时刻只能收或发 ==》半双工，从机只能在主机发送读取从机的命令后（或从机应答时），才能短暂控制数据线
    同步时序，半双工
    带数据应答
    支持总线挂载多设备（一主多从，多主多从）
*/

/*
    硬件电路：ppt128
        scl和sda均配置为开漏输出,并各加一个上拉电阻（4.7k）,总线空闲时，scl和sda由上拉电阻拉为高电平
*/

/*
    时序基本单元：
        start:scl高电平期间，sda从高电平切换到低电平
        

        发送一个字节：
            scl低电平期间，主机将数据位依次放到sda上（高位先行）
            释放scl,scl由低到高，从机在scl高电平期间读取sda电平（scl高电平期间，sda电平不允许变化）
            循环8次
        接收一个字节：
            scl低电平期间，从机将数据位依次放到sda上（高位先行）
            释放scl,scl由低到高，主机在scl高电平期间读取sda电平（scl高电平期间，sda电平不允许变化）
            循环8次
            主机接收前，需要释放sda

        发送一个应答：时序同发送一个字节
            主机在接收完一个字节后，在下一个时钟发送 一位数据，0表示应答，1表示非应答
        接收一个应答：时序同接收一个字节
            从机在发送完一个字节后,在下一个时钟接收 一位数据,判断从机是否应答，0表示应答,1表示非应答
            主机接收前，需要释放sda

        end:scl高电平期间，sda从低电平切换到高电平
            开始前和结束后，即空闲时，scl和sda由上拉电阻拉为高电平

*/

/*
    指定地址写：
        start
        确定通信设备
            主机发送一个 地址：出厂时厂商分配的7位（最多）or 十位地址，参考芯片手册，不同型号芯片地址不同
            如果总线挂载相同芯片
                可变地址：地址最后几位可以通过电路改变
        发送从机地址：7位
        发送读写位：1位 0
            0：写入
            1：读取
        接收一个应答位：
        发送从机寄存器地址：
        接收应答：
        发送写入的数据：
        end

    当前地址读：
        用的不多

    指定地址读：
        指定地址：调用指定地址写的前部分时序，获取读的地址
            start
            确定通信设备
                主机发送一个 地址：出厂时厂商分配的7位（最多）or 十位地址，参考芯片手册，不同型号芯片地址不同
                如果总线挂载相同芯片
                    可变地址：地址最后几位可以通过电路改变
            发送从机地址：7位
            发送读写位：1位 0
                0：写入
                1：读取
            接收一个应答位：
            发送从机寄存器地址：
            接收应答：
        当前地址读：
            start
            发送从机地址
            发送读写位
            主机接收一个字节 
            end
            
*/
void soft_iic_write_scl(uint8_t bitvalue)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)bitvalue);
    Delay_us(10); 
}

void soft_iic_write_sda(uint8_t bitvalue)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)bitvalue);
    Delay_us(10);
}

uint8_t soft_iic_read_sda(void)
{
    uint8_t bit_value;
    bit_value = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
    Delay_us(10);
    return bit_value;
}

void soft_iic_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);//拉高

}

void soft_iic_start(void)
{
    soft_iic_write_scl(1);
    soft_iic_write_sda(1);
    soft_iic_write_sda(0);
    soft_iic_write_scl(0);
}

void soft_iic_stop(void)
{
    soft_iic_write_sda(0);
    soft_iic_write_scl(1);
    soft_iic_write_sda(1);
}

void soft_iic_sendbyte(uint8_t byte)
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
        soft_iic_write_sda((0x80 >> i) & byte);
        soft_iic_write_scl(1);
        soft_iic_write_scl(0);
    } 
}

uint8_t soft_iic_recv_byte(void)
{
    uint8_t i;
    uint8_t byte = 0x00;
    soft_iic_write_sda(1);
    for(i = 0;i < 8;i++)
    {
        soft_iic_write_scl(1);
        if(soft_iic_read_sda())
        {
            byte |= (0x80 >> i);
        }
        soft_iic_write_scl(0);
    }
    return byte;
}

void soft_iic_send_ack(uint8_t ack)
{
    soft_iic_write_sda(ack);
    soft_iic_write_scl(1);
    soft_iic_write_scl(0);
}

uint8_t soft_iic_recv_ack(void)
{
    uint8_t ackbit;
    soft_iic_write_sda(1);
    soft_iic_write_scl(1);
    ackbit = soft_iic_read_sda();
    soft_iic_write_scl(0);
    return ackbit;
}
    


void hardware_iic_init(void)
{

}