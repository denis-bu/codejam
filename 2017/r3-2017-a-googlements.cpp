#include <iostream>

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <cmath>
#include <memory>
#include <exception>
#include <algorithm>
#include <limits>
#include <utility>
#include <cmath>
#include <queue>
#include <stack>
#include <fstream>
#include <functional>
#include <numeric>
#include <bitset>
#include <limits>
#include <type_traits>
#include <array>

using namespace std;
using uc = unsigned char;
using ll = long long;
using ull = unsigned long long;
using vi = vector<int>;
using vll = vector<ll>;
using vs = vector<string>;

namespace {
	const double PI = acos(-1);
	
	unordered_map<ull, ll> nck_cache;
	ull nck_key(int n, int k) {
		return ((ull)n << 32) + (ull)k;
	}
	ll nck(int n, int k) {
		if (k >  n)
			throw invalid_argument("nck: k > n");
		if (k == 0)
			return 1;
		if (k > n / 2)
			return nck(n, n - k);
		auto it = nck_cache.find(nck_key(n, k));
		if (it != nck_cache.end()) 
			return it->second;

		ll bc = nck(n - 1, k - 1) * n / k;
		nck_cache[nck_key(n, k)] = bc;
		return bc;
	}

    ll fac(int f)
    {
        ll fac = 1;
        for (int i = 2; i <= f; ++i)
            fac *= i;
        return fac;
    }

	int pop(unsigned x)
	{
		constexpr int bitwidth = std::numeric_limits<unsigned>::digits;
		std::bitset<bitwidth> bs(x);
		return bs.count();
	}

}
const size_t MAXGOOG = 9;
const size_t BASE = 10;
using googlement = array<uc, MAXGOOG>;
using googlements = vector<googlement>;
using googlement_perm = array<uc, BASE>;
    
ll polynome(const googlement_perm& perm, int size)
{
    ll f1=fac(size);
    ll f2 = 1;
    for (auto c : perm)
        f2 *= fac(c);
    
    //      cerr << "size=" <<size << ",perm=";
    //      for (auto c : perm)
    //          cerr << (int)c;
    //      cout <<endl;
    // cerr << "f1/f2=" << f1 << "/" << f2 << endl;
    return f1/f2;
}
    
void generate(const googlement_perm& perm, unsigned total, unsigned size,
        googlement& g, googlements& list)
{
    if (size == 0)
    {
        list.push_back(g);
        return;
    }
    for (uc i = 0; i < BASE; ++i)
    {
        if (perm[i] == 0)
            continue;
        googlement_perm p = perm;
        p[i] -= 1;
        g[total - size] = i;
        generate(p, total, size - 1, g, list);
    }
}

googlement_perm toperm(const googlement& g, int size)
{
    googlement_perm perm = {};
    int s = 0;
    for (int i = 0; i < size; ++i)
    {
        perm[i+1] += g[i];
        s += g[i];
    }
    perm[0] = size > s ? size - s : 0;
    // cerr << "size=" <<size << ",perm=";
    // for (auto c : perm)
    //     cerr << (int)c;
    // cerr << ",g=";
    // for (auto c : g)
    //     cerr << (int)c;
    // cout <<endl;
    return perm;
}  
  
bool terminal(const googlement& g, int size) 
{
    int sum = accumulate(g.begin(), g.end(), 0);

    //cerr << "sum=" << sum <<endl;
    return (sum > size);
}

bool terminal(const googlement_perm& perm, int size) 
{
    int sum = 0;
    for (int i = 1; i < BASE; ++i)
        sum += i * perm[i];
    //cerr << "sum=" << sum <<endl;
    return (sum > size);
}

int solve(const googlement& g, int size)
{
    // check cache
    
    // check if can't have ancestors
    if (terminal(g, size))
        return 1;
    if (terminal(toperm(g, size), size))
        return 1 + polynome(toperm(g, size), size);
    
    //: polynome(perm, size);
    // otherwise generate all ancestors and solve recursively for each
    int ancestors = 1; // self
    googlement buf = {};
    googlements list;
    generate(toperm(g, size), size, size, buf, list);
    
    // cerr << "bgen" << endl;
    // for (const auto& gg : list)
    // {
    //     cerr << ",g=";
    //     for (auto c : gg)
    //         cerr << (int)c;
    //     cerr <<endl;
    // }
    // cerr << "endgen" << endl;
    
    for (const auto& gg : list)
    {
        int anc = ancestors;
        if (terminal(toperm(gg, size), size))
            ancestors += 1 + polynome(toperm(gg, size), size);
        else if (gg != g)
            ancestors += solve(gg, size);
        // cerr << "delta for gg=";
        // for (auto c : gg)
        //     cerr << (int)c;
        // cerr << " is = " << (ancestors - anc) << endl;
    }
        
    // cache
    
    return ancestors;
}

/**************************/

googlement empty = {};

void generate2(unsigned length, unsigned left,
        googlement& buf, map<googlement, int>& universe)
{
    if (left == 0)
    {
        if (buf != empty)
            universe[buf] = 1;
        return;
    }
    for (uc i = 0; i <= length; ++i)
    {
        buf[length - left] = i;
        generate2(length, left - 1, buf, universe);
    }
}
googlement decay1(googlement g, int size)
{
    googlement g1 = {};
    for (uc c : g)
        if (c != 0)
            g1[c-1] += 1;
    return g1;
}
void decayAll(googlement g, int size, map<googlement, int>& universe)
{
    googlement g1 = decay1(g, size);
    while (g != g1) {
        // cerr << "decay g=";
        // for (auto c : g)
        //     cerr << (int)c;
        // cerr << " to g1=";
        // for (auto c : g1)
        //     cerr << (int)c;
        // cerr <<endl;

        g1.swap(g);
        universe[g] += 1;
        g1 = decay1(g, size);
    }
}

int solve2(googlement g, int size)
{
    map<googlement, int> universe;
    googlement buf = empty;
    generate2(size, size, buf, universe);
    
    // cerr << "bgen" << endl;
    // for (const auto& gg : universe)
    // {
    //     cerr << ",g=";
    //     for (auto c : gg.first)
    //         cerr << (int)c;
    //     cerr <<endl;
    // }
    // cerr << "endgen" << endl;
        
    
    for (const auto& p : universe)
        decayAll(p.first, size, universe);
    return universe[g];
    
}

int main() {
	int t;	
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++)
	{
		string gs;
		cin >> gs;

   
        googlement g = {};
        for (int i =0; i < gs.length(); ++i)
            g[i] = gs[i] - '0';
        
		int ancestors = solve(g, gs.length());
         
         // cout << "perm=";
         // for (auto c : perm)
         //     cout << (int)c;
         // cout <<endl;
          
        
        // for (const auto& g: list)
        // {
        //     for (auto c : g)
        //         cout << (int)c;
        //     cout <<endl;
        // }
		cout << "Case #" << tc << ": " << ancestors << endl;
	}
	
	
	return 0;
}