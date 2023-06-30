#ifndef __MPU6050_H
#define __MPU6050_H

void mpu6050_init(void);
uint8_t mpu6050_read_byte(uint8_t regaddr);
void mpu6050_write_byte(uint8_t regaddr,uint8_t data);
uint8_t mpu6050_get_id(void);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);


#endif