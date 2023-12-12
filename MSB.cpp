#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int TIME_LIM = 3600;

int N, M, K, wG;
pair<int, int> *E;

// gets() is deprecated
#define gets(str) (fgets(str, 200, stdin))

void read_clq() /*clq是一种二进制的数据库文件。*/
{ /*我没有想到这些代码里面居然还包括了去除重复边！*/ 
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

void read_graph() /*这个函数是一种通用的读取数据的方式。*/
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

/*find_id的作用是实现一个简单的顶点标识到内部标识的映射。如果顶点标识尚未映射，那么它将会被映射到一个新的、唯一的内部标识。这个内部标识始终是唯一的，并且随着映射的进行而增加。*/
map<int, int> mp;
inline int find_id(int u)
{
    auto it = mp.find(u); /*函数通过调用mp.find(u)来查找键为u的元素是否存在于mp中。find函数返回一个指向该元素的迭代器。如果u已经在mp中，那么这个迭代器会指向该元素，否则它将会指向mp.end()。*/
    if (it != mp.end()) /*如果顶点u已经在mp中*/
        return it->second; /*，那么函数就直接返回这个键对应的值，这就是映射的结果。*/
    /*如果键u不在mp中，那么函数首先获取mp的大小（即当前已经映射的键的数量），然后使用这个大小作为新的键u的值添加到mp中。这样，即使之前没有映射过u，现在也已经映射了，并且新的映射键值对中的值是唯一的，它等于当前映射的键的数量。*/
    int sz = mp.size();
    return mp[u] = sz; /*最后，函数返回这个新的映射值。从下一次调用开始，对于同一个键u，它将返回相同的结果。*/
}

/*这段代码的主要功能是从文本文件中读取图的信息，包括顶点数和边数，然后解析每条边的信息，将有效的边存储在E数组中。同时，它还处理自环边和重复边，并且在需要时对顶点进行重新编号。
注意，我已经按照我想要的数据格式进行了相应的读取修改，把一些不需要的跨行注释掉了。*/
void read_txt()
{
    char str[200]; /*声明一个字符数组str，用于临时存储从文件中读取的文本行。这里之所以声明一个200的大小是因为对于我们所读取的文件中，每一行顶多是一条边的两个顶点的编号，
    而一个200字符串的数组已经足够把这些信息存下去了。*/

    /*读取文本文件的一行内容，通常用于跳过文件中的注释或无关的信息。下面调用了两次，可能用于跳过前两行不相关的文本。对于我来说没用，我已经注释掉了。*/
    // gets(str);
    // gets(str);
    /*使用scanf函数从文本文件中读取格式化的数据。它会读取两个字符串（通常是标签或标识符），然后读取两个整数，分别存储为N和M。这里假定N是顶点数，M是边数。*/
    // scanf("%s%s%d%s%d", str, str, &N, str, &M);
    scanf("%d %d", &N, &M); /*分别读取顶点数目和边的数目。*/
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
        /*如果没有自环，那么就通过find_id方法检查是否有重复边，该函数是基于map实现的。*/
        u = find_id(u);
        v = find_id(v);
        if (u > v) /*确保u小于等于v，因为无向图中的边可以以两种顺序表示。这样有助于避免重复边。*/
            swap(u, v); /*交换的目的是把边按照第一个顶点序号较小，而第二个顶点序号较大的方式来存储，减少重复存储。*/
        if (st.count(make_pair(u, v))) /*检查(u, v)和(v, u)是否已经存在于集合st中，如果存在，说明这条边已经添加过了，跳过这条边。*/
            continue;
        st.insert(make_pair(u, v)); /*将当前边的信息(u, v)添加到集合st中，以便后续检查重复边。*/
        assert(m < M);              /*使用assert宏来确保m小于M，即有效边的数量没有超过预分配的数组大小。*/
        E[m++] = make_pair(u, v);   /*将当前边的信息(u, v)存储在数组E中，并增加有效边的计数器m。*/
    }
    M = m;      /* 更新边的数量M为有效边的数量m*/
    st.clear(); /* 清空集合st，以便后面在读取图文件的时候工作。*/
    mp.clear(); /*清空可能用于重新编号顶点的mp容器。*/
}

int twoPow[1 << 16], msk = (1 << 16) - 1;
/*
twoPow 是一个整数数组。1 << 16 是一个位移操作，意味着数字 1 向左移动 16 位。在大多数现代架构中，int 是32位，这意味着 1 << 16 将会是 65536（即 2 的 16 次方）。
因此，twoPow 被定义为一个包含 65536 个整数的数组。

msk 是一个整数变量。同样地，1 << 16 产生了 65536。然后从 65536 减去 1，得到 65535。所以 msk 被初始化为 65535，它的二进制表示是 16 个 1（即 1111111111111111）。

msk 可以用来快速获取一个数的低 16 位。当你将一个整数与 msk 进行按位与操作时，只有该整数的低16位参与计算，因为 msk 的高16位都是0（在32位整数中）。
这样，只有原始数的低16位能够“通过”掩码；高位由于与0进行了运算，因此在结果中都变为0。

具体来说，这段代码的作用是初始化一个数组 twoPow，该数组的索引表示 2 的幂，而数组中的元素表示对应的指数。换句话说，它建立了一个映射关系，用于将 2 的幂映射到对应的指数值。例如，twoPow[1] 将被设置为 0，
twoPow[2] 将被设置为 1，twoPow[4] 将被设置为 2，以此类推。这种映射通常用于一些算法中，特别是在处理二进制位操作时。例如，如果你想知道某个数是 2 的多少次方，你可以使用这个数组来查找。
例如，如果 twoPow[16] 的值为 4，那么你可以知道 2^4 = 16。这对于一些位操作和计算中非常有用。 */

struct BIT_SET /*位图运算，可以快速的判断出一个顶点的邻居和非邻居。*/
{
    int n, m; /*n表示顶点数量，m表示边的数量。*/
    unsigned *buf; /*这个数组在该位图的init的时候被设置了。*/
    BIT_SET() /*默认的构造函数。*/
    {
        n = m = 0;
        buf = nullptr;
    }
    ~BIT_SET()
    {
        if (buf != nullptr)
            delete[] buf;
    }
    void init(int _n) /*初始化一个位图数据结构，根据输入的位数 _n，确定位图需要多少个 unsigned 整数来存储，然后分配相应的内存空间，并将位图的数据初始化为全 0.这样子的分配效率的确高多了！*/
    {   /*这种位图通常用于位运算和集合操作，可以高效地表示和处理大量的布尔值信息。*/ 
        m = _n & 31;/*这行代码计算 _n 的低 5 位，它使用了位运算 &（按位与），31 的二进制表示为 11111，所以 & 操作可以提取 _n 的低 5 位，结果赋值给变量 m。这个操作的目的是确定每个 unsigned 整数中的位数，通常是 32 位。*/  
        n = _n >> 5;/*这行代码计算 _n 的高位，也就是除以 32 后的商，用右移运算符 >> 实现，将结果赋值给变量 n。这个操作的目的是确定需要多少个 unsigned 整数来表示 _n 位的位图。*/ 
        buf = new unsigned[n + 1];
        for (int i = 0; i <= n; ++i)
            buf[i] = 0; /*在循环中，将 buf 数组中的每个元素初始化为 0，以确保位图的初始状态是所有位都为 0。*/
    }
    void flip() /*对位图中的每个 unsigned 整数执行按位取反操作，*/
    {
        for (int i = 0; i < n; ++i)
            buf[i] = ~buf[i]; /*对位图中的每个 unsigned 整数执行按位取反操作，即将所有的 0 变为 1，将所有的 1 变为 0。这一步实现了位图中每个位的反转。*/
        buf[n] ^= ((unsigned)1 << m) - 1;
        /*这行代码执行了一个异或操作，用于反转最后一个 unsigned 整数中的高位。((unsigned)1 << m) - 1：这部分代码计算了一个值，它是将 1 左移 m 位，然后减去 1。
        这个操作实际上生成了一个二进制数，其中低 m 位为 1，高位为 0。例如，如果 m 等于 5，那么 (1 << m) - 1 的二进制表示为 0000011111。^：这是异或运算符. */

        /*反转最后一个 unsigned 整数中的高位是为了确保位图的长度（或位数）是精确的，这在位图操作中非常重要。位图通常用于表示一组元素的存在与否，其中每个位对应一个元素。
        在某些情况下，位图的位数可能不是 32 的整数倍，因此最后一个 unsigned 整数可能只部分用于存储位信息。这就需要考虑如何正确处理这些额外的位。*/
    }
    void set(int x) /*在特定位置设置一个位。*/
    {
        buf[x >> 5] ^= (unsigned)1 << (x & 31);
        /*x >> 5 是一个位运算操作，将整数 x 右移5位，相当于将 x 除以32，这是因为每个无符号整数通常有32位（也就是4个字节）。这一步用于定位到存储位 x 的无符号整数。
        (x & 31)：这是位与运算操作，用于获取 x 的最低5位的值，因为 31 的二进制表示是 11111，它只有最低5位是1，其余位是0。
        (unsigned)1 << (x & 31)：将整数1左移5位，得到一个只有第5位是1的无符号整数。这是一种位掩码，用于在 buf[x >> 5] 中设置指定位置的位。5=4+1，则4则是无符合整数的字节大小。
        */
    }
    bool test(int x) /*检查位集合中在位置x的位是否设置，这个程序中用来表示一个顶点是否存在于某个图的集合中。*/
    {   /*这个 test 函数通过位操作检查位集合中特定位置 x 的位是否设置。如果该位被设置（即为 1），则函数返回 true；如果未被设置（即为 0），则返回 false。*/
        return buf[x >> 5] >> (x & 31) & 1;
        /*& 1：最后，我们使用位与操作 &，将上一步的结果与 1 进行与运算。这样可以屏蔽掉除了最低位以外的其他位。如果最低位是 1，
        这个表达式的结果就是 1（即 true），如果最低位是 0，结果就是 0（即 false）。*/
    }
    int lb(unsigned x) /*根据 x 的值在 twoPow 中查找相应的结果。*/  /*lb 函数根据 x 的值在 twoPow 中查找相应的结果，并根据 x 的哪一部分（高16位或低16位）包含有效位来调整这个结果。*/
    {
        if (x & msk)   /*它检查 x 的低16位是否有任何位被设置。*/
            return twoPow[x & msk]; /*如果 x 的低16位中有设置的位，函数返回 twoPow 数组中相应索引处的值。*/
        return twoPow[x >> 16 & msk] + 16;
    }
    int lowbit() /*此函数的目的是查找并返回集合中从右往左数第一个非零位的位置。它使用了位运算来高效地完成这项任务。*/
    {
        for (int i = 0; i <= n; ++i)
        {
            unsigned x = buf[i] & ((~buf[i]) + 1); /*这行代码使用位运算找到 buf[i] 中最低位的非零位。
            ~buf[i]：首先对 buf[i] 进行按位取反操作。
            (~buf[i]) + 1：然后对结果加 1。这实际上是计算 buf[i] 的二进制补码。
            buf[i] & ((~buf[i]) + 1)：最后，原始值和它的补码进行按位与运算。这个运算的结果是保留了 buf[i] 中最右边的那个 1，而将其他所有位都置为 0*/
            if (x) /*如果 x 不为零，表示在当前的 buf[i] 中找到了非零位。*/
                return lb(x) + (i << 5);
        }
        return -1;
    }
    bool empty() /*用来高效的判断一个位图集合是否为空。*/
    {
        for (int i = 0; i <= n; ++i)
            if (buf[i])
                return false;
        return true;
    }
    void operator&=(const BIT_SET &rhs) /*这是 BIT_SET 类型的按位与赋值操作符的重载。该操作符接受一个常量引用参数 rhs，表示右侧的 BIT_SET 对象。*/
    {
        for (int i = 0; i <= n; ++i)
            this->buf[i] &= rhs.buf[i];
        /*这个函数通常用于将两个位集合进行逻辑与操作，可能用在诸如集合交集之类的场合，只有当两个 BIT_SET 对象中相同位置的位都被设置时，结果 BIT_SET 对象在这个位置的位才会被设置。*/
    }
    void cp(BIT_SET &rhs) const
    {  /*这个 cp 函数实际上实现了一个深拷贝操作，因为它不仅仅复制了成员变量的值，还复制了指向动态分配数组的指针所指向的数据。这是在处理涉及动态内存分配的类时常见的做法，
        以确保副本拥有自己的独立数据副本，防止潜在的悬挂指针或双重释放等问题。*/
        rhs.n = n;
        rhs.m = m;
        if (rhs.buf == NULL)
            rhs.buf = new unsigned[n + 1];
        for (int i = 0; i <= n; ++i)
            rhs.buf[i] = buf[i];
    }
} *adjN, *invN, inD, inG; /*adjN表示基于位图表示的邻接矩阵，invN表示非邻居矩阵，inD表示在候选集中的顶点集合，inG表示在当前图中的顶点集合。*/
/*这里声明了 adjN 和 invN 作为 BIT_SET 类型的指针。在 C++ 中，指针是一个变量，它存储了另一个变量的内存地址。这里，adjN 和 invN 是指向 BIT_SET 类型对象的指针。通常这种声明用于
指向动态分配的 BIT_SET 对象或对象数组。它们可能会在程序的其他部分使用 new 关键字进行内存分配。inD 和 inG 是 BIT_SET 类型的实例。与 adjN 和 invN 不同，它们不是指针，而是实际的对象。
这种声明表示在程序中直接创建了两个 BIT_SET 类型的对象，它们将拥有 BIT_SET 结构体中定义的所有属性和行为。这种声明方式允许在程序中灵活使用 BIT_SET 结构体。通过指针，可以管理动态分配
的内存，这在处理大量数据或在运行时确定数据大小时非常有用。而直接声明的对象则用于那些大小已知且生命周期与声明它们的作用域相关的情况。在实际应用中，adjN 和 invN 可能用于存储图的邻接
信息，比如一个图中顶点的邻接情况，而 inD 和 inG 可能表示图中顶点的某种状态或属性。具体的应用依赖于程序的其余部分以及这些变量是如何被使用的*/

