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

using namespace std;

using ll = long long;
using ull = unsigned long long;
using vi = vector<int>;
using vll = vector<ll>;
using vs = vector<string>;

namespace {
	const double PI = acos(-1);
	
	unordered_map<ull, ll> nck_cache;
	ull nck_key(int n, int k)
	{
		return ((ull)n << 32) + (ull)k;
	}
	ll nck(int n, int k)
	{
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

	int pop(unsigned x)
	{
		constexpr int bitwidth = std::numeric_limits<unsigned>::digits;
		std::bitset<bitwidth> bs(x);
		return bs.count();
	}

}

int countDamage(const string& p)
{
	int total = 0;
	int d = 1;
	for (auto i : p)
		if (i == 'C')
			d *= 2;
        else
			total += d;
		
	return total;
}

void hack(string& p)
{
	auto chargePos = p.length();
	auto lastShoot = p.rfind('S');
	auto lastCharge = p.rfind('C', lastShoot);
	// assert(lastCharge != string::npos && lastShoot != string::npos)
	p[lastCharge] = 'S';
	p[lastCharge + 1] = 'C';
}

int solve(int d, string p)
{
	int shoots = count(p.begin(), p.end(), 'S');
	if (shoots > d)
		return -1;
	int minHacks = 0;
	while (countDamage(p) > d)
	{
		minHacks++;
		hack(p);
	}
	return minHacks;
}

int main()
{
	int t;
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++)
	{
		int d;
		string p;
		cin >> d >> p;
		int minHacks = solve(d, p);
		cout << "Case #" << tc << ": ";
		if (minHacks < 0)
		    cout << "IMPOSSIBLE" << endl;
		else
			cout << minHacks << endl;
	}
	
	
	return 0;
}