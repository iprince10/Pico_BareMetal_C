#include <stdint.h>
#include <timer.h>
#include <sr04t.h>
#include <lora.h>

#define IO_BANK0_BASE 0x40014000u
#define SIO_BASE 0xd0000000u
#define SIO_GPIO_OE (*(volatile uint32_t *)(SIO_BASE + 0x020))
#define LED_PIN_25 25u
#define GPIO_FUNC_SIO (5u)
#define GPIO25_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x0cc))

int main(void)
{
    uart0_init();
    gpio3_init_input();
    gpio2_init_output();
    lora_tx_init();
    uart1_init();

    GPIO25_CTRL = GPIO_FUNC_SIO;
    SIO_GPIO_OE |= (1u << LED_PIN_25); // output enable for led pin gpio 25

    uart0_puts("Ready\r\n");
    __asm volatile("cpsie i");

    while (1)
    {
        // Reset state for clean cycle
        __asm volatile("cpsid i");
        __asm volatile("" ::: "memory");
        rise_captured = 0;
        ready_flag = 0;
        rise_time = 0;
        fail_time = 0;
        __asm volatile("" ::: "memory");
        __asm volatile("cpsie i");

        sr04t_send_trigger();
        delay_ms(100); // wait for echo

        // Read result
        __asm volatile("cpsid i");
        __asm volatile("" ::: "memory");
        uint64_t r = rise_time, f = fail_time;
        uint8_t flag = ready_flag;
        ready_flag = 0;
        __asm volatile("" ::: "memory");
        __asm volatile("cpsie i");

        uint8_t status;
        uint8_t dist_hi, dist_lo;

        if (flag && f > r)
        {
            uint64_t duration_us = f - r;
            uint64_t distance_cm = duration_us / 58;

            if (distance_cm > 500 || distance_cm <= 20)
            {
                // max & min distance
                uart0_puts("TANK FULL\r\n");
                status = 2;
                dist_hi = 0;
                dist_lo = 0;
            }
            else
            {
                uart0_putnum(distance_cm);
                uart0_puts(" cm\r\n");
                status = 1;
                dist_hi = (distance_cm >> 8) & 0xff;
                dist_lo = (distance_cm & 0xff);
            }
        }
        else
        {
            uart0_puts("no measurement\r\n");
            status = 3;
            dist_hi = 0;
            dist_lo = 0;
        }

        uint8_t checksum = status ^ dist_hi ^ dist_lo;
        uint8_t packet[6] = {0xAA, status, dist_hi, dist_lo, checksum, 0x55};

        uart1_write_bytes(packet, 6);
        delay_ms(100); // let the module clear the air before the next cycle

        delay_ms(500); // wait between triggers
    }
}
