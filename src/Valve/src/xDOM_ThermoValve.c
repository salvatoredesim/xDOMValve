/*
 * xDOM_ThermoValve.c
 *
 *  Created on: 21 apr 2017
 *      Author: Aeromechs PC
 */

#include "xDOM_ThermoValve.h"

uint8_t VALVE_MAC_ADDRESS[6] = {0x00,0x01,0x02,0x03,0x04,0x05};

uint8_t UUID_VALVE_SERVICE[16] = {0xFA, 0xC0, 0x3B, 0x4A, 0xEF, 0xBA, 0xAA, 0xFC,
								  0xFA, 0xC0, 0x3B, 0x4A, 0xEF, 0xBA, 0xAA, 0xFC};

uint8_t UUID_COMMAND_SERVICE[16] = {0xFB, 0xC0, 0x3B, 0x4A, 0xEF, 0xBA, 0xAA, 0xFC,
								  0xFA, 0xC0, 0x3B, 0x4A, 0xEF, 0xBA, 0xAA, 0xFC};
uint8_t UUID_MOTOR_CAR[16] = {0xAC, 0xC1, 0x1B, 0x41, 0x3F, 0x2A, 0xFA, 0xAC,
								  0xFA, 0xC0, 0x3B, 0x4A, 0xEF, 0xBA, 0xAA, 0xFC};
uint8_t UUID_COMMAND_CAR[16] = {0xAE, 0xC1, 0x1B, 0x41, 0x3F, 0x2A, 0xFA, 0xAC,
								  0xFA, 0xC0, 0x3B, 0x4A, 0xEF, 0xBA, 0xAA, 0xFC};
uint8_t UUID_TEMP_CAR[16] = {0xAF, 0xC1, 0x1B, 0x41, 0x3F, 0x2A, 0xFA, 0xAC,
								  0xFA, 0xC0, 0x3B, 0x4A, 0xEF, 0xBA, 0xAA, 0xFC};

// Let's define a connectionProfile BLE for the Valve
SPBTLE_RF_ConnectableProfile connectableProfile;

/* Define area for
 * Motor and Temp Characteristics
 * Valve Service
 */
SPBTLE_RF_Service valve_service,command_service;
SPBTLE_RF_Characteristic motor_caracteristic,command_characteristic;


void xDOM_Valve_Init(){

	SPBTLE_RF_Init(VALVE_MAC_ADDRESS,"Motor1");

	connectableProfile = SPBTLE_RF_SetDefaultConnectableProfile();

	// Name must be discovered dinamically, for the moment it's static
	connectableProfile.LocalName = "xDOM-FF:A1:B0-M01";
	connectableProfile.LocalNameLen = 17;

	xDOM_Valve_AddCharacteristic();

	SPBTLE_RF_SetPowerLevel();
}


void xDOM_Valve_AddCharacteristic(){

	memset(&valve_service,0x00,sizeof(SPBTLE_RF_Service));
	memcpy(valve_service.UUID,UUID_VALVE_SERVICE,16);
	valve_service.UUID_type = UUID_128;
	valve_service.max_num_attributes = 7;
	valve_service.service_type = PRIMARY_SERVICE;

	SPBTLE_RF_AddService(valve_service,&valve_service.service_handler);

	// TODO: Verifica aggiunta caratteristica
	memset(&motor_caracteristic,0x00,sizeof(SPBTLE_RF_Characteristic));
	memcpy(motor_caracteristic.UUID,UUID_MOTOR_CAR,16);
	motor_caracteristic.UUID_type = UUID_128;
	motor_caracteristic.char_len = 1;
	motor_caracteristic.char_prop = CHAR_PROP_NOTIFY|CHAR_PROP_READ;
	motor_caracteristic.security_perm = ATTR_PERMISSION_NONE;
	motor_caracteristic.gatt_evt_mask = GATT_DONT_NOTIFY_EVENTS;
	motor_caracteristic.enc_size = 16;
	motor_caracteristic.isVariable = 0;

	SPBTLE_RF_AddCharacteristic(&valve_service.service_handler,motor_caracteristic,&motor_caracteristic.charHandle);


	memset(&command_service,0x00,sizeof(SPBTLE_RF_Service));
	memcpy(command_service.UUID,UUID_COMMAND_SERVICE,16);
	command_service.UUID_type = UUID_128;
	command_service.max_num_attributes = 32;
	command_service.service_type = PRIMARY_SERVICE;

	SPBTLE_RF_AddService(command_service,&command_service.service_handler);

	// TODO: Verifica aggiunta caratteristica
	memset(&command_characteristic,0x00,sizeof(SPBTLE_RF_Characteristic));
	memcpy(command_characteristic.UUID,UUID_COMMAND_CAR,16);
	command_characteristic.UUID_type = UUID_128;
	command_characteristic.char_len = 32;
	command_characteristic.char_prop = CHAR_PROP_NOTIFY|CHAR_PROP_WRITE;
	command_characteristic.security_perm = ATTR_PERMISSION_NONE;
	command_characteristic.gatt_evt_mask = GATT_NOTIFY_ATTRIBUTE_WRITE;
	command_characteristic.enc_size = 16;
	command_characteristic.isVariable = 0;

	SPBTLE_RF_AddCharacteristic(&command_service.service_handler,command_characteristic,&command_characteristic.charHandle);


	/*
	SPBTLE_RF_Characteristic ch2;
	memset(&ch2,0x00,sizeof(SPBTLE_RF_Characteristic));
	memcpy(ch2.UUID,UUID_TEMP_CAR,16);
	ch2.UUID_type = UUID_128;
	ch2.char_len = 6;
	ch2.char_prop = CHAR_PROP_NOTIFY|CHAR_PROP_READ;
	ch2.security_perm = ATTR_PERMISSION_NONE;
	ch2.gatt_evt_mask = GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP;
	ch2.enc_size = 16;
	ch2.isVariable = 0;

	SPBTLE_RF_AddCharacteristic(&valve_service.service_handler,ch2,&ch2.charHandle);
	*/
}

void xDOM_Valve_BLE_Enable(){
    SPBTLE_RF_SetConnectable(connectableProfile);
}

void xDOM_Valve_UpdateStateDegree(uint8_t value_in_degree){
	SPBTLE_RF_UpdateCharacteristicValue(&valve_service.service_handler,&motor_caracteristic.charHandle,value_in_degree);
}

void ParseCommand(evt_gatt_attr_modified_IDB05A1* evt){
	xDOM_Message cmd;
	memset(&cmd,0x00,sizeof(xDOM_Message));
	cmd.source = *(evt->att_data);
	cmd.sqn = *(evt->att_data+1);
	cmd.op = *(evt->att_data+2);
	cmd.len = *(evt->att_data+3);

	uint8_t* payload = (uint8_t*) malloc(cmd.len*sizeof(uint8_t));
	memcpy(payload,evt->att_data+4,cmd.len);

	cmd.payload_address = payload;
}
