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
{
{
0, 950}, {
0, 850}, {
0, 750}, {
0, 650}, {
0, 550}, {
0, 450}, {
0, 350}, {
0, 250}}
,{
{
0, 850}, {
100, 850}, {
-100, 850}, {
200, 850}, {
-200, 850}, {
300, 850}, {
-300, 850}}
,{
{
0, 350}, {
-100, 550}, {
100, 550}, {
-200, 750}, {
0, 750}, {
200, 750}, {
-300, 950}, {
-100, 950}, {
100, 950}, {
300, 950}}
,{
{
-300, 650}, {
300, 650}, {
-255, 550}, {
255, 550}, {
-210, 450}, {
210, 450}, {
-165, 350}, {
165, 350}, {
-120, 250}, {
120, 250}}
,{
{
-300, 350}, {
-175, 350}, {
-50, 350}, {
75, 350}, {
200, 350}, {
325, 350}, {
-325, 550}, {
-200, 550}, {
-75, 550}, {
50, 550}, {
175, 550}, {
300, 550}, {
-300, 750}, {
-175, 750}, {
-50, 750}, {
75, 750}, {
200, 750}, {
325, 750}}
};

template<class T>
struct Point {
	T x, y;
	using P = Point;
	Point(T _x = 0, T _y = 0) : x(_x), y(_y) {}
	bool operator<(P p) const { return tie(x, y) < tie(p.x, p.y); }
    bool operator==(P p) const { return tie(x, y) == tie(p.x, p.y); }
    P operator+(P p) const { return P(x + p.x, y + p.y); }
    P operator-(P p) const { return P(x - p.x, y - p.y); }
    P operator*(T d) const { return P(x * d, y * d); }
    P operator/(T d) const { return P(x / d, y / d); }
	T dist() const {
		return hypot(x, y);
	}
};

ld final_score = 0.0;

struct Judge2025 {
	int sim_time_ms = 0;
	int hit_targets = 0;
	ld total_score = 0.0;
	
	const int TIME_LIMIT = 100000;
	const ld G = 9807.0;
	const ld START_HIGH = 700.0;
	const ld E = 0.9;
	const ld HIT_THRESHOLD = 25.0 - 12.7; // ターゲット内径 - 球の半径 = 12.3mm

	// 1ターンのアクションを実行し、時間とスコアを更新する関数
	void execute_action(int tn, int Ir, int Rr, int nu, int wt, ld zh, Point<ld> target) {
		(void)tn;
		int action_time = 1500 + wt;
		ld drop_time_ms = sim_time_ms + 1500.0; // 軸移動完了後に投下

		// 全体の100秒制限を超えていないか
		if (sim_time_ms + action_time <= TIME_LIMIT) {
			if (nu > 0) { // 球を投下した場合のみ物理判定
				ld ir_rad = Ir * M_PI / 180000.0;
				ld rr_rad = Rr * M_PI / 180000.0;
				ld v0 = sqrtl(2.0 * G * (START_HIGH - zh));

				ld baseVA = sinl(ir_rad) * v0;
				ld baseVB = cosl(ir_rad) * v0 * E;
				ld vz = cosl(ir_rad) * baseVB - sinl(ir_rad) * baseVA;
				ld v_horiz = cosl(ir_rad) * baseVA + sinl(ir_rad) * baseVB;

				ld vx_real = v_horiz * sinl(rr_rad);
				ld vy_real = v_horiz * cosl(rr_rad);

				// 中心がZ=0に達した時の時間を解の公式で計算
				ld flight_time_s = -1.0;
				if (vz * vz + 2 * G * zh >= 0) {
					flight_time_s = (vz + sqrtl(vz * vz + 2 * G * zh)) / G;
				}

				if (flight_time_s > 0) {
					ld land_x = vx_real * flight_time_s;
					ld land_y = vy_real * flight_time_s;
					ld land_time_ms = drop_time_ms + flight_time_s * 1000.0;

					// 着弾が100秒以内で、かつ設定した待ち時間Wtの間に着弾したか
					if (land_time_ms <= TIME_LIMIT && land_time_ms <= drop_time_ms + wt) {
						ld dist_to_target = hypotl(land_x - target.x, land_y - target.y);
						
						// 完全内包ルールの検証
						if (dist_to_target <= HIT_THRESHOLD) {
							hit_targets++;
							ld bonus = abs(Rr / 1000.0) + (zh / 10.0) + (Ir / 1000.0) + (target.dist() / 100.0);
							bonus = floorl(bonus * 1000.0) / 1000.0;
							total_score += 100.0 + bonus;
						}
					}
				}
			}
		}
		// アクション完了として全体の経過時間を進める
		sim_time_ms += action_time;
	}

