#include "encriptor.h"
#include <stdio.h>
#include <stdlib.h>

//  ALPHABET_SIZE = 62, defined in polyalphabetic.h file

void* createCodec(char key[ALPHABET_SIZE]) {
    // Check for key validity
    int i, j;
    for (i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (j = i + 1; j < ALPHABET_SIZE; j++) {
            if (key[i] == key[j]) {
                // Invalid key, characters mapped to the same value
                printf("Invalid key, characters mapped to the same value");
                return NULL;
            }
        }
    }

    // Key is valid, create the codec
    PolyAlphabeticCodec* codec = (PolyAlphabeticCodec*)malloc(sizeof(PolyAlphabeticCodec));
    if (codec == NULL) {
        // Memory allocation failure
        return NULL;
    }

    // Copy key to the codec structure
    for (i = 0; i < ALPHABET_SIZE; i++) {
        codec->my_key[i] = key[i];
        codec->template[i] = TEMPLATE[i];
    }    

    return codec;
}

// encoding one char 
char encode_char(char input, int len, void* codec)
{       
    // Cast the void pointer to the correct type
    PolyAlphabeticCodec* polyCodec = (PolyAlphabeticCodec*)codec;
    int index = 0;
    while (input != polyCodec->template[index])
    {
        index++;
    }
    // if the char is not in the key, return the char as is.
    if(index >= ALPHABET_SIZE)
    {
        return input;
    }
    return polyCodec->my_key[index];

}

int encode(char* textin, char* textout, int len, void* codec)
{   
    if(textin == NULL || len <= 0)
    {
        printf("textin is NULL or len is not positive\n");
        return -1;
    }
    // Cast the void pointer to the correct type
    PolyAlphabeticCodec* polyCodec = (PolyAlphabeticCodec*)codec;

    for(int i = 0; i < len; i++)
    {
        char curr = encode_char(textin[i], len, codec);
        textout[i] = curr;
    }
    
    return len + 1; // return number of bytes include \0 (each char is 1 byte). 
}

// decoding one char 
char decode_char(char input, int len, void* codec)
{
    // Cast the void pointer to the correct type
    PolyAlphabeticCodec* polyCodec = (PolyAlphabeticCodec*)codec;
    int index = 0;
    while (input != polyCodec->my_key[index])
    {
        index++;
    }
    
    // if the char is not in the key, return the char as is.
    if(index >= ALPHABET_SIZE)
    {
        return input;
    }
    
    return polyCodec->template[index];
}

int decode(char* textin, char* textout, int len, void* codec)
{   
    if(textin == NULL || len <= 0)
    {
        printf("textin is NULL or len is not positive\n");
        return -1;
    }
    // Cast the void pointer to the correct type
    PolyAlphabeticCodec* polyCodec = (PolyAlphabeticCodec*)codec;

    for(int i = 0; i < len; i++)
    {
        char curr = decode_char(textin[i], len, codec);
        textout[i] = curr; 
    }

    return len; // return number of bytes include (each char is 1 byte). 
};

void freecodec(void* codec)
{
    // Cast the void pointer to PolyAlphabeticCodec* before freeing
    PolyAlphabeticCodec* polyCodec = (PolyAlphabeticCodec*)codec;

    // Free the allocated memory
    free(polyCodec);
}