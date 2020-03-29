#include "hex2base.cpp"
#include "detectSingleXor.cpp"
#include <string>
#include <fstream>


int main(int argc, char* argv[]) {
  //hamming distance testing
  string s1 = "this is a test";
  string s2 = "wokka wokka!!!";
  unsigned char b1[s1.length()];
  plaintextToBytes(b1, s1, s1.length());
  unsigned char b2[s2.length()];
  plaintextToBytes(b2, s2, s2.length());
  cout << "The hamming distance is: " ;
  cout << hammingDistance(b1, s1.length(), b2, s2.length()) << endl;

  string baseText;
  string line;
  ifstream inFile(argv[1]);
  if(inFile.is_open()) {
    while(getline(inFile, line)){
      baseText += line;
    }
    inFile.close();
  }
  string ciphertext = baseToString(baseText);
  unsigned char cipherBytes[ciphertext.length()];
  plaintextToBytes(cipherBytes, ciphertext, ciphertext.length());
  int keySize = findKeySize(cipherBytes, 40, ciphertext.length());
  // keySize = 29;
  cout << "The key size is: " << keySize << endl;
  unsigned char blocks[keySize][ciphertext.length() / keySize + 1];
  string key;

  for(int i = 0; i < ciphertext.length(); i++){
    blocks[i % keySize][i / keySize] = cipherBytes[i];
  }
  for(int i = 0; i < keySize; i++){
    key += findFixedXorChar(blocks[i], ciphertext.length() / keySize);
  }
  cout << "The key is: " << key << endl;
  unsigned char keyBytes[keySize];
  unsigned char decryptedBytes[ciphertext.length()];
  plaintextToBytes(keyBytes, key, keySize);

  repeatedXor(cipherBytes, decryptedBytes, keyBytes, ciphertext.length(), keySize);
  //cout << UnsignedCharToString(decryptedBytes, ciphertext.length()) << endl;

  return 0;
}
