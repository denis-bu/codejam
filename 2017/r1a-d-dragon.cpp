#include <iostream>

#include <thread>
#include <future>
#include <atomic>
#include <condition_variable>

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

mutex cerrLock;
#define logerr(X) { std::unique_lock<std::mutex> g(cerrLock); cerr << X; }

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

	class FixedThreadPool
	{
		using scoped_lock = std::unique_lock<std::mutex>;

		std::mutex lock;
		std::atomic<bool> shutdown;
		std::condition_variable notEmpty;
		std::queue<std::function<void()>> tasks;

		std::vector<std::unique_ptr<std::thread>> pool;

		FixedThreadPool(const FixedThreadPool&) = delete;
		FixedThreadPool& operator= (const FixedThreadPool&)= delete;

	public:
		FixedThreadPool(int threads) : shutdown(false)
		{
			for (int i = 0; i < threads; ++i)
				pool.emplace_back(new std::thread([this, i] {run(i);}));
		}
		~FixedThreadPool()
		{
			shutdown = true;
			notEmpty.notify_all();
			for (auto& t : pool)
				t->join();
		}

		void run(int i)
		{
			while (!shutdown)
			{
				auto t = dequeue();
				if (t)
				{
					logerr("t " << i << " call task" << endl);
					t();
				}
			}
		}

		std::function<void()> dequeue()
		{
			scoped_lock guard(lock);
			while (!shutdown)
			{
				if (!tasks.empty())
				{
					auto t = tasks.front();
					tasks.pop();
					return t;
				}

				notEmpty.wait(guard, [this]() {return shutdown || !tasks.empty();});
			}
			return std::function<void()>();
		}

		template<typename Result>
		std::future<Result> enqueue(std::function<Result()> task)
		{
			std::shared_ptr<std::promise<Result>> pr(new std::promise<Result>());
			{
				scoped_lock guard(lock);
				tasks.push([pr, task]() {
					try
					{
						auto r = task();
						pr->set_value(r);
					}
					catch (std::exception&)
					{
						pr->set_exception(std::current_exception());
					}
				});
			}
			notEmpty.notify_one();
			return pr->get_future();
		}
	};


}
// calc # of turns to kill knight , disrespect knight's attack & cure
ll calcAttack(ll ad, ll hk, ll b, ll bcount)
{
	if (bcount < 0)
		return numeric_limits<int>::max();
	
	ll a = ad + b * bcount;
	ll toKill = static_cast<ll>(ceil(static_cast<double>(hk)/a));
	return toKill + bcount;
}

ll calcHits(ll h, ll a)
{
	return (((h % a) == 0) ? (h / a) - 1 : h / a);
}

ll calcCure(ll hd, ll hd0, ll ak, ll steps)
{
	// first we have hd0, then hd - ak after each cure step.
	// steps is the number of steps of useful work we need to do.
	// nore that at last step we allow health to reduce to 0,
	// since knight won't hit us.
	ll totalDamage = steps * ak - ak; // steps - 1
	if ((totalDamage - hd0) <= 0)
		return 0;
	totalDamage -= calcHits(hd0, ak) * ak; // num steps we can survive with hd0 * ak
	// now, after 1 cure dragon has hd - ak health.
	hd -= ak;
	// so before cure dragon can sustein that many hits
	ll hits = calcHits(hd, ak);
	if (totalDamage % (hits * ak) == 0)
		 	return totalDamage / (hits * ak);
	return 1 + totalDamage / (hits * ak); //+ (totalDamage % (hits * ak)) ? 0 : - 1;
}

