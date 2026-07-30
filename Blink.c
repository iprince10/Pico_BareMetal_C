#include <stdint.h>

#define LED_PIN 25u
#define GPIO_FUNC_SIO 5u
#define SIO_GPIO25_CTRL (*(volatile uint32_t *)(0x400140CC))
#define SIO_GPIO_OE (*(volatile uint32_t *)(0xd0000020))
#define SIO_GPIO_OUT (*(volatile uint32_t *)(0xd0000010))

void delay(volatile uint32_t);


int main(void)
{
    SIO_GPIO25_CTRL = GPIO_FUNC_SIO;
    SIO_GPIO_OE |= (1u << LED_PIN);

    while (1)
    {
        SIO_GPIO_OUT |= (1u << LED_PIN); //set using or
        delay(500000);
        SIO_GPIO_OUT &= ~(1u << LED_PIN); //clear using and not 
        delay(500000);
    }
}

void delay(volatile uint32_t count)
{
    while (count--)
    {
        __asm volatile("nop");
    }
}

