#include"miner.h"
#include <time.h>
#include <string>
#include <cmath>

void Delay(int count)
{
	DWORD t1, t2;
	t2 = t1 = GetTickCount();
	while (t2 - t1 < count)
		t2 = GetTickCount();
}


int get_probability()
{
	if (rand() % 16 < 1)
		return 3;
	else if (rand() % 8 < 1)
		return 2;
	else if (rand() %3  < 1)
		return 1;
	else if (rand() % 2 < 1)
		return 0;
}


bool Scene::is_overlap(int x, int y, int size)
{
	for (int i = 0; i < blocks.size(); i++)
	{
		if (x>blocks[i].x && x<blocks[i].x+blocks[i].size && y>blocks[i].y && y<blocks[i].y + blocks[i].size)
			return true;
		if (x > blocks[i].x && x<blocks[i].x + blocks[i].size && y+size>blocks[i].y && y+size < blocks[i].y + blocks[i].size)
			return true;
		if (x+size > blocks[i].x && x+size<blocks[i].x + blocks[i].size && y>blocks[i].y && y < blocks[i].y + blocks[i].size)
			return true;
		if (x+size > blocks[i].x && x+size<blocks[i].x + blocks[i].size && y+size >blocks[i].y && y + size < blocks[i].y + blocks[i].size)
			return true;
	}
	return false;
}


Scene::Scene(int a , int b ) :asset(0),target(200)
{
	using std::string;
	m = Miner();
	hook = Hook();
	//加载block图片
	for (int i = 0; i < 12; i++)
	{
		int size = rand() % 100 + 20;
		int x = rand() % 900 + 100;
		int y = rand() % 850 + 150;
		int proper = get_probability();
		switch (proper)
		{
		case 3:
			size = 30;
			break;
		case 2:
			size = 20;
			break;
		default:
			break;
		}
		Block blk(x, y, size, proper);
		while(is_overlap(x,y,size))
			size = rand() % 100 + 20;
			x = rand() % 900 + 100;
			y = rand() % 850 + 150;
			blk = Block (x, y,size, get_probability());
		switch (blk.get_type())
		{
		case 0://黄金
			if (blk.get_size() < 10)
			{
				loadimage(&blk.img, "./imgs/gold0.png",blk.get_size(),blk.get_size());
				loadimage(&blk.mask, "./imgs/gold0-ma.png", blk.get_size(), blk.get_size());
			}
			else if (blk.get_size() >= 10 && blk.get_size() < 25) {
				loadimage(&blk.img, "./imgs/gold1.png", blk.get_size(), blk.get_size());
				loadimage(&blk.mask, "./imgs/gold1-ma.png", blk.get_size(), blk.get_size());
			}
			else {
				loadimage(&blk.img, "./imgs/gold2.png", blk.get_size(), blk.get_size());
				loadimage(&blk.mask, "./imgs/gold2-ma.png", blk.get_size(), blk.get_size());
			}
			break;
		case 1://石头
			loadimage(&blk.img, "./imgs/rock1.png", blk.get_size(), blk.get_size());
			loadimage(&blk.mask, "./imgs/rock1-ma.png", blk.get_size(), blk.get_size());
			break;
		case 2://钻石
			loadimage(&blk.img, "./imgs/diamond.png", 20, 20);
			loadimage(&blk.mask, "./imgs/diamond-ma.png", 20, 20);
			break;
		case 3://药水
			loadimage(&blk.img, "./imgs/water.png", 30, 30);
			loadimage(&blk.mask, "./imgs/water-ma.png",30,30);
			break;
		default:
			break;
		}
		blocks.push_back(blk);
	}
	loadimage(&backgroud0, "./imgs/bg.jpg", WIDTH, HEIGHT - 120);
	loadimage(&backgroud1, "./imgs/bg1.jpg", WIDTH, 120);
}

void Block::Draw()
{
	putimage(x, y, &mask, SRCAND);
	putimage(x, y, &img, SRCPAINT);
}


void Scene::Draw()
{
	putimage(0, 120, &backgroud0);
	putimage(0, 0, &backgroud1);
	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i].Draw();
	}
	char s[30] = "";
	sprintf_s(s, "asset: %d", asset);
	char tar[30] = "";
	sprintf_s(tar, "target: %d", target);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, "Consolas");
	outtextxy(20, 20, s);
	outtextxy(800, 20, tar);
	hook.Draw();
}

