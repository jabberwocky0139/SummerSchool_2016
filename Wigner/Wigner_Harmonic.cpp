#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

// global変数
// tの最大インデックス
const int n = 100;
  // q, pの最大インデックス
const int l = 1000, m = 600;
// tの差分化サイズ
const double ht = 0.1;
// 位相空間の差分化サイズ
const double hqp = 0.01;
// 置き換えパラメータ
const double k1 = ht/hqp;
// 系のサイズ
const double L = hqp*l;
  
class variable{
public:
  variable();
  ~variable();
  // Wigner関数f[q][p]
  //double f[l][m];
  double** f;
  double** f_tmp;
};

// ガウス関数で初期化
variable::variable(){
  f = new double*[m];
  f_tmp = new double*[m];
  for(int i = 0; i < m; i++){
    f[i] = new double[l];
    f_tmp[i] = new double[l];
  }
  for(int i = 0; i < l; i++){
    for(int j = 0; j < m; j++){
      double q = -L/2 + i*hqp;
      double p = -L/2 + j*hqp;
      f[i][j] = 1.0/M_PI*exp(-(q+1)*(q+1) - 0.5*p*p);
      f_tmp[i][j] = 0;
    }
  }
}

variable::~variable(){
  for(int i = 0; i < m; i++){
    delete[] f[i];
    delete[] f_tmp[i];
  }
  delete[] f;
  delete[] f_tmp;
}

void TimeEvolution(variable& v){
  double f_tmp[l][m] = {0};
  
  // 時間発展の結果をf_tmpに代入  
  for(int i = 1; i < l-1; i++){
    for(int j = 1; j < m-1; j++){
      double q = -L/2.0 + i*hqp;
      double p = -L/2.0 + j*hqp;
      v.f_tmp[i][j] = k1*(q*v.f[i][j+1] - p*v.f[i+1][j]) + (k1*(-q + p) + 1)*v.f[i][j];
    }
  }
  // t_tmpをv.fにコピー
  for(int i = 0; i < l; i++){
    for(int j = 0; j < m; j++){
      v.f[i][j] = v.f_tmp[i][j];
    }
  }
}

void SetPlot(variable& v, ofstream& fout, bool flag){
  if(flag){
    for(int i = 0; i < l; i++){
      for(int j = 0; j < m; j++){
	fout << -L/2.0 + i*hqp << "\t" << -L/2.0 + j*hqp << "\t" << v.f[i][j] << endl;
      }
    }
    fout << endl;
  }
  TimeEvolution(v);
}

int main(void){
  /*
  variable v;
  ofstream fout("output.txt");
  
  fout << "#q" << "\t" << "p" << "\t" << "value" << endl;
  bool flag = true;
  for(int i = 0; i < 30; i++){
    //if(i > 30) flag = true;
    SetPlot(v, fout, flag);
    cout << i << endl;
  }
  */

  
  /*以下テストプログラム*/

  double f[l];
  double f_tmp[l] = {0};
  ofstream fout("output_test.txt");
  // 初期関数
  for(int i = 0; i < l; i++){
    double q = -L/2.0 + i*hqp;
    f[i] = exp(-(q-1)*(q-1));
    fout << q << "\t" << f[i] << endl;
  }
  fout << endl;

  // 時間発展
  for(int j = 0; j < 60; j++){
    for(int i = 1; i < l-1; i++){
      double q = -L/2.0 + i*hqp;
      double k = ht/(hqp*hqp);
      f_tmp[i] = -k/2.0*(f[i+1] + f[i-1]) + (ht/2.0*q*q + k + 1)*f[i];
      fout << q << "\t" << f_tmp[i] << endl;
    }
    fout << endl;
    for(int i = 0; i < l; i++){
      f[i] = f_tmp[i];
    }
  }
  return 0;
}
