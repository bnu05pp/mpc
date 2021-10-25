#include "graph.h"
#include <fstream>
#include <sstream>
#include <time.h>
//choice is used to determine if a property is selected as internal, 1 means internal

graph::graph() {}

graph::~graph() {}

void graph::init()
{
	vector<pair<int, int>> tmp1;
	bitset<EMAX> *tmp2 = new bitset<EMAX>;
	edge.push_back(tmp1);
	subjectSetOfPredicate.push_back(*tmp2);
	objectSetOfPredicate.push_back(*tmp2);
	delete tmp2;

	entityTriples.push_back(0);

	IDToEntity.push_back("");
	IDToPredicate.push_back("");
	preType = entityCnt = triples = invalidEdgeCnt = 0;
}

int graph::getPreNum()
{
	return predicate.size();
}

long long graph::getEntityNum()
{
	return (long long)IDToEntity.size() - 1;
}

void graph::loadGraph(string txt_name, string tag)
{
	ifstream in(txt_name);

	
	cout << txt_name << "========" << endl;
	vector<pair<int, int>> tmp1;
	bitset<EMAX> *tmp2 = new bitset<EMAX>;
	string line;
	while (getline(in, line))
	{
		if (triples % 10000 == 0)
			cout << "loading triples : " << triples << endl;
		triples++;
		line.resize(line.length() - 2);
		vector<string> s;
		s = split(line, tag);

		predicate.insert(s[1]);

		// To judge if this entity is valid
		for (int i = 0; i < 3; i += 2)
			if ((s[i][0] == '<' || s[i][0] == '_') && entityToID.count(s[i]) == 0)
			{
				entityToID[s[i]] = ++entityCnt;
				IDToEntity.push_back(s[i]);
				entityTriples.push_back(0);
			}

		edge_cnt[s[1]]++;
		int a = entityToID[s[0]];
		int b = entityToID[s[2]];
		entityTriples[a]++;

		// 1. To judge if this predicate & rdf is valid
		// 2. If predicate is valid, add subject and object to Set
		if ((s[0][0] == '<' || s[0][0] == '_') && (s[2][0] == '<' || s[2][0] == '_'))
		{
			if (predicateToID.count(s[1]) == 0)
			{
				predicateToID[s[1]] = ++preType;
				subjectSetOfPredicate.push_back(*tmp2);
				objectSetOfPredicate.push_back(*tmp2);
				IDToPredicate.push_back(s[1]);
				edge.push_back(tmp1);
			}
			int preid = predicateToID[s[1]];
			edge[preid].push_back(make_pair(a, b));
			entityTriples[b]++;
			subjectSetOfPredicate[preid].set(a, 1);
			objectSetOfPredicate[preid].set(b, 1);
		}
	}
	in.close();
	// for (int i = 1; i <= preType; ++i)
	// 	cout << "property id " << i << " : " << edge[i].size() << ' ' << subjectSetOfPredicate[i].count() << ' ' << objectSetOfPredicate[i].count() << endl;

	limit = entityCnt / part / 2;
	printf("limit: %lld\n", limit);
	printf("triples: %lld\n", triples);
	printf("entityCnt: %lld\n", entityCnt);
	printf("predicate: %lu\n", predicate.size());

	printf("entity->preType: %d\n", preType);

	printf("sizeof edge_cnt : %ld\n", edge_cnt.size());
	delete tmp2;
}

// split strings to vector from textline seperated by tag
vector<string>
graph::split(string textline, string tag)
{
	vector<string> res;
	std::size_t pre_pos = 0;
	std::size_t pos = textline.find(tag);

	while (pos != std::string::npos)
	{
		string curStr = textline.substr(pre_pos, pos - pre_pos);
		curStr.erase(0, curStr.find_first_not_of("\r\t\n "));
		curStr.erase(curStr.find_last_not_of("\r\t\n ") + 1);

		if (strcmp(curStr.c_str(), "") != 0)
			res.push_back(curStr);
		pre_pos = pos + tag.size();
		pos = textline.find(tag, pre_pos);
	}

	string curStr = textline.substr(pre_pos, pos - pre_pos);
	curStr.erase(0, curStr.find_first_not_of("\r\t\n "));
	curStr.erase(curStr.find_last_not_of("\r\t\n ") + 1);
	if (strcmp(curStr.c_str(), "") != 0)
		res.push_back(curStr);

	return res;
}

