/*
 * encryption.c
 *
 *  Created on: Dec 21, 2024
 *      Author: wario
 */

#include <string.h>
#include <stdio.h>
#include "mbedtls/aes.h"
#include"AES_User_defines.h"
#include"key_stoarge.h"
void padding_data_for_AES_encryption(unsigned char*original_text,size_t original_text_length,unsigned char * output){
    memcpy(output, original_text, original_text_length);
    memset(output + original_text_length, 0x00, 128 - original_text_length);  //padding
}
// Encryption
void AES_Encryption(size_t padded_message_len ,  unsigned char * padded_clear_message, unsigned char*padded_encrypted_message){
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
	if (mbedtls_aes_setkey_enc(&aes, key, 256) != 0) {
    printf("Failed to set AES encryption key\n");
    return 1;
}

if (mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, padded_message_len, iv, padded_clear_message, padded_encrypted_message) != 0) {
    printf("Encryption failed\n");
    return 1;
}
mbedtls_aes_free(&aes);
}
void AES_Decryption(size_t padded_message_len ,unsigned char * padded_Encrypted_message,unsigned char*padded_decrytped_message){

// Decryption
//   memcpy(iv_copy, iv, AES_BLOCK_SIZE); // Reset IV for decryption
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
if (mbedtls_aes_setkey_dec(&aes, key, 256) != 0) {
    printf("Failed to set AES decryption key\n");
    return 1;
}

if (mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, padded_message_len, iv, padded_Encrypted_message, padded_decrytped_message) != 0) {
    printf("Decryption failed\n");
    return 1;
}
mbedtls_aes_free(&aes);
}
