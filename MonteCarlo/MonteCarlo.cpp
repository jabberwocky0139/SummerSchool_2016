#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
using namespace std;
const int N = 101;
// 何処まで境界にする？
const int periodicN = 1;
// 何個離れたのスピンの相関まで考える？
int intNum = 2;

class Spin{
public:
  Spin();
  double x[N];
  double y[N];
  double value[N];
  double angle[N];
  double Energy;
};

// 初期値設定
Spin::Spin(){
  // 境界条件
  Energy = 0;
  for(int i = 0; i < periodicN; i++){
    x[i] = 0;
    y[i] = 1;
    value[i] = 1;
    angle[i] = M_PI/2.0;
    x[N-1-i] = 0;
    y[N-1-i] = -1;
    value[N-1-i] = -1;
    angle[N-1-i] = -M_PI/2.0;
  }
  // スピンをランダムに初期化
  random_device rd;
  mt19937 mt(rd());
  uniform_real_distribution<double> rand_spin(-M_PI/2.0, M_PI/2.0);
  for(int i = periodicN; i < N-1-periodicN; i++){
    double theta = rand_spin(mt);
    x[i] = cos(theta);
    y[i] = sin(theta);
    value[i] = theta/(M_PI/2.0);
    angle[i] = theta;
    // 初期値にソリトンを持ってくる場合(debug)
    /*
    if(i < 50){
      x[i] = 0;
      y[i] = 1;
      value[i] = 1;
      angle[i] = M_PI/2.0;
    }else{
        x[i] = 0;
	y[i] = -1;
	value[i] = -1;
	angle[i] = -M_PI/2.0;
    }
    */
    Energy -= x[i]*x[i-1] + y[i]*y[i-1];
  }
  Energy -= x[N-1]*x[N-2] + y[N-1]*y[N-2];
}

// MonteCalro 1ループ
void MonteCarlo(Spin& s){
  // 何処のサイトを弄るか
  random_device rd;
  mt19937 mt(rd());
  uniform_int_distribution<int> rand_site(periodicN, N-1-periodicN);
  int site = rand_site(mt);
  
  // となりのスピンとの内積を取ってエネルギーが最小になる(内積が最大になる)ように選ぶ
  // 角度をangleN分割
  const int angleN = 100;
  int minIndex = site;
  double Energy, minEnergy;
  for(int i = 1; i < intNum; i++){
    if(site+i < N and site-i >= 0){
      minEnergy = -(s.x[site+i]*s.x[site] + s.y[site+i]*s.y[site])/double(i*i);
      minEnergy += -(s.x[site-i]*s.x[site] + s.y[site-i]*s.y[site])/double(i*i);
    }
  }
  for(int i = 0; i <= angleN; i++){
    double theta = -M_PI/2.0 + i*M_PI/double(angleN);
    // intNum個離れたのスピンの相関まで考える
    for(int j = 1; j < intNum; j++){
      if(site+j < N and site-j >= 0){
	Energy = -(s.x[site+j]*cos(theta) + s.y[site+j]*sin(theta))/double(j*j);
	Energy += -(s.x[site-j]*cos(theta) + s.y[site-j]*sin(theta))/double(j*j);
      }
    }
    if(Energy < minEnergy){
      minIndex = i;
      minEnergy = Energy;
    }
  }
  
  // 更新
  // minIndexが更新されなければ更新せずにそのまま出る
  if(site == minIndex) return;
  double theta = -M_PI/2.0 + minIndex*M_PI/double(angleN);
  s.angle[site] = theta;
  s.x[site] = cos(theta);
  s.y[site] = sin(theta);
  s.value[site] = theta/(M_PI/2.0);
  s.Energy = 0;
  for(int i = 0; i < N; i++){
    for(int j = 1; j < intNum; j++){
      if(i+j < N and i-j >= 0){
	s.Energy -= (s.x[i]*s.x[i+j] + s.y[i]*s.y[i+j])/double(j*j);
	s.Energy -= (s.x[i]*s.x[i-j] + s.y[i]*s.y[i-j])/double(j*j);
      }
    }
  }
  //s.Energy -= s.x[N-1]*s.x[N-2] + s.y[N-1]*s.y[N-2];
}

// Spinオブジェクトの全情報をOutput
void Output(ofstream& fout, const Spin& s){
  fout << "#i" << "\t" << "s.x[i]" << "\t" << "s.y[i]" << "\t" << "value" << "\t" << "angle" << endl;
  for(int i = 0; i < N; i++) fout << i << "\t" << s.x[i] << "\t" << s.y[i] << "\t" << s.value[i] << "\t" << s.angle[i] << endl;
  fout << endl;
  cout << "Energy: " << s.Energy << endl;
}

int main(void){
  Spin s;
  ofstream fout("output.txt");

  // 初期値をOutput
  Output(fout, s);
  
  // MonteCalro何回やる？
  for(int i = 0; i < 1e5; i++) MonteCarlo(s);

  // MonteCalro実行後にOutput
  Output(fout, s);
  
  return 0;
}
