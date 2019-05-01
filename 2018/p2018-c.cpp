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

	int pop(unsigned x)
	{
		constexpr int bitwidth = std::numeric_limits<unsigned>::digits;
		std::bitset<bitwidth> bs(x);
		return bs.count();
	}

}


int main() {
	int t;
	
	cin >> t;
	cout.precision(6);
	for (int tc = 0; tc < t; tc++) {
		int d,n;
		cin >> d >> n;
		double maxtime = 0.0;
		for (int i = 0; i < n; ++i)
		{
			int k, s;
			cin >> k >> s;
			double t = static_cast<double>(d-k)/static_cast<double>(s);
			maxtime = max(maxtime, t);
		}
		cout << "Case #" << (tc+1) << ": "  << std::fixed << (d / maxtime) << endl;
		
	}
	
	
	return 0;
}