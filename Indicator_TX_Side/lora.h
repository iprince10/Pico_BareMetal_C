#ifndef lora_h
#define lora_h

void uart1_init(void);
void uart1_putc(char c);
void uart1_puts(const char *send);

void uart0_init(void);
void uart0_putc(char);
void uart0_puts(const char *);

void lora_tx_init(void);
int wait_aux_high(void);

#endif