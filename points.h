#ifndef _point_h__
#define _point_h__

#include <cstdio>
#include <cstring>
#include <cstdlib>

class Points{
	public:
		
		static constexpr int LEN_INIT = 10;
		
		struct Point{
			//coordinates
			int x, y;
			//distributed tracking
			int cnt, n_site, site, cntps;
			//list
			int next;
		};
		
		Points();
		~Points();
		
		int insert(int x, int y, int cnt);
		
		Point& operator[] (int id);
		
		void remove(int id);
		
		void setSite(int id, int s);
		
		void init(int id);
		
		int assign(int id);
		
		void collect(int id, int val);
		
		int getnp();
		
		int getrp();
		
		int report(int id, int val);
		
		int np, rp;
		
		Point *t;
		
	private:
	
		
		int unused, len, plen;
		
		//forbidden
		Points(const Points&);
		Points& operator = (const Points&);
		
};


#include "points.cpp"

#endif
