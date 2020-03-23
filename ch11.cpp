#include "ecbOracles.cpp"

using namespace std;

int main(int argc, char* argv[]) {
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
