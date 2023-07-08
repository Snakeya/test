#include "stm32f10x.h"


/*
    SPI:serial peripheral interface
    四根通信线：
        sck
        mosi:master output slave input 
        miso:master input slave output
        ss:slave select,从机选择,ss数量 == 从机数量，选择哪个从机通信，就将其对应的ss线拉低
            无需寻址，从机未被选时，从机miso必须切换为高阻态（避免多个从机导致冲突）（主机无需关心）
    引脚配置：
        输入：浮空 or 上拉
        输出：推挽（高低电平均有很强的驱动能力）
    同步，全双工
    支持总线挂载多设备


    
*/

/*
    时序基本单元：根据cpol，cpha配置为四种时序（模式）
    模式1：
        cpol = 0:空闲时，sck为低电平
        cpha = 1:sck的第一个边沿移出数据，第二个边沿移入数据
        移出 + 移入 == 一个数据位传输完成
    模式0：ppt150(应用最多)
        cpol = 0:空闲时，sck为低电平
        cpha = 0:sck的第一个边沿移入数据，第二个边沿移出数据
        移出 + 移入 == 一个数据位传输完成

    iic中，有效数据流第一个字节为寄存器地址     + 数据。。。。
    spi中，指令码 + 数据。。。
        指令码：芯片手册里定义好

    以模式0为例：
        空闲：sck为低电平，ss高电平
        start:ss从高到低
        end:ss从低到高

*/

#define SPI_PORT GPIOA
#define SPI_SS GPIOP_Pin_4
#define SPI_SCK GPIOP_Pin_5
#define SPI_MOSI GPIOP_Pin_7
#define SPI_MISO GPIO_Pin_6


void soft_spi_write_ss(uint8_t bitvalue)
{
    GPIO_WriteBit(SPI_PORT,SPI_SS,(BitAction)bitvalue);
}

void soft_spi_write_sck(uint8_t bitvalue)
{
    GPIO_WriteBit(SPI_PORT,SPI_SCK,(BitAction)bitvalue);
}

void soft_spi_write_mosi(uint8_t bitvalue)
{
    GPIO_WriteBit(SPI_PORT,SPI_MOSI,(BitAction)bitvalue);
}

uint8_t soft_spi_read_miso()
{
    return GPIO_ReadInputDataBit(SPI_PORT,SPI_MISO);
}

void soft_spi_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SPI_SS|SPI_SCK|SPI_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI_MISO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(SPI_PORT,&GPIO_InitStructure);
    
    soft_spi_write_ss(1);
    soft_spi_write_sck(0);
}

void soft_spi_start(void)
{
    soft_spi_write_ss(0);

}

void soft_spi_end(void)
{
    soft_spi_write_ss(1);
}

/*
    模式0
*/
uint8_t soft_spi_swap_byte(uint8_t sendbyte)
{
    uint8_t recv_byte;
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        soft_spi_write_mosi(sendbyte &(0x80 << i));
        soft_spi_write_sck(1);
        if(soft_spi_read_miso())
        {
            recv_byte |= (0x80 >> i);
        }
        soft_spi_write_sck(0);
    }


    return recv_byte;

}

