/*
 * encrption_header.h
 *
 *  Created on: Dec 21, 2024
 *      Author: wario
 */

#ifndef INC_ENCRPTION_HEADER_H_
#define INC_ENCRPTION_HEADER_H_




void AES_Encryption(unsigned char *key,unsigned char *iv,size_t padded_message_len ,  unsigned char * padded_clear_message, unsigned char*padded_encrypted_message);
void AES_Decryption(unsigned char *key,unsigned char *iv,size_t padded_message_len ,unsigned char * padded_Encrypted_message,unsigned char*padded_decrytped_message);
int aes_gcm_encrypt(unsigned char *key,size_t KEY_SIZE,unsigned char *iv,size_t IV_SIZE,unsigned char * input, size_t input_len,
		 unsigned char * output,unsigned char *aad,size_t aad_len, unsigned char * tag);
int aes_gcm_decrypt(unsigned char *key,size_t KEY_SIZE,unsigned char *iv,size_t IV_SIZE,unsigned char *input, size_t input_len,
		unsigned char *output,unsigned char *aad,size_t aad_len ,unsigned char *tag);
void generate_random(unsigned char *buffer, size_t length,unsigned char*personalized_text,size_t personalized_text_len);
#endif /* INC_ENCRPTION_HEADER_H_ */
