#include <stdint.h>

#define IO_BANK0_BASE 0x40014000u
#define PAD_BANK0_BASE 0x4001c000u

#define SIO_BASE 0xd0000000u
#define SIO_OE (*(volatile uint32_t *)(SIO_BASE + 0x20))
#define SIO_OUT (*(volatile uint32_t *)(SIO_BASE + 0x10))

#define LED_PIN_25 25u
#define GPIO_FUNC_SIO 5u
#define GPIO25_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x0cc))

#define GPIO2_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x014))
#define PAD_GPIO2_CTRL (*(volatile uint32_t *)(PAD_BANK0_BASE + 0x0c))
#define GPIO3_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x01c))
#define PAD_GPIO3_CTRL (*(volatile uint32_t *)(PAD_BANK0_BASE + 0x10))
#define PAD_GPIO_CTRL_IE (1u << 6)
#define PAD_GPIO_CTRL_PDE (1u << 2)
#define PAD_GPIO_CTRL_SCHMITT (1u << 1)

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

#define PROC0_INTR0 (*(volatile uint32_t *)(IO_BANK0_BASE + 0x0f0))
#define PROC0_INTE0 (*(volatile uint32_t *)(IO_BANK0_BASE + 0x100))
#define PROC0_INTS0 (*(volatile uint32_t *)(IO_BANK0_BASE + 0x120))
#define PROC0_INTE0_GPIO3_EDGE_HIGH_IRQ_EN (1u << 15)
#define PROC0_INTR0_GPIO3_HIGH_CLEAR (1u << 15)
#define PROC0_INTE0_GPIO3_EDGE_LOW_IRQ_EN (1u << 14)
#define PROC0_INTR0_GPIO3_LOW_CLEAR (1u << 14)

#define TIMER_BASE 0x40054000u
#define TIME_HR (*(volatile uint32_t *)(TIMER_BASE + 0x08))
#define TIME_LR (*(volatile uint32_t *)(TIMER_BASE + 0x0c))

#define ARM_CORTEX_NVIC_BASE (0xe0000000u)
#define NVIC_ISER (*(volatile uint32_t *)(ARM_CORTEX_NVIC_BASE + 0xe100))
#define NVIC_IO_IRQ_BANK0_EN (1u << 13)

uint64_t read_timer(void);
void delay_ms(uint64_t milliseconds);
void delay_us(uint64_t microseconds);

void uart0_init(void);
void uart0_putc(char);
void uart0_puts(const char *);
void uart0_putnum(uint64_t num);

void gpio3_init_input(void);

void gpio2_init_output(void);
void gpio2_set_high(void);
void gpio2_set_low(void);
void sr04t_send_trigger(void);

void isr_irq13(void);

volatile uint8_t rise_captured = 0;
volatile uint64_t rise_time = 0;
volatile uint64_t fail_time = 0;
volatile uint8_t ready_flag = 0;

int main(void)
{
    uart0_init();
    gpio3_init_input();
    gpio2_init_output();

    GPIO25_CTRL = GPIO_FUNC_SIO;
    SIO_OE |= (1u << LED_PIN_25); // output enable for led pin gpio 25

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

        if (flag && f > r)
        {
            uint64_t duration_us = f - r;
            uint64_t distance_cm = duration_us / 58;

            if (distance_cm > 500)
            {
                // max distance
                uart0_puts("TANK FULL\r\n");
            }
            else if (distance_cm <= 20)
            {
                uart0_puts("TANK FULL\r\n");
            }
            else
            {
                uart0_putnum(distance_cm);
                uart0_puts(" cm\r\n");
            }
        }
        else
        {
            uart0_puts("no measurement\r\n");
        }

        delay_ms(50); // wait between triggers
    }
}

void gpio2_init_output(void)
{
    GPIO2_CTRL = GPIO_FUNC_SIO;
    SIO_OE |= (1u << 2);
}

void gpio2_set_high(void)
{
    SIO_OUT |= (1u << 2);
}

void gpio2_set_low(void)
{
    SIO_OUT &= ~(1u << 2);
}

void sr04t_send_trigger(void)
{
    gpio2_set_high();
    delay_us(10);
    gpio2_set_low();
}

void gpio3_init_input(void)
{
    GPIO3_CTRL = GPIO_FUNC_SIO;
    SIO_OE &= ~(1u << 3); // input enable for gpio 3
    PAD_GPIO3_CTRL |= PAD_GPIO_CTRL_IE | PAD_GPIO_CTRL_PDE | PAD_GPIO_CTRL_SCHMITT;
    PROC0_INTE0 |= PROC0_INTE0_GPIO3_EDGE_HIGH_IRQ_EN | PROC0_INTE0_GPIO3_EDGE_LOW_IRQ_EN;

    NVIC_ISER |= NVIC_IO_IRQ_BANK0_EN;
}

void isr_irq13(void)
{
    if (PROC0_INTS0 & (1u << 15))
    {
        PROC0_INTR0 = PROC0_INTR0_GPIO3_HIGH_CLEAR;
        rise_time = read_timer();
        rise_captured = 1;
    }
    else if (PROC0_INTS0 & (1u << 14))
    {
        PROC0_INTR0 = PROC0_INTR0_GPIO3_LOW_CLEAR;
        if (rise_captured)
        {
            fail_time = read_timer();
            ready_flag = 1;
            rise_captured = 0;
        }
    }
    else
    {
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

void uart0_putnum(uint64_t num)
{
    if (num == 0)
    {
        uart0_putc('0');
        return;
    }
    uint8_t buffer[20];
    int index = 0;

    while (num > 0)
    {
        buffer[index] = (num % 10) + '0';
        num /= 10;
        index++;
    }

    for (int i = index - 1; i >= 0; i--)
    {
        uart0_putc(buffer[i]);
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

void delay_us(uint64_t microseconds)
{
    uint64_t start = read_timer();
    uint64_t target_us = microseconds;

    while ((read_timer() - start) < target_us)
    {
        // wait;
    }
}