#include "spi.h"
//指令集
#define W25Q64_WRITE_ENABLE							0x06
#define W25Q64_WRITE_DISABLE						0x04
#define W25Q64_READ_STATUS_REGISTER_1				0x05
#define W25Q64_READ_STATUS_REGISTER_2				0x35
#define W25Q64_WRITE_STATUS_REGISTER				0x01
#define W25Q64_PAGE_PROGRAM							0x02
#define W25Q64_QUAD_PAGE_PROGRAM					0x32
#define W25Q64_BLOCK_ERASE_64KB						0xD8
#define W25Q64_BLOCK_ERASE_32KB						0x52
#define W25Q64_SECTOR_ERASE_4KB						0x20
#define W25Q64_CHIP_ERASE							0xC7
#define W25Q64_ERASE_SUSPEND						0x75
#define W25Q64_ERASE_RESUME							0x7A
#define W25Q64_POWER_DOWN							0xB9
#define W25Q64_HIGH_PERFORMANCE_MODE				0xA3
#define W25Q64_CONTINUOUS_READ_MODE_RESET			0xFF
#define W25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID		0xAB
#define W25Q64_MANUFACTURER_DEVICE_ID				0x90
#define W25Q64_READ_UNIQUE_ID						0x4B
#define W25Q64_JEDEC_ID								0x9F
#define W25Q64_READ_DATA							0x03
#define W25Q64_FAST_READ							0x0B
#define W25Q64_FAST_READ_DUAL_OUTPUT				0x3B
#define W25Q64_FAST_READ_DUAL_IO					0xBB
#define W25Q64_FAST_READ_QUAD_OUTPUT				0x6B
#define W25Q64_FAST_READ_QUAD_IO					0xEB
#define W25Q64_OCTAL_WORD_READ_QUAD_IO				0xE3
//交换过去的无用数据
#define W25Q64_DUMMY_BYTE							0xFF

void W25Q64_init(void)
{
    soft_spi_init();
}

/*

*/
void W25Q64_read_id(uint8_t* mid,uint16_t * did)
{
    soft_spi_start();
    soft_spi_swap_byte(W25Q64_JEDEC_ID);//读id指令，主机发送
    *mid = soft_spi_swap_byte(W25Q64_DUMMY_BYTE);//发送ff(无意义)，接收厂商id
    *did = soft_spi_swap_word(W25Q64_DUMMY_BYTE);//发送ff(无意义),接收设备id高8位,放在did低八位
    *did <<= 8;//将数据放到高八位
    *did |= soft_spi_swap_byte(W25Q64_DUMMY_BYTE);//发送ff(无意义),接收设备id低8位
    soft_spi_end();

}

void W25Q64_write_enable(void)
{
    soft_spi_start();
    soft_spi_swap_byte(W25Q64_WRITE_ENABLE);
    soft_spi_end();
}

void W25Q64_waitbusy(void)
{
    uint32_t timeout = 10000;
    soft_spi_start();
    soft_spi_swap_byte(W25Q64_READ_STATUS_REGISTER_1);
    while((soft_spi_swap_byte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)//如果busy为1，则等待
    {
        timeout--;
        if (timeout == 0)
        {
            break;
        }
    }
    soft_spi_end();

}


/*
    @brief 写入数据
    @param
        addr：写入的地址
        data:写入的数据
        count:数据个数
    @return

*/
void W25Q64_page_write(uint32_t addr,uint8_t* data,uint16_t count)
{
    uint16_t i;
    W25Q64_write_enable();
    soft_spi_start();
    soft_spi_swap_byte(W25Q64_PAGE_PROGRAM);
    soft_spi_swap_byte(addr >> 16);
    soft_spi_swap_byte(addr >> 8);
    soft_spi_swap_byte(addr);

    for(i = 0;i < count;i++)
    {
        soft_spi_swap_byte(data[i]);
    }
    soft_spi_end();
    W25Q64_waitbusy();
}


void W25Q64_sector_erase(uint32_t addr)
{
    W25Q64_write_enable();
    soft_spi_start();
    soft_spi_swap_byte(W25Q64_SECTOR_ERASE_4KB);
    soft_spi_swap_byte(addr >> 16);
    soft_spi_swap_byte(addr >> 8);
    soft_spi_swap_byte(addr);
    soft_spi_end();
    W25Q64_waitbusy();
}


void W25Q64_read_data(uint32_t addr,uint8_t* data,uint32_t count)
{
    uint8_t i;
    soft_spi_start();
    soft_spi_swap_byte(W25Q64_READ_DATA);
    soft_spi_swap_byte(addr >> 16);
    soft_spi_swap_byte(addr >> 8); 
    soft_spi_swap_byte(addr);
    for(i = 0;i < count;i++)
    {
        data[i] = soft_spi_swap_byte(W25Q64_DUMMY_BYTE);
    }

    soft_spi_end();
}



