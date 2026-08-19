#include <stdint.h>
#include <bitmap.h>
#include <timer.h>

#define SIO_BASE 0xd0000000u
#define SIO_GPIO_OE (*(volatile uint32_t *)(SIO_BASE + 0x020))
#define SIO_GPIO_OE_SET (*(volatile uint32_t *)(SIO_BASE + 0x024))
#define SIO_GPIO_OE_CLEAR (*(volatile uint32_t *)(SIO_BASE + 0x028))
#define SIO_GPIO_OUT (*(volatile uint32_t *)(SIO_BASE + 0x010))
#define SIO_GPIO_OUT_SET (*(volatile uint32_t *)(SIO_BASE + 0x014))
#define SIO_GPIO_OUT_CLEAR (*(volatile uint32_t *)(SIO_BASE + 0x018))

#define LED_PIN_25 25u
#define GPIO25_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x0cc))

#define IO_BANK0_BASE 0x40014000u
#define GPIO19_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x09c)) // spio TX
#define GPIO16_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x084)) // spio RX
#define GPIO17_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x08c)) // spio CS
#define GPIO18_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x094)) // spio CLK
#define GPIO20_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x0a4)) // spio C/D
#define GPIO21_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x0ac)) // spio Hardware Reset
#define GPIO_FUNC_SPI0 (1u)
#define GPIO_FUNC_SIO (5u)
#define GPIO2_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x014))
#define GPIO3_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x01c))

#define PADS_BANK0_BASE 0x4001c000u
#define PAD_GPIO19 (*(volatile uint32_t *)(PADS_BANK0_BASE + 0x50))
#define PAD_GPIO16 (*(volatile uint32_t *)(PADS_BANK0_BASE + 0x44))
#define PAD_GPIO17 (*(volatile uint32_t *)(PADS_BANK0_BASE + 0x48))
#define PAD_GPIO18 (*(volatile uint32_t *)(PADS_BANK0_BASE + 0x4c))
#define PAD_GPIO20 (*(volatile uint32_t *)(PADS_BANK0_BASE + 0x54))
#define PAD_GPIO21 (*(volatile uint32_t *)(PADS_BANK0_BASE + 0x58))
#define PAD_GPIO3_CTRL (*(volatile uint32_t *)(PADS_BANK0_BASE + 0x10))
#define PAD_GPIO2_CTRL (*(volatile uint32_t *)(PADS_BANK0_BASE + 0x0c))
#define PAD_GPIO_CTRL_IE (1u << 6)
#define PAD_GPIO_CTRL_PDE (1u << 2)
#define PAD_GPIO_CTRL_SCHMITT (1u << 1)

#define SPI0_BASE 0x4003c000u
#define SPI0_SSPCR0 (*(volatile uint32_t *)(SPI0_BASE + 0x000))
#define SPI0_SSPCR1 (*(volatile uint32_t *)(SPI0_BASE + 0x004))
#define SPI0_SSPCR1_SSE (1u << 1)
#define SPI0_SSPDR (*(volatile uint32_t *)(SPI0_BASE + 0x008))
#define SPI0_SSPSR (*(volatile uint32_t *)(SPI0_BASE + 0x00c))
#define SPI0_SSPSR_BSY (1u << 4)
#define SPI0_SSPSR_TNF (1u << 1)
#define SPI0_SSPCPSR (*(volatile uint32_t *)(SPI0_BASE + 0x010))

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

#define CLOCKS_BASE 0x40008000u
#define CLK_PERI_CTRL (*(volatile uint32_t *)(CLOCKS_BASE + 0x48))
#define CLK_PERI_CTRL_ENABLE (1u << 11)

#define RESETS_BASE 0x4000c000u
#define RESETS_RESET (*(volatile uint32_t *)(RESETS_BASE + 0x0))
#define RESETS_RESET_SPI0 (1u << 16)
#define RESETS_RESET_DONE (*(volatile uint32_t *)(RESETS_BASE + 0x8))

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

void gpio3_init_input(void); // trigger

void gpio2_init_output(void); // echo
void gpio2_set_high(void);
void gpio2_set_low(void);
void sr04t_send_trigger(void);

