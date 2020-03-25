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

string ecb_oracle_aux_hard(string unknown, string s1, unsigned char* key){
  string r = "12345678901234";
  string in = r + s1 + unknown;
  in = autoPadPKCS7(in);
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
  while (s1.length() < blockSize - 1) s1 += 'A';
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

// string ecb_oracle_hard(string unknown, unsigned char* key){
//   // step 1: find length of random string.
//   // to be sure, we should do it twice with another letter(s).
//   string s1 = "AAAAAAAAAAAAAAA";
//   string c1;
//   int pos = 0;
//   int blockSize = 16;
//   while(pos == 0 && s1.length() <= 4 * blockSize){
//     s1 += 'A';
//     c1 = ecb_oracle_aux_hard(unknown, s1, key);
//     for(int i = 0; i < c1.length() - blockSize; i += 1){
//       if(! c1.substr(i, blockSize).compare(c1.substr(i+blockSize, blockSize))) pos = i;
//     }
//   }
//   int rlen = 2 * blockSize + pos - s1.length();
//   if (pos == 0){
//     cout << "not detected" << endl;
//   } else{
//     cout << "length of random string: " << rlen << endl;
//   }
//   // step 2: find length of unknown
//   int inlen = pos - rlen + blockSize;
//   int unkBlocks = (c1.length() - inlen - rlen) / blockSize;
//   unkBlocks = 9;
//   string foundBlocks[unkBlocks];
//   string dict[256];
//   string aux;
//   string subaux;
//   string decrypted;
//   string s2;
//   // step 3: loop over each block of ciphertext
//   for(int block = 0; block < unkBlocks; block++){
//     for(int i = 0; i < blockSize; i++){
//       s1 = "";
//       s2 = "";
//       while (s1.length() < inlen - i - 1) s1 += 'A';
//       int t = 0;
//       while (s1.length() + s2.length() < inlen - 1){
//         s2 += foundBlocks[block][t];
//         t++;
//       }
//       // s1 is crafted
//       for(int j = 0; j < 256; j++){
//         dict[j] = ecb_oracle_aux_hard(unknown, s1 + s2 + (char) j , key);
//       }
//       aux = ecb_oracle_aux_hard(unknown, s1, key);
//       for(int j = 0; j < 256; j++){
//         subaux = aux.substr(inlen + rlen + ((block-1) * blockSize), blockSize);
//         if(! subaux.compare(dict[j].substr(inlen + rlen + ((block-1) * blockSize), blockSize))){
//           foundBlocks[block] += (char) j;
//           break;
//         }
//       }
//     }
//   }
//   for(int i = 0; i < unkBlocks; i++){
//     cout << foundBlocks[i];
//     decrypted += foundBlocks[i];
//   }
//   cout << "decr: " << decrypted << endl;
//   return decrypted;
// }

int compareStrings_aux(string s1, string s2, int ini, int blockSize){
  for(int i = ini; i < s1.length(); i += blockSize){
    string a = s1.substr(i, blockSize);
    string b = s2.substr(ini, blockSize);
    if(! a.compare(b)){
      return 1;
    }
  }
  return 0;
}

string ecb_oracle_hard(string unknown, unsigned char* key){
  // step 1: find length of random string.
  // to be sure, we should do it twice with another letter(s).
  string s1 = "AAAAAAAAAAAAAAA";
  string c1;
  int pos = 0;
  int blockSize = 16;
  while(pos == 0 && s1.length() <= 4 * blockSize){
    s1 += 'A';
    c1 = ecb_oracle_aux_hard(unknown, s1, key);
    for(int i = 0; i < c1.length() - blockSize; i += 1){
      if(! c1.substr(i, blockSize).compare(c1.substr(i+blockSize, blockSize))) pos = i;
    }
  }
  int rlen = 2 * blockSize + pos - s1.length();
  if (pos == 0){
    cout << "not detected" << endl;
  } else{
    cout << "length of random string: " << rlen << endl;
  }
  // step 2: find length of unknown
  int inlen = pos - rlen + blockSize;
  int unklen = (c1.length() - inlen - rlen);
  string dict[256];
  string aux;
  string subaux;
  string decrypted;
  string s2;
  // step 3: loop over each block of ciphertext
  for(int i = 0; i < blockSize; i++){
    s1 = "";
    s2 = "";
    while (s1.length() < inlen - i - 1) s1 += 'A';
    int t = 0;
    while (s1.length() + s2.length() < inlen - 1){
      s2 += decrypted[t];
      t++;
    }
    // s1 is crafted
    for(int j = 0; j < 256; j++){
      dict[j] = ecb_oracle_aux_hard(unknown, s1 + s2 + (char) j , key);
    }
    aux = ecb_oracle_aux_hard(unknown, s1, key);
    for(int j = 0; j < 256; j++){
      subaux = aux.substr(inlen + rlen - blockSize, blockSize);
      if(! subaux.compare(dict[j].substr(inlen + rlen - blockSize, blockSize))){
        decrypted += (char) j;
        break;
      }
    }
  }


  s1 = ""; //this is now fixed
  while (s1.length() < inlen) s1 += 'A';
  string s3;
  int lens3;
  for(int i = blockSize; i < unklen; i++) {
    s3 = "";
    s2 = decrypted.substr(decrypted.length() - blockSize + 1, blockSize - 1); //fine
    lens3 = ((blockSize - i - 1) % blockSize) + blockSize;
    //lens3 = 31 - i ;
    while (s3.length() < lens3) s3 += 'A';
    for(int j = 0; j < 256; j++){
      dict[j] = ecb_oracle_aux_hard(unknown, s1 + s2 + (char) j , key);
    }
    aux = ecb_oracle_aux_hard(unknown, s1 + s3, key);
    for(int j = 0; j < 256; j++){
      if(compareStrings_aux(aux, dict[j], inlen + rlen, blockSize)){
        decrypted += (char) j;
        break;
      }
    }
  }

  return unpadPKCS7(decrypted);
}

#endif
