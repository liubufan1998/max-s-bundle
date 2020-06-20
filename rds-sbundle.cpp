#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int TIME_LIM = 3600;

int N, M, K, wG;
pair <int, int> *E;

void read_clq() {
    char str[200], stmp[10];
    while (true) {
        fgets(str, 200, stdin);
        if (*str == 'p')
            break;
    }
    sscanf(str, "%s%s%d%d", stmp, stmp, &N, &M);
    E = new pair <int, int> [M];
    static set < pair <int, int> > st;
    int m = 0;
    for (int i = 0; i < M; ++i) {
        int u, v;
        scanf("%s%d%d", stmp, &u, &v);
        if (u == v)
            continue;
        --u; --v;
        assert(0 <= u && u < N);
        assert(0 <= v && v < N);
        if (u > v)
            swap(u, v);
        if (st.count(make_pair(u, v)))
            continue;
        st.insert(make_pair(u, v));
        assert(m < M);
        E[m++] = make_pair(u, v);
    }
    M = m;
    st.clear();
}

void read_graph() {
    string str;
    getline(cin, str);
    stringstream fs;
    fs << str;
    fs >> N >> M;//M is not a reliable number of the edges
    vector<pair<int, int> > vedges;
    static set <int> st;
    //int m = 0;
    for (int i = 0; i < N; ++i) {
        getline(cin, str);
        stringstream ss;
        ss << str;
        int x;
        while (ss >> x) {
            --x;
            assert(0 <= x && x < N);
            if (x >= i || st.count(x)) 
                continue;
            st.insert(x);            
			//cerr << m << M <<endl;
			//assert(m < M);
            //E[m++] = make_pair(i, x);
			//m++;
			vedges.push_back(make_pair(i, x));
        }
        st.clear();
    }
	if (vedges.size() != M){
		fprintf(stderr, "Given edege number %d, Actual edge number %d\n", M, vedges.size());
	}
	M = vedges.size();
	E = new pair <int, int> [M];
	copy(vedges.begin(), vedges.end(), E);
}

map <int, int> mp;

inline int find_id(int u) {
    auto it = mp.find(u);
    if (it != mp.end())
        return it->second;
    int sz = mp.size();
    return mp[u] = sz;
}

void read_txt() {
    char str[200];
    fgets(str,200,stdin); fgets(str,200,stdin);
    scanf("%s%s%d%s%d", str, str, &N, str, &M);
    fgets(str,200,stdin); fgets(str,200,stdin);
    E = new pair <int, int> [M];
    int m = 0;
    static set < pair <int, int> > st;
    for (int i = 0; i < M; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        if (u == v)
            continue;
        u = find_id(u);
        v = find_id(v);
        if (u > v)
            swap(u, v);
        if (st.count(make_pair(u,v)))
            continue;
        st.insert(make_pair(u, v));
        assert(m < M);
        E[m++] = make_pair(u, v);
    }
    M = m;
    st.clear();
    mp.clear();
}

int *head, *nxt, *to, *cap_up, *cap, etot;
int *degree, *que, *nV, n;
bool *outcore;
int *up_bound, *ordID;
bool **adjN;
int *low_bound, *notadj;
int *pre, *vis, tag = 0;

inline void addedge(int *h, int v) {
    nxt[etot] = *h;
    to[etot] = v;
    *h = etot++;
}

