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


struct range
{
    int from;
    int to;
};
bool operator==(const range& r1, const range& r2)
{
    return r1.from == r2.from && r1.to == r2.to;
}
bool operator!=(const range& r1, const range& r2)
{
    return !(r1 == r2);
}
const range EMPTY = { 0, 0 };
bool fit (int pw, int nsw)
{
    return (pw * 100 >= nsw * 90) && (pw * 100 <= nsw * 110);
}

range torange(int w, int pw)
{
    // 90 to 110;
    double wd = static_cast<double>(w);
    double pwd = static_cast<double>(pw);
    int xl = static_cast<int>(pwd/wd/1.1);
    int xh = static_cast<int>(pwd/wd/0.9);
    
    xl = fit(pw, xl * w) ? xl : xl + 1;
    xh = fit(pw, xh * w + w) ? xh + 1 : xh;
    
    return xh >= xl ? range {xl, xh} : EMPTY;
}
//clang++ -std=c++14 -stdlib=libc++ -o <exec> <code>.cpp 
int main()
{
	int t;	
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++)
	{
		int n, p;
		cin >> n >> p;
        auto weights = readvi(cin, n);
        vector<vi> packages; packages.reserve(n);
        vector<vector<range>> ranges(n);
        
        for (int i = 0; i < n; ++i)
        {
            packages.push_back(readvi(cin, p));
            sort(packages[i].begin(), packages[i].end());
            for (int j = 0; j < p; ++j)
            {
                const auto r = torange(weights[i], packages[i][j]);
                if (r != EMPTY)
                    ranges[i].push_back(r);
            }
        }
        
		int total = 0;
        while (true)
        {
            bool anyempty = false;
            for (auto& ips : ranges) 
                anyempty = anyempty || ips.empty();
            if (anyempty)
                break;       
                 
            int minto = numeric_limits<int>::max();
            for (const auto& ips : ranges)
                minto = min(minto, ips.back().to);
            bool done = false;
            for (auto& ips : ranges)
            {
                while (!ips.empty() && ips.back().from > minto)
                    ips.pop_back();
                if (ips.empty())
                {
                    done = true;
                    break;
                }
            }
            if (done)
                break;
            total += 1;
            for (auto& ips : ranges)
                ips.pop_back();
        }
		cout << "Case #" << tc << ": " << total << endl;
	}
	
	
	return 0;
}