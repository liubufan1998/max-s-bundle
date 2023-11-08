#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int TIME_LIM = 3600;

int N, M, K, wG;
pair <int, int> *E;

//gets() is deprecated  
#define gets(str) (fgets(str,200,stdin))

void read_clq() { // 我没有想到这些代码里面居然还包括了去除重复边！
    char str[200], stmp[10];
    while (true) {
        gets(str);
        if (*str == 'p')
            break;
    }
    sscanf(str, "%s%s%d%d", stmp, stmp, &N, &M);
    E = new pair <int, int> [M];
    static set < pair <int, int> > st; // 使用一个 set 数据结构 st 来记录已经添加过的边，以防止重复添加相同的边。
    int m = 0;
    for (int i = 0; i < M; ++i) {
        int u, v;
        scanf("%s%d%d", stmp, &u, &v);
        if (u == v) // 如果 u 和 v 相等，表示自环，直接跳过。
            continue;
        --u; --v;
        assert(0 <= u && u < N);
        assert(0 <= v && v < N);
        if (u > v) // 如果 u 大于 v，将它们交换，以确保每个边的起始顶点编号小于结束顶点编号
            swap(u, v);
        if (st.count(make_pair(u, v)))
            continue;
        st.insert(make_pair(u, v));
        assert(m < M);
        E[m++] = make_pair(u, v);
    }
    M = m; // 最后，将 M 更新为实际有效的边数目 m，并清空 set。
    st.clear();
}

void read_graph() {
    string str;
    getline(cin, str);
    stringstream fs;
    fs << str;
    fs >> N >> M;//M is not a reliable number of the edges 
    // 从 fs 中提取两个整数 N 和 M，分别表示图的顶点数和边数。注意，M 这里被提取，但后面的代码会进行修正，因为它可能不是准确的边数。
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
            if (x >= i || st.count(x)) // 如果 x 大于或等于当前顶点的索引 i，或者在 st 集合中已经存在，就跳过这个邻接顶点，以避免处理重复的边。
                continue;
            st.insert(x);            
			//cerr << m << M <<endl;
			//assert(m < M);
            //E[m++] = make_pair(i, x);
			//m++;
			vedges.push_back(make_pair(i, x)); // 如果通过上述条件检查，该邻接顶点是有效的且未处理的边，则将其添加到 vedges 向量中，并将 (i, x) 形式的边信息存储在其中。
        }
        st.clear();
    }
	if (vedges.size() != M){
        // 检查 vedges 中的边的数量是否等于初始提供的 M 值，如果不相等，则输出一条错误信息，指出提供的边数与实际边数不一致。
		fprintf(stderr, "Given edege number %d, Actual edge number %d\n", M, vedges.size());
	}
	M = vedges.size();
	E = new pair <int, int> [M];
	copy(vedges.begin(), vedges.end(), E);
}

map <int, int> mp;

inline int find_id(int u) {
    // 这段代码的作用是将输入的顶点标识 u 映射到内部的标识，并返回映射后的标识值。这有助于管理和重排顶点标识，以便在图算法中使用。
    auto it = mp.find(u);
    if (it != mp.end())
        return it->second;
    int sz = mp.size();
    return mp[u] = sz;
}

void read_txt() {
    // 这段代码的作用是从文本文件中读取图的顶点数和边数，然后解析每条边的信息，并将有效的边存储在 E 数组中。
    // 同时，它会跳过自环边和重复的边。如果存在重新编号的操作，也会将顶点重新编号为连续的整数。
    char str[200];
    gets(str); gets(str);
    scanf("%s%s%d%s%d", str, str, &N, str, &M);
    gets(str); gets(str);
    E = new pair <int, int> [M];
    int m = 0;
    static set < pair <int, int> > st;
    for (int i = 0; i < M; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        if (u == v) // 检查 u 和 v 是否相等，如果相等，说明存在自环，跳过这条边。
            continue;
        u = find_id(u);
        v = find_id(v);
        if (u > v)
            swap(u, v);
        if (st.count(make_pair(u,v))) // 检查 (u, v) 和 (v, u) 是否已经存在于集合 st 中，如果存在，说明这条边已经添加过了，跳过。
            continue;
        st.insert(make_pair(u, v));
        assert(m < M);
        E[m++] = make_pair(u, v);
    }
    M = m;
    st.clear();
    mp.clear();
}

