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

// 【重要】バウンド盤の物理的な大きさ（半径）
const ld PLATE_RADIUS = 50.0; 

// ==========================================
// 3次元幾何学・多段バウンドエンジン
// ==========================================
struct Point3D {
    ld x, y, z;
    Point3D(ld _x = 0, ld _y = 0, ld _z = 0) : x(_x), y(_y), z(_z) {}
    Point3D operator+(Point3D p) const { return Point3D(x+p.x, y+p.y, z+p.z); }
    Point3D operator-(Point3D p) const { return Point3D(x-p.x, y-p.y, z-p.z); }
    Point3D operator*(ld d) const { return Point3D(x*d, y*d, z*d); }
    ld dot(Point3D p) const { return x*p.x + y*p.y + z*p.z; }
    ld dist() const { return sqrtl(x*x + y*y + z*z); }
};

// // 3次元ベクトル構造体に、外積（cross）の機能を追加しておきます
// struct Point3D {
//     ld x, y, z;
//     Point3D(ld _x = 0, ld _y = 0, ld _z = 0) : x(_x), y(_y), z(_z) {}
//     Point3D operator+(Point3D p) const { return Point3D(x+p.x, y+p.y, z+p.z); }
//     Point3D operator-(Point3D p) const { return Point3D(x-p.x, y-p.y, z-p.z); }
//     Point3D operator*(ld d) const { return Point3D(x*d, y*d, z*d); }
//     ld dot(Point3D p) const { return x*p.x + y*p.y + z*p.z; }
//     // 外積（2つのベクトルに垂直なベクトルを作る）
//     Point3D cross(Point3D p) const {
//         return Point3D(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
//     }
//     ld dist() const { return sqrtl(x*x + y*y + z*z); }
// };

// // 平面構造体を「円形」と「長方形」の両方に対応させる
// struct Plane {
//     Point3D n;   // 法線ベクトル
//     Point3D p0;  // 平面上の中心点
//     ld e;        // 反発係数
//     bool is_target_floor; 
    
//     // 円形の場合
//     ld radius;   // 0以下なら円形判定をしない
    
//     // 【新規】長方形の場合
//     bool is_rect;
//     ld width;    // 板の横幅
//     ld height;   // 板の縦幅（斜面の長さ）
//     Point3D local_x; // 板にとっての「真横」方向ベクトル
//     Point3D local_y; // 板にとっての「縦（斜面）」方向ベクトル
// };

struct Plane {
    Point3D n;   // 法線ベクトル
    Point3D p0;  // 平面上の中心点
    ld e;        // 反発係数
    bool is_target_floor; 
    ld radius;   // 【新規】平面の半径（0以下の場合は無限平面として扱う）
};

struct SimResult {
    ld landing_x, landing_y, time_taken;
};

