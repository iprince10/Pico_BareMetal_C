#ifndef lora_h
#define lora_h

void uart1_init(void);
void uart1_putc(char c);
void uart1_puts(const char *send);
void uart1_write_bytes(const uint8_t *data, uint8_t len);

void lora_tx_init(void);
int wait_aux_high(void);

#endif