int twoPow[1 << 16], msk = (1 << 16) - 1;

struct BIT_SET {
    int n, m;
    unsigned *buf;
    BIT_SET() {
        n = m = 0;
        buf = nullptr;
    }
    ~BIT_SET() {
        if (buf != nullptr)
            delete [] buf;
    }
    void init(int _n) {
        m = _n & 31;
        n = _n >> 5;
        buf = new unsigned [n + 1];
        for (int i = 0; i <= n; ++i)
            buf[i] = 0;
    }
    void flip() {
        for (int i = 0; i < n; ++i)
            buf[i] = ~buf[i];
        buf[n] ^= ((unsigned)1 << m) - 1;
    }
    void set(int x) {
        buf[x >> 5] ^= (unsigned)1 << (x & 31);
    }
    bool test(int x) {
        return buf[x >> 5] >> (x & 31) & 1;
    }
    int lb(unsigned x) {
        if (x & msk)
            return twoPow[x & msk];
        return twoPow[x >> 16 & msk] + 16;
    }
    int lowbit() {
        for (int i = 0; i <= n; ++i) {
            unsigned x = buf[i] & ((~buf[i]) + 1);
            if (x)
                return lb(x) + (i << 5);
        }
        return -1;
    }
    bool empty() {
        for (int i = 0; i <= n; ++i)
            if (buf[i])
                return false;
        return true;
    }
    void operator &=(const BIT_SET &rhs) {
        for (int i = 0; i <= n; ++i)
            this->buf[i] &= rhs.buf[i];
    }
    void cp(BIT_SET &rhs) const {
        rhs.n = n;
        rhs.m = m;
        if (rhs.buf == NULL)
            rhs.buf = new unsigned [n + 1];
        for (int i = 0; i <= n; ++i)
            rhs.buf[i] = buf[i];
    }
} *adjN, *invN, inD, inG;

int *head, *nxt, *to, etot;
int *degree, *que, *nV, n;
bool *outcore;
bool *del, *ins;
int *notadj;
int *up_bound, *ordID;
int *dis;

const int INF = 0x3f3f3f3f;
class MaximumFlow {
private:
    struct Node {
        int from, to, next;
        int cap;
    } *edge;
    int *cap_backup;
    int tol;
    int *head;
    int *dep;
    int *gap, *que;
    int *cur;
    int *S;
    int n;
    void BFS(int start, int end) {
        memset(dep, -1, n * sizeof(int));
        memset(gap, 0, n * sizeof(int));
        gap[0] = 1;
        int front, rear;
        front = rear = 0;
        dep[end] = 0;
        que[rear++] = end;
        while (front != rear) {
            int u = que[front++];
            for (int i = head[u]; i != -1; i = edge[i].next) {
                int v = edge[i].to;
                if (dep[v] != -1)
                    continue;
                que[rear++] = v;
                dep[v] = dep[u] + 1;
                ++gap[dep[v]];
            }
        }
    }
public:
    void reserve(int n, int m) {
        n += 3;
        edge = new Node [m * 4 + n * 2];
        cap_backup = new int [m * 4 + n * 2];
        head = new int [n + n];
        dep = new int [n + n];
        gap = new int [n + n];
        que = new int [n + n];
        cur = new int [n + n];
        S = new int [n + n];
    }
    void init(int _n) {
        tol = 0;
        n = _n + 2;
        memset(head, -1, n * sizeof(int));
    }
    void addedge(int u,int v,int w) {
        edge[tol].from = u;
        edge[tol].to = v;
        edge[tol].cap = w;
        cap_backup[tol] = w;
        edge[tol].next = head[u];
        head[u]= tol++;
        edge[tol].from = v;
        edge[tol].to = u;
        edge[tol].cap = 0;
        cap_backup[tol] = 0;
        edge[tol].next = head[v];
        head[v]= tol++;
    }
    int SAP(int start, int end) {
        for (int i = 0; i < tol; ++i)
            edge[i].cap = cap_backup[i];
        int res = 0;
        BFS(start, end);
        int top = 0;
        memcpy(cur, head, n * sizeof(int));
        int u = start;
        int i;
        while (dep[start] < n) {
            if (u == end) {
                int temp = INF + 1;
                int inser = 0;
                for (i = 0; i < top; ++i)
                    if (temp > edge[S[i]].cap) {
                        temp = edge[S[i]].cap;
                        inser = i;
                    }
                for (i = 0; i < top; ++i) {
                    edge[S[i]].cap -= temp;
                    edge[S[i] ^ 1].cap += temp;
                }
                res += temp;
                top = inser;
                u = edge[S[top]].from;
            }
            if (u != end && gap[dep[u] - 1] == 0)
                break;
            for (i = cur[u]; i != -1; i = edge[i].next)
                if (edge[i].cap != 0 && dep[u] == dep[edge[i].to] + 1)
                    break;
            if (i != -1) {
                cur[u] = i;
                S[top++] = i;
                u = edge[i].to;
            } else {
                int min = n;
                for (i = head[u]; i != -1; i = edge[i].next) {
                    if (edge[i].cap == 0)
                        continue;
                    if (min > dep[edge[i].to]) {
                        min = dep[edge[i].to];
                        cur[u] = i;
                    }
                }
                --gap[dep[u]];
                dep[u] = min + 1;
                ++gap[dep[u]];
                if (u != start)
                    u = edge[S[--top]].from;
            }
        }
        return res;
    }
} mf;

