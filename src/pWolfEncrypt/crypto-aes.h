/* crypto-aes.h
 *  by Scott R. Sideleau <scott@sideleau.com>
 *     29-APR-2017
 *
 * Derived from:
 *  aes-file-encrypt.c
 *
 * Copyright (C) 2006-2015 wolfSSL Inc.
 *
 * This file is part of wolfSSL. (formerly known as CyaSSL)
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef __WOLFSSL__CRYPTO_AES__
#define __WOLFSSL__CRYPTO_AES__

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/random.h>
#include <wolfssl/wolfcrypt/pwdbased.h>

#define SALT_SIZE 8

/*
 * Makes a cyptographically secure key by stretching a user entered key
 */
int GenerateKey(RNG* rng, unsigned char* key, int size, unsigned char* salt, int pad)
{
    int ret;

    ret = wc_RNG_GenerateBlock(rng, salt, SALT_SIZE);
    if (ret != 0)
        return -1020;

    if (pad == 0)
        salt[0] = 0;

    /* stretches key */
    ret = wc_PBKDF2(key, key, strlen((const char*)key), salt, SALT_SIZE, 4096,
      size, SHA256);
    if (ret != 0)
        return -1030;

    return 0;
}

/*
 * Encrypts a file using AES 
 */
int AesEncrypt(Aes* aes, unsigned char* key, int size, FILE* inFile, FILE* outFile)
{
    RNG              rng;
    unsigned char    iv[AES_BLOCK_SIZE];
    unsigned char*   input;
    unsigned char*   output;
    unsigned char    salt[SALT_SIZE] = {0};

    int     i = 0;
    int     ret = 0;
    int     inputLength;
    int     length;
    int     padCounter = 0;

    fseek(inFile, 0, SEEK_END);
    inputLength = ftell(inFile);
    fseek(inFile, 0, SEEK_SET);

    length = inputLength;
    /* pads the length until it evenly matches a block / increases pad number*/
    while (length % AES_BLOCK_SIZE != 0) {
        length++;
        padCounter++;
    }

    input = (unsigned char*)( malloc(length) );
    output = (unsigned char*)( malloc(length) );

    ret = wc_InitRng(&rng);
    if (ret != 0) {
        printf("Failed to initialize random number generator\n");
        return -1030;
    }

    /* reads from inFile and wrties whatever is there to the input array */
    ret = fread(input, 1, inputLength, inFile);
    if (ret == 0) {
        printf("Input file does not exist.\n");
        return -1010;
    }
    for (i = inputLength; i < length; i++) {
        /* padds the added characters with the number of pads */
        input[i] = padCounter;
    }

    ret = wc_RNG_GenerateBlock(&rng, iv, AES_BLOCK_SIZE);
    if (ret != 0)
        return -1020;

    /* stretches key to fit size */
    ret = GenerateKey(&rng, key, size, salt, padCounter);
    if (ret != 0) 
        return -1040;

    /* sets key */
    ret = wc_AesSetKey(aes, key, AES_BLOCK_SIZE, iv, AES_ENCRYPTION);
    if (ret != 0)
        return -1001;

    /* encrypts the message to the ouput based on input length + padding */
    ret = wc_AesCbcEncrypt(aes, output, input, length);
    if (ret != 0)
        return -1005;

    /* writes to outFile */
    fwrite(salt, 1, SALT_SIZE, outFile);
    fwrite(iv, 1, AES_BLOCK_SIZE, outFile);
    fwrite(output, 1, length, outFile);

    /* closes the opened files and frees the memory*/
    memset(input, 0, length);
    memset(output, 0, length);
    //memset(key, 0, size);
    free(input);
    free(output);
    //free(key);
    fclose(inFile);
    fclose(outFile);
    return ret;
}

/*
 * Decrypts a file using AES 
 */
int AesDecrypt(Aes* aes, unsigned char* key, int size, FILE* inFile, FILE* outFile)
{
    RNG              rng;
    unsigned char    iv[AES_BLOCK_SIZE];
    unsigned char*   input;
    unsigned char*   output;
    unsigned char    salt[SALT_SIZE] = {0};

    int     i = 0;
    int     ret = 0;
    int     length;
    int     aSize;

    fseek(inFile, 0, SEEK_END);
    length = ftell(inFile);
    fseek(inFile, 0, SEEK_SET);
    aSize = length;

    input = (unsigned char*)( malloc(aSize) );
    output = (unsigned char*)( malloc(aSize) );

    wc_InitRng(&rng);

    /* reads from inFile and wrties whatever is there to the input array */
    ret = fread(input, 1, length, inFile);
    if (ret == 0) {
        printf("Input file does not exist.\n");
        return -1010;
    }
    for (i = 0; i < SALT_SIZE; i++) {
        /* finds salt from input message */
        salt[i] = input[i];
    }
    for (i = SALT_SIZE; i < AES_BLOCK_SIZE + SALT_SIZE; i++) {
        /* finds iv from input message */
        iv[i - SALT_SIZE] = input[i];
    }

    /* replicates old key if keys match */
    ret = wc_PBKDF2(key, key, strlen((const char*)key), salt, SALT_SIZE, 4096,
      size, SHA256);
    if (ret != 0)
        return -1050;

    /* sets key */
    ret = wc_AesSetKey(aes, key, AES_BLOCK_SIZE, iv, AES_DECRYPTION);
    if (ret != 0)
        return -1002;

    /* change length to remove salt/iv block from being decrypted */
    length -= (AES_BLOCK_SIZE + SALT_SIZE);
    for (i = 0; i < length; i++) {
        /* shifts message: ignores salt/iv on message*/
        input[i] = input[i + (AES_BLOCK_SIZE + SALT_SIZE)];
    }
    /* decrypts the message to output based on input length + padding*/
    ret = wc_AesCbcDecrypt(aes, output, input, length);
    if (ret != 0)
        return -1006;

    if (salt[0] != 0) {
        /* reduces length based on number of padded elements */
        length -= output[length-1];
    }
    /* writes output to the outFile based on shortened length */
    fwrite(output, 1, length, outFile);

    /* closes the opened files and frees the memory*/
    memset(input, 0, aSize);
    memset(output, 0, aSize);
    //memset(key, 0, size);
    free(input);
    free(output);
    //free(key);
    fclose(inFile);
    fclose(outFile);

    return 0;
}

int SizeCheck(int size)
{
    int ret = 0;

    if (size != 128 && size != 192 && size != 256) {
        /* if the entered size does not match acceptable size */
        printf("Invalid AES key size\n");
        ret = -1080;
    }

    return ret;
}

#endif //__WOLFSSL__CRYPTO_AES__