ll solve(ll hd, ll ad, ll hk, ll ak, ll b, ll d)
{
	if (hd <= (ak-d)) 
	{
		if (hk > ad)
			return -1;
		else
			return 1;
	}
	if (hd <= (2 * ak - 3 * d))
	{
		if (hd > ak && (hk <= 2 * ad || hk <= (ad + b)))
			return 2;
		return -1;
	}
		
	// 1. find optimal # of turns to buff and attack
	double bcountF = b==0? 0.0 : (sqrt(static_cast<double>(b)*static_cast<double>(hk)) - ad)/b;
	ll bcount = bcountF > 0 ? ceil(bcountF) : 0;
	ll minAttack = calcAttack(ad, hk, b, bcount);
	minAttack = min(minAttack, calcAttack(ad, hk, b, bcount - 1));
	minAttack = min(minAttack, calcAttack(ad, hk, b, bcount + 1));
	// 2. compute # of turns we will spent to cure ourselfs
	
	ll zero = 0;
	ll debuffAtStart = 0;
	ll hd0 = hd;
	if (hd <= 2 * ak)
	{
		ak = max(ak-d, zero);
		hd0 -= ak;
		debuffAtStart = 1;
		// cerr << "debuff1 " << ak << " hd0=" << hd0 << endl;
		if (hd <= 2 * ak)
		{
			ak = max(ak-d, zero);
			hd0 -= ak;
			debuffAtStart = 2;
			//cerr << "debuff2 " << ak << " hd0=" << hd0 << endl;
		}
	}
	////cerr << "minAttack = " << minAttack << endl;
	////cerr << "countCure = " << calcCure(hd, hd0, ak, minAttack) << endl;
	ll cureSteps = calcCure(hd, hd0, ak, minAttack);
	if (d == 0)
		return cureSteps + minAttack;
		// now calc the game until attack
	ll maxDebuff = ak / d + (ak % d ? 1 : 0);
	maxDebuff = min(maxDebuff, max(cureSteps - 1, zero));
	ll minCure = cureSteps;
	//cerr << "cureSteps = " << cureSteps << " maxDebuff=" << maxDebuff << endl;
	ll cureWhileDebuff = 0;
	for (ll dc = 0; dc < maxDebuff;)
	{

		if (hd0 - ak + d <= 0)
		{
			hd0 = hd - ak;
			cureWhileDebuff++;
		}
		else
		{
			ak = max(ak-d, zero);
			hd0 -= ak;
			dc++;
		}
		minCure = min(minCure, calcCure(hd, hd0, ak, minAttack) + dc + cureWhileDebuff);
		//cerr << "hd0=" << hd0 << " ak=" << ak << " dc=" << dc << " cureWhileDebuff=" << cureWhileDebuff;
		//cerr << " minCure=" << minCure << " cc=" << calcCure(hd, hd0, ak, minAttack) << endl;
	}
	//cerr << "debuffAtStart = " << debuffAtStart << endl;
	//cerr << "minAttack = " << minAttack << endl;
	//cerr << "minCure = " << minCure << endl;
	return debuffAtStart + minAttack + minCure;
}
//clang++ -std=c++14 -stdlib=libc++ -O3 -g -o <exec> <code>.cpp 
int main()
{
	FixedThreadPool pool(8);
	vector<future<int>> results; results.reserve(100);

	const string IMP = "IMPOSSIBLE";
	int t;
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++)
	{
		int hd, ad, hk, ak, b, d;
		cin >> hd >> ad >> hk >> ak >> b >> d;
		//int turns = solve(hd, ad, hk, ak, b, d);
		std::function<int()> solveF = [=]{
			logerr("sol " << tc << endl);
			int r = solve(hd, ad, hk, ak, b, d);
			logerr("solved " << tc << " = " << r << endl);
			return r;
		};
		results.push_back(pool.enqueue(solveF));
	}
	logerr("Enqueued all" << endl); 
	for (int tc = 1; tc <= t; tc++)
	{
		auto& f = results[tc-1];
		cerr << "Get " << tc << endl;
		int turns = f.get();
		cerr << "Case #" << tc << ": ";
		cout << "Case #" << tc << ": ";
		if (turns <= 0)
			cout << IMP << endl;
		else
			cout << turns << endl;
	}	
	
	return 0;
}