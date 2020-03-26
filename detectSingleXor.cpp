#include "hex2base.cpp"


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
