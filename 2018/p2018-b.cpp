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

using party = pair<int, char>;

bool senate_cmp(const party& p1, const party& p2)
{
	return p1.first < p2.first;
}

party pop(vector<party>& senate)
{
	pop_heap(senate.begin(), senate.end(), senate_cmp);
	auto p = senate.back();
	senate.pop_back();
	return p;
}

void push(vector<party>& senate, party p)
{
	senate.push_back(p);
	push_heap(senate.begin(), senate.end(), senate_cmp);
}

int main() {
	int t;
	
	cin >> t;
	
	for (int tc = 0; tc < t; tc++) 
	{
		cout << "Case #" << (tc + 1) << ": ";
		int count = 0;
		int n;
		vector<party> senate;
		cin >> n;
		char c = 'A';
		for (int i = 0; i < n; ++i)
		{
			int p;
			cin >> p;
			count += p;
			senate.push_back(make_pair(p, c));
			c++;
		}
		make_heap(senate.begin(), senate.end(), senate_cmp);
		if (count % 2 == 1)
		{
			auto p1 = pop(senate);
			cout << p1.second << ' ';
			p1.first -= 1;
			count -= 1;
			if (p1.first)
				push(senate, p1);
		}
		while (count > 0)
		{
			
			auto p1 = pop(senate);
			auto p2 = pop(senate);
			
			if (p1.first == p2.first)
			{
				cout << p1.second << p2.second << ' ';
				p1.first -= 1;
				p2.first -= 1;
				count -= 2;
			}
			else
			{
				cout << p1.second << p1.second << ' ';
				p1.first -= 2;	
				count -= 2;			
			}
			
			if (p1.first)
				push(senate, p1);
			if (p2.first)
				push(senate, p2);

		}
		cout << endl;
	}
	
	
	return 0;
}