	void print_result(int tc, int n) {
		final_score += total_score;
		cerr << "Test Case " << tc << " | 獲得点数: " << fixed << setprecision(3) << total_score 
			 << " 点 (的中: " << hit_targets << "/" << n 
			 << " | 経過時間: " << sim_time_ms / 1000.0 << "s / 100.0s)\n";
	}
};

struct State {
	int tn, Ir, Rr, nu, wt;
	ld added_score;
};

struct SimResult {
	ld landing_x;
	ld landing_y;
	ld time_taken;
};

const ld G = 9807.0;
const ld E = 0.9;
const ld DT = 0.001;
const ld START_HIGH = 700.0;
const ld PI = acosl(-1);
const ld HOLE_RADIUS = 25.0;
const ld BALL_RADIUS = 12.7;
const ld HIT_THRESHOLD = HOLE_RADIUS - BALL_RADIUS;

SimResult simulate_full(int ir_1000, int rr_1000, ld zh) {
	ld ir_rad = (ir_1000 / 1000.0) * PI / 180.0;
	ld rr_rad = (rr_1000 / 1000.0) * PI / 180.0;

	ld v0 = sqrtl(2.0 * G * (START_HIGH - zh));

	ld nx = sinl(ir_rad) * sinl(rr_rad);
	ld ny = sinl(ir_rad) * cosl(rr_rad);
	ld nz = cosl(ir_rad);

	ld v_dot_n = -v0 * nz;

	ld vx = - (1.0 + E) * v_dot_n * nx;
	ld vy = - (1.0 + E) * v_dot_n * ny;
	ld vz = -v0 - (1.0 + E) * v_dot_n * nz;

	ld px = 0.0, py = 0.0, pz = zh;
	ld t = 0.0;

	while (pz > BALL_RADIUS) {
		px += vx * DT;
		py += vy * DT;
		pz += vz * DT - 0.5 * G * DT * DT;
		vz -= G * DT;
		t += DT;
		if (py > 1100 || abs(px) > 400 || py < -100) break;
	}

	return {px, py, t};
}

State calc(int target_idx, int target_id_1indexed, ld zh, const vector<Point<ld>>& holes) {
	Point<ld> tg = holes[target_idx];
	ld angle_to_target = atan2l(tg.x, tg.y);
	int rr_est = roundl((angle_to_target * 180.0 / PI) * 1000.0);
	rr_est = max(-25000, min(25000, rr_est));

	int best_ir = 5000, best_rr = rr_est;
	ld min_dist = 1e18;
	ld best_time = 0.0;
	
	for (int ir = 5000; ir <= 45000; ir += 10) {
		for (int rr = rr_est - 2000; rr <= rr_est + 2000; rr += 200) {
			if (rr < -25000 || rr > 25000) continue;
			SimResult res = simulate_full(ir, rr, zh);
			ld d = hypotl(res.landing_x - tg.x, res.landing_y - tg.y);
			if (d < min_dist) {
				min_dist = d;
				best_ir = ir;
				best_rr = rr;
				best_time = res.time_taken;
			}
		}
	}

	if (min_dist <= HIT_THRESHOLD) {
		int wt = ceil(best_time * 1000.0 + 500.0 + 200.0);
		wt = max(2000, min(10000, wt));
		
		ld bonus = abs(best_rr / 1000.0) + (zh / 10.0) + (best_ir / 1000.0) + (tg.dist() / 100.0);
		bonus = floorl(bonus * 1000.0) / 1000.0;

		return {target_id_1indexed, best_ir, best_rr, 1, wt, bonus};
	}

	return {target_id_1indexed, 5000, 0, 1, 2000, 0.0};
}

void solve(int idx) {
	int n = inp[idx].size();
	vec<Point<ld>> target(n);
	rep(i, 0, n) {
		target[i].x = inp[idx][i].first;
		target[i].y = inp[idx][i].second;
	}
	vec<int> ord(n);
	rep(i, 0, n) ord[i] = i;
	sort(ord.begin(), ord.end(), [&](int a, int b) {
		return target[a].dist() > target[b].dist();
	});

	Judge2025 judge;

	rep(i, 0, 21 - n) {
		cout << "1,5000,0,0,2000\n";
		judge.execute_action(1, 5000, 0, 0, 2000, 0, Point<ld>(0, 0));
	}

	rep(i, 0, n) {
		ld zh = (i + 21 - n) * 20.0;
		auto ans = calc(ord[i], ord[i] + 1, zh, target);
		cout << ans.tn << ',' << ans.Ir << ',' << ans.Rr << ',' << ans.nu << ',' << ans.wt;
		if (i == n - 1) cout << ";\n";
		else cout << "\n";
		judge.execute_action(ans.tn, ans.Ir, ans.Rr, ans.nu, ans.wt, zh, target[ord[i]]);
	}
	judge.print_result(idx + 1, n);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	(void)INF;
	(void)INFLL;
	rep(i, 0, 5) solve(i);
	cerr << "\n[Final score]: " << final_score;
	cout << el;
	return 0;
}