int *head, *nxt, *to, etot; /*to数组是存储一条边的另外一个顶点；etot是一个全局变量，用于追踪邻接表中边的总数。*/
int *degree, *que, *nV, n;
bool *outcore; /*用来表示顶点度数不符合要求的标记*/
bool *del, *ins; /*ins数组用来表示一个顶点是不是位于我们的解集顶点集合之中。*/
int *notadj;
int *up_bound, *ordID; /*这里的up_bound其实就是退化排序过程中每个顶点的core值。*/
int *dis; /*距离数组，用来存放某个顶点和其他所有顶点的距离。该数组在bfs（广度优先搜索）函数里面被初始化。*/

const int INF = 0x3f3f3f3f;
class MaximumFlow /*为了找出一个s-bundle，需要借助于最大流算法，因为根据最大流最小割定理，一个图的最小割在数值上是和最大流相等的。*/
{
private:
    struct Node /*定义了一个node结构体。*/
    {
        int from, to, next;
        int cap;
    } *edge; /*创建了一个指向 Node 类型的指针，名为edge*/
    int *cap_backup;
    int tol;
    int *head;
    int *dep;
    int *gap, *que;
    int *cur;
    int *S;
    int n;
    void BFS(int start, int end) /*定义了一个用于最大流算法里面的广度优先搜索算法。这个算法的核心思想是找到一条从源点到汇点的最短路径，这条路径被称为“增广路径”，
    并且在这条路径上可以增加最大的流量。通过遍历所有的邻接节点并更新它们的路径长度，我们可以找到从源点到汇点的最短路径。*/
    {   /*其实这个算法的思想非常简单：就是先把汇点的路径长度设置为0（因为目标就是到达汇点，而汇点自己到自己肯定是0.。），然后找汇点的所有一跳邻居，然后路径+1；然后
    找1条邻居的邻居，路径继续+1（其实就是广度优先搜索的思想）；直到遍历完所有的顶点。*/
        memset(dep, -1, n * sizeof(int)); /*dep用于存储从源点到每个点的最短路径长度，初始化为-1表示还没有计算路径长度。*/
        memset(gap, 0, n * sizeof(int)); /*gap用于存储到达每个点的增广路径的数量，初始化为0。*/
        gap[0] = 1; /*由于从源点出发的第一条边必然存在于增广路径中（除非存在负权环），将0号位置设置为1。*/
        int front, rear; /*初始化队列的前端和后端指针*/
        front = rear = 0;
        dep[end] = 0; /*为汇点设置路径长度为0*/
        que[rear++] = end; /*将汇点添加到队列中*/
        while (front != rear) /*持续进行，直到队列为空，遍历完所有的顶点。*/
        {
            int u = que[front++]; /*首先取出队列前端的一个节点u*/
            for (int i = head[u]; i != -1; i = edge[i].next) /*对于顶点u，遍历它的所有邻居顶点v*/
            {
                int v = edge[i].to; /*取出其中一个邻居顶点，命名为v。*/
                if (dep[v] != -1) /*如果 v 已经有了路径长度（即 dep[v] != -1）*/
                    continue; /*则跳过此节点*/
                que[rear++] = v; /*否则，将 v 添加到队列尾部*/
                dep[v] = dep[u] + 1; /*并将v的路径长度设为u的路径长度加1*/
                ++gap[dep[v]]; /*对于每个新的路径长度 dep[v]，我们将相应的 gap[dep[v]] 值加1*/
            }
        }
        /*对于广度优先搜索，我们需要注意的是：对于非连通图，广度优先搜索可以访问所有与起始节点直接或间接相连的节点。但是，如果图是完全非连通的，
        即没有任何路径可以从源节点到达某些节点，那么这些节点将不会被访问。总的来说，广度优先搜索可以用于遍历非连通图，但需要注意它可能无法访问到所有节点。*/
    }

public:
    /*reserve 方法是用于分配足够空间来存储图的边和顶点信息，这里专门用于最大流算法定制的reserve方法。*/
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

    void init(int _n) /*init 方法初始化网络流的图结构*/
    {
        tol = 0;   /*tol 用于追踪边的数量的计数器*/
        n = _n + 2;  /*n 被设置为 _n + 2，这里的+2是为了包括源点和汇点。*/
        memset(head, -1, n * sizeof(int)); /*head 数组被初始化为 -1，这可能表示每个顶点的邻接链表开始之前没有边。*/
    }

    /*addedge 方法用于向基于最大流的图中添加边。*/
    void addedge(int u, int v, int w) /*三个参数中，顶点u和v分别是两个不同的顶点，而w则表示边的容量。*/
    {
        /*它创建了两条边：从 u 到 v 有容量 w，从 v 到 u 的容量为 0。这通常是网络流中的正向边和反向边。每次添加边时，tol 都会递增，用于记录下一个边的索引。
        这里使用了邻接链表的数据结构，head 数组记录了每个顶点的第一条出边，而 next 属性用于指向下一条出边。这样的结构使得可以快速地迭代顶点的所有出边。
        cap_backup 用于存储边的初始容量，这在算法中可能用于复原图的初始状态。需要注意的是，to表示的是正向边，即从源点到汇点的方向；而from则表示的是反向边，表示
        从汇点到源点的方向。*/

        /*第一条边，正向边。*/
        edge[tol].from = u;
        edge[tol].to = v;
        edge[tol].cap = w;
        cap_backup[tol] = w;
        edge[tol].next = head[u];
        head[u] = tol++;

        /*第二条边，反向边。*/
        edge[tol].from = v;
        edge[tol].to = u;
        edge[tol].cap = 0;
        cap_backup[tol] = 0;
        edge[tol].next = head[v];
        head[v] = tol++;
    }

