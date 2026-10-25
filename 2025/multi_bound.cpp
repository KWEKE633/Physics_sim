#include <bits/stdc++.h>
using namespace std;
#define rep(i, a, n) for (int i = a; i < n; ++i)
using ll = long long;
using ld = long double;
#define el endl

constexpr ll INFLL = (1LL<<60);
constexpr int INF = (1<<30);

template<class T> bool chmax(T &a, const T &b) { if (a < b) { a = b; return true; } return false; }
template<class T> bool chmin(T &a, const T &b) { if (a > b) { a = b; return true; } return false; }

template<class T>
struct Point {
    T x, y;
    using P = Point;
    Point(T _x = 0, T _y = 0) : x(_x), y(_y) {}
    bool operator<(P p) const { return tie(x, y) < tie(p.x, p.y); }
    bool operator==(P p) const { return tie(x, y) == tie(p.x, p.y); }
    T dist() const { return hypot(x, y); }
};

vector<vector<pair<ld, ld>>> inp = {
    {{0, 950}, {0, 850}, {0, 750}, {0, 650}, {0, 550}, {0, 450}, {0, 350}, {0, 250}},
    {{0, 850}, {100, 850}, {-100, 850}, {200, 850}, {-200, 850}, {300, 850}, {-300, 850}},
    {{0, 350}, {-100, 550}, {100, 550}, {-200, 750}, {0, 750}, {200, 750}, {-300, 950}, {-100, 950}, {100, 950}, {300, 950}},
    {{-300, 650}, {300, 650}, {-255, 550}, {255, 550}, {-210, 450}, {210, 450}, {-165, 350}, {165, 350}, {-120, 250}, {120, 250}},
    {{-300, 350}, {-175, 350}, {-50, 350}, {75, 350}, {200, 350}, {325, 350}, {-325, 550}, {-200, 550}, {-75, 550}, {50, 550}, {175, 550}, {300, 550}, {-300, 750}, {-175, 750}, {-50, 750}, {75, 750}, {200, 750}, {325, 750}}
};

ld final_score = 0.0;
const ld G = 9807.0;
const ld E = 0.9;
const ld START_HIGH = 700.0;
const ld PI = acosl(-1);
const ld HOLE_RADIUS = 25.0;
const ld BALL_RADIUS = 12.7;
const ld HIT_THRESHOLD = HOLE_RADIUS - BALL_RADIUS;

// ==========================================
// 【新設】3次元幾何学・多段バウンドエンジン
// ==========================================
struct Point3D {
    ld x, y, z;
    Point3D(ld _x = 0, ld _y = 0, ld _z = 0) : x(_x), y(_y), z(_z) {}
    Point3D operator+(Point3D p) const { return Point3D(x+p.x, y+p.y, z+p.z); }
    Point3D operator-(Point3D p) const { return Point3D(x-p.x, y-p.y, z-p.z); }
    Point3D operator*(ld d) const { return Point3D(x*d, y*d, z*d); }
    ld dot(Point3D p) const { return x*p.x + y*p.y + z*p.z; }
};

struct Plane {
    Point3D n;   // 法線ベクトル
    Point3D p0;  // 平面上の点
    ld e;        // 反発係数
    bool is_target_floor; // この平面が最終目的地（床）かどうか
};

struct SimResult {
    ld landing_x, landing_y, time_taken;
};

// 解析解（二次方程式）を用いた汎用シミュレータ
SimResult simulate_full(int ir_1000, int rr_1000, ld zh) {
    ld ir_rad = (ir_1000 / 1000.0) * PI / 180.0;
    ld rr_rad = (rr_1000 / 1000.0) * PI / 180.0;

    // 1. 世界の定義
    Point3D A(0, 0, -G); // 重力加速度
    Point3D P(0, 0, START_HIGH); // 初期位置
    Point3D V(0, 0, 0);  // 初速

    vector<Plane> planes;
    
    // 平面1：バウンド盤（法線ベクトルを算出し、Z=zhに配置）
    Point3D plate_n(sinl(ir_rad) * sinl(rr_rad), sinl(ir_rad) * cosl(rr_rad), cosl(ir_rad));
    planes.push_back({plate_n, Point3D(0, 0, zh), E, false});
    
    // 平面2：ターゲットのある床（法線は真上、Z=ボール半径に配置）
    Point3D floor_n(0, 0, 1);
    planes.push_back({floor_n, Point3D(0, 0, BALL_RADIUS), 0.0, true});

    ld total_time = 0;

    // 2. 衝突ループ（板や壁が増えても自動で対応する）
    for (int step = 0; step < 10; ++step) { // 最大バウンド数制限（暴走防止）
        ld best_t = INF;
        int hit_plane_idx = -1;

        // 全ての平面に対して、放物線との交点時間を計算
        rep(i, 0, (int)planes.size()) {
            ld a = 0.5 * planes[i].n.dot(A);
            ld b = planes[i].n.dot(V);
            ld c = planes[i].n.dot(P - planes[i].p0);

            ld t = -1.0;
            if (abs(a) < 1e-9) { // 平面が垂直な壁の場合
                if (abs(b) > 1e-9) t = -c / b;
            } else { // 通常の二次方程式
                ld D = b * b - 4.0 * a * c;
                if (D >= 0) {
                    ld t1 = (-b - sqrtl(D)) / (2.0 * a);
                    ld t2 = (-b + sqrtl(D)) / (2.0 * a);
                    if (t1 > 1e-6 && t2 > 1e-6) t = min(t1, t2);
                    else if (t1 > 1e-6) t = t1;
                    else if (t2 > 1e-6) t = t2;
                }
            }
            
            // 最も早く衝突する平面を見つける
            if (t > 1e-6 && chmin(best_t, t)) hit_plane_idx = i;
        }

        if (hit_plane_idx == -1) break; // 宇宙の彼方へ飛んでいった

        // 時間を進め、衝突直前の位置と速度を更新
        P = P + V * best_t + A * (0.5 * best_t * best_t);
        V = V + A * best_t;
        total_time += best_t;

        // もし床に当たったならシミュレーション完了！
        if (planes[hit_plane_idx].is_target_floor) {
            return {P.x, P.y, total_time};
        }

        // 床以外（バウンド盤など）に当たった場合、ベクトル方程式で反射させる
        ld v_dot_n = V.dot(planes[hit_plane_idx].n);
        V = V - planes[hit_plane_idx].n * ((1.0 + planes[hit_plane_idx].e) * v_dot_n);
    }
    
    return {0, 0, -1.0}; // 失敗
}
// ==========================================


