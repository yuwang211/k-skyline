
StaticSkyline::StaticSkyline()
{
	key = NULL;
	sgt = NULL;
	bst = NULL;
}


StaticSkyline::~StaticSkyline()
{
	if (!key) free(key);
	if (!sgt) free(sgt);
	if (!bst) free(bst);
}


void StaticSkyline::build(Points *p)
{
	pts = p;
	
	//build sgt

	n= 0;
	for (int i = pts->np; i != 0; i = pts->t[i].next)
		++n;
	key = (int *)malloc(sizeof(int) * (n + 3));
	n = 0;
	for (int i = pts->np; i != 0; i = pts->t[i].next)
		key[++n] = -pts->t[i].x;
	std::sort(key + 1, key + n + 1);
	
	n_sgt = 1;
	for (int i = 2; i <= n; ++i)
	if (key[i] != key[n_sgt])
		key[++n_sgt] = key[i];
		
	sgt = (int *)malloc(sizeof(int) * (n_sgt + 3));
	for (int i = 1; i <= n_sgt; ++i)
		sgt[i] = 0;
	for (int i = pts->np; i != 0; i = pts->t[i].next)
	{
		int t = 0;
		for (int j = getRank(pts->t[i].x); j != 0; j -= lowbit(j))
		{
			++sgt[j];
			++t;
		}
		pts->setSite(i, t);
	}
	sgt[0] = 1;
	for (int i = 1; i <= n_sgt; ++i)
		sgt[i] += sgt[i - 1];
		
	//build bst
	
	bst = (Node *)malloc(sizeof(bst) * (sgt[n_sgt] + 3));
	bst[0].ena = 0;
	bst[0].size = 0;
	bst[0].min = INF;
	
	for (int i = pts->getnp(); i != 0; i = pts->getnp())
	{
		for (int j = getRank(pts->t[i].x); j != 0; j -= lowbit(j))
		{
			int p = (sgt[j-1]++);
			bst[p].id = i;
			bst[p].key = pts->t[i].y;
			bst[p].ena = 1;
			bst[p].val = pts->assign(i);
			bst[p].delta = 0;
		}
	}
	for (int i = n_sgt; i > 0; --i)
		sgt[i] = sgt[i - 1];
	sgt[0] = 1;
	
	for (int i = 1; i <= n_sgt; ++i)
	{
		std::sort(bst + sgt[i - 1], bst + sgt[i]);
		bst_build(sgt[i - 1], sgt[i] - 1);
	}	
}

void StaticSkyline::destroy()
{
	for (int i = 1; i <= n_sgt; ++i)
		bst_destroy(sgt[i - 1], sgt[i] - 1);
		
	free(key); key = NULL;
	free(sgt); sgt = NULL;
	free(bst); bst = NULL;
}

int StaticSkyline::count(int x, int y)
{
	int ret = 0;
	for (int i = getRank(x); i <= n_sgt; i += lowbit(i))
		ret += bst_count(sgt[i - 1], sgt[i] - 1, y, INF);
	return ret;
}

inline int StaticSkyline::getRank(int x)
{
	int rx = 0;
	for (int i = n_sgt; i > 0; i = (i >> 1))
		while (rx + i <= n_sgt && key[rx + i] <= x)
			rx += i;
	return rx;
}

inline int StaticSkyline::bst_mid(int l, int r)
{
	if (l > r) return 0;
	else return ((l + r) >> 1);
}

inline void StaticSkyline::bst_add(int p, int d)
{
	if (p == 0) return;
	bst[p].val += d;
	bst[p].min += d;
	bst[p].delta += d;
}

inline void StaticSkyline::bst_down(int l, int r)
{
	if (l > r) return;
	int mid = ((l + r) >> 1);
	if (bst[mid].delta != 0)
	{
		if (l < mid) bst_add(((l + mid - 1) >> 1), bst[mid].delta);
		if (mid < r) bst_add(((mid + 1 + r) >> 1), bst[mid].delta);
		bst[mid].delta = 0;
	}
}

int StaticSkyline::bst_build(int l, int r)
{
	if (l > r) return 0;
	int mid = ((l + r) >> 1);
	int lc = bst_build(l, mid - 1);
	int rc = bst_build(mid + 1, r);
	bst[mid].size = bst[lc].size + bst[rc].size + 1;
	bst[mid].min = min(min(bst[lc].size, bst[rc].size), bst[mid].val);
}

void StaticSkyline::bst_destroy(int l, int r, int d)
{
	if (l > r) return;
	int mid = ((l + r) >> 1);
	if (bst[mid].ena)
	{
		pts->collect(bst[mid].id, bst[mid].val + d);
		if (pts->t[bst[mid].id].next != -1)
		{
			pts->t[bst[mid].id].next = pts->np;
			pts->np = bst[mid].id;
		}
	}
	d += bst[mid].delta;
	bst_destroy(l, mid - 1, d);
	bst_destroy(mid + 1, r, d);
}