int graph::getParent(int son, vector<int> &fa)
{
	// return fa[son]==son?son:fa[son]=getParent(fa[son],fa);
	int i, j, k;
	k = son;
	while (k != fa[k])
		k = fa[k];
	i = son;
	while (i != k)
	{
		j = fa[i];
		fa[i] = k;
		i = j;
	}
	return k;
}

int graph::getParentMap(int son, unordered_map<int, int> &fa)
{
	int i, j, k;
	k = son;
	while (k != fa[k])
		k = fa[k];
	i = son;
	while (i != k)
	{
		j = fa[i];
		fa[i] = k;
		i = j;
	}
	return k;
}

void graph::coarsening()
{
	invalid = vector<bool>(preType + 1, 0);
	coarseningPoint = vector<unordered_map<int, int>>(preType + 1, unordered_map<int, int>());
	for (int preID = 1; preID <= preType; preID++)
	{
		vector<int> sonCnt = vector<int>(entityCnt + 1, 1);
		vector<int> rank = vector<int>(entityCnt + 1, 0);

		for (int p = 0; p < edge[preID].size(); p++)
		{
			int A = edge[preID][p].first, B = edge[preID][p].second;

			// 孤立点, 以自环形式加入 标签为preID 的WCC
			if (coarseningPoint[preID].count(A) == 0)
				coarseningPoint[preID].insert(make_pair(A, A));
			if (coarseningPoint[preID].count(B) == 0)
				coarseningPoint[preID].insert(make_pair(B, B));

			int parentA = getParentMap(A, coarseningPoint[preID]), parentB = getParentMap(B, coarseningPoint[preID]);

			if (rank[parentA] < rank[parentB])
				swap(parentA, parentB);

			if (parentA != parentB)
			{
				coarseningPoint[preID][parentB] = parentA;
				sonCnt[parentA] += sonCnt[parentB];
				rank[parentA] = max(rank[parentA], rank[parentB] + 1);

				// 某个WCC规模超过了limit
				if (sonCnt[parentA] > limit)
				{
					invalid[preID] = 1;
					invalidEdgeCnt++;
					printf("invalid: %d %s\n", preID, IDToPredicate[preID].data());
					break;
				}
			}
		}
	}
}

void graph::unionEdgeForEnum()
{
	coarsening();

	bool *invalidST = new bool[1 << preType]();
	for (int i = 1; i <= preType; i++)
		if (invalid[i])
			invalidST[1 << (i - 1)] = 1;

	// ans 1 -> internal 2 -> crossing
	ans = 0;
	vector<int> choice(preType + 1, 0);
	vector<int> parent = vector<int>(entityCnt + 1);
	for (int i = 1; i <= entityCnt; i++)
		parent[i] = i;
	vector<int> sonCnt = vector<int>(entityCnt + 1, 1);
	vector<int> rank = vector<int>(entityCnt + 1, 0);

	crossEgdeCnt = preType;
	puts("\n==========================================================================================\n");
	printf("enumPre\n");
	enumPre(0, 0, parent, sonCnt, rank, invalidST);

	printf("crossEgdeCnt: %d\n", preType - cal(ans));
	for (int i = 1; i <= preType; i++)
	{
		choice[i] = ((1 << (i - 1)) & ans) ? 1 : 0;
		if (choice[i] == 0)
			cout << "crossEdge : " << i << "	" << IDToPredicate[i] << endl;
	}
	puts("\n==========================================================================================\n");
	unionBlock(choice, part);
	delete[] invalidST;
}

