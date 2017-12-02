

#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "portmacro.h"

typedef void * TaskHandle_t;

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define DEFAULT_I2C          I2C0

#define I2C_EEPROM_BUS       DEFAULT_I2C
#define I2C_IOX_BUS          DEFAULT_I2C

#define SPEED_100KHZ         100000
#define SPEED_400KHZ         400000

static int mode_poll;   /* Poll/Interrupt mode flag */
static I2C_ID_T i2cDev = DEFAULT_I2C; /* Currently active I2C device */

/* EEPROM SLAVE data */
#define I2C_SLAVE_EEPROM_SIZE       64

extern TaskHandle_t xInfotainmentTask;
extern TaskHandle_t xLCDTask;

struct buffer_data{

	uint8_t MSB_Byte;
	uint8_t LSB_byte;
	unsigned int stacked_r0;
	unsigned int stacked_r1;
	unsigned int stacked_r2;
	unsigned int stacked_r3;
	unsigned int stacked_r12;
	unsigned int stacked_lr;
	unsigned int stacked_pc;
	unsigned int stacked_psr;
	unsigned int stacked_bfar;
	unsigned int stacked_cfsr;
	unsigned int stacked_hfsr;
	unsigned int stacked_mmfar;
	unsigned int stacked_afsr;
	unsigned int stacked_scb_shcsr;

}buffer_data;







/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* State machine handler for I2C0 and I2C1 */
static void i2c_state_handling(I2C_ID_T id)
{
	if (Chip_I2C_IsMasterActive(id)) {
		Chip_I2C_MasterStateHandler(id);
	} else {
		Chip_I2C_SlaveStateHandler(id);
	}
}

/* Set I2C mode to polling/interrupt */
static void i2c_set_mode(I2C_ID_T id, int polling)
{
	if(!polling) {
		mode_poll &= ~(1 << id);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandler);
		NVIC_EnableIRQ(id == I2C0 ? I2C0_IRQn : I2C1_IRQn);
	} else {
		mode_poll |= 1 << id;
		NVIC_DisableIRQ(id == I2C0 ? I2C0_IRQn : I2C1_IRQn);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandlerPolling);
	}
}

