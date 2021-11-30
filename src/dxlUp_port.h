#ifndef DXLUP_PORT_H_
#define DXLUP_PORT_H_

#include <stdint.h>

void dxlUp_delay(uint16_t delay);
void dxlUp_printStr(const char * s);
void dxlUp_printInt16(int16_t int16);
void dxlUp_printStrInt(const char * s, int16_t int16);

void dxlUp_begin(uint32_t baud);
uint8_t dxlUp_ping(uint8_t id);
uint8_t dxlUp_write_1byte(uint8_t id, uint8_t reg, uint8_t byte);
uint8_t dxlUp_write(uint8_t id, uint8_t reg, 
                         const uint8_t *ptr, uint8_t size);



#endif /* DXLUP_PORT_H_ */