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
#include <array>
#include <functional> 
#include <cctype>
#include <locale>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using vi = vector<int>;
using vll = vector<ll>;
using vs = vector<string>;

namespace {
	const double PI = acos(-1);

	struct pairhash
	{
		template <typename T, typename U>
		std::size_t operator()(const std::pair<T, U> &x) const
		{
			size_t result = 17;
			result = 31 * result + std::hash<T>()(x.first);
			result = 31 * result + std::hash<U>()(x.second);
			return result;
		}
	};
	//unordered_map<pair<int, int>, int, pairhash> cache;

	unordered_map<ull, ll> nck_cache;
	ull nck_key(int n, int k)
	{
		return ((ull)n << 32) + (ull)k;
	}
	ll nck(int n, int k)
	{
		if (k >	 n)
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


	void ltrim(string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](typename string::value_type ch) {
			return !std::isspace(ch);
		}));
	}

	void rtrim(string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](typename string::value_type ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	void trim(string &s) {
		ltrim(s);
		rtrim(s);
	}

	vi readvi(istream& is, size_t n)
	{
		vi seq; seq.reserve(n);
		if (n == 0)
			return seq;
		string tmp; tmp.reserve(10 * n);
		getline(is, tmp);
		trim(tmp);
		if (tmp.empty())
			getline(is, tmp);
		auto next = &tmp[0];
		for (int i = 0; i < n; ++i)
		{
			int x = strtol(next, &next, 10);
			seq.push_back(x);
		}
		return seq;
	}
}
bool filled = false;

constexpr int maxBalls = 500;
int dp[501][501][600] = {0};
struct Jug
{
	int r;
	int b;
};
using Jugs = vector<Jug>;
Jugs jugs;

int takeJug(int jc, int rb, int bb )
{
	if (rb < jugs[jc-1].r || bb < jugs[jc-1].b)
		return numeric_limits<int>::min();
	return 1 + dp[rb-jugs[jc-1].r][bb-jugs[jc-1].b][jc-1];
}
int solve(int r, int b)
{
	if (!filled)
	{
		// generate possible juglers
		for (int cb = 1; cb <= 32; ++cb )
			for (int rb = 0; rb <= cb; ++rb)
				jugs.push_back(Jug{rb, cb - rb});
		cerr << "Total jugs=" << jugs.size() << endl;
		// fill dp once
		dp[0][1][1] = 1;
		dp[1][0][1] = 1;
		for (int rb = 0; rb <= maxBalls; ++rb)
		{
			for (int bb = 0; bb <= maxBalls; ++bb)
			{
				for (int jc = 1; jc <= jugs.size(); ++jc)
				{
					dp[rb][bb][jc] = max(dp[rb][bb][jc-1], takeJug(jc, rb, bb)); 
				}
			}
		}
		filled = true;
	}
	int maxJ = 0;
	for (int i = 1; i <= jugs.size(); ++i)
		maxJ = max(maxJ, dp[r][b][i]);
	return maxJ;
}
//clang++ -std=c++14 -stdlib=libc++ -O3 -g -o <exec> <code>.cpp 
int main()
{
	int t;	
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++)
	{
		int d;
		int r, b;
		cin >> r >> b;
		auto ans = solve(r, b);
		cout << "Case #" << tc << ": " << ans << endl;
	}
	
	
	return 0;
}