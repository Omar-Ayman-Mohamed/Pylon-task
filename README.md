# Pylon-task
-implemented different encryption methods using Mbed TLS library to accomdate different secinarios for example :
1-Sending log data or messagaes via UART using secure communication 
2-Saving encrypted data on external memory
-The main encryption used here is AES-CBC and AES-GMC 
  AES-CBC :
          encrypts and decrypts data using a key , key must be secured in a safe place
          suitable for legacy systems and low resorces systems , consider using it with hashing to ensure data intergrity
  functions :
  
