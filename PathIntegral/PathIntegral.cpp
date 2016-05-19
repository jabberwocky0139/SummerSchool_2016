#include <iostream>
#include <cmath>
#include <fstream>
#include <random>
using namespace std;
// 空間分割数
const int Nq = 200;
// 空間微小量
const double eps_q = 0.02;
// 空間サイズ
const double L = Nq*eps_q;
// 時間分割数
const int Nt = 200;
// 時間微小量
const double eps_t = 0.1;
// 乱数の用意
random_device rd;
mt19937 mt(rd());
uniform_int_distribution<int> rand_site(0, Nq-1);
uniform_int_distribution<int> rand_time(0, Nt-1);
uniform_real_distribution<double> rand_flip(0, 1);

class Variable{
  
public:
  Variable();
  ~Variable();
  int* Path;
  int** psi;

};

Variable::Variable(){
  
  // Path[Nt]
  // psi[Nq][Nt]
  Path = new int[Nt];
  psi = new int*[Nt];
  for(int i = 0; i < Nt; i++){
    psi[i] = new int[Nq];
  }

  // 初期設定
  for(int i = 0; i < Nq; i++){
    Path[i] = Nq/2;
    for(int j = 0; j < Nt; j++){
      psi[i][j] = 0;
    }
  }
  
}

Variable::~Variable(){
  
  for(int i = 0; i < Nq; i++){
    delete[] psi[i];
    
  }
  delete[] psi;
  delete[] Path;
  
}


inline double V(double x){
  return 0.5*x*x;
}


double energy(const Variable& v){
  
  double energy = 0;
  for(int i = 0; i < Nt; i++){
    energy += 0.5*pow((v.Path[i+1] - v.Path[i])*eps_q/eps_t, 2);
    energy += V(-L/2.0 + (v.Path[i+1] + v.Path[i])*eps_q/2.0);
  }
  return energy;
  //return exp(-eps_t*energy);
  
}


double expW(const Variable& v){

  return exp(-eps_t*energy(v));

}


void Flip(Variable& v){
  // どのサイトをフリップするか
  const int t = rand_time(mt);

  // どこにフリップするか
  const int flip = rand_site(mt);
  const int old_flip = v.Path[t];
  double old_expW = expW(v);
  double old_energy = energy(v);
  v.Path[t] = flip;
  
  // フリップした時のボルツマン因子を計算し, フリップを許可しない場合はもとに戻す
  cout << expW(v) << endl;
  if(energy(v) > old_energy and expW(v)/old_expW <= rand_flip(mt)) v.Path[t] = old_flip;
  
  // psiに更新
  for(int i = 0; i < Nt; i++){
    v.psi[v.Path[i]][i] += 1;
  }
  
}


void Output(Variable& v, ofstream& fout){

  fout << "#q" << "\t"<< "t" << "\t" << "psi[q][t]" << endl;
  for(int i = 0; i < Nq; i++){
    for(int j = 0; j < Nt; j++){
      fout << -L/2.0 + i*eps_q << "\t" << j*eps_t << "\t" << v.psi[i][j] << endl;
    }
  }
  fout << endl;
  
}


int main(void){

  Variable v;
  ofstream fout("output.txt");

  Output(v, fout);
  
  for(int i = 0; i < 100; i++) Flip(v);

  Output(v, fout);
  
  return 0;
}