void StaticSkyline::bst_change(int l, int r, int lk, int rk, int d)
{
	if (l > r) return;
	int mid = ((l + r) >> 1);
	if (lk == -INF && rk == INF)
	{
		bst_add(mid, d);
		return;
	}
	if (bst[mid].delta != 0)
	{
		if (l < mid) bst_add(((l + mid - 1) >> 1), bst[mid].delta);
		if (mid < r) bst_add(((mid + 1 + r) >> 1), bst[mid].delta);
		bst[mid].delta = 0;
	}
	
	if (lk <= bst[mid].key && bst[mid].key <= rk)
	{
		bst[mid].val += d;
		bst_change(l, mid - 1, lk, INF, d);
		bst_change(mid + 1, r, -INF, rk, d);
	}
	else if (rk < bst[mid].key)
		bst_change(l, mid - 1, lk, rk, d);
	else
		bst_change(mid + 1, r, lk, rk, d);
		
	int lc = bst_mid(l, mid - 1);
	int rc = bst_mid(mid + 1, r);
	bst[mid].size = bst[lc].size + bst[rc].size + 1;
	bst[mid].min = min(min(bst[lc].size, bst[rc].size), bst[mid].val);
}

int StaticSkyline::bst_count(int l, int r, int lk, int rk)
{
	if (l > r) return 0;
	
	int mid = ((l + r) >> 1);
	if (lk == -INF && rk == INF)
	{
		return bst[mid].size;
	}
	else if (lk <= bst[mid].key && bst[mid].key <= rk)
	{
		return bst[mid].ena + bst_count(l, mid - 1, lk, INF) + bst_count(mid + 1, r, -INF, rk);
	}
	else if (rk < bst[mid].key)
		return bst_count(l, mid - 1, lk, rk);
	else
		return bst_count(mid + 1, r, lk, rk);
		
}

void StaticSkyline::bst_scanNegative(int l, int r)
{
	if (l > r) return;
	int mid = ((l + r) >> 1);
	if (bst[mid].delta != 0)
	{
		if (l < mid) bst_add(((l + mid - 1) >> 1), bst[mid].delta);
		if (mid < r) bst_add(((mid + 1 + r) >> 1), bst[mid].delta);
		bst[mid].delta = 0;
	}
	
	if (bst[mid].val < 0)
		bst[mid].val = pts->report(bst[mid].id, bst[mid].val);
		
	int lc = bst_mid(l, mid - 1);
	int rc = bst_mid(mid + 1, r);
	if (bst[lc].min < 0) bst_scanNegative(l, mid - 1);
	if (bst[rc].min < 0) bst_scanNegative(mid + 1, r);
	
	bst[mid].size = bst[lc].size + bst[rc].size + 1;
	bst[mid].min = min(min(bst[lc].size, bst[rc].size), bst[mid].val);
}

void StaticSkyline::bst_collect(int l, int r, int key, int id)
{
	int mid = ((l + r) >> 1);
	
	if (key == bst[mid].key && id == bst[mid].id)
	{
		pts->collect(bst[mid].id, bst[mid].val);
		return;
	}
	
	if (bst[mid].delta != 0)
	{
		if (l < mid) bst_add(((l + mid - 1) >> 1), bst[mid].delta);
		if (mid < r) bst_add(((mid + 1 + r) >> 1), bst[mid].delta);
		bst[mid].delta = 0;
	}
	
	if (key < bst[mid].key || (key == bst[mid].key && id < bst[mid].id))
	{
		bst_collect(l, mid - 1, key, id);
	}
	else
	{
		bst_collect(mid + 1, r, key, id);
	}
}

void StaticSkyline::bst_assign(int l, int r, int key, int id)
{
	int mid = ((l + r) >> 1);
	
	if (key == bst[mid].key && id == bst[mid].id)
	{
		bst[mid].val = pts->assign(bst[mid].id);
		return;
	}
	
	if (bst[mid].delta != 0)
	{
		if (l < mid) bst_add(((l + mid - 1) >> 1), bst[mid].delta);
		if (mid < r) bst_add(((mid + 1 + r) >> 1), bst[mid].delta);
		bst[mid].delta = 0;
	}
	
	if (key < bst[mid].key || (key == bst[mid].key && id < bst[mid].id))
	{
		bst_assign(l, mid - 1, key, id);
	}
	else
	{
		bst_assign(mid + 1, r, key, id);
	}
	
	int lc = bst_mid(l, mid - 1);
	int rc = bst_mid(mid + 1, r);
	bst[mid].size = bst[lc].size + bst[rc].size + 1;
	bst[mid].min = min(min(bst[lc].size, bst[rc].size), bst[mid].val);
}











