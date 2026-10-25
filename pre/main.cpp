#include <bits/stdc++.h>
using namespace std;
#define rep(i, a, n) for (int i = a; i < n; ++i)
#define repp(i, a, n) for (int i = a; i <= n; ++i)
#define ll long long
#define lrep(i, a, n) for (ll i = a; i < n; ++i)
#define lrepp(i, a, n) for (ll i = a; i <= n; ++i)
using ull = unsigned long long;
#define YES cout << "Yes" << "\n"
#define NO cout << "No" << "\n"

#define pb push_back
#define eb emplace_back

#define el endl

using pii = pair<int, int>;
using pll = pair<ll, ll>;

template <typename T>
using vec = vector<T>;

template <typename T>
using vvec = vector<vector<T>>;

template<class T> using pq = priority_queue<T>;
template<class T> using rpq = priority_queue<T, vector<T>, greater<T>>;

constexpr ll INFLL = (1LL<<60);
constexpr int INF = (1<<30);

template<class T>
bool chmax(T &a, const T &b) {
  if (a < b) { a = b; return true; }
  return false;
}

template<class T>
bool chmin(T &a, const T &b) {
  if (a > b) { a = b; return true; }
  return false;
}

template<class T>
void read(vec<T>& a) {
  for(auto &x : a) cin >> x;
}

template<class T>
void print_el(const vec<T>& a) {
  for(auto &x : a) cout << x << "\n";
}

template<class T>
void print(const vec<T>& a) {
  rep(i, 0, (int)a.size()) {
    if (i) cout << " ";
    cout << a[i];
  }
  cout << el;
}

int readInt() {
	int res = 0;
	char c;
	while (cin >> c && !isdigit(c));
	res = c - '0';
	while (cin.get(c) && isdigit(c)) res = res * 10 + (c - '0');
	return res;
}

set<char> skip = {' ', '"', '\n'};
char start_char = '{';
char end_char = '}';

string input(string s){
  vec<string> T = {""};
  vec<int> index;
  stack<int> st;
  st.push(0);
  index = {-1};
  rep(i, 0, (int)s.size()){
    if (skip.count(s[i])) continue;
    bool add = false;
    if (s[i] == start_char){
      add = true;
      st.push(index.size());
      index.push_back(-1);
    }
    else if (s[i] == end_char) add = true, st.pop();
    else if (s[i] == ','){
      add = true;
      index[st.top()]--;
    }
    else{
      if (T.back().empty()){
        index.push_back((int)T.size() - 1);
      }
      T.back() += s[i];
    }
    if (add && !T.back().empty()){
      T.push_back("");
    }
  }
  string res;
  for (auto i : index){
    if (i < 0) res += (to_string(-i));
    else res += T[i];
    res += "\n";
  }
  return res;
}

stringstream inp;

void solve() {
	int N;
	inp >> N;
	(void)N;

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	(void)INF;
	(void)INFLL;
	
	string main = "{},\n"
	"{},\n"
	"{}";
	
	// string sample = "{},\n"
	// "{},\n"
	// "{}";

	// inp << input(sample);
	inp << input(main);
	int T;
	inp >> T;
	rep(i, 0, T) {
		if (i) cout << ",";
		solve();
	}
	cout << el;
	return 0;
}
