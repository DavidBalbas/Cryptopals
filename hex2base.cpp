#ifndef hex2base_cpp
#define hex2base_cpp
#include <iostream>
#include <string>

using namespace std;

void decodeHex(unsigned char* bytes, string hexInput);
unsigned char charToByte(char input);
string encodeBase64(unsigned char* bytes, int length);
char intToBase(int n);
string hex2base(string hexInput);
char intToHex(int n);
string encodeHex(unsigned char* bytes, int lenBytes);
void decodeBase64(unsigned char* bytes, string baseInput, int lenBytes);

string hex2base(string hexInput){
  int lenBytes = (hexInput.length() + 1) / 2;
  unsigned char bytes[lenBytes];
  decodeHex(bytes, hexInput);
  return encodeBase64(bytes, lenBytes);
}

string UnsignedCharToString(unsigned char* bytes, int len){
  string out;
  for (int i = 0; i < len; i++) {
    out += (char) bytes[i];
  }
  return out;
}

void plaintextToBytes(unsigned char* bytes, string plaintext, int len){
  for (int i = 0; i < len; i++){
    bytes[i] = (unsigned char) plaintext[i];
  }
}

void decodeHex(unsigned char* bytes, string hexInput){
  // output char[] size = (1 + len) / 2
  int len = hexInput.length();
  int encLen = (len + 1) / 2;
  for (int i = 0; i < encLen; i++){
    bytes[i] = (charToByte(hexInput[2*i]) << 4) + charToByte(hexInput[2*i + 1]);
  }
  if ((len % 2) == 1){
    bytes[encLen - 1] = charToByte(hexInput[len - 1]) << 4;
  }
}

string encodeHex(unsigned char* bytes, int lenBytes){
  string enc;
  for(int i = 0; i < lenBytes; i++){
    enc += intToHex((int) bytes[i] >> 4);
    enc += intToHex((int) bytes[i] % 16);
  }
  return enc;
}

char intToHex(int n){
  if(n >= 0 && n <= 9){
    return '0' + n;
  }
  if(n >= 10 && n <= 15){
    return 'a' + n - 10;
  }
}

unsigned char charToByte(char input){
  if(input >= '0' && input <= '9')
    return (unsigned char) input - '0';
  if(input >= 'A' && input <= 'F')
    return (unsigned char) input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return (unsigned char) input - 'a' + 10;
}

string encodeBase64(unsigned char* bytes, int length){
  string encoding;
  int endLen = length % 3;
  // each 3 bytes correspond to 4 base
  int i;
  for(i = 0; i < length - endLen; i += 3){
    encoding += intToBase((int) (bytes[i] >> 2));
    encoding += intToBase((int) (((bytes[i] % 4) * 16) + (bytes[i+1] >> 4)));
    encoding += intToBase((int) (((bytes[i+1] % 16) * 4) + (bytes[i+2] >> 6)));
    encoding += intToBase((int) ((bytes[i+2] % 64)));
  }
  i = length - endLen;
  if (endLen == 1){
    encoding += (intToBase((int) bytes[i] >> 2));
    encoding += (intToBase((int) (bytes[i] % 4) << 4));
    encoding += (intToBase(-1));
    encoding += (intToBase(-1));
  }
  if (endLen == 2) {
    encoding += (intToBase((int) bytes[i] >> 2));
    encoding += (intToBase((int) ((bytes[i] % 4) << 4 + bytes[i+1] >> 4)));
    encoding += (intToBase((int) (bytes[i+1] % 16) << 2));
    encoding += (intToBase(-1));
  }
  return encoding;
}

char intToBase(int n){
  if (n < 0){
    return '=';
  } else if(n < 26){
    return 'A' + n;
  } else if(n < 52){
    return 'a' + (n - 26);
  } else if(n < 62){
    return '0' + (n - 52);
  } else if(n == 62){
    return '+';
  } else if(n == 63){
    return '/';
  }
}

int baseToInt(char c){
  if(c == '='){
    return 0; //Not sure this should work like this
  }
  if(c >= 'A' && c <= 'Z'){
    return c - 'A';
  }
  if(c >= 'a' && c <= 'z'){
    return c - 'a' + 26;
  }
  if(c >= '0' && c <= '9'){
    return c - '0' + 52;
  }
  if(c == '+') return 62;
  if(c == '/') return 63;
}

void decodeBase64(unsigned char* bytes, string baseInput, int lenBytes){
  int j = 0;
  for(int i = 0; i < lenBytes; i += 3){
    bytes[i] = (baseToInt(baseInput[j]) << 2) + (baseToInt(baseInput[j+1]) >> 4);
    bytes[i+1] = ((baseToInt(baseInput[j+1]) % 16) << 4) + (baseToInt(baseInput[j+2]) >> 2);
    bytes[i+2] = ((baseToInt(baseInput[j+2]) % 4) << 6) + (baseToInt(baseInput[j+3]));
    j += 4;
  }
}

int calculateLenBytesBase64(int lenBase){
  // Each 4 base chars require 3 bytes
  return (lenBase + 1) * 3 / 4; // Watch out the padding
}

string baseToString(string baseText){
  int len = calculateLenBytesBase64(baseText.length());
  unsigned char bytes[len];
  decodeBase64(bytes, baseText, len);
  return UnsignedCharToString(bytes, len);
}

int padPKCS7(string *plaintext, int totalLength){
  if(totalLength < plaintext->length()) return -1;
  unsigned char padLen = totalLength - plaintext->length();
  for(int i = 0; i < padLen; i++){
    *plaintext += (char) padLen;
  }
  return 0;
}

string autoPadPKCS7(string plaintext){
  int padLen = 16 - (plaintext.length() % 16);
  padPKCS7(&plaintext, padLen + plaintext.length());
  return plaintext;
}

string unpadPKCS7(string plaintext){
  unsigned char lastChar = (unsigned char) plaintext[plaintext.length() - 1];
  for(unsigned char i = 1; i <= lastChar; i++){
    if(((unsigned char) plaintext[plaintext.length() - i]) != lastChar){
      return "";
    }
  }
  string s;
  for(int i = 0; i < plaintext.length() - (int) lastChar; i++){
    s += plaintext[i];
  }
  return s;
}

void fixedXor(unsigned char* in, unsigned char* pad, int len){
  for (int i = 0; i < len; i++) {
    in[i] ^= pad[i];
  }
}

void singleXor(unsigned char* in, unsigned char* out, unsigned char c, int len){
  for(int i = 0; i < len; i++){
    out[i] = c;
  }
  fixedXor(out, in, len);
}

void repeatedXor(unsigned char* in, unsigned char* out, unsigned char* key, int lenIn, int lenKey){
  for(int i = 0; i < lenIn; i++){
    out[i] = key[i % lenKey];
  }
  fixedXor(out, in, lenIn);
}

#endif
