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

int main() {
	int t;
	
	cin >> t;
	for (int tc = 0; tc < t; tc++) {
		int a,b,n;
		cin >> a >> b >> n;
		string ans;
		while (ans != "CORRECT") {
			int guess = a + (b - a) / 2 + (b - a) % 2;
			cout << guess << endl;
			cin >> ans;
			if (ans == "TOO_SMALL")
			{
				a = guess;
			}
			else if (ans == "TOO_BIG")
			{
				b = guess - 1;
			}
		}
		
	}
	
	
	return 0;
}