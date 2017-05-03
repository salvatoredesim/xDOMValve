/*
 * xDOM_ThermoValve.h
 *
 *  Created on: 21 apr 2017
 *      Author: Aeromechs PC
 */

#ifndef VALVE_INC_XDOM_THERMOVALVE_H_
#define VALVE_INC_XDOM_THERMOVALVE_H_

#include "SPBTLE_RF.h"
#include "uln2003.h"

typedef struct{
	uint8_t source;
	uint8_t sqn;
	uint8_t op;
	uint8_t len;
	uint8_t* payload_address;
}xDOM_Message;

/* Valve Initializer */
void xDOM_Valve_Init();

/* Initialize the Valve Characteristic in the Valve Service */
void xDOM_Valve_AddCharacteristic();

/* Enable BLE Interface */
void xDOM_Valve_BLE_Enable();

/* Update the Valve state opening value */
void xDOM_Valve_UpdateStateDegree(uint8_t value_in_degree);

void ParseCommand(evt_gatt_attr_modified_IDB05A1* evt);

#endif /* VALVE_INC_XDOM_THERMOVALVE_H_ */