vector <int> svex;
class VertexConnectivity {
private:
    int *nV, *oID;
public:
    void reserve(int n) {
        n += 3;
        nV = new int [n];
        oID = new int [n];
    }
    bool solve(bool cals, int bound) {
        int nn = 0;
        if (cals) {
            for (auto u : svex)
                nV[u] = nn, oID[nn++] = u;
            mf.init(nn + nn);
            for (auto u : svex)
                mf.addedge(nV[u], nV[u] + nn, 1);
            for (auto u : svex) {
                for (auto v : svex) {
                    if (!adjN[u].test(v))
                        continue;
                    mf.addedge(nV[u] + nn, nV[v], INF);
                }
            }
            int u = nV[svex.back()];
            for (int i = 0; i < nn; ++i) {
                if (i == u || adjN[oID[u]].test(oID[i]))
                    continue;
                if (mf.SAP(u + nn, i) < bound)
                    return false;
            }
            return true;
        } else {
            for (int i = 0; i < n; ++i)
                if (!del[i])
                    nV[i] = nn, oID[nn++] = i;
            mf.init(nn + nn);
            for (int i = 0; i < n; ++i)
                if (!del[i])
                    mf.addedge(nV[i], nV[i] + nn, 1);
            for (int i = 0; i < n; ++i) {
                if (del[i])
                    continue;
                for (int e = head[i]; ~e; e = nxt[e]) {
                    int v = to[e];
                    if (del[v])
                        continue;
                    mf.addedge(nV[i] + nn, nV[v], INF);
                }
            }
            for (int i = 0; i < nn; ++i)
                for (int j = i + 1; j < nn; ++j)
                    if (!adjN[oID[i]].test(oID[j]) && mf.SAP(nn + i, j) < bound)
                        return false;
            return true;
        }
    }
} vc;

inline void addedge(int *h, int v) {
    nxt[etot] = *h;
    to[etot] = v;
    *h = etot++;
}