// 解析解（二次方程式）を用いた汎用シミュレータ (有限サイズ対応)
SimResult simulate_full(int ir_1000, int rr_1000, ld zh) {
    ld ir_rad = (ir_1000 / 1000.0) * PI / 180.0;
    ld rr_rad = (rr_1000 / 1000.0) * PI / 180.0;

    Point3D A(0, 0, -G); 
    Point3D P(0, 0, START_HIGH); 
    Point3D V(0, 0, 0);  

    vector<Plane> planes;
    
    // 平面1：有限サイズのバウンド盤（半径 PLATE_RADIUS を設定）
    Point3D plate_n(sinl(ir_rad) * sinl(rr_rad), sinl(ir_rad) * cosl(rr_rad), cosl(ir_rad));
    planes.push_back({plate_n, Point3D(0, 0, zh), E, false, PLATE_RADIUS});
    
    // 平面2：ターゲットのある床（無限に広がる床として扱うため radius = -1.0）
    Point3D floor_n(0, 0, 1);
    planes.push_back({floor_n, Point3D(0, 0, BALL_RADIUS), 0.0, true, -1.0});

	// Point3D plate_n(sinl(ir_rad) * sinl(rr_rad), sinl(ir_rad) * cosl(rr_rad), cosl(ir_rad));
    
    // // 【新規】ローカル座標軸の算出
    // // 板のローカルX軸（地面に対して常に水平な横方向）
    // Point3D local_x(cosl(rr_rad), -sinl(rr_rad), 0);
    // // 板のローカルY軸（斜面に沿った縦方向。法線とローカルXの外積で作る）
    // Point3D local_y = plate_n.cross(local_x);

    // // 長方形のバウンド盤として登録
    // planes.push_back({
    //     plate_n, Point3D(0, 0, zh), E, false, 
    //     -1.0,  // radius (円判定しない)
    //     true,  // is_rect (長方形判定する)
    //     100.0, // width
    //     150.0, // height
    //     local_x, local_y
    // });

    ld total_time = 0;

    for (int step = 0; step < 10; ++step) { 
        ld best_t = INF;
        int hit_plane_idx = -1;

        rep(i, 0, (int)planes.size()) {
            ld a = 0.5 * planes[i].n.dot(A);
            ld b = planes[i].n.dot(V);
            ld c = planes[i].n.dot(P - planes[i].p0);

            ld t1 = -1.0, t2 = -1.0;
            if (abs(a) < 1e-9) { 
                if (abs(b) > 1e-9) t1 = -c / b;
            } else { 
                ld D = b * b - 4.0 * a * c;
                if (D >= 0) {
                    t1 = (-b - sqrtl(D)) / (2.0 * a);
                    t2 = (-b + sqrtl(D)) / (2.0 * a);
                }
            }
            
            // 【新規】交点座標が板の「枠内」に収まっているか判定するラムダ関数
            auto is_valid_hit = [&](ld t, Plane& pl) {
                if (t <= 1e-6) return false; // 過去の時間は無視
                if (pl.radius <= 0.0) return true; // 無限平面なら常にOK
                Point3D hit_pos = P + V * t + A * (0.5 * t * t);
                return (hit_pos - pl.p0).dist() <= pl.radius;
            };

			// auto is_valid_hit = [&](ld t, Plane& pl) {
            //     if (t <= 1e-6) return false;
            //     Point3D hit_pos = P + V * t + A * (0.5 * t * t);
                
            //     // 1. 円形判定モード
            //     if (!pl.is_rect && pl.radius > 0.0) {
            //         return (hit_pos - pl.p0).dist() <= pl.radius;
            //     }
                
            //     // 2. 【新規】長方形判定モード
            //     if (pl.is_rect) {
            //         // 板の中心から衝突点へのベクトル
            //         Point3D v_hit = hit_pos - pl.p0;
                    
            //         // 内積を使って、ローカルなX方向とY方向にどれだけズレているか（射影）を測る
            //         ld dist_x = abs(v_hit.dot(pl.local_x));
            //         ld dist_y = abs(v_hit.dot(pl.local_y));
                    
            //         // ズレが幅と高さの「半分」以内に収まっていれば枠内
            //         return (dist_x <= pl.width / 2.0) && (dist_y <= pl.height / 2.0);
            //     }
                
            //     return true; // 無限平面モード
            // };

            ld valid_t = INF;
            if (is_valid_hit(t1, planes[i])) chmin(valid_t, t1);
            if (is_valid_hit(t2, planes[i])) chmin(valid_t, t2);

            // 最も早く、かつ「枠内に」衝突する平面を見つける
            if (valid_t != INF && chmin(best_t, valid_t)) {
                hit_plane_idx = i;
            }
        }

        if (hit_plane_idx == -1) break; 

        // 衝突位置まで時間を進める
        P = P + V * best_t + A * (0.5 * best_t * best_t);
        V = V + A * best_t;
        total_time += best_t;

        if (planes[hit_plane_idx].is_target_floor) {
            return {P.x, P.y, total_time};
        }

        // 床以外（バウンド盤など）に当たった場合、ベクトル方程式で反射させる
        ld v_dot_n = V.dot(planes[hit_plane_idx].n);
        
        // もし裏面から板にぶつかった場合（上に跳ね上がって板の裏に当たる等）は失敗とみなす
        if (v_dot_n > 0) return {-999, -999, -1.0}; 

        V = V - planes[hit_plane_idx].n * ((1.0 + planes[hit_plane_idx].e) * v_dot_n);
    }
    
    return {0, 0, -1.0}; 
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
    
    sort(ord.begin(), ord.end(), [&](int a, int b) {
        return target[a].dist() > target[b].dist();
    });

    Judge2025 judge;

    rep(i, 0, n) {
        ld zh = i * 20.0; 
        auto ans = calc(ord[i], ord[i] + 1, zh, target);
        
        cout << ans.tn << ',' << ans.Ir << ',' << ans.Rr << ',' << ans.nu << ',' << ans.wt;
        if (i == n - 1 && n == 21) cout << ";\n";
        else cout << "\n";
        
        judge.execute_action(ans.tn, ans.Ir, ans.Rr, ans.nu, ans.wt, zh, target[ord[i]]);
    }

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
