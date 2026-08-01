#include <stdint.h>

#define LED_PIN 25u
#define GPIO_FUNC_SIO 5u
#define SIO_GPIO25_CTRL (*(volatile uint32_t *)(0x400140CC))
#define SIO_GPIO_BASE 0xd0000000u
#define SIO_GPIO_OE (*(volatile uint32_t *)(SIO_GPIO_BASE + 0x20))
#define SIO_GPIO_OUT (*(volatile uint32_t *)(SIO_GPIO_BASE + 0x10))
#define BLINK_PERIOD_US 500000u
#define TIMER_BASE 0x40054000u
#define TIME_HR (*(volatile uint32_t *)(TIMER_BASE + 0x08))
#define TIME_LR (*(volatile uint32_t *)(TIMER_BASE + 0x0c))

#define INTR (*(volatile uint32_t *)(TIMER_BASE + 0x34))
#define INTE (*(volatile uint32_t *)(TIMER_BASE + 0x38))
#define ALARM0 (*(volatile uint32_t *)(TIMER_BASE + 0x10))

#define ARM_CORTEX_M0_BASE (0xe0000000u)
#define NVIC_ISER (*(volatile uint32_t *)(ARM_CORTEX_M0_BASE + 0xe100))


void isr_irq0(void);

int main(void)
{
    SIO_GPIO25_CTRL = GPIO_FUNC_SIO;
    SIO_GPIO_OE |= (1u << LED_PIN);

    INTE |= (1u << 0);

    uint32_t current_time = TIME_LR; 
    ALARM0 = current_time + BLINK_PERIOD_US;

    NVIC_ISER |= (1u<<0);

    __asm volatile("cpsie i");

    while (1)
    {
        // empty
    }
}

void isr_irq0(void)
{

    INTR = 1u;

    SIO_GPIO_OUT ^= (1u << LED_PIN);

    uint32_t current_time = TIME_LR; 
    ALARM0 = current_time + BLINK_PERIOD_US;
}