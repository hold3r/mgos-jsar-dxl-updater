#ifndef MGOS_DXL_UP_H_
#define MGOS_DXL_UP_H_

#include <stdint.h>

#define DXLUP_ERR_NONE  0
#define DXLUP_ERR_FLASH_FAIL 122
#define DXLUP_ERR_BLOCKS_IS_ZERO 123
#define DXLUP_ERR_ERASE_FAIL 124
#define DXLUP_ERR_BOOTLOAD_TIMEOUT 125  
#define DXLUP_ERR_SIZE_IS_ZERO 126  
#define DXLUP_ERR_FW_IS_NULL  127   


uint8_t update_all();
uint8_t update_one_from_bootloader(uint8_t id);

#endif /* MGOS_DXL_UP_H_ */