#include "hex2base.cpp"

using namespace std;

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

// int main(int argc, char* argv[]){
//   string hexIn;
//   string hexPad;
//   if(argc < 3){
//     cout << "Input hex string" << '\n';
//     getline(cin, hexIn);
//     cout << "Input hex pad" << '\n';
//     getline(cin, hexPad);
//   } else {
//     hexIn = argv[1];
//     hexPad = argv[2];
//   }
//
//   int len = (hexIn.length() + 1) / 2;
//   unsigned char in[len];
//   unsigned char pad[len];
//
//   decodeHex(in, hexIn);
//   decodeHex(pad, hexPad);
//   fixedXor(in, pad, len);
//   cout << encodeHex(in, len);
//   return 0;
// }
