#define BUFSIZE 1000000
int Tests, cnum;
// #define USEWIN
#define MANYTESTS 1
// #define LINEBYLINE

#include <algorithm>
#include <string>
#include <stdio.h>
using namespace std;

typedef long long ll;

#define LS <
#define Size(x) (int((x).size()))

// All macros with parameters "k,a,b" run the "k" variable in range [a,b)
#define FOR(k,a,b) for(auto k=(a); k LS (b); ++k)
// the range is traversed from b-1 to a in FORREV
#define FORREV(k,a,b) for(auto k=(b); (a) <= (--k);)

string getLine() {
  string s;
  while(!feof(stdin)) {
    char c = fgetc(stdin);
    if(c == 13) continue;
    if(c == 10) return s;
    s += c;
    }
  return s;
  }

int scanerr;

int getNum() {
#ifdef LINEBYLINE
  string s = getLine();
  return atoi(s.c_str());
#else
  int i;
  scanerr = scanf("%d", &i);
  return i;
#endif
  }

#ifndef BUFSIZE
#define BUFSIZE 1000000
#endif

char buf[BUFSIZE];

#line 9 "work.cpp"

/// ----


//Eryx

// !FDI

ll wgts[1000000];

ll lightest[1000000];

#define XINF 1000000000000000ll

void solveCase() {
  int res = 0;

  int N = getNum();
  FOR(i,0,N) wgts[i] = getNum();
  
  lightest[0] = 0;
  FOR(i,1,1000000) lightest[i] = XINF;
  
  int most = 0;
  
  FOR(i,0,N) {
    FORREV(j,1,most+2)  {
      if(lightest[j-1] <= wgts[i]*6 && lightest[j-1] + wgts[i] < lightest[j])
        lightest[j] = lightest[j-1] + wgts[i];
      if(lightest[j] < XINF && j > most) most = j;
      }
    // FOR(j,0,i+2) printf("%d: %Ld\n", j, lightest[j]);
    }
      
  int best = 0;
  while(lightest[best+1] < XINF) best++;
  
  printf("Case #%d: %d\n", cnum, best);
  }

#define P 1000000007

int main() {

  if(!MANYTESTS) Tests = 1;
  else Tests = getNum();
  
  for(cnum=1; cnum<=Tests; cnum++)
    solveCase();
    
  // finish
  return 0;
  }

// This solution includes hidden routines to solve test cases in separate
// processes in order to make it faster. I will update them to run on a
// cluster if I get one ;)
// See https://github.com/eryxcc/templates
