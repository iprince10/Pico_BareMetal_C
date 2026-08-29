#include <stdint.h>
#include <timer.h>

#define IO_BANK0_BASE 0x40014000u
#define GPIO0_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x004))
#define GPIO4_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x024))
#define GPIO6_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x034))
#define GPIO7_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x03c))
#define GPIO8_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x044))

#define SIO_BASE 0xd0000000u
#define SIO_FUNC 5u
#define SIO_GPIO_IN (*(volatile uint32_t *)(SIO_BASE + 0x004))
#define SIO_GPIO_OE (*(volatile uint32_t *)(SIO_BASE + 0x020))
#define SIO_GPIO_OE_SET (*(volatile uint32_t *)(SIO_BASE + 0x024))
#define SIO_GPIO_OE_CLEAR (*(volatile uint32_t *)(SIO_BASE + 0x028))
#define SIO_GPIO_OUT (*(volatile uint32_t *)(SIO_BASE + 0x010))
#define SIO_GPIO_OUT_SET (*(volatile uint32_t *)(SIO_BASE + 0x014))
#define SIO_GPIO_OUT_CLEAR (*(volatile uint32_t *)(SIO_BASE + 0x018))

#define PAD_BANK0_BASE 0x4001c000u
#define PAD_GPIO8_CTRL (*(volatile uint32_t *)(PAD_BANK0_BASE + 0x24))
#define PAD_GPIO_CTRL_IE (1u << 6)

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
#define UART1_CR_RXE_EN (1u << 9)
#define UART1_LCR_H (*(volatile uint32_t *)(UART1_BASE + 0x02c))
#define UART1_LCR_H_FEN (1u << 4)
#define UART1_LCR_H_WLEN (3u << 5)

void uart1_init(void);
void uart1_putc(char c);
void uart1_puts(const char *send);
void uart1_write_bytes(const uint8_t *data, uint8_t len);

void lora_tx_init(void);
int wait_aux_high(void);
void set_param_config_tx(void);

void lora_tx_init(void)
{
    GPIO6_CTRL = SIO_FUNC;
    GPIO7_CTRL = SIO_FUNC;
    GPIO8_CTRL = SIO_FUNC;

    SIO_GPIO_OE_SET = (1u << 6) | (1u << 7);
    SIO_GPIO_OUT_CLEAR = (1u << 6) | (1u << 7);

    SIO_GPIO_OE_CLEAR = (1u << 8); // input enable for gpio 8
    PAD_GPIO8_CTRL |= PAD_GPIO_CTRL_IE;
}

int wait_aux_high(void)
{
    if (SIO_GPIO_IN & (1 << 8))
    {
        return 1;
    }
    return 0;
}

void uart1_init(void)
{
    UART1_CR = 0;
    GPIO4_CTRL = UART1_FUNC;

    UART1_IBRD = 813; // for 9600 baud rate
    UART1_FBRD = 51;

    UART1_LCR_H = UART1_LCR_H_FEN | UART1_LCR_H_WLEN; // 8N1 8data, No parity, 1 stop bit, enable fifo

    UART1_CR = UART1_CR_UARTEN | UART1_CR_TXE_EN;
}

void uart1_putc(char c)
{
    while ((UART1_FR & UART1_FR_TXFF) != 0)
    {
        // wait while buffer full
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

void uart1_write_bytes(const uint8_t *data, uint8_t len)
{
    for (int i = 0; i < len; i++)
    {
        uart1_putc((char)data[i]);
    }
}

void set_param_config_tx(void)
{
    SIO_GPIO_OUT_SET = (1u << 6) | (1u << 7);
    delay_ms(50);
    uint8_t packet[6] = {0xc0, 0x00, 0x00, 0x1a, 0x17, 0xc4};

    for (int i = 0; i < 6; i++)
    {
        uart1_putc(packet[i]);
    }
    delay_ms(50);
    SIO_GPIO_OUT_CLEAR = (1u << 6) | (1u << 7);
}