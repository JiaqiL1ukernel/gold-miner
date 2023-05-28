#ifndef _MINER_H_
#define _MINER_H_
#include <easyx.h>
#include <vector>
#include <stdlib.h>
#include <graphics.h>
#include <iostream>
#include <conio.h>
#include <Digitalv.h>
static const int WIDTH = 1024;
static const int HEIGHT = 1000;
static const double PI = 3.1415926535898;

void Delay(int count);
int get_location(int x, int size);
int get_probability();

class Miner
{
private:

	int status;//0:stop	1:work
	
public:
	int x;
	int y;
	IMAGE img_up;
	IMAGE img_down;
	IMAGE mask_up;
	IMAGE mask_down;
	Miner();
	void shift_status() { status=(status == 0?1:0); }
	int get_status()const { return status; }

};
class Block
{
protected:
	int type;
private:
public:
	int bonus;
	int size;
	int x;
	int y;
	enum status{Gold,Stone,Money};
	IMAGE img;
	IMAGE mask;
	Block(int a, int b,int s, int t);
	virtual ~Block() {}
	void Draw();
	int get_size()const { return size; }
	int get_type()const { return type; }
	void block_move(int a, int b);
	
};

class Hook 
{
private:
	int x;
	int y;
	int endx;
	int endy;
	int dir;
	int ang;
	double len;
	enum {LEFT,RIGHT,MAXAng=80};
public:
	enum {H_NORMAL,H_LONG,H_SHORT};
	int status;
	Hook(int x=WIDTH/2+10,int y=90);
	void Draw();
	int get_endx()const { return endx; }
	int get_endy()const { return endy; }
	void hock_swing();
	void set_endxy();
	void grow(int speed);
	void shorten(int speed);
	double get_len()const { return len; }

};


class Scene
{
private:
	int asset;
	int target;
	Miner m;
	IMAGE backgroud0;
	IMAGE backgroud1;
public:
	Hook hook;
	std::vector<Block> blocks;
	Scene(int a = 0, int b = 200);
	void Draw();
	void Run();
	bool is_overlap(int x, int y, int size);
	void miner_rest();
	void miner_work_play(int speed);
	void delete_block(int);
	int is_catched();
	bool is_Border();
	void flush();
	void miss_music( UINT wDeviceID);
	void catched_music(UINT wDeviceID);
};





#endif