#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

// Format the data as a hexadecimal string. The buffer must have
// space for `2 * length + 1` characters.
const char *hexString(unsigned char *data, size_t length, char *buffer) {
    const char *hexDigits = "0123456789abcdef";
    char *dest = buffer;
    for (size_t i = 0; i < length; i++) {
        *dest++ = hexDigits[data[i] >> 4];
        *dest++ = hexDigits[data[i] & 0x0F];
    }
    *dest = 0;
    return buffer;
}

int main(void) {
    FILE *infile = fopen("file.txt", "r");
    if (infile == NULL) {
        perror("Cannot open input file");
        exit(EXIT_FAILURE);
    }
    FILE *outfile = fopen("MD.txt","w");
    if (outfile == NULL) {
        perror("Cannot open output file");
        exit(EXIT_FAILURE);
    }

    // Read file line-by-line
    char *line = NULL;
    size_t linecap = 0;
    ssize_t lineLength;
    while ((lineLength = getline(&line, &linecap, infile)) != -1) {
        if (lineLength > 0 && line[lineLength - 1] == '\n') {
            // Remove newline character
            lineLength -= 1;
            line[lineLength] = 0;
        }

        // Compute SHA384 hash
        unsigned char sha384hash[SHA384_DIGEST_LENGTH];
        SHA384((unsigned char*)line, lineLength, sha384hash);

        // Print hash as hex string
        char hexBuffer[2 * SHA384_DIGEST_LENGTH + 1];
        fputs(hexString(sha384hash, SHA384_DIGEST_LENGTH, hexBuffer), outfile);
        fputc('\n', outfile);
    }
    free(line);

    // Close output files
    fclose(infile);
    fclose(outfile);
}
