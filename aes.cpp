#include "aes.h"

void subWord(unsigned char* word){
  for (int i = 0; i < 4; i++) {
    word[i] = sbox[word[i] / 16][word[i] % 16];
  }
}

void rotWord(unsigned char* word){
  unsigned char temp = word[0];
  word[0] = word[1];
  word[1] = word[2];
  word[2] = word[3];
  word[3] = temp;
}

void keyExpansion(unsigned char* key, unsigned char* w) {
  //unsigned char temp[4];
  int i = 0;
  while (i < 16) {
    w[i] = key[i];
    i++;
  }
  i = 4;
  while (i < 44) {
    unsigned char temp[4] = { w[4 * i - 4], w[4 * i - 3], w[4 * i - 2], w[4 * i - 1] };
    if (i % 4 == 0) {
      rotWord(temp);
      subWord(temp);
      temp[0] ^= rcon[i/4];
    }
    w[4*i] = w[4*i-16] ^ temp[0];
    w[4*i+1] = w[4*i-15] ^ temp[1];
    w[4*i+2] = w[4*i-14] ^ temp[2];
    w[4*i+3] = w[4*i-13] ^ temp[3];
    i += 1;
  }
}

void addRoundKey(unsigned char state[][4], unsigned char* w) {
  for(int i = 0; i < 16; i++){
    state[i%4][i/4] ^= w[i];
  }
}

void subBytes(unsigned char state[][4]) {
  for(int i = 0; i < 16; i++){
    state[i/4][i%4] = sbox[state[i/4][i%4]/16][state[i/4][i%4]%16];
  }
}

void invSubBytes(unsigned char state[][4]) {
  for(int i = 0; i < 16; i++){
    state[i/4][i%4] = rsbox[state[i/4][i%4]/16][state[i/4][i%4]%16];
  }
}

void shiftRows(unsigned char state[][4]) {
  unsigned char temp, temp2;
  //Row 1 (row 0 unchanged)
  temp = state[1][0];
  for(int j = 0; j < 3; j++){
    state[1][j] = state[1][(1+j) % 4];
  }
  state[1][3] = temp;
  //Row 2
  temp = state[2][0];
  temp2 = state[2][1];
  state[2][0] = state[2][2];
  state[2][1] = state[2][3];
  state[2][2] = temp;
  state[2][3] = temp2;
  //Row 3
  temp = state[3][0];
  state[3][0] = state[3][3];
  state[3][3] = state[3][2];
  state[3][2] = state[3][1];
  state[3][1] = temp;
}

void invShiftRows(unsigned char state[][4]) {
  unsigned char temp, temp2;
  //Row 1 (row 0 unchanged)
  temp = state[3][0];
  for(int j = 0; j < 3; j++){
    state[3][j] = state[3][(1+j) % 4];
  }
  state[3][3] = temp;
  //Row 2
  temp = state[2][0];
  temp2 = state[2][1];
  state[2][0] = state[2][2];
  state[2][1] = state[2][3];
  state[2][2] = temp;
  state[2][3] = temp2;
  //Row 3
  temp = state[1][0];
  state[1][0] = state[1][3];
  state[1][3] = state[1][2];
  state[1][2] = state[1][1];
  state[1][1] = temp;
}


unsigned char mult(unsigned char n, unsigned char m){
  if (n == 0x02) {
    return mul2[m];
  }
  if (n == 0x03) {
    return mul3[m];
  }
  return m;
}

unsigned char imult(unsigned char n, unsigned char m){
  if (n == 0x09) {
    return mul9[m];
  }
  if (n == 0x0b) {
    return mul11[m];
  }
  if (n == 0x0d) {
    return mul13[m];
  }
  if (n == 0x0e) {
    return mul14[m];
  }
  return m;
}

