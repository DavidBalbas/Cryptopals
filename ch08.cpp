#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Exercise 8
int main(int argc, char* argv[]) {
  string line;

  ifstream inFile(argv[1]);
  if(inFile.is_open()) {

    while(getline(inFile, line)){
      for(int i = 0; i < line.length(); i += 32){
        string a = line.substr(i, 32);
        for(int j = i + 32; j < line.length(); j += 32){
          string b = line.substr(j, 32);
          if(! a.compare(b)){
            cout << line << endl;
          }
        }
      }
    }
    inFile.close();
  }

  return 0;
}
