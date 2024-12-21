# Pylon-task

-implemented different encryption methods using Mbed TLS library to accomdate different secinarios for example :
##
1-Sending log data or messagaes via UART using secure communication 
##
2-Saving encrypted data on external memory
-The main encryption used here is AES-CBC and AES-GMC 
  AES-CBC :
          encrypts and decrypts data using a key , key must be secured in a safe place
          suitable for legacy systems and low resorces systems , consider using it with hashing to ensure data intergrity
  functions :
  ``` C
  void padding_data_for_AES_CBC_encryption(unsigned char*original_text,size_t original_text_length,unsigned char * output){
    memcpy(output, original_text, original_text_length);
    memset(output + original_text_length, 0x00, 128 - original_text_length);  //padding
}
```
Here the AES frame works with data of 16 byte blocks , so we must ensure that the data is padded before encryption 
