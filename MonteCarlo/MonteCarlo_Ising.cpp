#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
using namespace std;
const int N = 21;
// 何処まで境界にする？
const int periodicN = 1;

class Spin{
public:
  Spin();
  int angle[N];
  int Energy;
};

// 初期値設定
Spin::Spin(){
  
  // 境界条件  
  angle[0] = 1;
  angle[N-1] = -1;

  // 乱数の用意
  random_device rd;
  mt19937 mt(rd());
  uniform_real_distribution<double> rand_spin(-1, 1);
  
  // スピンをランダムに初期化
  Energy = 0;
  for(int i = periodicN; i < N-periodicN; i++){
    if(rand_spin(mt) > 0) angle[i] = 1;
    else angle[i] = -1;
    // ソリトンの用意(debug)
    /*
    if(i < 50) angle[i] = 1;
    else angle[i] = -1;
    */
    Energy -=  angle[i]*angle[i-1];
  }
  Energy -= angle[N-2]*angle[N-1];
}

// MonteCalro 1ループ
void MonteCarlo(Spin& s){
  // 何処のサイトを弄るか
  random_device rd;
  mt19937 mt(rd());
  uniform_int_distribution<int> rand_site(periodicN, N-1-periodicN);
  int site = rand_site(mt);
  
  // となりのスピンとの内積を取ってエネルギーが最小になる(内積が最大になる)ように選ぶ
  int EnergyP = s.angle[site+1] + s.angle[site-1];
  int EnergyN = -(s.angle[site+1] + s.angle[site-1]);
  if(EnergyP > EnergyN) s.angle[site] = 1;
  else if(EnergyP > EnergyN) s.angle[site] = -1;
  
  // エネルギーの更新
  s.Energy = 0;
  for(int i = 1; i < N; i++) s.Energy -=  s.angle[i]*s.angle[i-1];
}

// Spinオブジェクトの全情報をOutput
void Output(ofstream& fout, const Spin& s){
  fout << "#i" << "\t" <<  "angle" << endl;
  for(int i = 0; i < N; i++) fout << i << "\t" << s.angle[i] << endl;
  fout << endl;
  cout << "Energy: " << s.Energy << endl;
}

int main(void){
  Spin s;
  ofstream fout("output_ising.txt");

  // 初期値をOutput
  Output(fout, s);
  
  // MonteCalro何回やる？
  for(int i = 0; i < 1e4; i++) MonteCarlo(s);

  // MonteCalro実行後にOutput
  Output(fout, s);
  
  return 0;
}
