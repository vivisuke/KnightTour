#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <assert.h>
using namespace std;
typedef unsigned char uchar;
typedef const int cint;
#define		WALL		0xff

class Board {	//	枝刈り無し版
public:
	Board(int wd = 10, int ht = 3)
	{
		setSize(wd, ht);
	}
public:
	inline int xyToIX(int x, int y) const { return (x+2) + (y+2) * m_ARY_WIDTH; }
	void setSize(int wd, int ht) {
		m_BD_WIDTH = wd;
		m_BD_HEIGHT = ht;
		m_BD_SIZE = wd*ht;
		m_ARY_WIDTH = wd+4;
		m_ARY_HEIGHT = ht+4;
		m_ARY_SIZE = (wd+4) * (ht+4);
		//	盤面配列初期化
		m_bd.resize(m_ARY_SIZE);
		for(auto& x: m_bd) x = WALL;	//	番人
		for (int y = 0; y < ht; ++y) {
			for (int x = 0; x < wd; ++x) {
				m_bd[xyToIX(x, y)] = 0;
			}
		}
		//printBoard();
	}
	void printBoard() const {		//	盤面表示
		for (int y = 0; y < m_BD_HEIGHT; ++y) {
			for (int x = 0; x < m_BD_WIDTH; ++x) {
				auto t = to_string((int)m_bd[xyToIX(x, y)]);
				if( t.size() < 2 ) t = ' ' + t;
				cout << t << " ";
			}
			cout << "\n";
		}
		cout << "\n";
	}
	void KnightTour(int n, int x, int y) {
		m_bd[xyToIX(x, y)] = n;
		//cout << n << ":\n";
		//printBoard();
		if( x == 1 && y == 2 ) {
			if( n == m_BD_SIZE ) {	//	found!
				++m_count;
				if (m_count % 1000 == 0) cout << ".";
				//cout << m_count << ":\n";
				//printBoard();
			} else if( n != 2 ) {
				m_bd[xyToIX(x, y)] = 0;
				return;
			}
		}
		if (m_bd[xyToIX(x - 1, y - 2)] == 0)
			KnightTour(n + 1, x - 1, y - 2);
		if (m_bd[xyToIX(x + 1, y - 2)] == 0)
			KnightTour(n + 1, x + 1, y - 2);
		if (m_bd[xyToIX(x - 2, y - 1)] == 0)
			KnightTour(n + 1, x - 2, y - 1);
		if (m_bd[xyToIX(x + 2, y - 1)] == 0)
			KnightTour(n + 1, x + 2, y - 1);
		if (m_bd[xyToIX(x - 2, y + 1)] == 0)
			KnightTour(n + 1, x - 2, y + 1);
		if (m_bd[xyToIX(x + 2, y + 1)] == 0)
			KnightTour(n + 1, x + 2, y + 1);
		if (m_bd[xyToIX(x - 1, y + 2)] == 0)
			KnightTour(n + 1, x - 1, y + 2);
		if (m_bd[xyToIX(x + 1, y + 2)] == 0)
			KnightTour(n + 1, x + 1, y + 2);
		m_bd[xyToIX(x, y)] = 0;
		//cout << n << "(ret):\n";
		//printBoard();
	}
	int countKnightTour(int wd, int ht) {
		setSize(wd, ht);
		//
		m_count = 0;
		m_bd[xyToIX(0, 0)] = 1;		//	左上隅から開始
		//updateNConnect(0, 0, -1);
		KnightTour(2, 2, 1);		//	最初は (2, 1) に移動
		return m_count;
	}
private:
	int m_count = 0;
	int m_BD_WIDTH;		//	盤面幅
	int m_BD_HEIGHT;	//	盤面高
	int m_BD_SIZE;
	int m_ARY_WIDTH;	//	番人を含んだ配列幅
	int m_ARY_HEIGHT;	//	番人を含んだ配列高
	int m_ARY_SIZE;
	vector<uchar>	m_bd;			//	番人を含んだ盤面配列
	//vector<uchar>	m_nConnect;		//	連結可能数
};
class BoardPruning {	//	枝刈り有り版
public:
	BoardPruning(int wd = 10, int ht = 3)
	{
		setSize(wd, ht);
	}
public:
	inline int xyToIX(int x, int y) const { return (x+2) + (y+2) * m_ARY_WIDTH; }
	void setSize(int wd, int ht) {
		m_BD_WIDTH = wd;
		m_BD_HEIGHT = ht;
		m_BD_SIZE = wd*ht;
		m_ARY_WIDTH = wd+4;
		m_ARY_HEIGHT = ht+4;
		m_ARY_SIZE = (wd+4) * (ht+4);
		//	盤面配列初期化
		m_bd.resize(m_ARY_SIZE);
		for(auto& x: m_bd) x = WALL;	//	番人
		for (int y = 0; y < ht; ++y) {
			for (int x = 0; x < wd; ++x) {
				m_bd[xyToIX(x, y)] = 0;
			}
		}
		//printBoard();
		//	連結数配列初期化
		m_nConnect.resize(m_ARY_SIZE);
		for(auto& x: m_nConnect) x = 0;
		for (int y = 0; y < ht; ++y) {
			for (int x = 0; x < wd; ++x) {
				if( m_bd[xyToIX(x-1, y-2)] == 0 ) m_nConnect[xyToIX(x, y)] += 1;
				if( m_bd[xyToIX(x+1, y-2)] == 0 ) m_nConnect[xyToIX(x, y)] += 1;
				if( m_bd[xyToIX(x-2, y-1)] == 0 ) m_nConnect[xyToIX(x, y)] += 1;
				if( m_bd[xyToIX(x+2, y-1)] == 0 ) m_nConnect[xyToIX(x, y)] += 1;
				if( m_bd[xyToIX(x-2, y+1)] == 0 ) m_nConnect[xyToIX(x, y)] += 1;
				if( m_bd[xyToIX(x+2, y+1)] == 0 ) m_nConnect[xyToIX(x, y)] += 1;
				if( m_bd[xyToIX(x-1, y+2)] == 0 ) m_nConnect[xyToIX(x, y)] += 1;
				if( m_bd[xyToIX(x+1, y+2)] == 0 ) m_nConnect[xyToIX(x, y)] += 1;
			}
		}
		//printNConnect();
	}
	void printBoard() const {		//	盤面表示
		for (int y = 0; y < m_BD_HEIGHT; ++y) {
			for (int x = 0; x < m_BD_WIDTH; ++x) {
				auto t = to_string((int)m_bd[xyToIX(x, y)]);
				if( t.size() < 2 ) t = ' ' + t;
				cout << t << " ";
			}
			cout << "\n";
		}
		cout << "\n";
	}
	void printNConnect() const {		//	連結数表示
		for (int y = 0; y < m_BD_HEIGHT; ++y) {
			for (int x = 0; x < m_BD_WIDTH; ++x) {
				auto t = to_string((int)m_nConnect[xyToIX(x, y)]);
				if( t.size() < 2 ) t = ' ' + t;
				cout << t << " ";
			}
			cout << "\n";
		}
		cout << "\n";
	}
	bool decNConnect(int x, int y, int& ix1) {
#if	1
		int nc1 = 0, ix;
		if( m_bd[ix = xyToIX(x-1, y-2)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) {
				ix1 = ix;
				++nc1;
			}
		}
		if( m_bd[ix = xyToIX(x+1, y-2)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) {
				ix1 = ix;
				++nc1;
			}
		}
		if( m_bd[ix = xyToIX(x-2, y-1)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) {
				ix1 = ix;
				++nc1;
			}
		}
		if( m_bd[ix = xyToIX(x+2, y-1)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) {
				ix1 = ix;
				++nc1;
			}
		}
		if( m_bd[ix = xyToIX(x-2, y+1)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) {
				ix1 = ix;
				++nc1;
			}
		}
		if( m_bd[ix = xyToIX(x+2, y+1)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) {
				ix1 = ix;
				++nc1;
			}
		}
		if( m_bd[ix = xyToIX(x-1, y+2)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) {
				ix1 = ix;
				++nc1;
			}
		}
		if( m_bd[ix = xyToIX(x+1, y+2)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) {
				ix1 = ix;
				++nc1;
			}
		}
		return nc1 <= 1;
#else
		bool notDeadEnd = true;
		int ix;
		if( m_bd[ix = xyToIX(x-1, y-2)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) notDeadEnd = false;
			else if( m_nConnect[ix] == 1 ) ix1 = ix;
		}
		if( m_bd[ix = xyToIX(x+1, y-2)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) notDeadEnd = false;
			else if( m_nConnect[ix] == 1 ) ix1 = ix;
		}
		if( m_bd[ix = xyToIX(x-2, y-1)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) notDeadEnd = false;
			else if( m_nConnect[ix] == 1 ) ix1 = ix;
		}
		if( m_bd[ix = xyToIX(x+2, y-1)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) notDeadEnd = false;
			else if( m_nConnect[ix] == 1 ) ix1 = ix;
		}
		if( m_bd[ix = xyToIX(x-2, y+1)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) notDeadEnd = false;
			else if( m_nConnect[ix] == 1 ) ix1 = ix;
		}
		if( m_bd[ix = xyToIX(x+2, y+1)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) notDeadEnd = false;
			else if( m_nConnect[ix] == 1 ) ix1 = ix;
		}
		if( m_bd[ix = xyToIX(x-1, y+2)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) notDeadEnd = false;
			else if( m_nConnect[ix] == 1 ) ix1 = ix;
		}
		if( m_bd[ix = xyToIX(x+1, y+2)] == 0 ) {
			if( --m_nConnect[ix] == 0 ) notDeadEnd = false;
			else if( m_nConnect[ix] == 1 ) ix1 = ix;
		}
		return notDeadEnd;
#endif
	}
	void incNConnect(int x, int y) {
		if( m_bd[xyToIX(x-1, y-2)] == 0 )
			m_nConnect[xyToIX(x-1, y-2)] += 1;
		if( m_bd[xyToIX(x+1, y-2)] == 0 )
			m_nConnect[xyToIX(x+1, y-2)] += 1;
		if( m_bd[xyToIX(x-2, y-1)] == 0 )
			m_nConnect[xyToIX(x-2, y-1)] += 1;
		if( m_bd[xyToIX(x+2, y-1)] == 0 )
			m_nConnect[xyToIX(x+2, y-1)] += 1;
		if( m_bd[xyToIX(x-2, y+1)] == 0 )
			m_nConnect[xyToIX(x-2, y+1)] += 1;
		if( m_bd[xyToIX(x+2, y+1)] == 0 )
			m_nConnect[xyToIX(x+2, y+1)] += 1;
		if( m_bd[xyToIX(x-1, y+2)] == 0 )
			m_nConnect[xyToIX(x-1, y+2)] += 1;
		if( m_bd[xyToIX(x+1, y+2)] == 0 )
			m_nConnect[xyToIX(x+1, y+2)] += 1;
	}
	void updateNConnect(int x, int y, int d) {
		if( m_bd[xyToIX(x-1, y-2)] == 0 )
			m_nConnect[xyToIX(x-1, y-2)] += d;
		if( m_bd[xyToIX(x+1, y-2)] == 0 )
			m_nConnect[xyToIX(x+1, y-2)] += d;
		if( m_bd[xyToIX(x-2, y-1)] == 0 )
			m_nConnect[xyToIX(x-2, y-1)] += d;
		if( m_bd[xyToIX(x+2, y-1)] == 0 )
			m_nConnect[xyToIX(x+2, y-1)] += d;
		if( m_bd[xyToIX(x-2, y+1)] == 0 )
			m_nConnect[xyToIX(x-2, y+1)] += d;
		if( m_bd[xyToIX(x+2, y+1)] == 0 )
			m_nConnect[xyToIX(x+2, y+1)] += d;
		if( m_bd[xyToIX(x-1, y+2)] == 0 )
			m_nConnect[xyToIX(x-1, y+2)] += d;
		if( m_bd[xyToIX(x+1, y+2)] == 0 )
			m_nConnect[xyToIX(x+1, y+2)] += d;
	}
	void KnightTour(int n, int x, int y) {
		m_bd[xyToIX(x, y)] = n;
		//cout << n << ":\n";
		//printBoard();
		if( x == 1 && y == 2 ) {
			if( n == m_BD_SIZE ) {	//	found!
				++m_count;
				if (m_count % 10000 == 0) cout << ".";
				//cout << m_count << ":\n";
				//printBoard();
			} else if( n != 2 ) {
				m_bd[xyToIX(x, y)] = 0;
				return;
			}
		}
		int ix1 = -1;
	#ifdef _DEBUG
		vector<uchar> nc(m_nConnect);
	#endif // DEBUG
		if (!decNConnect(x, y, ix1)) {
			//	袋小路の場合 → 枝刈り
			//cout << "Dead End\n";
			m_bd[xyToIX(x, y)] = 0;
			incNConnect(x, y);
			assert( nc == m_nConnect );
			return;
		}
		//printNConnect();
		if (ix1 >= 0) {
			//	連結が1のセルがあれば、そこに強制移動
			//cout << "forced\n";
			KnightTour(n + 1, ix1 % m_ARY_WIDTH - 2, ix1 / m_ARY_WIDTH - 2);
		}
		else {
			if (m_bd[xyToIX(x - 1, y - 2)] == 0)
				KnightTour(n + 1, x - 1, y - 2);
			if (m_bd[xyToIX(x + 1, y - 2)] == 0)
				KnightTour(n + 1, x + 1, y - 2);
			if (m_bd[xyToIX(x - 2, y - 1)] == 0)
				KnightTour(n + 1, x - 2, y - 1);
			if (m_bd[xyToIX(x + 2, y - 1)] == 0)
				KnightTour(n + 1, x + 2, y - 1);
			if (m_bd[xyToIX(x - 2, y + 1)] == 0)
				KnightTour(n + 1, x - 2, y + 1);
			if (m_bd[xyToIX(x + 2, y + 1)] == 0)
				KnightTour(n + 1, x + 2, y + 1);
			if (m_bd[xyToIX(x - 1, y + 2)] == 0)
				KnightTour(n + 1, x - 1, y + 2);
			if (m_bd[xyToIX(x + 1, y + 2)] == 0)
				KnightTour(n + 1, x + 1, y + 2);
		}
		m_bd[xyToIX(x, y)] = 0;
		//cout << n << "(ret):\n";
		//printBoard();
		incNConnect(x, y);
		assert( nc == m_nConnect );
	}
	int countKnightTour(int wd, int ht) {
		setSize(wd, ht);
		//
		m_count = 0;
		m_bd[xyToIX(0, 0)] = 1;		//	左上隅から開始
		//updateNConnect(0, 0, -1);
		KnightTour(2, 2, 1);		//	最初は (2, 1) に移動
		return m_count;
	}
private:
	int m_count = 0;
	int m_BD_WIDTH;		//	盤面幅
	int m_BD_HEIGHT;	//	盤面高
	int m_BD_SIZE;
	int m_ARY_WIDTH;	//	番人を含んだ配列幅
	int m_ARY_HEIGHT;	//	番人を含んだ配列高
	int m_ARY_SIZE;
	vector<uchar>	m_bd;			//	番人を含んだ盤面配列
	vector<uchar>	m_nConnect;		//	連結可能数
};
int main()
{
	if( 0 ) {
		cout << "not Pruning:\n\n";
		Board bd;
		cout << "10x3: " << bd.countKnightTour(10, 3) << "\n";
		cout << "6x5: " << bd.countKnightTour(6, 5) << "\n";
		cout << "6x6: " << bd.countKnightTour(6, 6) << "\n";
	}
	if( 1 ) {
		cout << "with Pruning:\n\n";
		BoardPruning bd;
		cout << "10x3: " << bd.countKnightTour(10, 3) << "\n";
		cout << "6x5: " << bd.countKnightTour(6, 5) << "\n";
		cout << "6x6: " << bd.countKnightTour(6, 6) << "\n";
		if( 1 ) {
			auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
			cout << bd.countKnightTour(6, 7) << "\n";
			auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
		    auto dur = end - start;        // 要した時間を計算
		    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		    cout << msec << "msec\n";
		}
	}
    std::cout << "\nOK!\n";
}
