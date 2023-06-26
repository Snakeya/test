#include "stm32f10x.h"
#include "Serial.h"

/*
    C8T6 adc资源：adc1,adc2,10个外部信号通道

    分辨率：12位，0~4095
    转换时间：1us,如果信号频率大于1mhz,可能会有问题
    输入电压范围：0~3.3v
    18个输入通道，可测量16个外部+2个内部信号（内部温度传感器、内部参考电压1.2v），根据型号不同
*/

void ad_init(void)
{
    //rcc开启adc和gpio的时钟，配置adcclk的分频器
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//adc1挂载总线apb2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); //6分频 72/6 = 12mhz
    //配置gpio为模拟输入
    GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA,&GPIO_InitStructure);
    //。。
    ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);//转定义
    //配置adc结构体
    ADC_InitTypeDef ADC_InitStruct;//转定义
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//单次转换，非扫描
    //ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//连续转换，非扫描
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1,&ADC_InitStruct);
    //cdc_cmd
    ADC_Cmd(ADC1,ENABLE);
    //adc校准
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1) == SET);
    //ADC_SoftwareStartConvCmd(ADC1,ENABLE);//连续转换，非扫描模式时，只需要初始化时调用一次
}

/*
    单次转换，非扫描
    返回值：0~4095
*/
uint16_t ad_get_val(void)
{
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
    printf("ad val is %d\r\n",ADC_GetConversionValue(ADC1));
    return ADC_GetConversionValue(ADC1);
}

void ad_test_1(void)
{
    while(1)
    {
        ad_get_val();
    }
    
}

/*
    连续转换，非扫描
    adc会连续不断对指定通道转换，并将结果存放在数据寄存器
*/
uint16_t ad_get_val_2(void)
{
    return ADC_GetConversionValue(ADC1);
}

