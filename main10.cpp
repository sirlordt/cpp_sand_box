#include <iostream>
//#include <string>
//#include <cstring>
#include <utility>
#include <map>
#include <stdexcept>

#define __DEBUG__     true
#define __DEBUG_OUT__ std::cout

template<class T>
class vector
{
	int sz;		// number of elements
	T* elem;	// address of first element
	int space;	// number of elements plus free-space

public:
	vector() : sz(0), elem(0), space(0) {}
	vector(int s) : sz(s), elem(new T[s]), space(s) {
		for(int i = 0; i < sz; ++i) elem[i] = 0;
	}
	vector(const vector&);
	vector& operator=(const vector & v);

	~vector() { delete[] elem; }

	T& at(int n);
	const T& at(int n) const;

	T& operator[](int n);
	const T& operator[](int n) const;

	int size() const { return sz; }
	int capacity() const { return space; }

	void reserve(int alloc_size);
	void resize(int resize_size);
	void push_back(const T& d);
};

template<class T>
void vector<T>::reserve(int alloc_size) {
	if(alloc_size <= space) return;
	T *p = new T[alloc_size];
	for(int i = 0; i < sz; ++i) p[i] = elem[i];
	delete[] elem;
	elem = p;
	space = alloc_size;
}

template<class T>
T& vector<T>::at(int n) {
	if(n < 0 || sz <= n) throw std::out_of_range( "" );
	return elem[n];
}

template<class T>
const T& vector<T>::at(int n) const {
	if(n < 0 || sz <= n) throw std::out_of_range( "" );
	return elem[n];
}

template<class T>
T& vector<T>::operator[](int n) {
	return elem[n];
}

template<class T>
const T& vector<T>::operator[](int n) const {
	return elem[n];
}

template<class T>
vector<T>& vector<T>::operator=(const vector<T> & v) {
	if(this == &v) return *this;

	if(v.sz <= space) {
		for(int i = 0; i < v.sz; ++i)
			elem[i] = v.elem[i];
		sz = v.sz;
		return *this;
	}

	T *p = new T[v.sz];
	for(int i = 0; i < v.sz; ++i)
		p[i] = v.elem[i];
	delete[] elem;
	space = sz = v.sz;
	elem = p;
	return *this;
}

template<class T>
void vector<T>::resize(int resize_size) {
	reserve(resize_size);
	for(int i = 0; i < resize_size; ++i) elem[i] = 0;
	sz = resize_size;
}

template<class T>
void vector<T>::push_back(const T& d){
	if(space == 0)
		reserve(10);
	else if(sz == space)
		reserve(2*space);
	elem[sz] = d;
	++sz;
}

int main( int argc, char *argv[] )
{

  #ifdef __DEBUG__

    __DEBUG_OUT__ << "[begin][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

  vector<double> dv(3);
	dv.resize(5);

	for(int i = 0; i < dv.size(); ++i) {

  	std::cout << dv[i] << " " << std::endl << std::endl;

	}

  #ifdef __DEBUG__

    __DEBUG_OUT__ << "[end][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

}
