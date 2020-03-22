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
void decodeBase64(unsigned char* bytes, string baseInput);

// int main(int argc, char* argv[]){
//   cout << intToBase(1) << intToBase(25) << intToBase(-1) << '\n';
//   string hexInput;
//   hexInput = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
//   cout << "input: " << '\n';
//   cout << hexInput;
//   cout << '\n';
//   int lenBytes = (hexInput.length() + 1) / 2;
//
//   unsigned char bytes[lenBytes];
//   decodeHex(bytes, hexInput);
//
//   string baseEnc = encodeBase64(bytes, lenBytes);
//
//   for (int j = 0; j < lenBytes; j++){
//     cout << hex << (int) bytes[j];
//   }
//   cout << '\n' << baseEnc;
//   return 0;
// }

// int main(int argc, char* argv[]){
//   string hexInput;
//   if(argc == 1){
//     cout << "Input hex string to encode" << '\n';
//     getline(cin, hexInput);
//   } else {
//     hexInput = argv[1];
//   }
//   cout << hex2base(hexInput);
//   return 0;
// }

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

int padPKCS7(string *plaintext, int totalLength){
  if(totalLength < plaintext->length()) return -1;
  unsigned char padLen = totalLength - plaintext->length();
  for(int i = 0; i < padLen; i++){
    *plaintext += (char) padLen;
  }
  return 0;
}

#endif
