#include <string.h>
#include "dxlUp.h"
#include "dxlUp_port.h"

#include "firmwares.h"

#define BAUD_57600  (57600) 
#define BAUD_1kk  (1000000) 
#define CMD_STATUS_OK  (0x00) 
#define BLOCK_SIZE 64
#define CRC_AND_ERASE_SIZE 16
#define FW_VERSION 1

#define STOP_REG 0
#define RESET_REG 23
#define BOOT_ID 1
#define ID_START 0x02 
#define ID_MAX 0x20
#define TRY 5

static uint8_t sensor_update(uint8_t id); 

/* Update firmware */
static uint8_t sensor_update(uint8_t id) 
{
    const uint8_t * firmware = NULL;
    uint16_t fw_size = 0;
    uint8_t res = 0;

    res = firmware_get(id, &firmware, &fw_size);
    if (firmware == NULL) { return DXLUP_ERR_FW_IS_NULL; }
    if (fw_size == 0) {     return DXLUP_ERR_SIZE_IS_ZERO; }

    dxlUp_begin(BAUD_1kk);
    uint8_t try = 50;
    while (dxlUp_write_1byte(BOOT_ID, STOP_REG, 0) != CMD_STATUS_OK 
           && try-- != 0) {
        dxlUp_printStr("\n\tBoot isn't responding.");
        dxlUp_printStrInt("Try to connect: ", try);
        dxlUp_delay(100); 
        if (try == 0) {
            return DXLUP_ERR_BOOTLOAD_TIMEOUT;
        }
    }
    
    dxlUp_printStrInt("\n\tfw name: ", id);
    dxlUp_printStrInt("\n\tfw size: ", fw_size);
 
    /* Send Erase cmd */
    uint8_t crc[CRC_AND_ERASE_SIZE]; 
    memcpy(crc, firmware, CRC_AND_ERASE_SIZE);   
    if (dxlUp_write(BOOT_ID, FW_VERSION, crc, CRC_AND_ERASE_SIZE) 
                                                            != CMD_STATUS_OK) {
        return DXLUP_ERR_ERASE_FAIL;
    }

    /* Calculate blocks */
	uint8_t fullPacks = fw_size / BLOCK_SIZE;
    dxlUp_printStrInt("\n\tBlocks: ", fullPacks);
	if (fullPacks <= 0)
		return DXLUP_ERR_BLOCKS_IS_ZERO;

    /* Send firmware */
	dxlUp_printStr("\n\tWriting new fw:\n\t");
	for (uint8_t i = 0; i < fullPacks; i++) {
		uint8_t addr_and_data_block[BLOCK_SIZE + 1];

        dxlUp_printStr("."); 
        if (i % 32 == 0 && i > 0) dxlUp_printStr("\n\t");

		addr_and_data_block[0] = i;
		memcpy(addr_and_data_block + 1, 
               firmware + CRC_AND_ERASE_SIZE + i * BLOCK_SIZE, 
               BLOCK_SIZE);

        uint8_t res = 0;
        res = dxlUp_write(BOOT_ID, 0, addr_and_data_block, BLOCK_SIZE + 1);
        if (res != CMD_STATUS_OK) {
            dxlUp_printStrInt(" Fail at ", i);
            dxlUp_printStrInt(" Write status: ", res);
            return DXLUP_ERR_FLASH_FAIL;
        }
		
	}

    dxlUp_printStr("\n\tComplete!");
    dxlUp_printStr("\n\tRebooting sensor");
    dxlUp_write_1byte(BOOT_ID, RESET_REG, 0); 

    return DXLUP_ERR_NONE;
}

/* Update all alive sensors */
uint8_t update_all() {
    for (uint8_t id = ID_START; id < ID_MAX; id++) 
    {
        dxlUp_printStrInt("\nId = ", id); 

        dxlUp_delay(10);
        dxlUp_begin(BAUD_57600);

        /* Pinging sensor */
        uint8_t res = dxlUp_ping(id);
        if (res != CMD_STATUS_OK) {
            dxlUp_printStr(" no sensor");
            continue;
        } else {
            dxlUp_printStr(" found!");
        }

        /* Send reset */
        dxlUp_printStr("\n\tReset. ");
        dxlUp_write_1byte(id, RESET_REG, 0); 
        dxlUp_begin(BAUD_1kk);

        /* Connect to bootloader */
        dxlUp_printStr("Connecting.");
        for (int i = 0; i < TRY; i++) {
            dxlUp_printStr(".");  
            dxlUp_delay(10);
            if (dxlUp_write_1byte(BOOT_ID, STOP_REG, 0) == CMD_STATUS_OK) {
                /* Flash new fw */
                uint8_t res = sensor_update(id);
                if (res == DXLUP_ERR_NONE) {
                    dxlUp_printStr("\n\tUpdate - Ok!");
                } else {
                    dxlUp_printStrInt("\n\tUpdate - Error: ", res);
                }

                dxlUp_delay(2500);
                break;
            }

            dxlUp_delay(200);
        }

    }

   return 1;
}


/* Update one dead sensor */
uint8_t update_one_from_bootloader(uint8_t id)
{
    return sensor_update(id);
}


/*
  // Print fw to console 
  for (uint16_t i = 0; i < fw_size + CRC_AND_ERASE_SIZE;) 
  { 
    for (uint8_t k = 0; k < 16; k++) 
    { 
      Serial.print("0x");
      Serial.print(firmware[i + k] < 16 ? "0" : "");
      Serial.print(firmware[i + k], HEX);
      Serial.print(" ");
      delay(1);
    }
    i += 16;
    Serial.println();
    
  }
  Serial.println();
*/