#ifndef __PWM_H
#define __PWM_H

void pwminit(void);
void pwm_led(void);
void pwm_servo_init(void);
void pwm_servo_test(void);
void Servo_SetAngle(float Angle);

void input_capture_test1(void);

#endif