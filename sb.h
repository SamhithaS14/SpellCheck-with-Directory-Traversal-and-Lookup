#ifndef SB_H
#define SB_H


typedef struct {
    char *data;
    unsigned length;
    unsigned capacity;
} strbuf_t;

void sb_init(strbuf_t *B, unsigned size);
void sb_destroy(strbuf_t *B);
void sb_ensure(strbuf_t *B, unsigned size);
void sb_append(strbuf_t *B, char *str);

#endif /* SB_H */
