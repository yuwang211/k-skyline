Points::Points()
{
	t = (Point *)malloc(sizeof(Point) * LEN_INIT);
	plen = LEN_INIT;
	len = 1;
	unused = 0;
	np = 0;
	rp = 0;
	mp = 0;
}

Points::~Points()
{
	if (!t) free(t);
}

int Points::insert(int x, int y, int cnt)
{
	int id;
	if (unused > 0)
	{
		id = unused;
		unused = t[unused].next;
	}
	else if (len < plen)
	{
		id = (len++);
	}
	else
	{
		int n_plen = plen + (plen >> 1)  + 1;
		Point *nt = (Point *)malloc(sizeof(Point) * n_plen);
		memcpy(nt, t, sizeof(Point) * plen);
		t = nt;
		plen = n_plen;
		id = (len++);
	}
	t[id].x = x;
	t[id].y = y;
	t[id].cnt = cnt;
	t[id].next = np;
	np = id;
	return id;
}

inline Points::Point& Points::operator[] (int id)
{
	return t[id];
}

void Points::remove(int id)
{
	if (t[id].next == -1)
	{
		t[id].next = mp;
		mp = id;
	}
	else
	{
		printf("ERROR\n");
	}
}

inline void Points::setSite(int id, int s)
{
	t[id].n_site = (s << 1);
}

inline void Points::init(int id)
{
	t[id].site = t[id].n_site;
	t[id].cntps = t[id].cnt / t[id].n_site;
}

inline int Points::assign(int id)
{
	printf("Assign %d, %d,%d\n", t[id].x, t[id].y, t[id].cntps);
	t[id].cnt -= t[id].cntps;
	--t[id].site;
	return t[id].cntps;
}

inline void Points::collect(int id, int val)
{
	t[id].cnt += val;
}

inline int Points::getnp()
{
	if (np == 0) return 0;
	int ret = np;
	np = t[np].next;
	t[ret].next = -1;
	return ret;
}

inline int Points::getrp()
{
	if (rp == 0) return 0;
	int ret = rp;
	rp = t[rp].next;
	t[ret].next = -1;
	return ret;
}

inline int Points::getmp()
{
	if (mp == 0) return 0;
	int ret = mp;
	mp = t[mp].next;
	t[ret].next = unused;
	unused = ret;
	return ret;
}

inline int Points::report(int id, int val)
{
	printf("Report %d, %d,%d\n", t[id].x, t[id].y, val);
	if (t[id].site > 0)
	{
		--t[id].site;
		t[id].cnt += val - t[id].cntps;
		return val + t[id].cntps;
	}
	else
	{
		t[id].cnt += val;
		if (t[id].next == -1)
		{
			t[id].next = rp;
			rp = id;
		}
		return 0;
	}
}



