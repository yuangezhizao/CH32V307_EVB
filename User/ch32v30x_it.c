/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v30x_it.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main Interrupt Service Routines.
 *******************************************************************************/
#include "ch32v30x_it.h"
#include "tos_k.h"
#include "tos_at.h"
extern at_agent_t esp8266_tf_agent;
// extern at_agent_t esp8266_agent;

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART7_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
// void EXTI8_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*******************************************************************************
 * Function Name  : NMI_Handler
 * Description    : This function handles NMI exception.
 * Input          : None
 * Return         : None
 *******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
 * Function Name  : HardFault_Handler
 * Description    : This function handles Hard Fault exception.
 * Input          : None
 * Return         : None
 *******************************************************************************/
void HardFault_Handler(void)
{
  printf("hardfault\r\n");
  printf("mepc   = %08x\r\n", __get_MEPC());
  printf("mcause = %08x\r\n", __get_MCAUSE());
  printf("mtval  = %08x\r\n", __get_MTVAL());
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      USART2_IRQHandler
 *
 * @brief   This function handles USART2 global interrupt request.
 *
 * @return  none
 */
void USART2_IRQHandler(void)
{
  uint8_t data;
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    data = USART_ReceiveData(USART2);
    tos_at_uart_input_byte(&esp8266_tf_agent, data);
  }
}

/*********************************************************************
 * @fn      USART2_IRQHandler
 *
 * @brief   This function handles USART2 global interrupt request.
 *
 * @return  none
 */
void UART6_IRQHandler(void)
{
  uint8_t data;
  if (USART_GetITStatus(UART6, USART_IT_RXNE) != RESET)
  {
    data = USART_ReceiveData(UART6);
    tos_at_uart_input_byte(&esp8266_tf_agent, data);
  }
}
/*********************************************************************
 * @fn      USART2_IRQHandler
 *
 * @brief   This function handles USART2 global interrupt request.
 *
 * @return  none
 */
void UART7_IRQHandler(void)
{
  uint8_t data;
  if (USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)
  {
    data = USART_ReceiveData(UART7);
    tos_at_uart_input_byte(&esp8266_tf_agent, data);
  }
}

/*********************************************************************
 * @fn      EXTI0_IRQHandler
 *
 * @brief   This function handles EXTI0 Handler.
 *
 * @return  none
 */

volatile uint8_t User_LED_1_Status = 0;
volatile uint8_t User_LED_2_Status = 0;
// volatile uint8_t User_LED_4_Status = 0;
volatile uint8_t sunset_light_status;

void EXTI0_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
#if 0
    printf("Run at EXTI\r\n");

#endif
    EXTI_ClearITPendingBit(EXTI_Line0); /* Clear Flag */
    User_LED_2_Status = !User_LED_2_Status;
    GPIO_WriteBit(GPIOE, GPIO_Pin_3, User_LED_2_Status);
    if (User_LED_2_Status == 0)
    {
      turn_on_bedroom_light();
    }
    else
    {
      turn_off_bedroom_light();
    }
  }
}

#define OPEN_SUNSET_LIGHT GPIO_SetBits(GPIOC, GPIO_Pin_10)
#define CLOSE_SUNSET_LIGHT GPIO_ResetBits(GPIOC, GPIO_Pin_10)
#define SUNSET_LIGHT_IS_OPEN GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_10)

void EXTI1_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
#if 0
    printf("Run at EXTI\r\n");

#endif
    EXTI_ClearITPendingBit(EXTI_Line1); /* Clear Flag */
    if (SUNSET_LIGHT_IS_OPEN)
    {
      CLOSE_SUNSET_LIGHT;
    }
    else
    {
      OPEN_SUNSET_LIGHT;
    }
    GPIO_WriteBit(GPIOE, GPIO_Pin_4, !SUNSET_LIGHT_IS_OPEN);
  }
}

// void EXTI8_IRQHandler(void)
//{
//   if (EXTI_GetITStatus(EXTI_Line8) != RESET)
//   {
//#if 0
//     printf("Run at EXTI\r\n");
//
//#endif
//     EXTI_ClearITPendingBit(EXTI_Line8); /* Clear Flag */
//     User_LED_4_Status = !User_LED_4_Status;
//     GPIO_WriteBit(GPIOE, GPIO_Pin_5, User_LED_4_Status);
//   }
// }

void TIM6_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM6, TIM_FLAG_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update); /* Clear Flag */
    User_LED_1_Status = !User_LED_1_Status;
    GPIO_WriteBit(GPIOE, GPIO_Pin_2, User_LED_1_Status);
  }
}
