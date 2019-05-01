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
	const long double PI = acos(-1.0l);
	
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
    template<typename T>
    class Matrix {
    public:
            typedef typename std::vector<T>::size_type size_type;
    private:
            size_type rows_;
            size_type cols_;
            T modulo_;
            std::vector<T> d_;
    public:
            template<typename U>
            static Matrix<U> identity(const U& modulo, typename Matrix<U>::size_type n) {
                    Matrix<U> id(modulo, n, n);
                    for (typename Matrix<U>::size_type i = 0; i < n; ++i) {
                            id(i, i) = static_cast<U>(1); // TODO: type traits
                    }
                    return id;
            }

            Matrix(size_type rows, size_type cols) : rows_(rows), cols_(cols), modulo_(), d_(rows * cols) {	}
            Matrix(const T& modulo, size_type rows, size_type cols) : rows_(rows), cols_(cols), modulo_(modulo), d_(rows * cols) {	}

            size_type rows() const { return rows_; }
            size_type cols() const { return cols_; }
            const T& modulo() const { return modulo_; }

            T& operator()(size_type row, size_type col) {
                    return d_[row * cols_ + col];
            }

            const T& operator()(size_type row, size_type col) const {
                    return d_[row * cols_ + col];
            }

            bool operator==(const Matrix<T>& other) const {
                    if (!(rows_ == other.rows_ && cols_ == other.cols_))
                            return false;
                    for (size_t i = 0; i < rows_; ++i) {
                            for (size_t j = 0; j < cols_; ++j) {
                                    if ((*this)(i, j) != other(i, j))
                                            return false;
                            }
                    }
                    return true;
            }

            void reset(const T& v) {
                    d_.assign(d_.size(), v);
            }

            Matrix<T> identity() const {
                    if (rows_ != cols_)
                            throw std::runtime_error("cols != rows");
                    return identity(modulo_, rows_);
            }

    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
            for (size_t i = 0; i < m.rows(); ++i) {
                    for (size_t j = 0; j < m.cols(); ++j) {
                            std::cout << m(i, j) << ", ";
                    }
                    std::cout << std::endl;
            }
            return os;
    }

    template<typename T>
    Matrix<T> operator*(const Matrix<T>& left, const Matrix<T>& right) {
            typedef typename Matrix<T>::size_type st;
            if (left.cols() != right.rows())
                    throw std::runtime_error("left.cols != right.rows");
            if (left.modulo() != right.modulo())
                    throw std::runtime_error("left.modulo != right.modulo");
            bool hasModulo = left.modulo() != T();
            const T& mod = left.modulo();
            Matrix<T> r(mod, left.rows(), right.cols());

                    for (st i = 0; i < left.rows(); ++i) {
                            for (st j = 0; j < right.cols(); ++j) {
                                    for (st k = 0; k < left.cols(); ++k) {
                                            r(i, j) += left(i, k) * right(k, j);
                                    }
                            }
                    }


            return r;
    }

}
using ld = long double;
Matrix<ld> solve(ld a)
{
	Matrix<ld> pts (3, 3);
    pts(0, 0) = .5; pts(1, 0) = .0; pts(2, 0) = .0;
    pts(0, 1) = .0; pts(1, 1) = .5; pts(2, 1) = .0;
    pts(0, 2) = .0; pts(1, 2) = .0; pts(2, 2) = .5;
    cerr << pts << endl << endl;
    ld sr2 = sqrt(2.0l);
    ld alpha = (a > sr2) ? 0 : acos(a / sr2);
    alpha -= PI / 4;
    cerr << "a="<<a << ",acos=" << acos(a / sr2) << ",Alpha " << alpha << endl;
    Matrix<ld> malpha (3, 3);
    malpha(0, 0) = 1.; malpha(1, 0) = .0; malpha(2, 0) = .0;
    malpha(0, 1) = .0; malpha(1, 1) = cos(alpha); malpha(2, 1) = sin(alpha);
    malpha(0, 2) = .0; malpha(1, 2) = -sin(alpha); malpha(2, 2) = cos(alpha);    
    pts = malpha * pts;
    cerr << pts << endl << endl;
    if (a > sr2)
    {
        ld beta = acos(a / 2);
        beta -= PI / 4;
        Matrix<ld> mbeta (3, 3);
        mbeta(0, 0) = cos(beta); mbeta(1, 0) = sin(beta); mbeta(2, 0) = .0;
        mbeta(0, 1) = -sin(beta); mbeta(1, 1) = cos(beta); mbeta(2, 1) = .0;
        mbeta(0, 2) = .0; mbeta(1, 2) = 0.; mbeta(2, 2) = 1.;    
        pts = mbeta * pts;        
        cerr << pts << endl << endl;
    }
    return pts;    
}
//clang++ -std=c++14 -stdlib=libc++ -o <exec> <code>.cpp 
int main()
{
	int t;	
	cin >> t;
	cout.precision(16);
	for (int tc = 1; tc <= t; tc++)
	{
		double a;
		string p;
		cin >> a;
		auto pts = solve(a);
		cout << "Case #" << tc << ":" << endl; 
        cout << pts(0, 0) << ' ' << pts(1, 0) << ' ' << pts(2, 0) << endl;
        cout << pts(0, 1) << ' ' << pts(1, 1) << ' ' << pts(2, 1) << endl;
        cout << pts(0, 2) << ' ' << pts(1, 2) << ' ' << pts(2, 2) << endl;
	}
	
	
	return 0;
}