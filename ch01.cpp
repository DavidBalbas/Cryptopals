#include "hex2base.cpp"

int main(int argc, char* argv[]){
  string hexInput;
  if(argc == 1){
    cout << "Input hex string to encode" << '\n';
    getline(cin, hexInput);
  } else {
    hexInput = argv[1];
  }
  cout << hex2base(hexInput);
  return 0;
}
