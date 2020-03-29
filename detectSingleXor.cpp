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

// Brian Kerningham's Algorithm
int countOnesInByte(unsigned char n) {
  int i = 0;
  while(n){
    n &= (n - 1);
    i++;
  }
  return i;
}

int hammingDistance(unsigned char* b1, int l1, unsigned char* b2, int l2){
  int distance = 0;
  unsigned char ** blong;
  if(l1 > l2){
    blong = &b1;
  } else{
    blong = &b2;
  }
  for(int i = 0; i < max(l1, l2); i++){
    if(i < min(l1, l2)){
      distance += countOnesInByte(b1[i] ^ b2[i]);
    } else {
      distance += countOnesInByte(*blong[i]);
    }
  }
  return distance;
}

int findKeySize(unsigned char* ciphertext, int maxSize, int len){
  float distances[maxSize - 1];
  int minSize = 2;
  for(int size = 2; size <= maxSize; size++){
    int numPieces = len / size - 1;
    distances[size - 2] = 0;
    for(int j = 0; j < numPieces; j++) {
      distances[size - 2] += hammingDistance(ciphertext, size, ciphertext + size * j, size);
    }
    distances[size - 2] = distances[size - 2] / (size * numPieces);
    cout << size << '\t' << distances[size - 2] << endl;
    if(distances[size - 2] < distances[minSize - 2]){
      minSize = size;
    }
  }
  return minSize;
}
