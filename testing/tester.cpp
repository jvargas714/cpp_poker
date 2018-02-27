#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std; 


class base 
{
public:
	base(){
		cout << "base ctor called" << endl; 
	}
	virtual ~base(){;}
	void assess(int i){
		switch(i){
			case 1:
				functA();
				break;
			case 2:
				functB();
				break;
			default:
				cout << "Nothing called asshole" << endl; 
		}
	}
private:
	virtual void functA()=0;
	virtual void functB()=0;
};

class derived: public base 
{
public:
	derived(): base() {
		cout << "derived ctor called..." << endl; 
	}

private:
	void functA(){
		cout << "derived functA called" << endl;
	}

	void functB(){
		cout << "derived functB called" << endl; 
	}
};




int main()
{
	base* bs;
	derived der;

	bs = &der;

	bs->assess(1);
	bs->assess(2);

	return 0; 
}