KSkyline::KSkyline(int kp)
{
	k = kp;
	pts = new Points();
}

KSkyline::~KSkyline()
{
	delete pts;
}

void KSkyline::insert(int x, int y)
{
	int cnt = 0;
	for (int i = 0; i < LOGMAXINT; ++i)
		if (s[i].enable())
			cnt += s[i].count(x, y);
	int id = pts->insert(x, y, k - cnt);
	if (cnt >= k)
	{
		pts->getnp();
		pts->remove(id);
		return;
	}
	
	int cur_size = 1;
	int cur_pos = 0;
	while (s[cur_pos].enable())
	{
		cur_size = s[cur_pos].destroy();
		while (cur_size > (1 << cur_pos)) ++cur_pos;
	}
	
	s[cur_pos].build(pts);
	for (int i = 0; i < LOGMAXINT; ++i)
		if (s[i].enable())
			s[i].add(x, y);
}

bool KSkyline::report(int &x, int &y)
{
	int id = pts->getmp();
	if (id == 0) return false;
	x = pts->t[id].x;
	y = pts->t[id].y;
	return true;
}