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
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include"AES_User_defines.h"

/**
 * function to pad data for AES-CBC encryption , as this format deals with data blocks of 16 bytes
 * if the data is shorter , we will need padding
 * this padding is inspired by PKCS#7 , but i padded the values with zero's instead
 * */
void padding_data_for_AES_CBC_encryption(unsigned char*original_text,size_t original_text_length,unsigned char * output){
    memcpy(output, original_text, original_text_length);
    memset(output + original_text_length, 0x00, 128 - original_text_length);  //padding
}
/*
 * function that encrypts the data using AES-CBC encryption ,
 * it takes data in blocks of 16 bytes , so check if padding is needed
 * Parms :
 * key:provide key for encryption can be saved in secure place or randomly generated
 * iv: intilization vector for encryption can be saved in secure place or randomly generated
 *  padded_message_len length of message to be encrypted after padding
 * padded_clear_message : padded message to be encrypted
 * padded_encrypted_message : output buffer for data after encryption
 *
 *  */
void AES_Encryption(unsigned char *key,unsigned char *iv,size_t padded_message_len ,  unsigned char * padded_clear_message, unsigned char*padded_encrypted_message){
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
/*always make sure to free resources after finishing*/
mbedtls_aes_free(&aes);
}
/*
 * function that decrypts the data using AES-CBC encryption ,
 * it takes data in blocks of 16 bytes , so check if padding is needed
 * Parms :
 * key:provide key for encryption can be saved in secure place or randomly generated
 * iv: intilization vector for encryption can be saved in secure place or randomly generated
 *  padded_message_len length of message to be encrypted after padding
 * padded_clear_message : padded message to be encrypted
 * padded_encrypted_message : output buffer for data after encryption
 *
 *  */
void AES_Decryption(unsigned char *key,unsigned char *iv,size_t padded_message_len ,unsigned char * padded_Encrypted_message,unsigned char*padded_decrytped_message){
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
/*always make sure to free resources after finishing*/
mbedtls_aes_free(&aes);
}

/*
 * function that encrypts the data using AES-GCM encryption ,
 * it takes addtional parameters as tag and aad for data integrity checks
 * Parms :
 * key:provide key for encryption can be saved in secure place or randomly generated
 * KEY_SIZE : size of supplied key
 * iv: intilization vector for encryption can be saved in secure place or randomly generated
 * IV_SIZE : size of supplied iv
 * input : data to be encrypted
 * output : output buffer
 * aad : addtive authentication data
 * tag : for data integurity check
 *  */

int aes_gcm_encrypt(unsigned char *key,size_t KEY_SIZE,unsigned char *iv,size_t IV_SIZE,unsigned char * input, size_t input_len,
		 unsigned char * output,unsigned char *aad,size_t aad_len, unsigned char * tag)
{
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);
    mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, key, KEY_SIZE * 8);
    int ret = mbedtls_gcm_crypt_and_tag(&gcm, MBEDTLS_GCM_ENCRYPT, input_len, iv, IV_SIZE,
                              aad, aad_len, input, output, TAG_SIZE, tag);
    /*always make sure to free resources after finishing*/
    mbedtls_gcm_free(&gcm);
    return ret;
}
/*
 * function that decrypts the data using AES-GCM encryption ,
 * it takes addtional parameters as tag and aad for data integrity checks
 * Parms :
 * key:provide key for encryption can be saved in secure place or randomly generated
 * KEY_SIZE : size of supplied key
 * iv: intilization vector for encryption can be saved in secure place or randomly generated
 * IV_SIZE : size of supplied iv
 * input : data to be encrypted
 * output : output buffer
 * aad : addtive authentication data
 * tag : for data integurity check
 *  */
int aes_gcm_decrypt(unsigned char *key,size_t KEY_SIZE,unsigned char *iv,size_t IV_SIZE,unsigned char *input, size_t input_len,
		unsigned char *output,unsigned char *aad,size_t aad_len ,unsigned char *tag)
{
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);
    mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, key, KEY_SIZE * 8);
    int ret = mbedtls_gcm_auth_decrypt(&gcm, input_len, iv,IV_SIZE, aad, aad_len,
                                    tag, TAG_SIZE, input, output);
    mbedtls_gcm_free(&gcm);
    return ret;
}
/**
 * function that generates random data for generation of random keys and ivs for addtional securtiy
 * best used for tempory communication that is needed for only one session as the key and iv are different
 * each time the program runs
 * parms :
 * buffer : input container for random data to be generated
 * personlaized text : any string to ensure randomnnes of generated data
 *  personalized_text_len : length of personalized string
 *
 *
 * */

void generate_random(unsigned char *buffer, size_t length,unsigned char*personalized_text,size_t personalized_text_len) {
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    /*intilization*/
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    /*seed generation*/
    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, personalized_text, personalized_text_len);
   /*random data generation*/
    mbedtls_ctr_drbg_random(&ctr_drbg, buffer, length);
    /*freeing resources after finishing*/
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
}







