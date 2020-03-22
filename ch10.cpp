#include "aes.cpp"

int main(int argc, char* argv[]){

  unsigned char iv[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  string baseText;
  string key;
  cout << "Input KEY" << endl;
  getline(cin, key);

  int lenKey = key.length();
  if(lenKey != 16) return -1;

  unsigned char k[lenKey];
  plaintextToBytes(k, key, lenKey);

  if(argc < 2){
    cout << "REQUIRES INPUT FILE" << endl;
  } else {
    //int lenBytes = 48;
    ifstream inFile(argv[1]);
    if(inFile.is_open()) {
      char c;
      while(inFile.get(c)) {
        if(c != '\n'){
          baseText += c;
        }
      }
      inFile.close();
    }
    int lenBytes = calculateLenBytesBase64(baseText.length());
    unsigned char in[lenBytes];
    unsigned char out[lenBytes];
    decodeBase64(in, baseText, lenBytes);
    aes_decrypt_cbc(in, k, out, iv, lenBytes);

    cout << UnsignedCharToString(out, lenBytes) << endl;


    // if(baseText.length() == 64){
    //   unsigned char in[lenBytes];
    //   unsigned char out[lenBytes];
    //   decodeBase64(in, baseText, lenBytes);
    //
    //   aes_decrypt_cbc(in, k, out, iv, 48);
    //
    //   cout << UnsignedCharToString(out, lenBytes) << endl;
    //
    //   baseText = "";
    //   for(int i = 0; i < 16; i++){
    //     iv[i] = out[i + 32];
    //   }
  }
  return 0;
}
