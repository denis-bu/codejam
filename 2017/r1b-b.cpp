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

enum Color {Red, Ora, Yel, Gre, Blu, Vio, End};
vector<char> cols = {'R', 'O', 'Y', 'G', 'B', 'V'};
pair<Color, int> pickMax(const vi& u, Color ex, Color pref)
{
	Color mpos = End; int max = -1;
	for (size_t i = 0; i < u.size(); ++i)
	{
		if (i == ex)
			continue;
		if (u[i] > max || (u[i] == max && i == pref))
		{
			max = u[i];
			mpos = static_cast<Color>(i);
		}
	}
	return make_pair(mpos, max);
}

string aug(string pcmt, Color one, Color two, int countTwo)
{
	if (countTwo > 0)
	{
		auto pos = pcmt.find(cols[one]);
		string ins; ins.reserve(countTwo * 2);
		for (int i = 0; i < countTwo; ++i)
		{
			ins += cols[two];
			ins += cols[one];
		}
		pcmt = pcmt.substr(0, pos+1) + ins + pcmt.substr(pos+1);
	}
	return pcmt;
}

string solve(int N , int R , int O , int Y , int G , int B , int V)
{

	vi unicorns = {R-G, 0, Y-V, 0, B-O, 0};
	if (G > 0 && R < G)
		return "IMPOSSIBLE";
	if (O > 0 && B < O)
		return "IMPOSSIBLE";
	if (V > 0 && Y < V)
		return "IMPOSSIBLE";
	if (G > 0 && R == G)
	{
		if (N != R + G)
			return "IMPOSSIBLE";
		return aug("", Red, Gre, G);
	}
	if (O > 0 && B == O)
	{
		if (N != B + O)
			return "IMPOSSIBLE";
		return aug("", Blu, Ora, O);
	}
	if (V > 0 && Y == V)
	{
		if (N != Y + V)
			return "IMPOSSIBLE";
		return aug("", Yel, Vio, V);
	}
	Color last = End;
	string pcmt; pcmt.reserve(N);
	const int nn = N - 2*G - 2*V - 2*O;
	Color first = End;
	for (int unicorn = 1; unicorn <= nn; unicorn++)
	{
		auto posCount = pickMax(unicorns, last, first);
		if (posCount.second <= 0)
			return "IMPOSSIBLE";

		last = posCount.first;
		pcmt += cols.at(last);
		unicorns[last] -= 1; 
		if (first == End)
			first = last;
	}
	if (pcmt[0] == pcmt.back())
	{
		
		return "IMPOSSIBLE";
	}

	pcmt = aug(pcmt, Red, Gre, G);
	pcmt = aug(pcmt, Blu, Ora, O);
	pcmt = aug(pcmt, Yel, Vio, V);
	return pcmt;
}
//clang++ -std=c++14 -stdlib=libc++ -O3 -g -o <exec> <code>.cpp 
int main()
{
	int t;	
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++)
	{
		int N, R, O, Y, G, B, V;
		cin >> N >> R >> O >> Y >> G >> B >> V;
		auto ans = solve(N, R, O, Y, G, B, V);
		cout << "Case #" << tc << ": " << ans << endl;
	}
	/*
	2 red 
	1 yel
	1 gre
	2 blu
	*/
	
	return 0;
}