#ifndef _skyline_h__
#define _skyline_h__

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "points.h"
#include "staticskyline.h"

class KSkyline{
	public:
		
		KSkyline(int kp);
		
		~KSkyline();
		
		void insert(int x, int y);
		
		bool report(int &x, int &y);
		
	private:
		
		static constexpr int LOGMAXINT = 32;
		
		int k;
		
		Points *pts;
		
		StaticSkyline s[LOGMAXINT];
		
		//tools
		int lowbit(int x){return x & -x;}
		
		//forbidden
		KSkyline(const KSkyline&);
		KSkyline& operator = (const KSkyline&);
		
};


#include "kskyline.cpp"

#endif
