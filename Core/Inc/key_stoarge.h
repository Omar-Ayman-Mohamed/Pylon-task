/*
 * key_stoarge.h
 *
 *  Created on: Dec 21, 2024
 *      Author: wario
 */

#ifndef INC_KEY_STOARGE_H_
#define INC_KEY_STOARGE_H_
#include"AES_User_defines.h"
const unsigned char key[32] = "0123456789abcdef0123456789abcdef"; // 256-bit key
const unsigned char iv[AES_BLOCK_SIZE] = "0123456789abcdef";        // 128-bit IV

#endif /* INC_KEY_STOARGE_H_ */
