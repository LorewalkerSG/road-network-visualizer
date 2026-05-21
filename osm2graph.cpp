#include<cstdio>
#include<cstring>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<algorithm>
#include<cmath>
#include<ctime>
//#include "oyd.graph.h"
using namespace std;
vector<string> origin_string;
vector<string> label;


vector<string> split(const string& str, const char& c) {
    vector<string> res;
    res.clear();
    string s = "";
    //  cout << "split" << str << endl;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == c) {
            //      cout << "push_back: " << s << endl;
            if (s == "")
                ;
            else res.push_back(s);
            s = "";
        }
        else s += str[i];
    }
    if (s != "")
        res.push_back(s);
    // cout <<"res.size(): " << res.size()<< endl;
    return res;
}


class TreeNode {
public:
    string s, type;
    map<string, string> value_map;
    vector<int> ch;
    TreeNode() {
        s = "";
        ch.clear();
    }
    TreeNode(string _s) {
        s = _s;
        ch.clear();

        vector<string> a = split(s, ' ');
        type = a[0].substr(1, a[0].length() - 1);
        /*
        for (int i = 1; i < a.size(); i++){
            string[] b = a[i].Split('=');
            if (b[1][0] == '"')
                if (b[)
            value_map.insert(b[0], )
        }
         */
    }
};

vector<TreeNode> TN;
int root;
map<int, int> id_map;
vector<int> id;
vector<double> lat, lon;
vector<pair< pair< int, int >, int > > E;
int n;
char buffer[100000];


bool is_single_label(string& s) {
    if (s[s.length() - 2] == '/')
        return true;
    return false;
}

bool is_single_label(TreeNode& p) {
    return is_single_label(p.s);
}

bool is_end_label(string& s) {
    if (s[1] == '/')
        return true;
    return false;
}

void read_data(char* filename, vector<string>& s) {
    s.clear();
    ifstream in(filename);
    while (!in.eof()) {
        in.getline(buffer, 1000);
        //     cout << buffer << endl;
        s.push_back(string(buffer));
    }
    in.close();
    printf("s.size(): %d\n", s.size());
}

int analyse(vector<string>& label, int& i) {
    //  printf("HH");
    if (label[i][1] == '?')
        i++;
    //    printf("HH");
    int p = TN.size();
    //  cout << "p: " << p << " " << label[i] << endl;
    TN.push_back(TreeNode(label[i]));
    //  printf("HH");
    //  cout << p << " " << label[i] << endl;
    i++;
    //  printf("HH");
    if (is_single_label(TN[p]))
        //     cout << "single_label" << endl;
        ;
    else {
        //     cout << "is not single label" << endl;
        while (true) {
            if (is_end_label(label[i]))
                break;
            //       printf("HH");
            int k = analyse(label, i);
            //        printf("p, TN[p].ch.size(): %d %d\n", p, TN[p].ch.size());
                //    printf("HH");
            TN[p].ch.push_back(k);
            //    printf("HH");
            //    TN[p].ch.push_back(analyse(label, i));
          //      printf("HH");
        }
        //     printf("HH");
        i++;
        //     printf("HH");
    }
    //   cout << "return " << p << " " << label[i - 1] << endl;
    //   printf("HH");
    return p;
    //   printf("HH");
}

void transform_to_label(vector<string>& s, vector<string>& t) {
    printf("s.size(), t.size(): %d %d\n", s.size(), t.size());
    t.clear();
    int i = 0, j = 0;
    string a;
    while ((i < (s.size() - 1)) || (i == (s.size() - 1) && j < s[i].size())) {
        //  cout << i << " " << j << " : " << a << endl;
        if (s[i][j] == '<')
            a = "<";
        else if (s[i][j] == '>') {
            a = a + s[i][j];
            t.push_back(a);
            a.clear();
        }
        else if (a.length() > 0)
            a = a + s[i][j];

        j++;
        //    cout << s[i].length() << " " << endl;
        if (s[i].length() <= j) {
            j = 0;
            i++;
        }
    }
}

string get_value(int p, int k) {
    vector<string> a = split(TN[p].s, ' ');
    int i;
    for (i = 0; i < a[k].length(); i++)
        if (a[k][i] == '=')
            break;
    i += 2;

    string t;
    t.clear();
    for (; a[k][i] != '"'; i++)
        t = t + a[k][i];
    return t;
}

int string2int(string s) {
    int k = 0;
    for (int i = 0; i < s.length(); i++)
        k = k * 10 + s[i] - '0';
    return k;
}

