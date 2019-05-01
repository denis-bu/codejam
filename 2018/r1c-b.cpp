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


//clang++ -std=c++14 -stdlib=libc++ -O3 -g -o <exec> <code>.cpp 
struct loli
{
	int id;
	int ask;
};
int main()
{
	int t;	
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++)
	{
		int N;
		cin >> N;
		vector<loli> stat(N, loli{0, 0});
		for (int i = 0; i < N; ++i)
			stat[i].id = i;
		for (int cust = 0; cust < N; ++cust)
		{
			int D;
			cin >> D;
			auto pref1 = readvi(cin, D);
			set<int> pref(pref1.begin(), pref1.end());
			bool suc = false;
			for (int i = 0; i < stat.size(); ++i)
			{
				auto it = pref.find(stat[i].id);
				if (it != pref.end())
				{
					cout << stat[i].id << endl;
					suc = true;
					stat.erase(next(stat.begin(), i));
					break;
				}
			} 
			if (!suc)
			{
				cout << -1 << endl;
			}
			else 
			{
				for (int i = 0; i < stat.size(); ++i)
				{
					auto it = pref.find(stat[i].id);
					if (it != pref.end())
					{
						stat[i].ask +=1;
					}
				} 
				sort(stat.begin(), stat.end(), [](auto l1, auto l2) {return l1.ask < l2.ask;});
		 	}

		}
	}
	
	
	return 0;
}