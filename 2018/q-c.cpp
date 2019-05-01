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

using point = pair<int, int>;
const int CELLS = 9;
struct rect
{
    rect(point pc) : center(pc), free(CELLS)
    { }
    
    point center;
    int free;
};
using points = vector<point>;
using rects = vector<rect>;
using prects = vector<rect*>; // bad code :)

point pt(int r, int c)
{
    return make_pair(r, c);
}

class game
{

    rects area;
    map<point, prects> undone;
    points done;
    
    void makeGo(points& ps, point c)
    {
        ps.clear();
        ps.push_back(pt(c.first-1, c.second-1));
        ps.push_back(pt(c.first-1, c.second));
        ps.push_back(pt(c.first-1, c.second+1));

        ps.push_back(pt(c.first, c.second-1));
        ps.push_back(pt(c.first, c.second));
        ps.push_back(pt(c.first, c.second+1));

        ps.push_back(pt(c.first+1, c.second-1));
        ps.push_back(pt(c.first+1, c.second));
        ps.push_back(pt(c.first+1, c.second+1));
    }
public:
    
    game(int a)
    {
        area.reserve(256);
        if (a == 20)
        {
            points buf;
            for (int r = 2; r <= 3; ++r)
            {
                for (int c = 2; c <= 4; ++c)
                {
                    rect rec = rect(pt(r, c));
                    makeGo(buf, rec.center);
                    area.push_back(rec);
                    rect* prec = &area.back(); // bad bad bad ... who cares it's cp contest)
                    for (auto p : buf)
                        undone[p].push_back(prec);              
                }
            }
        }
        else // a == 200
        {
            // copy-n-paste never hert anyone ;)
            points buf;
            for (int r = 2; r <= 9; ++r)
            {
                for (int c = 2; c <= 19; ++c)
                {
                    rect rec = rect(pt(r, c));
                    makeGo(buf, rec.center);
                    area.push_back(rec);
                    rect* prec = &area.back(); // bad bad bad ... who cares it's cp contest)
                    for (auto p : buf)
                        undone[p].push_back(prec);               
                }
            }            
        }
    }
    
    point pick()
    {
        int maxFree = 0;
        point maxCenter;
        for (const auto& r : area)
        {
            if (r.free > maxFree)
            {
                maxFree = r.free;
                maxCenter = r.center;
            }
            if (maxFree == CELLS)
                return maxCenter;
        }
        return maxCenter;
    }
    
    void accept(point p)
    {
        auto it = undone.find(p);
        if (it != undone.end())
        {
            for (rect* r : it->second)
                r->free -= 1;
            
            undone.erase(it);
            done.push_back(p);
        }
    }
};
//clang++ -std=c++14 -stdlib=libc++ -o <exec> <code>.cpp 
int main()
{
	int t;	
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++)
	{
		int a;
		cin >> a;
        // create game instance
        game g(a);
        // pick next cell
        int r = 1;
        int c = 1;
        while (true) {
            auto p = g.pick();
            cout << p.first << ' ' << p.second << endl;

            cin >> r >> c;
            if (r == 0 && c == 0)
                break;
            if (r < 0)
                return 0xdeadbeef;
            g.accept(pt(r, c));
        }
	}
	
	
	return 0;
}