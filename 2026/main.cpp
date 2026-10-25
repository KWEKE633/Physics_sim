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

template <class T>
using pq = priority_queue<T>;
template <class T>
using rpq = priority_queue<T, vector<T>, greater<T>>;

constexpr ll INFLL = (1LL << 60);
constexpr int INF = (1 << 30);

template <class T>
bool chmax(T &a, const T &b)
{
	if (a < b)
	{
		a = b;
		return true;
	}
	return false;
}

template <class T>
bool chmin(T &a, const T &b)
{
	if (a > b)
	{
		a = b;
		return true;
	}
	return false;
}

template <class T>
void read(vec<T> &a)
{
	for (auto &x : a)
		cin >> x;
}

template <class T>
void print_el(const vec<T> &a)
{
	for (auto &x : a)
		cout << x << "\n";
}

template <class T>
void print(const vec<T> &a)
{
	rep(i, 0, (int)a.size())
	{
		if (i)
			cout << " ";
		cout << a[i];
	}
	cout << el;
}

int readInt()
{
	int res = 0;
	char c;
	while (cin >> c && !isdigit(c))
		;
	res = c - '0';
	while (cin.get(c) && isdigit(c))
		res = res * 10 + (c - '0');
	return res;
}

set<char> skip = {' ', '"', '\n'};
char start_char = '{';
char end_char = '}';

string input(string s)
{
	vec<string> T = {""};
	vec<int> index;
	stack<int> st;
	st.push(0);
	index = {-1};
	rep(i, 0, (int)s.size())
	{
		if (skip.count(s[i]))
			continue;
		bool add = false;
		if (s[i] == start_char)
		{
			add = true;
			st.push(index.size());
			index.push_back(-1);
		}
		else if (s[i] == end_char)
			add = true, st.pop();
		else if (s[i] == ',')
		{
			add = true;
			index[st.top()]--;
		}
		else
		{
			if (T.back().empty())
			{
				index.push_back((int)T.size() - 1);
			}
			T.back() += s[i];
		}
		if (add && !T.back().empty())
		{
			T.push_back("");
		}
	}
	string res;
	for (auto i : index)
	{
		if (i < 0)
			res += (to_string(-i));
		else
			res += T[i];
		res += "\n";
	}
	return res;
}

vvec<tuple<ld, ld, ld>> inp = {
    {{-450, 370, 50}, {450, 480, 50}, {-450, 630, 50}, {450, 680, 50}, {-435, 870, 50}, {-285, 920, 75}, {-205, 840, 50}, {-35, 870, 50}, {95, 850, 50}, {295, 880, 50}, {445, 940, 50}, {-425, 1090, 100}, {-255, 1040, 100}, {-95, 1030, 100}, {25, 1080, 75}, {105, 970, 100}, {255, 1070, 100}, {425, 1110, 100}},
};

vvec<tuple<ld, ld, ld>> sim_inp = {
    {{0,400,50},{0,465,30},{0,550,100},{0,635,30},{0,700,50}},
    {{310,500,100},{225,500,30},{-150,500,50},{-85,500,30},{0,500,100},{85,500,30},{150,500,50},{-225,500,30},{-310,500,100}},
    {{0,350,30},{-100,450,30},{100,450,30},{-200,550,30},{0,550,30},{200,550,30},{-300,650,30},{-100,650,30},{100,650,30},{300,650,30}},
    {{-350,250,100},{350,250,100},{-250,385,100},{250,385,100},{-150,520,100},{150,520,100},{-250,655,100},{250,655,100},{-350,790,100},{350,790,100}},
    {{352,980,30},{-299,1002,100},{252,904,60},{-148,847,40},{3,789,30},{286,671,30},{-220,675,80},{50,640,50},{-154,587,30},{252,498,120},{-53,512,70},{-243,430,40},{423,369,30},{-267,332,70},{319,305,40},{0,289,90},{401,251,100},{-425,260,100}},
};

template <class T> struct Point
{
    T x, y, z;
    using P = Point;
    Point(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) {}
    bool operator<(P p) const { return tie(x, y) < tie(p.x, p.y); }
    bool operator==(P p) const { return tie(x, y) == tie(p.x, p.y); }
    P operator+(P p) const { return P(x + p.x, y + p.y); }
    P operator-(P p) const { return P(x - p.x, y - p.y); }
    P operator*(T d) const { return P(x * d, y * d); }
    P operator/(T d) const { return P(x / d, y / d); }
    T dist() const { return hypot(x, y); }
};

