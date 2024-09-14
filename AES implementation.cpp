#include <AESLib.h>

AESLib aesLib;

// AES Encryption Key (128-bit key example)
byte aes_key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 
                   0xab, 0xf7, 0x3e, 0xb6, 0x30, 0x30, 0x30, 0x30 };

// Initialization Vector (IV)
byte aes_iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                   0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

char encrypted_data[512];  // Array to hold encrypted data
char decrypted_data[512];  // Array to hold decrypted data
char input_buffer[128];    // Buffer to store the input from Serial Monitor

// Function to add padding to the input data to fit AES block size (16 bytes)
void addPadding(char* data, uint16_t& length) {
  int padLen = 16 - (length % 16);
  for (int i = 0; i < padLen; i++) {
    data[length + i] = padLen;  // Add padding byte
  }
  length += padLen;  // Update length to reflect padding
}

void encryptAES(char* plaintext) {
  uint16_t plaintext_length = strlen(plaintext);  // Length of the plaintext
  addPadding(plaintext, plaintext_length);  // Pad the plaintext to block size
  
  int encrypted_length = aesLib.encrypt64((byte*)plaintext, plaintext_length, encrypted_data, aes_key, 128, aes_iv); 

  Serial.print("Encrypted Data (HEX): ");
  for (int i = 0; i < encrypted_length; i++) {
    Serial.print((unsigned char) encrypted_data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

// Function to remove padding from decrypted data
void removePadding(char* data, uint16_t& length) {
  int padLen = data[length - 1];  // Last byte is the padding length
  length -= padLen;  // Remove padding length from actual data length
  data[length] = '\0';  // Null-terminate the string
}

void decryptAES() {
  uint16_t encrypted_length = strlen(encrypted_data);  // Get length of the encrypted data
  uint16_t decrypted_length = aesLib.decrypt64(encrypted_data, encrypted_length, decrypted_data, aes_key, 128, aes_iv);

  removePadding(decrypted_data, decrypted_length);  // Remove padding from decrypted data

  Serial.print("Decrypted Data (ASCII): ");
  Serial.println(decrypted_data);  // Print the decrypted data in ASCII
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Enter a string to encrypt:");
}

void loop() {
  // Check if data is available in the serial buffer
  if (Serial.available()) {
    // Read the input string from Serial Monitor
    int length = Serial.readBytesUntil('\n', input_buffer, sizeof(input_buffer) - 1);
    input_buffer[length] = '\0';  // Null-terminate the string

    Serial.print("You entered: ");
    Serial.println(input_buffer);

    // Encrypt the entered string
    encryptAES(input_buffer);

    // Decrypt the encrypted data and print in ASCII
    decryptAES();

    Serial.println("Enter another string to encrypt:");
  }
}