/* Initialize the I2C bus */
static void i2c_app_init(I2C_ID_T id, int speed)
{
	Board_I2C_Init(id);

	/* Initialize I2C */
	Chip_I2C_Init(id);
	Chip_I2C_SetClockRate(id, speed);

	/* Set default mode to interrupt */
	i2c_set_mode(id, 0);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	I2C Interrupt Handler
 * @return	None
 */
void I2C1_IRQHandler(void)
{
	i2c_state_handling(I2C1);
}

/**
 * @brief	I2C0 Interrupt handler
 * @return	None
 */
void I2C0_IRQHandler(void)
{
	i2c_state_handling(I2C0);
}

/**
 * @brief	Main program body
 * @return	int
 */
void test(void)
{
	int tmp,size;

	uint8_t my_buf[40]={0};
	my_buf[0]=0x00;
	my_buf[1]=0x00;
	my_buf[2]=buffer_data.stacked_pc>>24;
	my_buf[3]=buffer_data.stacked_pc>>16;
	my_buf[4]=buffer_data.stacked_pc>>8;
	my_buf[5]=buffer_data.stacked_pc;

	my_buf[6]=buffer_data.stacked_r0>>24;
	my_buf[7]=buffer_data.stacked_r0>>16;
	my_buf[8]=buffer_data.stacked_r0>>8;
	my_buf[9]=buffer_data.stacked_r0;


	my_buf[10]=buffer_data.stacked_r1>>24;
	my_buf[11]=buffer_data.stacked_r1>>16;
	my_buf[12]=buffer_data.stacked_r1>>8;
	my_buf[13]=buffer_data.stacked_r1;




	my_buf[14]=buffer_data.stacked_r2>>24;
	my_buf[15]=buffer_data.stacked_r2>>16;
	my_buf[16]=buffer_data.stacked_r2>>8;
	my_buf[17]=buffer_data.stacked_r2;



	my_buf[18]=buffer_data.stacked_r3>>24;
	my_buf[19]=buffer_data.stacked_r3>>16;
	my_buf[20]=buffer_data.stacked_r3>>8;
	my_buf[21]=buffer_data.stacked_r3;

	my_buf[22]=buffer_data.stacked_r12>>24;
	my_buf[23]=buffer_data.stacked_r12>>16;
	my_buf[24]=buffer_data.stacked_r12>>8;
	my_buf[25]=buffer_data.stacked_r12;


	my_buf[26]=buffer_data.stacked_psr>>24;
	my_buf[27]=buffer_data.stacked_psr>>16;
	my_buf[28]=buffer_data.stacked_psr>>8;
	my_buf[29]=buffer_data.stacked_psr;


	my_buf[30]=buffer_data.stacked_mmfar>>24;
	my_buf[31]=buffer_data.stacked_mmfar>>16;
	my_buf[32]=buffer_data.stacked_mmfar>>8;
	my_buf[33]=buffer_data.stacked_mmfar;


	static I2C_XFER_T xfer;

	SystemCoreClockUpdate();
	Board_Init();
	i2c_app_init(I2C0, SPEED_100KHZ);

	size=35;
	xfer.slaveAddr = 0x50;
	xfer.txSz = size;
	xfer.txBuff = my_buf;

	tmp = Chip_I2C_MasterSend(i2cDev, xfer.slaveAddr, xfer.txBuff, xfer.txSz);
	for(int i=0;i<96000;i++)
		for(int j=0;j<10;j++);


	//while(1);
	return 0;
}

__attribute__ ((section(".after_vectors")))
void MemManage_Handler(void)
{
	__asm volatile
	(
			"push {lr}											\n"
			" tst lr, #4										\n"
			" ite eq											\n"
			" mrseq r0, msp										\n"
			" mrsne r0, psp										\n"
			" ldr r1, [r0, #24]									\n"
			" ldr r2, handler2_address_const						\n"
			" bx r2												\n"
			" handler2_address_const: .word pop_registers_from_fault_stack\n"
	);


	//while(1);
}

void delay(void)
{
	for(volatile uint32_t i=0;i<96000;i++)
		for(volatile uint32_t j=0;j<10;j++);
}

void pop_registers_from_fault_stack(unsigned int * hardfault_args)
{

	volatile unsigned long i=0;

	Chip_GPIO_SetDir(LPC_GPIO, 0, 10, true);
	Chip_GPIO_SetDir(LPC_GPIO, 2, 2, true);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 10, true);
	Chip_GPIO_SetPinState(LPC_GPIO, 2, 2, true);

	buffer_data.stacked_r0 = ((unsigned long) hardfault_args[0]);
	buffer_data.stacked_r1 = ((unsigned long) hardfault_args[1]);
	buffer_data.stacked_r2 = ((unsigned long) hardfault_args[2]);
	buffer_data.stacked_r3 = ((unsigned long) hardfault_args[3]);

	buffer_data.stacked_r12 = ((unsigned long) hardfault_args[4]);
	buffer_data.stacked_lr = ((unsigned long) hardfault_args[5]);
	buffer_data.stacked_pc = ((unsigned long) hardfault_args[6]);
	buffer_data.stacked_psr = ((unsigned long) hardfault_args[7]);


	buffer_data.stacked_bfar = (*((volatile unsigned long *)(0xE000ED38)));
	buffer_data.stacked_cfsr = (*((volatile unsigned long *)(0xE000ED28)));
	buffer_data.stacked_hfsr = (*((volatile unsigned long *)(0xE000ED2C)));
	buffer_data.stacked_mmfar =*((volatile unsigned long *)(0xE000ED34));
	buffer_data.stacked_scb_shcsr =(*((volatile unsigned long *)(0xE000ED24)));


	SCB->CFSR|=(1<<1)|(1<<7);
	__set_CONTROL(0x00);

	//test();

	if(buffer_data.stacked_pc>=0x7be4 && buffer_data.stacked_pc<=0x7d64)
	{
		Chip_UART_SendBlocking(LPC_UART0,"Task Terminated", 15);
		vTaskDelete(xInfotainmentTask);

		xTaskResumeFromISR(xLCDTask);
	}
	else
	{

		while(1)
		{
			//uint32_t foo = Chip_Clock_GetMainClockRate();
			//foo = Chip_Clock_GetMainOscRate();
			Chip_GPIO_SetPinState(LPC_GPIO, 2, 2, false);
			Chip_GPIO_SetPinState(LPC_GPIO, 0, 10, false);

			delay();
			Chip_GPIO_SetPinState(LPC_GPIO, 2, 2, true);
			Chip_GPIO_SetPinState(LPC_GPIO, 0, 10, true);
			delay();

			i++;
			if(i>10)
				break;
		}
		NVIC_SystemReset();

	}
}

