// this code is from my bare metal practice

#include <stdint.h>

#define LED_PIN 25u
#define GPIO_FUNC_SIO 5u
#define SIO_GPIO25_CTRL (*(volatile uint32_t *)(0x400140CC))
#define SIO_GPIO_OE (*(volatile uint32_t *)(0xd0000020))
#define SIO_GPIO_OUT (*(volatile uint32_t *)(0xd0000010))
#define TIMER_BASE 0x40054000u
#define TIME_HR (*(volatile uint32_t *)(TIMER_BASE + 0x08))
#define TIME_LR (*(volatile uint32_t *)(TIMER_BASE + 0x0c))

uint64_t read_timer(void);

void delay_ms(uint64_t milliseconds);

int main(void)
{
    SIO_GPIO25_CTRL = GPIO_FUNC_SIO;
    SIO_GPIO_OE |= (1u << LED_PIN);

    while (1)
    {
        SIO_GPIO_OUT |= (1u << LED_PIN); // set using or
        delay_ms(500);
        SIO_GPIO_OUT &= ~(1u << LED_PIN); // clear using and not
        delay_ms(500);
    }
}

uint64_t read_timer(void)
{
    uint32_t high1, low, high2;
    do
    {
        high1 = TIME_HR;
        low = TIME_LR;
        high2 = TIME_HR;
    } while (high1 != high2);

    return ((uint64_t)high2 << 32) | low;
}

void delay_ms(uint64_t milliseconds)
{
    uint64_t start = read_timer();
    uint64_t target_us = milliseconds * 1000;

    while ((read_timer() - start) < target_us)
    {
        // wait;
    }
}