#import "ch11.cpp"


int main(int argc, char* argv[]) {
  baseUnknown = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
  unsigned char unknown[calculateLenBytesBase64(baseUnknown.length())];
  decodeBase64(unknown, baseUnknown, calculateLenBytesBase64(baseUnknown.length()));

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
