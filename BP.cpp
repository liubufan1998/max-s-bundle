#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int MaxN = 1100000;

const int MaxM = 7700000;

const int Maxn = 20000;

const int TIME_LIM = 3600;

int N, M, K, wG;

pair <int, int> *E;

void read_clq() {
    char str[200], stmp[10];
    while (true) {
        gets(str);
        if (*str == 'p')
            break;
    }
    sscanf(str, "%s%s%d%d", stmp, stmp, &N, &M);
    assert(N <= MaxN);
    assert(M <= MaxM);
    E = new pair <int, int> [M];
    static set < pair <int, int> > st;
    int m = 0;
    for (int i = 0; i < M; ++i) {
        int u, v;
        scanf("%s%d%d", stmp, &u, &v);
        if (u == v)
            continue;
        --u; --v;
        if (u > v)
            swap(u, v);
        if (st.count(make_pair(u, v)))
            continue;
        st.insert(make_pair(u, v));
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
    fs >> N >> M;
    int x;
    while (fs >> x);
    assert(N <= MaxN);
    assert(M <= MaxM);
    E = new pair <int, int> [M];
    static set <int> st;
    int m = 0;
    for (int i = 0; i < N; ++i) {
        getline(cin, str);
        stringstream ss;
        ss << str;
        int x;
        while (ss >> x) {
            --x;
            if (x >= i || st.count(x))
                continue;
            st.insert(x);
            E[m++] = make_pair(i, x);
        }
        st.clear();
    }
    M = m;
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
    gets(str); gets(str);
    scanf("%s%s%d%s%d", str, str, &N, str, &M);
    gets(str); gets(str);
    assert(N <= MaxN);
    assert(M <= MaxM);
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
        buf = NULL;
    }
    ~BIT_SET() {
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
} *adjN, *invN, inD;

int head[MaxN], *nxt, *to, etot;

inline void addedge(int *h, int v) {
    nxt[etot] = *h;
    to[etot] = v;
    *h = etot++;
}

int degree[MaxN], que[MaxN], nV[MaxN], n;

bool outcore[MaxN];

bool del[Maxn], ins[Maxn];

int notadj[Maxn];

vector <int> svex;

bool PreProcess(int S) {
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
    assert(n <= Maxn);
    int m = 0;
    for (int i = 0; i < M; ++i) {
        int u = E[i].first, v = E[i].second;
        if (outcore[u] || outcore[v])
            continue;
        E[m++] = make_pair(nV[u], nV[v]);
    }
    M = m;
    return true;
}

int up_bound[Maxn], ordID[Maxn];

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
    sort(ordID, ordID + n, [](const int &x, const int &y) { return up_bound[x] < up_bound[y]; } );
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
    memset(del, 0, n * sizeof(bool));
    memset(ins, 0, n * sizeof(bool));
    memset(notadj, 0, n * sizeof(int));
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
    delete [] nxt;
    delete [] to;
    delete [] adjN;
    delete [] invN;
    exit(0);
}

inline void delfrD(int u) {
    inD.set(u);
    del[u] = true;
    for (int e = head[u]; ~e; e = nxt[e])
        --degree[to[e]];
}

inline void addtoD(int u) {
    inD.set(u);
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

const int INF = 0x3f3f3f3f;

const int MaxSn = 500;

const int MaxSm = 500000;

class MaximumFlow {
private:
    struct Node {
        int from, to, next;
        int cap;
    } edge[MaxSm];
    int cap_backup[MaxSm];
    int tol;
    int head[MaxSn];
    int dep[MaxSn];
    int gap[MaxSn], que[MaxSn];
    int cur[MaxSn];
    int S[MaxSn];
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
            if (front == MaxSn)
                front = 0;
            for (int i = head[u]; i != -1; i = edge[i].next) {
                int v = edge[i].to;
                if (dep[v] != -1)
                    continue;
                que[rear++] = v;
                if (rear == MaxSn)
                    rear = 0;
                dep[v] = dep[u] + 1;
                ++gap[dep[v]];
            }
        }
    }

public:
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
};

class VertexConnectivity {
private:
    MaximumFlow mf;
    int nV[Maxn], oID[Maxn];
public:
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

int dis[Maxn];

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
//curS  是当前整张图的大小
void dfs(int curS) {
    ++dfs_node;
    LB = max(LB, (int)svex.size());
    if (curS <= LB)
        return;
    int minID = -1;
    for(int i = 0; i < n; ++ i) if(!del[i]) {
        if(minID == -1 || degree[i] < degree[minID])
            minID = i;
    }
	//check if the whole graph is a k-plex and k-bundle
    if(degree[minID] >= curS - K && vc.solve(false, curS - K)) {
        LB = curS;
        return;
    }
    if(degree[minID] < curS - K) {
        if(degree[minID] < LB + 1 - K) { //reduction by degree
            if(ins[minID]) 
				return;//minID is in the current solution
            delfrD(minID);//remove from the current graph
            dfs(curS - 1);
            addtoD(minID);
            return;
        }
        int maxID = -1; //maxID和当前解集不相邻最多的点，
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
		//如果maxID在解集合中，且是sat点。reduce 所有和其不相邻的点
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
			//从svex中随机取出一个点，删除所有和该点距离>max(2,K+K-LB)的点  
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
    }
    check_timeout();
	//Find the  vertex with minimum degree

	if (ins[minID]){
		//find another minId not in set S
		int minID= - 1;
		for(int i = 0; i < n; ++ i) if(!del[i] && !ins[i]) {
			if(minID == -1 || degree[i] < degree[minID])
				minID = i;    
		}
	}
	//binary parition rule
	addtoS(minID);
	dfs(curS);
	delfrS(minID);
	
	delfrD(minID);
	dfs(curS - 1);
	addtoD(minID);
	/*
    vector < pair <int, int> > lst; //first:vertex second: color-bound
    get_color(lst);
    int m = lst.size();
    for (int i = m - 1; i >= 0; --i) {
        if ((int)svex.size() + lst[i].second <= LB) {
            for (int j = i + 1; j < m; ++j)
                addtoD(lst[j].first);
            return;
        }
        if (canadd(lst[i].first)) {
            addtoS(lst[i].first);
            dfs(curS);
            delfrS(lst[i].first);
        }
        --curS;
        delfrD(lst[i].first);
    }
    for (int i = 0; i < m; ++i)
        addtoD(lst[i].first);
	*/
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