void Scene::Run()
{
	int index;
	ExMessage msg;

	MCI_OPEN_PARMS mciOpenParms1;
	mciOpenParms1.lpstrElementName = "D:\\C++code\\Gold_miners\\Gold_miners\\imgs\\long.mp3";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, DWORD_PTR(&mciOpenParms1));
	UINT wDeviceID1 = mciOpenParms1.wDeviceID;	

	MCI_OPEN_PARMS mciOpenParms2;
	mciOpenParms2.lpstrElementName = "D:\\C++code\\Gold_miners\\Gold_miners\\imgs\\get.mp3";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, DWORD_PTR(&mciOpenParms2));
	UINT wDeviceID2 = mciOpenParms2.wDeviceID;

	BeginBatchDraw();
	while (1) {
		
		hook.hock_swing();
		if (peekmessage(&msg, EX_KEY)) {
			if (msg.message == WM_KEYDOWN)
			{
				while (1) {
					hook.grow(20);
					flush();
					if ((index = is_catched()) != -1) {
						catched_music(wDeviceID2);
						while (hook.get_len() > 70) {
							hook.shorten((120 - blocks[index].size)/8);
							blocks[index].block_move(hook.get_endx(), hook.get_endy());
							flush();
						}
						asset += blocks[index].bonus;
						delete_block(index);
						flush();
						break;
					}
					if(is_Border()) {
						miss_music( wDeviceID1);
						while (hook.get_len() > 70) {
							hook.shorten(20);
							flush();
						}
						
						break;
					}
					
				}
			}
		}
		hook.status = Hook::H_NORMAL;
		flush();
		Delay(5);
	}

	
	
}

void Scene::flush()
{
	Draw();
	miner_rest();
	FlushBatchDraw();
}



Block::Block(int a, int b, int s, int t) :x(a), y(b), bonus(s), size(s), type(t)
{

}

Miner::Miner():x(WIDTH/2-15),y(120-114),status(0)
{
	loadimage(&img_up, "./imgs/worker1.png");
	loadimage(&img_down, "./imgs/worker2.png");
	loadimage(&mask_up, "./imgs/worker1-ma.png");
	loadimage(&mask_down, "./imgs/worker2-ma.png");

}

void Scene::miner_rest()
{
	putimage(m.x, m.y, &m.mask_up, SRCAND);
	putimage(m.x, m.y, &m.img_up, SRCPAINT);
}

void Scene::miner_work_play(int speed)
{
	Delay(speed);
	putimage(m.x, m.y, &m.mask_up, SRCAND);
	putimage(m.x, m.y, &m.img_up, SRCPAINT);
	Delay(speed);
	putimage(m.x, m.y, &m.mask_down, SRCAND);
	putimage(m.x, m.y, &m.img_down, SRCPAINT);

}

Hook::Hook(int a,int b)
{
	x = a;
	y = b;
	len = 70;
	endx = a;
	endy = b + len;
	ang = 0;
	dir = RIGHT;
	status = H_NORMAL;
}

void Hook::Draw()
{
	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 5);
	line(x, y, endx, endy);
}

void Hook::hock_swing()
{
	if (status == H_NORMAL) {
		if (ang > MAXAng) {
			--ang;
			set_endxy();
			dir = LEFT;
		}
		else if (ang < -MAXAng) {
			++ang;
			set_endxy();
			dir = RIGHT;
		}
		if (dir == LEFT) {
			--ang;
			set_endxy();
		}
		else if (dir == RIGHT) {
			++ang;
			set_endxy();
		}

	}
	
}

void Hook::set_endxy()
{
	endx = x+sin(PI/180*ang)*len;
	endy = y+cos(PI / 180 * ang)*len;
	
}

void Hook::grow(int speed)
{	
	Delay(20);
	status = H_LONG;
	len+=speed;
	set_endxy();	

}


void Hook::shorten(int speed)
{
	Delay(40);
	status = H_SHORT;
	len-=speed;
	set_endxy();
}

int Scene::is_catched()
{

	for (int i = 0; i < blocks.size(); i++)
	{
		if (hook.get_endx() > blocks[i].x && hook.get_endx() < blocks[i].x + blocks[i].size
			&& hook.get_endy() > blocks[i].y && hook.get_endy() < blocks[i].y + blocks[i].size)
			return i;
	}
	return -1;
}

bool Scene::is_Border()
{
	if (hook.get_endx() < 0 || hook.get_endx() > WIDTH ||
		hook.get_endy() > HEIGHT)
		return true;
	return false;
}

void Block::block_move(int a, int b)
{
	x = a;
	y = b;
}


void Scene::delete_block(int index)
{
	std::vector<Block>::iterator it;
	
	for (it = blocks.begin(); it < blocks.end(); it++,index--)
	{
		if (index == 0) 
			break;
	}
	blocks.erase(it);
}



void Scene::catched_music( UINT wDeviceID)
{	
	MCI_PLAY_PARMS mciPlayParms;
	mciSendCommand(wDeviceID, MCI_PLAY, 0, DWORD_PTR(&mciPlayParms));
}


void Scene::miss_music(UINT wDeviceID)
{
	MCI_PLAY_PARMS mciPlayParms;
	mciSendCommand(wDeviceID, MCI_PLAY, 0, DWORD_PTR(&mciPlayParms));

}
