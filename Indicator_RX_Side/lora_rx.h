#ifndef lora_rx_h
#define lora_rx_h

void uart1_init(void);
char uart1_getc(void);

void uart0_init(void);
void uart0_putc(char);
void uart0_puts(const char *);

void lora_rx_init(void);
int wait_aux_high(void);

#endif