bool PreProcess(int S) {
    degree = new int [N];
    head = new int [N];
    que = new int [N];
    nV = new int [N];
    outcore = new bool [N];
    nxt = new int [M + M];
    to = new int [M + M];
    memset(degree, 0, N * sizeof(int));
    memset(head, -1, N * sizeof(int));
    etot = 0;
    for (int i = 0; i < M; ++i) {
        int u = E[i].first, v = E[i].second;
        ++degree[u]; ++degree[v];
        addedge(head + u, v);
        addedge(head + v, u);
    }
    memset(outcore, 0, N * sizeof(bool));
    int fr = 0, re = 0;
    for (int i = 0; i < N; ++i)
        if (degree[i] < S - K)
            que[re++] = i, outcore[i] = true;
    while (fr ^ re) {
        int u = que[fr++];
        for (int e = head[u]; ~e; e = nxt[e]) {
            int v = to[e];
            if (outcore[v])
                continue;
            if (--degree[v] < S - K)
                que[re++] = v, outcore[v] = true;
        }
    }
    n = 0;
    for (int i = 0; i < N; ++i)
        if (!outcore[i])
            nV[i] = n++;
    if (n < S)
        return false;
    int m = 0;
    for (int i = 0; i < M; ++i) {
        int u = E[i].first, v = E[i].second;
        if (outcore[u] || outcore[v])
            continue;
        E[m++] = make_pair(nV[u], nV[v]);
    }
    M = m;
    delete [] nxt;
    delete [] to;
    delete [] head;
    delete [] que;
    head = new int [n + n];
    que = new int [n + n];
    pre = new int [n + n];
    vis = new int [n + n];
    nxt = new int [m * 4 + 2 * n];
    to = new int [m * 4 + 2 * n];
    cap_up = new int [m * 4 + 2 * n];
    cap = new int [m * 4 + 2 * n];
    up_bound = new int [n];
    ordID = new int [n];
    low_bound = new int [n];
    notadj = new int [n];
    memset(notadj, 0, n * sizeof(int));
    memset(vis, 0, n * sizeof(int));
    adjN = new bool* [n];
    for (int i = 0; i < n; ++i) {
        adjN[i] = new bool [n];
        memset(adjN[i], 0, n * sizeof(bool));
    }
    return true;
}

void getOrd() {
    memset(degree, 0, n * sizeof(int));
    memset(head, -1, n * sizeof(int));
    etot = 0;
    for (int i = 0; i < M; ++i) {
        int u = E[i].first, v = E[i].second;
        ++degree[u]; ++degree[v];
        addedge(head + u, v);
        addedge(head + v, u);
    }
    static priority_queue < pair <int, int> > Q;
    memset(outcore, 0, n * sizeof(bool));
    for (int i = 0; i < n; ++i)
        Q.push(make_pair(-degree[i], i));
    int curB = 1;
    while (!Q.empty()) {
        int u = Q.top().second; Q.pop();
        if (outcore[u])
            continue;
        outcore[u] = true;
        if (degree[u] < curB - K) {
            up_bound[u] = curB;
        } else {
            curB = degree[u] + K + 1;
            up_bound[u] = curB;
        }
        for (int e = head[u]; ~e; e = nxt[e]) {
            int v = to[e];
            if (outcore[v])
                continue;
            --degree[v];
            Q.push(make_pair(-degree[v], v));
        }
    }
    for (int i = 0; i < n; ++i)
        ordID[i] = i;
    sort(ordID, ordID + n, [](const int &x, const int &y) {
         if (up_bound[x] != up_bound[y])
            return up_bound[x] < up_bound[y];
         return x < y; } );
    for (int i = 0; i < n; ++i)
        nV[ordID[i]] = i;
    for (int i = 0; i < M; ++i) {
        int u = E[i].first, v = E[i].second;
        u = nV[u]; v = nV[v];
        adjN[u][v] = adjN[v][u] = true;
    }
    memset(head, -1, (n + n) * sizeof(int));
    etot = 0;
}

clock_t startTime;
long long dfs_node;
char filename[100];
int LB;

void exit_program() {
    printf("Filename: %s\n", filename);
    printf("Value of K: %d\n", K);
    printf("Search nodes: %lld\n", dfs_node);
    printf("Lower bound: %d\n", LB);
    printf("Used Time: %.10f s\n", (double)(clock() - startTime) / CLOCKS_PER_SEC);
    exit(0);
}

void check_timeout() {
    if ((double)(clock() - startTime) / CLOCKS_PER_SEC > TIME_LIM)
        exit_program();
}

inline void add_flow_edge(int u, int v) {
    cap_up[etot] = 1;
    to[etot] = v;
    nxt[etot] = head[u];
    head[u] = etot++;
    cap_up[etot] = 0;
    to[etot] = u;
    nxt[etot] = head[v];
    head[v] = etot++;
}

vector <int> P, sat;

void add_to_P(int u, vector <int> &C) {
    sat.clear();
    add_flow_edge(u, u + n);
    for (auto x : P)
        if (adjN[u][x])
            add_flow_edge(u + n, x), add_flow_edge(x + n, u);
        else {
            ++notadj[x];
            if (notadj[x] == K - 1)
                sat.push_back(x);
        }
    if (notadj[u] == K - 1)
        sat.push_back(u);
    for (auto x : C)
        if (!adjN[u][x])
            ++notadj[x];
    P.push_back(u);
}