bool PreProcess(int S) {
    nxt = new int [M + M];
    to = new int [M + M];
    degree = new int [N];
    head = new int [N];
    outcore = new bool [N];
    que = new int [N];
    nV = new int [N];
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
    del = new bool [n];
    memset(del, 0, n * sizeof(bool));
    ins = new bool [n];
    memset(ins, 0, n * sizeof(bool));
    notadj = new int [n];
    memset(notadj, 0, n * sizeof(int));
    up_bound = new int [n];
    ordID = new int [n];
    dis = new int [n];
    mf.reserve(n, m);
    vc.reserve(n);
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
    memset(degree, 0, n * sizeof(int));
    memset(head, -1, n * sizeof(int));
    etot = 0;
    adjN = new BIT_SET [n];
    invN = new BIT_SET [n];
    for (int i = 0; i < n; ++i)
        adjN[i].init(n);
    for (int i = 0; i < M; ++i) {
        int u = E[i].first, v = E[i].second;
        u = nV[u]; v = nV[v];
        ++degree[u]; ++degree[v];
        addedge(head + u, v);
        addedge(head + v, u);
        adjN[u].set(v);
        adjN[v].set(u);
    }
    for (int i = 0; i < n; ++i) {
        adjN[i].cp(invN[i]);
        invN[i].flip();
    }
    inD.init(n); inD.flip();
    inG.init(n); inG.flip();
    svex.clear();
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

inline void delfrD(int u) {
    inD.set(u);
    inG.set(u);
    del[u] = true;
    for (int e = head[u]; ~e; e = nxt[e])
        --degree[to[e]];
}

inline void addtoD(int u) {
    inD.set(u);
    inG.set(u);
    del[u] = false;
    for (int e = head[u]; ~e; e = nxt[e])
        ++degree[to[e]];
}

inline void delfrS(int u) {
    inD.set(u);
    ins[u] = false;
    svex.pop_back();
    for (int i = 0; i < n; ++i) {
        if (i != u && !adjN[u].test(i))
            --notadj[i];
    }
}

inline void addtoS(int u) {
    inD.set(u);
    ins[u] = true;
    svex.push_back(u);
    for (int i = 0; i < n; ++i) {
        if (i != u && !adjN[u].test(i))
            ++notadj[i];
    }
}

inline bool canadd(int u) {
    int tot = 0;
    for (auto v : svex) if (!adjN[u].test(v)) {
        if (++tot >= K)
            return false;
        if (notadj[v] >= K - 1)
            return false;
    }
    svex.push_back(u);
    bool ret = vc.solve(true, svex.size() - K);
    svex.pop_back();
    return ret;
}

void bfs(int s) {
    memset(dis, -1, n * sizeof(int));
    dis[s] = 0;
    int fr = 0, re = 0;
    que[re ++] = s;
    while(fr ^ re) {
        int u = que[fr ++];
        for(int e = head[u]; ~e; e = nxt[e]) {
            int v = to[e];
            if(del[v] || dis[v] != -1)
                continue;
            dis[v] = dis[u] + 1;
            que[re ++] = v;
        }
    }
}

void check_timeout() {
    if ((double)(clock() - startTime) / CLOCKS_PER_SEC > TIME_LIM)
        exit_program();
}

int get_color() {
    BIT_SET Q, P;
    inG.cp(P);
    int tot = 0;
    while (!P.empty()) {
        int cnt = 0;
        P.cp(Q);
        do {
            int u = Q.lowbit();
            ++cnt;
            if (cnt > K)
                cnt = K;
            P.set(u); Q.set(u);
            Q &= invN[u];
        } while (!Q.empty());
        tot += cnt;
    }
    return tot;
}

void get_color(vector < pair <int, int> > &lst) {
    BIT_SET Q, P;
    inD.cp(P);
    int tot = 0;
    while (!P.empty()) {
        int cnt = 0;
        P.cp(Q);
        do {
            int u = Q.lowbit();
            ++cnt;
            if (cnt > K)
                cnt = K;
            lst.emplace_back(make_pair(u, tot + cnt));
            P.set(u); Q.set(u);
            Q &= invN[u];
        } while (!Q.empty());
        tot += cnt;
    }
}

void dfs_kbundle(int curS, bool check) {
    check_timeout();
    ++dfs_node;
    LB = max(LB, (int)svex.size());
    if (curS <= LB)
        return;
    if (check && vc.solve(false, curS - K)) {
        LB = curS;
        return;
    }
    vector < pair <int, int> > lst;
    get_color(lst);
    int m = lst.size();
    if (m == 0)
        return;
    check = false;
    for (int i = m - 1; i >= 0; --i) {
        if ((int)svex.size() + lst[i].second <= LB) {
            for (int j = i + 1; j < m; ++j)
                addtoD(lst[j].first);
            return;
        }
        if (canadd(lst[i].first)) {
            addtoS(lst[i].first);
            dfs_kbundle(curS, check);
            delfrS(lst[i].first);
        }
        --curS;
        check = true;
        delfrD(lst[i].first);
    }
    for (int i = 0; i < m; ++i)
        addtoD(lst[i].first);
}

void dfs(int curS) {
    check_timeout();
    ++dfs_node;
    LB = max(LB, (int)svex.size());
    if (curS <= LB)
        return;
    int minID = -1;
    for(int i = 0; i < n; ++ i) if(!del[i]) {
        if(minID == -1 || degree[i] < degree[minID])
            minID = i;
    }
    if(degree[minID] >= curS - K) {
        dfs_kbundle(curS, true);
        return;
    }
    if(degree[minID] < LB + 1 - K) {
        if(ins[minID]) return;
        delfrD(minID);
        dfs(curS - 1);
        addtoD(minID);
        return;
    }
    int maxID = -1;
    for(int i = 0; i < n; ++ i) if(!del[i]) {
        if(maxID == -1 || notadj[i] > notadj[maxID])
            maxID = i;
    }
    if(!ins[maxID] && notadj[maxID] >= K) {
        delfrD(maxID);
        dfs(curS - 1);
        addtoD(maxID);
        return;
    }
    if(ins[maxID] && notadj[maxID] >= K - 1) {
        if(notadj[maxID] >= K) return;
        vector <int> todel;
        for(int i = 0; i < n; ++ i) if(!del[i] && !ins[i] && !adjN[maxID].test(i)) {
            todel.push_back(i);
        }
        if(todel.size()) {
            for(auto x : todel) delfrD(x);
            dfs(curS - todel.size());
            for(auto x : todel) addtoD(x);
            return;
        }
    }
    if (svex.size()) {
        vector <int> sofar;
        int u = svex[rand() % svex.size()];
        bfs(u);
        for(int v = 0; v < n; ++ v) if(!del[v]) {
            if(dis[v] == -1 || dis[v] > max(2, K + K - LB)) {
                if(ins[v]) return;
                sofar.push_back(v);
            }
        }
        if(sofar.size()) {
            for(auto x : sofar) delfrD(x);
            dfs(curS - sofar.size());
            for(auto x : sofar) addtoD(x);
            return;
        }
    }
    if (get_color() <= LB)
        return;
    vector <int> branch;
    for(int x = 0; x < n; ++ x) if(!del[x] && x != minID && !ins[x]) {
        if(!adjN[minID].test(x))
            branch.push_back(x);
    }
    if(ins[minID]) {
        int canselect = K - 1 - notadj[minID], pos = -1;
        bool all = true;
        for(int i = 0; i < canselect; ++ i) {
            delfrD(branch[i]);
            if(i && !canadd(branch[i - 1])) {
                addtoD(branch[i]);
                all = false;
                break;
            }
            if(i) {
                addtoS(branch[i - 1]);
                pos = i - 1;
            }
            dfs(curS - 1);
            addtoD(branch[i]);
        }
        if (all && (canselect == 0 || canadd(branch[canselect - 1]))) {
            for(int i = canselect; i < (int)branch.size(); ++ i)
                delfrD(branch[i]);
            if(canselect) addtoS(branch[canselect - 1]);
            dfs(curS - branch.size() + canselect);
            if(canselect) delfrS(branch[canselect - 1]);
            for(int i = canselect; i < (int)branch.size(); ++ i)
                addtoD(branch[i]);
        }
        for(int i = 0; i <= pos; ++ i)
            delfrS(branch[i]);
    } else {
        delfrD(minID);
        dfs(curS - 1);
        addtoD(minID);
        if(!canadd(minID)) return;
        addtoS(minID);
        int canselect = K - 1 - notadj[minID];
        int pos = -1;
        bool all = true;
        for(int i = 0; i < canselect; ++ i) {
            delfrD(branch[i]);
            if(i && !canadd(branch[i - 1])) {
                addtoD(branch[i]);
                all = false;
                break;
            }
            if(i) {
                addtoS(branch[i - 1]);
                pos = i - 1;
            }
            dfs(curS - 1);
            addtoD(branch[i]);
        }
        if (all && (canselect == 0 || canadd(branch[canselect - 1]))) {
            for(int i = canselect; i < (int)branch.size(); ++ i)
                delfrD(branch[i]);
            if(canselect) addtoS(branch[canselect - 1]);
            dfs(curS - branch.size() + canselect);
            if(canselect) delfrS(branch[canselect - 1]);
            for(int i = canselect; i < (int)branch.size(); ++ i)
                addtoD(branch[i]);
        }
        for(int i = 0; i <= pos; ++ i)
            delfrS(branch[i]);
        delfrS(minID);
    }
}

void work() {
    srand(time(NULL));
    startTime = clock();
    dfs_node = 0;
    LB = max(wG, K);
    for (int i = 0; i < 16; ++i)
        twoPow[1 << i] = i;
    if (!PreProcess(LB + 1))
        exit_program();
    getOrd();
    dfs(n);
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