// 【完全準拠ジャッジ】
struct Judge2025 {
    int sim_time_ms = 0;
    int hit_targets = 0;
    ld total_score = 0.0;
    const int TIME_LIMIT = 100000;

    void execute_action(int tn, int Ir, int Rr, int nu, int wt, ld zh, Point<ld> target) {
        (void)tn;
        int action_time = 1500 + wt;
        ld drop_time_ms = sim_time_ms + 1500.0;

        if (sim_time_ms + action_time <= TIME_LIMIT) {
            if (nu > 0) { 
                // ジャッジ側も汎用シミュレータと同じ結果を呼び出す（完全互換）
                SimResult res = simulate_full(Ir, Rr, zh);

                if (res.time_taken > 0) {
                    ld land_time_ms = drop_time_ms + res.time_taken * 1000.0;
                    if (land_time_ms <= TIME_LIMIT && land_time_ms <= drop_time_ms + wt) {
                        ld dist_to_target = hypotl(res.landing_x - target.x, res.landing_y - target.y);
                        
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

State calc(int target_idx, int target_id_1indexed, ld zh, const vector<Point<ld>>& holes) {
    Point<ld> tg = holes[target_idx];
    ld angle_to_target = atan2l(tg.x, tg.y);
    int rr_est = roundl((angle_to_target * 180.0 / PI) * 1000.0);
    rr_est = max(-25000, min(25000, rr_est));

    int best_ir = 5000, best_rr = rr_est;
    ld min_dist = 1e18;
    ld best_time = 0.0;
    
    // 探索解像度 Ir+=10, Rr+=200 で精密狙撃
    for (int ir = 5000; ir <= 45000; ir += 10) {
        for (int rr = rr_est - 2000; rr <= rr_est + 2000; rr += 200) {
            if (rr < -25000 || rr > 25000) continue;
            SimResult res = simulate_full(ir, rr, zh);
            if (res.time_taken < 0) continue;
            
            ld d = hypotl(res.landing_x - tg.x, res.landing_y - tg.y);
            if (chmin(min_dist, d)) {
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
    vector<Point<ld>> target(n);
    rep(i, 0, n) {
        target[i].x = inp[idx][i].first;
        target[i].y = inp[idx][i].second;
    }
    vector<int> ord(n);
    rep(i, 0, n) ord[i] = i;
    
    // 遠くの的ほど、バウンド盤が低くエネルギーが必要な序盤に狙う
    sort(ord.begin(), ord.end(), [&](int a, int b) {
        return target[a].dist() > target[b].dist();
    });

    Judge2025 judge;

    // 【重要修正】本命のターゲットを先に消化し、最大エネルギー(zhが低い状態)を保証する
    rep(i, 0, n) {
        ld zh = i * 20.0; 
        auto ans = calc(ord[i], ord[i] + 1, zh, target);
        
        cout << ans.tn << ',' << ans.Ir << ',' << ans.Rr << ',' << ans.nu << ',' << ans.wt;
        if (i == n - 1 && n == 21) cout << ";\n";
        else cout << "\n";
        
        judge.execute_action(ans.tn, ans.Ir, ans.Rr, ans.nu, ans.wt, zh, target[ord[i]]);
    }

    // 余ったターン（空撃ち）は最後に消化する
    rep(i, 0, 21 - n) {
        cout << "1,5000,0,0,2000";
        if (i == (21 - n) - 1) cout << ";\n";
        else cout << "\n";
        
        judge.execute_action(1, 5000, 0, 0, 2000, 0, Point<ld>(0, 0));
    }
    
    judge.print_result(idx + 1, n);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    (void)INF;
    (void)INFLL;
    rep(i, 0, 5) solve(i);
    cerr << "\n[Final score]: " << fixed << setprecision(3) << final_score << "\n";
    cout << el;
    return 0;
}
