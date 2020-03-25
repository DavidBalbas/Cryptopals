#include "ecbOracles.cpp"

unsigned char key[16];

int main(int argc, char* argv[]) {
  generateKey(key);

  string baseUnknown = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
  int len = calculateLenBytesBase64(baseUnknown.length());
  unsigned char unknown[len];
  decodeBase64(unknown, baseUnknown, len);
  string strUnknown = UnsignedCharToString(unknown, len);
  string decrypted = ecb_oracle_hard(strUnknown, key);
  cout << endl << "Decrypted string:" << endl << endl;
  cout << decrypted << endl;
  return 0;
}
