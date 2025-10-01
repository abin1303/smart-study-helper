/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "clcd.h"
#include "dht.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define ARR_CNT 10
#define CMD_SIZE 50


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c3;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

volatile int keyNo;

volatile int time3SecFlag = 1;
volatile int time3Cnt = 0;
volatile int time3SecCnt = 0;
volatile int seatFlag = 0;
uint8_t rx2char;
volatile unsigned char rx2Flag = 0;
volatile char rx2Data[50];
volatile unsigned char btFlag = 0;
uint8_t btchar;
char btData[50];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C3_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void bluetooth_Event();
long map(long x, long in_min, long in_max, long out_min, long out_max);
void set_rgb_led(int red, int green, int blue);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

	int buttonstate[7] = {0};
	//int buttonFlag = 0;
	int buttonStateOld[7] = {0};
	int cnt = 0;
	int i = 0;

	char lcd_buffer1[17];
	char lcd_buffer2[17];

	char buff[30];
	int pluse = 500;
	int red = 100;
	int green = 100;
	int blue = 100;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C3_Init();
  MX_USART6_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2, &rx2char,1);
  HAL_UART_Receive_IT(&huart6, &btchar,1);
  DHT11_Init();
  LCD_init(&hi2c3);

  DHT11_TypeDef dht11Data;
  char buff_button[30];
  char buff_dht[30];
  char btn_send_message[50] = {0};

  LCD_init(&hi2c3);
  LCD_writeStringXY(0, 0, "hello lcd");

  if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) //timer3 start
	  Error_Handler();

  if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK)
    {
  	  Error_Handler();
    }
    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,(pluse-1)<0?0:pluse-1);


    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,(red-1)<0?0:red-1);
    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,(green-1)<0?0:green-1);
    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,(blue-1)<0?0:blue-1);
    if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2) != HAL_OK)
    {
  	  Error_Handler();
    }
    if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK)
    {
  	  Error_Handler();
    }
    if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4) != HAL_OK)
    {
  	  Error_Handler();
    }


  printf("main() start!\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(rx2Flag)
	  {
			printf("recv2 : %s\r\n",rx2Data);
			rx2Flag =0;
	//	    HAL_UART_Transmit(&huart6, (uint8_t *)buf, strlen(buf), 0xFFFF);
	  }
	  if(btFlag)
	  {
	  //		printf("bt : %s\r\n",btData);
		  btFlag =0;
		  bluetooth_Event();
	  }
	  if(time3SecFlag) {
#if 0
		  sprintf(buff, "DATE: %s", "2025-06-02"); //16
		   printf("%s\r\n", buff);
		  LCD_writeStringXY(0,0,buff);
		  sprintf(buff, "Seat no:  %s","1"); //11
		   printf("%s\r\n", buff);
		  LCD_writeStringXY(1,0,buff);
		  HAL_Delay(5000);

		  sprintf(buff, " Using  %s", "00:00:00"); //15
		   printf("%s\r\n", buff);
		  LCD_writeStringXY(0,0,buff);
		  sprintf(buff, " Study  %s", "00:00:00"); //15
		  printf("%s\r\n", buff);
		  LCD_writeStringXY(1,0,buff);
#endif
#if 1

		  /*
	     //문자열 치환
	      char send_UsingTime[16];
	      strcpy(send_UsingTime, UsingTime);   //send_UsingTime <- UsingTime 복사
	      char *Using = strstr(send_UsingTime, ":");
	      while (Using != NULL)
	      {
	        Using = strstr(send_UsingTime+1, ":");
	        strncpy(Using, "\-", 1);
	      }
	        Serial.print("send_UsingTime: ");//test
	        Serial.println(send_UsingTime);//test
	        */

	     char Origin[9] = "12-34-56";
	     //char Using[9];
		 //strcpy(Using, Origin);   //send_UsingTime <- UsingTime 복사
		 char *Using = strstr(Origin, "-");
		 while (Using != NULL)
		 {
			 Using = strstr(Origin+1, "-");
			 strncpy(Using, "\:",1);
		 }
		 //printf("Using : %s\r\n", Using);

		 sprintf(buff, " Using  %s", Origin); //15
		  printf("%s\r\n", buff);
		 LCD_writeStringXY(0,0,buff);

//		 sprintf(lcdbuff, " Study  %s", pArray[6]); //15
//		 printf("%s\r\n", lcdbuff);
//		 LCD_writeStringXY(1,0,lcdbuff);
		 HAL_Delay(5000);
#endif

	  	  dht11Data = DHT11_readData();
	  	  sprintf(buff,"h: %d%% t: %d.%d'C", dht11Data.rh_byte1, dht11Data.temp_byte1, dht11Data.temp_byte2);
	  	  sprintf(buff_dht,"[CSB_LIN]humi@%d@temp@%d.%d\r\n", dht11Data.rh_byte1, dht11Data.temp_byte1, dht11Data.temp_byte2);
	  	  //HAL_UART_Transmit(&huart6, (uint8_t *)buff_dht, strlen(buff_dht), 0xFFFF);
	  	  printf("%s\r\n", buff);
	  	  LCD_writeStringXY(0, 0, "LibraryCondition");
	  	  LCD_writeStringXY(1, 0, buff);
	  	  //HAL_Delay(2000);
	  	  time3SecFlag = 0;
	  }
	  if(seatFlag) {
		  sprintf(buff,"[CSB_LIN]REQ@SEAT\r\n");
		  HAL_UART_Transmit(&huart6, (uint8_t *)buff, strlen(buff), 0xFFFF);
		  seatFlag = 0;

	  }
	  if(keyNo != 0)
	  {
		  printf("keyNo : %d\r\n", keyNo);
		  sprintf(buff_button,"[CSB_LIN]BUTTON@%d\n",keyNo);
		  HAL_UART_Transmit(&huart6, (uint8_t *)buff_button, strlen(buff_button), 0xFFFF);
		  keyNo = 0;
	 	  //printf("cnt : %d\r\n", cnt++);
	 	    	//}
	 	  buttonStateOld[i] = buttonstate[i];
	 	  	  //}

	 	  }



	  HAL_Delay(2000);


#if 0
	  dht11Data = DHT11_readData();
	  sprintf(buff,"h: %d%% t: %d.%d'C", dht11Data.rh_byte1, dht11Data.temp_byte1, dht11Data.temp_byte2);
	  printf("%s\r\n", buff);
	  LCD_writeStringXY(1, 0, buff);
	  HAL_Delay(2000);
#endif

	  //set_rgb_led(red, green, blue);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 10000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 84-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED6_Pin|LED7_Pin|LED0_Pin|LED1_Pin
                          |LED2_Pin|LED3_Pin|LED4_Pin|LED5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN0_Pin BTN1_Pin BTN2_Pin BTN3_Pin
                           BTN4_Pin BTN5_Pin BTN6_Pin BTN7_Pin */
  GPIO_InitStruct.Pin = BTN0_Pin|BTN1_Pin|BTN2_Pin|BTN3_Pin
                          |BTN4_Pin|BTN5_Pin|BTN6_Pin|BTN7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED6_Pin LED7_Pin LED0_Pin LED1_Pin
                           LED2_Pin LED3_Pin LED4_Pin LED5_Pin */
  GPIO_InitStruct.Pin = LED6_Pin|LED7_Pin|LED0_Pin|LED1_Pin
                          |LED2_Pin|LED3_Pin|LED4_Pin|LED5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : DHT11_Pin */
  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void bluetooth_Event()
{

  int i=0;
  int Red,Green,Blue,seat_num = 0;
  char lcdbuff[17];
  int seat_no = 0;

  char * pToken;
  char * pArray[ARR_CNT]={0};
  char recvBuf[CMD_SIZE]={0};
  char sendBuf[CMD_SIZE]={0};
  strcpy(recvBuf,btData);

  printf("btData : %s\r\n",btData);

  pToken = strtok(recvBuf,"[@]");
  while(pToken != NULL)
  {
    pArray[i] =  pToken;
    if(++i >= ARR_CNT)
      break;
    pToken = strtok(NULL,"[@]");
  }
//  printf("pArray[0] : %s\r\n",pArray[0]);
//  printf("pArray[1] : %s\r\n",pArray[1]);
//  printf("pArray[2] : %s\r\n",pArray[2]);
  if(!strcmp(pArray[1],"LED"))
  {
		if(!strcmp(pArray[2],"ON"))
		{
			//MX_GPIO_LED_ON(LD2_Pin);
		}
		else if(!strcmp(pArray[2],"OFF"))
		{
			//MX_GPIO_LED_OFF(LD2_Pin);
		}
  }
  else if(!strcmp(pArray[1],"LAMP"))
  {
		if(!strcmp(pArray[2],"ON"))
		{
			///MX_GPIO_TEST_LED_ON(TEST_LED_Pin);	//PC12 High
		}
		else if(!strcmp(pArray[2],"OFF"))
		{
			///MX_GPIO_TEST_LED_OFF(TEST_LED_Pin);	//PC12 Low
		}
  }
  else if(!strncmp(pArray[1]," New conn",sizeof(" New conn")))
  {
      return;
  }
  else if(!strncmp(pArray[1]," Already log",sizeof(" Already log")))
  {
      return;
  }
  else if(!strcmp(pArray[1],"SEAT"))
  {
	    if(!strcmp(pArray[2],"1"))
	    	HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,SET);
	    else
	    	HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,RESET);
	    if(!strcmp(pArray[3],"1"))
	    	    	HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,SET);
	    	    else
	    	    	HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,RESET);
	    if(!strcmp(pArray[4],"1"))
	    	    	HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,SET);
	    	    else
	    	    	HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,RESET);
	    if(!strcmp(pArray[5],"1"))
	    	    	HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,SET);
	    	    else
	    	    	HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,RESET);
	    if(!strcmp(pArray[6],"1"))
	    	    	HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,SET);
	    	    else
	    	    	HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,RESET);
	    if(!strcmp(pArray[7],"1"))
	    	    	HAL_GPIO_WritePin(LED5_GPIO_Port,LED5_Pin,SET);
	    	    else
	    	    	HAL_GPIO_WritePin(LED5_GPIO_Port,LED5_Pin,RESET);
	    if(!strcmp(pArray[8],"1"))
	    	    	HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,SET);
	    	    else
	    	    	HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,RESET);
	    if(!strcmp(pArray[9],"1"))
	    	    	HAL_GPIO_WritePin(LED7_GPIO_Port,LED7_Pin,SET);
	    	    else
	    	    	HAL_GPIO_WritePin(LED7_GPIO_Port,LED7_Pin,RESET);
	    for(int j = 2;j<10;j++)
	    {
	    	if(!strcmp(pArray[j],"0"))
	    		seat_num++;
	    }
	    printf("seat_num : %d\r\n", seat_num);
	    if (seat_num <= 0)
	    {
	    	Red = 100;
	    	Green = 100;
	    	Blue = 100;
	    	set_rgb_led(Red, Green, Blue);
	    }
	    else if (seat_num <= 3)
	    {
	    	Red = 10;
	    	Green = 10;
	    	Blue = 500;
	    	set_rgb_led(Red, Green, Blue);
	    }
	    else if (seat_num <= 7)
	    {
	    	Red = 10;
	    	Green = 500;
	    	Blue = 10;
	    	set_rgb_led(Red, Green, Blue);
	    }
	    else if (seat_num > 7)
	    {
	    	Red = 500;
	    	Green = 10;
	    	Blue = 10;
	    	set_rgb_led(Red, Green, Blue);
	    }
  }

  else if(!strcmp(pArray[1],"D"))
  {
	  //Date = pArray[2];
	  //Using = pArray[4];
	  //Study = pArray[6];
	  //SeatNo = pArray[8];
	  seat_no = atoi(pArray[8]);

	  printf("pArray[4] : %s", pArray[4]);
	  printf("pArray[6] : %s", pArray[6]);

	//Using "-" -> ":"
	 char Using_Org[];
	 strcpy(Using_Org, pArray[4]);
	 Using_Org[9]='\n';
	 char *Using = strstr(Using_Org, "-");
	 while (Using != NULL)
	 {
		 Using = strstr(Using_Org+1, "-");
		 strncpy(Using, "\:",1);
	 }


	//Study "-" -> ":"
	 char Study_Org[];
	 strcpy(Study_Org, pArray[6]);
	 Study_Org[9]='\n';
	 char *Study = strstr(Study_Org, "-");
	 while (Study != NULL)
	 {
		 Study = strstr(Study_Org+1, "-");
		 strncpy(Study, "\:",1);
	 }

#if 1
     sprintf(lcdbuff, "DATE: %s", pArray[2]); //16
      printf("%s\r\n", lcdbuff);
     LCD_writeStringXY(0,0,lcdbuff);
     seat_no = atoi(pArray[8]);
     //seat_no = 1;
     sprintf(lcdbuff, "Seat no:  %d",seat_no); //11
      printf("%s\r\n", lcdbuff);
     LCD_writeStringXY(1,0,lcdbuff);
     HAL_Delay(5000);

     sprintf(lcdbuff, " Using  %s", Using); //15
      printf("%s\r\n", lcdbuff);
     LCD_writeStringXY(0,0,lcdbuff);
     sprintf(lcdbuff, " Study  %s", Study); //15
     printf("%s\r\n", lcdbuff);
     LCD_writeStringXY(1,0,lcdbuff);
     HAL_Delay(5000);
#endif
  }

  else
      return;



}
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART6 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2)
    {
    	static int i=0;
    	rx2Data[i] = rx2char;
    	if((rx2Data[i] == '\r')||(btData[i] == '\n'))
    	{
    		rx2Data[i] = '\0';
    		rx2Flag = 1;
    		i = 0;
    	}
    	else
    	{
    		i++;
    	}
    	HAL_UART_Receive_IT(&huart2, &rx2char,1);
    }
    if(huart->Instance == USART6)
    {
    	static int i=0;
    	btData[i] = btchar;
    	if((btData[i] == '\n') || btData[i] == '\r')
    	{
    		btData[i] = '\0';
    		btFlag = 1;
    		i = 0;
    	}
    	else
    	{
    		i++;
    	}
    	HAL_UART_Receive_IT(&huart6, &btchar,1);
    }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{

	switch(GPIO_PIN)
	{
		case BTN0_Pin :
			keyNo = 1;
			break;
		case BTN1_Pin :
			keyNo = 2;
			break;
		case BTN2_Pin :
			keyNo = 3;
			break;
		case BTN3_Pin :
			keyNo = 4;
			break;
		case BTN4_Pin :
			keyNo = 5;
			break;
		case BTN5_Pin :
			keyNo = 6;
			break;
		case BTN6_Pin :
			keyNo = 7;
			break;
		case BTN7_Pin :
			keyNo = 8;
			break;
		case B1_Pin :
				keyNo = 9;
				break;
	}

	//nucleo보드가 눌러지면 flag_Sw를 1로 설정
	//if( GPIO_PIN == GPIO_PIN_Nucleo_Sw) flag_Sw1 = 1;
	//Sw1이 눌러지면 flag_Sw1을 1로 설정
	//if (GPIO_PIN == GPIO_PIN_Sw1) flag_Sw1 = 1;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	time3Cnt++;
	if(time3Cnt >= 1000) //1ms x 1000 -> 1s
	{
		time3SecFlag = 1;
		time3SecCnt++;
		time3Cnt = 0;
	}
	if(time3SecCnt >= 10)
	{
		seatFlag = 1;
		time3SecCnt = 0;

	}
}
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void set_rgb_led(int red, int green, int blue)
{
__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,(red-1)<0?0:red-1);
__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,(green-1)<0?0:green-1);
__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,(blue-1)<0?0:blue-1);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
