#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int TIME_LIM = 3600;

int N, M, K, wG;
pair<int, int> *E;

// gets() is deprecated
#define gets(str) (fgets(str, 200, stdin))

void read_clq()
{ // 我没有想到这些代码里面居然还包括了去除重复边！
    char str[200], stmp[10];
    while (true)
    {
        gets(str);
        if (*str == 'p')
            break;
    }
    sscanf(str, "%s%s%d%d", stmp, stmp, &N, &M);
    E = new pair<int, int>[M];
    static set<pair<int, int>> st; // 使用一个 set 数据结构 st 来记录已经添加过的边，以防止重复添加相同的边。
    int m = 0;
    for (int i = 0; i < M; ++i)
    {
        int u, v;
        scanf("%s%d%d", stmp, &u, &v);
        if (u == v) // 如果 u 和 v 相等，表示自环，直接跳过。
            continue;
        --u;
        --v;
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

void read_graph()
{
    string str;
    getline(cin, str);
    stringstream fs;
    fs << str;
    fs >> N >> M; // M is not a reliable number of the edges
    // 从 fs 中提取两个整数 N 和 M，分别表示图的顶点数和边数。注意，M 这里被提取，但后面的代码会进行修正，因为它可能不是准确的边数。
    vector<pair<int, int>> vedges;
    static set<int> st;
    // int m = 0;
    for (int i = 0; i < N; ++i)
    {
        getline(cin, str);
        stringstream ss;
        ss << str;
        int x;
        while (ss >> x)
        {
            --x;
            assert(0 <= x && x < N);
            if (x >= i || st.count(x)) // 如果 x 大于或等于当前顶点的索引 i，或者在 st 集合中已经存在，就跳过这个邻接顶点，以避免处理重复的边。
                continue;
            st.insert(x);
            // cerr << m << M <<endl;
            // assert(m < M);
            // E[m++] = make_pair(i, x);
            // m++;
            vedges.push_back(make_pair(i, x)); // 如果通过上述条件检查，该邻接顶点是有效的且未处理的边，则将其添加到 vedges 向量中，并将 (i, x) 形式的边信息存储在其中。
        }
        st.clear();
    }
    if (vedges.size() != M)
    {
        // 检查 vedges 中的边的数量是否等于初始提供的 M 值，如果不相等，则输出一条错误信息，指出提供的边数与实际边数不一致。
        fprintf(stderr, "Given edege number %d, Actual edge number %d\n", M, vedges.size());
    }
    M = vedges.size();
    E = new pair<int, int>[M];
    copy(vedges.begin(), vedges.end(), E);
}

map<int, int> mp;

inline int find_id(int u)
{
    // 这段代码的作用是将输入的顶点标识 u 映射到内部的标识，并返回映射后的标识值。这有助于管理和重排顶点标识，以便在图算法中使用。
    auto it = mp.find(u);
    if (it != mp.end())
        return it->second;
    int sz = mp.size();
    return mp[u] = sz;
}

/*这段代码的主要功能是从文本文件中读取图的信息，包括顶点数和边数，然后解析每条边的信息，将有效的边存储在E数组中。
同时，它还处理自环边和重复边，并且在需要时对顶点进行重新编号。*/
void read_txt()
{
    /*这段代码的作用是从文本文件中读取图的顶点数和边数，然后解析每条边的信息，并将有效的边存储在 E 数组中。
    同时，它会跳过自环边和重复的边。如果存在重新编号的操作，也会将顶点重新编号为连续的整数。*/
    char str[200]; /*声明一个字符数组str，用于临时存储从文件中读取的文本行*/
    /*读取文本文件的一行内容，通常用于跳过文件中的注释或无关的信息。下面调用了两次，可能用于跳过前两行不相关的文本。*/
    // gets(str);
    // gets(str);
    /*使用scanf函数从文本文件中读取格式化的数据。它会读取两个字符串（通常是标签或标识符），然后读取两个整数，分别存储为N和M。这里假定N是顶点数，M是边数。*/
    // scanf("%s%s%d%s%d", str, str, &N, str, &M);
    scanf("%d %d", &N, &M);
    /*再次使用gets跳过两行不相关的文本。*/
    // gets(str);
    // gets(str);
    E = new pair<int, int>[M];     /*动态分配一个大小为M的pair<int, int>数组E，用于存储图的边。*/
    int m = 0;                     /* 声明一个整数m，用于跟踪有效边的数量。*/
    static set<pair<int, int>> st; /*创建一个静态的set容器st，用于存储已经添加的边的信息，以便检查重复边。*/
    for (int i = 0; i < M; ++i)    /*开始一个循环，迭代处理每条边的信息*/
    {
        int u, v;
        scanf("%d%d", &u, &v); /*使用scanf函数读取两个整数，表示当前边的两个顶点u和v。*/
        if (u == v) /*检查u和v是否相等，如果相等，说明存在自环，跳过这条边，不将其存储在E中。*/
            continue;
        /*如果没有自环，将顶点u和v分别传递给find_id函数，可能是用于重新编号顶点。这一行假设find_id函数执行了重新编号的操作*/
        u = find_id(u);
        v = find_id(v);
        if (u > v) /*确保u小于等于v，因为无向图中的边可以以两种顺序表示。这样有助于避免重复边。*/
            swap(u, v);
        if (st.count(make_pair(u, v))) /*检查(u, v)和(v, u)是否已经存在于集合st中，如果存在，说明这条边已经添加过了，跳过这条边。*/
            continue;
        st.insert(make_pair(u, v)); /*将当前边的信息(u, v)添加到集合st中，以便后续检查重复边。*/
        assert(m < M);              /*使用assert宏来确保m小于M，即有效边的数量没有超过预分配的数组大小。*/
        E[m++] = make_pair(u, v);   /*将当前边的信息(u, v)存储在数组E中，并增加有效边的计数器m。*/
    }
    M = m;      /* 更新边的数量M为有效边的数量m*/
    st.clear(); /* 清空集合st，以便在下一轮迭代中存储新的边信息*/
    mp.clear(); /*清空可能用于重新编号顶点的mp容器。*/
}

int twoPow[1 << 16], msk = (1 << 16) - 1;
/*
twoPow 是一个整数数组。
1 << 16 是一个位移操作，意味着数字 1 向左移动 16 位。在大多数现代架构中，int 是32位，这意味着 1 << 16 将会是 65536（即 2 的 16 次方）。
因此，twoPow 被定义为一个包含 65536 个整数的数组。

msk 是一个整数变量。
同样地，1 << 16 产生了 65536。
然后从 65536 减去 1，得到 65535。
所以 msk 被初始化为 65535，它的二进制表示是 16 个 1（即 1111111111111111）。

msk 可以用来快速获取一个数的低 16 位。当你将一个整数与 msk 进行按位与操作时，只有该整数的低16位参与计算，
因为 msk 的高16位都是0（在32位整数中）。这样，只有原始数的低16位能够“通过”掩码；高位由于与0进行了运算，因此在结果中都变为0。
*/

struct BIT_SET
{
    int n, m;
    unsigned *buf;
    BIT_SET()
    {
        n = m = 0;
        buf = nullptr;
    }
    ~BIT_SET()
    {
        if (buf != nullptr)
            delete[] buf;
    }
    void init(int _n)
    {
        /*这段代码的作用是初始化一个位图数据结构，根据输入的位数 _n，确定位图需要多少个 unsigned 整数来存储，然后分配相应的内存空间，并将位图的数据初始化为全 0。
        这种位图通常用于位运算和集合操作，可以高效地表示和处理大量的布尔值信息。*/
        m = _n & 31;
        /*这行代码计算 _n 的低 5 位，它使用了位运算 &（按位与），31 的二进制表示为 11111，所以 & 操作可以提取 _n 的低 5 位，
        结果赋值给变量 m。这个操作的目的是确定每个 unsigned 整数中的位数，通常是 32 位。*/
        n = _n >> 5;
        /*
        这行代码计算 _n 的高位，也就是除以 32 后的商，用右移运算符 >> 实现，将结果赋值给变量 n。这个操作的目的是确定需要多少个 unsigned 整数来表示 _n 位的位图。
        */
        buf = new unsigned[n + 1];
        for (int i = 0; i <= n; ++i)
            buf[i] = 0; /*在循环中，将 buf 数组中的每个元素初始化为 0，以确保位图的初始状态是所有位都为 0。*/
    }
    void flip()
    {
        for (int i = 0; i < n; ++i)
            buf[i] = ~buf[i]; /*对位图中的每个 unsigned 整数执行按位取反操作，即将所有的 0 变为 1，将所有的 1 变为 0。这一步实现了位图中每个位的反转。*/
        buf[n] ^= ((unsigned)1 << m) - 1;
        /*这行代码执行了一个异或操作，用于反转最后一个 unsigned 整数中的高位。
       ((unsigned)1 << m) - 1：这部分代码计算了一个值，它是将 1 左移 m 位，然后减去 1。这个操作实际上生成了一个二进制数，其中低 m 位为 1，高位为 0。
       例如，如果 m 等于 5，那么 (1 << m) - 1 的二进制表示为 0000011111。
       ^：这是异或运算符. */

        /*反转最后一个 unsigned 整数中的高位是为了确保位图的长度（或位数）是精确的，这在位图操作中非常重要。
        位图通常用于表示一组元素的存在与否，其中每个位对应一个元素。在某些情况下，位图的位数可能不是 32 的整数倍，
        因此最后一个 unsigned 整数可能只部分用于存储位信息。这就需要考虑如何正确处理这些额外的位。*/
    }
    void set(int x)
    {
        buf[x >> 5] ^= (unsigned)1 << (x & 31);
        /*x >> 5 是一个位运算操作，将整数 x 右移5位，相当于将 x 除以32，这是因为每个无符号整数通常有32位（4字节）。这一步用于定位到存储位 x 的无符号整数。
        (x & 31)：这是位与运算操作，用于获取 x 的最低5位的值，因为 31 的二进制表示是 11111，它只有最低5位是1，其余位是0。
        (unsigned)1 << (x & 31)：将整数1左移 x & 31 位，得到一个只有第 x & 31 位是1的无符号整数。这是一种位掩码，用于在 buf[x >> 5] 中设置指定位置的位。
        */
    }
    bool test(int x)
    {
        /*这个 test 函数通过位操作检查位集合中特定位置 x 的位是否设置。如果该位被设置（即为 1），则函数返回 true；如果未被设置（即为 0），则返回 false。*/
        return buf[x >> 5] >> (x & 31) & 1;
        /*& 1：最后，我们使用位与操作 &，将上一步的结果与 1 进行与运算。这样可以屏蔽掉除了最低位以外的其他位。如果最低位是 1，
        这个表达式的结果就是 1（即 true），如果最低位是 0，结果就是 0（即 false）。*/
    }
    int lb(unsigned x)
    {
        /*lb 函数根据 x 的值在 twoPow 中查找相应的结果，并根据 x 的哪一部分（高16位或低16位）包含有效位来调整这个结果。*/
        if (x & msk)                /*它检查 x 的低16位是否有任何位被设置。*/
            return twoPow[x & msk]; /*如果 x 的低16位中有设置的位，函数返回 twoPow 数组中相应索引处的值。*/
        return twoPow[x >> 16 & msk] + 16;
    }
    int lowbit()
    {
        /*此函数的目的是查找并返回集合中第一个非零位的位置。它使用了位运算来高效地完成这项任务。*/
        for (int i = 0; i <= n; ++i)
        {
            unsigned x = buf[i] & ((~buf[i]) + 1); /*这行代码使用位运算找到 buf[i] 中最低位的非零位。
            ~buf[i]：首先对 buf[i] 进行按位取反操作。
            (~buf[i]) + 1：然后对结果加 1。这实际上是计算 buf[i] 的二进制补码。
            buf[i] & ((~buf[i]) + 1)：最后，原始值和它的补码进行按位与运算。这个运算的结果是保留了 buf[i] 中最右边的那个 1，而将其他所有位都置为 0*/
            if (x)                                 /*如果 x 不为零，表示在当前的 buf[i] 中找到了非零位。*/
                return lb(x) + (i << 5);
        }
        return -1;
    }
    bool empty()
    {
        for (int i = 0; i <= n; ++i)
            if (buf[i])
                return false;
        return true;
    }
    void operator&=(const BIT_SET &rhs)
    {
        /*这是 BIT_SET 类型的按位与赋值操作符的重载。该操作符接受一个常量引用参数 rhs，表示右侧的 BIT_SET 对象。*/
        for (int i = 0; i <= n; ++i)
            this->buf[i] &= rhs.buf[i];
        /*这个函数通常用于将两个位集合进行逻辑与操作，可能用在诸如集合交集之类的场合，
        只有当两个 BIT_SET 对象中相同位置的位都被设置时，结果 BIT_SET 对象在这个位置的位才会被设置。*/
    }
    void cp(BIT_SET &rhs) const
    {
        /*这个 cp 函数实际上实现了一个深拷贝操作，因为它不仅仅复制了成员变量的值，还复制了指向动态分配数组的指针所指向的数据。
        这是在处理涉及动态内存分配的类时常见的做法，以确保副本拥有自己的独立数据副本，防止潜在的悬挂指针或双重释放等问题。*/
        rhs.n = n;
        rhs.m = m;
        if (rhs.buf == NULL)
            rhs.buf = new unsigned[n + 1];
        for (int i = 0; i <= n; ++i)
            rhs.buf[i] = buf[i];
    }
} *adjN, *invN, inD, inG;
/*这里声明了 adjN 和 invN 作为 BIT_SET 类型的指针。在 C++ 中，指针是一个变量，它存储了另一个变量的内存地址。
这里，adjN 和 invN 是指向 BIT_SET 类型对象的指针。
通常这种声明用于指向动态分配的 BIT_SET 对象或对象数组。它们可能会在程序的其他部分使用 new 关键字进行内存分配。

inD 和 inG 是 BIT_SET 类型的实例。与 adjN 和 invN 不同，它们不是指针，而是实际的对象。这种声明表示在程序中直接创建了两个 BIT_SET 类型的对象，
它们将拥有 BIT_SET 结构体中定义的所有属性和行为。

这种声明方式允许在程序中灵活使用 BIT_SET 结构体。通过指针，可以管理动态分配的内存，这在处理大量数据或在运行时确定数据大小时非常有用。
而直接声明的对象则用于那些大小已知且生命周期与声明它们的作用域相关的情况。

在实际应用中，adjN 和 invN 可能用于存储图的邻接信息，比如一个图中顶点的邻接情况，而 inD 和 inG 可能表示图中顶点的某种状态或属性。
具体的应用依赖于程序的其余部分以及这些变量是如何被使用的*/

int *head, *nxt, *to, etot;
int *degree, *que, *nV, n;
bool *outcore;
bool *del, *ins;
int *notadj;
int *up_bound, *ordID;
int *dis;

const int INF = 0x3f3f3f3f;
class MaximumFlow
{
private:
    struct Node
    {
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
    void BFS(int start, int end)
    {
        memset(dep, -1, n * sizeof(int));
        memset(gap, 0, n * sizeof(int));
        gap[0] = 1;
        int front, rear;
        front = rear = 0;
        dep[end] = 0;
        que[rear++] = end;
        while (front != rear)
        {
            int u = que[front++];
            for (int i = head[u]; i != -1; i = edge[i].next)
            {
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
    /*reserve 方法是用于分配足够空间来存储图的边和顶点信息*/
    void reserve(int n, int m) /*n 和 m 分别是顶点和边的数量。*/
    {
        /*这里进行了额外的空间分配（n += 3 和 m * 4 + n * 2），这可能是为了处理算法中的特定边界情况或是为了避免数组越界。*/
        n += 3;
        edge = new Node[m * 4 + n * 2];      /*edge 用于存储边信息*/
        cap_backup = new int[m * 4 + n * 2]; /*cap_backup 用于备份边的容量*/
        head = new int[n + n];               /*head 用于指示每个顶点的邻接链表的开始，*/
        dep = new int[n + n];
        gap = new int[n + n];
        que = new int[n + n];
        cur = new int[n + n];
        S = new int[n + n];
    }

    /*init 方法初始化网络流的图结构*/
    void init(int _n)
    {
        tol = 0;                           /*tol 用于追踪边的数量的计数器*/
        n = _n + 2;                        /*n 被设置为 _n + 2，这里的 +2 可能是为了包括源点和汇点。*/
        memset(head, -1, n * sizeof(int)); /*head 数组被初始化为 -1，这可能表示每个顶点的邻接链表开始之前没有边。*/
    }

    /*addedge 方法用于向图中添加边。*/
    void addedge(int u, int v, int w)
    {
        /*它创建了两条边：从 u 到 v 有容量 w，从 v 到 u 的容量为 0。这通常是网络流中的正向边和反向边。
        每次添加边时，tol 都会递增，用于记录下一个边的索引。这里使用了邻接链表的数据结构，
        head 数组记录了每个顶点的第一条出边，
        而 next 属性用于指向下一条出边。这样的结构使得可以快速地迭代顶点的所有出边。
        cap_backup 用于存储边的初始容量，这在算法中可能用于复原图的初始状态。*/
        edge[tol].from = u;
        edge[tol].to = v;
        edge[tol].cap = w;
        cap_backup[tol] = w;
        edge[tol].next = head[u];
        head[u] = tol++;
        edge[tol].from = v;
        edge[tol].to = u;
        edge[tol].cap = 0;
        cap_backup[tol] = 0;
        edge[tol].next = head[v];
        head[v] = tol++;
    }

    /*这段代码实现了网络流中的SAP（Shortest Augmenting Path）算法，用于计算从起点start到终点end的最大流。
    代码中使用了很多网络流算法的标准技术，例如前向边和反向边、层次图（level graph）、以及GAP优化。以下是对代码的详细分析：*/
    int SAP(int start, int end)
    {
        /* 恢复每条边的初始容量，为新的流计算做准备*/
        for (int i = 0; i < tol; ++i)
            edge[i].cap = cap_backup[i];

        /*初始化结果变量和BFS函数构建层次图*/
        int res = 0;
        BFS(start, end);
        /*cur数组用于记录当前弧优化，即当前遍历到的边*/
        int top = 0;
        memcpy(cur, head, n * sizeof(int));
        /*开始主循环，u表示当前遍历到的节点*/
        int u = start;
        int i;
        while (dep[start] < n) /*当start的层次小于n时继续循环*/
        {                      /*如果到达终点，尝试更新流*/
            if (u == end)
            {
                /*找到增广路径中容量最小的边*/
                int temp = INF + 1;
                int inser = 0;
                for (i = 0; i < top; ++i)
                    if (temp > edge[S[i]].cap)
                    {
                        temp = edge[S[i]].cap;
                        inser = i;
                    }
                /*更新路径上所有边的容量*/
                for (i = 0; i < top; ++i)
                {
                    edge[S[i]].cap -= temp;
                    edge[S[i] ^ 1].cap += temp;
                }
                res += temp; /*增加结果*/
                /*回退到增广路径上容量最小边的起点*/
                top = inser;
                u = edge[S[top]].from;
            }
            /*如果找不到增广路径，且GAP优化条件成立，退出循环*/
            if (u != end && gap[dep[u] - 1] == 0)
                break;
            /*查找当前弧指向的下一个合适的边*/
            for (i = cur[u]; i != -1; i = edge[i].next)
                if (edge[i].cap != 0 && dep[u] == dep[edge[i].to] + 1)
                    break;

            /*如果找到了合适的边，前进到该边的终点*/
            if (i != -1)
            {
                cur[u] = i;
                S[top++] = i;
                u = edge[i].to;
            }
            else /*如果当前节点没有合适的边，则退回到前一个节点，并更新层次*/
            {
                int min = n;
                for (i = head[u]; i != -1; i = edge[i].next)
                {
                    if (edge[i].cap == 0)
                        continue;
                    if (min > dep[edge[i].to])
                    {
                        min = dep[edge[i].to];
                        cur[u] = i;
                    }
                }
                --gap[dep[u]];
                dep[u] = min + 1;
                ++gap[dep[u]];
                /*如果不是起点，则回退*/
                if (u != start)
                    u = edge[S[--top]].from;
            }
        }
        return res; /*返回最大流的结果*/
    }
} mf;

/*这个函数通过 SAP 算法来判断移除一定数量的顶点后图是否仍然连通。
如果移除少于 bound 个顶点后图不再连通，则认为图的顶点连通性小于 bound。
这是一个复杂的算法，通常用于图论和网络流中的高级问题。*/
vector<int> svex; /*用于存储图中的一组特定顶点*/
class VertexConnectivity
{
private:
    /*两个私有成员是指针，用于动态分配数组*/
    int *nV, *oID; /*nV 可能用于存储顶点的新编号，oID 可能用于存储原始编号。*/

public:
    /*为 nV 和 oID 数组分配内存空间*/
    void reserve(int n)
    {
        n += 3;
        nV = new int[n];
        oID = new int[n];
    }

    /*类的主要函数，它用于计算顶点连通性。*/
    /*函数有两种不同的模式，取决于参数 cals 的值。
    如果 cals 为真，函数用于检查子图 svex 的连通性。
    如果 cals 为假，函数用于检查整个图的全局连通性。*/
    bool solve(bool cals, int bound)
    {
        int nn = 0; /*定义并初始化一个局部变量 nn，可能用于跟踪数组 nV 和 oID 的填充情况。*/
        if (cals)   /*如果 cals 为真，则遍历 svex 向量中的每个顶点 u*/
        {
            for (auto u : svex)
                nV[u] = nn, oID[nn++] = u;        /*，并在 nV 中设置新的编号，同时在 oID 中保持其原始编号。*/
            mf.init(nn + nn);                     /*初始化 mf 对象，可能是一个用于网络流计算的对象，参数 nn + nn 暗示了一个源点和一个汇点。*/
            for (auto u : svex)                   /*遍历 svex 中的每个顶点，*/
                mf.addedge(nV[u], nV[u] + nn, 1); /*添加从顶点到其对应超级汇点的边，边的容量设置为 1。*/
            for (auto u : svex)
            {
                for (auto v : svex)
                {
                    if (!adjN[u].test(v))
                        continue;
                    mf.addedge(nV[u] + nn, nV[v], INF); /*然后为所有相邻顶点对添加无限大容量的边。*/
                }
            }
            int u = nV[svex.back()];     /*选取 svex 向量中最后一个顶点的新编号 u。*/
            for (int i = 0; i < nn; ++i) /*对于 nn 范围内的每个顶点 i，如果它不是顶点 u，并且 u 不与 i 邻接，
                                         则运行 SAP 算法计算从超级源点 u + nn 到 i 的最大流。*/
            {
                if (i == u || adjN[oID[u]].test(oID[i]))
                    continue;
                if (mf.SAP(u + nn, i) < bound) /*如果结果小于 bound，则返回 false，表示无法达到给定的顶点连通性。*/
                    return false;
            }
            return true; /*如果所有测试都通过，返回 true，表示图至少具有 bound 的顶点连通性。*/
        }
        else /*整个 else 分支的目的是检查图的全局顶点连通性，而不是 svex 子图的连通性。
        通过对所有未删除顶点进行网络流计算，它验证图是否在移除任意两个不相邻顶点后仍然连通。
        如果在任何情况下连通性小于 bound，则图的顶点连通性认为是小于 bound。*/
        {
            for (int i = 0; i < n; ++i)
                if (!del[i])                          /*如果顶点 i 没有被删除（即 del[i] 为假）*/
                    nV[i] = nn, oID[nn++] = i;        /*则将 i 存储在 nV 中，并且在 oID 中以新的编号 nn 存储原始编号 i，nn 递增。*/
            mf.init(nn + nn);                         /*初始化网络流对象 mf，节点数量为 nn + nn，这可能包括了为每个顶点添加的超级源点和超级汇点。*/
            for (int i = 0; i < n; ++i)               /*再次遍历所有未被删除的顶点。*/
                if (!del[i])                          /*对于每个未删除的顶点，*/
                    mf.addedge(nV[i], nV[i] + nn, 1); /*添加一条从顶点到其对应的超级汇点的边，边的容量为 1。*/
            for (int i = 0; i < n; ++i)               /*对于每个未删除的顶点 i，遍历其邻接表中的所有边*/
            {
                if (del[i]) /*这个条件语句检查顶点 i 是否已被删除，如果已删除，则跳过当前顶点的处理。*/
                    continue;
                for (int e = head[i]; ~e; e = nxt[e]) /*遍历顶点 i 的邻接表中的所有边。*/
                {
                    int v = to[e]; /*获取当前边的另一端点 v。*/
                    if (del[v])    /*检查另一端点 v 是否已被删除，如果已删除，则跳过当前边的处理。*/
                        continue;
                    mf.addedge(nV[i] + nn, nV[v], INF); /*如果边的另一端点 v 未删除，则在超级汇点和顶点之间添加一条容量为无限大的边。
                    这表示从顶点 i 出发，可以无限大地流向顶点 v，反映了原始图中的连通性。*/
                }
            }
            /*使用两层循环遍历所有顶点对 (i, j)*/
            for (int i = 0; i < nn; ++i)         /*外层循环，用于遍历顶点编号 i，其中 nn 是未删除顶点的数量。*/
                for (int j = i + 1; j < nn; ++j) /*内层循环，用于遍历顶点编号 j，确保 i 和 j 不是同一个顶点，避免重复检查。*/
                                                 /*这个条件语句检查顶点 oID[i] 和顶点 oID[j] 是否不相邻。它通过检查邻接矩阵 adjN 中的对应位来确定两个顶点之间是否没有边连接。
                                                 mf.SAP(nn + i, j)：这是调用一个网络流算法（可能是最大流算法）来计算从超级源点 nn + i 到顶点 j 的最大流。
                                                 这里的目的是验证在移除顶点 oID[i] 后，是否仍然可以通过网络流从超级源点到达顶点 j。*/
                    if (!adjN[oID[i]].test(oID[j]) && mf.SAP(nn + i, j) < bound)
                        /*将计算得到的最大流值与指定的 bound 进行比较，看是否小于 bound。如果小于 bound，
                        表示从超级源点到达顶点 j 的最大流小于 bound，即移除顶点 oID[i] 后，图的顶点连通性小于 bound。*/
                        return false;
            return true;
        }
    }
} vc;

/*代码定义了一个名为 addedge 的内联函数，该函数用于在图的邻接表中添加一条边*/
inline void addedge(int *h, int v)
{
    /*将当前顶点链表头 *h（这是一个链表中上一个元素的索引）的值赋给 nxt 数组的 etot 位置，
    它表示新边将插入到链表的开始位置，并且现在的头部将成为新边的下一个元素。*/
    nxt[etot] = *h;

    /*在 to 数组的 etot 位置记录目标顶点 v，to 数组用于存储与每条边相对应的目标顶点。*/
    to[etot] = v;

    /*更新头指针 *h 的值为当前边的索引 etot，然后 etot 自增，为下一次添加边做准备。
    etot 是全局变量，用于追踪邻接表中边的总数。*/
    *h = etot++;

    /*总之，这个函数是图的邻接表实现的一部分，它以高效的方式添加边。
    每次添加边时，它都会将新边插入到对应顶点的链表头部，并更新链表头指针。由于这种表示方法，图的所有边可以通过遍历链表来访问*/
}

/*PreProcess 函数，它用于对图数据进行预处理，以便之后进行算法操作。*/
bool PreProcess(int S)
{
    /*根据我从下面的代码中来看，这里面的预处理部分仅仅使用了对于顶点度数的判断来筛选顶点*/

    /*首先进行了内存分配*/
    /*M 是边的数量，而 N 是顶点的数量*/
    nxt = new int[M + M]; /*nxt 和 to 用于存储邻接链表的信息，*/
    to = new int[M + M];
    degree = new int[N];                /*degree 用于存储每个顶点的度数*/
    head = new int[N];                  /*head 是邻接链表的头指针*/
    outcore = new bool[N];              /*outcore 用于标记哪些顶点在预处理阶段被过滤掉*/
    que = new int[N];                   /*que 是用于广度优先搜索的队列*/
    nV = new int[N];                    /*nV 用于存储顶点的新编号。*/
    memset(degree, 0, N * sizeof(int)); /*degree 数组被初始化为零（表示所有顶点的初始度数为零）*/
    memset(head, -1, N * sizeof(int));  /*head 数组被初始化为 -1（表示链表的开始）*/
    etot = 0;                           /*etot 被设置为 0，它是边的索引，用于构建邻接链表。*/
    for (int i = 0; i < M; ++i)
    {
        /*这个循环处理图中的每一条边，增加相关顶点的度数，并通过 addedge 函数添加边。
        这里看起来是在构建一个无向图，因为它同时添加了 u 到 v 和 v 到 u 的边。*/

        /*对于每一条边，它会增加两个顶点 u 和 v 的度数，*/
        int u = E[i].first, v = E[i].second;
        ++degree[u];
        ++degree[v];

        /*然后调用 addedge 函数将这两个顶点互相连接，构建无向图的邻接链表。*/
        /*head + u 和 head + v 分别代表顶点 u 和 v 的邻接链表头部的位置。*/
        addedge(head + u, v);
        addedge(head + v, u);
    }
    memset(outcore, 0, N * sizeof(bool)); /*初始化 outcore 数组，用于标记顶点是否满足某些条件*/
    int fr = 0, re = 0;                   // 队列用于宽度优先搜索，fr 代表队列的头部，re 代表队列的尾部。

    /*循环初始化一个处理队列，把那些度数不满足条件的顶点标记出来*/
    for (int i = 0; i < N; ++i)
        if (degree[i] < S - K) /*遍历每个顶点，如果它的度数小于 S - K，则将该顶点加入队列，并在 outcore 中标记为 true。*/
            que[re++] = i, outcore[i] = true;
    /*用异或运算来检查队列是否为空，开始进行宽度优先搜索。*/
    while (fr ^ re)
    {                      // 这段代码通过队列实现了类似宽度优先搜索的操作，用于进一步处理顶点和边的关系。
        int u = que[fr++]; /*从队列中取出一个顶点 u。*/

        /*循环检查每个顶点的度数，如果度数低于阈值，则顶点会被加入 outcore。*/
        for (int e = head[u]; ~e; e = nxt[e])
        { /*遍历与顶点 u 相邻的所有顶点。*/
            /*循环的条件 ~e 意味着只要 e 不是 -1（~ 是按位取反操作符），循环就会继续。这里 -1 似乎用于表示边的结束。*/
            int v = to[e]; /*通过边的索引 e 得到与 u 相邻的顶点 v。*/
            if (outcore[v])
                continue; /*如果 v 已经在 outcore 中，则跳过它。*/
            if (--degree[v] < S - K)
                que[re++] = v, outcore[v] = true; /*如果 v 的度数减一后小于 S - K，则将 v 加入队列并标记在 outcore 中*/
        }
    }
    n = 0;
    for (int i = 0; i < N; ++i)
        if (!outcore[i])
            /*对于每个未标记为 outcore 的顶点 i，它在 nV 数组中被赋予一个新编号，然后 n 增加。这是为了创建一个新的顶点集，其中只包含特定条件的顶点。*/
            nV[i] = n++;
    if (n < S) /*如果处理后的顶点数量 n 小于某个阈值 S，函数返回 false。这表示图可能不满足某些预期条件或大小要求。*/
        return false;
    int m = 0; /*初始化边的计数器 m。*/

    /*
        下面代码遍历所有边，并对边进行过滤。如果边的任一端点被标记在 outcore 中，则该边被忽略。
        对于未被忽略的边，更新它们的端点为新的编号，并将其存储回 E 数组。这样，E 数组中的边仅包含未被 outcore 标记的顶点。
        m 计数未被忽略的边的数量，并最终用于更新 M，即图中边的总数。
        所以说outcore的意思就是不符合要求的顶点。
    */
    for (int i = 0; i < M; ++i)
    {
        /*循环遍历所有的边（由数组 E 存储）*/
        int u = E[i].first, v = E[i].second;
        /*对于每一条边 (u, v)，如果它的任一端点 u 或 v 被标记为 outcore（即它们中的至少一个不在核心子图中），则跳过这条边。*/
        if (outcore[u] || outcore[v])
            continue;
        /*否则，它将边重新映射为处理后的顶点编号，并存储在 E 数组中。*/
        E[m++] = make_pair(nV[u], nV[v]);
    }
    M = m;             /*更新 M 的值为 m，即过滤后的边的数量。*/
    del = new bool[n]; /*del 用于标记已经从当前考虑的子图中删除的顶点，*/
    memset(del, 0, n * sizeof(bool));
    ins = new bool[n]; /*ins 用于标记已经加入子图的顶点。*/
    memset(ins, 0, n * sizeof(bool));
    notadj = new int[n]; /*notadj 用于记录与某个顶点不相邻的顶点数量。*/
    memset(notadj, 0, n * sizeof(int));
    up_bound = new int[n];
    ordID = new int[n];
    dis = new int[n];
    mf.reserve(n, m);
    vc.reserve(n);
    return true;
}

/*getOrd 函数的主要作用是进行预处理和排序，以准备后续的图算法操作。其中，顶点的排序是基于度数和上界的，这有助于优化后续算法的性能。*/
void getOrd()
{
    /*初始化度数数组degree为0，头指针数组head为-1，边的总数etot为0。这些操作准备数据结构，以存储图中每个顶点的邻接信息。*/
    memset(degree, 0, n * sizeof(int));
    memset(head, -1, n * sizeof(int));
    etot = 0; // etot 用于记录边的总数。
    /*遍历所有边，更新顶点的度数，并通过addedge函数将边添加到邻接表中。*/
    for (int i = 0; i < M; ++i)
    {
        int u = E[i].first, v = E[i].second;
        ++degree[u];
        ++degree[v];
        addedge(head + u, v); // 调用 addedge 函数，将 v 添加到 u 的邻接表中，同时将 u 添加到 v 的邻接表中。
        addedge(head + v, u); // 在addedge函数里面，etot的数量进行了自增。
    }

    /*声明一个优先队列Q以及一个布尔数组outcore，并初始化outcore为0。*/
    /*Q用于存储顶点和它们的度数，*/
    static priority_queue<pair<int, int>> Q; // 创建一个名为 Q 的优先队列（最小堆），其中元素为 pair<int, int> 类型，表示顶点的度数和顶点的编号。
    /*outcore用来标记顶点是否已经处理。*/
    memset(outcore, 0, n * sizeof(bool));

    /*将所有顶点按度数的降序放入优先队列。*/
    for (int i = 0; i < n; ++i)
        Q.push(make_pair(-degree[i], i));
    /*这段代码是在一个循环中，将每个顶点按照其度数（degree）加入了一个优先队列（Priority Queue）Q 中，
    同时使用负数形式将度数变为负数，这是因为默认的优先队列是按照元素值的升序排列，
    而我们希望将度数最大的顶点排在队列的前面，因此将度数取负数来实现降序排列。*/

    /*使用优先队列进行迭代，更新顶点的上界curB。*/
    int curB = 1; /*初始化当前的上界（curB）为1。*/
    while (!Q.empty())
    {
        /*当优先队列不为空时，取出队列顶部的顶点u（度数最小的顶点）并将其从队列中移除。*/
        int u = Q.top().second;
        Q.pop();

        /*如果顶点u已经被标记为outcore（即已经处理过的顶点），则跳过它。否则，将u标记为outcore。*/
        if (outcore[u])
            continue;
        outcore[u] = true;
        /*检查顶点u的度数是否小于curB - K*/
        if (degree[u] < curB - K)
        {
            up_bound[u] = curB; /*如果是，u的上界设置为当前的curB。*/
        }
        else
        {
            curB = degree[u] + K + 1; /*否则，更新curB为degree[u] + K + 1*/
            up_bound[u] = curB;       /*并将u的上界设置为新的curB。*/
        }
        /*遍历顶点u的所有邻居。*/
        for (int e = head[u]; ~e; e = nxt[e])
        {
            /*对于每个邻居顶点v，如果v还没有被处理（不是outcore），则减少v的度数，并将v（及其新的度数）重新插入优先队列。*/
            int v = to[e];
            if (outcore[v])
                continue;
            --degree[v];
            Q.push(make_pair(-degree[v], v));
        }
    }

    /*初始化一个数组ordID，用于存储顶点的新顺序。*/
    for (int i = 0; i < n; ++i)
        ordID[i] = i;
    /*使用自定义比较函数的sort函数，根据上界up_bound对顶点进行排序*/
    sort(ordID, ordID + n, [](const int &x, const int &y)
         {
         if (up_bound[x] != up_bound[y])
            return up_bound[x] < up_bound[y];
         return x < y; });
    /*根据排序结果，更新顶点的新编号映射。*/
    for (int i = 0; i < n; ++i)
        nV[ordID[i]] = i;
    /*重新初始化 degree、head 和 etot，并创建了两个名为 adjN 和 invN 的位集合数组，用于存储邻接关系和逆邻接关系。*/
    memset(degree, 0, n * sizeof(int));
    memset(head, -1, n * sizeof(int));
    etot = 0;

    /*创建两个新的位集合数组adjN和invN，分别用于存储邻接关系和逆邻接关系。*/
    adjN = new BIT_SET[n];
    invN = new BIT_SET[n];
    /*初始化adjN数组，为每个顶点准备邻接位集*/
    for (int i = 0; i < n; ++i)
        adjN[i].init(n);
    /*对每条边进行遍历，将边的两个顶点重新映射为排序后的新编号，并更新相应的邻接关系和逆邻接关系。*/
    for (int i = 0; i < M; ++i)
    {
        int u = E[i].first, v = E[i].second; /*从边数组E中获取当前边的两个顶点u和v。*/
        /*将顶点u和v重新映射为排序后的新编号，nV是一个数组，存储了顶点的新编号映射关系。*/
        u = nV[u];
        v = nV[v];
        /*增加顶点u和v的度数，表示它们相邻的边数增加了*/
        ++degree[u];
        ++degree[v];
        /*调用addedge函数，将边的信息添加到邻接表中，同时增加了etot的值，用于记录边的总数。这两行代码用于构建图的邻接关系。*/
        addedge(head + u, v);
        addedge(head + v, u);
        /*使用位集合数组adjN记录顶点之间的邻接关系。这两行代码将位集合中的相应位设置为1，表示u和v之间有一条边*/
        adjN[u].set(v);
        adjN[v].set(u);
    }
    /*复制adjN到invN并对invN执行翻转操作，得到逆邻接关系。*/
    for (int i = 0; i < n; ++i)
    {
        adjN[i].cp(invN[i]); /*复制顶点i的位集合到逆邻接关系invN[i]*/
        invN[i].flip();      /* 对invN[i]执行翻转操作，将原来为1的位变为0，为0的位变为1，得到顶点i的逆邻接关系*/
    }
    /*初始化并翻转另外两个位集合inD和inG，它们可能用于在算法中存储其他的顶点属性或状态。*/
    inD.init(n); /*用init(n)初始化位集合的大小为n*/
    inD.flip();  /*使用flip()函数对位集合进行翻转，将所有位的值从0变为1，表示初始化为"true"状态。*/
    inG.init(n); /*同样初始化了另一个位集合inG，具有相同的功能，用于存储顶点的属性或状态。*/
    inG.flip();
    svex.clear(); /*清空一个名为svex的容器*/
}

clock_t startTime;
long long dfs_node;
char filename[100];
int LB;

void exit_program()
{
    printf("Filename: %s\n", filename);
    printf("Value of K: %d\n", K);
    printf("Search nodes: %lld\n", dfs_node);
    printf("Lower bound: %d\n", LB);
    printf("Used Time: %.10f s\n", (double)(clock() - startTime) / CLOCKS_PER_SEC);
    exit(0);
}

/*用于从图数据结构中删除节点u*/
inline void delfrD(int u)
{
    inD.set(u);                           /*设置一个名为inD的数据结构中的第u位，通常用于标记节点u在删除集合中。*/
    inG.set(u);                           /*设置一个名为inG的数据结构中的第u位，通常用于标记节点u在图中。*/
    del[u] = true;                        /*将名为del的布尔数组中的节点u标记为已删除*/
    for (int e = head[u]; ~e; e = nxt[e]) /*遍历节点u的所有边，*/
        --degree[to[e]];                  /*将边所连接的节点的度数减一，即将节点to[e]的度数减一。这是因为要删除节点u，其连接的节点的度数需要更新。*/
}

/*将节点u添加回图数据结构中*/
inline void addtoD(int u)
{
    inD.set(u);                           /*设置一个名为inD的数据结构中的第u位，通常用于标记节点u在删除集合中。*/
    inG.set(u);                           /*设置一个名为inG的数据结构中的第u位，通常用于标记节点u在图中*/
    del[u] = false;                       /*将名为del的布尔数组中的节点u标记为未删除*/
    for (int e = head[u]; ~e; e = nxt[e]) /*遍历节点u的所有边*/
        /*~e 是一个按位取反操作，用于检查 e 是否为 -1（通常 -1 用作哨兵值表示结束）。如果 e 不等于 -1，循环继续。*/
        ++degree[to[e]]; /*将边所连接的节点的度数加一，即将节点to[e]的度数加一。这是因为要将节点u添加回图中，其连接的节点的度数需要更新*/
}

/*用于将节点u从"活跃"状态删除*/
inline void delfrS(int u)
{
    inD.set(u);      /*设置一个名为inD的数据结构中的第u位，通常用于标记节点u在删除集合中。*/
    ins[u] = false;  /*更新 ins 数组，将顶点 u 标记为“未插入”或“非活跃”。*/
    svex.pop_back(); /*从名为svex的向量中移除最后一个元素。这行代码假设u是svex中最近添加的顶点*/
    for (int i = 0; i < n; ++i)
    {
        /*更新与顶点 u 不相邻的顶点数量*/
        if (i != u && !adjN[u].test(i)) /*检查顶点i是否与顶点u不相邻（没有边相连）*/
            --notadj[i];                /*如果顶点i与顶点u不相邻，则将notadj[i]减一。*/
    }
}

/*用于将节点u添加回"活跃"状态*/
inline void addtoS(int u)
{
    inD.set(u);        /*设置一个名为inD的数据结构中的第u位，通常用于标记节点u在删除集合中。*/
    ins[u] = true;     /*将节点u在名为ins的布尔数组中标记为"已插入"或"活跃"状态。*/
    svex.push_back(u); /*将节点u添加到名为svex的向量的末尾，假设u是最近添加的活跃顶点。*/
    for (int i = 0; i < n; ++i)
    {
        if (i != u && !adjN[u].test(i)) /*检查顶点i是否与顶点u不相邻（没有边相连）*/
            ++notadj[i];                /*如果顶点i与顶点u不相邻，则将notadj[i]加一。这可能是用于跟踪与某个顶点不相邻的其他顶点数量的数组*/
    }
}

/*delfrS 函数（Delete from S）的目标是将节点从 "活跃" 状态移动到 "非活跃" 状态。
在这个过程中，它会使与节点 u 不相邻的其他节点更有可能成为 "活跃" 节点。
因此，它会递减与这些节点相关的计数器（例如 notadj 数组），以反映它们更可能成为下一个 "活跃" 节点。

addtoS 函数（Add to S）的目标是将节点从 "非活跃" 状态移动回 "活跃" 状态。
在这个过程中，它会减少其他节点成为 "活跃" 节点的机会。因此，它会递增与这些节点相关的计数器，以反映它们更不可能成为下一个 "活跃" 节点。*/

/*bool canadd(int u) 函数是用于检查是否可以将节点 u 添加到当前的 svex 集合中，而不违反算法的约束条件。*/
inline bool canadd(int u)
{
    int tot = 0; /*初始化一个计数器 tot，用于计算节点 u 与当前 svex 集合中的节点不相邻的数量。*/
    for (auto v : svex)
        if (!adjN[u].test(v)) /*检查节点 u 和节点 v 是否不相邻。如果不相邻，执行以下操作：*/
        {
            if (++tot >= K) /*如果不相邻的节点数量大于等于 K，表示添加节点 u 将导致超过 K 个不相邻的节点存在于 svex 集合中，
            这将违反算法的约束条件，因此返回 false。*/
                return false;
            if (notadj[v] >= K - 1) /*如果节点 v 的不相邻节点数量大于等于 K-1，表示添加节点 u 可能导致节点 v 成为一个不相邻的节点，
            这也将违反算法的约束条件，因此返回 false。*/
                return false;
        }
    svex.push_back(u); /*将节点 u 添加到 svex 集合中*/

    /*调用 vc.solve 函数来解决一个与 svex 集合相关的问题，其中 svex.size() - K 是一个参数。vc.solve 函数返回一个布尔值，表示是否找到了解决方案。*/
    bool ret = vc.solve(true, svex.size() - K);

    svex.pop_back(); /*将节点 u 从 svex 集合中移除。*/
    return ret;      /*最后，返回 ret，它表示是否可以将节点 u 添加到 svex 集合中。*/
}

/*void bfs(int s) 函数是一个广度优先搜索算法，用于计算从节点 s 出发到其他节点的最短距离，并将结果存储在 dis 数组中。*/
void bfs(int s)
{
    memset(dis, -1, n * sizeof(int)); /* 初始化 dis 数组，将所有节点的距离初始化为 -1，表示尚未访问。*/
    dis[s] = 0;                       /*设置节点 s 到自身的距离为 0。*/
    int fr = 0, re = 0;               /*初始化队列的前后指针 fr 和 re。*/
    que[re++] = s;                    /* 将起始节点 s 放入队列*/
    while (fr ^ re)                   /*进行广度优先搜索*/
    {
        int u = que[fr++];                    /*从队列中取出一个节点 u 进行处理。*/
        for (int e = head[u]; ~e; e = nxt[e]) /*对于节点 u 的每个邻接节点 v*/
        {
            int v = to[e];
            if (del[v] || dis[v] != -1) /*如果 v 尚未被访问（dis[v] == -1），则执行以下操作：*/
                continue;
            dis[v] = dis[u] + 1; /*设置节点 v 的距离为节点 u 的距离加 1*/
            que[re++] = v;       /* 将节点 v 放入队列以进行后续处理。*/
        }
    }
}

void check_timeout()
{
    if ((double)(clock() - startTime) / CLOCKS_PER_SEC > TIME_LIM)
        exit_program();
}

int get_color()
{
    BIT_SET Q, P;
    inG.cp(P);   /*表示将 inG（可能是另一个 BIT_SET 对象或类似结构，代表图的某个状态）的内容复制到 P 中。cp 对应于拷贝或复制操作。*/
    int tot = 0; // tot 被初始化为0，用于累计最终结果。
    while (!P.empty())
    {
        /*
        在每次外部循环迭代中，初始化 cnt 为0，然后将 P 的当前状态复制到 Q。
        进入一个 do-while 循环，直到 Q 为空。
        在 do-while 循环中，首先获取 Q 中最低设置位的索引（lowbit 方法）并将其赋值给 u。
        累加 cnt。如果 cnt 超过 K，则将其设置为 K。这里 K 可能是一个上限或阈值。
        通过 set 方法在 P 和 Q 中设置位 u。
        然后更新 Q 为 Q 与 invN[u] 的按位与。invN 可能是一个数组，包含与每个顶点相关的位掩码，用于表示某种特定关系或属性。*/
        int cnt = 0;
        P.cp(Q);
        do
        {
            int u = Q.lowbit();
            ++cnt;
            if (cnt > K)
                cnt = K;
            P.set(u);
            Q.set(u);
            Q &= invN[u];
        } while (!Q.empty());
        tot += cnt;
    }
    return tot;
}

void get_color(vector<pair<int, int>> &lst)
{
    /*初始化 BIT_SET 对象并拷贝状态*/
    BIT_SET Q, P;
    inD.cp(P);
    int tot = 0;
    /*循环处理直至 P 为空*/
    while (!P.empty())
    { /*P.empty() 检查 P 是否没有任何设置的位。*/
        int cnt = 0;
        P.cp(Q);
        /*在 Q 中进行操作并更新 lst*/
        do
        {
            /*在一个 do-while 循环中，Q.lowbit() 获取 Q 中最低的设置位的位置，并赋值给 u。
            cnt 用于计数，如果超过某个阈值 K，则将 cnt 设置为 K。
            向 lst 中添加一对整数 (u, tot + cnt)。这可能表示顶点 u 以及与之相关的某种计数或度量。
            P.set(u); Q.set(u); 将 P 和 Q 中位置 u 的位设置为1。
            Q &= invN[u]; 更新 Q 为 Q 与 invN[u] 的按位与。这可能是用于移除与顶点 u 相关的位或表示某种特定的关系。*/
            int u = Q.lowbit();
            ++cnt;
            if (cnt > K)
                cnt = K;
            lst.emplace_back(make_pair(u, tot + cnt));
            P.set(u);
            Q.set(u);
            Q &= invN[u];
        } while (!Q.empty());
        tot += cnt;
    }
}

void dfs_kbundle(int curS, bool check)
{
    /*基本检查和设置*/
    check_timeout();
    ++dfs_node;
    LB = max(LB, (int)svex.size());
    if (curS <= LB)
        return;
    /*检查条件并尝试解决*/
    if (check && vc.solve(false, curS - K))
    {
        /*如果 check 为真且 vc.solve(false, curS - K) 返回真，则将 LB 设置为 curS 并返回。*/
        LB = curS;
        return;
    }
    /*获取颜色列表并进行检查*/
    vector<pair<int, int>> lst;
    get_color(lst);
    int m = lst.size();
    if (m == 0) /*如果 lst 为空（即 m == 0），则返回。*/
        return;
    check = false;
    /*处理列表中的元素*/
    for (int i = m - 1; i >= 0; --i)
    {
        /*从 lst 的末尾开始反向遍历其元素。
        对每个元素，根据特定条件进行处理，包括添加到某个集合、调用 dfs_kbundle 进行递归搜索，或者从集合中删除。*/
        if ((int)svex.size() + lst[i].second <= LB)
        {
            for (int j = i + 1; j < m; ++j)
                addtoD(lst[j].first);
            return;
        }
        if (canadd(lst[i].first))
        {
            /*在循环中，如果满足某个条件，就将元素添加到 svex 集合（addtoS），进行递归搜索，然后再将其移除（delfrS）。*/
            addtoS(lst[i].first);
            dfs_kbundle(curS, check);
            delfrS(lst[i].first);
        }
        /*更新 curS 并在条件满足时将元素从另一个集合中删除（delfrD）。*/
        --curS;
        check = true;
        delfrD(lst[i].first);
    }
    /*循环结束后，对 lst 中的所有元素执行 addtoD 操作，可能表示将这些元素重新添加回某个数据结构或集合。*/
    for (int i = 0; i < m; ++i)
        addtoD(lst[i].first);
}

void dfs(int curS)
{
    check_timeout(); /*用于检查算法运行是否超时。*/
    ++dfs_node;      // 用于追踪递归调用的次数。
    LB = max(LB, (int)svex.size());
    if (curS <= LB) /*如果 curS（当前解）小于等于 LB，则函数返回，停止进一步搜索。*/
        return;
    /*寻找度数最小的未删除顶点。*/
    int minID = -1;
    for (int i = 0; i < n; ++i)
        if (!del[i])
        {
            if (minID == -1 || degree[i] < degree[minID])
                minID = i;
        }
    /*如果最小度数的顶点的度数大于等于 curS - K，则调用 dfs_kbundle 函数并返回。*/
    if (degree[minID] >= curS - K)
    {
        dfs_kbundle(curS, true);
        return;
    }
    /*这里检查最小度数顶点的度数是否低于特定值，并进行相应的操作，包括递归调用 dfs。*/
    if (degree[minID] < LB + 1 - K)
    {
        if (ins[minID]) /*如果最小度数顶点已经被插入（ins[minID]为真），则直接返回。*/
            return;
        delfrD(minID); /*从当前解集中删除最小度数顶点minID。*/
        dfs(curS - 1); /*递归地调用dfs函数，减少当前解的大小。*/
        addtoD(minID); /* 将最小度数顶点minID重新添加到当前解集中*/
        return;
    }
    /*寻找和处理另一个特定条件的顶点, 这次是寻找未被删除的度数最大的顶点*/
    int maxID = -1; /*初始化一个变量maxID，用于存储度数最大的未删除顶点的编号。*/
    for (int i = 0; i < n; ++i)
        if (!del[i])
        {
            if (maxID == -1 || notadj[i] > notadj[maxID]) /* 如果maxID是初始值或顶点i的未相邻顶点数量大于maxID对应顶点的未相邻顶点数量，*/
                maxID = i;                                /*则更新maxID为i*/
        }
    /*处理 maxID 对应的顶点*/
    if (!ins[maxID] && notadj[maxID] >= K) /*检查顶点maxID是否未被插入（未被标记为已插入）且与 maxID 不相邻的顶点数量大于等于 K*/
    {
        delfrD(maxID); /*从当前解中删除顶点maxID*/
        dfs(curS - 1); /*这里就是很常见的一种思想，先假装把一个顶点给删除掉，然后在深度搜索后又把这个顶点加回来*/
        addtoD(maxID); /*将顶点maxID重新添加到当前解中。*/
        return;
        /*这一段代码的目的是考虑了一种情况，即顶点maxID可以被删除以获得更优解的可能性*/
    }
    if (ins[maxID] && notadj[maxID] >= K - 1) /*检查顶点maxID是否已经被插入（已被标记为已插入）且与顶点maxID不相邻的顶点数量是否大于等于K - 1*/
    {
        /*如果上述两个条件都满足，表示顶点maxID已被插入且与其不相邻的顶点数量足够多，就会执行以下操作*/
        if (notadj[maxID] >= K) /*如果与顶点maxID不相邻的顶点数量大于等于K，则直接返回，不再继续搜索。*/
            return;
        vector<int> todel; /*创建一个空的整数向量vector<int> todel;，用于存储待删除的顶点*/
        for (int i = 0; i < n; ++i)
            if (!del[i] && !ins[i] && !adjN[maxID].test(i)) /*检查顶点i是否未被删除、未被插入，并且与顶点maxID不相邻。*/
            {
                todel.push_back(i); /*如果条件满足，将顶点i添加到待删除的顶点列表中*/
            }
        if (todel.size()) /*如果待删除的顶点列表中有顶点*/
        {
            for (auto x : todel) /*遍历待删除的顶点列表，并依次删除这些顶点*/
                delfrD(x);
            dfs(curS - todel.size()); /*递归地调用dfs函数，减少当前解的大小*/
            for (auto x : todel)
                addtoD(x); /*恢复先前删除的顶点到当前解中*/
            return;
        }
    }
    /*基于 svex 数组的随机选择和处理*/
    if (svex.size()) /*检查向量svex是否非空*/
    {
        /*如果 svex 非空，选择其中的一个随机顶点进行广度优先搜索（BFS），然后基于 BFS 的结果收集一组特定条件下的顶点进行进一步处理。*/
        vector<int> sofar;                  /*存储待处理的顶点*/
        int u = svex[rand() % svex.size()]; /*如果 svex 非空，代码随机选择 svex 中的一个顶点 u。选择是通过 rand() % svex.size() 实现的，
        这会生成一个介于 0 和 svex.size() - 1 之间的随机索引。对选中的顶点 u 执行广度优先搜索 (BFS)。*/
        bfs(u);                             /*执行广度优先搜索*/
        /*下面的循环遍历图中的所有顶点 v，收集那些未被删除且满足特定条件的顶点。
        条件是顶点 v 要么在 BFS 中未被访问过（dis[v] == -1），要么其距离超过了某个阈值（dis[v] > max(2, K + K - LB)）。这个阈值可能与图的某些性质有关。
        如果顶点 v 已经被插入 (ins[v] 为真)，则函数提前返回。*/
        for (int v = 0; v < n; ++v)
            if (!del[v]) /*检查顶点v是否未被删除*/
            {
                if (dis[v] == -1 || dis[v] > max(2, K + K - LB)) /*顶点v在广度优先搜索中未被访问过（dis[v]的值为-1），
                且顶点v的距离超过了某个阈值，这个阈值是max(2, K + K - LB)*/
                {
                    if (ins[v])
                        return;
                    sofar.push_back(v); /*如果上述条件满足，并且顶点v未被插入，则将顶点v添加到sofar向量中：*/
                }
            }
        if (sofar.size()) /*如果sofar向量中有顶点*/
        {
            for (auto x : sofar)
                delfrD(x);            /*遍历sofar向量中的顶点，通过delfrD函数删除这些顶点*/
            dfs(curS - sofar.size()); /*递归地调用dfs函数，更新当前搜索深度curS*/
            for (auto x : sofar)
                addtoD(x); /*通过addtoD函数将这些顶点重新添加到数据结构中*/
            return;
        }
    }
    /*颜色获取与分支处理*/
    if (get_color() <= LB) /*如果颜色值小于等于LB，则函数结束*/
        return;
    /*收集分支处理的顶点*/
    vector<int> branch; /*在这里创建一个整数向量（vector<int>）branch，用于存储待处理的顶点。*/

    for (int x = 0; x < n; ++x)
        if (!del[x] && x != minID && !ins[x]) /*如果顶点x未被删除、顶点x不等于minID且顶点x未被插入*/
        {
            /*如果上述条件都满足，继续检查另一个条件*/
            if (!adjN[minID].test(x)) /*顶点x不与minID相邻。这里使用adjN[minID].test(x)来检查顶点之间是否有边相连。*/
                branch.push_back(x);  /*如果上述条件都满足，将顶点x添加到branch向量中*/
        }
    /*处理 minID 对应的顶点*/
    if (ins[minID]) /*当顶点minID已被插入时*/
    {
        int canselect = K - 1 - notadj[minID], pos = -1;
        /*首先计算canselect，它表示可以选择的顶点数量, 初始化pos为-1，可能用于追踪处理过程中的某个特定位置。*/
        bool all = true; /*all 初始化为 true，可能用于标记是否所有顶点都符合某个条件。*/
        /*对 branch 中的顶点执行操作*/
        for (int i = 0; i < canselect; ++i)
        {
            /*
            循环遍历 branch 中的顶点，但只处理到 canselect 数量的顶点。
            对每个顶点，先从当前结构中删除 (delfrD)，然后检查是否能够添加上一个顶点（canadd(branch[i - 1])）。
            如果不能添加，将当前顶点重新添加 (addtoD)，设置 all 为 false 并跳出循环。
            如果可以添加，将上一个顶点的状态更改为已插入 (addtoS) 并更新 pos。
            每次循环中，递归调用 dfs 函数，更新搜索深度。*/
            delfrD(branch[i]);               /*删除当前顶点*/
            if (i && !canadd(branch[i - 1])) /*检查是否可以添加上一个顶点*/
            {
                addtoD(branch[i]); /*如果不能添加，将当前顶点重新添加*/
                all = false;       /*将all设置为false，并跳出循环。*/
                break;
            }
            if (i)
            {
                addtoS(branch[i - 1]); /*如果可以添加，将上一个顶点的状态更改为已插入*/
                pos = i - 1;           /*并更新pos*/
            }
            dfs(curS - 1);     /*每次循环中，递归调用dfs函数，更新搜索深度*/
            addtoD(branch[i]); /*最后，通过addtoD(branch[i])将当前顶点重新添加到数据结构中*/
        }
        /*后续处理*/
        if (all && (canselect == 0 || canadd(branch[canselect - 1]))) /*如果所有顶点都符合某个条件且要么canselect为0，要么能够添加branch中最后一个顶点*/
        {
            /*
            如果 all 为 true 且满足特定条件，对 branch 中剩余的顶点执行类似的操作，包括删除 (delfrD)、更改状态 (addtoS 和 delfrS)，以及递归调用 dfs。
            循环处理直到 pos 指示的位置，用于撤销先前的状态更改操作。   */
            for (int i = canselect; i < (int)branch.size(); ++i) /*循环从canselect位置开始，遍历branch中的剩余顶点。*/
                delfrD(branch[i]);                               /*删除当前顶点*/
            if (canselect)                                       /*如果canselect不为0*/
                addtoS(branch[canselect - 1]);                   /*将上一个顶点的状态更改为已插入。*/
            dfs(curS - branch.size() + canselect);               /*归调用 dfs 函数，更新搜索深度。*/
            if (canselect)                                       /*如果canselect不为0，*/
                delfrS(branch[canselect - 1]);                   /*撤销上一个状态更改*/
            for (int i = canselect; i < (int)branch.size(); ++i)
                addtoD(branch[i]); /*重新添加当前顶点到数据结构中*/
        }
        for (int i = 0; i <= pos; ++i) /*这个循环从0到pos，用于遍历之前处理的顶点。*/
            delfrS(branch[i]);         /*对每个顶点执行撤销状态更改操作，即将其状态从已插入更改为未插入。*/
    }
    else
    { // 当 minID 未被插入时
        /*这一部分代码处理 minID 未被插入的情况。
        先从当前图结构中删除 minID，递归地调用 dfs 函数，然后将 minID 再次添加回图中。
        如果 minID 不能被添加（!canadd(minID)），则函数提前返回。
        对 branch 中的顶点进行类似的处理。*/
        delfrD(minID);      /*通过调用delfrD函数从当前图结构中删除minID，即将minID标记为已删除。*/
        dfs(curS - 1);      /*接着，递归地调用dfs函数，传入curS - 1作为新的搜索深度*/
        addtoD(minID);      /*将minID再次添加回图中，即将其标记为未删除*/
        if (!canadd(minID)) /*在将minID重新添加后，检查minID是否可以再次添加*/
            return;         /*如果 minID 不能被添加（由 canadd 函数检查），则函数提前返回。这可能是一种优化，避免无效或重复的搜索。*/
        addtoS(minID);      /*如果minID能够再次添加，将其插入当前的解集S中*/
        /*处理 branch 中的其他顶点*/
        int canselect = K - 1 - notadj[minID]; /*这里指的就是一个顶点最多还能容忍的断连顶点数量了*/
        int pos = -1;                          /* 初始化pos为-1，可能用于追踪处理过程中的某个特定位置*/
        bool all = true;                       /*初始化all为true，可能用于标记是否所有顶点都符合某个条件*/
        for (int i = 0; i < canselect; ++i)    /*循环从0到canselect - 1遍历顶点*/
        {
            delfrD(branch[i]);               /*首先，将当前顶点从当前图结构中删除，即标记为已删除*/
            if (i && !canadd(branch[i - 1])) /*对于除第一个顶点外的每个顶点，检查是否能够添加上一个顶点*/
            {
                /*如果不能添加，则将当前顶点重新添加到图中，将all设置为false，并跳出循环*/
                addtoD(branch[i]);
                all = false;
                break;
            }
            if (i)
            { /*对于除第一个顶点外的每个顶点，将上一个顶点的状态更改为已插入，并更新pos*/
                addtoS(branch[i - 1]);
                pos = i - 1;
            }
            dfs(curS - 1);     /*在每次循环中，递归地调用dfs函数，更新搜索深度，相当于在假设这些顶点都已插入的情况下继续搜索。*/
            addtoD(branch[i]); /*最后，将当前顶点重新添加到图中，即标记为未删除。*/
        }
        /*如果 all 为真并满足特定条件，则对 branch 中剩余的顶点执行删除和添加操作，并递归调用 dfs。
        循环处理直到 pos 指示的位置，撤销先前的状态更改操作。
        最后，将 minID 的状态从已插入更改回未插入。*/
        if (all && (canselect == 0 || canadd(branch[canselect - 1]))) /*all为真表示所有的branch中的顶点都符合某个条件，并且
        要么没有可选择的顶点，要么最后一个可选择的顶点可以被添加*/
        {
            for (int i = canselect; i < (int)branch.size(); ++i) /*遍历branch中剩余的顶点，即从canselect开始到branch.size() - 1*/
                delfrD(branch[i]);                               /* 对于每个剩余的顶点，将其从当前图结构中删除，即标记为已删除*/
            if (canselect)                                       /*如果canselect大于0，即还有可选择的顶点*/
                addtoS(branch[canselect - 1]);                   /*将最后一个可选择的顶点的状态更改为已插入，即将其插入到当前解集S中*/
            dfs(curS - branch.size() + canselect);               /*递归地调用dfs函数，传入新的搜索深度，该深度减去已处理的branch顶点数，再加上可选择的顶点数canselect*/
            if (canselect)                                       /*如果canselect大于0，即还有可选择的顶点*/
                delfrS(branch[canselect - 1]);                   /*将最后一个可选择的顶点的状态从已插入更改回未插入，即将其从解集S中删除*/
            for (int i = canselect; i < (int)branch.size(); ++i) /*对于每个剩余的顶点*/
                addtoD(branch[i]);                               /*将其添加回当前图结构中，即标记为未删除*/
        }
        for (int i = 0; i <= pos; ++i) /*循环遍历branch中已处理的顶点，*/
            delfrS(branch[i]);         /*将它们的状态从已插入更改回未插入，即将它们从解集S中删除*/
        delfrS(minID);                 /*最后，将minID的状态从已插入更改回未插入，即将minID从解集S中删除。*/
    }
}

void work()
{
    /*初始化随机数生成器的种子为当前时间。这通常用于确保每次程序运行时产生的随机数序列都是不同的。*/
    srand(time(NULL));

    /*记录函数开始执行时的处理器时钟，这用于计算程序执行的时间。*/
    startTime = clock();

    /*初始化dfs_node变量，这可能用于跟踪在深度优先搜索（DFS）中访问的节点数量。*/
    dfs_node = 0;

    /*设置变量LB（可能代表下界 Lower Bound）为变量wG和K的最大值。*/
    LB = max(wG, K);

    /*初始化一个名为twoPow的数组。该数组的索引表示2的幂，而数组中的元素表示对应的指数。
    例如，twoPow[1]将被设置为0，twoPow[2]将被设置为1，twoPow[4]将被设置为2，以此类推。
    这个映射关系通常用于处理二进制位操作。这个循环将twoPow数组初始化为一个指数映射表。*/
    /*twoPow这个数组的主要目的是让我们快速的找出某个数是2的几次方。*/
    for (int i = 0; i < 16; ++i)
        twoPow[1 << i] = i;
    /*具体来说，这段代码的作用是初始化一个数组 twoPow，该数组的索引表示 2 的幂，而数组中的元素表示对应的指数。
    换句话说，它建立了一个映射关系，用于将 2 的幂映射到对应的指数值。例如，twoPow[1] 将被设置为 0，
    twoPow[2] 将被设置为 1，twoPow[4] 将被设置为 2，以此类推。
    这种映射通常用于一些算法中，特别是在处理二进制位操作时。例如，如果你想知道某个数是 2 的多少次方，你可以使用这个数组来查找。
    例如，如果 twoPow[16] 的值为 4，那么你可以知道 2^4 = 16。这对于一些位操作和计算中非常有用。 */

    /*调用PreProcess函数，并传递LB + 1作为参数。*/
    if (!PreProcess(LB + 1)) /*如果PreProcess返回false（可能表示预处理失败），则调用exit_program函数退出程序。*/
        exit_program();
    getOrd();       /*获取顶点顺序或排序。*/
    dfs(n);         /*执行深度优先搜索函数，参数n表示图中的节点数量*/
    exit_program(); /*用于退出程序*/
}

void MAIN()
{
    /*初始化一个整数p为字符串filename的长度减一。目的是找到文件名中的最后一个字符，通常是文件扩展名的一部分。*/
    int p = strlen(filename) - 1;
    while (filename[p] != '.') /*这个循环递减p直到找到'.'字符，表示文件扩展名的开始。*/
        --p;
    ++p; /*找到'.'后，p增加，指向文件扩展名的开始。*/
    /*程序检查文件扩展名，并根据扩展名调用不同的函数。*/
    if (strcmp(filename + p, "clq") == 0) /*如果扩展名是"clq"，它调用read_clq()；*/
        read_clq();
    else if (strcmp(filename + p, "graph") == 0) /*如果是"graph"，调用read_graph()；*/
        read_graph();
    else /*否则，调用read_txt()。*/
        read_txt();
    work();
}

int main(int argc, char **argv)
{
    assert(argc == 4);                         /*程序断言传递给它的参数正好是四个，包括程序名本身。*/
    strcpy(filename, argv[1]);                 /*这将第二个参数复制到filename变量中，表示要处理的文件名。*/
    if (freopen(filename, "r", stdin) == NULL) /*试用filename指定的文件重新打开标准输入流（stdin）。如果失败，打印错误信息并退出程序。*/
    {
        printf("Open %s error.\n", filename);
        return 0;
    }
    /*这些行从命令行参数中读取整数值，并将它们存储在变量K和wG中。*/
    sscanf(argv[2], "%d", &K);  /*读取s-bundle中的s值*/
    sscanf(argv[3], "%d", &wG); /*读取下界*/
    MAIN();
    return 0;
}
