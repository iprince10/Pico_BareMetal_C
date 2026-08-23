#include <stdint.h>

#define LED_PIN 25u
#define GPIO_FUNC_SIO 5u
#define SIO_GPIO25_CTRL (*(volatile uint32_t *)(0x400140CC))
#define SIO_GPIO25_BASE 0xd0000000u
#define SIO_GPIO25_OE (*(volatile uint32_t *)(SIO_GPIO25_BASE + 0x20))
#define SIO_GPIO25_OUT (*(volatile uint32_t *)(SIO_GPIO25_BASE + 0x10))

#define TIMER_BASE 0x40054000u
#define TIME_HR (*(volatile uint32_t *)(TIMER_BASE + 0x08))
#define TIME_LR (*(volatile uint32_t *)(TIMER_BASE + 0x0c))

#define IO_BANK0_BASE 0x40014000u

#define GPIO0_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x004))
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
#define UART0_LCR_H (*(volatile uint32_t *)(UART0_BASE + 0x02c))
#define UART0_LCR_H_FEN (1u << 4)
#define UART0_LCR_H_WLEN (3u << 5)

#define GPIO5_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x02c))
#define UART1_FUNC 2u
#define UART1_BASE 0x40038000u
#define UART1_DR (*(volatile uint32_t *)(UART1_BASE + 0x000))
#define UART1_FR (*(volatile uint32_t *)(UART1_BASE + 0x018))
#define UART1_FR_RXFE (1u << 4)
#define UART1_IBRD (*(volatile uint32_t *)(UART1_BASE + 0x024))
#define UART1_FBRD (*(volatile uint32_t *)(UART1_BASE + 0x028))
#define UART1_CR (*(volatile uint32_t *)(UART1_BASE + 0x030))
#define UART1_CR_UARTEN (1u << 0)
#define UART1_CR_RXE_EN (1u << 9)
#define UART1_LCR_H (*(volatile uint32_t *)(UART1_BASE + 0x02c))
#define UART1_LCR_H_FEN (1u << 4)
#define UART1_LCR_H_WLEN (3u << 5)

void uart0_init(void); // initialisation of uart0
void uart1_init(void); // initialisation of uart1

void uart0_putc(char); // transmit character
char uart1_getc(void); // receive character

void uart0_puts(const char *);
void uart1_gets(char *, int);

int string_compare(const char *, const char *);

void delay_ms(uint64_t);
uint64_t read_timer(void);

int main(void)
{
    uart0_init();
    uart1_init();

    SIO_GPIO25_CTRL = GPIO_FUNC_SIO;
    SIO_GPIO25_OE |= (1u << LED_PIN);

    const char send[] = "HELLO";
    char receive[6];

    while (1)
    {
        uart0_puts(send);

        uart1_gets(receive, 5);

        if (string_compare(send, receive))
        {
            SIO_GPIO25_OUT ^= (1u << LED_PIN);
            delay_ms(500);
        }
    }
}

void uart0_init(void)
{
    UART0_CR = 0;

    GPIO0_CTRL = UART0_FUNC;

    UART0_IBRD = 67;
    UART0_FBRD = 52;

    UART0_LCR_H = UART0_LCR_H_WLEN | UART0_LCR_H_FEN; // 8N1 8data, No parity, 1 stop bit, enable fifo

    UART0_CR = UART0_CR_UARTEN | UART0_CR_TXE_EN;
}

void uart0_putc(char c)
{
    while ((UART0_FR & UART0_FR_TXFF) != 0)
    {
        // wait buffer full
    }
    UART0_DR = c;
}

void uart0_puts(const char *send)
{

    while (*send != '\0')
    {
        uart0_putc(*send);
        send++;
    }
}

void uart1_init(void)
{
    UART1_CR = 0;

    GPIO5_CTRL = UART1_FUNC;

    UART1_IBRD = 67;
    UART1_FBRD = 52;

    UART1_LCR_H = UART1_LCR_H_WLEN | UART1_LCR_H_FEN;

    UART1_CR = UART1_CR_UARTEN | UART1_CR_RXE_EN;
}

char uart1_getc(void)
{
    while ((UART1_FR & UART1_FR_RXFE) != 0)
    {
        // wait buffer empty
    }
    uint32_t data = UART1_DR;
    return (char)(data & 0x00ff);
}

void uart1_gets(char *receive, int length)
{
    while (length--)
    {
        *receive = uart1_getc();
        receive++;
    }
    *receive = '\0';
}

int string_compare(const char *send, const char *receive)
{
    while (*send != '\0' && *receive != '\0')
    {
        if (*send != *receive)
        {
            return 0;
        }
        send++;
        receive++;
    }
    return (*send == '\0' && *receive == '\0');
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