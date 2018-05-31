/**
  ******************************************************************************
  * @file    SysTick/SysTick_Example/main.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup SysTick_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay;
__IO uint32_t counting = 0;
__IO uint8_t scan_step = 0;
uint8_t scan_flag = 0;

EXTI_InitTypeDef   EXTI_InitStructure;
GPIO_InitTypeDef   GPIO_InitStructure;
NVIC_InitTypeDef   NVIC_InitStructure;

/* Private function prototypes -----------------------------------------------*/
static void USART_Config(void);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	
static void Delay(__IO uint32_t nTime);
static void EXTI4_15_Config(void);
static void scan8(void);
static void scan6(void);
static void scan5(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
__IO uint8_t exti_flag = 0;//record which pin has interrupt
	
int main(void)
{
	
	/* USART configuration */
  USART_Config();
	while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}
    
	if (SysTick_Config(SystemCoreClock / 1000))
  {
    /* Capture error */ 
    while (1);
  }
	    /* Enable GPIO */ 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIOA->BSRR = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_12;
	
  EXTI4_15_Config();
  
	
/*------------------------------------------------------------------------------------------*/  
	while (1)
  {
    switch(exti_flag)
		{
      case 8:
        //exti_flag = 0;
        Delay(5);
        scan8();
        Delay(5);      
        if(scan_flag == 10){
          printf("star");}
        else{
          printf("%d",scan_flag);}
        Delay(10);
        exti_flag = 0;
      break;
      
      case 6:
        Delay(5);
        scan6();
        Delay(5);      
        printf("%d",scan_flag);
        Delay(10);
        exti_flag = 0;
      break;
      
      case 5:
        Delay(5);
        scan5();
        Delay(5);      
        if(scan_flag == 11){
          printf("number");}
        else{
         printf("%d",scan_flag);}
        Delay(10);
        exti_flag = 0;
      break;
      
      default:
      break;
	}
}
}
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */

static void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None  
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{	
	while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

static void EXTI4_15_Config(void)
{
	 /* Enable GPIOC clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure PC8 and PC13 pins as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
   /* Connect EXTI8 Line to PC8 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6);
  
  /* Configure EXTI8 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line8|EXTI_Line5|EXTI_Line6;  
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//DISABLE
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set EXTI4_15 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

static void USART_Config(void)
{ 
  USART_InitTypeDef USART_InitStructure;
  
  /* USARTx configured as follow:
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - Stop Bit = 1 Stop Bit
  - Parity = No Parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(EVAL_COM1_RX_GPIO_CLK, ENABLE);

  /* Enable USART clock */
  RCC_APB1PeriphClockCmd(EVAL_COM1_CLK, ENABLE); 

  /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(EVAL_COM1_TX_GPIO_PORT, EVAL_COM1_TX_SOURCE, EVAL_COM1_TX_AF);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(EVAL_COM1_RX_GPIO_PORT, EVAL_COM1_RX_SOURCE, EVAL_COM1_RX_AF);
  
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = EVAL_COM1_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(EVAL_COM1_TX_GPIO_PORT, &GPIO_InitStructure);
    
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = EVAL_COM1_RX_PIN;
  GPIO_Init(EVAL_COM1_TX_GPIO_PORT, &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(EVAL_COM1, &USART_InitStructure);
    
  /* Enable USART */
  USART_Cmd(EVAL_COM1, ENABLE);
	
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET)
  {}

  return ch;
}

static void scan8(void)
{ 
  while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) != RESET){
    if((counting % 20) == 0)
      scan_step = 0;
    else if((counting % 20) == 5)
	    scan_step = 1;
    else if((counting % 20) == 10)
	    scan_step = 2;
    else if((counting % 20) == 15)
	    scan_step = 3;

    switch(scan_step)
    {
      case 0:
        GPIO_WriteBit(GPIOA, GPIO_Pin_6 | GPIO_Pin_7, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_SET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) != RESET)
          scan_flag = 1;
        break;
      case 1:
        GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
        GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_SET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) != RESET)
          scan_flag = 4;
        break;
      case 2:
        GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
        GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) != RESET)
          scan_flag = 7;
        break;
      case 3:
        GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
        GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) != RESET)
          scan_flag = 10;
        break;
      default:
        break;
    }
  GPIOA->BSRR = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_12;
  }
}

static void scan6(void)
{   
  while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) != RESET){
    if((counting % 20) == 0)
      scan_step = 0;
    else if((counting % 20) == 5)
	    scan_step = 1;
    else if((counting % 20) == 10)
	    scan_step = 2;
    else if((counting % 20) == 15)
	    scan_step = 3;

    switch(scan_step)
    {
    
      case 0:
        GPIO_WriteBit(GPIOA, GPIO_Pin_6 | GPIO_Pin_7, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_SET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) != RESET)
          scan_flag = 2;
        break;
      case 1:
        GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
        GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_SET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) != RESET)
          scan_flag = 5;
        break;
      case 2:
        GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
        GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) != RESET)
          scan_flag = 8;
        break;
      case 3:
        GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
        GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) != RESET)
          scan_flag = 0;
        break;
      default:
        break;
    }
  GPIOA->BSRR = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_12;
  }
}

static void scan5(void)
{ 
  while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != RESET){
    if((counting % 20) == 0)
      scan_step = 0;
    else if((counting % 20) == 5)
	    scan_step = 1;
    else if((counting % 20) == 10)
	    scan_step = 2;
    else if((counting % 20) == 15)
	    scan_step = 3;

    switch(scan_step)
    {
    
      case 0:
        GPIO_WriteBit(GPIOA, GPIO_Pin_6 | GPIO_Pin_7, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_SET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != RESET)
          scan_flag = 3;
        break;
      case 1:
        GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
        GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_SET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != RESET)
          scan_flag = 6;
        break;
      case 2:
        GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
        GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != RESET)
          scan_flag = 9;
        break;
      case 3:
        GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
        GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
        GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);
        scan_step = 5;
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != RESET)
          scan_flag = 11;
        break;
      default:
        break;
    }
  GPIOA->BSRR = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_12;
  }
}


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
