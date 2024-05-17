#ifndef __POLYALPHABETIC__
#define __POLYALPHABETIC__

#define ALPHABET_SIZE 62
#define TEMPLATE "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

typedef struct {
    char my_key[ALPHABET_SIZE];
    char template[ALPHABET_SIZE];
} PolyAlphabeticCodec;

void * createCodec (char key[ALPHABET_SIZE]);
int encode(char* textin, char* textout, int len, void* codec);
int decode(char* textin, char* textout, int len, void* codec);
void freecodec(void* codec); 
char encode_char(char input, int len, void* codec);
char decode_char(char input, int len, void* codec);

#endif
