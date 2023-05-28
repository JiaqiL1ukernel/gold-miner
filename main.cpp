#include "miner.h"

#pragma comment(lib, "winmm.lib")
int main(void)
{
	initgraph(WIDTH, HEIGHT);
	MCI_OPEN_PARMS mciOpenParms0;
	
	mciOpenParms0.lpstrElementName = "D:\\C++code\\Gold_miners\\Gold_miners\\imgs\\8278.mp3";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, DWORD_PTR(&mciOpenParms0));
	UINT wDeviceID0 = mciOpenParms0.wDeviceID;
	MCI_PLAY_PARMS mciPlayParms0;


	mciSendCommand(wDeviceID0, MCI_PLAY, MCI_DGV_PLAY_REPEAT, DWORD_PTR(&mciPlayParms0));
	IMAGE welcome_page;
	loadimage(&welcome_page, "./imgs/startbg.png", WIDTH, HEIGHT);
	
	putimage(0, 0, &welcome_page);
	char c;
	std::cin >> c;
	mciSendCommand(wDeviceID0, MCI_STOP, 0, NULL);	// Õ£÷π≤•∑≈
	mciSendCommand(wDeviceID0, MCI_CLOSE, 0, NULL);	// πÿ±’“Ù¿÷
	Scene s;

	s.Run();

	return 0;
}