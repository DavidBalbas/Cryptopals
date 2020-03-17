#include "fixedXor.cpp"

using namespace std;

float freqs[26] = {8.55, 1.60, 3.16, 3.87, 12.1, 2.18, 2.09, 4.96, 7.33, 0.22,
                   0.81, 4.21, 2.53, 7.17, 7.47, 2.07, 0.10, 6.33, 6.73, 8.94,
                   2.68, 1.06, 1.83, 0.19, 1.72, 0.11};
float spaceScore = 20;

//Note: Fix the type conversions and pointers

float stringScore(string s){
  float score;
  for(char& c : s){
    if(c >= 'a' && c <= 'z'){
      score += freqs[(int) c - 'a'];
    }
    if(c >= 'A' && c <= 'Z'){
      score += freqs[(int) c - 'A'];
    }
    if(c == ' '){
      score += spaceScore;
    }
  }
  return score;
}

char findFixedXorChar(unsigned char* bytes, int len){
  float scores[256];
  int c;
  unsigned char xored[len];
  for(c = 0; c < 256; c++){
    singleXor(bytes, xored, (char) c, len);
    string str = UnsignedCharToString(xored, len);
    scores[c] = stringScore(str);
  }
  int first = 0;
  for(int i = 0; i < 256; i++){
    if(scores[i] > scores[first]){
      first = i;
    }
  }
  return (char) first;
}

// CODE FOR EXERCISE 3

// int main(int argc, char* argv[]){
//   string hexIn;
//   if(argc < 2){
//     cout << "Input hex string" << endl;
//     getline(cin, hexIn);
//   } else {
//     hexIn = argv[1];
//   }
//
//   int len = (hexIn.length() + 1) / 2;
//   unsigned char in[len];
//   decodeHex(in, hexIn);
//   char key = findFixedXorChar(in, len);
//   cout << key << endl;
//   unsigned char out[len];
//   singleXor(in, out, key, len);
//   string str = UnsignedCharToString(out, len);
//   cout << str << endl;
//   return 0;
// }

//  CODE FOR EXERCISE 4

// int main(int argc, char* argv[]){
//   string hexIn;
//   while(getline(cin, hexIn)){
//     int len = (hexIn.length() + 1) / 2;
//     unsigned char in[len];
//     decodeHex(in, hexIn);
//     char key = findFixedXorChar(in, len);
//     cout << key << endl;
//     unsigned char out[len];
//     singleXor(in, out, key, len);
//     string str = UnsignedCharToString(out, len);
//     cout << str << endl;
//   }
//   return 0;
// }


// CODE FOR EXERCISE 5

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
