/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "UART_REG.h"
#include "string.h"
#include "SPI_REG.h"
#include "GPIO_REG.h"
#include "FLASH_REG.h"
#include "I2C_reg2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
void StartDefaultTask(void *argument);

/* USER CODE BEGIN PFP */
	uint8_t rx_buf[32] = {0};
	int count = 0;
	uint8_t buf = 0;
	 uint16_t x_data = 0;
	 uint16_t y_data = 0;
	 uint16_t z_data = 0;

#define WHO_AM_I 0X0F
#define CTRL_REG1 0X20

#define OUT_X_L 0X28
#define OUT_X_H 0X29

#define OUT_Y_L 0X2A
#define OUT_Y_H 0X2B

#define OUT_Z_L 0X2C
#define OUT_Z_H 0X2D
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void UART_Reveice_Interrupt_Handler()
{
	rx_buf[count++] = (UART2 -> DR2 & (0xff));
}

void UART_DMA_Init()
{
	DMA_1C7 -> S7NDTR2 = sizeof(rx_buf);
	DMA_1C7 -> S7PAR2 = UART2 -> DR2;
	DMA_1C7 -> S7M0AR2 = (uint32_t)rx_buf;
	DMA_1C7 -> S7CR2 = (0b110 << 25) | (1 << 10) | (1);
}

void DMA_1C7_Handler()
{
  __asm("NOP");
}

void SM303_IC2_read(uint8_t address, uint8_t reg, uint8_t* buffer)
{
	 I2C_Start();
	 I2C_Address(address);
	 I2C_Wite(reg);
	 I2C_Start();
	 I2C_Read_OneByte(address + 0x01, buffer);
	 I2C_Stop();
}


void Tim1_Init()
{

	    GPIOD2 -> MODER2 |= (0b10 << 24)|(0b10 << 26)|(0b10 << 28)|(0b10 << 30); // PD12-15 as alternate function

		GPIOD2 -> AFRH2 &= ~(0b1111 << 16)|~(0b1111 << 20)|~(0b1111 << 24)|~(0b1111 << 28); // PD12-15
		GPIOD2 -> AFRH2 |= (0b0010 << 16)|(0b0010 << 20)|(0b0010 << 24)|(0b0010 << 28);

        __HAL_RCC_TIM4_CLK_ENABLE();

		uint16_t* PSC = (uint16_t*)0x40000828;
		*PSC = 16000/100 - 1;

		uint16_t* ARR = (uint16_t*)0x4000082c;
		*ARR = 1000 - 1; //100%

		uint16_t* CCR1 = (uint16_t*)0x40000834;
		*CCR1 = 0;

		uint16_t* CCMR1_Output = (uint16_t*)0x40000818; // OUTPUT
		*CCMR1_Output |= (0b110 << 4) | (0b110 << 12); // CH1, CH2 as PWM mode 1

		uint16_t* CCMR2_Output = (uint16_t*)0x4000081c; // OUTPUT
	    *CCMR2_Output |= (0b110 << 4) | (0b110 << 12); // CH3, CH4 as PWM mode 1


		uint16_t* CCER = (uint16_t*)0x40000820; // OUTPUT
		*CCER |= (1 << 0) | (1 << 4) | (1 << 8) | (1 << 12);

		uint16_t* CR1 = (uint16_t*)0x40000800;
		*CR1 |= (1 << 0);
}


#define Tim5_BaseAdrr 0x40000c00
void Timer5_capture()
{
	GPIOA2 -> MODER2 |= (0b10 << 0 ); // ATF
    GPIOA2 -> AFRL2 |= 2;

	__HAL_RCC_TIM5_CLK_ENABLE();

	uint16_t * PSC = (uint32_t*)(Tim5_BaseAdrr + 0x28); //0x40000c28
	uint16_t * CR1 = (uint32_t*)(Tim5_BaseAdrr + 0x00); //0x40000c00
	uint16_t * ARR = (uint32_t*)(Tim5_BaseAdrr + 0x2c); //0x40000c2c

	*ARR = 0xffff;
	*PSC = 15999;

	/*Time capture - channel 1 - rising*/
	uint16_t * CCER = (uint32_t*)(Tim5_BaseAdrr + 0x20); //0x40000c20
	uint16_t * CCMR1_Output = (uint32_t*)(Tim5_BaseAdrr + 0x18); //0x40000c18
	uint16_t * SMCR = (uint32_t*)(Tim5_BaseAdrr + 0x08); //0x40000c08

	*CCMR1_Output |= 0b01;
	*CCER &= ~ (1 << 1) | (1 << 3);
	*CCER |= 1;


	 /*Reset CNT to 0 when detect rising*/
	*SMCR |= 0b100;
	*SMCR |= 0b101 << 4;
	*CR1 |= 1;

}

void SM303_IC2_read1(uint8_t address, uint8_t reg, uint8_t* buffer)
{
	 I2C_Start1();
	 I2C_Address1(address);
	 I2C_Write1(reg);
	 I2C_Start1();
	 I2C_Read1 (address + 0x01, buffer, 1);
	 I2C_Stop1();
}

uint16_t read_CCR1_Tim4()
{
	uint16_t * CCR1 = (uint32_t*)(Tim5_BaseAdrr + 0x34); //0x40000c2c
	return *CCR1;
}

int x;
void  TIM1_UP_TIM10_IRQHandler()
{
	// __asm("NOP");
	if(x == 0) x = 1;
	//else if (x == 1) x = 0;

	//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, x);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
 // UART_Init();
  //UART_RX_Interrupt_Init();
 // UART_DMA_Init();
  //char msg[] = "Hello World";
  //Flash_Section_Erase(7);
//  cccc(msg,sizeof(msg));
  //Flash_Program(msg,sizeof(msg));
 // Flash_Program(0x08060000 ,msg,sizeof(msg));
 // SPI1_INIT();
 // uint8_t check = SPI_Read(WHO_AM_I);
 // SPI_Write(CTRL_REG1, 0b00001111);
  //I2C_Config1();
  uint8_t check =0;
  HAL_I2C_Mem_Read (&hi2c1, 0x33 ,0x0f,1, &check, 1, 1000);
  //SM303_IC2_read1(0x32, 0x0f, &check);
  //Tim1_Init();
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
   while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  /* for(int i = 0; i < 999; i++)
	   {	HAL_Delay(3);
		   uint16_t* CCR1 = (uint16_t*)0x40000834;
		   *CCR1 = i;

		   uint16_t* CCR2 = (uint16_t*)0x40000838;
		   		   *CCR2 = i;

		   		uint16_t* CCR3 = (uint16_t*)0x4000083c;
		   				   *CCR3 = i;

		   				uint16_t* CCR4 = (uint16_t*)0x40000840;
		   						   *CCR4 = i;
	   }

	   for(int i = 999; i > 0; i--)
		   {	HAL_Delay(3);
			   uint16_t* CCR1 = (uint16_t*)0x40000834;
			   *CCR1 = i;

			   uint16_t* CCR2 = (uint16_t*)0x40000838;
			   		   *CCR2 = i;

			   		uint16_t* CCR3 = (uint16_t*)0x4000083c;
			   				   *CCR3 = i;

			   				uint16_t* CCR4 = (uint16_t*)0x40000840;
			   						   *CCR4 = i;
		   }*/

	  /* HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	   Tim_Delay();
	   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	   Tim_Delay();1*/

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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD11 PD12 PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

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
