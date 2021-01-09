#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <assert.h>
using namespace std;
typedef unsigned char uchar;
#define		WALL		0xff		//	番人

class Board {
public:
	Board(int wd = 10, int ht = 3) {
		setSize(wd, ht);
	}
public:
	inline int xyToIX(int x, int y) const { return (x + 2) + (y + 2) * m_ARY_WIDTH; }
	inline int inrXyToIX(int x, int y) const { return x + y * m_BD_WIDTH; }
	void setSize(int wd, int ht) {
		m_BD_WIDTH = wd;
		m_BD_HEIGHT = ht;
		m_BD_SIZE = wd * ht;
		m_ARY_WIDTH = wd + 4;
		m_ARY_HEIGHT = ht + 4;
		m_ARY_SIZE = (wd + 4) * (ht + 4);
		//	盤面配列初期化
		m_bd.resize(m_ARY_SIZE);
		for (auto& x : m_bd) x = WALL;	//	番人
		for (int y = 0; y < ht; ++y) {
			for (int x = 0; x < wd; ++x) {
				m_bd[xyToIX(x, y)] = 0;
			}
		}
		//printBoard();
		m_nth.resize(m_BD_SIZE);
		for(auto& x: m_nth) x = 0;
		//	連結リスト初期化
		m_nConnect.resize(m_BD_SIZE);
		m_connects.resize(m_BD_SIZE);
		int ix = 0;
		for (int y = 0; y < ht; ++y) {
			for (int x = 0; x < wd; ++x, ++ix) {
				m_connects[ix].clear();
				if (m_bd[xyToIX(x - 1, y - 2)] == 0) m_connects[ix].push_back(inrXyToIX(x - 1, y - 2));
				if (m_bd[xyToIX(x + 1, y - 2)] == 0) m_connects[ix].push_back(inrXyToIX(x + 1, y - 2));
				if (m_bd[xyToIX(x - 2, y - 1)] == 0) m_connects[ix].push_back(inrXyToIX(x - 2, y - 1));
				if (m_bd[xyToIX(x + 2, y - 1)] == 0) m_connects[ix].push_back(inrXyToIX(x + 2, y - 1));
				if (m_bd[xyToIX(x - 2, y + 1)] == 0) m_connects[ix].push_back(inrXyToIX(x - 2, y + 1));
				if (m_bd[xyToIX(x + 2, y + 1)] == 0) m_connects[ix].push_back(inrXyToIX(x + 2, y + 1));
				if (m_bd[xyToIX(x - 1, y + 2)] == 0) m_connects[ix].push_back(inrXyToIX(x - 1, y + 2));
				if (m_bd[xyToIX(x + 1, y + 2)] == 0) m_connects[ix].push_back(inrXyToIX(x + 1, y + 2));
			}
		}
		for (int i = 0; i != m_BD_SIZE; ++i) {
			m_nConnect[i] = (uchar)m_connects[i].size();
		}
	}
	void printBoard() const {		//	m_bd[] 盤面表示
		for (int y = 0; y < m_BD_HEIGHT; ++y) {
			for (int x = 0; x < m_BD_WIDTH; ++x) {
				auto t = to_string((int)m_bd[xyToIX(x, y)]);
				if (t.size() < 2) t = ' ' + t;
				cout << t << " ";
			}
			cout << "\n";
		}
		cout << "\n";
	}
	void printNth() const {		//	m_nth[] 盤面表示
		for (int y = 0; y < m_BD_HEIGHT; ++y) {
			for (int x = 0; x < m_BD_WIDTH; ++x) {
				auto t = to_string((int)m_nth[x+y*m_BD_WIDTH]);
				if (t.size() < 2) t = ' ' + t;
				cout << t << " ";
			}
			cout << "\n";
		}
		cout << "\n";
	}
#if	0
	void printNth() const {
		int ix = 0;
		for (int y = 0; y < m_BD_HEIGHT; ++y) {
			for (int x = 0; x < m_BD_WIDTH; ++x) {
				m_bd[xyToIX(x, y)] = m_nth[ix++];
			}
		}
		printBoard();
	}
#endif
	void printConnects() const {	//	連結数、連結リスト表示
		for (int i = 0; i != m_BD_SIZE; ++i) {
			cout << i << ": " << (int)m_nConnect[i] << " {";
			for (auto& x : m_connects[i]) cout << (int)x << ", "; cout << "}\n";
		}
		cout << "\n";
	}
public:
	//	ix に連結しているセルの連結数を減らす
	//	連結数が1になったセルがあれば、そこに強制移動になるので ix1 にその位置を設定
	//	連結数が1になったセル複数ある場合はデッドエンドなので false を返す
	bool decNConnect(int ix, int& ix1) {
		int nc1 = 0;
		for(auto& dst: m_connects[ix]) {
			if( m_nth[dst] == 0 ) {
				if( --m_nConnect[dst] == 1 ) {
					++nc1;
					ix1 = dst;
				}
			}
		}
		return nc1 <= 1;		//	連結数１のマスが１個以下ならOK、そうでなければ枝刈りされる
	}
	void incNConnect(int ix) {
		for(auto& dst: m_connects[ix]) {
			if( m_nth[dst] == 0 )
				++m_nConnect[dst];
		}
	}
	void KnightTour(int n, int ix) {
		m_nth[ix] = n;
		//printNth();
		if( ix == inrXyToIX(1, 2) ) {
			if( n == m_BD_SIZE ) {
				++m_count;
				if (m_count % 10000 == 0) cout << ".";
			}
			m_nth[ix] = 0;
			return;
		}
		int ix1 = -1;
		if (!decNConnect(ix, ix1)) {
			//	袋小路の場合 → 枝刈り
			//cout << "Dead End\n";
			m_nth[ix] = 0;
			incNConnect(ix);
			//assert( nc == g_nConnect );
			return;
		}
		if( ix1 >= 0 ) {	//	強制移動の場合
			KnightTour(n+1, ix1);
		} else {
			for(auto& dst: m_connects[ix]) {
				if( m_nth[dst] == 0 )
					KnightTour(n+1, dst);
			}
		}
		m_nth[ix] = 0;
		incNConnect(ix);
	}
	int	countKnightTour(int wd, int ht) {
		setSize(wd, ht);
		//
		m_count = 0;
		m_nth[0] = 1;		//	左上隅から開始
		KnightTour(2, inrXyToIX(2, 1));		//	最初は (2, 1) に移動
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
	vector<uchar>	m_nth;			//	
	vector<uchar>	m_nConnect;		//	各セル連結数
	vector<vector<uchar>>	m_connects;		//	各セルと連結しているセルインデックスリスト
};
int main()
{
	Board bd(10, 3);
#if	0
	bd.printBoard();
	bd.printNth();
	bd.printConnects();
#endif
	cout << bd.countKnightTour(10, 3) << "\n";
	cout << bd.countKnightTour(6, 5) << "\n";
	cout << bd.countKnightTour(6, 6) << "\n";
	if( 1 ) {
		auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存
		cout << bd.countKnightTour(6, 7) << "\n";
		auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
	    auto dur = end - start;        // 要した時間を計算
	    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	    cout << msec << "msec\n";
	}
    std::cout << "\nOK!\n";
}
