#include <stdint.h>

#define GPIO0_BASE 0x40014000u
#define GPIO0_CTRL (*(volatile uint32_t *)(GPIO0_BASE + 0x004))
#define GPIO1_CTRL (*(volatile uint32_t *)(GPIO0_BASE + 0x00c))
#define UART0_FUNC 2u
#define UART0_BASE 0x40034000u
#define UART0_DR (*(volatile uint32_t *)(UART0_BASE + 0x000))
#define UART0_FR (*(volatile uint32_t *)(UART0_BASE + 0x018))
#define UART0_FR_TXFF (1u << 5)
#define UART0_IBRD (*(volatile uint32_t *)(UART0_BASE + 0x024))
#define UART0_FBRD (*(volatile uint32_t *)(UART0_BASE + 0x028))
#define UART0_CR (*(volatile uint32_t *)(UART0_BASE + 0x030))
#define UART0_CR_UARTEN (1u << 0)
#define UART0_CR_TXE_EN (1u << 8)
#define UART0_CR_RXE_EN (1u << 9)
#define UART0_LCR_H (*(volatile uint32_t *)(UART0_BASE + 0x02c))

void uart_init(void);

void uart_putc(char);

int main(void)
{
    uart_init();
    while (1)
    {
        uart_putc('A');
    }
}

void uart_init(void)
{
    UART0_CR = 0;

    GPIO0_CTRL = UART0_FUNC;
    GPIO1_CTRL = UART0_FUNC;

    UART0_IBRD = 67;
    UART0_FBRD = 52;

    UART0_LCR_H = (3u << 5) | (1u << 4);  //8N1 8data, No parity, 1 stop bit, enable fifo

    UART0_CR = UART0_CR_UARTEN | UART0_CR_TXE_EN | UART0_CR_RXE_EN;
}

void uart_putc(char c)
{
    while ((UART0_FR & UART0_FR_TXFF) != 0)
    {
        // wait buffer full
    }
    UART0_DR = c;
}