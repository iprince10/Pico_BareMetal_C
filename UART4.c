#include <stdint.h>

#define LED_PIN 25u
#define GPIO_FUNC_SIO 5u
#define SIO_GPIO25_CTRL (*(volatile uint32_t *)(0x400140CC))
#define SIO_GPIO25_BASE 0xd0000000u
#define SIO_GPIO25_OE (*(volatile uint32_t *)(SIO_GPIO25_BASE + 0x20))
#define SIO_GPIO25_OUT (*(volatile uint32_t *)(SIO_GPIO25_BASE + 0x10))

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
#define UARTIMSC (*(volatile uint32_t *)(UART1_BASE + 0x038))
#define UARTIMSC_RXIM (1u << 4)

#define ARM_CORTEX_M0_BASE (0xe0000000u)
#define NVIC_ISER (*(volatile uint32_t *)(ARM_CORTEX_M0_BASE + 0xe100))
#define NVIC_ISER_UART1_IRQ (1u << 21)

typedef struct
{
    char buffer[16];
    uint8_t head;
    uint8_t tail;
} RingBuffer;

int ring_buffer_put(RingBuffer *, char);
int ring_buffer_get(RingBuffer *, char *);


int main(void)
{
    RingBuffer uart1_rx_buffer = {0};
    char c = 'a';
    char out[16];
    ring_buffer_put(&uart1_rx_buffer, c);
    ring_buffer_get(&uart1_rx_buffer, &out);
}

// writing a byte to the buffer
int ring_buffer_put(RingBuffer *rb, char c)
{
    uint8_t next_head = (rb->head + 1) % 16;

    if (next_head == (rb->tail))
    {
        return 0;
    }

    rb->buffer[rb->head] = c;
    rb->head = next_head;
    return 1;
}

int ring_buffer_get(RingBuffer *rb, char *out)
{
    if (rb->tail == rb->head)
    {
        return 0;
    }

    *out = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % 16;
    return 1;
}
