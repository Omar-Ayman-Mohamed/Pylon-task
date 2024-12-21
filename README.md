# Pylon-task

-implemented different encryption methods using Mbed TLS library to accomdate different secinarios for example :
##
1-Sending log data or messagaes via UART using secure communication 

2-Saving encrypted data on external memory


-The main encryption used here is AES-CBC and AES-GMC 
###
To run code download stmide , i used st-linkv2 for debugging but proteus would work fine if you have access to version 8.13
download stmcubemx if you want to edit code confguirations or enable different perphirals 
# AES-CBC encryption :
  -encrypts and decrypts data using a key , key must be secured in a safe place
  -suitable for legacy systems and low resorces systems , consider using it with hashing to ensure data intergrity
  ###
  functions :
  ``` C
  void padding_data_for_AES_CBC_encryption(unsigned char*original_text,size_t original_text_length,unsigned char * output){
    memcpy(output, original_text, original_text_length);
    memset(output + original_text_length, 0x00, 128 - original_text_length);  //padding
}
```
Here the AES frame works with data of 16 byte blocks , so we must ensure that the data is padded before encryption 

```C
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
mbedtls_aes_free(&aes);
}
```
The function uses aes_crypt_cbc api for encryption , it returns 0 if encryption is sucessful
for more info : https://mbed-tls.readthedocs.io/en/latest/kb/how-to/encrypt-with-aes-cbc/

```C
void AES_Decryption(unsigned char *key,unsigned char *iv,size_t padded_message_len ,unsigned char * padded_Encrypted_message,unsigned char*padded_decrytped_message){

// Decryption
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
```
same as before used to decrypt data
###
we can use this encryption for low resource systems or for simple implemntation, however it doesn't validate data integrity that's why we will discuss 
AES-GCM next
####
# AES-GCM
  implemnts encryption and data integrity check by introducing a tag , if it mismatches no decrption occur
# Code 
```C
int aes_gcm_encrypt(unsigned char *key,size_t KEY_SIZE,unsigned char *iv,size_t IV_SIZE,unsigned char * input, size_t input_len,
		 unsigned char * output,unsigned char *aad,size_t aad_len, unsigned char * tag)
{
    mbedtls_gcm_context gcm;
    mbedtls_gcm_init(&gcm);
    mbedtls_gcm_setkey(&gcm, MBEDTLS_CIPHER_ID_AES, key, KEY_SIZE * 8);
    int ret = mbedtls_gcm_crypt_and_tag(&gcm, MBEDTLS_GCM_ENCRYPT, input_len, iv, IV_SIZE,
                              aad, aad_len, input, output, TAG_SIZE, tag);
    mbedtls_gcm_free(&gcm);
    return ret;
}
```
the main differenc is we have a tag and AAD which stands for additional authenticated data 
tag Ensures integrity and authenticity of both ciphertext and AAD , while AAD Authenticates associated plaintext data that is not encrypted but still needs integrity checks.
 ```C
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
````
Decryption function 
######
# Random generator 
for extra security , we can create random keys and iv's to ensure safety , but this comes at the cost that these data if encrypted and the mcu reset before decryption , another key will be generated causing this data to be lost , so it's best used for quick one session communications 
```C
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
```
here we utilize the Mbed TLS library to genrate random data buffers that can be used for keys or iv's adding another layer of security for your application

