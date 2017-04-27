/*
 * ble.h
 *
 *  Created on: 13 apr 2017
 *      Author: Aeromechs
 */

#ifndef BLE_INC_SPBTLE_RF_H_
#define BLE_INC_SPBTLE_RF_H_

#include "config.h"
#include "osal.h"
#include "stm32_bluenrg_ble.h"
#include "bluenrg_utils.h"

#define BDADDR_SIZE 6

#define UUID_128 UUID_TYPE_128

#define COPY_MAC(mac_struct, mac_5, mac_4, mac_3, mac_2, mac_1, mac_0) \
do {\
    mac_struct[0] = mac_0; mac_struct[1] = mac_1; mac_struct[2] = mac_2; mac_struct[3] = mac_3; \
        mac_struct[4] = mac_4; mac_struct[5] = mac_5 \
}while(0)

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

#define INVERT_UUID_128(uuid_struct,inverted_uuid) COPY_UUID_128(inverted_uuid,uuid_struct[0],uuid_struct[1],uuid_struct[2],uuid_struct[3],uuid_struct[4],uuid_struct[5],uuid_struct[6],uuid_struct[7],\
		uuid_struct[8],uuid_struct[9],uuid_struct[10],uuid_struct[11],uuid_struct[12],uuid_struct[13],uuid_struct[14],uuid_struct[15])

#define INVERT_MAC(mac_struct,inverted_mac) COPY_UUID_128(inverted_mac,uuid_struct[0],mac_struct[1],mac_struct[2],mac_struct[3],mac_struct[4],mac_struct[5])

typedef uint16_t SPBTLE_RF_ServiceHandler;
typedef uint16_t SPBTLE_RF_CharHandler;

/* Define a connection profile for the BlueNRG-MS device */
typedef struct{
	uint8_t AdvType; // Allows to set if device launches discoverable and/or scannable events
	uint16_t AdvIntervMin; // Optional, if set to 0 acquires default values
	uint16_t AdvIntervMax;
	uint8_t OwnAddrType; // Define if the address is public or self generated
	uint8_t AdvFilterPolicy; // Define if device is scannable and/or discoverable
	uint8_t LocalNameLen; // Size of name char by char +1
	const char *LocalName; // Name of device
	uint8_t ServiceUUIDLen; //
	uint8_t* ServiceUUIDList; //
    uint16_t SlaveConnIntervMin;
    uint16_t SlaveConnIntervMax;
}SPBTLE_RF_ConnectableProfile;

typedef struct{
	SPBTLE_RF_ServiceHandler service_handler;
	uint8_t UUID[16];
	uint8_t UUID_type;
	uint8_t max_num_attributes;
	uint8_t service_type;
}SPBTLE_RF_Service;


typedef struct{
	uint8_t UUID[16];
	uint8_t UUID_type;
	uint8_t char_len;
	uint8_t char_prop;
	uint8_t security_perm;
	uint8_t gatt_evt_mask;
	uint8_t enc_size;
	uint8_t isVariable;
	SPBTLE_RF_CharHandler charHandle;
}SPBTLE_RF_Characteristic;

/* Initalizes BLE Device */
void SPBTLE_RF_Init(uint8_t* mac_address, const char *name);

/* Set Name BT Device */
void SPBTLE_RF_SetConnectable(SPBTLE_RF_ConnectableProfile connectableProfile);

/* Defines a working default connection profile */
SPBTLE_RF_ConnectableProfile SPBTLE_RF_SetDefaultConnectableProfile();

/* Get Firmware Version */
uint16_t SPBTLE_RF_GetFWVersion();

/* Get Hardware Version */
uint8_t SPBTLE_RF_GetHWVersion();

/* Process HCI Commands */
void SPBTLE_RF_HCI_Process();

/* Add a service with a defined UUID value and type and return an handler to attach characteristics*/
void SPBTLE_RF_AddService(SPBTLE_RF_Service service,SPBTLE_RF_ServiceHandler* service_handle);

/* Add a characteristic from a given configuration */
void SPBTLE_RF_AddCharacteristic(SPBTLE_RF_ServiceHandler* service_handle, SPBTLE_RF_Characteristic characteristic, SPBTLE_RF_CharHandler* char_handler);

/* Update the single-value of a selected caracteristic */
void SPBTLE_RF_UpdateCharacteristicValue(SPBTLE_RF_ServiceHandler* service_handler, SPBTLE_RF_CharHandler* char_handler,uint8_t data);

/* Set power level for the BLE Interface */
void SPBTLE_RF_SetPowerLevel();

void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle);
void GAP_DisconnectionComplete_CB(void);

void HCI_Event_CB(void *pckt);

#endif /* BLE_INC_SPBTLE_RF_H_ */