// dfs enumerate all 2 ^ preID circumstances
void graph::enumPre(int preID, long long choice, vector<int> &curParent, vector<int> &curSonCnt, vector<int> &curRank, bool *invalidST)
{
	// optimal trim
	if (cal(ans) >= cal(choice) + preType - preID - invalidEdgeCnt)
		return;

	// bound reached, compare with current optimal ans
	if (preID == preType)
	{
		printf("choice(Binary) when preID == preType:%lld\n", choice);
		compareCrossingEdgeCnt(choice);
		return;
	}
	preID++;

	// add current preID into internal set
	long long nextchoice = choice | (1LL << (preID - 1));

	// flag 1 -> invalid
	bool flag = invalidST[nextchoice] | invalidST[1LL << (preID - 1)];

	if (!flag)
	{
		vector<int> nextFa(curParent);
		vector<int> nextSonCnt(curSonCnt);
		vector<int> nextRank(curRank);
		for (const auto &it : coarseningPoint[preID])
		{
			int point = it.first;
			int parentA = getParent(point, nextFa), parentB = getParent(getParentMap(point, coarseningPoint[preID]), nextFa);
			if (nextRank[parentA] < nextRank[parentB])
				swap(parentA, parentB);
			if (parentA != parentB)
			{
				nextFa[parentB] = parentA;
				nextSonCnt[parentA] += nextSonCnt[parentB];
				nextRank[parentA] = max(nextRank[parentA], nextRank[parentB] + 1);
				if (nextSonCnt[parentA] > limit)
				{
					flag = 1;
					break;
				}
			}
		}

		if (!flag)
			enumPre(preID, nextchoice, nextFa, nextSonCnt, nextRank, invalidST);
	}

	// if preID is invalid, just tag and skip it
	if (flag)
	{
		for (int i = 1; i <= preType; i++)
			invalidST[(1LL << (i - 1)) | nextchoice] = 1;
	}
	enumPre(preID, choice, curParent, curSonCnt, curRank, invalidST);
}

