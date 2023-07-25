#include "stm32f10x.h"
#include "iic.h"
#include "mpu6050.h"
/*
    mpu6050:
        6轴姿态传感器
            3轴加速度：acc
                静态稳定，动态不稳定
            3轴角速度:gyroscope
                静态不稳定,动态稳定
        参数:
            16位adc采集模拟信号，量化范围：-32768~32768
            acc满量程选择：+-2 +-4 +-8 +-16 单位为g,1g = 9.8m/s^2
            gyroscope满量程选择：+-250 +-500 +-1000 +-2000 单位为 度每秒
                满量程越小，adc分辨率越大
            可配置低通滤波
            可配置时钟源
            可配置采样分频
            地址：1101000 or 1101001 通过ad0引脚配置

         
*/
#define MPU6050_ADDR 0xD0

#define	MPU6050_SMPLRT_DIV		0x19
#define	MPU6050_CONFIG			0x1A
#define	MPU6050_GYRO_CONFIG		0x1B
#define	MPU6050_ACCEL_CONFIG	0x1C

#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B
#define	MPU6050_PWR_MGMT_2		0x6C
#define	MPU6050_WHO_AM_I		0x75



uint8_t mpu6050_read_byte(uint8_t regaddr)
{
    uint8_t data = 0;
    soft_iic_start();
    soft_iic_sendbyte(MPU6050_ADDR);
    soft_iic_recv_ack();
    soft_iic_sendbyte(regaddr);
    soft_iic_recv_ack();

    soft_iic_start();
    soft_iic_sendbyte(MPU6050_ADDR | 0x01);
    soft_iic_recv_ack();
    data = soft_iic_recv_byte();
    soft_iic_send_ack(1);//不应答
    soft_iic_stop();

    return data;

}

void mpu6050_write_byte(uint8_t regaddr,uint8_t data)
{
    soft_iic_start();
    soft_iic_sendbyte(MPU6050_ADDR);
    soft_iic_recv_ack();
    soft_iic_sendbyte(regaddr);
    soft_iic_recv_ack();
    soft_iic_sendbyte(data);
    soft_iic_recv_ack();
    soft_iic_stop();
}
void mpu6050_init(void) 
{
    soft_iic_init();
    mpu6050_write_byte(MPU6050_PWR_MGMT_1, 0x01);
	mpu6050_write_byte(MPU6050_PWR_MGMT_2, 0x00);
	mpu6050_write_byte(MPU6050_SMPLRT_DIV, 0x09);
	mpu6050_write_byte(MPU6050_CONFIG, 0x06);
	mpu6050_write_byte(MPU6050_GYRO_CONFIG, 0x18);
	mpu6050_write_byte(MPU6050_ACCEL_CONFIG, 0x18);
}

/*
    test ok
*/
uint8_t mpu6050_get_id(void)
{
    return mpu6050_read_byte(MPU6050_WHO_AM_I);
}


/*
    test ok 
*/
void MPU6050_GetData(mpu6050_data * data)
{
	uint8_t DataH, DataL;
	
	DataH = mpu6050_read_byte(MPU6050_ACCEL_XOUT_H);
	DataL = mpu6050_read_byte(MPU6050_ACCEL_XOUT_L);
	data->AccX = (DataH << 8) | DataL;
	
	DataH = mpu6050_read_byte(MPU6050_ACCEL_YOUT_H);
	DataL = mpu6050_read_byte(MPU6050_ACCEL_YOUT_L);
	data->AccY = (DataH << 8) | DataL;
	
	DataH = mpu6050_read_byte(MPU6050_ACCEL_ZOUT_H);
	DataL = mpu6050_read_byte(MPU6050_ACCEL_ZOUT_L);
	data->AccZ = (DataH << 8) | DataL;
	
	DataH = mpu6050_read_byte(MPU6050_GYRO_XOUT_H);
	DataL = mpu6050_read_byte(MPU6050_GYRO_XOUT_L);
	data->GyroX = (DataH << 8) | DataL;
	
	DataH = mpu6050_read_byte(MPU6050_GYRO_YOUT_H);
	DataL = mpu6050_read_byte(MPU6050_GYRO_YOUT_L);
	data->GyroY = (DataH << 8) | DataL;
	
	DataH = mpu6050_read_byte(MPU6050_GYRO_ZOUT_H);
	DataL = mpu6050_read_byte(MPU6050_GYRO_ZOUT_L);
	data->GyroZ = (DataH << 8) | DataL;
}

