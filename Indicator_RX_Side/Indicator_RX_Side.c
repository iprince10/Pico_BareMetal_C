#include <stdint.h>
#include <bitmap.h>
#include <timer.h>
#include <ili9341.h>
#include <lora_rx.h>

#define IO_BANK0_BASE 0x40014000u
#define SIO_BASE 0xd0000000u
#define SIO_GPIO_OE (*(volatile uint32_t *)(SIO_BASE + 0x020))
#define LED_PIN_25 25u
#define GPIO_FUNC_SIO (5u)
#define GPIO25_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x0cc))

int main(void)
{
    ili9341_init();
    ili9341_fill_white();
    display_init_log();
    lora_rx_init();
    delay_ms(1000);
    uart1_init();
    uart0_init();
    uart0_puts("READY\r\n");
    // set_param_config_rx();
    check_config_rx();
    display_add_line("PRINCE");

    GPIO25_CTRL = GPIO_FUNC_SIO;
    SIO_GPIO_OE |= (1u << LED_PIN_25); // output enable for led pin gpio 25

    while (1)
    {
        if (wait_aux_high())
        {
            while (uart1_has_data())
            {
                uint8_t byte = (uint8_t)uart1_getc();

                // Print readable hex to serial monitor
                const char hex[] = "0123456789ABCDEF";
                uart0_putc(hex[(byte >> 4) & 0x0F]);
                uart0_putc(hex[byte & 0x0F]);
                uart0_putc(' ');
                // uart0_putc(uart1_getc());
                // display_add_line("PRINCE JHA");
            }
        }
        delay_ms(500); // wait between triggers
    }
}