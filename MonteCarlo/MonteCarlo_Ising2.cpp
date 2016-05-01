#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <cstdio>
extern "C"{
#include <omp.h>
}
using namespace std;

// 系のサイズ
const int N = 16;

// 乱数の用意
random_device rd;
mt19937 mt(rd());
uniform_int_distribution<int> rand_site1(0, N-1);
uniform_real_distribution<double> rand_site2(0, 1);
uniform_int_distribution<int> rand_spin3(0, 1);

// 相互作用定数
double J = 1.0;
// 外部磁場
double H = 0.05;

class Spin{
  
public:
  Spin();
  ~Spin();
  double SetSpin(int siteX, int siteY);
  double **angle;
  double Energy;
  double M;
  double T;
  
};


// 初期値設定
Spin::Spin(){

  // 系の温度の初期設定
  T = 0.0001;
  
  // スピンを動的確保
  angle = new double*[N];
  for(int i = 0; i < N; i++) angle[i] = new double[N];
  
  // 境界条件(必要なら書くべし)
  
  // スピンをランダムに初期化
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      //angle[i][j] = rand_spin3(mt) == 0 ? 1 : -1;
      // debug
      angle[i][j] = 1;
    }
  }
  
  // エネルギーを設定
  Energy = 0;
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      Energy += -J*SetSpin(i, j)*(SetSpin(i, j+1) + SetSpin(i, j-1) + SetSpin(i+1, j) + SetSpin(i-1, j));
      //Energy +=  - H*SetSpin(i, j);
    }
  }
  
}


Spin::~Spin(){
  
  for(int i = 0; i < N; i++){
    delete[] angle[i];
  }
  delete[] angle;
  
}


inline double Spin::SetSpin(int siteX, int siteY){

  // 周期的境界条件
  if(siteX == -1) siteX = N-1;
  if(siteY == -1) siteY = N-1;
  if(siteX == N) siteX = 0;
  if(siteY == N) siteY = 0;
  return angle[siteX][siteY];
  
}

// MonteCalro 1ループ
inline void MonteCarlo(Spin& s){

  // 何処のサイトを弄るか
  int siteX = rand_site1(mt);
  int siteY = rand_site1(mt);
  
  // となりのスピンとの内積を取ってエネルギーが最小になる(内積が最大になる)ように選ぶ
  double EnergyBefore = -J*s.SetSpin(siteX, siteY)*(s.SetSpin(siteX, siteY-1) + s.SetSpin(siteX, siteY+1) + s.SetSpin(siteX-1, siteY) + s.SetSpin(siteX+1, siteY));
  //EnergyBefore += -H*s.SetSpin(siteX, siteY);
  double EnergyAfter = -EnergyBefore;
  if(EnergyBefore > EnergyAfter){
    // 反転したほうがエネルギーが小さく系が安定. スピン反転
    s.angle[siteX][siteY] = -s.angle[siteX][siteY];
  }else{
    // 反転してもエネルギーが変わらないかエネルギーが大きくなる.
    // 確率的にスピン反転を許可する. 
    if(exp(-(EnergyAfter-EnergyBefore)/s.T) > rand_site2(mt)) s.angle[siteX][siteY] = -s.angle[siteX][siteY];
  }  
  
}

// 磁化を設定
inline void SetMag(Spin& s){
  
  s.M = 0;
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      s.M += s.angle[i][j];
    }
  }
  s.M /= N*N;
  
}

// エネルギーを設定
inline void SetEnergy(Spin& s){
  
  s.Energy = 0;
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      s.Energy += -J*s.SetSpin(i, j)*(s.SetSpin(i, j+1) + s.SetSpin(i, j-1) + s.SetSpin(i+1, j) + s.SetSpin(i-1, j));
      //s.Energy +=  - H*s.SetSpin(i, j);
    }
  }
  s.Energy /= N*N;
  
}


// Spinオブジェクトの全情報をOutput
inline void Output(ofstream& fout, const Spin& s){
  
  fout << "#i" << "\t" << "j" << "\t" <<  "angle[i][j]" << endl;
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      fout << i << "\t" << j << "\t" <<  s.angle[i][j] << endl;
    }
  }
  fout << endl;
  cout << "Energy: " << s.Energy << endl;
  
}


// 磁化とエネルギーの出力
inline void Output_MC(ofstream& fout_MC, Spin& s){

  fout_MC << s.T << "\t" << s.Energy << "\t" <<  s.M << endl;
  fflush(stdout);
  
}


int main(void){
  
  ofstream fout("output_ising2_3e-0.txt");
  ofstream fout_MC("output_ising2_MC3.txt");

  // 磁化・比熱の出力
  fout_MC << "#T" << "\t" << "E" << "\t" <<  "M" << endl;
  int Nequ = 50;
  Spin s[50];
  for(int i = 0; i < 50; i++){
    for(int j = 0; j < Nequ; j++) s[j].T = i*0.1 + 0.0001;
    cout << "-*- T = " << s[0].T << " -*-" << endl;
    double mag = 0, E = 0;

    for(int j = 0; j <= 1e7; j++){
      for(int k = 0; k < Nequ; k++){
	MonteCarlo(s[k]);
      }
      if(j%10000 == 0) cout << j/10000 << "-";
      fflush(stdout);
    }
    for(int k = 0; k < 1000; k++){
      for(int j = 0; j < Nequ; j++){
	MonteCarlo(s[j]);
	SetMag(s[j]);
	SetEnergy(s[j]);
	mag += s[j].M/Nequ/1000;
	E += s[j].Energy/Nequ/1000;
      }
    }
    
    s[0].M = mag;
    s[0].Energy = E;
    cout << endl;
    Output_MC(fout_MC, s[0]);
  }
  
  
  // スピンの出力
  /* 
  // 初期値をOutput
  Output(fout, s);
  
  // MonteCalro何回やる？
  for(int i = 0; i < N*N*50; i++){      
    cout << i << endl;
    MonteCarlo(s);      
  }
  
  // MonteCalro実行後にOutput
  Output(fout, s);
  */
  
  return 0;
  
}
