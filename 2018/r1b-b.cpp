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
const int NOD = numeric_limits<int>::min();

struct Seq
{
	int n, m, b, e;

	int size() const { return e - b;}
	bool ok() const { return b != -1; }
	bool md(int n1, int n2) const { return n1 == n2 || n1 == NOD || n2 == NOD; }
	Seq join(Seq other) const
	{
		Seq no = Seq{NOD, NOD, -1,-1};
		if (!(ok() && other.ok()))
			return no;
		int bi = max(b, other.b); int ei = min(e, other.e);
		if (bi - ei > 1) // not intersects or adjacent
			return no;
		if (!(md(n, other.n) && md(m, other.m)))
			return no;
		return Seq{max(n, other.n), max(m, other.m), min(b, other.b), max(e, other.e)};
	}
};

vector<Seq> solve1(const vi& ns, bool isn)
{
	vector<Seq> res(ns.size());
	//dest.clear(); dest.resize(ns.size());
	int start = 0;
	int cur = ns[0];
	for (int i = 1; i < ns.size(); ++i)
	{
		if (ns[i] != cur)
		{
			for (int j = start; j < i; ++j)
			{
				res[j] = Seq{(isn ? cur : NOD), (isn ? NOD : cur), start, i };
				//dest[j] = cur;
			}
			cur=ns[i];
			start = i;
		}
	}
	for (int j = start; j < ns.size(); ++j)
	{
		res[j] = Seq{(isn ? cur : NOD), (isn ? NOD : cur), start, static_cast<int>(ns.size()) };
		//dest[j] = cur;
	}	
	return res;
}

Seq getseq(const vector<Seq>& ns, int i)
{
	return i < ns.size() ? ns[i] : Seq{NOD, NOD, -1,-1};
}
void addseq(vector<Seq>& v, Seq s) { if (s.ok()) v.push_back(s); }
void solve2(const vector<Seq>& ns, const vector<Seq>& ms, set<pair<int,int>>& maxSeq, int& maxSeqSize)
{
	//cerr << "solv2:ns={";
	//for (auto cs : ns)
		//cerr << "[" << cs.b << "-" << cs.e << ",n=" << cs.n << ",m=" << cs.m <<"],";
	//cerr << endl << "solv2:ms={";
	//for (auto cs : ms)
		//cerr << "[" << cs.b << "-" << cs.e << ",n=" << cs.n << ",m=" << cs.m <<"],";
	//cerr << endl;
	// (n, m)
	vector<Seq> prev;
	vector<Seq> cur;
	int start = 0;
	while (start < ns.size())
	{
		//cerr << "start=" << start << endl;
		const auto seqi = ns[start];
		auto lseq = seqi.join(getseq(ms, start-1));
		lseq = lseq.ok() ? lseq : seqi; // no need to check both seqi & lseq, wider on is fine.
		auto rseq = seqi.join(getseq(ms, seqi.e));
		auto lrseq = lseq.join(rseq);
		if (lrseq.ok())
		{
			lseq = lrseq;
			rseq = Seq{NOD, NOD, -1,-1};
		}
		int purge = lseq.b;

		// now merje prev & cur. update mazSeq along the way.
		bool added = false;
		for (auto ps : prev)
		{
			if (ps.e < purge)
				continue;
			auto js = lseq.join(ps);
			added = added || js.ok();
			addseq(cur, js.ok() ? js : ps);
		}
		if (!added)
			addseq(cur, lseq);
		addseq(cur, rseq);
		for (auto cs : cur)
		{
			if (cs.size() > maxSeqSize)
			{
				maxSeqSize = cs.size();
				maxSeq.clear();
				//cerr << "new s=" << maxSeqSize << endl;
			}
			if (cs.size() == maxSeqSize)
			{
				//cerr << "ins seq " << cs.b << "-" << cs.e << ",n=" << cs.n << ",m=" << cs.m << endl;
				maxSeq.insert(make_pair(cs.b, cs.e));
			}
		}
		cur.swap(prev);
		cur.clear();
		start = seqi.e;
	}

}

//clang++ -std=c++14 -stdlib=libc++ -O3 -g -o <exec> <code>.cpp 
int main()
{
	int t;	
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++)
	{
		int S;
		cin >> S;
		vi Ns; Ns.reserve(S);
		vi Ms; Ms.reserve(S);
		for (int i = 0; i < S; ++i)
		{
			int D, A, B;
			cin >> D >> A >> B;
			Ns.push_back(D - B);
			Ms.push_back(D + A);
		}

		int maxSeqSize = 0;
		set<pair<int, int>> maxSeq;
		auto NsMap = solve1(Ns, true);
		auto MsMap = solve1(Ms, false);
		solve2(NsMap, MsMap, maxSeq, maxSeqSize);
		solve2(MsMap, NsMap, maxSeq, maxSeqSize);
		//for (auto p : maxSeq)
			//cerr << p.first << "-" << p.second << ",";
		//cerr << endl;
		cout << "Case #" << tc << ": "  << maxSeqSize << " " << maxSeq.size() << endl;
	}
	
	
	return 0;
}