#include <stdint.h>

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

int ring_buffer_get(RingBuffer *rb, char *out){
    if(rb->tail == rb->head){
        return 0;
    }

    *out = rb->buffer[rb->tail];
    rb-> tail = (rb-> tail + 1)% 16;
    return 1;
}