ld final_score = 0.0;
const ld G = 9807.0;
const ld E = 0.9;                // 公式シミュレータ用
const ld E_REAL = 0.76;         // 実機用の暫定推定値
const ld E_FLOOR = 0.76;        // 床の反発係数
[[maybe_unused]]const ld DT = 0.001;           // 元コードの定数を維持（解析解に変更したため未使用）
const ld START_HIGH = 700.0;
const ld PI = acosl(-1);
const ld BALL_RADIUS = 12.7;
const int WT = 2000;
const int ACTION_MS = 1500 + WT;
const int TIME_LIMIT_MS = 100000;
const int MAX_ROW = min(50, TIME_LIMIT_MS / ACTION_MS); // 28行
bool real_mode = true;

struct Judge2025
{
    int sim_time_ms = 0;
    int hit_targets = 0;
    ld total_score = 0.0;
    const int TIME_LIMIT = 100000;
    const ld G = 9807.0;
    const ld START_HIGH = 700.0;
    const ld E = 0.9;

    // 公式シミュレータ用の簡易ローカル判定。実機の精密な再現ではない。
    void execute_action(int tn, int Ir, int Rr, int nu, int wt, ld zh, Point<ld> target, ld ratio)
    {
        (void)tn;
        int action_time = 1500 + wt * max(1, nu);
        ld drop_time_ms = sim_time_ms + 1500.0 + wt + 500.0;
        if (nu > 0 && drop_time_ms <= TIME_LIMIT)
        {
            ld ir_rad = Ir * PI / 180000.0;
            ld rr_rad = Rr * PI / 180000.0;
            ld v0 = sqrtl(2.0 * G * (START_HIGH - zh));
            ld baseVA = sinl(ir_rad) * v0;
            ld baseVB = cosl(ir_rad) * v0 * E;
            ld vz = cosl(ir_rad) * baseVB - sinl(ir_rad) * baseVA;
            ld v_horiz = cosl(ir_rad) * baseVA + sinl(ir_rad) * baseVB;
            ld flight_time_s = (vz + sqrtl(vz * vz + 2 * G * zh)) / G;
            ld land_time_ms = drop_time_ms + flight_time_s * 1000.0;
            if (land_time_ms <= TIME_LIMIT && flight_time_s * 1000.0 <= wt)
            {
                ld land_x = v_horiz * sinl(rr_rad) * flight_time_s;
                ld land_y = v_horiz * cosl(rr_rad) * flight_time_s;
                ld dist_to_target = hypotl(land_x - target.x, land_y - target.y);
                if (dist_to_target <= ratio / 2.0 - BALL_RADIUS)
                {
                    hit_targets++;
                    ld bonus = fabsl(Rr / 1000.0L) + zh / 10.0L + Ir / 1000.0L + target.dist() / 100.0L;
                    bonus = floorl(bonus * 1000.0L) / 1000.0L;
                    total_score += 100.0L + bonus;
                }
            }
        }
        sim_time_ms += action_time;
    }
    void print_result(int tc, int n)
    {
        final_score += total_score;
        cerr << "Test Case " << tc << " | 獲得点数: " << fixed << setprecision(3) << total_score
             << " 点 (的中: " << hit_targets << "/" << n
             << " | 経過時間: " << sim_time_ms / 1000.0 << "s / 100.0s)\n";
    }
};

struct State
{
    int tn, Ir, Rr, nu, wt;
    ld added_score;
};
struct SimResult
{
    ld landing_x, landing_y, time_taken;
};

// 解析解。最初の着地点を返す（シミュレータ用）。
SimResult simulate_full(int ir_1000, int rr_1000, ld zh)
{
    ld ir_rad = ir_1000 * PI / 180000.0L;
    ld rr_rad = rr_1000 * PI / 180000.0L;
    ld v0 = sqrtl(2.0L * G * (START_HIGH - zh));
    ld v = (1.0L + (real_mode ? E_REAL : E)) * v0 * sinl(ir_rad) * cosl(ir_rad);
    ld vz = (real_mode ? E_REAL : E) * v0 * cosl(ir_rad) * cosl(ir_rad) - v0 * sinl(ir_rad) * sinl(ir_rad);
    ld t = (vz + sqrtl(vz * vz + 2.0L * G * zh)) / G;
    return {v * t * sinl(rr_rad), v * t * cosl(rr_rad), t};
}

struct Shot
{
    bool valid = false;
    int ir = 5000, rr = 0, bounce = 0;
    ld margin = -1e18, score = -1e18, time_taken = 0;
};

