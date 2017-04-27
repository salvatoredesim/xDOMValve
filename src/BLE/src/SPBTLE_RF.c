/*
 * ble.c
 *
 *  Created on: 13 apr 2017
 *      Author: Aeromechs
 */

#include "SPBTLE_RF.h"

extern void ParseCommand(evt_gatt_attr_modified_IDB05A1* evt);

volatile int connected = FALSE;
volatile uint8_t set_connectable = 1;
volatile uint16_t connection_handle = 0;
volatile uint8_t notification_enabled = FALSE;

/* Hw and Fw Firmware Data */
uint16_t fwVersion;
uint8_t hwVersion;

/* Board Data */
uint8_t bnrg_expansion_board = IDB04A1;
uint8_t bdaddr[BDADDR_SIZE];

uint16_t service_handle, dev_name_char_handle, appearance_char_handle;


/* Service define data */
uint16_t serviceHandle,charHandle;
uint16_t accServHandle,freeFallCharHandle;
uint8_t retCode;

void SPBTLE_RF_Init(uint8_t* mac_address, const char *name){

	/* Initalizes SPI connection to the expansion board */
	BNRG_SPI_Init();

	/* Activate HCI to send ACI messages */
	HCI_Init();

	/* Reset and get info about software and firmware */
	BlueNRG_RST();

	getBlueNRGVersion(&hwVersion,&fwVersion);

	BlueNRG_RST();

	if (hwVersion > 0x30) { /* X-NUCLEO-IDB05A1 expansion board is used */
		bnrg_expansion_board = IDB05A1;
		    /*
		     * Change the MAC address to avoid issues with Android cache:
		     * if different boards have the same MAC address, Android
		     * applications unless you restart Bluetooth on tablet/phone
		     */
		*(mac_address+5) = 0xF1;
	}

	retCode = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
		                                    CONFIG_DATA_PUBADDR_LEN,
		                                    mac_address);

	if(retCode){
		PRINTF("Setting BD_ADDR failed. \n");
	}

	retCode = aci_gatt_init();

	if(retCode){
		PRINTF("GATT Init Failed. \n");
	}

	if (bnrg_expansion_board == IDB05A1) {
		retCode = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);
	}else {
		retCode = aci_gap_init_IDB04A1(GAP_PERIPHERAL_ROLE_IDB04A1, &service_handle, &dev_name_char_handle, &appearance_char_handle);
	}

	if(retCode != BLE_STATUS_SUCCESS){
		PRINTF("GAP_Init failed.\n");
	}

	retCode = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0,
		                                     strlen(name), (uint8_t *)name);

	if(retCode){
		PRINTF("aci_gatt_update_char_value failed.\n");
		while(1);
	}

	retCode = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,OOB_AUTH_DATA_ABSENT,
		                                         NULL,7,16,USE_FIXED_PIN_FOR_PAIRING,
		                                         123456,BONDING);

	if (retCode == BLE_STATUS_SUCCESS) {
		PRINTF("BLE Stack Initialized.\n");
	}

	PRINTF("SERVER: BLE Stack Initialized\n");

}

void SPBTLE_RF_SetConnectable(SPBTLE_RF_ConnectableProfile connectableProfile)
{
  tBleStatus ret;

  char* name = (char*) malloc(sizeof(char)*(connectableProfile.LocalNameLen+1));
  name[0] = AD_TYPE_COMPLETE_LOCAL_NAME;
  memcpy(name+1,connectableProfile.LocalName,connectableProfile.LocalNameLen);

  /* disable scan response */
  hci_le_set_scan_resp_data(0,NULL);
  PRINTF("General Discoverable Mode.\n");

  ret = aci_gap_set_discoverable(connectableProfile.AdvType, connectableProfile.AdvIntervMin,
		connectableProfile.AdvIntervMax, connectableProfile.OwnAddrType,
		connectableProfile.AdvFilterPolicy, connectableProfile.LocalNameLen+1,
		name, connectableProfile.ServiceUUIDLen,
		connectableProfile.ServiceUUIDList, connectableProfile.SlaveConnIntervMin,
		connectableProfile.SlaveConnIntervMax);
  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("Error while setting discoverable mode (%d)\n", ret);
  }

}

uint16_t SPBTLE_RF_GetFWVersion(){
	return fwVersion;
}

uint8_t SPBTLE_RF_GetHWVersion(){
	return hwVersion;
}


void SPBTLE_RF_HCI_Process(){
	HCI_Process();
}

void SPBTLE_RF_AddService(SPBTLE_RF_Service service,SPBTLE_RF_ServiceHandler* service_handle){
	tBleStatus ret;

	uint8_t uuid[16];

	INVERT_UUID_128(service.UUID, uuid);
	ret = aci_gatt_add_serv(service.UUID_type, uuid, service.service_type,
			service.max_num_attributes, service_handle);
	if (ret != BLE_STATUS_SUCCESS)
		PRINTF("Error while adding service.\n");
}

