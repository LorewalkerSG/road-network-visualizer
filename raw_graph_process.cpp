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
int n, m;
vector<vector<pair<int, int > > > E;
int* f, * g, * h;

int find(int x) {
    if (f[x] == x)
        return x;
    f[x] = find(f[x]);
    return f[x];
}
const double eps = 1e-8;
// ./raw_graph_process BJ.gr BJ-d.gr BJ_raw.co BJ.co BJ.bound
int main(int argc, char* argv[]) {
    ifstream ifs;
    ifs.open(argv[5]);
    double lonb[2], latb[2];
    ifs >> latb[1] >> lonb[0] >> lonb[1] >> latb[0];
    latb[1] = (int)(latb[1] * 1000000);
    latb[0] = (int)(latb[0] * 1000000);
    lonb[0] = (int)(lonb[0] * 1000000);
    lonb[1] = (int)(lonb[1] * 1000000);
    printf("bound:la %f %f lo %f %f\n", latb[0], latb[1], lonb[0], lonb[1]);
    FILE* fin;
    fin = fopen(argv[1], "r");
    fscanf(fin, "%d %d", &n, &m);
    E.clear();
    vector<pair<int, int> > tmp;
    tmp.clear();
    for (int i = 0; i < n; i++)
        E.push_back(tmp);

    f = (int*)malloc(sizeof(int) * (n + 1));
    g = (int*)malloc(sizeof(int) * (n + 1));
    h = (int*)malloc(sizeof(int) * (n + 1));
    bool* os;
    os = (bool*)malloc(sizeof(bool) * (n + 1));
    for (int i = 0; i <= n; i++) {
        os[i] = false;
        f[i] = i;
        g[i] = 0;
    }


    //-----------------
    FILE* fco = fopen(argv[3], "r");
    vector<int> co1, co2;
    co1.clear();
    co2.clear();
    int tt;
    fscanf(fco, "%d", &tt);
    for (int i = 0; i < n; i++) {
        int x, y;
        fscanf(fco, "%d %d", &x, &y);
        co1.push_back(x); // lat
        co2.push_back(y); // lon
        if (co1[i] < latb[0] - eps || co1[i] > latb[1] + eps)
            os[i] = true;
        if (co2[i] < lonb[0] - eps || co2[i] > lonb[1] + eps)
            os[i] = true;
        // if (co2[i] == 115440397) {
        //     if (os[i])
        //         cout << true << endl;
        //     else cout << false << endl;
        //     //     while (1);
        // }

    }
    fclose(fco);
    //---------------------

    // int cnt = 0;
    for (int i = 0; i < m; i++) {
        int x, y, z;
        fscanf(fin, "%d %d %d", &x, &y, &z);
        E[x].push_back(make_pair(y, z));
        E[y].push_back(make_pair(x, z));
        // int cnt = 0;
        if (os[x] || os[y]) {
            // printf("passed %d %d cnt %d\n", x, y, cnt++);
            continue;
        }
        if (find(x) != find(y))
            f[find(x)] = y;
    }
    for (int i = 0; i < n; i++)
        g[find(i)]++;
    int k = 0;
    for (int i = 0; i < n; i++)
        if (g[i] > g[k])
            k = i;
    cout << g[k] << endl;


    vector<int> a;
    a.clear();
    for (int i = 0; i < n; i++) {
        if ((find(i) == k) && (os[i] == false)) {
            h[i] = a.size();
            a.push_back(i);
            //    cout << i << " " << a[i] << " " << endl;
            if (co2[a[a.size() - 1]] == 115440397)
                while (1);
        }
    }
    cout << a.size() << endl;




    vector<pair<pair<int, int>, int> > b;
    b.clear();
    for (int i = 0; i < n; i++)
        if ((find(i) == k) && (os[i] == false))
            for (int j = 0; j < E[i].size(); j++)
                if (find(E[i][j].first) == k) {
                    if (E[i][j].first <= i)
                        continue;
                    if (os[E[i][j].first])
                        continue;
                    b.push_back(make_pair(make_pair(h[i], h[E[i][j].first]), E[i][j].second));
                    b.push_back(make_pair(make_pair(h[E[i][j].first], h[i]), E[i][j].second));
                }




    FILE* fout = fopen(argv[2], "w");
    fprintf(fout, "%d %d\n", g[k], b.size());
    for (int i = 0; i < b.size(); i++)
        fprintf(fout, "%d %d %d\n", b[i].first.first + 1, b[i].first.second + 1, b[i].second);
    fclose(fout);

    FILE* fco_out;
    fco_out = fopen(argv[4], "w");
    fprintf(fco_out, "%d\n", a.size());
    for (int i = 0; i < a.size(); i++) {
        fprintf(fco_out, "%d %d %d\n", i + 1, co2[a[i]], co1[a[i]]);
    }
    fclose(fco_out);
    return 0;
}