ld get_score(int ir, int rr, ld zh, Point<ld> tg)
{
    ld dist_bonus = floorl(tg.dist() * 10.0L) / 1000.0L;
    return 100.0L + abs(rr) / 1000.0L + zh / 10.0L + ir / 1000.0L + dist_bonus;
}

// 実機では前の着地点で他の穴に入ってしまう射撃を候補から除外。
bool blocked_before_bounce(ld p0, ld c, ld s, const vec<Point<ld>> &holes)
{
    ld x = p0 * s, y = p0 * c;
    for (const auto &tg : holes)
        if (hypotl(tg.x - x, tg.y - y) <= tg.z / 2.0L)
            return true;
    return false;
}

// 1組の (row, target) について角度を探索。
Shot search_shot(int target_idx, ld zh, const vec<Point<ld>> &holes)
{
    Point<ld> tg = holes[target_idx];
    ld q = tg.dist();
    ld limit = tg.z / 2.0L - BALL_RADIUS;
    Shot best;
    if (limit <= 0 || q <= 0) return best;
    ld phi = atan2l(tg.x, tg.y);
    ld e = real_mode ? E_REAL : E;
    ld v0 = sqrtl(2.0L * G * (START_HIGH - zh));
    int end_ir = real_mode ? 20000 : 45000;
    // シミュレータでは角度ボーナス最大化、実機では中心への命中余裕最大化。
    for (int ir = 5000; ir <= end_ir; ++ir)
    {
        ld theta = ir * PI / 180000.0L;
        ld st = sinl(theta), ct = cosl(theta);
        ld v = (1.0L + e) * v0 * st * ct;
        ld vz = e * v0 * ct * ct - v0 * st * st;
        ld t0 = (vz + sqrtl(vz * vz + 2.0L * G * zh)) / G;
        ld p0 = v * t0;
        ld w = vz - G * t0;
        ld t1 = -2.0L * E_FLOOR * w / G;
        ld pos[2] = {p0, p0 + v * t1};
        int cnt = real_mode ? 2 : 1;
        rep(b, 0, cnt)
        {
            ld p = pos[b];
            if (fabsl(p - q) > limit || p <= 0) continue;
            ld cos_bound = (p * p + q * q - limit * limit) / (2.0L * p * q);
            cos_bound = max(-1.0L, min(1.0L, cos_bound));
            ld angle_bound = acosl(cos_bound);
            vec<int> rrs;
            int rr_center = (int)roundl(phi * 180000.0L / PI);
            if (real_mode)
            {
                // 反射係数の実測誤差に備え、まず中心に向ける。
                rrs = {rr_center, rr_center - 1, rr_center + 1};
            }
            else
            {
                // 得点は |Rr| の増加でも上がるため、命中可能な角度区間の両端を調べる。
                ld lo = max(-55000.0L, (phi - angle_bound) * 180000.0L / PI);
                ld hi = min(55000.0L, (phi + angle_bound) * 180000.0L / PI);
                if (lo > hi) continue;
                // 浮動小数点誤差を避け、境界から1/1000度だけ内側にする。
                int a = (int)ceill(lo + 0.000001L);
                int z = (int)floorl(hi - 0.000001L);
                rrs = {a, z, rr_center};
            }
            for (int rr : rrs)
            {
                if (rr < -55000 || rr > 55000) continue;
                ld rr_rad = rr * PI / 180000.0L;
                ld c = cosl(rr_rad), s = sinl(rr_rad);
                ld diff = hypotl(p * s - tg.x, p * c - tg.y);
                ld margin = limit - diff;
                if (margin < 1e-7L) continue;
                if (b == 1 && blocked_before_bounce(p0, c, s, holes)) continue;
                // 実機では1回目の着地が別の穴でも、狙った穴でも遮られる。
                ld score = get_score(ir, rr, zh, tg);
                bool better = !best.valid;
                if (real_mode)
                {
                    if (margin > best.margin + 1e-8L) better = true;
                    else if (fabsl(margin - best.margin) <= 1e-8L && score > best.score) better = true;
                }
                else
                {
                    if (score > best.score + 1e-8L) better = true;
                    else if (fabsl(score - best.score) <= 1e-8L && margin > best.margin) better = true;
                }
                if (better)
                    best = {true, ir, rr, b, margin, score, t0 + (b ? t1 : 0)};
            }
        }
    }
    return best;
}

// 元の calc は削除せず、現在の候補探索を呼び出す形で残す。
State calc(int target_idx, int target_id_1indexed, ld zh, const vector<Point<ld>> &holes)
{
    Shot shot = search_shot(target_idx, zh, holes);
    if (!shot.valid) return {target_id_1indexed, 5000, 0, 0, WT, 0.0L};
    return {target_id_1indexed, shot.ir, shot.rr, 1, WT, shot.score};
}

