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
#include <cstdlib>

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

int solve(int n, const vi& seq)
{
	vi odd; odd.reserve(n / 2 );
	vi even; even.reserve(n / 2 );
	for (size_t i = 0; i < n; i += 2)
	{
		cerr << "i="<< i << endl;
		even.push_back(seq[i]);
		odd.push_back(seq[i+1]);
	}
	sort(odd.begin(), odd.end());
	sort(even.begin(), even.end());
	for (size_t i = 0; i < even.size(); ++i)
	{
		if (even[i] > odd[i])
			return 2*i;
		if ((i+1) < even.size() && (odd[i] > even[i+1]))
			return 2*i + 1;
	}	
	return -1;
}
//clang++ -std=c++14 -stdlib=libc++ -o q-a q-a.cpp 
int main()
{
	int t;	
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++)
	{
		int n;
		cin >> n;
		vi seq; seq.reserve(n+1);
		string tmp; tmp.reserve(10 * 100000);
		getline(cin, tmp);
		getline(cin, tmp);
		char* next = &tmp[0];
		for (int i = 0; i < n; ++i)
		{
			int x = strtol(next, &next, 10);
			seq.push_back(x);
		}
		if (n % 2 != 0) 
			seq.push_back(numeric_limits<int>::max());
		
		int firstError = solve(n, seq);
		cout << "Case #" << tc << ": ";
		if (firstError < 0)
			cout << "OK" << endl;
		else
			cout << firstError << endl;
	}
	
	
	return 0;
}