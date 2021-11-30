#include "dxlUp_port.h"
#include "mgos_jsar.h"
#include "mgos.h"

#define UART0 0

void dxlUp_delay(uint16_t delay)
{
    mgos_msleep(delay);
}

void dxlUp_printStr(const char * s)
{
    mgos_uart_printf(UART0, s);
}

void dxlUp_printInt16(int16_t int16)
{
    mgos_uart_printf(UART0, "%d", int16);
}

void dxlUp_printStrInt(const char * s, int16_t int16)
{
    mgos_uart_printf(UART0, s);
    mgos_uart_printf(UART0, "%d", int16);
}


/* Begin */
void dxlUp_begin(uint32_t baud) 
{
    mgos_dxl_direct_begin(baud);
}

/* Ping */
uint8_t dxlUp_ping(uint8_t id) 
{
    return mgos_dxl_direct_ping(id);
}

/* Write byte */
uint8_t dxlUp_write_1byte(uint8_t id, uint8_t reg, uint8_t byte) 
{
    return mgos_dxl_direct_write_byte(id, reg, byte);
}

/* Write seq */
uint8_t dxlUp_write(uint8_t id, uint8_t reg, 
                         const uint8_t *ptr, uint8_t size) 
{
    return mgos_dxl_direct_write(id, reg, ptr, size);
}

