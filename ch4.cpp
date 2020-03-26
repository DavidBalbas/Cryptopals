#include "detectSingleXor.cpp"

int main(int argc, char* argv[]){
  string hexIn;
  while(getline(cin, hexIn)){
    int len = (hexIn.length() + 1) / 2;
    unsigned char in[len];
    decodeHex(in, hexIn);
    char key = findFixedXorChar(in, len);
    cout << key << endl;
    unsigned char out[len];
    singleXor(in, out, key, len);
    string str = UnsignedCharToString(out, len);
    cout << str << endl;
  }
  return 0;
}
