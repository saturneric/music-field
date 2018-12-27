#include "pch.h"
#pragma comment(lib, "E:\\Lib\\libfftw3-3.lib")
#pragma comment(lib, "E:\\Lib\\libfftw3f-3.lib")
#pragma comment(lib, "E:\\Lib\\libfftw3l-3.lib")
#include <stdio.h> 
#include <stdlib.h> 
#include "fftw3.h"  
#include <cmath>
#include <algorithm>
#include <iostream>
//假设采样频率为 FR , 采样点数为1024 (NN),那么横坐标第N个点代表的频率 X(N) = (FR / NN) * N
#define FR 44100
class Get_RGB {
public:
	static void fftw3(float *in,int NN, int &hz);
	double hztospectrun(double hz);
	static void spectral_color(double &r, double &g, double &b, double l);
	static void GetRGB( float* in,int NN, double &R, double &G,double &B){//获得RGB值
		int hz;
		double l=0;
		fftw3(in,NN, hz);
		spectral_color(R, G, B, l);
	}
};
void Get_RGB::fftw3(float *in ,int NN, int &frequency)
{
	struct Max {
		int id=0;
		int value=0;
	}M;	

	int i;
	fftwf_complex *out = NULL; // fftwf_complex --> 即为float版本
	fftwf_plan p;
	in = (float *)fftwf_malloc(sizeof(float) * NN);
	out = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) *NN);
	// 傅里叶变换
	p = fftwf_plan_dft_r2c_1d(NN, in, out, FFTW_ESTIMATE);
	fftwf_execute(p);
	// 输出幅度谱
	for (i = 0; i < NN; i++)
	{
		float len = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
		if (len > M.value) {
			M.id=i ;
		}
	//	printf("%d ",i);
	//	printf("%.2f ", len);
	}
	frequency = M.id*(FR / NN);
	//frequency = M.id*(FR / NN);
	// 释放资源


	return;
}
int main() {
	float in[10000];
	double R, G, B;
	int NN;
	std::cin >> NN;
	for (int i=0;i < NN;i++) {
		std::cin >> in[i];
	}
	Get_RGB::GetRGB(in,NN, R, G, B);
	std::cout << R << " " << G << " " << B;
	std::cout << "\n"<< R << " " << G << " " << B;
}
double  Get_RGB::hztospectrun(double hz) {
	double hbox;
	hbox = hz;
	for (;hbox > 800;hbox = hbox / 2);
	return 	hbox;
}
void  Get_RGB::spectral_color(double &r, double &g, double &b, double l) // RGB <0,1> <- lambda l <400,700> [nm]
{
	double t;  r = 0.0; g = 0.0; b = 0.0;
	if ((l >= 400.0) && (l < 410.0)) { t = (l - 400.0) / (410.0 - 400.0); r = +(0.33*t) - (0.20*t*t); }
	else if ((l >= 410.0) && (l < 475.0)) { t = (l - 410.0) / (475.0 - 410.0); r = 0.14 - (0.13*t*t); }
	else if ((l >= 545.0) && (l < 595.0)) { t = (l - 545.0) / (595.0 - 545.0); r = +(1.98*t) - (t*t); }
	else if ((l >= 595.0) && (l < 650.0)) { t = (l - 595.0) / (650.0 - 595.0); r = 0.98 + (0.06*t) - (0.40*t*t); }
	else if ((l >= 650.0) && (l < 700.0)) { t = (l - 650.0) / (700.0 - 650.0); r = 0.65 - (0.84*t) + (0.20*t*t); }
	if ((l >= 415.0) && (l < 475.0)) { t = (l - 415.0) / (475.0 - 415.0); g = +(0.80*t*t); }
	else if ((l >= 475.0) && (l < 590.0)) { t = (l - 475.0) / (590.0 - 475.0); g = 0.8 + (0.76*t) - (0.80*t*t); }
	else if ((l >= 585.0) && (l < 639.0)) { t = (l - 585.0) / (639.0 - 585.0); g = 0.84 - (0.84*t); }
	if ((l >= 400.0) && (l < 475.0)) { t = (l - 400.0) / (475.0 - 400.0); b = +(2.20*t) - (1.50*t*t); }
	else if ((l >= 475.0) && (l < 560.0)) { t = (l - 475.0) / (560.0 - 475.0); b = 0.7 - (t)+(0.30*t*t); }
}