void graph::greed1(vector<int> &choice, vector<int> &curParent, vector<int> &curSonCnt, vector<int> &curRank, vector<bool> &invalid, vector<int> &internalPre)
{
	double nextBestSimilarity = 0;
	auto &s0 = subjectSetOfPredicate[0], &o0 = objectSetOfPredicate[0];
	int nextBestBlockNum = 0;
	int chosen_preID = 0;
	int choose_flag = 0; // 1 -> similarity  2 -> WCC

	vector<int> nextBestParent;
	vector<int> nextBestSonCnt;
	vector<int> nextBestRank;
	vector<int> nextBestChoice(choice);
	// Use similarity stratety when internal set is not empty
	if (!internalPre.empty())
	{
		choose_flag = 1;
		for (int preID = 1; preID <= preType; preID++)
		{
			if (choice[preID] == 0 && !invalid[preID])
			{
				double nextSimilarity = 0;
				auto &s1 = subjectSetOfPredicate[preID], &o1 = objectSetOfPredicate[preID];

				double s_and_size = (s0 & s1).count(), s_or_size = (s0 | s1).count();
				double o_and_size = (o0 & o1).count(), o_or_size = (o0 | o1).count();
				nextSimilarity = (s_and_size / s_or_size + o_and_size / o_or_size) / 2;
				// cout << preID << ' ' << nextSimilarity << ' ' << (s_and_size / s_or_size) << ' ' << (o_and_size / o_or_size) << endl;

				if (!nextBestSimilarity || nextBestSimilarity < nextSimilarity)
				{
					nextBestSimilarity = nextSimilarity;
					nextBestChoice.assign(choice.begin(), choice.end());
					chosen_preID = preID;
					nextBestChoice[preID] = 1;
				}
			}
		}
	}

	// Use WCC stratety when internal set is empty(first time greedy choosing) OR similarity failed
	if (internalPre.empty() || nextBestSimilarity == 0)
	{
		choose_flag = 2;
		for (int preID = 1; preID <= preType; preID++)
			if (choice[preID] == 0 && !invalid[preID])
			{
				vector<int> nextParent(curParent);
				vector<int> nextSonCnt(curSonCnt);
				vector<int> nextRank(curRank);
				int nextBlockNum = 0;
				bool flag = 0;
				for (const auto &it : coarseningPoint[preID])
				{
					int point = it.first;
					int parentA = getParent(point, nextParent), parentB = getParent(getParentMap(point, coarseningPoint[preID]), nextParent);
					if (nextRank[parentA] < nextRank[parentB])
						swap(parentA, parentB);
					if (parentA != parentB)
					{
						nextParent[parentB] = parentA;
						nextSonCnt[parentA] += nextSonCnt[parentB];
						nextRank[parentA] = max(nextRank[parentA], nextRank[parentB] + 1);
						if (nextSonCnt[parentA] > limit)
						{
							flag = 1;
							break;
						}
					}
				}
				if (flag)
					continue;
				for (int p = 1; p <= entityCnt; p++)
					if (getParent(p, nextParent) == p)
						nextBlockNum++;
				if (!nextBestBlockNum || nextBestBlockNum < nextBlockNum)
				{
					nextBestBlockNum = nextBlockNum;
					nextBestParent.assign(nextParent.begin(), nextParent.end());
					nextBestSonCnt.assign(nextSonCnt.begin(), nextSonCnt.end());
					nextBestRank.assign(nextRank.begin(), nextRank.end());
					nextBestChoice.assign(choice.begin(), choice.end());
					nextBestChoice[preID] = 1;
					chosen_preID = preID;
				}
			}
	}

	internalPre.push_back(chosen_preID);

	bool greed_stop_flag = false;

	// merge the disjoint forest to the current one
	if (choose_flag == 1)
	{
		for (auto &it : coarseningPoint[chosen_preID])
		{
			int point = it.first;
			int parentA = getParent(point, curParent), parentB = getParent(getParentMap(point, coarseningPoint[chosen_preID]), curParent);
			if (curRank[parentA] < curRank[parentB])
				swap(parentA, parentB);
			if (parentA != parentB)
			{
				curParent[parentB] = parentA;
				curSonCnt[parentA] += curSonCnt[parentB];
				curRank[parentA] = max(curRank[parentA], curRank[parentB] + 1);
				if (curSonCnt[parentA] > limit)
				{
					greed_stop_flag = 1;
					break;
				}
			}
		}
	}
	// assign the disjoint forest to the current one
	else if (choose_flag == 2)
	{
		curParent.assign(nextBestParent.begin(), nextBestParent.end());
		curSonCnt.assign(nextBestSonCnt.begin(), nextBestSonCnt.end());
		curRank.assign(nextBestRank.begin(), nextBestRank.end());
	}

	choice.assign(nextBestChoice.begin(), nextBestChoice.end());

	cout << "choose_flag : " << choose_flag << endl;
	cout << "select " << chosen_preID << " : " + IDToPredicate[chosen_preID] << " as internal predicate." << endl;
	cout << "internal size : " << internalPre.size() << endl;

	s0 |= subjectSetOfPredicate[chosen_preID], o0 |= objectSetOfPredicate[chosen_preID];
	puts("\n======================================================================\n");

	if (internalPre.size() == preType - 1)
		greed_stop_flag = 1;
	if (!greed_stop_flag)
		greed1(choice, curParent, curSonCnt, curRank, invalid, internalPre);
}

void graph::unionEdgeForGreed1_2(int greedMode)
{
	coarsening();
	vector<int> internalPre;
	ans = 0;
	vector<int> choice(preType + 1, 0);
	vector<int> parent = vector<int>(entityCnt + 1);
	for (int i = 1; i <= entityCnt; i++)
		parent[i] = i;
	vector<int> sonCnt = vector<int>(entityCnt + 1, 1);
	vector<int> rank = vector<int>(entityCnt + 1, 1);
	puts("\n==========================================================================================\n");
	cout << "greed" << greedMode << endl;

	int threshold = entityCnt * 0.0001;

	int optim = 0;
	for (int preID = 1; preID <= preType; preID++)
	{
		// if the edge cnt of preID is in smaller than threshold, choose it as internal
		if (edge_cnt[IDToPredicate[preID]] < threshold)
		{
			for (int p = 0; p < edge[preID].size(); p++)
			{

				int A = edge[preID][p].first, B = edge[preID][p].second;
				int parentA = getParent(A, parent), parentB = getParent(B, parent);

				if (rank[parentA] < rank[parentB])
					swap(parentA, parentB);
				if (parentA != parentB)
				{
					parent[parentB] = parentA;
					sonCnt[parentA] += sonCnt[parentB];
					rank[parentA] = max(rank[parentA], rank[parentB] + 1);
				}
			}
			choice[preID] = 1;
			optim++;
			internalPre.push_back(preID);
		}
	}

	printf("opt: %d\n", optim);
	if (greedMode == 1)
		greed1(choice, parent, sonCnt, rank, invalid, internalPre);
	if (greedMode == 2)
		greed2(choice, parent, sonCnt, rank, invalid);

	puts("\n==========================================================================================\n");
	int crossEdge = 0;
	for (int preID = 1; preID <= preType; preID++)
		if (choice[preID] == 0)
			cout << preID << "	" << IDToPredicate[preID] << endl, crossEdge++;
		
	printf("crossEdge: %d\n", crossEdge);
	puts("\n==========================================================================================\n");
	unionBlock(choice, part);
}

