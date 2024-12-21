/*
 * key_stoarge.h
 *
 *  Created on: Dec 21, 2024
 *      Author: wario
 */
/*this file includes global keys for encryption and decryption
 * must be protected by securing it , using password to acess , save in a secure place etc.
 * */
#ifndef INC_KEY_STOARGE_H_
#define INC_KEY_STOARGE_H_
#include"AES_User_defines.h"
/*key and iv for AES-CBC operations */
const unsigned char key[32] = "0123456789abcdef0123456789abcdef"; // 256-bit key
const unsigned char iv[AES_BLOCK_SIZE] = "0123456789abcdef";        // 128-bit IV
/*key,iv and aad for AES-GCM operations */
const unsigned char aad[] = "header data";
const unsigned char GCM_key[AES_GCM_KEY_SIZE] = "abcdefghilmnopq";
const unsigned char GCM_iv[AES_GCM_IV_SIZE] = "123456789101";
size_t aad_len = strlen((char *)aad);
/*personalized string for random data generation*/
const unsigned char pers[] = "ahmed is -";
size_t pers_len = strlen((char *)pers);
#endif /* INC_KEY_STOARGE_H_ */
