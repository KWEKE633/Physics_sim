#include <bits/stdc++.h>
using namespace std;
#define rep(i, a, n) for (int i = a; i < n; ++i)
#define repp(i, a, n) for (int i = a; i <= n; ++i)
#define ll long long
#define lrep(i, a, n) for (ll i = a; i < n; ++i)
#define lrepp(i, a, n) for (ll i = a; i <= n; ++i)
using ull = unsigned long long;
using ld = long double;
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

// #include <chrono>
// class Timer {
// 	chrono::system_clock::time_point start_time;
// 	public:
// 		Timer() {
// 			start_time = chrono::system_clock::now();
// 		}
// 		double elapsed() {
// 			auto now = chrono::system_clock::now();
// 			return chrono::duration_cast<chrono::microseconds>(now - start_time).count() / 1e3; // ms
// 		}
// };

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

vvec<pair<ld, ld>> inp = {
	{{-270, 800}, {-210, 800}, {-150, 800}, {-90, 800}, {-30, 800}, {30, 800}, {90, 800}, {150, 800}, {210, 800}, {270, 800}},
	{{-300, 600}, {-150, 600}, {0, 600}, {150, 600}, {300, 600}, {-300, 800}, {-150, 800}, {0, 800}, {150, 800}, {300, 800}},
	{{-300, 650}, {-300, 550}, {-300, 450}, {-300, 350}, {-300, 250}, {300, 900}, {300, 800}, {300, 700}, {300, 600}, {300, 500}},
	{{0, 350}, {0, 550}, {0, 750}, {0, 950}, {-150, 400}, {-225, 600}, {-300, 800}, {150, 400}, {225, 600}, {300, 800}},
	{{0, 450}, {85, 575}, {340, 700}, {170, 700}, {0, 700}, {-340, 700}, {-280, 825}, {255, 950}, {0, 950}, {-340, 950}}
};

template<class T>
struct Point {
	T x, y;
	Point(T _x = 0, T _y = 0) : x(_x), y(_y) {}
	T dist() const { return hypot(x, y); }
};

struct state {
	int ch, lh, rh, theta, nu, wt;
};

struct SimResult {
	int hit_hole_idx;
	ld time_taken;
};

const ld PI = acosl(-1); 
const ld G = 9806.65;
const ld HOLE_RADIUS = 20;

SimResult simulate_full(int theta_1000, int Ch, int Lh, int Rh, const vector<Point<ld>>& holes) {
	ld v0 = 500.0;
	ld rad = (theta_1000 / 1000.0) * PI / 180.0;
	ld vx = v0 * sin(rad);
	ld vy = v0 * cos(rad);

	ld ax = (5.0 / 7.0) * G * ((Lh - Rh) / 1000.0) / 800.0;
	ld ay = (5.0 / 7.0) * G * ((Ch - (Lh + Rh) / 2.0) / 1000.0) / 1000.0;

	ld px = 0, py = 0;
	ld dt = 0.002;

	for (int i = 0; i < 5000; i++) {
		ld next_px = px + vx * dt + 0.5 * ax * dt * dt;
		ld next_py = py + vy * dt + 0.5 * ay * dt * dt;

		int hit_idx = -1;
		ld min_ratio = 2.0;

		rep(h, 0, (int)holes.size()) {
			if (py <= holes[h].y && holes[h].y < next_py) {
				ld ratio = (holes[h].y - py) / (next_py - py);
				ld hit_x = px + (next_px - px) * ratio;
				if (abs(hit_x - holes[h].x) <= HOLE_RADIUS) {
					if (ratio < min_ratio) {
						min_ratio = ratio;
						hit_idx = h;
					}
				}
			}
		}

		if (hit_idx != -1) {
			return {hit_idx, i * dt + min_ratio * dt};
		}

		px = next_px;
		py = next_py;
		vx += ax * dt;
		vy += ay * dt;

		if (py > 1000 || abs(px) > 400) break;
	}
	return {-1, -1.0};
}

state calc(int target_idx, const vector<Point<ld>>& holes) {
	// int curves[] = {0, 1, -1};
	
	for (int Ch = -25000; Ch <= 25000; Ch += 10000) {
		for (int Lh = -25000; Lh <= 25000; Lh += 10000) {
			for (int Rh = -25000; Rh <= 25000; Rh += 10000) {
				if (Ch < (Lh + Rh) / 2) continue;
				int hit_start = -100000;
				int hit_end = -100000;
				ld best_time = 0;

				for (int dir = -45000; dir <= 45000; dir += 20) {
					SimResult res = simulate_full(dir, Ch, Lh, Rh, holes);
					if (res.hit_hole_idx == target_idx) {
						if (hit_start == -100000) hit_start = dir;
						hit_end = dir;
						best_time = res.time_taken;
					} else if (hit_start != -100000) {
						break;
					}
				}

				if (hit_start != -100000) {
					int best_dir = (hit_start + hit_end) / 2;
					int wt = ceil(9 * 500.0 + best_time * 1000.0 + 200.0);
					return {Ch, Lh, Rh, best_dir, 10, wt};
				}
			}
		}
	}
	return {0, 0, 0, 0, 10, 5000};
}

void solve(int idx) {
	int n = inp[idx].size();
	vector<Point<ld>> holes(n);
	rep(i, 0, n) {
		holes[i].x = inp[idx][i].first;
		holes[i].y = inp[idx][i].second;
	}
	
	vector<int> ord(n);
	rep(i, 0, n) ord[i] = i;
	sort(ord.begin(), ord.end(), [&](int a, int b) {
		return holes[a].dist() < holes[b].dist();
	});
	
	int sim_time_ms = 0;
	int hit_targets = 0;
	int total_valid_balls = 0;
	const int TIME_LIMIT = 100000;

	vector<int> hole_balls(n, 0);

	rep(i, 0, n) {
		state ans = calc(ord[i], holes);
		sim_time_ms += 1700;
		
		rep(k, 0, ans.nu) {
			int shoot_time = sim_time_ms + k * 500;
			SimResult res = simulate_full(ans.theta, ans.ch, ans.lh, ans.rh, holes);
			
			if (res.hit_hole_idx != -1) {
				int reach_time = shoot_time + (int)ceil(res.time_taken * 1000.0);
				if (shoot_time <= TIME_LIMIT && reach_time <= sim_time_ms + ans.wt) {
					hole_balls[res.hit_hole_idx]++;
				}
			}
		}

		sim_time_ms += ans.wt;

		cout << ans.ch << ',' << ans.lh << ',' << ans.rh << ',' 
			 << ans.theta << ',' << ans.nu << ',' << ans.wt;
		if (i != n - 1) cout << "\n";
		else cout << ";\n";
	}
	
	rep(i, 0, n) {
		int valid = min(hole_balls[i], 10);
		if (valid > 0) {
			hit_targets++;
			total_valid_balls += valid;
		}
	}

	int score = hit_targets * total_valid_balls;
	cerr << "Test Case " << idx + 1 << " | Score: " << score 
		 << " (Targets: " << hit_targets << ", Balls: " << total_valid_balls 
		 << ", Total Time: " << sim_time_ms / 1000.0 << "s / 100.0s)\n";
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	(void)INF;
	(void)INFLL;
	rep(i, 0, 5) {
		solve(i);
	}
	return 0;
}
