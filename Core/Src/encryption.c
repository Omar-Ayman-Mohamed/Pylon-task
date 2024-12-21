/*
 * encryption.c
 *
 *  Created on: Dec 21, 2024
 *      Author: wario
 */

#include <string.h>
#include <stdio.h>
#include "mbedtls/aes.h"
#include "mbedtls/gcm.h"
#include"AES_User_defines.h"
#include"key_stoarge.h"
/**
 * function to pad data for AES-CBC encryption , as this format deals with data blocks of 16 bytes
 * if the data is shorter , we will need padding
 * this padding is inspired by PKCS#7 , but i padded the values with zero's instead
 * */
void padding_data_for_AES_encryption(unsigned char*original_text,size_t original_text_length,unsigned char * output){
    memcpy(output, original_text, original_text_length);
    memset(output + original_text_length, 0x00, 128 - original_text_length);  //padding
}
/*
 * function that encrypts the data using AES-CBC encryption ,
 * it takes data in blocks of 16 bytes , so check if padding is needed
 * Parms : padded_message_len length of message to be encrypted after padding
 * padded_clear_message : padded message to be encrypted
 * padded_encrypted_message : output buffer for data after encryption
 *
 *  */
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

int aes_gcm_encrypt(unsigned char * input, size_t input_len,
		 unsigned char * output, unsigned char * tag)
{
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);
    mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, GCM_key, AES_GCM_KEY_SIZE * 8);
    int ret = mbedtls_gcm_crypt_and_tag(&gcm, MBEDTLS_GCM_ENCRYPT, input_len, GCM_iv, AES_GCM_IV_SIZE,
                              aad, aad_len, input, output, TAG_SIZE, tag);
    mbedtls_gcm_free(&gcm);
    return ret;
}
int aes_gcm_decrypt(unsigned char *input, size_t input_len,
		unsigned char *output, unsigned char *tag)
{
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);
    mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, GCM_key, AES_GCM_KEY_SIZE * 8);
    int ret = mbedtls_gcm_auth_decrypt(&gcm, input_len, GCM_iv, AES_GCM_IV_SIZE, aad, aad_len,
                                    tag, TAG_SIZE, input, output);
    mbedtls_gcm_free(&gcm);
    return ret;
}










