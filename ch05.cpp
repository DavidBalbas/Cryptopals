#include "hex2base.cpp"

int main(int argc, char* argv[]){
  string text;
  string key;
  if(argc < 3){
    cout << "Input TEXT" << endl;
    getline(cin, text);
    cout << "Input KEY" << endl;
    getline(cin, key);
  } else {
    text = argv[1];
    key = argv[2];
  }

  int lenText = text.length();
  int lenKey = key.length();
  unsigned char in[lenText];
  unsigned char k[lenKey];
  plaintextToBytes(in, text, lenText);
  plaintextToBytes(k, key, lenKey);

  unsigned char out[lenText];
  repeatedXor(in, out, k, lenText, lenKey);
  string str = encodeHex(out, lenText);
  cout << str << endl;
  return 0;
}
