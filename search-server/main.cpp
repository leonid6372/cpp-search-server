#include <iostream>
using namespace std;

int main() {
	const int from_number = 1;
  	const int to_number = 1000;
    int counter = 0;
    
    for(int i = from_number; i <= to_number; ++i){
    	if(i % 10 == 3){
        	++counter;
        }else if(i % 100 / 10 == 3){
        	++counter;
        }else if(i / 100 == 3){
        	++counter;
        }
    }
    
    cout << counter;
  return 0;
}
