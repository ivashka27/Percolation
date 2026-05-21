#include "../inc/UnionFind.h"


UnionFind::UnionFind(size_t size) 
{
    parent.resize(size);
    rank.resize(size,0);

    for (size_t i=0; i<size; ++i) 
    { 
        parent[i] = i;
    }
}

size_t UnionFind::find(size_t v)
{
    if (parent[v] != v) 
    {
        parent[v] = find(parent[v]);
    }
    return parent[v];

}

void UnionFind::unite(size_t a, size_t b)
{
    a = find(a);
    b = find(b);

    if (a == b) return;

    if (rank[a] < rank[b]) 
    {
        parent[a] = b;
    } 
    else if (rank[a] > rank[b]) 
    {
        parent[b] = a;
    } 
    else 
    {
        parent[b] = a;
        ++rank[a];
    }
}

bool UnionFind::connected(size_t a, size_t b)
{
    return find(a) == find(b);
}