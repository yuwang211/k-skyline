Points::Points()
{
	t = (Point *)malloc(sizeof(Point) * LEN_INIT);
	plen = LEN_INIT;
	len = 1;
	unused = 0;
	np = 0;
	rp = 0;
}

Points::~Points()
{
	free(t);
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
	t[id].next = unused;
	unused = id;
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
	t[ret].next = 0;
	return ret;
}

inline int Points::getrp()
{
	if (rp == 0) return 0;
	int ret = rp;
	rp = t[rp].next;
	t[ret].next = 0;
	return ret;
}

inline int Points::report(int id, int val)
{
	if (t[id].site > 0)
	{
		--t[id].site;
		t[id].cnt += val - t[id].cntps;
		return val + t[id].cntps;
	}
	else
	{
		t[id].cnt += 0;
		if (t[id].next == 0)
		{
			t[id].next = rp;
			rp = id;
		}
		return 0;
	}
}



