#ifndef ecbOracles
#define ecbOracles

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

int aes_decrypt_ecb(unsigned char* ciphertext, unsigned char* key, unsigned char* plaintext, int length){
  if(length % 16 != 0){
    return -1;
  }
  for(int i = 0; i < length; i += 16){
    aes_decrypt(ciphertext + i, key, plaintext + i);
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

string ecb_oracle_aux(string s1, char c, unsigned char* key){
  string in = s1 + c;
  //in = autoPadPKCS7(in);
  int len = in.length();
  unsigned char plaintext[len];
  unsigned char ciphertext[len];
  plaintextToBytes(plaintext, in, len);
  aes_encrypt_ecb(plaintext, key, ciphertext, len);
  return UnsignedCharToString(ciphertext, len);
}

string ecb_oracle(string unknown, unsigned char* key){
  int blockSize = 16;
  string s1;
  while (s1.length() < blockSize -1 ) s1 += 'A';
  string dict[256];
  for(int i = 0; i < 256; i++){
    dict[i] = ecb_oracle_aux(s1, (char) i, key);
  }
  string aux;
  string decrypted;
  for(int i = 0; i < unknown.length(); i++){
    aux = ecb_oracle_aux(s1, unknown[i], key);
    for(int j = 0; j < 256; j++){
      if(! aux.compare(dict[j])){
        decrypted += (char) j;
        break;
      }
    }
  }
  return decrypted;
}

#endif
