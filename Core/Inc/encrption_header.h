/*
 * encrption_header.h
 *
 *  Created on: Dec 21, 2024
 *      Author: wario
 */

#ifndef INC_ENCRPTION_HEADER_H_
#define INC_ENCRPTION_HEADER_H_




void padding_data_for_AES_encryption(unsigned char *original_text,size_t original_text_length ,unsigned char * output);
void AES_Encryption(size_t padded_message_len ,  unsigned char * padded_clear_message, unsigned char*padded_encrypted_message);
void AES_Decryption(size_t padded_message_len ,unsigned char * padded_Encrypted_message,unsigned char*padded_decrytped_message);
#endif /* INC_ENCRPTION_HEADER_H_ */