    /*这段代码实现了网络流中的SAP（Shortest Augmenting Path）算法，用于计算从起点start到终点end的最大流。代码中使用了很多网络流算法的标准技术，
    例如前向边和反向边、层次图（level graph）、以及GAP优化。该算法会返回一个int类型res，表示最大流的结果，也就是最小割的数值。注意了！！在求解具有连通度属性的max s-bundle问题的
    时候，最重要的一个定理依据就是“最大流最小割”定理！也就是说s-bundle是想找出来最小割的大小，然而我们要是直接求解最小割的话会非常的不方便，所以说我们就求解了最大流，
    因为最大流问题有非常成熟的方法来进行求解。该算法采用的是Ford-Fulkerson算法和SAP（Shortest Augmenting Path，最短增广路径）算法的结合。
    关于Ford-Fulkerson算法的详细说明：第一步：构建一个残差图，根据图中边的容量来初始化所有的残差。
    第二步：如果该图中存在着增广路径，则：
    1）在残差图中寻找一条增广路径；
    2）寻找该条增广路径上的瓶颈容量x；
    3）更新剩余的残差；即（residual = residual - x);
    4）增加一条反向路径，在该路径上，所有的边的权重都是x。
    不过需要特别注意的一点是！！！下面的SAP算法的实现不是基于所谓的Ford-Fulkerson算法和Emonds-card算法，而是更加高效的dinic's算法，因为有层级的概念！！
    所谓的Dinic's算法的具体步骤是：
    1）首先构建残差图residual graph，该残差图是原图的一份拷贝。
    2）不断重复下述过程：
    a. 构建残差图的层次图level graph。
    b. 找到该层次图上的一个阻塞流blocking flow。
    c. 更新残差图（更新权重、移除饱和边、并且增加反向边。）*/
    int SAP(int start, int end) 
    {
        /* 恢复每条边的初始容量，为新的流计算做准备*/
        for (int i = 0; i < tol; ++i)
            edge[i].cap = cap_backup[i];
        int res = 0; /*初始化结果变量res为0，这个变量用来存储最大流的流量*/
        BFS(start, end); /*使用BFS（广度优先搜索）构建一个层次图，里面包含了每个顶点到达汇点的最短路径长度和增广路径数量。*/
        /*调用了BFS函数后，我们可以得到两个非常关键的数组：（1）dep用于存储从源点到每个点的最短路径长度；（2）gap用于存储到达每个点的增广路径的数量。*/
        int top = 0; /*记录当前遍历到的边*/
        memcpy(cur, head, n * sizeof(int)); /*复制head数组到cur数组。*/
        int u = start; /*初始化u为源节点*/
        int i;
        while (dep[start] < n) /*当源节点的最短路径小于n时，继续循环*/
        {                      
            if (u == end)/*如果到达终点，尝试更新流*/
            {
                /*找到增广路径中容量最小的边*/
                int temp = INF + 1; /*使用了一个辅助数组temp来记录当前最小容量*/
                int inser = 0; /*inser记录最小容量的边的索引。*/
                for (i = 0; i < top; ++i) /*遍历当前增广路径上的所有边。*/
                    if (temp > edge[S[i]].cap) /*如果当前边的容量小于temp*/
                    {
                        temp = edge[S[i]].cap; /*更新temp和inser*/
                        inser = i;
                    }
                /*更新路径上所有边的容量*/
                for (i = 0; i < top; ++i)
                {
                    edge[S[i]].cap -= temp; /*更新当前边的容量*/
                    edge[S[i] ^ 1].cap += temp; /*更新反向边的容量。这里使用了异或运算符，是因为在有向图中，一个边的两个方向是相反的。*/
                }
                res += temp; /*增加结果，即增加最大流的流量。*/
                top = inser;/*回退到增广路径上容量最小边的起点*/
                u = edge[S[top]].from; /*更新当前节点*/
            }
            if (u != end && gap[dep[u] - 1] == 0) /*如果找不到增广路径，且GAP优化条件成立（即深度小于n的节点中已经没有可以增广的边），退出循环。*/
                break;
            for (i = cur[u]; i != -1; i = edge[i].next)  /*查找当前节点u指向的下一个合适的边*/
                if (edge[i].cap != 0 && dep[u] == dep[edge[i].to] + 1) /*如果当前边有容量并且指向的节点与当前节点在同一层，则退出循环。*/
                    break;

            /*如果找到了合适的边（即前面的for循环中没有出现异常情况break掉），前进到该边的终点*/
            if (i != -1)
            {
                cur[u] = i; /*更新当前节点记录的边*/
                S[top++] = i; /*将当前边加入到增广路径中。*/
                u = edge[i].to; /*前进到下一个节点。*/
            }
            else /*如果当前节点没有合适的边，则退回到前一个节点，并更新层次*/
            {
                int min = n; /*这个变量用于存储当前节点可以回退到的最早的层次。*/
                for (i = head[u]; i != -1; i = edge[i].next) /*遍历当前节点u的所有后继节点。*/
                {
                    if (edge[i].cap == 0) /*如果当前边的容量为0，则跳过后续代码，继续遍历下一个后继节点。*/
                        continue;
                    if (min > dep[edge[i].to]) /*如果当前前继节点的目标深度大于min，则更新min*/
                    { 
                        min = dep[edge[i].to]; /*更新min为当前前继节点的目标深度*/
                        cur[u] = i; /*更新当前节点u记录的边为当前前继节点。*/
                    }
                }
                --gap[dep[u]]; /*减少当前节点所在层次在GAP数组中的计数，因为我们在回溯。*/
                dep[u] = min + 1; /*更新当前节点的深度为min+1，这是回溯的体现。*/
                ++gap[dep[u]]; /*增加新深度在GAP数组中的计数，因为我们又前进了一步。*/
                /*如果不是起点，则回退*/
                if (u != start) /* 如果当前节点不是源节点。*/
                    u = edge[S[--top]].from; /*回退到源节点，这是通过从增广路径中移除最后一个边并回退到源节点实现的。*/
            }
        }
        return res; /*返回最大流的结果*/
    }
} mf;

vector<int> svex; /*存储最后获得的s-bundle的顶点集合*/
class VertexConnectivity
{
private:
    /*两个私有成员是指针，用于动态分配数组*/
    int *nV, *oID; /*nV用于存储顶点的新编号，oID用于存储顶点的原始编号。*/

public:
    /*为 nV 和 oID 数组分配内存空间*/
    void reserve(int n)
    {
        n += 3;
        nV = new int[n];
        oID = new int[n];
    }

    /*solve函数是整个求解max s-bundle中的核心函数！！！它主要用于计算顶点连通性以及图的最小割，或者说至少移除多少个顶点会使得当前的图不连通，这是非常重要的一个函数功能。*/
    /*该函数有两种不同的模式，取决于参数 cals 的值。（1）如果 cals 为真，函数用于检查子图 svex 的连通性。（2）如果 cals 为假，函数用于检查整个图的全局连通性。*/
    bool solve(bool cals, int bound) /*传入的两个参数中，第一个参数是是否计算当前解集svex的连通性；第二个参数是当前的下界bound。经过漫长的试错，我终于搞懂了这个bound的具体作用！
    由于我们求解的是max s-bundle问题，而s-bundle指的是最少移除|V|-s个顶点后图不连通，而这个bound就是指的|V|-s，所以和s-bundle中的s相关的参数就体现在solve函数的bound参数中。*/
    {
        int nn = 0; /*定义并初始化一个局部变量 nn，用于跟踪数组 nV 和 oID 的填充情况。*/
        if (cals)   /*如果 cals 为真，则计算当前解集svex的连通性*/
        {
            for (auto u : svex)
                nV[u] = nn, oID[nn++] = u;        /*，并在 nV 中设置新的编号，同时在 oID 中保持其原始编号。这里类似于常老师写的代码。*/
            mf.init(nn + nn);                     /*初始化 mf(max flow)对象，初始化网络流的图结构，参数 nn + nn 表示在一个流网络中的双向边。*/
            for (auto u : svex)                   /*遍历 svex 中的每个顶点，*/
                mf.addedge(nV[u], nV[u] + nn, 1); /*这里是添加正向边，添加从顶点到其对应超级汇点的边，边的容量设置为 1。*/
            for (auto u : svex)
            {
                for (auto v : svex)
                {
                    if (!adjN[u].test(v)) /*如果顶点u和顶点v不相邻，则跳过这一次循环。*/
                        continue;
                    mf.addedge(nV[u] + nn, nV[v], INF); /*这里添加反向边，为所有相邻顶点对添加无限大容量的边。*/
                }
            }
            int u = nV[svex.back()];     /*选取 svex 向量中最后一个顶点的新编号 u。back方法返回一个指向std::vector中最后一个元素的迭代器*/
            for (int i = 0; i < nn; ++i) /*对于 nn 范围内的每个顶点 i，则运行 SAP 算法计算从超级源点 u + nn 到 i 的最大流。*/
            {
                if (i == u || adjN[oID[u]].test(oID[i])) /*如果该顶点是顶点 u自身，或者u与i邻接*/
                    continue; /*则跳过这一次循环。*/
                if (mf.SAP(u + nn, i) < bound) /*如果结果小于 bound，则返回 false，表示无法达到给定的顶点连通性。注意，这里调用了SAP算法。u+nn表示反向流量。*/
                    return false;
            }
            return true; /*如果所有测试都通过，返回 true，表示图至少具有bound的顶点连通性。*/
        }
        else /*整个 else 分支的目的是检查图的全局顶点连通性，而不是 svex 子图的连通性。 通过对所有未删除顶点进行网络流计算，它验证图是否在移除任意两个不相邻顶点后仍然连通。
             如果在任何情况下连通性小于 bound，则图的顶点连通性认为是小于 bound。*/
        {
            for (int i = 0; i < n; ++i)
                if (!del[i])                          /*如果顶点 i 没有被删除（即 del[i] 为假）*/
                    nV[i] = nn, oID[nn++] = i;        /*则将 i 存储在 nV 中，并且在 oID 中以新的编号 nn 存储原始编号 i，nn 递增。*/
            mf.init(nn + nn);   /*初始化网络流对象 mf，节点数量为 nn + nn，这包括了为每个顶点添加的超级源点和超级汇点。*/
            /*注意，初始化的时候之所以变成nn+nn，是因为需要添加两个方向的流！然后下面的代码中也刚好体现了这一点！！！
            首先，在正向的流中，边的容量正常设置为1；其次，在反方向的流中，边的容量被设置成无穷大！这是为了运行最大流算法做铺垫。*/
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
            /*现在已经完成了对正反两个方向的边的初始化和添加。*/
            /*接下来使用两层循环遍历所有顶点对 (i, j)*/
            for (int i = 0; i < nn; ++i)         /*外层循环，用于遍历顶点编号 i，其中 nn 是未删除顶点的数量。*/
                for (int j = i + 1; j < nn; ++j) /*内层循环，用于遍历顶点编号 j，确保 i 和 j 不是同一个顶点，避免重复检查。*/
                     /*这个条件语句检查顶点 oID[i] 和顶点 oID[j] 是否不相邻。它通过检查邻接矩阵 adjN 中的对应位来确定两个顶点之间是否没有边连接。mf.SAP(nn + i, j)：
                     这是调用一个网络流算法（可能是最大流算法）来计算从超级源点 nn + i 到顶点 j 的最大流。这里的目的是验证在移除顶点 oID[i] 后，是否仍然可以通过网络流
                     从超级源点到达顶点 j。*/
                    if (!adjN[oID[i]].test(oID[j]) && mf.SAP(nn + i, j) < bound)
                        /*将计算得到的最大流值与指定的 bound 进行比较，看是否小于 bound。如果小于 bound，表示从超级源点到达顶点 j 的最大流小于 bound，即移除顶点 oID[i] 后，
                        图的顶点连通性小于 bound。*/
                        return false;
            return true;
        }
    }
} vc;