void del_fr_P(int u, vector <int> &C) {
    P.pop_back();
    head[u] = head[u + n] = -1;
    for (auto x : P)
        if (adjN[u][x])
            head[x] = nxt[head[x]], head[n + x] = nxt[head[n + x]];
        else
            --notadj[x];
    for (auto x : C)
        if (!adjN[u][x])
            --notadj[x];
}

bool aug(int s, int t) {
    int fr = 0, re = 0;
    que[re++] = s;
    vis[s] = ++tag;
    while (fr ^ re) {
        int u = que[fr++];
        for (int e = head[u]; ~e; e = nxt[e]) {
            if (cap[e] == 0)
                continue;
            int v = to[e];
            if (vis[v] == tag)
                continue;
            pre[v] = e;
            vis[v] = tag;
            que[re++] = v;
            if (v == t)
                break;
        }
    }
    if (vis[t] != tag)
        return false;
    for (int i = t; i != s; i = to[pre[i] ^ 1])
        --cap[pre[i]], ++cap[pre[i] ^ 1];
    return true;
}

bool mx_flow(int s, int t, int bound) {
    bool ret = true;
    for (int i = 0; i < etot; ++i)
        cap[i] = cap_up[i];
    for (int i = 0; ret && i < bound; ++i)
        if (!aug(s, t))
            ret = false;
    return ret;
}

bool can_add(int u) {
    if (P.size() + 1 - K < 1)
        return true;
    if (notadj[u] >= K)
        return false;
    for (auto x : sat)
        if (!adjN[u][x])
            return false;
    int cure = etot;
    add_flow_edge(u, u + n);
    for (auto x : P)
        if (adjN[u][x])
            add_flow_edge(u + n, x), add_flow_edge(x + n, u);
    bool ret = true;
    for (auto x : P) {
        if (adjN[u][x])
            continue;
        if (!mx_flow(u + n, x, P.size() + 1 - K)) {
            ret = false;
            break;
        }
    }
    head[u] = head[u + n] = -1;
    for (auto x : P)
        if (adjN[u][x])
            head[x] = nxt[head[x]], head[n + x] = nxt[head[n + x]];
    etot = cure;
    return ret;
}

void FindMax(vector <int> C) {
    check_timeout();
    ++dfs_node;
    if (C.empty()) {
        if ((int)P.size() > LB)
            LB = P.size();
        return;
    }
    while (!C.empty()) {
        if (C.size() + P.size() <= LB)
            return;
        int u = C.back();
        if (low_bound[u] + P.size() <= LB)
            return;
        C.pop_back();
        int cure = etot;
        add_to_P(u, C);
        vector <int> nC;
        for (auto x : C)
            if (can_add(x))
                nC.push_back(x);
        FindMax(nC);
        del_fr_P(u, C);
        etot = cure;
    }
}

void work() {
    startTime = clock();
    dfs_node = 0;
    LB = max(wG, K);
    if (!PreProcess(LB + 1))
        exit_program();
    getOrd();
    LB = 0;
    for (int i = 0; i < n; ++i) {
        if (i + 1 <= K) {
            low_bound[i] = LB = i + 1;
        } else {
            vector <int> C, nC;
            for (int j = 0; j < i; ++j)
                C.push_back(j);
            add_to_P(i, C);
            for (auto x : C)
                if (can_add(x))
                    nC.push_back(x);
            FindMax(nC);
            del_fr_P(i, C);
            etot = 0;
            low_bound[i] = LB;
        }
    }
    exit_program();
}

void MAIN() {
    int p = strlen(filename) - 1;
    while (filename[p] != '.')
        --p;
    ++p;
    if (strcmp(filename + p, "clq") == 0)
        read_clq();
    else if (strcmp(filename + p, "graph") == 0)
        read_graph();
    else
        read_txt();
    work();
}

int main(int argc, char **argv) {
    assert(argc == 4);
    strcpy(filename, argv[1]);
    if (freopen(filename, "r", stdin) == NULL) {
        printf("Open %s error.\n", filename);
        return 0;
    }
    sscanf(argv[2], "%d", &K);
    sscanf(argv[3], "%d", &wG);
    MAIN();
    return 0;
}
