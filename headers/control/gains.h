#ifndef _MY_COEF_H
#define _MY_COEF_H


#define Fac_LQT 1000000
#define Ku1 -0.0635
#define Ku2 6.3123
#define Ku3 1.3009
#define Ku4 -11.3775
#define Ku5 0.5781
#define m 0.0001
#define Fac_PID 10000
#define Ku1_1 1010
#define Ku1_2 -2.0100e+03
#define Ku1_3 1000
#define Ku1_4 1
#define Ku1_5 0
#define K -40
#define pi 3.14159265
//#define TIME_MESURE 100000000

void LQT_control(float rk_rad, int x1_rad, int x3_rad);
void PID_controler(float r1_rad, int r2_rad, int x1_rad, int x3_rad);

using namespace std;

#endif