void graph::greed2(vector<int> &choice, vector<int> &curParent, vector<int> &curSonCnt, vector<int> &curRank, vector<bool> &invalid)
{
	int nextBestBlockNum = 0;
	int chosen_preID = 0;
	if (true)
	{
		vector<int> nextBestParent;
		vector<int> nextBestSonCnt;
		vector<int> nextBestRank;
		vector<int> nextBestChoice(choice);
		for (int preID = 1; preID <= preType; preID++)
			if (choice[preID] == 0 && !invalid[preID])
			{
				vector<int> nextParent(curParent);
				vector<int> nextSonCnt(curSonCnt);
				vector<int> nextRank(curRank);
				int nextBlockNum = 0;
				bool flag = 0;
				for (auto &it : coarseningPoint[preID])
				{
					int point = it.first;
					int parentA = getParent(point, nextParent), parentB = getParent(getParentMap(point, coarseningPoint[preID]), nextParent);
					if (nextRank[parentA] < nextRank[parentB])
						swap(parentA, parentB);
					if (parentA != parentB)
					{
						nextParent[parentB] = parentA;
						nextSonCnt[parentA] += nextSonCnt[parentB];
						nextRank[parentA] = max(nextRank[parentA], nextRank[parentB] + 1);
						if (nextSonCnt[parentA] > limit)
						{
							flag = 1;
							break;
						}
					}
				}
				if (flag)
					continue;
				for (int p = 1; p <= entityCnt; p++)
					if (getParent(p, nextParent) == p)
						nextBlockNum++;
				if (!nextBestBlockNum || nextBestBlockNum < nextBlockNum)
				{
					nextBestBlockNum = nextBlockNum;
					nextBestParent.assign(nextParent.begin(), nextParent.end());
					nextBestSonCnt.assign(nextSonCnt.begin(), nextSonCnt.end());
					nextBestRank.assign(nextRank.begin(), nextRank.end());
					nextBestChoice.assign(choice.begin(), choice.end());
					nextBestChoice[preID] = 1;
					chosen_preID = preID;
				}
			}
		choice.assign(nextBestChoice.begin(), nextBestChoice.end());
		curParent.assign(nextBestParent.begin(), nextBestParent.end());
		curSonCnt.assign(nextBestSonCnt.begin(), nextBestSonCnt.end());
		curRank.assign(nextBestRank.begin(), nextBestRank.end());

		if (chosen_preID)
			cout << "select " << chosen_preID << " : " + IDToPredicate[chosen_preID] << " as internal predicate." << endl;
	}
	if (nextBestBlockNum)
		greed2(choice, curParent, curSonCnt, curRank, invalid);
}

//count the number of 1 in (binary)cur
int graph::cal(long long cur)
{
	int ret = 0;
	while (cur)
		ret += (cur & 1), cur >>= 1;
	return ret;
}

// To maximize the size of internal set
// judge if the current choice has more internal property than the current optimal
void graph::compareCrossingEdgeCnt(long long cur)
{
	int c = 0, cnt = 0;
	for (int i = 0; i < preType; i++)
	{
		if (cur & (1LL << i))
			c++;
		else
			cnt += edge[i + 1].size();
	}
	if (cal(ans) < c)
		ans = cur, crossEgdeCnt = cnt;
	else if (cal(ans) == c && crossEgdeCnt > cnt)
		ans = cur, crossEgdeCnt = cnt;
}

