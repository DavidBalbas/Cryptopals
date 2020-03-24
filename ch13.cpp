#include "ecbOracles.cpp"
#include <map>
#include <algorithm>

using namespace std;

map <string, string> parsingKV(string input){
  map <string, string> parsed;
  vector <string> blocks;
  size_t ini = 0;
  size_t end = 0;
  while((end = input.find('&', ini)) < input.length()){
    blocks.push_back(input.substr(ini, end - ini));
    ini = end + 1;
  }
  if(ini < input.length() - 1){
    blocks.push_back(input.substr(ini, (input.length() - ini)));
  }
  // for (auto i = blocks.begin(); i != blocks.end(); ++i){
  //   std::cout << *i << endl;
  // }
  string key;
  string val;
  for (auto i = blocks.begin(); i != blocks.end(); ++i){
    if((ini = i->find('=', 0)) < i->length()){
      key = i->substr(0, ini);
      val = i->substr(ini + 1, i->length());
      parsed.insert(pair<string,string>(key,val));
      // cout << key;
      // cout << val;
    }
  }
  return parsed;
}

string profile_for(string email){
  email.erase(remove(email.begin(), email.end(), '='), email.end());
  email.erase(remove(email.begin(), email.end(), '&'), email.end());
  return "email=" + email + "&uid=10&role=user";
}

string padAndEcbString(string str, unsigned char* key){
  str = autoPadPKCS7(str);
  int len = str.length();

  unsigned char plaintext[len];
  unsigned char ciphertext[len];
  plaintextToBytes(plaintext, str, len);
  aes_encrypt_ecb(plaintext, key, ciphertext, len);
  return UnsignedCharToString(ciphertext, len);
}

string unpadAndDecEcbString(string str, unsigned char* key){
  int len = str.length();
  unsigned char plaintext[len];
  unsigned char ciphertext[len];
  plaintextToBytes(ciphertext, str, len);
  aes_decrypt_ecb(ciphertext, key, plaintext, len);
  return unpadPKCS7(UnsignedCharToString(plaintext, len));
}

int main(int argc, char* argv[]) {
  unsigned char key[16];
  generateKey(key);

  string toParse = "foo=bar&baz=qux&zap=zazzle";
  std::map <string, string> parsed = parsingKV(toParse);
  cout << "Original:  " << profile_for("foo@bar.com") << endl;
  cout << "With added &, =:  " << profile_for("foo@ba=&r&&.com") << endl;

  string userProfile = profile_for("foo@bar.com");
  // userProfile = autoPadPKCS7(userProfile);
  // int len = userProfile.length();
  //
  // unsigned char plaintext[len];
  // unsigned char ciphertext[len];
  // plaintextToBytes(plaintext, userProfile, len);
  // aes_encrypt_ecb(plaintext, key, ciphertext, len);
  // string encryptedUser = UnsignedCharToString(ciphertext, len);
  string encryptedUser = padAndEcbString(userProfile, key);
  cout << "Encrypted:  " << encryptedUser << endl;

  // aes_decrypt_ecb(ciphertext, key, plaintext, len);
  // string decrypted = unpadPKCS7(UnsignedCharToString(plaintext, len));
  string decrypted = unpadAndDecEcbString(encryptedUser, key);
  cout << "Decrypted and unpadded:  " << decrypted << endl;

  parsed = parsingKV(decrypted);
  cout << "Parsed cookie:" << endl;
  for (map<string, string>::iterator iter = parsed.begin(); iter != parsed.end(); ++iter) {
    cout << iter->first << '\t' << iter->second << endl;
  }

  // Nice maps and strings exercise. Now it's when the real fun comes up.

  // First: cipher admin and a valid pkcs7 pad
  // Second: make the string from email to role= a multiple of 16 long
  // Third: replace last block. Voila!
  unsigned char adminPlaintext[16];
  string evil = "admin";
  evil = autoPadPKCS7(evil);
  string encryptedEvil = padAndEcbString(profile_for("abcdabcdab" + evil), key);
  // plaintextToBytes(adminPlaintext, evil, 16);
  // We need email=foo@bar.com&uid=10&role= to be 32 char long.
  // this is given to us encrypted
  encryptedUser = padAndEcbString(profile_for("iamevil@ha.ha"), key);
  string hackedCookie;
  for(int i = 0; i < encryptedUser.length() - 16; i++){
    hackedCookie += encryptedUser[i];
  }
  for(int i = 16; i < 32; i++){
    hackedCookie += encryptedEvil[i];
  }
  string decryptedHackedCookie = unpadAndDecEcbString(hackedCookie, key);
  cout << "Hacked cookie (unparsed):" << endl;
  cout << decryptedHackedCookie << endl;
  cout << "Parsed hacked cookie:" << endl;
  parsed = parsingKV(decryptedHackedCookie);
  for (map<string, string>::iterator iter = parsed.begin(); iter != parsed.end(); ++iter) {
    cout << iter->first << '\t' << iter->second << endl;
  }
  return 0;
}
