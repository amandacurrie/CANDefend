#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "bsp/board.h"
#include "tusb.h"

#include "hardware/irq.h"
#include "can2040.h"
#include "RP2040.h"
#include "pico/stdlib.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "can2040/src/can2040.h"


static struct can2040 cbus;
bool messageReceived = false;

static void
can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    messageReceived = true;
    // Add message processing code here...
    printf("Message received\nId: %d\nMessage: %s\n", msg->id, msg->data);
    for( int i = 0; i < 5; i++ ) {
        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(500);
        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(500);
    }
}

static void
PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

void
canbus_setup(void)
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 500000;
    // Recieving GP4 (6), transmitting GP5 (7)
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    NVIC_SetPriority(PIO0_IRQ_0, 1);
    NVIC_EnableIRQ(PIO0_IRQ_0);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

int main() {
    stdio_init_all();
    
    // Sets up rx and tx, starts can bus
    canbus_setup();

    // Loop until report state 
    while( !messageReceived ) {
        sleep_ms(1000);
    }


    can2040_stop( &cbus );
    return 0;
}