/*代码定义了一个名为 addedge 的内联函数，该函数用于在图的邻接表中添加一条边*/
inline void addedge(int *h, int v) /*它接受两个参数，一个是指向顶点链表头的指针h，另一个是要添加的边的目标顶点v。*/
{
    nxt[etot] = *h;   /*将当前顶点链表头 *h（这是一个链表中上一个元素的索引）的值赋给 nxt 数组的 etot 位置，它表示新边将插入到链表的开始位置，并且现在的头部将成为新边的下一个元素。*/
    to[etot] = v; /*在 to 数组的 etot 位置记录目标顶点 v，to 数组用于存储与每条边相对应的目标顶点。*/
    *h = etot++;  /*更新头指针 *h 的值为当前边的索引 etot，然后 etot 自增，为下一次添加边做准备。etot 是全局变量，用于追踪邻接表中边的总数。*/
}

/*PreProcess 函数用于对图进行预处理，以便减小要处理的图的大小。PS：这里所采用的预处理方法实在是太过于简易了，仅仅根据顶点的度数进行了预处理，*/
bool PreProcess(int S)
{
    /*根据我从下面的代码中来看，这里面的预处理部分仅仅使用了对于顶点度数的判断来筛选顶点*/

    /*首先进行了内存分配*/
    /*M 是边的数量，而 N 是顶点的数量*/
    nxt = new int[M + M]; /*nxt 和 to 用于存储邻接链表的信息，*/
    to = new int[M + M];
    degree = new int[N];                /*顶点度数数组*/
    head = new int[N];                  /*head 是邻接链表的头指针*/
    outcore = new bool[N];              /*outcore 用于标记哪些顶点在预处理阶段被过滤掉*/
    que = new int[N];                   /*que 是用于广度优先搜索的队列*/
    nV = new int[N];                    /*nV 用于存储根据顶点度数删减后的顶点集合。*/
    memset(degree, 0, N * sizeof(int)); /*degree 数组被初始化为零（表示所有顶点的初始度数为零）*/
    memset(head, -1, N * sizeof(int));  /*head 数组被初始化为 -1（表示链表的开始）*/
    etot = 0;                           /*etot 被设置为 0，它表示边的总数，全称是e-total*/
    for (int i = 0; i < M; ++i) /*遍历所有的边，构建度数数组degree和构建邻接表。*/
    {
        /*对于每一条边，它会增加两个顶点 u 和 v 的度数。因为边在读取文件的时候是去掉了重复的双向存储的，所以可以直接这么写，不需要除以2了。*/
        int u = E[i].first, v = E[i].second;
        ++degree[u];
        ++degree[v];
        /*这个循环处理图中的每一条边，增加相关顶点的度数，并通过 addedge 函数添加边。 这里看起来是在构建一个无向图，因为它同时添加了 u 到 v 和 v 到 u 的边。*/
        addedge(head + u, v); /*然后调用 addedge 函数将这两个顶点互相连接，构建无向图的邻接链表。*/
        addedge(head + v, u); /*head + u 和 head + v 分别代表顶点 u 和 v 的邻接链表头部的位置。*/
    }
    memset(outcore, 0, N * sizeof(bool)); /*初始化 outcore 数组，用于标记顶点是否满足某些条件*/
    int fr = 0, re = 0;  /*队列用于宽度优先搜索，fr 代表队列的头部，re 代表队列的尾部。*/  
    for (int i = 0; i < N; ++i)/*根据顶点度数将不符合要求的顶点筛选出来。*/
        if (degree[i] < S - K) /*遍历每个顶点，如果它的度数小于 S - K，则将该顶点加入队列，并在 outcore 中标记为 true。这里利用的就是kplex的性质来处理的。*/
            que[re++] = i, outcore[i] = true; /*再次验证了outcore里面的顶点*/
    /*用异或运算来检查队列是否为空，开始进行宽度优先搜索。*/
    while (fr ^ re) /*通过队列实现了类似宽度优先搜索的操作，用于进一步处理顶点和边的关系。这个符号是异或操作，异或操作的技巧是不同为1，相同为0。*/
    {                      
        int u = que[fr++]; /*从删除队列中取出一个顶点u，检查是否会因为这个顶点被删除后，它的邻居顶点也会不符合相应的要求。*/
        for (int e = head[u]; ~e; e = nxt[e])  /*循环检查每个顶点的邻接表（即该顶点的度数），如果度数低于阈值，则顶点会被加入 outcore。*/
        { /*遍历与顶点 u 相邻的所有顶点。循环的条件 ~e 意味着只要 e 不是 -1（~ 是按位取反操作符），循环就会继续。这里 -1 用于表示边的结束。*/
            int v = to[e]; /*通过边的索引 e 得到与 u 相邻的顶点 v。*/
            if (outcore[v])
                continue; /*如果 v 已经在 outcore 中，则跳过它。*/
            if (--degree[v] < S - K) /*如果 v 的度数减一后小于 S - K，则说明它不再符合kplex的特征，而我们知道，一个s-bundle一定也会是一个s-plex。*/
                que[re++] = v, outcore[v] = true; /*则将 v 加入队列并标记在 outcore 中*/
        }
    }
    n = 0; /*为了构造新的顶点集合所设置的一个索引，初始化为0，后面自增。*/
    for (int i = 0; i < N; ++i)
        if (!outcore[i]) /*现在开始把没有被删除的顶点放在新数组里面*/
            nV[i] = n++;  /*对于每个未标记为 outcore 的顶点 i，它在 nV 数组中被赋予一个新编号，然后 n 增加。这是为了创建一个新的顶点集，其中只包含特定条件的顶点。*/
    if (n < S) /*如果处理后的顶点数量 n 小于阈值 S，函数返回 false。这里的S是我们传入的下界，也就是说至少要这么多顶点数才有意义。*/
        return false;

    /*下面代码遍历所有边，并对边进行过滤。如果边的任一端点被标记在 outcore 中，则该边被忽略。对于未被忽略的边，更新它们的端点为新的编号，并将其存储回 E 数组。
    这样，E 数组中的边仅包含未被 outcore 标记的顶点。m 计数未被忽略的边的数量，并最终用于更新 M，即图中边的总数。所以说outcore的意思就是不符合要求的顶点。*/
    int m = 0; /*初始化边的计数器 m。*/
    for (int i = 0; i < M; ++i)
    {  /*循环遍历所有的边（由数组 E 存储）*/
        int u = E[i].first, v = E[i].second;
        if (outcore[u] || outcore[v])
            continue; /*对于每一条边 (u, v)，如果它的任一端点 u 或 v 被标记为 outcore（即它们中的至少一个不在核心子图中），则跳过这条边。*/
        /*否则，它将边重新映射为处理后的顶点编号，并存储在 E 数组中。*/
        E[m++] = make_pair(nV[u], nV[v]);
    }
    M = m; /*更新新的边数量。*/
    del = new bool[n]; /*del用于标记已经从当前考虑的子图中删除的顶点，*/
    memset(del, 0, n * sizeof(bool));
    ins = new bool[n]; /*ins 用于标记已经加入子图的顶点。*/
    memset(ins, 0, n * sizeof(bool));
    notadj = new int[n]; /*notadj 用于记录与某个顶点不相邻的顶点数量。*/
    memset(notadj, 0, n * sizeof(int));
    up_bound = new int[n]; /*其实up_bound装的是每个顶点的core值。*/
    ordID = new int[n];
    dis = new int[n];
    mf.reserve(n, m);
    vc.reserve(n);
    return true;
}

