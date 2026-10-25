#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ifstream file("input.txt");
    ofstream of("output.txt");
    if (!file.is_open()) {
        cerr << "エラー: input.txt が見つかりません。" << endl;
        return 1;
    }
    string s;
    while (getline(file, s)) {
        stringstream ss(s);
        int x, y, tmp, z;
        bool is_first = true;
		ss >> tmp;
		(void)tmp;
        of << "{";
        while (ss >> x >> y >> z) {
            if (!is_first) {
                of << ", ";
            }
            of << "{" << x << ", " << y << ", " << z << "}";
            is_first = false;
        }
        
        of << "},\n";
    }

    file.close();
    of.close();
    
    cout << "変換が完了し、output.txtに出力されました。" << "\n";
    return 0;
}
