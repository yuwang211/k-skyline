#ifndef _staticskyline_h__
#define _staticskyline_h__


#include <cstdio>
#include <cstring>
#include <algorithm>
#include "points.h"

class StaticSkyline{
	public:
		
		static constexpr int INF = 0x7F7F7F7F;
		
		StaticSkyline();
		~StaticSkyline();
		
		void build(Points *p);
		
		void destroy();
		
		int count(int x, int y);
		
		int n;
		int n_sgt;
		
		
		
	private:
	
		struct Node{
			//struct
			int key, id, ena, size;
			//tracking
			int val, min, delta;
			
			bool operator < (const Node &p)
			{
				return (key == p.key) ? (id < p.id) : (key < p.key);
			}
		};
	
		Points *pts;
		
		int *key;
		int *sgt;
		Node *bst;
		
		int getRank(int x);
		
		int bst_mid(int l, int r);
		void bst_add(int p, int d);
		void bst_down(int l, int r);
		
		int bst_build(int l, int r);
		void bst_destroy(int l, int r, int d = 0);
		
		void bst_change(int l, int r, int lk, int rk, int d);
		int bst_count(int l, int r, int lk, int rk);
		void bst_scanNegative(int l, int r);
		
		void bst_collect(int l, int r, int key, int id);
		void bst_assign(int l, int r, int key, int id);
		
		
		//tools
		int lowbit(int x){ return x & -x;}
		int min(int x, int y){ return x < y ? x : y;}
		
		//forbidden
		StaticSkyline(const StaticSkyline&);
		StaticSkyline& operator = (const StaticSkyline&);
		
};


#include "staticskyline.cpp"


#endif