/*getOrd 函数的主要作用是进行了退化排序（degeneracy ordering）。*/
void getOrd()
{
    /*初始化度数数组degree为0，头指针数组head为-1，边的总数etot为0。这些操作准备数据结构，以存储图中每个顶点的邻接信息。*/
    memset(degree, 0, n * sizeof(int));
    memset(head, -1, n * sizeof(int));
    etot = 0; // etot 用于记录边的总数。

    /*遍历所有边，更新顶点的度数，并通过addedge函数将边添加到邻接表中。*/
    for (int i = 0; i < M; ++i) /*更新顶点的度数数组，因为前面已经删除过一些顶点了，所以需要重新更新。*/
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
    /*这是一个通用的优先队列，可以容纳任何类型的元素。元素按升序排列，也就是说，队列的顶部总是包含最小的元素。关于优先队列的常用成员函数：
    empty() 如果优先队列为空，则返回真； 
    pop() 删除第一个元素 ；
    push() 加入一个元素 ；
    size() 返回优先队列中拥有的元素的个数； 
    top() 返回优先队列中有最高优先级的元素。 */
    memset(outcore, 0, n * sizeof(bool));     /*outcore用来标记顶点是否符合kplex的基本度数要求，其实它就是一种变了形式的visited数组，只不过这里强调了它的度数概念。*/

    /*将所有顶点按度数的降序放入优先队列。*/
    for (int i = 0; i < n; ++i)
        Q.push(make_pair(-degree[i], i));
    /*这段代码是在一个循环中，将每个顶点按照其度数（degree）加入了一个优先队列（Priority Queue）Q 中，
    同时使用负数形式将度数变为负数，这是因为默认的优先队列是按照元素值的升序排列，而我们希望将度数最小的顶点排在队列的前面，因此将度数取负数来实现降序排列。
    在默认的优先队列中，较大的数优先出列。但是我们想要实现退化排序，所以我们加了一个符号，从而可以使得度数最小的顶点优先出列。*/

    /*接下来进行退化排序！！首先是不断的删除当前所有顶点中顶点度数最小的顶点*/
    /*使用优先队列进行迭代，更新顶点的上界curB。*/
    int curB = 1; /*初始化当前的上界（curB）为1。*/
    while (!Q.empty()) /*当顶点度数队列Q不为空的时候*/
    {
        int u = Q.top().second; /*取出队列顶部的顶点u（度数最小的顶点）并将其从队列中移除。*/
        Q.pop();

        /*如果顶点u已经不符合顶点的度数要求了，则跳过它。*/
        if (outcore[u])
            continue;
        outcore[u] = true; /*将这个顶点标记为已经处理过一次了，防止后面重复处理。*/
        /*接下来设置每一个顶点的上界。*/
        if (degree[u] < curB - K) /*如果顶点u在当前顶点数量下都不符合kplex的要求，那么就把它的上界设为当前的下界curB*/
        {
            up_bound[u] = curB; /*如果是，顶点u的上界设置为当前的curB。*/
        }
        else
        {
            curB = degree[u] + K + 1; /*否则，更新curB为degree[u] + K + 1*/
            up_bound[u] = curB;       /*并将u的上界设置为新的curB。*/
        }
        /*记录完刚刚弹出来的最小度数的顶点后，现在就要把该顶点给删除，并更新顶点u的所有邻居的度数，减1。*/
        for (int e = head[u]; ~e; e = nxt[e])
        {
            /*对于每个邻居顶点v，如果v还没有被处理（不是outcore），则减少v的度数，并将v（及其新的度数）重新插入优先队列。*/
            int v = to[e];
            if (outcore[v])
                continue;
            --degree[v]; /*更新因为度数最小的顶点u被删除之后，它的邻居顶点的度数变化*/
            Q.push(make_pair(-degree[v], v)); /*把度数变化后的邻居顶点再次存入优先级队列Q中，由于Q是优先级队列，所以它会自动的把元素放在正确的位置。*/
        }
    }
    /*现在已经把所有的顶点都已经处理过了，已经计算出来了每个顶点的上界和更新了它们的度数了。*/

    /*初始化一个数组ordID，用于存储顶点的新顺序。*/
    for (int i = 0; i < n; ++i)
        ordID[i] = i;

    /*然后根据顶点的up_bound（其实就是core值）来对顶点进行升序排序。*/
    /*在lambda函数中,它接受两个整数参数x和y, 然后根据自定义的规则进行比较。*/
    sort(ordID, ordID + n, [](const int &x, const int &y) 
         {
         if (up_bound[x] != up_bound[y]) /*如果它们不相等，那么返回值取决于up_bound[x]是否小于up_bound[y]。*/
            return up_bound[x] < up_bound[y];
         return x < y; }); /*如果两个顶点具有相同的upper_bound值，那么按照顶点编号顺序从小到大进行排列。*/

    /*根据升序排序的结果，更新顶点的新编号映射。*/
    for (int i = 0; i < n; ++i)
        nV[ordID[i]] = i;

    /*重新初始化 degree、head 和 etot，并创建了两个名为 adjN 和 invN 的位集合数组，用于存储邻接关系和逆邻接关系。*/
    memset(degree, 0, n * sizeof(int));
    memset(head, -1, n * sizeof(int));
    etot = 0;

    /*创建两个新的位集合数组adjN和invN，分别用于存储邻接关系和逆邻接关系。*/
    adjN = new BIT_SET[n]; /*adjN数组表示顶点之间的邻接关系矩阵*/
    invN = new BIT_SET[n]; /*invN表示顶点之间的逆邻接关系矩阵*/
    /*初始化adjN数组，为每个顶点准备邻接位集*/
    for (int i = 0; i < n; ++i)
        adjN[i].init(n);
    /*对每条边进行遍历，将边的两个顶点重新映射为排序后的新编号，并更新相应的邻接关系和逆邻接关系。*/
    for (int i = 0; i < M; ++i) /*注意，这里的M在经过preprocess操作之后已经被更新了，现在应该对应着缩减后的边集合。*/
    {
        int u = E[i].first, v = E[i].second; /*从边数组E中获取当前边的两个顶点u和v。*/
        /*将顶点u和v重新映射为排序后的新编号，nV存储了顶点的新编号映射关系。*/
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
    /*复制adjN到invN并对invN执行翻转操作，得到逆邻接关系。这个逆邻接关系矩阵在后面判断两个顶点不相邻的时候特别便捷！*/
    for (int i = 0; i < n; ++i)
    {
        adjN[i].cp(invN[i]); /*复制顶点i的位集合到逆邻接关系invN[i]。注意，这里的复制关系是把adjN复制到invN！*/
        invN[i].flip();      /* 对invN[i]执行翻转操作，将原来为1的位变为0，为0的位变为1，得到顶点i的逆邻接关系*/
    }
    /*初始化并翻转另外两个位集合inD和inG*/
    inD.init(n); /*用init(n)初始化位集合候选集D的大小为n*/
    inD.flip();  /*使用flip()函数对位集合进行翻转，将所有位的值从0变为1，表示初始化为"true"状态。*/
    inG.init(n); /*同样初始化了另一个位集合inG，表示在图中。*/
    inG.flip();
    svex.clear(); /*由于还没有开始深度优先搜索，所以现在先清空svex*/
}

clock_t startTime;
long long dfs_node;
char filename[100];
int LB;

void exit_program() /*退出程序的时候打印信息：（1）文件的名称filename；（2）s-bundle的s的值K；（3）深度优先搜索经历过的顶点数量dfs_node；（4）我们传入的s-bundle的下界LB；（5）算法运行所消耗的时间。*/
{
    printf("Filename: %s\n", filename);
    printf("Value of K: %d\n", K);
    printf("Search nodes: %lld\n", dfs_node);
    printf("Lower bound: %d\n", LB);
    printf("Used Time: %.10f s\n", (double)(clock() - startTime) / CLOCKS_PER_SEC);
    // printf("The obtained solution is: %d\n",);
    // for(int i = 0; i < n; i++){
    //     if(!ins[i]){cout << i << ", ";} 
    // }
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


inline void addtoD(int u) /*addtoD表示把一个顶点放回至候选集中。*/
{
    inD.set(u);                           /*设置一个名为inD的数据结构中的第u位，通常用于标记节点u在候选集中。*/
    inG.set(u);                           /*设置一个名为inG的数据结构中的第u位，通常用于标记节点u在图中*/
    del[u] = false;                       /*将名为del的布尔数组中的节点u标记为未删除*/
    for (int e = head[u]; ~e; e = nxt[e]) /*遍历节点u的所有边，增加相应的邻居的度数*/
        /*~e 是一个按位取反操作，用于检查 e 是否为 -1（通常 -1 用作哨兵值表示结束）。如果 e 不等于 -1，循环继续。*/
        ++degree[to[e]]; /*将边所连接的节点的度数加一，即将节点to[e]的度数加一。这是因为要将节点u添加回图中，其连接的节点的度数需要更新*/
}

/*用于将顶点u从已选集顶点集合svex中删除，并将其重新放回候选集中。*/
inline void delfrS(int u) /*delfrS表示把一个原本在解集S中的顶点拿出来又放回到集合D中去，通常用于回溯操作。*/
{
    inD.set(u);      /*标记顶点u在候选集合中。*/
    ins[u] = false;  /*将顶点u从当前的解集中进行删除。*/
    svex.pop_back(); /*把顶点u从当前的解集集合svex中弹出来，*/
    for (int i = 0; i < n; ++i) /*更新与顶点u不相邻的顶点数量*/
    {
        if (i != u && !adjN[u].test(i)) /*检查顶点i是否与顶点u不相邻（没有边相连）*/
            --notadj[i];                /*如果顶点i与顶点u不相邻，则将notadj[i]减一。*/
    }
}

/*用于将一个顶点u添加到已选集S中。*/
inline void addtoS(int u)
{
    inD.set(u);        /*标记节点u在候选集合中。*/
    ins[u] = true;     /*将顶点u标记为已经插入到已选集中。*/
    svex.push_back(u); /*将顶点u添加到解集向量svex的末尾。*/
    for (int i = 0; i < n; ++i) /*接下来更新与刚刚加入的顶点u不相邻的顶点的数量，便于后面进行剪枝判断的时候使用。*/
    {
        if (i != u && !adjN[u].test(i)) /*检查顶点i是否与顶点u不相邻（没有边相连）*/
            ++notadj[i];  /*如果顶点i与顶点u不相邻，则将非邻居矩阵notadj[i]加1*/
    }
}

/*bool canadd(int u) 函数是用于检查是否可以将节点 u 添加到当前的 svex 集合中，而不违反算法的约束条件。判断包括两个部分，第一是判断是不是一个kplex，第二则是判断是否连通且最大流是否满足条件。*/
inline bool canadd(int u)
{
    int tot = 0; /*初始化一个计数器 tot，用于计算节点 u 与当前 svex 集合中的节点不相邻的数量。*/
    for (auto v : svex)
        if (!adjN[u].test(v)) /*如果顶点u和顶点v不相邻，则执行以下操作：*/
        {
            if (++tot >= K) /*如果不相邻的节点数量大于等于 K，表示添加节点 u 将导致超过 K 个不相邻的节点存在于 svex 集合中，
            这将违反算法的约束条件，因此返回 false。这里是根据kplex的性质进行的判断。*/
                return false;
            if (notadj[v] >= K - 1) /*如果节点 v 的不相邻节点数量大于等于 K-1，表示添加节点 u 可能导致节点 v 成为一个不相邻的节点，这也将违反算法的约束条件，因此返回 false。*/
                return false;
        }
    /*如果前面没有问题，那么就可以把这个顶点暂时加到我们的解集svex中。*/
    svex.push_back(u); /*将节点 u 添加到 svex 集合中*/

    /*暂时加入了顶点u之后我们还要进一步的判断这个图是不是一个连通图，调用solve方法，设置标记为true，表示只看svex的连通性，同时计算svex的最小割。*/
    bool ret = vc.solve(true, svex.size() - K);

    svex.pop_back(); /*将节点 u 从 svex 集合中移除。这是因为这个函数的目的仅仅是判断顶点u能不能被加入到我们的解集中，而不是真正的添加。真的添加还在其他的操作中。*/
    return ret;      /*最后，返回ret，它表示是否可以将节点 u 添加到 svex 集合中。*/
}

/*广度优先搜索算法用于计算从节点s出发到其他节点的最短距离，并将结果存储在 dis 数组中。便于后面进行和距离相关的一些剪枝操作，这些都在论文中有提及。*/
void bfs(int s)
{
    memset(dis, -1, n * sizeof(int)); /* 初始化 dis 数组，大小和总的顶点数目相当，并将所有节点的距离初始化为 -1，表示尚未访问。*/
    dis[s] = 0;                       /*设置节点 s 到自身的距离为 0。*/
    int fr = 0, re = 0;               /*初始化队列的前后指针 fr 和 re。*/
    que[re++] = s;                    /* 将起始节点 s 放入队列*/
    while (fr ^ re)                   /*进行广度优先搜索*/
    {
        int u = que[fr++];                    /*从队列中取出一个节点 u 进行处理。*/
        for (int e = head[u]; ~e; e = nxt[e]) /*遍历节点 u 的每个邻接节点 v*/
        {
            int v = to[e];
            if (del[v] || dis[v] != -1) /*如果顶点v没有被删除并且v尚未被访问（dis[v] == -1），则执行以下操作：*/
                continue;
            dis[v] = dis[u] + 1; /*设置节点 v 的距离为节点 u 的距离加 1，因为现在的顶点v都是顶点u的1跳邻居，所以它们距离原始顶点u的距离就是1。*/
            que[re++] = v;       /* 计算完1跳邻居后，将节点放入队列以进行后续距离计算的处理。*/
        }
    }
}

void check_timeout()
{
    if ((double)(clock() - startTime) / CLOCKS_PER_SEC > TIME_LIM)
        exit_program();
}

/*这个函数对应于作者在论文中提到的基于图着色的解的上界，在3.3节。具体的求解方法描述在原文中非常详细。*/
int get_color()
{
    BIT_SET Q, P;
    inG.cp(P);   /*inG表示在整个图中的顶点。这里是将inG这个位图复制给P。*/
    int tot = 0; // tot 被初始化为0，用于统计最终得到的顶点上界。
    while (!P.empty())
    {
        /*在每次外部循环迭代中，初始化 cnt 为0，然后将 P 的当前状态复制到 Q。进入一个 do-while 循环，直到 Q 为空。
        在 do-while 循环中，首先获取 Q 中最低设置位的索引（lowbit 方法）并将其赋值给 u。累加 cnt。如果 cnt 超过 K，则将其设置为 K。
        通过 set 方法在 P 和 Q 中设置位 u。然后更新 Q 为 Q 与 invN[u] 的按位与。invN 可能是一个数组，包含与每个顶点相关的位掩码，用于表示某种特定关系或属性。*/
        int cnt = 0;
        P.cp(Q);
        do
        {/*现在这里完全对应着论文中的lemma2，计算出所有的独立集里面能够提供的上界。*/
            int u = Q.lowbit();
            ++cnt;
            if (cnt > K) /*每个独立集里面能提供的上界值是独立集大小和s-bundle中的s的较小值。*/
                cnt = K;
            P.set(u);
            Q.set(u);
            Q &= invN[u]; /*不断的取出顶点u的非邻居，就是为了计算染色数。*/
        } while (!Q.empty());
        tot += cnt;
    }
    return tot;
}

/*这个函数用来获得一个基于图着色的顶点数量上界，用来剪枝的。在论文中的3.3节。*/
void get_color(vector<pair<int, int>> &lst) /*这个传入了参数的get color和前面没有传入参数的get color的区别在于，有参数的get color主要计算inD里面的最大图染色上界，
也就是说这个函数专门针对于候选集里面的顶点集合进行图着色上界的的计算。*/
{
    BIT_SET Q, P; /*初始化 BIT_SET 对象并拷贝状态*/
    inD.cp(P); /*把inD拷贝到刚刚创建的集合P中。注意，inD表示在候选集合中。*/
    int tot = 0; /*total的意思就是指当前的图的集合能够获得的基于染色的最大上界。*/
    while (!P.empty())  /*循环处理直至P为空，其中P是inD的副本，表示删除顶点集合。*/
    { /*P.empty() 检查 P 是否没有任何设置的位。*/
        int cnt = 0;
        P.cp(Q); /*在每一次循环中，把位集合P中的内容复制到集合Q中。*/ 
        do/*在 Q 中进行操作并更新 lst*/
        {
            /*在一个 do-while 循环中，Q.lowbit() 获取 Q 中最低的设置位的位置，并赋值给 u。
            cnt 用于计数，如果超过某个阈值 K，则将 cnt 设置为 K。
            向 lst 中添加一对整数 (u, tot + cnt)。这可能表示顶点 u 以及与之相关的某种计数或度量。
            P.set(u); Q.set(u); 将 P 和 Q 中位置 u 的位设置为1。
            Q &= invN[u]; 更新 Q 为 Q 与 invN[u] 的按位与。这可能是用于移除与顶点 u 相关的位或表示某种特定的关系。*/
            int u = Q.lowbit(); /*获取 Q 中最低的设置位（从右往左的方向）的位置，并赋值给 u。假设此时的u为u1，那么当进行第二次while循环时候得到的u2则是u1的非邻居顶点了。*/
            ++cnt; /*计数自增。*/
            if (cnt > K) /*如果超过某个阈值 K，则将 cnt 设置为 K。*/
                cnt = K;
            /*lst里面存放的是候选集中每一个顶点的最大图染色上界。*/
            lst.emplace_back(make_pair(u, tot + cnt)); /*lst集合中每次添加的是一对整数，其中第一个参数表示顶点的原始编号，而第二个参数表示一个从1开始一直递增的一个数字。*/
            P.set(u); /*将 P 和 Q 中位置 u 的位设置为1。*/
            Q.set(u);
            Q &= invN[u]; /*更新Q为Q与invN[u]的按位与，其中invN是非邻接顶点位集合，用来快速判断两个顶点是否是不相邻。invN[u]表示和顶点u不相邻的顶点集合。
            执行Q &= invN[u]操作的意思是找出集合Q里面的顶点和顶点u不相邻的顶点且这些顶点也是存在Q里面的。简单来说，执行完这句后，Q里面剩下的顶点都是和顶点u不相邻的顶点位置为1。*/
        } while (!Q.empty());
        tot += cnt;
    }
}

/*这个dfs是在当前的图是一个合格的kplex的情况下调用的，属于dfs里面的一种特殊情况，因为只有当当前的图是一个kplex，那么它才有可能是一个s-bundle，反之则不然。*/
void dfs_kbundle(int curS, bool check) /*第一个参数表示当前的顶点集合大小；第二个参数表示是否要检查当前子图svex的连通性。*/
{
    check_timeout();
    ++dfs_node; /*递归次数加1。*/
    LB = max(LB, (int)svex.size()); /*更新下界。*/
    if (curS <= LB)
        return;
    if (check && vc.solve(false, curS - K)) /*如果 check 为真且solve函数返回真（意味着当前的curS构成的图是连通的），就说明当前的图是一个合格的s-bundle！
    则将下界LB 设置为 curS 并返回。*/
    {
        // printf("LB is %d, the size of svex is %d, and the curS is %d\n", LB, (int)svex.size(), curS);
        LB = curS; /*现在的LB就是真的和s-bundle相关的下界了*/
        return;
    }
    /*获取颜色列表并进行检查*/
    vector<pair<int, int>> lst; /*定义了一个顶点对集合，这个集合里面存储了候选集D中每个顶点的最大染色上界数。*/
    get_color(lst); /*关于lst顶点对集合的赋值在get_color函数中进行。*/
    int m = lst.size(); /*获得lst集合的大小，*/
    if (m == 0) /*如果 lst 为空（即 m == 0），就意味着一个图为空，则返回。*/
        return;
    check = false; /*当check为假的时候，意味着再次调用dfs_kbundle的时候，solve函数将会检查整个图的连通性，而不仅仅是检查当前的解集svex的连通性。*/
    /*处理列表中的元素*/
    for (int i = m - 1; i >= 0; --i) /*进行反向遍历。注意了！这里是进行的反向遍历！m应该是候选集D的大小！先从最后一个元素遍历，这时候的lst[i]是最大的。*/
    {
        if ((int)svex.size() + lst[i].second <= LB) /*lst.second存放的是和顶点数量相关的。这个if语句表达的意思是如果lst[i]（即候选集中的顶点i的最大上界）无法
        取得一个比当前下界更优的解的时候，就把剩余的顶点都加入到候选集D中。*/
        {
            for (int j = i + 1; j < m; ++j)
                addtoD(lst[j].first); /*重新把这些顶点放回到候选集合中去，D表示候选集。因为在循环后面调用了delfrD将这些顶点从候选集中删除掉了。*/
            return;
        }
        if (canadd(lst[i].first)) /*如果经判断，该顶点可以被添加到解集svex中不违反kplex的性质并且还是连通的话，则进行真正的添加进svex中。*/
        {
            addtoS(lst[i].first); /*addtoS表示把一个顶点添加到解集中去，S是solution的首字母缩写。*/
            dfs_kbundle(curS, check); /*进行递归操作*/
            delfrS(lst[i].first); /*delfrS表示把一个原本在解集S中的顶点拿出来又放回到集合D中去，通常用于回溯操作。*/
        }
        /*更新 curS 并在条件满足时将元素从另一个集合中删除（delfrD）。*/
        --curS; /*把当前传入的顶点集合的大小减1，用来表示已经处理了一个顶点*/
        check = true; /*再次把check标志置为真，表示下一次调用solve的时候只需要检查当前解集svex是否连通就行了。*/
        delfrD(lst[i].first); /*把已经处理过的该顶点从候选集中删掉。*******我现在开始觉得这个集合D是候选集了。*/
    }
    for (int i = 0; i < m; ++i)/*循环结束后，重新把lst集合中的所有元素放回到候选集合中去*/
        addtoD(lst[i].first);
}

void dfs(int curS) /*传入的参数curS是当要搜索的顶点集合的大小。*/
{
    // printf("the current curS is %d\n", curS);
    check_timeout(); /*用于检查算法运行是否超时。*/
    ++dfs_node;   /*追踪递归调用的次数。*/
    // printf("LB is %d, the size of svex is %d\n", LB, (int)svex.size());
    LB = max(LB, (int)svex.size()); /*更新当前的下界，其中svex是当前的解s-bundle。*/
    if (curS <= LB) /*递归的终止条件！！*/
        return; /*如果 curS（当前解）小于等于 LB，则函数返回，停止进一步搜索。*/

    /*找出当前顶点度数最小的未删除顶点。*/
    int minID = -1;
    for (int i = 0; i < n; ++i)
        if (!del[i])
        {
            if (minID == -1 || degree[i] < degree[minID])
                minID = i;
        }

    /*如果最小度数的顶点的度数大于等于 curS - K，则调用 dfs_kbundle 函数并返回。注意了，后面的dfs会一直执行到满足这个if条件为止，因为dfs_kbundle是最核心的分支方法，这也是建立
    在kplex的基础上的。也就是说，一个s-bundle一定先得是一个s-plex，其次再从这些s-plex里面提取出一个合格的s-bundle。*/
    if (degree[minID] >= curS - K) /*这里就验证了当前的顶点集合是一个kplex，这也就满足了当前的图是一个s-bundle的前置条件。*/
    {   /*其实这里的思想和我想要实现的思想差不多，就是先验证是不是kplex，然后再进一步判断是不是s-bundle。因为验证是否是kplex要简单许多。*/
        dfs_kbundle(curS, true); /*在调用这个函数时所传入的参数：第一个表示当前的顶点集合大小；第二个表示check标致置为真。*/
        return;
    }
    /*假如最小度数的顶点不符合kplex的度数要求，那么执行下面的操作。*/
    if (degree[minID] < LB + 1 - K) /*如果该最小度数的顶点甚至比不上我们已经得到的下界LB所构成的kplex，那么这个顶点就符合reduction条件，可以被删去。*/
    {  
        if (ins[minID]) /*如果最小度数顶点已经被插入了解集S中，那么就说明当前解集S不是我们要找的最大s-bundle，因为它甚至都不是一个kplex，这时候可以提前终止该分支的搜索。*/
            return;
        delfrD(minID); /*如果该最小度数顶点没有在解集S中，那么就从候选集中删除该最小度数顶点minID。*/
        dfs(curS - 1); /*递归进一步的进行，因为已经确保处理了一个顶点。递归地调用dfs函数，减少当前解的大小。根据dfs函数前面的终止条件：如果在将度数最小的顶点删除后的图里面
        没有能找到比LB更优的解，那么就会终止这个递归子算法。当所有的子算法里面的递归都消失了后，我们就可以终止我们最外面的递归，得到我们在递归过程中所得到的答案了。*/
        addtoD(minID); /* 如果没能在上面一行的dfs中找到令人满意的解，那么又将该最小度数顶点minID重新添加到候选集中*/
        /*需要说明的是，在递归函数里面，并不是说一个return就会终止整个函数！而是return会不断的把值往回抛！但是直到满足递归终止条件的时候才会结束算法！！！*/
        return;
    }
    /*经过前面的代码后，现在的minID的顶点度数既不符合kplex的要求，但是也不比LB+1-K小。那么就执行另外一种情况。*/
    /*寻找和处理另一个特定条件的顶点, 这次是寻找未被删除的度数最大的顶点*/
    int maxID = -1; /*初始化一个变量maxID，用于存储度数最大的未删除顶点的编号。*/
    for (int i = 0; i < n; ++i)
        if (!del[i]) /*如果该顶点没有被删除。*/
        {
            if (maxID == -1 || notadj[i] > notadj[maxID]) /* 判断是否需要更新当前度数最大的顶点的编号*/
                maxID = i;   /*更新maxID为i*/
        }
    /*处理 maxID 对应的顶点*/
    /*第一种情况：当前度数最大的顶点没有在解集S中，并且它的度数不符合kplex的要求。*/
    if (!ins[maxID] && notadj[maxID] >= K) /*如果最大度数的顶点maxID没有被插入到解集S中并且与 maxID 不相邻的顶点数量大于等于 K（说明不符合基本的kplex的要求）*/
    {
        delfrD(maxID); /*从候选集中删除最大度数的顶点*/
        dfs(curS - 1); /*递归搜索没有该最大度数顶点存在的图是否能找到更大的s-bundle。这里就是很常见的一种思想，先假装把一个顶点给删除掉，然后在深度搜索后又把这个顶点加回来*/
        addtoD(maxID); /*将顶点maxID重新添加到候选集中。*/
        return;
        /*这一段代码的目的是考虑了一种情况，即顶点maxID可以被删除以获得更优解的可能性*/
    }
    /*第二种情况：当前度数最大的顶点已经在解集S中，并且它的度数已经快要不满足kplex的要求了。这里对应着论文中原文的3.4部分的Reduction，属于饱和顶点。*/
    if (ins[maxID] && notadj[maxID] >= K - 1) /*检查顶点maxID是否已经被插入（已被标记为已插入）且与顶点maxID不相邻的顶点数量是否大于等于K - 1*/
    {
        /*如果与顶点maxID不相邻的顶点数量大于等于K，说明它不符合kplex的基本要求，则直接返回，不再继续搜索。*/
        if (notadj[maxID] >= K) /*这个if语句不同于前面的第一种情况，因为这里是表示顶点maxID已经被插入到了已选集中的。*/
            return;
        /*如果在这里没有返回，那么就说明这个maxID是一个饱和顶点！！！它已经濒临被剔除的边缘了！！*/
        vector<int> todel; /*创建一个空的整数向量用于存储待删除的顶点*/
        for (int i = 0; i < n; ++i)
            if (!del[i] && !ins[i] && !adjN[maxID].test(i)) /*如果顶点i未被删除、未被插入（也就是说是候选集中的顶点），并且与顶点maxID不相邻。*/
            {
                todel.push_back(i); /*如果条件满足，那么将顶点i添加到待删除的顶点列表中。因为这些顶点已经不能再加入到有maxID在的解集里面去了！它们缺失的邻居顶点数目已经达到了k个了！*/
            }
        if (todel.size()) /*如果待删除的顶点列表中有顶点*/
        {
            for (auto x : todel) /*遍历待删除的顶点列表，并从候选集中全部删除这些顶点*/
                delfrD(x);
            dfs(curS - todel.size()); /*递归地调用dfs函数，减少当前待搜索空间的大小。到了这里我们不难发现，dfs越往后面会越来越小，也就越来越容易找到解。*/
            for (auto x : todel) /*如果前面的dfs没有能找到更大的s-bundle，那么我们再把刚刚从候选集中删除掉的顶点再依次加回候选集中去。*/
                addtoD(x); /*恢复先前删除的顶点到当前解中*/
            return;
        }
    }

    if (svex.size()) /*如果当前的解集svex不为空*/
    {
        /*如果 svex 非空，选择其中的一个随机顶点进行广度优先搜索（BFS），然后基于 BFS 的结果收集一组特定条件下的顶点进行进一步处理。*/
        vector<int> sofar; /*存储待处理的顶点*/
        int u = svex[rand() % svex.size()]; /*如果 svex 非空，代码随机选择 svex 中的一个顶点 u。选择是通过 rand() % svex.size() 实现的，这会生成一个介于 0 和 svex.size() - 1 之间的随机索引。*/
        bfs(u);   /*对随机选中的顶点 u 执行广度优先搜索 (BFS)。在bfs中会把顶点u的相关距离进行初始化，存放在dis数组中。*/
        /*下面的循环遍历图中的所有顶点 v，收集那些未被删除且满足特定条件的顶点。条件是顶点 v 要么在 BFS 中未被访问过（dis[v] == -1），
        要么其距离超过了某个阈值（dis[v] > max(2, K + K - LB)）。这个阈值用于图的约简。 如果顶点 v 已经被插入 (ins[v] 为真)，则函数提前返回。*/
        for (int v = 0; v < n; ++v) /*遍历当前所有的顶点*/
            if (!del[v]) /*如果顶点v未被删除*/
            {
                if (dis[v] == -1 || dis[v] > max(2, K + K - LB)) /*顶点v在广度优先搜索中未被访问过（dis[v]的值为-1，说明这个顶点和我们的顶点u不连通），
                且顶点v的距离超过了某个阈值，这个阈值是max(2, K + K - LB)*/
                {/*如果满足了上面的任何一个条件，则说明该顶点的距离已经超过了一个合格s-bundle所允许的最大距离了。*/
                    if (ins[v]) /*如果这个顶点已经被插入解集S中了，那么就说明整个解集都是不符合s-bundle要求的（因为此时包含顶点v的图是一个不连通图）。此时直接返回。*/
                        return;
                    sofar.push_back(v); /*但如果顶点v未被插入，则将该不符合距离要求的顶点v添加到sofar数组中进行存储，便于后面统一处理。
                    push_back的主要作用是在vector的尾部添加一个新的元素。*/
                }
            }
        if (sofar.size()) /*如果存放距离不符合的顶点的sofar数组不为空，那么就把这些不合格的顶点们都删掉， 然后再进行深度优先递归搜索。*/
        {
            for (auto x : sofar)
                delfrD(x);  /*将sofar数组中所有的顶点从候选集中删除。*/
            dfs(curS - sofar.size()); /*递归地调用dfs函数，传入的参数是缩小后的候选集的大小*/
            for (auto x : sofar) 
                addtoD(x); /*如果没有找到最优解，那么将删除的顶点重新加回到候选集中。*/
            return;
        }
    }
    /*颜色获取与分支处理*/
    if (get_color() <= LB) /*如果根据染色法计算出来的上界upper bound比不上当前的下界，那么说明当前的分支可以被砍掉。*/
        return;
    /*收集分支处理的顶点*/
    vector<int> branch; /*branch就是对应着最小度数顶点不相邻的顶点集合。*/

    /*接下来开始进入多分支策略，每次都从当前图中度数最小的顶点出发，然后找和这个顶点不相邻的顶点集合。*/
    for (int x = 0; x < n; ++x)
        if (!del[x] && x != minID && !ins[x]) /*如果顶点x未被删除、且顶点x不是度数最小的顶点，同时顶点x未被插入*/
        {
            /*如果上述条件都满足，继续检查另一个条件*/
            if (!adjN[minID].test(x)) /*如果该顶点x不是最小度数的顶点的邻居。这里使用adjN[minID].test(x)来检查顶点之间是否有边相连。*/
                branch.push_back(x);  /*那么就把该顶点x添加到branch数组中。注意!!!作者这里完全没有使用任何的排序规则，就是按照顶点序号放进去的。*/
        }

    /*接下来我将要按照我的想法来给这个分支集合branch中的顶点进行一个排序。排序需要借助退化排序序列。*/
    // printf("the size of branch is %d\n", branch.size());    
    /*处理 minID 对应的顶点，这里就是对应的原文中的多分支策略，每次都是选择的度数最小的这个顶点。*/
    if (ins[minID]) /*当顶点minID已被插入时，就需要找它所能够加入的最大非邻居顶点数量。对应着原文中的分支规则2！*/
    {
        int canselect = K - 1 - notadj[minID], pos = -1;/*首先计算canselect，它表示可以继续被加入解集S中的顶点数量, 初始化pos为-1，便于后面把这些顶点从解集S中取出。*/
        bool all = true; /*all 初始化为 true，可能用于标记是否所有顶点都符合某个条件。*/
        for (int i = 0; i < canselect; ++i) /*对branch 中的顶点执行操作，选择canselect数量个顶点*/
        {
            /* 循环遍历 branch 中的顶点，但只处理到 canselect 数量的顶点。对每个顶点，先从当前结构中删除 (delfrD)，然后检查是否能够添加上一个顶点（canadd(branch[i - 1])）。
            如果不能添加，将当前顶点重新添加 (addtoD)，设置 all 为 false 并跳出循环。
            如果可以添加，将上一个顶点的状态更改为已插入 (addtoS) 并更新 pos。每次循环中，递归调用 dfs 函数，更新搜索深度。*/
            delfrD(branch[i]); /*正是对应着论文中多分支的策略，即把第i个顶点从候选集中删掉。第一个分支直接把第一个顶点从图中删除，也就是delfrD。*/
            if (i && !canadd(branch[i - 1])) /*如果这个顶点不是第一个顶点，即i!=0，并且通过canadd判断分支顶点集合中的第i-1个顶点能否和当前第i个顶点一起被加入到解集中。
            这里是判断不能被同时加入。*/
            {
                addtoD(branch[i]); /*如果不能添加，将当前顶点重新添加回候选集中*/
                all = false;  /*将all设置为false，并跳出循环。all表示第i个顶点不能和前面已经分支过的顶点共存。*/
                break;
            }
            if (i) /*这里就刚好对应了第0个分支先把第i个顶点删除，然后后面的第i个分支把第i个顶点删除，然而前面的i-1个顶点都加入到解集中。*/
            {
                addtoS(branch[i - 1]); /*如果可以添加，则将前面的顶点加入解集S中。*/
                pos = i - 1; /*更新pos的位置*/
            }
            dfs(curS - 1); /*递归调用dfs函数，更新搜索深度为当前的顶点数量减去1。因为每次循环都只是处理1个顶点。*/
            addtoD(branch[i]); /*由于前面我们在第i个分支把第i个顶点给暂时删掉了，现在递归后又把它加回到候选集中。*/
        }
        /*因为在上面的循环中，并没有对canselect里面的最后一个顶点进行判断，所以下面单独把这个特殊位置的顶点拉出来处理。
        其实这里就是对应着多分支规则的最后一个分支！！！即把最小度数顶点所有允许的可加入非邻居都剔除掉的那个分支！*/
        if (all && (canselect == 0 || canadd(branch[canselect - 1]))) /*如果canselect集合中所有顶点都没有产生冲突，同时要么canselect为0（意味着没有不相邻的顶点可以被用来分支）
        ，要么能够添加branch中最后一个顶点*/
        {
            for (int i = canselect; i < (int)branch.size(); ++i) /*循环从canselect位置开始，遍历branch中除了canselect后面的剩余顶点。根据分支规则，这些顶点都是要被删掉的。*/
                delfrD(branch[i]);                               /*将这些剩余顶点从候选集中删除。*/
            if (canselect)                                       /*如果还有剩余的可以被选择的分支顶点*/
                addtoS(branch[canselect - 1]);                   /*将branch中的第canselect个顶点加入到解集S中*/
            dfs(curS - branch.size() + canselect);               /*递归调用 dfs 函数，更新搜索深度为当前顶点数量减去分支数量并加上可以被选择的分支顶点。*/
            if (canselect)                                       /*如果还有剩余的可以被选择的分支顶点*/
                delfrS(branch[canselect - 1]);                   /*则在上面进行过dfs后还没有找到最优解的时候，把之前加入的顶点从已选集中拿出来放回到候选集D中。*/
            for (int i = canselect; i < (int)branch.size(); ++i) /*对于前面删除过的branch中除了canselect后面的剩余顶点，在结束递归后又重新加回去*/
                addtoD(branch[i]); /*重新将添加过的顶点放回到候选集中*/
        }
        for (int i = 0; i <= pos; ++i) /*现在pos就派上用场了，用来将前面添加到解集里面的顶点恢复原来状态。*/
            delfrS(branch[i]);   /*对每个顶点执行撤销状态更改操作，即将其从已选集S中拿出来放回到候选集D中。*/
    }
    else /*当度数最小的顶点没有被存放到解集S中时应采取的动作。对应着原文中的分支规则3！！*/
    { 
        delfrD(minID);      /*将度数最小的顶点从候选集中删除*/
        dfs(curS - 1);      /*接着，递归地调用dfs函数，传入curS - 1作为新的搜索深度*/
        addtoD(minID);      /*完成递归后重新将minID添加回候选集中。*/
        if (!canadd(minID)) /*如果 minID 不能被添加（由 canadd 函数检查）*/
            return;         /*则函数提前返回。因为此时度数最小的顶点已经不能构成一个合格的s-bundle了*/
        addtoS(minID);      /*否则如果minID能够被添加到解集S中，则将其插入*/
        /*继续处理 branch 中的其他顶点*/
        int canselect = K - 1 - notadj[minID]; /*这里指的就是一个顶点最多还能容忍的断连顶点数量了*/
        int pos = -1;                          /* 初始化pos为-1，可能用于追踪处理过程中的某个特定位置*/
        bool all = true;                       /*初始化all为true，用于标记是否所有branch中的顶点都符合某个条件*/
        for (int i = 0; i < canselect; ++i)    /*依次遍历在已经把度数最小的顶点加入到解集S中后，最多还能添加的顶点数量*/
        {
            delfrD(branch[i]);               /*首先，将该分支顶点从候选集中删除*/
            if (i && !canadd(branch[i - 1])) /*对于除第一个顶点外的每个顶点，检查是否能够添加上一个顶点*/
            {
                /*如果不能添加，则将当前顶点重新添加到候选集中，将all设置为false，并跳出循环*/
                addtoD(branch[i]);
                all = false;
                break;
            }
            if (i) /*！！！注意，从这里我看出了！！原来这就是利用了多分支的剪枝！一旦前面的任何一个分支不符合要求，都直接返回了！！！*/
            { /*对于除第一个顶点外的每个顶点，将上一个顶点插入到已选集中，并更新pos*/
                addtoS(branch[i - 1]); 
                pos = i - 1;
            }
            dfs(curS - 1);     /*递归地调用dfs函数，更新搜索深度，相当于在假设这些顶点都已插入的情况下继续搜索。*/
            addtoD(branch[i]); /*最后，将当前顶点重新添加到候选集中，即标记为未删除，方便其他的分支使用。*/
        }
        /*因为在上面的循环中，并没有对canselect里面的最后一个顶点进行判断，所以下面单独把这个特殊位置的顶点拉出来处理。
        其实这里就是对应着多分支规则的最后一个分支！！！即把最小度数顶点所有允许的可加入非邻居都剔除掉的那个分支！*/
        if (all && (canselect == 0 || canadd(branch[canselect - 1]))) /*如果canselect集合中所有顶点都没有产生冲突，同时要么canselect为0，要么能够添加branch中最后一个顶点*/
        {
            for (int i = canselect; i < (int)branch.size(); ++i) /*循环从canselect位置开始，遍历branch中除了canselect后面的剩余顶点。根据分支规则，这些顶点都是要被删掉的。*/
                delfrD(branch[i]);                               /* 对于每个剩余的顶点，将其从当前候选集中删除*/
            if (canselect)                                       /*如果canselect大于0，即还有可选择的顶点*/
                addtoS(branch[canselect - 1]);                   /*将最后一个可选择的顶点的状态插入到当前解集S中*/
            dfs(curS - branch.size() + canselect);               /*递归地调用dfs函数，传入新的搜索深度，该深度减去已处理的branch顶点数，再加上可选择的顶点数canselect*/
            if (canselect)                                       /*如果canselect大于0，即还有可选择的顶点*/
                delfrS(branch[canselect - 1]);                   /*将最后一个可选择的顶点的状态从解集S中删除，重新放回到候选集D中。*/
            for (int i = canselect; i < (int)branch.size(); ++i) /*对于前面删除过的branch中除了canselect后面的剩余顶点，在结束递归后又重新加回去*/
                addtoD(branch[i]);                               /*重新添加当前顶点到候选集中*/
        }
        for (int i = 0; i <= pos; ++i) /*现在pos就派上用场了，用来将前面添加到解集里面的顶点恢复原来状态，*/
            delfrS(branch[i]);         /*对每个顶点执行撤销状态更改操作，即将其从已选集S中拿出来放回到候选集D中。*/
        delfrS(minID);                 /*最后，将之前插入的度数最小的顶点minID的状态从已选集S中拿出来放回到候选集D中。*/
    }
}

void work() /*核心的函数，调用分支限界方法来求解max s-bundle问题。*/
{
    srand(time(NULL));/*初始化随机数生成器的种子为当前时间。这通常用于确保每次程序运行时产生的随机数序列都是不同的。*/
    startTime = clock(); /*记录函数开始执行时的处理器时钟，这用于计算程序执行的时间。*/
    dfs_node = 0;/*初始化dfs_node变量，用于跟踪在深度优先搜索（DFS）中访问的节点数量。*/ 
    LB = max(wG, K); /*设置变量LB（可能代表下界 Lower Bound）为变量wG和K的最大值。这样设置的原因是s-bundle的最起码大小要为s，也就是这里的K。我感觉这个作者写的代码太烂了，远远不如常老师、师弟和开强的代码质量，后面我要好好的重构。*/    
    for (int i = 0; i < 16; ++i)/*初始化一个名为twoPow的数组。该数组的索引表示2的幂，而数组中的元素表示对应的指数。例如，twoPow[1]将被设置为0，twoPow[2]将被设置为1，twoPow[4]将被设置为2，以此类推。
    这个映射关系通常用于处理二进制位操作。这个循环将twoPow数组初始化为一个指数映射表。这个数组的主要目的是让我们快速的找出某个数是2的几次方。*/
        twoPow[1 << i] = i;
    if (!PreProcess(LB + 1)) /*preProcess的主要作用是根据顶点的度数是否符合kplex来进行删点的操作。如果PreProcess返回false（可能表示预处理失败），则调用exit_program函数退出程序。*/
        exit_program();
    getOrd();  /*根据退化排序degeneracy ordering来确定顶点的顺序，按照顶点的core值从小到大升序排列，并初始了许多基础的信息，例如度数矩阵、邻接表、邻接位图矩阵等。*/
    dfs(n); /*排完序之后执行深度优先搜索函数，这也是整个代码里面最核心的地方。*/
    exit_program(); /*用于退出程序*/
}

void MAIN() /*主要分为两个部分：1）从文件中读取图数据；2）调用work函数。*/
{
    /* 1）从文件中读取图数据；*/
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

    /*2）调用work函数。*/
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
