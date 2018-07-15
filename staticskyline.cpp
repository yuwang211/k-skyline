
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
	delete[] bst;
	//if (!bst) free(bst);
}

bool StaticSkyline::enable()
{
	return key != NULL;
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
	
	//bst = (Node *)malloc(sizeof(bst) * (sgt[n_sgt] + 3));
	bst = new Node[sgt[n_sgt] + 3];
	bst[0].ena = 0;
	bst[0].size = 0;
	bst[0].min = INF;
	bst[0].l = 0;
	bst[0].r = 0;
	
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
	
	for (int i = n_sgt; i >= 1; --i)
	{
		std::sort(bst + sgt[i - 1], bst + sgt[i]);
		sgt[i] = bst_build(sgt[i - 1], sgt[i] - 1);
	}	
}

int StaticSkyline::destroy()
{
	int ret = 0;
	for (int i = 1; i <= n_sgt; ++i)
		ret += bst_destroy(sgt[i]);
		
	free(key); key = NULL;
	free(sgt); sgt = NULL;
	delete[] bst; bst = NULL;
	//free(bst); bst = NULL;
	return ret;
}

int StaticSkyline::count(int x, int y)
{
	int ret = 0;
	for (int i = 1; i <= n_sgt; i += lowbit(i))
	{
		ret += bst_count(sgt[i], y, INF);
	}
	for (int i = getRank(x) + 1; i <= n_sgt; i += lowbit(i))
	{
		ret -= bst_count(sgt[i], y, INF);
	}
	return ret;
}

void StaticSkyline::add(int x, int y)
{
	for (int i = getPreRank(x) + 1; i <= n_sgt; i += lowbit(i))
	{
		bst_change(sgt[i], -INF, y, -1);
		bst_scanNegative(sgt[i]);
	}
	
	for (int i = pts->getrp(); i != 0; i = pts->getrp())
	{
		for (int j = getRank(pts->t[i].x); j != 0; j -= lowbit(j))
			bst_collect(sgt[j], pts->t[i].y, i);
		printf("(%d, %d): %d\n", pts->t[i].x, pts->t[i].y, pts->t[i].cnt);
		if (pts->t[i].cnt < 0)
		{
			//printf("Point (%d, %d) is removed from the k-skyline\n", pts->t[i].x, pts->t[i].y);
			for (int j = getRank(pts->t[i].x); j != 0; j -= lowbit(j))
				bst_remove(sgt[j], pts->t[i].y, i);
			pts->remove(i);
		}
		else
		{
			pts->init(i);
			for (int j = getRank(pts->t[i].x); j != 0; j -= lowbit(j))
				bst_assign(sgt[j], pts->t[i].y, i);
		}
	}
}

inline int StaticSkyline::getRank(int x)
{
	x = -x;
	int rx = 0;
	for (int i = n_sgt; i > 0; i = (i >> 1))
		while (rx + i <= n_sgt && key[rx + i] <= x)
			rx += i;
	return rx;
}

inline int StaticSkyline::getPreRank(int x)
{
	x = -x;
	int rx = 0;
	for (int i = n_sgt; i > 0; i = (i >> 1))
		while (rx + i <= n_sgt && key[rx + i] < x)
			rx += i;
	return rx;
}


inline void StaticSkyline::bst_add(int p, int d)
{
	bst[p].val += d;
	bst[p].min += d;
	bst[p].delta += d;
}

inline void StaticSkyline::bst_down(int p)
{
	if (bst[p].delta != 0)
	{
		if (bst[p].l != 0) bst_add(bst[p].l, bst[p].delta);
		if (bst[p].r != 0) bst_add(bst[p].l, bst[p].delta);
		bst[p].delta = 0;
	}
}

inline void StaticSkyline::bst_up(int p)
{
	bst[p].size = bst[bst[p].l].size + bst[bst[p].r].size + bst[p].ena;
	bst[p].min = min(min(bst[bst[p].l].min, bst[bst[p].r].min), bst[p].val);
}

