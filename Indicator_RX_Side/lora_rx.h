#ifndef lora_rx_h
#define lora_rx_h

void uart1_init(void);
void uart1_putc(char c);
char uart1_getc(void);

void uart0_init(void);
void uart0_putc(char);
void uart0_puts(const char *);

void lora_rx_init(void);
int wait_aux_high(void);
int uart1_has_data(void);
void set_param_config_rx(void);
void check_config_rx(void);

#endif