#include <iostream>
#include <algorithm>
#include <utility>
#include <map>

using namespace std;

using ll = long long;
pair<ll, ll> div(ll w)
{
	w -= 1;
	return make_pair(w / 2, w / 2 + w % 2);
}
pair<ll, ll> solve(ll k, ll ws, ll nws, ll wb, ll nwb)
{
	if (k <= (nws + nwb))
	{
		auto pp = div(k <= nwb ? wb : ws);
		return make_pair(max(pp.first, pp.second), min(pp.first, pp.second));
	}
	
	map<ll, ll> count_map;
	auto pp = div(ws);
	count_map[pp.first] += nws;
	count_map[pp.second] += nws;
	if (wb > 0)
	{
		pp = div(wb);
		count_map[pp.first] += nwb;
		count_map[pp.second] += nwb;		
	}
	// assert count_map.size() == 1 || count_map.size() == 2;
	if (count_map.size() == 1)
		return solve(k - nws - nwb, count_map.begin()->first, count_map.begin()->second, -1, 0);
	else
	{
		auto it1 = count_map.begin();
		auto it2 = ++count_map.begin();
		return solve(k - nws - nwb, it1->first, it1->second,
		                it2->first, it2->second);

	}
}


int main() 
{
	int t;
	
	cin >> t;
	
	for (int tc = 1; tc <= t; tc++) 
	{
		ll n,k;
		cin >> n >> k;
		auto pp = solve(k, n, 1, -1, 0);
		cout << "Case #" << tc << ": " << pp.first << ' ' << pp.second << endl;		
	}
	
	
	return 0;
}