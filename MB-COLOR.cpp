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
	//去重边
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
} *adjN, *invN, inD, inG;

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

vector <int> svex; //解集

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

//up_bound[i] core number 
int up_bound[Maxn], ordID[Maxn];

void getOrd() {
	//core decomposition
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
	/**
	按照Up_bound对点进行排序，然后重新编号，建立一个新的图
	*/
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
        invN[i].flip();//补图0-1 bitset
    }
    inG.init(n); inG.flip(); //inG 进行分支时，inG[i]=1表示在整个图里面（有可能解集里面）
    inD.init(n); inD.flip();
    memset(del, 0, n * sizeof(bool));//标记一个点是否已经被删除
    memset(ins, 0, n * sizeof(bool));//标记是否在解集中
    memset(notadj, 0, n * sizeof(int));//notadj[i] 在解集中和i不相邻的点的个数
    svex.clear(); //解集合内的点
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
//将u从图中删除
inline void delfrD(int u) {
    inG.set(u);
    inD.set(u);
    del[u] = true;
    for (int e = head[u]; ~e; e = nxt[e])
        --degree[to[e]]; //所有u的邻接点度数减1
}
//将u加回图中
inline void addtoD(int u) {
    inG.set(u);
    inD.set(u);//标记当前在图中，尚未加入解集合的点 inD.test(u) == 1 
    del[u] = false;
    for (int e = head[u]; ~e; e = nxt[e])
        ++degree[to[e]];
}
//从解集合中删除
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
    int tol;//总的边数
    int head[MaxSn];//head[u]点u的邻接边的起始点
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
    bool solve(bool cals, int bound) { //检查集合svex内的点是否构成一个k-bundle
        int nn = 0;
        if (cals) {
			//构造最大流的图
            for (auto u : svex)//重新编号
                nV[u] = nn, oID[nn++] = u;
            mf.init(nn + nn);
			for (auto u : svex)//每个点拆分为边nv[u],nv[u]+nn
				mf.addedge(nV[u], nV[u] + nn, 1);
            for (auto u : svex) {
                for (auto v : svex) {
                    if (!adjN[u].test(v))
                        continue;
                    mf.addedge(nV[u] + nn, nV[v], INF);
                }
            }
			//计算最后加入的点与其他点之间最大流，此处可以优化，减少最大流运行次数
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
    for (auto v : svex) 
		if (!adjN[u].test(v)) {
			if (++tot >= K)
				return false;
			if (notadj[v] >= K - 1)
				return false;
    }
    svex.push_back(u);
    bool ret = vc.solve(true, svex.size() - K);//检查点联通度
    svex.pop_back();
    return ret;
}

int dis[Maxn];

//以s为起点，检查所有点到s的距离
void bfs(int s) {
    memset(dis, -1, n * sizeof(int));
    dis[s] = 0;
    int fr = 0, re = 0;
    que[re ++] = s;
    while(fr ^ re) { //while (fr != re)
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

/*计算染色数*/
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
            int u = Q.lowbit(); //最低位
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
    ++dfs_node;
    LB = max(LB, (int)svex.size());
    if (curS <= LB)
        return;
    if (check && vc.solve(false, curS - K)) {//判断当前解集是不是一个kbundle
        LB = curS;
        return;
    }
    vector < pair <int, int> > lst;
    get_color(lst); //染色，分支 list[i].second是第i个点的染色数
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
        check_timeout();
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

//curS：当前整张图的大小
void dfs(int curS) {
    ++dfs_node;
    LB = max(LB, (int)svex.size());
    if (curS <= LB)
        return;
    int minID = -1;
	//当前图中度数最小的点minID，判断整张图是否是一个k-plex
    for(int i = 0; i < n; ++ i) if(!del[i]) {
        if(minID == -1 || degree[i] < degree[minID])
            minID = i;
    }
    if(degree[minID] >= curS - K) { //整张图都是一个ｋ-plex，利用k-bundle分支
        dfs_kbundle(curS, true); //进入k-bundle分支
        return;
    }
	
	//reduce minID
    if(degree[minID] < LB + 1 - K) {
        if(ins[minID])
			return; //minID在解集合中
        delfrD(minID); //从图中Reduce
        dfs(curS - 1);	
        addtoD(minID);
        return;
    }
	//图中maxID和当前解集合不相邻最多的点，
    int maxID = -1;
    for(int i = 0; i < n; ++ i) if(!del[i]) {
        if(maxID == -1 || notadj[i] > notadj[maxID])
            maxID = i;
    }
	//如果maxID不在解集合中，直接reduce 该点。
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
		//reduce所有和maxid不相邻的点
        for(int i = 0; i < n; ++ i) 
			if(!del[i] && !ins[i] && !adjN[maxID].test(i)) {
            todel.push_back(i);
        }
        if(todel.size()) {
            for(auto x : todel)
				delfrD(x);
            dfs(curS - todel.size());
            for(auto x : todel) 
				addtoD(x);
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
    if (get_color() <= LB)
        return;
    check_timeout();
	//分支：branch包含所有和最小度数点,i.e., minId 不相邻且不在svex中的点
    vector <int> branch;
    for(int x = 0; x < n; ++ x) 
		if(!del[x] && x != minID && !ins[x]) {
			if(!adjN[minID].test(x))
				branch.push_back(x);
		}
    if(ins[minID]) {//k-plex分支
        int canselect = K - 1 - notadj[minID], pos = -1;
        bool all = true;
        for(int i = 0; i < canselect; ++ i) {
            delfrD(branch[i]);
            if(i && !canadd(branch[i - 1])) { //加入i后是否仍旧是一个kbundle
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
            if(canselect) 
				addtoS(branch[canselect - 1]);
            dfs(curS - branch.size() + canselect);
            if(canselect) 
				delfrS(branch[canselect - 1]);
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
    LB = max(wG, K);//wg=最大团 
    for (int i = 0; i < 16; ++i)
        twoPow[1 << i] = i;//预先算出2^i 
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
