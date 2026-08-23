#include <stdint.h>

#define IO_BANK0_BASE 0x40014000u
#define GPIO4_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x024))
#define GPIO6_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x034))
#define GPIO7_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x03c))
#define GPIO8_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x044))

#define SIO_BASE 0xd0000000u
#define SIO_FUNC 5u
#define SIO_GPIO_OE (*(volatile uint32_t *)(SIO_BASE + 0x020))
#define SIO_GPIO_OE_SET (*(volatile uint32_t *)(SIO_BASE + 0x024))
#define SIO_GPIO_OE_CLEAR (*(volatile uint32_t *)(SIO_BASE + 0x028))
#define SIO_GPIO_OUT (*(volatile uint32_t *)(SIO_BASE + 0x010))
#define SIO_GPIO_OUT_SET (*(volatile uint32_t *)(SIO_BASE + 0x014))
#define SIO_GPIO_OUT_CLEAR (*(volatile uint32_t *)(SIO_BASE + 0x018))

#define UART1_BASE 0x40038000u
#define UART1_FUNC 2u
#define UART1_DR (*(volatile uint32_t *)(UART1_BASE + 0x000))
#define UART1_FR (*(volatile uint32_t *)(UART1_BASE + 0x018))
#define UART1_FR_TXFF (1u << 5)
#define UART1_IBRD (*(volatile uint32_t *)(UART1_BASE + 0x024))
#define UART1_FBRD (*(volatile uint32_t *)(UART1_BASE + 0x028))
#define UART1_CR (*(volatile uint32_t *)(UART1_BASE + 0x030))
#define UART1_CR_UARTEN (1u << 0)
#define UART1_CR_TXE_EN (1u << 8)
#define UART1_LCR_H (*(volatile uint32_t *)(UART1_BASE + 0x02c))
#define UART1_LCR_H_FEN (1u << 4)
#define UART1_LCR_H_WLEN (3u << 5)

#define TIMER_BASE 0x40054000u
#define TIME_HR (*(volatile uint32_t *)(TIMER_BASE + 0x08))
#define TIME_LR (*(volatile uint32_t *)(TIMER_BASE + 0x0c))

void uart1_init(void);
void uart1_putc(char c);
void uart1_puts(const char *send);

void lora_tx_init(void);

uint64_t read_timer(void);
void delay_ms(uint64_t milliseconds);

int main(void)
{
    uart1_init();

    while(1){

    }
}

void lora_tx_init(void){
    GPIO6_CTRL = SIO_FUNC;
    GPIO7_CTRL = SIO_FUNC;
    GPIO8_CTRL = SIO_FUNC;
}

void uart1_init(void)
{
    UART1_CR = 0;
    GPIO4_CTRL = UART1_FUNC;

    UART1_IBRD = 813; // for 9600 baud rate
    UART1_FBRD = 13;

    UART1_LCR_H = UART1_LCR_H_FEN | UART1_LCR_H_WLEN; // 8N1 8data, No parity, 1 stop bit, enable fifo

    UART1_CR = UART1_CR_UARTEN | UART1_CR_TXE_EN;
}

void uart1_putc(char c)
{
    while ((UART1_FR & UART1_FR_TXFF) != 0)
    {
        // wait buffer full
    }
    UART1_DR = c;
}

void uart1_puts(const char *send)
{

    while (*send != '\0')
    {
        uart1_putc(*send);
        send++;
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