void mixColumns(unsigned char state[][4]) {
  unsigned char c[4], d[4];
  for (int j = 0; j < 4; j++) {
    //Create columns to operate on them
    for (int i = 0; i < 4; i++) {
      c[i] = state[i][j];
    }
    d[0] = mult(0x02, c[0]) ^ mult(0x03, c[1]) ^ c[2] ^ c[3];
    d[1] = c[0] ^ mult(0x02, c[1]) ^ mult(0x03, c[2]) ^ c[3];
    d[2] = c[0] ^ c[1] ^ mult(0x02, c[2]) ^ mult(0x03, c[3]);
    d[3] = mult(0x03, c[0]) ^ c[1] ^ c[2] ^ mult(0x02, c[3]);
    for(int i = 0; i < 4; i++){
      state[i][j] = d[i];
    }
  }
}

void invMixColumns(unsigned char state[][4]) {
  unsigned char c[4], d[4];
  for (int j = 0; j < 4; j++) {
    //Create columns to operate on them
    for (int i = 0; i < 4; i++) {
      c[i] = state[i][j];
    }
    d[0] = imult(0x0e, c[0]) ^ imult(0x0b, c[1]) ^ imult(0x0d, c[2]) ^ imult(0x09, c[3]);
    d[1] = imult(0x09, c[0]) ^ imult(0x0e, c[1]) ^ imult(0x0b, c[2]) ^ imult(0x0d, c[3]);
    d[2] = imult(0x0d, c[0]) ^ imult(0x09, c[1]) ^ imult(0x0e, c[2]) ^ imult(0x0b, c[3]);
    d[3] = imult(0x0b, c[0]) ^ imult(0x0d, c[1]) ^ imult(0x09, c[2]) ^ imult(0x0e, c[3]);
    for(int i = 0; i < 4; i++){
      state[i][j] = d[i];
    }
  }
}

void printState(unsigned char state[][4]) {
for(int i=0; i<16; i++){
  cout << encodeHex(&state[i%4][i/4], 1);
}
cout << endl;
}


unsigned char* aes_decrypt(unsigned char* ciphertext, unsigned char* key, unsigned char* plaintext) {
  // Plaintext, ciphertext and key are arrays unsigned char[16]
  const int ROUNDS = 10;
  unsigned char state[4][4]; //state[i] is a ROW of the matrix. Each column is a WORD.
  unsigned char w[16 * (ROUNDS + 1)];

  keyExpansion(key, w);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = ciphertext[i + 4 * j];
    }
  }
  addRoundKey(state, w + ROUNDS * 16);

  for (int i = ROUNDS - 1; i > 0; i--) {

    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, w + i * 16);
    invMixColumns(state);
  }

  invSubBytes(state);
  invShiftRows(state);
  addRoundKey(state, w + 0);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      plaintext[i + 4 * j] = state[i][j];
    }
  }
  return plaintext;
}

unsigned char* aes_encrypt(unsigned char* plaintext, unsigned char* key, unsigned char* ciphertext) {
  // Plaintext, ciphertext and key are arrays unsigned char[16]
  const int ROUNDS = 10;
  unsigned char state[4][4]; //state[i] is a ROW of the matrix. Each column is a WORD.
  unsigned char w[16 * (ROUNDS + 1)];

  keyExpansion(key, w);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = plaintext[i + 4 * j];
    }
  }
  addRoundKey(state, w);

  for (int i = 0; i < ROUNDS - 1; i++) {
    subBytes(state);
    shiftRows(state);
    mixColumns(state);
    addRoundKey(state, w + (i+1) * 16);

  }

  subBytes(state);
  shiftRows(state);
  addRoundKey(state, w + ROUNDS * 16);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ciphertext[i + 4 * j] = state[i][j];
    }
  }
  return ciphertext;
}

// Adapted for Exercise 7, Set 1
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

// int main(int argc, char* argv[]) {
//
//   unsigned char out[16];
//   unsigned char in[16];
//   unsigned char key[16];
//   decodeHex(in, "00112233445566778899aabbccddeeff");
//   decodeHex(key, "000102030405060708090a0b0c0d0e0f");
//
//   aes_encrypt(in, key, out);
//   cout << encodeHex(out, 16) << endl;
//
//   cout << "decryption" << endl;
//   aes_decrypt(out, key, in);
//   cout << encodeHex(in, 16) << endl;
//
//   return 0;
// }
