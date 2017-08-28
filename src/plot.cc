#include<iostream>
#include<fcntl.h>//O_CRATE
#include<unistd.h>//ftruccate
#include<sys/mman.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<cmath>
#include<cassert>
#include<thread>
#include"autothread.h"
#include"plot.h"
using namespace std;

valarray<float> linspace(float start, float stop, int size)
{
	valarray<float> v(size);
	for(int i=0; i<size; i++) v[i] = start + i * (stop-start)/size;
	return v;
}

std::valarray<float> arange(float start, float step, float stop)
{
	int size = (stop - start) / step;
	valarray<float> v(size);
	for(int i=0; i<size; i++) v[i] = start + step * i;
	return v;
}

string psstm(string command)
{//return system call output as string
	string s;
	char tmp[1000];
	FILE* f = popen(command.c_str(), "r");
	while(fgets(tmp, sizeof(tmp), f)) s += tmp;
	pclose(f);
	return s;
}

string plot(const valarray<float>& x, const valarray<float>& y)
{
	int sz = x.size();
	assert(sz == y.size());
	int bytes = sz * sizeof(float) * 2;
	const char* name = "plot1";
	int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, bytes);
	float* ptr = (float*)mmap(0, bytes, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	for(int i=0; i<sz; i++) {
		*ptr++ = x[i];
		*ptr++ = y[i];
	}
	
	string command = "plot.py ";
	string s = psstm(command + to_string(sz));
	shm_unlink(name);
	return s;
}

string plot(const valarray<float>& x, const valarray<complex<float>>& y)
{
	valarray<float> z(y.size());
	for(int i=0; i<z.size(); i++) z[i] = y[i].real();
	return plot(x, z);
}

complex<float> DFT1(const valarray<float>& x, float w)
{//discrete fourier transform
	complex<float> im = 0;
	for(float i=0; i<x.size(); i++) im += x[i] * exp(-1if * w * i);//sampling rate
	return im;
}

valarray<complex<float>> DFT(const valarray<float>& x, const valarray<float>& w)
{
	AutoThread at;
	vector<future<complex<float>>> vf;
	valarray<complex<float>> v(w.size());
	for(int i=0; i<v.size(); i++) vf.push_back(at.add_thread(bind(DFT1, x, w[i])));
	for(int i=0; i<v.size(); i++) v[i] = vf[i].get();
	return v;
}

complex<float> IDFT(const std::valarray<std::complex<float>>& Xw, int n)
{//inverse DFT
	float dw = 2 * M_PI / Xw.size();
	complex<float> im = 0;
	for(float w=-M_PI, i=0; w<M_PI; w+=dw, i++) 
		im += Xw[i] * exp(1if * w * (float)n) * dw;
	return im / 2.f / (float)M_PI;
}

 
// Cooley–Tukey FFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive
void fft(valarray<complex<float>>& x)
{
	const float PI = 3.141592653589793238460;

	typedef std::complex<float> Complex;
	typedef std::valarray<Complex> CArray;

    const size_t N = x.size();
    if (N <= 1) return;
 
    // divide
    CArray even = x[std::slice(0, N/2, 2)];
    CArray  odd = x[std::slice(1, N/2, 2)];
 
    // conquer
    fft(even);
    fft(odd);
 
    // combine
    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0f, -2 * PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}
// inverse fft (in-place)
void ifft(valarray<complex<float>>& x)
{
    // conjugate the complex numbers
    x = x.apply(std::conj);
 
    // forward fft
    fft( x );
 
    // conjugate the complex numbers again
    x = x.apply(std::conj);
 
    // scale the numbers
    x /= x.size();
}
