#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <assert.h>
using namespace std;
typedef unsigned char uchar;
typedef const int cint;
#define		WALL		0xff
int g_count = 0;
int g_BD_WIDTH;		//	盤面幅
int g_BD_HEIGHT;		//	盤面高
int g_BD_SIZE;
int g_ARY_WIDTH;	//	番人を含んだ配列幅
int g_ARY_HEIGHT;	//	番人を含んだ配列高
vector<uchar>	g_bd;			//	番人を含んだ盤面配列
vector<uchar>	g_nConnect;		//	連結可能数
inline int xyToIX(int x, int y) { return (x+2) + (y+2) * g_ARY_WIDTH; }
void print() {		//	盤面表示
	for (int y = 0; y < g_BD_HEIGHT; ++y) {
		for (int x = 0; x < g_BD_WIDTH; ++x) {
			auto t = to_string((int)g_bd[xyToIX(x, y)]);
			if( t.size() < 2 ) t = ' ' + t;
			cout << t << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}
void printNConnect() {		//	連結数表示
	for (int y = 0; y < g_BD_HEIGHT; ++y) {
		for (int x = 0; x < g_BD_WIDTH; ++x) {
			auto t = to_string((int)g_nConnect[xyToIX(x, y)]);
			if( t.size() < 2 ) t = ' ' + t;
			cout << t << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}
bool decNConnect(int x, int y, int& ix1) {
	bool notDeadEnd = true;
	int ix;
	if( g_bd[ix = xyToIX(x-1, y-2)] == 0 ) {
		if( --g_nConnect[ix] == 0 ) notDeadEnd = false;
		else if( g_nConnect[ix] == 1 ) ix1 = ix;
	}
	if( g_bd[ix = xyToIX(x+1, y-2)] == 0 ) {
		if( --g_nConnect[ix] == 0 ) notDeadEnd = false;
		else if( g_nConnect[ix] == 1 ) ix1 = ix;
	}
	if( g_bd[ix = xyToIX(x-2, y-1)] == 0 ) {
		if( --g_nConnect[ix] == 0 ) notDeadEnd = false;
		else if( g_nConnect[ix] == 1 ) ix1 = ix;
	}
	if( g_bd[ix = xyToIX(x+2, y-1)] == 0 ) {
		if( --g_nConnect[ix] == 0 ) notDeadEnd = false;
		else if( g_nConnect[ix] == 1 ) ix1 = ix;
	}
	if( g_bd[ix = xyToIX(x-2, y+1)] == 0 ) {
		if( --g_nConnect[ix] == 0 ) notDeadEnd = false;
		else if( g_nConnect[ix] == 1 ) ix1 = ix;
	}
	if( g_bd[ix = xyToIX(x+2, y+1)] == 0 ) {
		if( --g_nConnect[ix] == 0 ) notDeadEnd = false;
		else if( g_nConnect[ix] == 1 ) ix1 = ix;
	}
	if( g_bd[ix = xyToIX(x-1, y+2)] == 0 ) {
		if( --g_nConnect[ix] == 0 ) notDeadEnd = false;
		else if( g_nConnect[ix] == 1 ) ix1 = ix;
	}
	if( g_bd[ix = xyToIX(x+1, y+2)] == 0 ) {
		if( --g_nConnect[ix] == 0 ) notDeadEnd = false;
		else if( g_nConnect[ix] == 1 ) ix1 = ix;
	}
	return notDeadEnd;
}
void incNConnect(int x, int y) {
	if( g_bd[xyToIX(x-1, y-2)] == 0 )
		g_nConnect[xyToIX(x-1, y-2)] += 1;
	if( g_bd[xyToIX(x+1, y-2)] == 0 )
		g_nConnect[xyToIX(x+1, y-2)] += 1;
	if( g_bd[xyToIX(x-2, y-1)] == 0 )
		g_nConnect[xyToIX(x-2, y-1)] += 1;
	if( g_bd[xyToIX(x+2, y-1)] == 0 )
		g_nConnect[xyToIX(x+2, y-1)] += 1;
	if( g_bd[xyToIX(x-2, y+1)] == 0 )
		g_nConnect[xyToIX(x-2, y+1)] += 1;
	if( g_bd[xyToIX(x+2, y+1)] == 0 )
		g_nConnect[xyToIX(x+2, y+1)] += 1;
	if( g_bd[xyToIX(x-1, y+2)] == 0 )
		g_nConnect[xyToIX(x-1, y+2)] += 1;
	if( g_bd[xyToIX(x+1, y+2)] == 0 )
		g_nConnect[xyToIX(x+1, y+2)] += 1;
}
void updateNConnect(int x, int y, int d) {
	if( g_bd[xyToIX(x-1, y-2)] == 0 )
		g_nConnect[xyToIX(x-1, y-2)] += d;
	if( g_bd[xyToIX(x+1, y-2)] == 0 )
		g_nConnect[xyToIX(x+1, y-2)] += d;
	if( g_bd[xyToIX(x-2, y-1)] == 0 )
		g_nConnect[xyToIX(x-2, y-1)] += d;
	if( g_bd[xyToIX(x+2, y-1)] == 0 )
		g_nConnect[xyToIX(x+2, y-1)] += d;
	if( g_bd[xyToIX(x-2, y+1)] == 0 )
		g_nConnect[xyToIX(x-2, y+1)] += d;
	if( g_bd[xyToIX(x+2, y+1)] == 0 )
		g_nConnect[xyToIX(x+2, y+1)] += d;
	if( g_bd[xyToIX(x-1, y+2)] == 0 )
		g_nConnect[xyToIX(x-1, y+2)] += d;
	if( g_bd[xyToIX(x+1, y+2)] == 0 )
		g_nConnect[xyToIX(x+1, y+2)] += d;
}
void KnightTour(int n, int x, int y) {
	g_bd[xyToIX(x, y)] = n;
	//cout << n << ":\n";
	//print();
	if( x == 1 && y == 2 ) {
		if( n == g_BD_SIZE ) {	//	found!
			++g_count;
			if (g_count % 1000 == 0) cout << ".";
			//cout << g_count << ":\n";
			//print();
		} else if( n != 2 ) {
			g_bd[xyToIX(x, y)] = 0;
			return;
		}
	}
	int ix1 = -1;
#ifdef _DEBUG
	vector<uchar> nc(g_nConnect);
#endif // DEBUG
	if (!decNConnect(x, y, ix1)) {
		//	袋小路の場合 → 枝刈り
		//cout << "Dead End\n";
		g_bd[xyToIX(x, y)] = 0;
		incNConnect(x, y);
		assert( nc == g_nConnect );
		return;
	}
	//printNConnect();
	if (ix1 >= 0) {
		//	連結が1のセルがあれば、そこに強制移動
		//cout << "forced\n";
		KnightTour(n + 1, ix1 % g_ARY_WIDTH - 2, ix1 / g_ARY_WIDTH - 2);
	}
	else {
		if (g_bd[xyToIX(x - 1, y - 2)] == 0)
			KnightTour(n + 1, x - 1, y - 2);
		if (g_bd[xyToIX(x + 1, y - 2)] == 0)
			KnightTour(n + 1, x + 1, y - 2);
		if (g_bd[xyToIX(x - 2, y - 1)] == 0)
			KnightTour(n + 1, x - 2, y - 1);
		if (g_bd[xyToIX(x + 2, y - 1)] == 0)
			KnightTour(n + 1, x + 2, y - 1);
		if (g_bd[xyToIX(x - 2, y + 1)] == 0)
			KnightTour(n + 1, x - 2, y + 1);
		if (g_bd[xyToIX(x + 2, y + 1)] == 0)
			KnightTour(n + 1, x + 2, y + 1);
		if (g_bd[xyToIX(x - 1, y + 2)] == 0)
			KnightTour(n + 1, x - 1, y + 2);
		if (g_bd[xyToIX(x + 1, y + 2)] == 0)
			KnightTour(n + 1, x + 1, y + 2);
	}
	g_bd[xyToIX(x, y)] = 0;
	//cout << n << "(ret):\n";
	//print();
	incNConnect(x, y);
	assert( nc == g_nConnect );
}
int countKnightTour(int wd, int ht) {
	g_BD_SIZE = wd * ht;
	g_ARY_WIDTH = (g_BD_WIDTH = wd) + 4;		//	+4 for 番人
	g_ARY_HEIGHT = (g_BD_HEIGHT = ht) + 4;		//	+4 for 番人
	const auto ARY_SIZE = g_ARY_WIDTH * g_ARY_HEIGHT;
	//	盤面配列初期化
	g_bd.resize(ARY_SIZE);
	for(auto& x: g_bd) x = WALL;	//	番人
	for (int y = 0; y < ht; ++y) {
		for (int x = 0; x < wd; ++x) {
			g_bd[xyToIX(x, y)] = 0;
		}
	}
	//print();
	//	連結数配列初期化
	g_nConnect.resize(ARY_SIZE);
	for(auto& x: g_nConnect) x = 0;
	for (int y = 0; y < ht; ++y) {
		for (int x = 0; x < wd; ++x) {
			if( g_bd[xyToIX(x-1, y-2)] == 0 ) g_nConnect[xyToIX(x, y)] += 1;
			if( g_bd[xyToIX(x+1, y-2)] == 0 ) g_nConnect[xyToIX(x, y)] += 1;
			if( g_bd[xyToIX(x-2, y-1)] == 0 ) g_nConnect[xyToIX(x, y)] += 1;
			if( g_bd[xyToIX(x+2, y-1)] == 0 ) g_nConnect[xyToIX(x, y)] += 1;
			if( g_bd[xyToIX(x-2, y+1)] == 0 ) g_nConnect[xyToIX(x, y)] += 1;
			if( g_bd[xyToIX(x+2, y+1)] == 0 ) g_nConnect[xyToIX(x, y)] += 1;
			if( g_bd[xyToIX(x-1, y+2)] == 0 ) g_nConnect[xyToIX(x, y)] += 1;
			if( g_bd[xyToIX(x+1, y+2)] == 0 ) g_nConnect[xyToIX(x, y)] += 1;
		}
	}
	//printNConnect();
	//
	g_count = 0;
	g_bd[xyToIX(0, 0)] = 1;		//	左上隅から開始
	//updateNConnect(0, 0, -1);
	KnightTour(2, 2, 1);		//	最初は (2, 1) に移動
	return g_count;
}
int main()
{
	cout << countKnightTour(10, 3) << "\n";
	cout << countKnightTour(3, 10) << "\n";
	cout << countKnightTour(5, 6) << "\n";
	cout << countKnightTour(6, 5) << "\n";
	cout << countKnightTour(6, 6) << "\n";
	cout << countKnightTour(6, 7) << "\n";
	std::cout << "\nOK!\n";
}
