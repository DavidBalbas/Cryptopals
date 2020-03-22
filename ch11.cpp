#ifndef ch11_cpp
#define ch11_cpp

#include "aes.cpp"
#include <random>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

using namespace std;

void generateKey(unsigned char* key){
  random_device rd;
  for(int i = 0; i < 16; i++){
    key[i] = (unsigned char) (rd() >> 24);
  }
}

string generateRandomString(int length){
  string str;
  for(int i = 0; i < length; i++){
    str =+ (char) (rand() >> 24);
  }
  return str;
}

int aes_encrypt_ecb(unsigned char* plaintext, unsigned char* key, unsigned char* ciphertext, int length){
  if(length % 16 != 0){
    return -1;
  }
  for(int i = 0; i < length; i += 16){
    aes_encrypt(plaintext + i, key, ciphertext + i);
  }
  return 0;
}

string aes_encrypt_random(string preText){
  random_device rd;
  srand(rd());
  int beginLen = rand() % 6 + 5;
  int endLen = rand() % 6 + 5;
  string beginPad = generateRandomString(beginLen);
  string endPad = generateRandomString(beginLen);
  string text = beginPad + preText + endPad;

  int length = text.length();
  if(length % 16 != 0){
    length += 16 - (length % 16);
  }
  padPKCS7(&text, length);
  //Remove
  if(length % 16 != 0){
    cout << "la liaste wey";
  }
  unsigned char plaintext[length];
  unsigned char ciphertext[length];
  unsigned char key[16];
  generateKey(key);
  plaintextToBytes(plaintext, text, length);

  if(rand() % 2){
    //generate iv and key
    unsigned char iv[16];
    generateKey(iv);
    aes_encrypt_cbc(plaintext, key, ciphertext, iv, length);
    cout << "Encrypting in CBC" << endl;
  } else{
    aes_encrypt_ecb(plaintext, key, ciphertext, length);
    cout << "Encrypting in ECB" << endl;
  }
  return UnsignedCharToString(plaintext, length);
}

/**
Returns 1 if ECB, 0 if CBC
 */
int cbc_ecb_oracle(string str){
  for(int i = 0; i < str.length(); i += 1){
    string a = str.substr(i, 16);
    for(int j = i + 16; j < str.length(); j += 16){
      string b = str.substr(j, 16);
      if(! a.compare(b)){
        return 1;
      }
    }
  }
  return 0;
}

int main(int argc, char* argv[]) {
  string str;
  char c;
  ifstream inFile(argv[1]);
  if(inFile.is_open()) {
    while(inFile.get(c)) {
      str += c;
    }
    inFile.close();
  }
  string encr = aes_encrypt_random(str);
  int mode = cbc_ecb_oracle(encr);
  if (mode){
    cout << "Guessed ECB" << endl;
  } else{
    cout << "Guessed CBC" << endl;
  }
  return 0;
}

#endif
