#ifndef lora_tx_h
#define lora_tx_h

void uart0_init(void);
void uart0_putc(char);
void uart0_puts(const char *);
void uart0_putnum(uint64_t num);

void uart1_init(void);
void uart1_putc(char c);
void uart1_puts(const char *send);
void uart1_write_bytes(const uint8_t *data, uint8_t len);
char uart1_getc(void);
int uart1_has_data(void);

void lora_tx_init(void);
int wait_aux_high(void);
void set_param_config_tx(void);
void check_config_rx(void);

#endif