// double string2double(string s){
//   //  cout << s << endl;
//     double k = 0.0;
//     int i;
//     for (i = 0; (s[i] != '.') && (i < s.length()); i++){
//         k = k * 10 + (double)(s[i] - '0');
//       //  cout << i << " " << k << " " << s[i] << endl;
//     }
//   //  cout << i << "zhenshu: " << k << endl;
//     i++;
//     double v = 1.0;
//     for (; i < s.length(); i++){
//         v *= 0.1;
//         k = k + v * (double)(s[i] - '0');
//         //cout << k << endl;
//    //     printf("%.8lf\n", k);
//    //     cout << v * (double)(s[i] - '0') << endl;
//    //     printf("%.8lf\n", v * (double)(s[i] - '0'));
//   //      cout << i << endl;
//   //      cout << s[i] << " " << s[i] - '0' << " " << v << endl;
//     }
//   //  cout << k << endl;
//   //  cout << i << endl;
//   //  while (1);
//     return k;
// }
double string2double(string s) {
    double k = 0.0;
    int i = 0;
    bool isNegative = false;

    // 检查是否为负数
    if (s[i] == '-') {
        isNegative = true;  // 记录是负数
        i++;  // 跳过负号
    }
    else if (s[i] == '+') {
        i++;  // 跳过正号
    }

    // 处理整数部分
    for (; (s[i] != '.') && (i < s.length()); i++) {
        k = k * 10 + (double)(s[i] - '0');
    }

    // 跳过小数点
    i++;

    // 处理小数部分
    double v = 1.0;
    for (; i < s.length(); i++) {
        v *= 0.1;
        k = k + v * (double)(s[i] - '0');
    }

    // 如果是负数，则将结果取反
    if (isNegative) {
        k = -k;
    }

    return k;
}
/*
int get_nd_ref(int p){
    if (TN[p].type == string("nd")){
        int k = string2int(get_value(p, 1));
        return k;
    }
    else return -1;
}
*/

void match_vertex(int p) {
    //   printf("matching p: %d\n", p);
    if (TN[p].type == string("node")) {
        int k = string2int(get_value(p, 1));

        if (id_map.find(k) == id_map.end()) {
            int v = id.size();
            id_map.insert(make_pair(k, v));
            id.push_back(k);
            //    cout << TN[p].s << endl;
            double t = string2double(get_value(p, 2));
            //   cout << t << endl;
             //  printf("%.6lf\n", t);
            lat.push_back(t);
            t = string2double(get_value(p, 3));
            //   cout << t << endl;
           //    printf("%.6lf\n", t);
            lon.push_back(t);
            //    while (1);

        }
    }
    for (int i = 0; i < TN[p].ch.size(); i++)
        match_vertex(TN[p].ch[i]);
}



double deg2rad(double deg) {
    return (deg * M_PI / 180);
}
double rad2deg(double rad) {
    return (rad * 180 / M_PI);
}
inline long long _cal(double x1, double y1, double x2, double y2) { // return (dm)
    //    printf("x1,y1,x2,y2: %.2lf %.2lf %.2lf %.2lf", x1, y1, x2, y2);
    double a1, o1, a2, o2, u, v;
    a1 = deg2rad(x1);
    o1 = deg2rad(y1);
    a2 = deg2rad(x2);
    o2 = deg2rad(y2);
    u = sin((a2 - a1) / 2);
    v = sin((o2 - o1) / 2);
    return ((long long)(2.0 * 6371.0 * asin(sqrt(u * u + cos(a1) * cos(a2) * v * v)) * 10000));
}
const double offset1 = 1.0;

int get_distance(int i, int j) {
    int k = _cal(double(lon[i]) * offset1, double(lat[i]) * offset1, double(lon[j]) * offset1, double(lat[j]) * offset1);
    //   cout << lon[i] << " " << lat[i] << " " << lon[j] << " " << lat[j] << endl;
     //  printf("%.6lf %.6lf %.6lf %.6lf \n", lon[i], lat[i], lon[j], lat[j]);
     //  printf("%d %d %d\n", i, j, k);
     //  cout << i << " " << j << " " << k << endl;
    return k;
}

void detect_way(int p) {
    //  cout << p << " " << TN[p].type << endl;
    if (TN[p].type == string("way")) {
        vector<int> a;
        a.clear();
        for (int i = 0; i < TN[p].ch.size(); i++) {
            int q = TN[p].ch[i];
            if (TN[q].type == string("nd")) {
                int k = string2int(get_value(q, 1));
                a.push_back(id_map[k]);
                if (id_map.find(k) == id_map.end()) {
                    cout << "hehe" << endl;
                    cout << k << endl;
                    while (1);
                }

            }
        }
        for (int i = 0; i < a.size() - 1; i++) {
            pair< pair<int, int>, int > tmp = make_pair(make_pair(a[i], a[i + 1]), get_distance(a[i], a[i + 1]));
            E.push_back(tmp);
        }
    }

    for (int i = 0; i < TN[p].ch.size(); i++)
        detect_way(TN[p].ch[i]);
}


// ./osm2graph map BJ.gr BJ_raw.co

int main(int argc, char* argv[]) {
    read_data(argv[1], origin_string);
    TN.clear();
    //TN.push_back(TreeNode());
    //root = 0;
    transform_to_label(origin_string, label);
    printf("lable.size(): %d\n", label.size());
    int now = 0;
    root = analyse(label, now);

    n = 0;
    id_map.clear();
    id.clear();
    lat.clear();
    lon.clear();
    E.clear();

    match_vertex(root);
    detect_way(root);
    cout << E.size() << endl;
    for (int i = 0; i < 10; i++)
        cout << E[i].first.first << " " << E[i].first.second << " " << E[i].second << endl;
    FILE* fout, * fco;
    fout = fopen(argv[2], "w");
    fprintf(fout, "%d %d\n", id_map.size(), E.size());
    for (int i = 0; i < E.size(); i++)
        fprintf(fout, "%d %d %d\n", E[i].first.first, E[i].first.second, E[i].second);
    fclose(fout);
    fco = fopen(argv[3], "w");
    fprintf(fco, "%d\n", id_map.size());
    for (int i = 0; i < id_map.size(); i++)
        fprintf(fco, "%d %d\n", (int)(lat[i] * 1e6), (int)(lon[i] * 1e6));
    fclose(fco);
    return 0;
}
