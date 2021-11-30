#include "mgos.h"
#include "mgos_jsar.h"
#include "dxlUp.h"

#define UARTno 0
#define LED_PIN   (2)

static void help_text()
{
    LOG(LL_INFO, ("Start updater"));
    LOG(LL_INFO, ("Input ID in dec for flash one sensor"
    LOG(LL_INFO, ("or type '0' for update all connected sensors"));
}

/* Led task - green led on board */
static void led_task(void *arg)
{
    mgos_gpio_toggle(LED_PIN);
    (void) arg;
}

/* User UART callback */
static void user_cb(uint16_t len, uint8_t *data, void *userdata)
{
    mgos_uart_write(UARTno, data, len);
    
    if (len == 1 && data[0] == '0') {
        LOG(LL_INFO, ("\nStart update all sensors"));
        update_all();

    } else if (len == 2) {
        uint8_t id = 10 * (data[0] - '0') + (data[1] - '0');
        LOG(LL_INFO, ("id: %d", id));
        LOG(LL_INFO, ("\nConnect only one sensor"));
        update_one_from_bootloader(id);
    }
    
    help_text();

    (void) userdata;
}


enum mgos_app_init_result mgos_app_init(void) 
{
    mgos_dxl_setUserUartCb(user_cb, NULL);
    mgos_set_timer(1000 , MGOS_TIMER_REPEAT, led_task, NULL);

    help_text();

    return MGOS_APP_INIT_SUCCESS;
}