void graph::greed3()
{
	puts("\n==========================================================================================\n");
	printf("greed3\n");
	invalid = vector<bool>(preType + 1, 0);
	int threshold = entityCnt * 0.0001;
	vector<int> fa(entityCnt + 1);
	vector<int> FA(entityCnt + 1);
	for (int i = 1; i <= entityCnt; i++)
		fa[i] = FA[i] = i;
	vector<int> RANK(entityCnt + 1, 0);

	//SONCNT record the size of each weakly connected component in final result
	vector<int> SONCNT(entityCnt + 1, 1);

	vector<int> choice(preType + 1, 0);
	vector<pair<int, int>> arr;

	for (int preID = 1; preID <= preType; preID++)
	{
		// if the edge cnt of preID is in smaller than threshold, choose it as internal
		if (edge_cnt[IDToPredicate[preID]] < threshold)
		{
			for (int p = 0; p < edge[preID].size(); p++)
			{
				int A = edge[preID][p].first, B = edge[preID][p].second;
				int parentA = getParent(A, FA), parentB = getParent(B, FA);

				if (RANK[parentA] < RANK[parentB])
					swap(parentA, parentB);
				if (parentA != parentB)
				{
					FA[parentB] = parentA;
					SONCNT[parentA] += SONCNT[parentB];
					RANK[parentA] = max(RANK[parentA], RANK[parentB] + 1);
				}
			}
			choice[preID] = 1;
		}
		else
		{
			vector<int> parent(fa);
			vector<int> sonCnt = vector<int>(entityCnt + 1, 1);

			for (int p = 0; p < edge[preID].size(); p++)
			{

				int A = edge[preID][p].first, B = edge[preID][p].second;
				int parentA = getParent(A, parent), parentB = getParent(B, parent);

				if (parentA != parentB)
				{
					parent[parentB] = parentA;
					//sonCnt record the size of each weakly connected component in intermediate result
					sonCnt[parentA] += sonCnt[parentB];
					if (sonCnt[parentA] > limit)
					{
						invalid[preID] = 1;
						printf("invalid: %d\n", preID);
						break;
					}
				}
			}
			if (invalid[preID])
				continue;

			//sorting properties by the numbers of weakly connected components
			int wcc_cnt = 0;
			for (int p = 1; p <= entityCnt; p++)
				if (getParent(p, parent) == p)
					wcc_cnt++;
			arr.push_back({wcc_cnt, preID});
		}
	}

	sort(arr.begin(), arr.end());
	// put as many properties as possbile into the internal properties
	for (int i = arr.size() - 1; i >= 0; i--)
	{
		int preID = arr[i].second;
		for (int p = 0; p < edge[preID].size(); p++)
		{

			int A = edge[preID][p].first, B = edge[preID][p].second;
			int parentA = getParent(A, FA), parentB = getParent(B, FA);

			if (RANK[parentA] < RANK[parentB])
				swap(parentA, parentB);
			if (parentA != parentB)
			{
				FA[parentB] = parentA;
				SONCNT[parentA] += SONCNT[parentB];
				RANK[parentA] = max(RANK[parentA], RANK[parentB] + 1);

				if (SONCNT[parentA] > limit)
				{
					invalid[preID] = 1;
					break;
				}
			}
		}
		if (invalid[preID])
			break;
		choice[preID] = 1;
	}

	int crossEdge = 0;
	for (int preID = 1; preID <= preType; preID++)
		if (choice[preID] == 0)
			cout << preID << "	" << IDToPredicate[preID] << endl, crossEdge++;
	printf("crossEdge: %d\n", crossEdge);
	puts("\n==========================================================================================\n");
	unionBlock(choice, part);
}

