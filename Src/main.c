/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

uint8_t returnCol();
void setLowOutput(uint8_t row);
void printButton(int row, int col);

char pressedChar[16] = {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};


	//0x4002 0800 - 0x4002 0BFF GPIOC

	//0x4002 3800 - 0x4002 3BFF RCC

	//6.3.10 RCC AHB1 peripheral clock enable register (RCC_AHB1ENR) Address offset: 0x30

	//7.4.1 GPIO port mode register (GPIOx_MODER) (x = A..H) Address offset: 0x00
	//Bits 2y:2y+1 00 in, 01 out

	//7.4.4 GPIO port pull-up/pull-down register (GPIOx_PUPDR) (x = A..H) Address offset: 0x0C
	//Bits 2y:2y+1 00 none, 01 pull-up, 10 pull-down

	//7.4.5 GPIO port input data register (GPIOx_IDR) (x = A..H) Address offset: 0x10

	//7.4.6 GPIO port output data register (GPIOx_ODR) (x = A..H) Address offset: 0x14


	uint32_t volatile * const pClkCtrlReg = (uint32_t*) (0x40023800 + 0x30);

	uint32_t volatile * const pMode_C_Reg = (uint32_t*) (0x40020800 + 0x00);

	uint32_t volatile * const pPUPD_C_Reg = (uint32_t*) (0x40020800 + 0x0C);
	uint32_t volatile * const pInD_C_Reg = (uint32_t*) (0x40020800 + 0x10);

	uint32_t volatile * const pOutD_C_Reg = (uint32_t*) (0x40020800 + 0x14);



int main(void)
{
	// 4 input (pull up) 0 = pressed
	// PC(0-3) on pad(4-7)
	// 4 outputs
	// PC(4-7) on pad(0-3)


	*pClkCtrlReg |= (4);


	*pMode_C_Reg &= ~(0xFF);

	*pPUPD_C_Reg &= ~(0xFF);
	*pPUPD_C_Reg |= (0b01010101);

	*pMode_C_Reg |= (0b01010101<<8);

    /* Loop forever */
	while(1){
		for(int i=0; i<4; i++){
			setLowOutput(i);
			uint8_t col = returnCol();
			if(col!=0){
				for(uint32_t i=0; i<300000; i++);	//debouncing delay
				printButton(i+1,col);
			}
		}
	}
}

uint8_t returnCol(){
	for(int i=0; i<4; i++){
		if(!(*pInD_C_Reg & (1<<i))){
			return (uint8_t)(i+1);
		}
	}
	return (uint8_t)0;
}

void setLowOutput(uint8_t row){

	*pOutD_C_Reg |= (0b1111<<4);

	*pOutD_C_Reg &= ~(1<<(row+4));
}

void printButton(int row, int col){
	printf("%c\n",pressedChar[(row-1)*4+(col-1)]);
}