void SPBTLE_RF_AddCharacteristic(SPBTLE_RF_ServiceHandler* service_handle, SPBTLE_RF_Characteristic characteristic, SPBTLE_RF_CharHandler* char_handler){
	tBleStatus ret;
	uint8_t uuid[16];
	INVERT_UUID_128(characteristic.UUID, uuid);
	ret = aci_gatt_add_char(*service_handle, characteristic.UUID_type, uuid,
			characteristic.char_len, characteristic.char_prop,
			characteristic.security_perm, characteristic.gatt_evt_mask,
			characteristic.enc_size, characteristic.isVariable,
			char_handler);
	if (ret != BLE_STATUS_SUCCESS)
		PRINTF("Error while adding service.\n");
}

void SPBTLE_RF_UpdateCharacteristicValue(SPBTLE_RF_ServiceHandler* service_handler, SPBTLE_RF_CharHandler* char_handler,uint8_t data)
{
  tBleStatus ret;

  ret = aci_gatt_update_char_value(*service_handler, *char_handler, 0, 1, &data);

  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating ACC characteristic.\n") ;
  }
}

void SPBTLE_RF_SetPowerLevel(){
	/* Set output power level */
	  	retCode = aci_hal_set_tx_power_level(1,4);
	  	if(retCode){
	  		PRINTF("Error setting TX power Level Parameters.\n");
	  	}
}

SPBTLE_RF_ConnectableProfile SPBTLE_RF_SetDefaultConnectableProfile(){

	// Define an empty connectable profile
	SPBTLE_RF_ConnectableProfile connectableProfile;
	memset(&connectableProfile,0x00,sizeof(SPBTLE_RF_ConnectableProfile));

	connectableProfile.AdvType = ADV_IND;
	connectableProfile.AdvIntervMin = 0;
	connectableProfile.AdvIntervMax = 0;
	connectableProfile.OwnAddrType = PUBLIC_ADDR;
	connectableProfile.AdvFilterPolicy = NO_WHITE_LIST_USE;
	const char name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','l','u','e','N','R','G','-','M','S'};
	connectableProfile.LocalNameLen = sizeof(name);
	connectableProfile.LocalName = name;
	connectableProfile.ServiceUUIDLen = 0;
	connectableProfile.ServiceUUIDList = NULL;
	connectableProfile.SlaveConnIntervMin = 0;
	connectableProfile.SlaveConnIntervMax = 0;

	return connectableProfile;
}

/**
 * @brief  This function is called when there is a LE Connection Complete event.
 * @param  uint8_t Address of peer device
 * @param  uint16_t Connection handle
 * @retval None
 */
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle)
{
  connected = TRUE;
  connection_handle = handle;

  PRINTF("Connected to device:");
  for(int i = 5; i > 0; i--){
    PRINTF("%02X-", addr[i]);
  }
  PRINTF("%02X\n", addr[0]);
}

/**
 * @brief  This function is called when the peer device gets disconnected.
 * @param  None
 * @retval None
 */
void GAP_DisconnectionComplete_CB(void)
{
  connected = FALSE;
  PRINTF("Disconnected\n");
  /* Make the device connectable again. */
  set_connectable = TRUE;
  notification_enabled = FALSE;
}


/**
 * @brief  Callback processing the ACI events.
 * @note   Inside this function each event must be identified and correctly
 *         parsed.
 * @param  void* Pointer to the ACI packet
 * @retval None
 */
void HCI_Event_CB(void *pckt)
{
  hci_uart_pckt *hci_pckt = pckt;
  /* obtain event packet */
  hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;

  if(hci_pckt->type != HCI_EVENT_PKT)
    return;

  switch(event_pckt->evt){

  case EVT_DISCONN_COMPLETE:
    {
      GAP_DisconnectionComplete_CB();
    }
    break;

  case EVT_LE_META_EVENT:
    {
      evt_le_meta_event *evt = (void *)event_pckt->data;

      switch(evt->subevent){
      case EVT_LE_CONN_COMPLETE:
        {
          evt_le_connection_complete *cc = (void *)evt->data;
          GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
        }
        break;
      }
    }
    break;

  case EVT_VENDOR:
    {
      evt_blue_aci *blue_evt = (void*)event_pckt->data;
      switch(blue_evt->ecode){

      case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
      {
          evt_gatt_attr_modified_IDB05A1 *evt = (evt_gatt_attr_modified_IDB05A1*)blue_evt->data;
          ParseCommand(evt);
      }

#if NEW_SERVICES
      case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
        {
          /* this callback is invoked when a GATT attribute is modified
          extract callback data and pass to suitable handler function */
          if (bnrg_expansion_board == IDB05A1) {
            evt_gatt_attr_modified_IDB05A1 *evt = (evt_gatt_attr_modified_IDB05A1*)blue_evt->data;
            Attribute_Modified_CB(evt->attr_handle, evt->data_length, evt->att_data);
          }
          else {
            evt_gatt_attr_modified_IDB04A1 *evt = (evt_gatt_attr_modified_IDB04A1*)blue_evt->data;
            Attribute_Modified_CB(evt->attr_handle, evt->data_length, evt->att_data);
          }
        }
        break;
#endif

      case EVT_BLUE_GATT_READ_PERMIT_REQ:
        {
          evt_gatt_read_permit_req *pr = (void*)blue_evt->data;
          //Read_Request_CB(pr->attr_handle);
        }
        break;
      }
    }
    break;
  }
}