void graph::unionBlock(vector<int> &choice, int goal)
{
	cout << "unionBlock: " << endl;
	vector<int> parent(entityCnt + 1);
	for (int p = 1; p <= entityCnt; p++)
		parent[p] = p;
	vector<int> rank = vector<int>(entityCnt + 1, 0);
	for (int preID = 1; preID <= preType; preID++)
		if (choice[preID] == 1)
		{
			for (int p = 0; p < edge[preID].size(); p++)
			{
				int parentA = getParent(edge[preID][p].first, parent), parentB = getParent(edge[preID][p].second, parent);
				if (rank[parentA] < rank[parentB])
					swap(parentA, parentB);
				if (parentA != parentB)
				{
					rank[parentA] = max(rank[parentA], rank[parentB] + 1);
					parent[parentB] = parentA;
					entityTriples[parentA] += entityTriples[parentB];
				}
			}
		}
	vector<pair<int, int>> block;
	int blockNum = 0;
	for (int p = 1; p <= entityCnt; p++)
		if (p == getParent(p, parent))
			block.push_back(make_pair(entityTriples[p], p)), ++blockNum;
	printf("blockNum: %d\n", blockNum);

	sort(block.begin(), block.end());

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> Q;

	for (int i = 1; i <= goal; i++)
		Q.push(make_pair(0, i));
	vector<int> blockTogoal(entityCnt + 1, 0);
	for (int i = block.size() - 1; i >= 0; i--)
	{
		pair<int, int> tmp = Q.top();
		Q.pop();
		tmp.first += block[i].first;

		//tmp = make_pair(block[i].first, 加入序号)
		blockTogoal[block[i].second] = tmp.second;
		Q.push(tmp);
	}

	while (!Q.empty())
	{
		printf("%d %d\n", Q.top().first, Q.top().second);
		Q.pop();
	}

	vector<int> CNT(goal + 1, 0);

	ofstream outFile(RDF + "InternalPoints.txt");
	for (int pos, p = 1; p <= entityCnt; p++)
	{
		string t = IDToEntity[p];
		pos = blockTogoal[getParent(p, parent)];
		int groupID = pos - 1;
		group[t] = groupID;
		outFile << t << "	" << groupID << endl;
		CNT[pos]++;
	}
	outFile.close();
	printf("\n");
	for (int i = 1; i <= goal; i++)
		printf("%d %d\n", i, CNT[i]);

	ofstream File(RDF + "crossingEdges.txt");
	for (unordered_map<string, int>::iterator it = edge_cnt.begin(); it != edge_cnt.end(); it++)
	{
		File << it->first << "\t" << it->second << "\t";
		if (predicateToID.count(it->first))
			File << (!choice[predicateToID[it->first]]);
		else
			File << "0";
		File << endl;
	}
	File.close();
}

void graph::randEntity(string txt_name, string tag)
{
	ifstream in(txt_name.data());
	string line;
	while (getline(in, line))
	{
		triples++;
		line.resize(line.length() - 2);
		vector<string> s;
		s = split(line, tag);
		predicate.insert(s[1]);
		for (int i = 0; i < 3; i += 2)
			if ((s[i][0] == '<' || s[i][0] == '_') && entityToID.count(s[i]) == 0)
			{
				entityToID[s[i]] = ++entityCnt;
				IDToEntity.push_back(s[i]);
			}
	}
	in.close();
	ofstream outFile(RDF + "sub_hash_InternalPoints.txt");
	for (int i = 1; i <= entityCnt; i++)
	{
		outFile << IDToEntity[i] << "\t" << rand() % part << "\n";
	}
	outFile.close();
	// update();
}

void graph::randPre(string txt_name, string tag)
{
	map<string, int> preToID;
	map<int, string> IDToPre;
	int triples = 0;
	int preCnt = 0;
	vector<pair<long long, int>> block;

	ifstream in(txt_name.data());
	string line;
	while (getline(in, line))
	{
		triples++;
		if (triples % 10000 == 0)
			cout << "loading triples for vertical partitioning : " << triples << endl;
		line.resize(line.length() - 2);
		vector<string> s;
		s = split(line, tag);
		//predicate.insert(s[1]);
		string _pre = s[1];
		if (preToID.count(_pre) == 0)
		{
			preToID[_pre] = ++preCnt;
			IDToPre[preCnt] = _pre;
			block.push_back(make_pair(0, preCnt));
		}
		block[preToID[_pre] - 1].first++;
	}
	in.close();

	sort(block.begin(), block.end());
	vector<int> pre_pos(preCnt + 1);
	priority_queue<pair<long long, int>> q;
	for (int i = 0; i < part; i++)
		q.push(make_pair(0, i));
	ofstream outFile(RDF + "vp_InternalPoints.txt");
	for (int i = block.size() - 1; i >= 0; i--)
	{
		pair<long long, int> tmp = q.top();
		q.pop();
		tmp.first -= block[i].first;
		pre_pos[block[i].second] = tmp.second;
		q.push(tmp);
		outFile << IDToPre[block[i].second] << "\t" << tmp.second << endl;
	}
	outFile.close();
	// update();
}

