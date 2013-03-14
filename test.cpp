#include <iostream>
using namespace std;
int main(){
	int a = 23; 
	cout << a << endl;

	int result = a ^ 31;
	cout << result << endl; 
	int is;
	for(int i = 0 ; i < 5; i++){
		result = result >> i; 
		cout << "result" << result << endl;
		is = result & 1;
		if(is){
			cout << "place: " << i+1 << endl;
		}
	}
}