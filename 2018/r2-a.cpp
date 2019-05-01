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

vector<string> solve1(int c, vi bs)
{
	if (bs[0] == 0 || bs[c-1] == 0)
		return {};
	//bs[0] -= 1; bs[c-1] -= 1;
	vi state = vi(c, 1);

	vector<string> ans;

int ccc = 0;
	while (state != bs) 
	{
		ccc++;
		if (ccc > 120)
			return {};

		string cur = "";
		int curp = 0;
		for (int i =0; i < c; i++)
		{
			if (bs[i] > 0)
			{
				for (int j = 1; j <= (bs[i]-state[i]) + 1; ++j)
				{

					if (curp == i)
					{
						cur += '.';
					}
					else if (curp > i)
					{
						cur += '/';
						state[curp] -=1; state[curp-1] +=1;
					}
					else
					{
						cur += '\\';
						state[curp] -=1; state[curp+1] +=1;
					}
					curp ++;
				}
			}
		}
		if (cur.length() < c)
			for(int i = 0; i < (cur.length()-c); ++i) cur += '.';

		cerr << cur << endl;
		for (auto x : state)
			cerr << x << ",";
		cerr << endl;
		ans.push_back(cur);
	}
	ans.push_back(string(c, '.'));
	return ans;
}

struct region
{
	int begin;
	int end;
	int bucket; // -1 if none.
	int size() { return end - begin; }
	bool done() { return begin == bucket && size() == 1; }
	bool space() { return bucket == -1; }
};

vector<string> solve(int c, vi bs)
{
	if (bs[0] == 0 || bs[c-1] == 0)
		return {};
	vi state = vi(c, 1);
	vector<region> regions;
	{
		int wp = 0;
		for (int i = 0; i < bs.size(); ++i)
		{
			int pwp = wp; wp += bs[i];
			if (wp > pwp)
				regions.push_back(region{pwp, wp, i});
		}
	}
	vector<string> ans;

	int iter = 1;
	bool done = false;
	while (!done) 
	{
		done = true;
		vector<region> next;
		string line;
		for (int i = 0; i < regions.size(); ++i)
		{
			//int spaceAfter = 0;
			auto r = regions[i];
			// case 0 region is done or space
			if (r.done() || r.space())
			{
				for (int j = r.begin; j < r.end; ++j)
					line += '.';
			}
			// case 1 region is on the right of bucket
			else if (r.begin > r.bucket)
			{
				for (int j = r.begin; j < r.end; ++j)
					line += '/';
				r.begin -= 1; r.end -= 1;
			}
			// case 2 region is adjacent with bucket from left
			else if (r.begin == r.bucket)
			{
				line += '.';
				for (int j = r.begin+1; j < r.end; ++j)
					line += '/';
				r.end -= 1;
			}
			// case 3 bucket within the region
			else if (r.begin < r.bucket && r.bucket < (r.end - 1))
			{
				for (int j = r.begin; j < r.bucket; ++j)
					line += '\\';
				line += '.';
				for (int j = r.bucket + 1; j < r.end; ++j)
					line += '/';				
				r.begin += 1; r.end -= 1;
			}
			// case 4 region is adjacent with bucket from right
			else if ((r.end - 1)  == r.bucket)
			{
				for (int j = r.begin; j < (r.end-1); ++j)
					line += '\\';
				line += '.';
				r.begin += 1;
			}			
			// case 5 region is on the left of bucket
			else
			{
				for (int j = r.begin; j < r.end; ++j)
					line += '\\';
				r.begin += 1; r.end += 1;				
			}
			if (!r.space())
				next.push_back(r);
			done = done && (regions[i].done() || regions[i].space());
		}
		ans.push_back(line);
		regions.clear();
		regions.push_back(next[0]);
		for (int j = 1; j < next.size(); ++j)
		{
			if (next[j].begin != next[j-1].end)
				regions.push_back(region{next[j-1].end, next[j].begin, -1});
			regions.push_back(next[j]);
		}
	}
	return ans;
}
//clang++ -std=c++14 -stdlib=libc++ -O3 -g -o <exec> <code>.cpp 
int main()
{
	short x = -17;
	cout << hex << x << endl;
	x /= 2;
	cout << hex << x << endl;
	cout << "-17/2=" << x << endl;
	int t;	
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++)
	{
		int c;
		cin >> c;
		auto bs = readvi(cin, c);

		auto ans = solve(c, bs);
		if (ans.empty())
		{
			cout << "Case #" << tc << ": " << "IMPOSSIBLE" << endl;
		}
		else
		{
			cout << "Case #" << tc << ": " << ans.size() << endl;		
			for (auto s : ans) cout << s << endl;
		}
		
	}
	
	
	return 0;
}