void graph::metis(string txt_name, string tag)
{
	ifstream in(txt_name.data());
	string str;
	entityCnt = 0;
	vector<vector<int>> EDGE;
	EDGE.push_back(vector<int>());
	triples = 0;
	int edge_count = 0;
	while (getline(in, str))
	{
		triples++;
		if (triples % 10000 == 0)
			cout << "loading triples : " << triples << endl;
		str.resize(str.length() - 2);
		vector<string> s;
		s = split(str, tag);
		predicate.insert(s[1]);
		for (int i = 0; i < 3; i += 2)
			if ((s[i][0] == '<' || s[i][0] == '_') && entityToID.count(s[i]) == 0)
			{
				entityToID[s[i]] = ++entityCnt;
				IDToEntity.push_back(s[i]);
				EDGE.push_back(vector<int>());
			}
		if (entityToID.count(s[0]) != 0 && entityToID.count(s[2]) != 0)
		{
			EDGE[entityToID[s[0]]].push_back(entityToID[s[2]]);
			EDGE[entityToID[s[2]]].push_back(entityToID[s[0]]);
			edge_count++;
		}
	}

	ofstream out((RDF + ".tmp").data());
	out << entityCnt << " " << edge_count << endl;
	for (int i = 1; i < EDGE.size(); i++)
	{
		for (int j = 0; j < EDGE[i].size(); j++)
			out << EDGE[i][j] << " ";
		out << endl;
	}

	stringstream cmd_ss;
	cmd_ss << "./gpmetis " << RDF << ".tmp " << part;
	cout << cmd_ss.str().c_str() << endl;
	system(cmd_ss.str().c_str());
	stringstream metis_ss;
	metis_ss << "./" << RDF << ".tmp.part." << part;
	cout << metis_ss.str().c_str() << endl;
	ifstream In(metis_ss.str().c_str());
	ofstream Out(("./" + RDF + "METISInternalPoints.txt").data());
	int idx = 1;
	while (getline(In, str))
	{
		Out << IDToEntity[idx++] << "\t" << atoi(str.c_str()) << endl;
	}
	//update();
}

void graph::partition(string txt_name, string tag, string out_file)
{
	string line;
	ifstream readGraph(txt_name.data());
	triples = 0;

	vector<pair<pair<string, string>, string>> tmp;
	for (int i = 1; i <= part; ++i)
		edgeGroup.push_back(tmp);

	while (getline(readGraph, line))
	{
		triples++;
		if (triples % 10000 == 0)
			cout << "grouping triples : " << triples << endl;
		line.resize(line.length() - 2);
		vector<string> s;
		s = split(line, tag);

		int u = entityToID[s[0]], v = entityToID[s[2]], p = predicateToID[s[1]];
		// // if(u == 0)	continue;
		int uID = group[s[0]], vID = group[s[2]];
		// cout << triples << ":" << u << " " << v << " ";
		edgeGroup[uID].push_back({{s[0], s[2]}, s[1]});
		if (uID != vID && v != 0)
			edgeGroup[vID].push_back({{s[0], s[2]}, s[1]});
	}

	int partCnt = part;

	for (int i = 0; i < partCnt; ++i)
	{
		string SubGraphName = out_file + to_string(i) + ".txt";
		ofstream out(SubGraphName);
		int groupSize = edgeGroup[i].size();
		cout << groupSize << endl;
		for (int j = 0; j < groupSize; ++j)
			out << edgeGroup[i][j].first.first << " " << edgeGroup[i][j].second << " " << edgeGroup[i][j].first.second << " ." << endl;
		out.close();
	}
}
