#include <algorithm>
#include "ecbOracles.cpp"


string appendStrings_aux(string in, unsigned char* key, unsigned char* iv){
  string pre = "comment1=cooking%20MCs;userdata=";
  string post = ";comment2=%20like%20a%20pound%20of%20bacon";
  in.erase(remove(in.begin(), in.end(), '='), in.end());
  in.erase(remove(in.begin(), in.end(), ';'), in.end());
  string all = pre + in + post;
  cout << all << endl;
  all = autoPadPKCS7(all);

  // Encryption
  unsigned char plaintext[all.length()];
  unsigned char ciphertext[all.length()];
  plaintextToBytes(plaintext, all, all.length());

  aes_encrypt_cbc(plaintext, key, ciphertext, iv, all.length());
  return UnsignedCharToString(ciphertext, all.length());
}

int searchForAdmin(string in, unsigned char* key, unsigned char* iv){
  unsigned char plaintext[in.length()];
  unsigned char ciphertext[in.length()];
  plaintextToBytes(ciphertext, in, in.length());
  aes_decrypt_cbc(ciphertext, key, plaintext, iv, in.length());
  string out = UnsignedCharToString(plaintext, in.length());
  out = unpadPKCS7(out);
  cout << out << endl;
  string search = ";admin=true;";
  for(int i = 0; i < in.length() - search.length(); i++){
    if(!search.compare(out.substr(i, search.length()))) {
      return 1;
    }
  }
  return 0;
}

int injectBitflipping(unsigned char* key, unsigned char* iv){
  int lenBlock = 16;
  //pre is exactly two blocks long. Otherwise we would pad
  string b1 = "1234567890123456";
  string b2 = "0admin0true00000"; //< is = + 1
  string ciphertext = appendStrings_aux(b1+b2, key, iv);
  //xor character in b2 with the aimed character. The error propagates
  ciphertext[32] ^= ('0' ^ ';');
  ciphertext[38] ^= ('0' ^ '=');
  ciphertext[43] ^= ('0' ^ ';');

  return searchForAdmin(ciphertext, key, iv);
}


int main(int argc, char* argv[]) {
  unsigned char key[16];
  unsigned char iv[16];
  generateKey(key);
  // test code: everything works
  // string a = "helloIamSth";
  // string b = ";admin=true;haha";
  // string acheck = appendStrings_aux(a, key, iv);
  // string bcheck = appendStrings_aux(b, key, iv);
  // cout << acheck << endl;
  // cout << bcheck << endl;
  // int adesc = searchForAdmin(acheck, key, iv);
  // int bdesc = searchForAdmin(bcheck, key, iv);
  // cout << adesc << endl;
  // cout << bdesc << endl;
  int ind = injectBitflipping(key, iv);
  cout << "Result (1 if correct):  " << ind << endl;

  return 0;
}
