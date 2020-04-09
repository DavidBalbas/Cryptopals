#include "hex2base.cpp"

int main(int argc, char* argv[]) {
  int totalLength;
  string str;
  cout << "Input plaintext" << endl;
  getline(cin, str);
  cout << "Input total length" << endl;
  cin >> totalLength;

  if(padPKCS7(&str, totalLength) == -1){
    cout << "Total length shorter than plaintext" << endl;
  } else{
    cout << str << endl;
  }



  return 0;
}
