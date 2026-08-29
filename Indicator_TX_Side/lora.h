#ifndef lora_h
#define lora_h

void uart1_init(void);
void uart1_putc(char c);
void uart1_puts(const char *send);
void uart1_write_bytes(const uint8_t *data, uint8_t len);
char uart1_getc(void);

void lora_tx_init(void);
int wait_aux_high(void);
void set_param_config_tx(void);
void check_config_tx(void);

#endif