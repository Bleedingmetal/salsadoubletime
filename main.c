/*

CS4801/ECE4802
Team E1
Jacob Boyle
Aditya Krishna
Olivia Olsen
Alan Wang

*/


#include <stdio.h>
#include <stdlib.h>
#include "ecrypt-sync.h"
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#define WORDS (4096 * 128)
#define KEYSIZE (256)
#define IVSIZE (64)
#define TRIALS (1024)

#define KEYBYTES (KEYSIZE / 8)
#define IVBYTES (IVSIZE / 8)
#define BYTES (WORDS * 4)

float getms(int sec, int usec) {
    return (sec * 1000) + (usec / 1000.0);
}

int main() {

    float totaltime = 0.0;

    ECRYPT_ctx ctx;
    uint8_t key[KEYBYTES];
    uint8_t iv[IVBYTES];
    uint32_t words[WORDS];
    uint8_t cyphertext[BYTES];
    uint8_t newplaintext[BYTES];

    srand(time(NULL));

    for (int t = 0; t < TRIALS; t++) {

        for (uint32_t i = 0; i < KEYBYTES; i++) {
            key[i] = (uint8_t)(rand() & 0xFF);
            // printf("%02X", key[i]);
        }

        for (uint32_t i = 0; i < WORDS; i++) {
            words[i] = (uint32_t)rand();
        }
        uint8_t* plaintext = (uint8_t*) words;

        ECRYPT_keysetup(&ctx, key, KEYBYTES, IVBYTES);

        struct timeval starttime;
        gettimeofday(&starttime, NULL);

        ECRYPT_ivsetup(&ctx, iv);
        ECRYPT_encrypt_bytes(&ctx, plaintext, cyphertext, BYTES);
        
        ECRYPT_ivsetup(&ctx, iv);
        ECRYPT_decrypt_bytes(&ctx, cyphertext, newplaintext, BYTES);

        struct timeval endtime;
        gettimeofday(&endtime, NULL);

        totaltime += getms(endtime.tv_sec - starttime.tv_sec, endtime.tv_usec - starttime.tv_usec);
        // printf("Time: %fms\n", getms(endtime.tv_sec - starttime.tv_sec, endtime.tv_usec - starttime.tv_usec));

        uint8_t good = 1;

        for (int i = 0; i < 10; i++) {
            if (plaintext[i] != newplaintext[i]) {
                good = 0;
                break;
            }
        }

        if (!good) {
            puts("Error!");
            break;
        }
    }

    printf("Total time: %fms\n", totaltime);
}