void isr_irq13(void);

volatile uint8_t rise_captured = 0;
volatile uint64_t rise_time = 0;
volatile uint64_t fail_time = 0;
volatile uint8_t ready_flag = 0;

void ili9341_init();
void spi0_init();
void ili9341_hw_reset();
void spi_write_byte(uint8_t byte);
void ili9341_write_command(uint8_t cmd);
void ili9341_write_data(uint8_t data);
void ili9341_send_init_commands();
void ili9341_fill_white(void);
void ili9341_draw_char(uint16_t x, uint16_t y, char c, uint16_t fg_color, uint16_t bg_color, uint8_t scale);
void ili9341_draw_string(uint16_t x, uint16_t y, const char *str, uint16_t fg_color, uint16_t bg_color, uint8_t scale);

int main(void)
{
    ili9341_init();
    ili9341_fill_white();
    uart0_init();
    gpio3_init_input();
    gpio2_init_output();

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
    SIO_GPIO_OE |= (1u << 2);
}

void gpio2_set_high(void)
{
    SIO_GPIO_OUT |= (1u << 2);
}

void gpio2_set_low(void)
{
    SIO_GPIO_OUT &= ~(1u << 2);
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
    SIO_GPIO_OE &= ~(1u << 3); // input enable for gpio 3
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

void ili9341_init()
{
    PAD_GPIO16 = 0x63;
    PAD_GPIO17 = 0x23;
    PAD_GPIO18 = 0x23;
    PAD_GPIO19 = 0x23;
    PAD_GPIO20 = 0x23;
    PAD_GPIO21 = 0x23;

    GPIO16_CTRL = GPIO_FUNC_SPI0; // spi0 rx miso
    GPIO17_CTRL = GPIO_FUNC_SPI0; // spi0 csn
    GPIO18_CTRL = GPIO_FUNC_SPI0; // spi0 sck
    GPIO19_CTRL = GPIO_FUNC_SPI0; // spi0 tx mosi
    GPIO20_CTRL = GPIO_FUNC_SIO;  // sio d/c pin
    GPIO21_CTRL = GPIO_FUNC_SIO;  // sio hardware reset pin

    SIO_GPIO_OE_SET = (1u << 20) | (1u << 21); // enable output for gpio 20 nd 21
    SIO_GPIO_OUT_SET = (1u << 21);             // rst high
    SIO_GPIO_OUT_CLEAR = (1u << 20);           // clear d/c pin

    spi0_init();

    ili9341_hw_reset();

    ili9341_send_init_commands();
}

void spi0_init(void)
{
    CLK_PERI_CTRL |= CLK_PERI_CTRL_ENABLE;
    RESETS_RESET &= ~(RESETS_RESET_SPI0);
    while (!(RESETS_RESET_DONE & RESETS_RESET_SPI0))
    {
    };

    SPI0_SSPCPSR = 2;
    SPI0_SSPCR0 = 0x0507;
    SPI0_SSPCR1 = SPI0_SSPCR1_SSE;
    delay_ms(100);
}

void ili9341_hw_reset()
{
    SIO_GPIO_OUT_CLEAR = (1u << 21);
    delay_ms(1);
    SIO_GPIO_OUT_SET = (1u << 21);
    delay_ms(120);
}

void spi_write_byte(uint8_t byte)
{
    while (!(SPI0_SSPSR & SPI0_SSPSR_TNF))
    {
    }

    SPI0_SSPDR = byte;
    while (SPI0_SSPSR & SPI0_SSPSR_BSY)
    {
    }
    volatile uint32_t dummy = SPI0_SSPDR;
    (void)dummy;
}

void ili9341_write_command(uint8_t cmd)
{
    SIO_GPIO_OUT_CLEAR = (1u << 20); // DC = low command is sending
    spi_write_byte(cmd);
}

void ili9341_write_data(uint8_t data)
{
    SIO_GPIO_OUT_SET = (1u << 20); // DC = high data is sending
    spi_write_byte(data);
}

void ili9341_send_init_commands(void)
{
    ili9341_write_command(0x11); // exit sleep mode
    delay_ms(120);

    ili9341_write_command(0x3A); // pixel fomart to rgb565
    ili9341_write_data(0x55);

    ili9341_write_command(0x36); // memory access orientation
    ili9341_write_data(0x48);

    ili9341_write_command(0x29); // turn display on
    delay_ms(100);
}

void ili9341_fill_white(void)
{
    ili9341_write_command(0x2A); // column address set
    ili9341_write_data(0x00);    // 0
    ili9341_write_data(0x00);
    ili9341_write_data(0x00);
    ili9341_write_data(0xEF); // 239

    ili9341_write_command(0x2B); // row address set
    ili9341_write_data(0x00);    // 0
    ili9341_write_data(0x00);
    ili9341_write_data(0x01);
    ili9341_write_data(0x3F); // 319

    ili9341_write_command(0x2C); // memory write

    for (uint32_t i = 0; i < 76800; i++)
    {
        ili9341_write_data(0xFF);
        ili9341_write_data(0xFF);
    }
}

void ili9341_draw_char(uint16_t x, uint16_t y, char c, uint16_t fg_color, uint16_t bg_color, uint8_t scale)
{
    int index;
    if (c >= '0' && c <= '9')
    {
        index = c - '0';
    }
    else if (c >= 'A' && c <= 'Z')
    {
        index = c - 'A' + 10;
    }
    else if (c >= 'a' && c <= 'z')
    {
        index = c - 'a' + 36;
    }
    else if (c == ',')
    {
        index = 62;
    }
    else if (c == '.')
    {
        index = 63;
    }
    else if (c == ':')
    {
        index = 64;
    }
    else if (c == ';')
    {
        index = 65;
    }
    else if (c == '"')
    {
        index = 66;
    }
    else if (c == '/')
    {
        index = 67;
    }
    else if (c == '\\')
    {
        index = 68;
    }
    else if (c == '|')
    {
        index = 69;
    }
    else if (c == '(')
    {
        index = 70;
    }
    else if (c == ')')
    {
        index = 71;
    }
    else
    {
        return;
    }

    uint16_t width = 10 * scale;
    uint16_t height = 14 * scale;

    // column write
    ili9341_write_command(0x2A);
    ili9341_write_data(x >> 8);
    ili9341_write_data((x & 0xff));
    ili9341_write_data((x + width - 1) >> 8);
    ili9341_write_data((x + width - 1) & 0xff);

    // row write
    ili9341_write_command(0x2B);
    ili9341_write_data(y >> 8);
    ili9341_write_data((y & 0xff));
    ili9341_write_data((y + height - 1) >> 8);
    ili9341_write_data((y + height - 1) & 0xff);

    // memory write
    ili9341_write_command(0x2C);

    for (int row = 0; row < height; row++)
    {
        uint16_t bitmap_row = row / scale;
        uint16_t bitmap = font_10x14[index][bitmap_row];
        for (int col = 0; col < width; col++)
        {
            uint16_t bitmap_col = col / scale;
            if (bitmap & (1u << (9 - bitmap_col)))
            {
                ili9341_write_data(fg_color >> 8);
                ili9341_write_data(fg_color & 0xFF);
            }
            else
            {
                ili9341_write_data(bg_color >> 8);
                ili9341_write_data(bg_color & 0xFF);
            }
        }
    }
}

void ili9341_draw_string(uint16_t x, uint16_t y, const char *str, uint16_t fg_color, uint16_t bg_color, uint8_t scale)
{
    uint16_t cursor_x = x;
    uint16_t cursor_y = y;
    uint16_t char_width = 10 * scale;
    uint16_t char_height = 14 * scale;
    uint16_t x_spacing = 6;
    uint16_t y_spacing = 4;

    while (*str)
    {
        if (*str == '\n')
        {
            cursor_x = x;
            cursor_y += char_height + y_spacing;
        }
        else
        {
            ili9341_draw_char(cursor_x, cursor_y, *str, fg_color, bg_color, scale);
            cursor_x += char_width + x_spacing;
        }
        str++;
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