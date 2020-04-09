#include "aes.cpp"

using namespace std;

//Adapted for Exercise 7, Set 1
int main(int argc, char* argv[]) {
  string baseText;
  string key;
  cout << "Input KEY" << endl;
  getline(cin, key);

  int lenKey = key.length();
  if(lenKey != 16) return -1;

  unsigned char k[lenKey];
  plaintextToBytes(k, key, lenKey);

  if(argc < 2){
    cout << "Input TEXT base64" << endl;
    getline(cin, baseText);

  } else {
    int lenBytes = 48;
    ifstream inFile(argv[1]);
    if(inFile.is_open()) {
      char c;
      while(inFile.get(c)) {
        if(c != '\n'){
          baseText += c;
        }
        if(baseText.length() == 64){
          unsigned char in[lenBytes];
          unsigned char out[lenBytes];
          decodeBase64(in, baseText, lenBytes);

          aes_decrypt(in, k, out);
          aes_decrypt(in + 16, k, out + 16);
          aes_decrypt(in + 32, k, out + 32);

          cout << UnsignedCharToString(out, lenBytes);

          baseText = "";
        }
      }
      inFile.close();
    }
    // GETTING BASE TEXT AS ARGV 2
  }

  return 0;
}