void solve(int idx)
{
    int n = inp[idx].size();
    vec<Point<ld>> target(n);
    rep(i, 0, n)
    {
        target[i].x = get<0>(inp[idx][i]);
        target[i].y = get<1>(inp[idx][i]);
        target[i].z = get<2>(inp[idx][i]);
    }
    vec<int> ord(n);
    rep(i, 0, n) ord[i] = i;
    // 元の距離順ソートは残す。DPではこの順序に依存しない。
    sort(ord.begin(), ord.end(), [&](int a, int b) { return target[a].dist() > target[b].dist(); });
    Judge2025 judge;

    int rows = MAX_ROW;
    vvec<Shot> shots(rows, vec<Shot>(n));
    rep(row, 0, rows)
    {
        ld zh = 420 - 5 * row;
        rep(i, 0, n) shots[row][i] = search_shot(i, zh, target);
    }
    const int states = 1 << n;
    const ll NEG = -(1LL << 60);
    // スコアは0.001点単位でDPする。
    vec<ll> dp(states, NEG), ndp(states, NEG);
    vec<int8_t> pr((rows + 1) * states, -2);
    dp[0] = 0;
    pr[0] = -1;
    rep(row, 0, rows)
    {
        fill(ndp.begin(), ndp.end(), NEG);
        rep(mask, 0, states)
        {
            if (dp[mask] == NEG) continue;
            if (chmax(ndp[mask], dp[mask])) pr[(row + 1) * states + mask] = -1;
            rep(j, 0, n)
            {
                if ((mask >> j) & 1) continue;
                if (!shots[row][j].valid) continue;
                int nm = mask | (1 << j);
                ll add = llroundl(shots[row][j].score * 1000.0L);
                if (chmax(ndp[nm], dp[mask] + add)) pr[(row + 1) * states + nm] = (int8_t)j;
            }
        }
        dp.swap(ndp);
    }
    int best_mask = 0;
    rep(mask, 0, states) if (dp[mask] > dp[best_mask]) best_mask = mask;
    vec<State> ans;
    int mask = best_mask;
    for (int row = rows; row > 0; --row)
    {
        int j = pr[row * states + mask];
        if (j == -1)
        {
            ans.pb({1, 5000, 0, 0, WT, 0});
        }
        else
        {
            assert(j >= 0 && shots[row - 1][j].valid);
            Shot s = shots[row - 1][j];
            ans.pb({j + 1, s.ir, s.rr, 1, WT, s.score});
            mask ^= 1 << j;
        }
    }
    reverse(ans.begin(), ans.end());
    // 復元結果の後方にある不要なスキップ行は省く。
    while (ans.size() > 1 && ans.back().nu == 0) ans.pop_back();
    int used = 1;
    rep(i, 0, (int)ans.size())
    {
        State &s = ans[i];
        if (s.nu == 0) s.tn = used;
        used = s.tn;
        cout << s.tn << ',' << s.Ir << ',' << s.Rr << ',' << s.nu << ',' << s.wt << '\n';
        if (!real_mode && s.nu > 0)
        {
            ld zh = 420 - 5 * i;
            judge.execute_action(s.tn, s.Ir, s.Rr, s.nu, s.wt, zh, target[s.tn - 1], target[s.tn - 1].z);
        }
        else if (!real_mode)
        {
            ld zh = 420 - 5 * i;
            judge.execute_action(s.tn, s.Ir, s.Rr, 0, s.wt, zh, target[s.tn - 1], target[s.tn - 1].z);
        }
    }
    // judge.print_result(idx + 1, n); // 必要ならローカル判定を表示
    cerr << "case=" << idx + 1 << " score_est=" << fixed << setprecision(3)
         << (ld)dp[best_mask] / 1000.0L << " hits=" << __builtin_popcount((unsigned)best_mask)
         << " rows=" << ans.size() << " mode=" << (real_mode ? "real" : "sim") << '\n';
}

int main(int argc, char **argv)
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    (void)INF;
    (void)INFLL;
    if (argc >= 2 && string(argv[1]) == "--sim")
    {
        real_mode = false;
        auto old_inp = inp;
        inp = sim_inp;
        rep(i, 0, (int)inp.size())
        {
            if (i) cout << ";\n";
            solve(i);
        }
        cout << ";\n";
        inp = old_inp;
    }
    else
    {
        real_mode = true;
        solve(0);
    }
    return 0;
}