int StaticSkyline::bst_build(int l, int r)
{
	if (l > r) return 0;
	int mid = ((l + r) >> 1);
	bst[mid].l = bst_build(l, mid - 1);
	bst[mid].r = bst_build(mid + 1, r);
	bst_up(mid);
	return mid;
}

int StaticSkyline::bst_destroy(int p)
{
	int ret = 0;
	if (p == 0) return 0;
	bst_down(p);
	if (bst[p].ena)
	{
		pts->collect(bst[p].id, bst[p].val);
		if (pts->t[bst[p].id].next != -1)
		{
			++ret;
			pts->t[bst[p].id].next = pts->np;
			pts->np = bst[p].id;
		}
	}
	ret += bst_destroy(bst[p].l);
	ret += bst_destroy(bst[p].r);
	return ret;
}

void StaticSkyline::bst_change(int p, int lk, int rk, int d)
{
	if (p == 0) return;
	if (lk == -INF && rk == INF)
	{
		bst_add(p, d);
		return;
	}
	bst_down(p);
	
	if (lk <= bst[p].key && bst[p].key <= rk)
	{
		bst[p].val += d;
		bst_change(bst[p].l, lk, INF, d);
		bst_change(bst[p].r, -INF, rk, d);
	}
	else if (rk < bst[p].key)
		bst_change(bst[p].l, lk, rk, d);
	else
		bst_change(bst[p].r, lk, rk, d);
	
	bst_up(p);
}

int StaticSkyline::bst_count(int p, int lk, int rk)
{
	if (p == 0) return 0;
	if (lk == -INF && rk == INF)
	{
		return bst[p].size;
	}
	else if (lk <= bst[p].key && bst[p].key <= rk)
	{
		return bst[p].ena + bst_count(bst[p].l, lk, INF) + bst_count(bst[p].r, -INF, rk);
	}
	else if (rk < bst[p].key)
		return bst_count(bst[p].l, lk, rk);
	else
		return bst_count(bst[p].r, lk, rk);
}

void StaticSkyline::bst_scanNegative(int p)
{
	if (p == 0) return;
	bst_down(p);
	if (bst[p].val < 0)
		bst[p].val = pts->report(bst[p].id, bst[p].val);
	if (bst[bst[p].l].min < 0) bst_scanNegative(bst[p].l);
	if (bst[bst[p].r].min < 0) bst_scanNegative(bst[p].r);
	bst_up(p);
}

void StaticSkyline::bst_collect(int p, int key, int id)
{
	if (key == bst[p].key && id == bst[p].id)
	{
		pts->collect(bst[p].id, bst[p].val);
		return;
	}
	bst_down(p);
	if (key < bst[p].key || (key == bst[p].key && id < bst[p].id))
	{
		bst_collect(bst[p].l, key, id);
	}
	else
	{
		bst_collect(bst[p].r, key, id);
	}
}

void StaticSkyline::bst_assign(int p, int key, int id)
{
	if (key == bst[p].key && id == bst[p].id)
	{
		bst[p].val = pts->assign(bst[p].id);
		return;
	}
	bst_down(p);
	if (key < bst[p].key || (key == bst[p].key && id < bst[p].id))
	{
		bst_assign(bst[p].l, key, id);
	}
	else
	{
		bst_assign(bst[p].r, key, id);
	}
	bst_up(p);
}

void StaticSkyline::bst_remove(int p, int key, int id)
{
	if (key == bst[p].key && id == bst[p].id)
	{
		bst[p].val = INF;
		bst[p].ena = 0;
		return;
	}
	bst_down(p);
	if (key < bst[p].key || (key == bst[p].key && id < bst[p].id))
	{
		bst_remove(bst[p].l, key, id);
	}
	else
	{
		bst_remove(bst[p].r, key, id);
	}
	bst_up(p);
}











