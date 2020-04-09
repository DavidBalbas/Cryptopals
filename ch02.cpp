#include "hex2base.cpp"

int main(int argc, char* argv[]){
  string hexIn;
  string hexPad;
  if(argc < 3){
    cout << "Input hex string" << '\n';
    getline(cin, hexIn);
    cout << "Input hex pad" << '\n';
    getline(cin, hexPad);
  } else {
    hexIn = argv[1];
    hexPad = argv[2];
  }

  int len = (hexIn.length() + 1) / 2;
  unsigned char in[len];
  unsigned char pad[len];

  decodeHex(in, hexIn);
  decodeHex(pad, hexPad);
  fixedXor(in, pad, len);
  cout << encodeHex(in, len);
  return 0;
}
