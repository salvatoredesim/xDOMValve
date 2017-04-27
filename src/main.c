/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
#define BDADDR_SIZE 6

#include "cube_hal.h"
#include "xDOM_ThermoValve.h"

void User_Process();
extern volatile uint8_t set_connectable;
int main(void)
{

	/* Initialize HAL */
	HAL_Init();

	/* Configure the User Button in GPIO Mode */
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

	/* Initialize Clock Info */
	SystemClock_Config();

	BSP_LED_Init(LED2);

	// Initialize the Valve
	xDOM_Valve_Init();

	while(1){
	    SPBTLE_RF_HCI_Process();
	    User_Process();
	    BSP_LED_Toggle(LED2);
	    HAL_Delay(1000);
	}

	/**
	 * @brief  Process user input (i.e. pressing the USER button on Nucleo board)
	 *         and send the updated acceleration data to the remote client.
	 *
	 * @param  AxesRaw_t* p_axes
	 * @retval None
	 */

}



void User_Process()
	{
	  if(set_connectable){
		xDOM_Valve_BLE_Enable();
		set_connectable = FALSE;
	  }

	}
