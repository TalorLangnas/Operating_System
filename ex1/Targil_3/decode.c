#include "encriptor.h"
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define KEY "defghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abc"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s src_file dst_file\n", argv[0]);
        return 1;
    }

    // Open source file for reading
    FILE *src_file = fopen(argv[1], "r");
    if (src_file == NULL) {
        printf("Error opening source file: %s\n", argv[1]);
        return 1;
    }

    // Open destination file for writing
    FILE *dst_file = fopen(argv[2], "w");
    if (dst_file == NULL) {
        printf("Error opening destination file: %s\n", argv[2]);
        fclose(src_file);
        return 1;
    }

    // Count the characters in the file
    int count = 0;
    int ch;

    while ((ch = fgetc(src_file)) != EOF) {
        count++;
    }
    // Return to the beginning of the file
    fseek(src_file, 0, SEEK_SET);

    // Allocate memory for the reading buffer
    char *input_word = (char *)malloc((count + 1) * sizeof(char));
    if (input_word == NULL) {
        printf("Error allocating memory\n");
        fclose(src_file);
        fclose(dst_file);
        return 1;
    }

    // Allocate memory for the decoding buffer
    char *decoding_word = (char *)malloc((count + 1) * sizeof(char));
    if (decoding_word == NULL) {
        printf("Error allocating memory\n");
        fclose(src_file);
        fclose(dst_file);
        return 1;
    }

    // Read data from the file into the buffer
    size_t read_size = fread(input_word, sizeof(char), count, src_file);
    if (read_size != (size_t)count) {
        printf("Error reading file\n");
        free(input_word);
        fclose(src_file);
        return 1;
    }

    // Add null terminator to make it a valid C string
    input_word[count] = '\0';

    // Close the file
    fclose(src_file);

// Decode the input word:

    // 1. Create the codec:
    void *codec = createCodec(KEY); // assuming that we have the encoding key in advanced.
    if (codec == NULL) {
        printf("Failed to create codec\n");
        return 1;
    }

    // 2. decoding the input word to text:
    int len = decode(input_word, decoding_word, count * sizeof(char), codec);
    if (len < 0) {
        printf("Failed to decode the input word\n");
        return 1;
    }
    // printf("decoded word is: %s\n", decoding_word);
    int input_len = count * sizeof(char);
    printf("input_word is:\n%s\n", input_word);
    printf("decoded word is:\n%s\n", decoding_word);

    // 3. write encoding_word to output file:
    if (fputs(decoding_word, dst_file) != EOF) {
        printf("String successfully written to the file.\n");
    } else {
        perror("Error writing to the file");
    }

    // Close the file
    fclose(dst_file);

    // 4. free the buffers and the codec:
    free(input_word);
    free(decoding_word);
    freecodec(codec);

    return 0;
}
