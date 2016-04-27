#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

// global変数
// tの最大インデックス
const int n = 100;
  // q, pの最大インデックス
const int l = 200, m = 200;
// tの差分化サイズ
const double ht = 0.001;
// 位相空間の差分化サイズ
const double hqp = 0.25;
// 置き換えパラメータ
const double k1 = ht/hqp;
// 系のサイズ
const double L = hqp*l;
  
class variable{
public:
  variable();
  ~variable();
  // Wigner関数f[q][p]
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
      double q = -L/2.0 + i*hqp;
      double p = -L/2.0 + j*hqp;
      f[i][j] = 1.0/M_PI*exp(-(q+2)*(q+2) -p*p);
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
  // 時間発展の結果をf_tmpに代入
  for(int i = 2; i < l-2; i++){
    for(int j = 2; j < m-2; j++){
      double q = -L/2.0 + i*hqp;
      double p = -L/2.0 + j*hqp;
      // k1 = 0.04
      //v.f_tmp[i][j] = v.f[i][j] + k1*hqp*(q*v.f[i][j+1] - p*v.f[i+1][j]) + k1*hqp*(-q + p)*v.f[i][j];
      v.f_tmp[i][j] = v.f[i][j] + k1*hqp*(-p*(v.f[i-2][j] - 8*v.f[i-1][j] + 8*v.f[i+1][j] - v.f[i+2][j]) + q*(v.f[i][j-2] - 8*v.f[i][j-1] + 8*v.f[i][j+1] - v.f[i][j+2]));
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
  
  variable v;
  ofstream fout("output.txt");
  
  fout << "#q" << "\t" << "p" << "\t" << "value" << endl;
  bool flag = false;
  for(int i = 0; i < 5000; i++){
    if(i%100 == 0) flag = true;
    SetPlot(v, fout, flag);
    cout << i << endl;
    flag = false;
  }
  
  return 0;
}
