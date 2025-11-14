#include "DxLib.h"
#include <stdio.h>
#include <math.h>
#include <direct.h>


const char debug = 0;

const int gameid = 2;
const int ver = 10;

const double pi = 3.141592653589793;
const int objmax = 30000;
const int shotmax = 16;

const int white = GetColor(255, 255, 255);

int winmode = 1;
int windowx = 640;
int windowy = 300;
int winsizex = 640;
int winsizey = 480;
int vsync = 1;
int sleeptype = 1;
int taskprepare = 8;
int effecton = 1;
int bilinearon = 1;
int shotkey = 1;
int slowkey = 11;
int pausekey = 9;
int joypadzone = 35;
int musicvol = 80;
int sevol = 80;
int fastspeed = 20;
int slowspeed = 10;


//savedata
int recordid = gameid;
int recordver = ver;
int windowtime = 0;
int allmiss = 0;
int allkill = 0;
int hiscore[4] = { 0 };
int lowmiss[4] = { 0 };
int reachscene[4] = { 0 };
int playcount[4] = { 0 };
int clearcount[4] = { 0 };
int playtime[4] = { 0 };
int screenshotcount = 0;


//replaydata
struct rpmeta{
	int ver;
	DATEDATA date;
	unsigned int rseed;
	int isd;
	int fastsp;
	int slowsp;
	int score;
	int miss;
	int kill;
	int time;
	int stage;
	double avgfps;
};
char inputreplay[54000] = { 0 };


char gamemode = 1, replaymode = 0;
// 1:title 2:stageselect 3:replayselect 4:records 5:musicroom 6:setting 7:keyconfig 10:game 11:scoreboard 12:replaysave 13:namereplay 0:exit 9:pause
//obj 0:nothing 1:bullet 2:enemy 3:mahouzin 4:effect 5:bg

rpmeta replayreadall(int num){
	rpmeta rpdata={0};
	FILE* rp;
	char path[]="replay/rp00";
	path[9]+=num/10;
	path[10]+=num%10;
	if(fopen_s(&rp,path,"rb")==0){
		fread(&rpdata, sizeof(rpdata), 1, rp);
		for(int i=0;i<rpdata.time;i++){
			fread(&inputreplay[i], sizeof(char), 1, rp);
		}
		fclose(rp);
	}
	return rpdata;
}

rpmeta replayread(int num){
	rpmeta rpdata={0};
	FILE* rp;
	char path[]="replay/rp00";
	path[9]+=num/10;
	path[10]+=num%10;
	if(fopen_s(&rp,path,"rb")==0){
		fread(&rpdata, sizeof(rpdata), 1, rp);
		fclose(rp);
	}
	return rpdata;
}


void replaywrite(rpmeta data,int num){
	FILE* rp;
	char path[]="replay/rp00";
	path[9]+=num/10;
	path[10]+=num%10;
	_mkdir("replay");
	if (fopen_s(&rp, path, "wb") == 0) {
		fwrite(&data, sizeof(rpmeta), 1, rp);
		for(int i=0;i<data.time;i++){
			fwrite(&inputreplay[i],sizeof(char),1,rp);
		}
		fclose(rp);
	}
}


void configread() {
	FILE* config = 0;
	if (fopen_s(&config, "config.ini", "r") == 0) {
		int readflag = 0;
		for (int i = 0; i < 18; i++) {
			int* nump = 0;
			switch (i) {
			case 0:
				nump = &winmode;
				break;
			case 1:
				nump = &windowx;
				break;
			case 2:
				nump = &windowy;
				break;
			case 3:
				nump = &winsizex;
				break;
			case 4:
				nump = &winsizey;
				break;
			case 5:
				nump = &vsync;
				break;
			case 6:
				nump = &sleeptype;
				break;
			case 7:
				nump = &taskprepare;
				break;
			case 8:
				nump = &effecton;
				break;
			case 9:
				nump = &bilinearon;
				break;
			case 10:
				nump = &shotkey;
				break;
			case 11:
				nump = &slowkey;
				break;
			case 12:
				nump = &pausekey;
				break;
			case 13:
				nump = &joypadzone;
				break;
			case 14:
				nump = &musicvol;
				break;
			case 15:
				nump = &sevol;
				break;
			case 16:
				nump = &fastspeed;
				break;
			case 17:
				nump = &slowspeed;
				break;
			}
			while (readflag != '"') {
				readflag = fgetc(config);
				if (readflag == EOF) {
					fclose(config);
					return;
				}
			}
			fscanf_s(config, "%d", nump);
			fgetc(config);
			readflag = 0;
		}
		fclose(config);
	}
}


void configwrite() {
	FILE* config = 0;
	if (fopen_s(&config, "config.ini", "w") == 0) {
		fprintf(config, "[Screen]\n");
		fprintf(config, "WindowMode = \"%d\"\n", winmode);
		fprintf(config, "X = \"%d\"\n", windowx);
		fprintf(config, "Y = \"%d\"\n", windowy);
		fprintf(config, "Width = \"%d\"\n", winsizex);
		fprintf(config, "Height = \"%d\"\n", winsizey);
		fprintf(config, "\n[Process]\n");
		fprintf(config, "VSync = \"%d\"\n", vsync);
		fprintf(config, "SleepType = \"%d\"\n", sleeptype);
		fprintf(config, "BltPrepareTime = \"%d\"\n", taskprepare);
		fprintf(config, "GraphicalEffect = \"%d\"\n", effecton);
		fprintf(config, "DrawMode = \"%d\"\n", bilinearon);
		fprintf(config, "\n[KeyConfig]\n");
		fprintf(config, "Shot = \"%d\"\n", shotkey);
		fprintf(config, "Slow = \"%d\"\n", slowkey);
		fprintf(config, "Pause = \"%d\"\n", pausekey);
		fprintf(config, "JoypadDeadZone = \"%d\"\n", joypadzone);
		fprintf(config, "\n[Option]\n");
		fprintf(config, "MusicVolume = \"%d\"\n", musicvol);
		fprintf(config, "SEVolume = \"%d\"\n", sevol);
		fprintf(config, "MoveSpeedFast = \"%d\"\n", fastspeed);
		fprintf(config, "MoveSpeedSlow = \"%d\"\n", slowspeed);
		fclose(config);
	}
}


void recordread() {
	FILE* savedata = 0;
	if (fopen_s(&savedata, "records.dat", "rb") == 0) {
		fread(&recordid, sizeof(int), 1, savedata);
		if (recordid != gameid) {
			fclose(savedata);
			recordid = gameid;
			return;
		}
		fread(&recordver, sizeof(int), 1, savedata);
		fread(&windowtime, sizeof(int), 1, savedata);
		fread(&allmiss, sizeof(int), 1, savedata);
		fread(&allkill, sizeof(int), 1, savedata);
		fread(&hiscore, sizeof(int), 4, savedata);
		fread(&lowmiss, sizeof(int), 4, savedata);
		fread(&reachscene, sizeof(int), 4, savedata);
		fread(&playcount, sizeof(int), 4, savedata);
		fread(&clearcount, sizeof(int), 4, savedata);
		fread(&playtime, sizeof(int), 4, savedata);
		fread(&screenshotcount, sizeof(int), 1, savedata);
		fclose(savedata);
	}
}


void recordwrite() {
	FILE* savedata = 0;
	if (fopen_s(&savedata, "records.dat", "wb") == 0) {
		fwrite(&gameid, sizeof(int), 1, savedata);
		fwrite(&ver, sizeof(int), 1, savedata);
		fwrite(&windowtime, sizeof(int), 1, savedata);
		fwrite(&allmiss, sizeof(int), 1, savedata);
		fwrite(&allkill, sizeof(int), 1, savedata);
		fwrite(&hiscore, sizeof(int), 4, savedata);
		fwrite(&lowmiss, sizeof(int), 4, savedata);
		fwrite(&reachscene, sizeof(int), 4, savedata);
		fwrite(&playcount, sizeof(int), 4, savedata);
		fwrite(&clearcount, sizeof(int), 4, savedata);
		fwrite(&playtime, sizeof(int), 4, savedata);
		fwrite(&screenshotcount, sizeof(int), 1, savedata);
		fclose(savedata);
	}
}


class GameTask {
	double objx[objmax] = { 0 }, objy[objmax] = { 0 }, objspeed[objmax] = { 0 }, objangle[objmax] = { 0 };
	char objgroup[objmax] = { 0 }, objtype[objmax] = { 0 }, objgraph[objmax] = { 0 };
	int objhp[objmax] = { 0 }, objf[objmax] = { 0 };

	double shotx[shotmax] = { 0 }, shoty[shotmax] = { 0 };
	char shottype[shotmax] = { 0 };

	double px = 0, py = 0;
	int shield = 0, repfast = 0, repslow = 0;
	int star = 0, autoshot = 0, slowpush = 0, pangler = 0, panglel = 0, starsc = 0;
	int score = 0, miss = 0, kill = 0;

	int isdmode=0,power=0,isdwait=0;

	double bosx = 0, bosy = 0, bossp = 0, bosang = 0;
	int bostime = 0, boshpmax = 0, boshp = 0, bosbonus = 0, bonusdrawf = 0, bosflag = 0, bosgraph = 0;

	int shake = 0, shakex = 0, shakey = 0, stage = 0, scene = 0, bggraph = 0;
	int gamef = 0, scenef = 0;
	char bulletsewait[3] = { 0 };

	unsigned int randnum = 0x5c396a7e, nrandnum = 0x5c365930,rseed;

	char nowkey = 0;

	rpmeta rpline[30];
	int playsaved,rpslow,rpfast;

	int menubar = 0, clickwait = 0, barmove = 0, backwait = 0, deathwaitf = 0, deathflag = 0, pausecount = 0;
	int frametime = 0, fps = 0, bulletcount = 0;
	double drawfps = 0;
	double avgfps[900];


	int screenshotwait = 0;

	int playerimg[4];
	int shotimg[2][8];
	int bulletimg[6][7];
	int objimg[3][7];
	int bgimg[5];

	int menuse[3];
	int playerse[3];
	int bulletse[2];
	int enemyse[2];


	int bgm;

public:
	GameTask() {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		char playerpath[] = "pbd/img/player0.bmp";
		for (int i = 0; i < 4; i++) {
			playerimg[i] = LoadGraph(playerpath);
			playerpath[14]++;
		}

		char bulletpath[] = "pbd/img/bullet0.bmp";
		for (int i = 0; i < 6; i++) {
			for (int n = 0; n < 7; n++) {
				bulletimg[i][n] = LoadGraph(bulletpath);
				GraphFilter(bulletimg[i][n], DX_GRAPH_FILTER_HSB, 1, n * 51, 0, 0);
			}
			bulletpath[14]++;
		}

		char bgpath[] = "pbd/img/bg_a.bmp";
		for (int i = 0; i < 5; i++) {
			bgimg[i] = LoadGraph(bgpath);
			bgpath[11]++;
		}

		for (int i = 0; i < 8; i++) {
			shotimg[0][i] = LoadGraph("pbd/img/shot.bmp");
			shotimg[1][i] = LoadGraph("pbd/img/shoth.bmp");
			GraphFilter(shotimg[0][i], DX_GRAPH_FILTER_HSB, 1, i * 45, 0, 0);
			GraphFilter(shotimg[1][i], DX_GRAPH_FILTER_HSB, 1, i * 45, 0, 0);
		}

		for (int i = 0; i < 7; i++) {
			objimg[0][i] = LoadGraph("pbd/img/square.bmp");
			objimg[1][i] = LoadGraph("pbd/img/circle.bmp");
			objimg[2][i] = LoadGraph("pbd/img/mahou.bmp");
			GraphFilter(objimg[0][i], DX_GRAPH_FILTER_HSB, 1, i * 51, 0, 0);
			GraphFilter(objimg[1][i], DX_GRAPH_FILTER_HSB, 1, i * 51, 0, 0);
			GraphFilter(objimg[2][i], DX_GRAPH_FILTER_HSB, 1, i * 51, 0, 0);
		}

		char menupath[] = "pbd/se/menu0.wav";
		for (int i = 0; i < 3; i++) {
			menuse[i] = LoadSoundMem(menupath);
			menupath[11]++;
		}

		char bulletsepath[] = "pbd/se/bullet0.wav";
		for (int i = 0; i < 2; i++) {
			bulletse[i] = LoadSoundMem(bulletsepath);
			bulletsepath[13]++;
		}

		char playersepath[] = "pbd/se/player0.wav";
		for (int i = 0; i < 3; i++) {
			playerse[i] = LoadSoundMem(playersepath);
			playersepath[13]++;
		}

		char enemysepath[] = "pbd/se/enemy0.wav";
		for (int i = 0; i < 2; i++) {
			enemyse[i] = LoadSoundMem(enemysepath);
			enemysepath[12]++;
		}

		setbgm(0);
		
		frametime = GetNowCount();
		setsoundvol();
	}
	void setbgm(char musicnum) {
		if (musicnum != -1) {
			char musicpath[] = "pbd/bgm/music0.wav";
			musicpath[13] += musicnum;
			DeleteSoundMem(bgm);
			bgm = LoadSoundMem(musicpath);
			setloop(musicnum);
			PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, 1);
			setsoundvol();
		}
		else {
			DeleteSoundMem(bgm);
		}
	}
	void setloop(char musicnum) {
		switch (musicnum) {
		case 0:
			SetLoopAreaSamplePosSoundMem(399494, 3512435, bgm);
			break;
		case 1:
			SetLoopAreaSamplePosSoundMem(1081161, 3357290, bgm);
			break;
		case 2:
			SetLoopAreaSamplePosSoundMem(52920, 3969000, bgm);
			break;
		case 3:
			SetLoopAreaSamplePosSoundMem(1320400, 6562000, bgm);
			break;
		case 4:
			SetLoopAreaSamplePosSoundMem(3969000, 4674600, bgm);
			break;
		}
	}
	int rnd(int randmin, int randmax) {
		randnum ^= randnum << 5;
		randnum ^= randnum >> 13;
		randnum ^= randnum << 6;

		int returnnum = randnum % (randmax + 1 - randmin);
		returnnum += randmin;
		return returnnum;
	}
	int nrnd(int randmin, int randmax) {
		nrandnum ^= nrandnum << 5;
		nrandnum ^= nrandnum >> 13;
		nrandnum ^= nrandnum << 6;

		int returnnum = nrandnum % (randmax + 1 - randmin);
		returnnum += randmin;
		return returnnum;
	}
	int rnd2(int randmax) {
		return rnd(0 - randmax, randmax);
	}
	double degrnd(int randmin, int randmax) {
		return deg(rnd(randmin, randmax));
	}
	double degrnd2(int randmax) {
		return deg(rnd2(randmax));
	}
	void draw(double getx, double gety, double extrate, double getangle, int grhandle, int turnflag = 0) {
		DrawRotaGraphF((float)getx + shakex, (float)gety + shakey, extrate, getangle, grhandle, 1, turnflag);
	}
	void playse(int sehandle) {
		PlaySoundMem(sehandle, DX_PLAYTYPE_BACK, 1);
	}
	double deg(int getdeg) {
		double radian = getdeg % 360;
		radian = radian * pi / 180.0;
		return radian;
	}
	double degd(double getdeg) {
		double radian = fmod(getdeg, 360.0);
		radian = radian * pi / 180.0;
		return radian;
	}
	double aim(double getx, double gety) {
		double aimangle = atan2(py - gety, px - getx);
		return aimangle;
	}
	double aimrnd(double getx, double gety, int randmax) {
		double anglereturn = aim(getx, gety) + degrnd2(randmax);
		return anglereturn;
	}
	void keybit(char keytype) {
		if (keytype > 0) {
			char shiftcount = keytype - 1;
			char inputbit = 0b00000001;
			inputbit <<= shiftcount;
			nowkey |= inputbit;
		}
	}
	void keyinput(int inputnum) {
		if (inputnum == 7) {
			if (gamemode == 10) {
				gamemode = 9;
				playse(menuse[1]);
				if (replaymode == 0) {
					pausecount++;
					if (pausecount >= 10) {
						deathflag = 1;
						gamemode = 11;
					}
				}
			}
		}
		else {
			keybit(inputnum);
		}
	}
	int getpadstate(int inputnum) {
		int keymask = 0;
		switch (inputnum) {
		case 1:
			keymask = PAD_INPUT_1;
			break;
		case 2:
			keymask = PAD_INPUT_2;
			break;
		case 3:
			keymask = PAD_INPUT_3;
			break;
		case 4:
			keymask = PAD_INPUT_4;
			break;
		case 5:
			keymask = PAD_INPUT_5;
			break;
		case 6:
			keymask = PAD_INPUT_6;
			break;
		case 7:
			keymask = PAD_INPUT_7;
			break;
		case 8:
			keymask = PAD_INPUT_8;
			break;
		case 9:
			keymask = PAD_INPUT_9;
			break;
		case 10:
			keymask = PAD_INPUT_10;
			break;
		case 11:
			keymask = PAD_INPUT_11;
			break;
		case 12:
			keymask = PAD_INPUT_12;
			break;
		}
		return keymask;
	}
	void inputtask() {
		nowkey = 0;
		for (int i = 1; i < 8; i++) {
			int keystate = 0;
			switch (i) {
			case 1:
				keystate = getpadstate(shotkey);
				break;
			case 2:
				keystate = getpadstate(slowkey);
				break;
			case 3:
				keystate = PAD_INPUT_UP;
				break;
			case 4:
				keystate = PAD_INPUT_DOWN;
				break;
			case 5:
				keystate = PAD_INPUT_LEFT;
				break;
			case 6:
				keystate = PAD_INPUT_RIGHT;
				break;
			case 7:
				keystate = getpadstate(pausekey);
				break;
			}
			if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & keystate) != 0) {
				keyinput(i);
			}
		}
		int joypadx = 0;
		int joypady = 0;
		double joypadangle = 0;
		GetJoypadAnalogInput(&joypadx, &joypady, DX_INPUT_PAD1);
		if (joypadx * joypady != 0) {
			joypadangle = atan2(joypady, joypadx);
			if (degd(-157.5) < joypadangle && joypadangle < degd(-22.5)) {
				keybit(3);
			}
			if (degd(22.5) < joypadangle && joypadangle < degd(157.5)) {
				keybit(4);
			}
			if (degd(112.5) < joypadangle || joypadangle < degd(-112.5)) {
				keybit(5);
			}
			if (degd(-67.5) < joypadangle && joypadangle < degd(67.5)) {
				keybit(6);
			}
		}
	}
	void maintask() {
		windowtime++;

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		inputtask();

		if (gamemode == 1) {
			titletask();
		}
		else if (gamemode == 2) {
			stageselect();
		}
		else if (gamemode == 3) {
			replaylineup();
		}
		else if (gamemode == 4) {
			watchrecord();
		}
		else if (gamemode == 5) {
			musicroomtask();
		}
		else if (gamemode == 6) {
			settingtask();
		}
		else if (gamemode == 7) {
			keycontask();
		}
		else if (gamemode == 9) {
			pausetask();
		}
		else if (gamemode == 10) {
			maingame();
		}
		else if (gamemode == 11) {
			scoreboard();
		}

		fpstask();

		screenshottask();

		if (debug == 1) {
			debugtask();
		}
	}
	void playertask() {

		if (autoshot > 0) {
			autoshot--;
		}

		if (star > 0) {
			star--;
		}

		if (starsc > 0) {
			starsc--;
			/*
			if (starsc == 25) {
				px = 250;
				py = 420;
				slowpush = 0;
			}
			return;
			*/
		}

		if (deathflag == 1) {
			deathwaitf--;
			slowpush = 0;
			if (starsc < 22) {
				starsc = 22;
			}
			if (deathwaitf <= 0) {
				gamemode = 11;
			}
			return;
		}

		if(isdmode){
			if(gamef%2==0){
				if(power>=997&&power<1000){
					playse(playerse[1]);
				}
				power+=3;
			}
		}

		double movex = 0;
		double movey = 0;
		int movefastp = 0;
		int moveslowp = 0;
		if (replaymode == 0) {
			movefastp = fastspeed;
			moveslowp = slowspeed;
		}else{
			movefastp=rpfast;
			moveslowp=rpslow;
		}
		if (getkey(3)) {
			if (!getkey(2)) {
				movey -= movefastp / 4.0;
			}
			else {
				movey -= moveslowp / 4.0;
			}
			pangler += 2;
			panglel -= 2;
		}
		if (getkey(4)) {
			if (!getkey(2)) {
				movey += movefastp / 4.0;
			}
			else {
				movey += moveslowp / 4.0;
			}
			pangler -= 2;
			panglel += 2;
		}
		if (getkey(5)) {
			if (!getkey(2)) {
				movex -= movefastp / 4.0;
			}
			else {
				movex -= moveslowp / 4.0;
			}
			pangler -= 2;
			panglel -= 2;
		}
		if (getkey(6)) {
			if (!getkey(2)) {
				movex += movefastp / 4.0;
			}
			else {
				movex += moveslowp / 4.0;
			}
			pangler += 2;
			panglel += 2;
		}
		if (movex * movey != 0) {
			movex *= sin(deg(45));
			movey *= sin(deg(45));
		}
		px += movex;
		py += movey;
		if (px < 26) {
			px = 26;
		}
		if (px > 474) {
			px = 474;
		}
		if (py < 20) {
			py = 20;
		}
		if (py > 460) {
			py = 460;
		}

		if (pangler > 0) {
			pangler--;
		}
		if (pangler < 0) {
			pangler++;
		}
		if (panglel > 0) {
			panglel--;
		}
		if (panglel < 0) {
			panglel++;
		}
		if (pangler > 5) {
			pangler = 5;
		}
		if (pangler < -5) {
			pangler = -5;
		}
		if (panglel > 5) {
			panglel = 5;
		}
		if (panglel < -5) {
			panglel = -5;
		}

		if (getkey(2)) {
			slowpush++;
		}
		else {
			slowpush--;
		}
		if (slowpush < 0) {
			slowpush = 0;
		}
		if (slowpush > 5) {
			slowpush = 5;
		}

		if (getkey(1)) {
			autoshot = 15;
		}

		if (autoshot > 0) {
			if (gamef % 4 == 0) {
				shot(0);
				playse(playerse[0]);
			}
			else if (gamef % 4 == 2) {
				shot(15);
				shot(-15);
			}
		}
	}
	void shot(int getx) {
		int num = 0;
		while (shottype[num] != 0) {
			num++;
			if (num == shotmax) {
				return;
			}
		}
		shotx[num] = px + getx;
		shoty[num] = py + 20;
		shottype[num] = 1;
	}
	void object(double getx, double gety, double getspeed, double getangle, char getgroup, char gettype, char getgraph, int gethp) {
		int num = 0;
		while (objgroup[num] != 0) {
			num++;
			if (num == objmax) {
				return;
			}
		}
		objx[num] = getx;
		objy[num] = gety;
		objspeed[num] = getspeed;
		objangle[num] = getangle;
		objgroup[num] = getgroup;
		objtype[num] = gettype;
		objgraph[num] = getgraph;
		objhp[num] = gethp;
		objf[num] = 0;
	}
	void bullet(double getx, double gety, double getspeed, double getangle, char getgraph, char gettype = 125, int gethp = 1) {
		object(getx, gety, getspeed, getangle, 1, gettype, getgraph, gethp);
		switch (getgraph) {
		case 0:
			if (bulletsewait[0] == 0) {
				playse(bulletse[0]);
				bulletsewait[0] = 1;
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			if (bulletsewait[1] == 0) {
				playse(bulletse[1]);
				bulletsewait[1] = 1;
			}
			break;
		}
	}
	void way(double getx, double gety, double getspeed, double getangle, char getgraph, int getway, double getwayangle, char gettype = 125, int gethp = 1) {
		double bulletangle = getangle - (getwayangle / 2.0 * (getway - 1));
		for (int i = 0; i < getway; i++) {
			bullet(getx, gety, getspeed, bulletangle, getgraph, gettype, gethp);
			bulletangle += getwayangle;
		}
	}
	void enemy(double getx, double gety, double getspeed, double getangle, char getgraph, char gettype, int gethp = 1) {
		char ecolor = gamef % 7;
		ecolor += getgraph * 7;
		object(getx, gety, getspeed, getangle, 2, gettype, ecolor, gethp);
	}
	void mahou(double getx, double gety, double getspeed, double getangle, char gettype, int gethp = 1) {
		char mcolor = gamef % 7;
		object(getx, gety, getspeed, getangle, 3, gettype, mcolor, gethp);
	}
	void effect(double getx, double gety, double getspeed, double getangle, char gettype, char getgraph) {
		object(getx, gety, getspeed, getangle, 4, gettype, getgraph, 1);
	}
	void effectd(double getx, double gety) {
		for (int i = 0; i < 80; i++) {
			effect(getx, gety, rnd(4, 24) / 2.0, degrnd2(180), 1, rnd(0, 6));
		}
		for (int i = 0; i < 20; i++) {
			effect(getx, gety, rnd(4, 10) / 2.0, degrnd2(180), 2, rnd(0, 6));
		}
	}
	void setobjhp(int getobjnum, int gethp) {
		if (objf[getobjnum] == 1) {
			objhp[getobjnum] = gethp;
		}
	}
	void maingame() {
		if (deathflag == 0) {
			resumebgm();
		}
		else {
			stopbgm();
		}

		replaytask();

		playertask();
		stagetask();
		objtask();
		
		drawobj();
	}
	int cols(double getx1, double gety1, double getx2, double gety2, double getdist) {
		double returndist = pow(pow(getx1 - getx2, 2.0) + pow(gety1 - gety2, 2.0), 0.5);
		if (returndist < getdist) {
			return 1;
		}
		return 0;
	}
	void phit() {
		if (star == 0) {
			miss++;
			shake = 20;
			playse(enemyse[1]);
			int res;
			if(isdmode){
				res=(power>=1000);
			}else{
				res=(shield == 0);
				shield = 480;
			}
			if (res) {
				power/=2;
				//star = 170;
				starsc = 50;
				star=50;
			}
			else {
				star = 100;
				starsc = 50;
				deathflag = 1;
				deathwaitf = 40;
			}
			if (replaymode == 0) {
				allmiss++;
			}
		}
	}
	void healshield() {
		score += 10;
		if (bulletsewait[2] == 0) {
			playse(playerse[2]);
			bulletsewait[2] = 1;
		}
		if (shield > 0) {
			shield--;
			if (shield == 0) {
				playse(playerse[1]);
			}
		}
	}
	void ehit(int getnum) {
		objhp[getnum]--;
		healshield();
		if (objhp[getnum] <= 0) {
			objgroup[getnum] = 0;
			ekill(getnum);
			healshield();
		}
	}
	void boshit() {
		if(star>0&&scene==8&&stage==3){
			return;
		}
		boshp--;
		healshield();
	}
	void ekill(int getnum) {
		kill++;
		score += (100 + kill) * 10;
		effectd(objx[getnum], objy[getnum]);
		playse(enemyse[0]);
		if (replaymode == 0) {
			allkill++;
		}
	}
	void bosmove(double getspeed, int getrndangle) {
		bossp = getspeed;
		bosang = degrnd2(getrndangle);
		int setangle = 5;
		if (bosx < 240) {
			setangle = 4;
		}
		if (bosx > 260) {
			setangle = 6;
		}
		if (rnd(0, 9) < setangle) {
			bosang += deg(180);
		}
	}
	void bosbrake(double getx1, double gety1, double getx2, double gety2, double getaccel = 0.04, double getmove = 3.0) {
		if (bossp > 0) {
			bossp -= getaccel;
		}
		if (bosx < getx1) {
			bosx += getmove;
		}
		if (bosy < gety1) {
			bosy += getmove;
		}
		if (bosx > getx2) {
			bosx -= getmove;
		}
		if (bosy > gety2) {
			bosy -= getmove;
		}
	}
	int outwindow(double getx, double gety, double getdist) {
		if (getx < 20 - getdist) {
			return 1;
		}
		if (gety < 10 - getdist) {
			return 1;
		}
		if (getx > 480 + getdist) {
			return 1;
		}
		if (gety > 470 + getdist) {
			return 1;
		}
		return 0;
	}
	void replaytask() {
		if (replaymode == 0) {
			inputreplay[gamef] = nowkey;
		}
		else {
			nowkey = inputreplay[gamef];
		}
	}
	void stagetask() {
		gamef++;
		scenef++;
		bostime--;

		if (replaymode == 0) {
			playtime[stage]++;
		}

		changescene();

		//memo^^

		//debugst(0);

		//st1
		if (stage == 0) {
			if (gamef == 1) {
				setbgm(1);
			}
			if (scene == 0) {
				setscene(1200, 0, 0);
				if (stp(50, 430, 5)) {
					enemy(rnd(50, 450), 0, 6, degrnd(60, 120), 1, 0);
				}
				if (scenef == 50) {
					enemy(250, 0, 12, deg(90), 0, 1, 45);
				}
				if (stp(100, 420, 60)) {
					enemy(rnd(150, 350), 0, 12, deg(90), 0, 1, 10);
				}
				if (scenef == 470) {
					enemy(200, 0, 12, deg(90), 0, 1, 10);
					enemy(300, 0, 12, deg(90), 0, 1, 10);
				}
				if (scenef == 520) {
					enemy(100, 0, 12, deg(90), 0, 1, 10);
					enemy(400, 0, 12, deg(90), 0, 1, 10);
				}
				if (scenef == 550) {
					enemy(250, 0, 12, deg(90), 0, 1, 10);
				}
				if (stp(620, 1150, 5)) {
					enemy(rnd(50, 450), 0, 6, degrnd(60, 120), 1, 2);
				}
				if (scenef == 1160) {
					eraceobj(1);
					eraceobj(2);
					shake=20;
					for (int i = 0; i < 240; i++) {
						bullet(rnd(0, 500), 0, rnd(10, 80) / 10.0, deg(90), 1);
					}
				}
			}
			if (scene == 1) {
				setscene(1800, 200, 0, 6);
				bosbrake(150, 160, 350, 200, 0.1);
				if (scenef == 1) {
					bosy = 180;
				}
				if (scenef%80==0) {
					bosmove(2, 30);
				}
				if (stp(0, 35, 15, 1800, 1)) {
					for (int i = 0; i < 10; i++) {
						way(bosx, bosy, rnd(300, 320) / 10.0, degrnd2(180), 4, 5, deg(1), 1);
						way(bosx, bosy, rnd(300, 320) / 10.0, degrnd2(180), 2, 2, deg(71), 1);
					}
				}
				if (scenef > 30 && scenef % 10 == 0) {
					for (int i = 0; i < 10; i++) {
						way(bosx, bosy, rnd(300, 320) / 10.0, degrnd2(180), 4, 5, deg(1), 1);
						way(bosx, bosy, rnd(300, 320) / 10.0, degrnd2(180), 2, 2, deg(71), 1);
					}
				}
				if (scenef > 50 && gamef % 7 == 5) {
					for (int i = 0; i < 4; i++) {
						bullet(rnd(0, 500), 0, rnd(20, 30) / 10.0, deg(90), 1);
					}
				}
			}
			if(scene==2){
				setscene(1800,220);
				bosbrake(150, 160, 350, 180, 0.1);
				if(scenef%50==0){
					double ddd=degrnd2(180);
					for(int i=0;i<5;i++){
						mahou(bosx,bosy,3.5,ddd+deg(i*72)+ddd,0);
					}
				}
			}
			if(scene==3){
				setscene(600);
				bosx=250;
				bosy=-80;
				if(stp(0,500,5)){
					enemy(rnd(0,500),0,8,deg(90),0,3);
				}
			}
			if(scene==4){
				setscene(1800,320);
				if(scenef<30){
					bosbrake(px-8,py-8,px+8,py+8,0,18);
				}else{
					bosx=px;bosy=py;
				}	
				if(scenef%160==40){
					double awawa=degrnd2(72);
					for(int i=0;i<5;i++){
						mahou(0,0,500,awawa+deg(i*72),3,100);
					}
				}
				if(scenef%160==120){
					double awawa=degrnd2(72);
					for(int i=0;i<5;i++){
						mahou(0,0,500,awawa+deg(i*72),4,100);
					}
				}
			}
			if(scene==5){
				setscene(2700,320);
				bosbrake(248,200,252,220,0.1,4);
				if(scenef>40){
					way(bosx+rnd2(50),bosy+rnd2(50),2+pow(scenef-20,0.5)/4.0,aim(bosx,bosy),2,4,deg(90),3);
				}
			}
			if(scene==6){
				setscene(3600,600);
				bosbrake(248,200,252,220,0.1,8);
				if(scenef%60==40){
					double ddr=degrnd2(180);
					int ddw=8+scenef/120;
					for(int i=0;i<ddw;i++){
						mahou(bosx,bosy,4.5,ddr+degd(i*360.0/ddw),2);
					}
				}
			}
			if(scene==7){
				scene = 100;
			}
		}

		//st2
		if (stage == 1) {
			if (gamef == 1) {
				setbgm(2);
			}
			if (scene == 0) {
				setscene(1510, 0, 0);
				if (stp(50, 250, 5)) {
					enemy(rnd(400, 500), 0, 5, deg(90), 1, 0);
				}
				if (stp(100,300,120)) {
					enemy(200, 0, 6, aim(200, 0), 0, 1, 12);
				}
				if (stp(230, 500, 5)) {
					enemy(rnd(0, 100), 0, 5, deg(90), 1, 2);
				}
				if (stp(480, 850, 5)) {
					enemy(rnd(0, 500), 0, 8, degrnd(60, 120), 1, 3);
				}
				if (stp(300, 500, 120)) {
					enemy(300, 0, 6, aim(300, 0), 0, 1, 15);
				}
				if (stp(500, 800, 100)) {
					enemy(rnd(180, 320), 0, 8, deg(90), 0, 4, 20);
				}
				if (stp(870, 1180, 5)) {
					enemy(50, 0, 4, deg(90), 1, 5);
					enemy(450, 0, 4, deg(90), 1, 6);
				}
				if (scenef == 1130) {
					for (int i = 0; i < 5; i++) {
						enemy(50 + i * 100, 0, 1, deg(90), 0, 7, 150);
					}
				}
			}
			if (scene == 1) {
				setscene(1800, 240, 0, 0);
				bosbrake(200, 120, 300, 200, 0);
				if (stp(0, 99, 20)) {
					way(bosx + rnd2(59), bosy + rnd2(40), 1, degrnd2(180), 2, 30, deg(12), 1);
				}
				if (stp(100, 199, 10)) {
					way(bosx + rnd2(59), bosy + rnd2(40), 1, degrnd2(180), 2, 30, deg(12), 1);
				}
				if (stp(200, 1800, 5)) {
					way(bosx + rnd2(59), bosy + rnd2(40), 1, degrnd2(180), 2, 30, deg(12), 1);
				}
			}
			if (scene == 2) {
				setscene(1800, 260);
				bosbrake(150, 100, 350, 140, 0.04);
				if (stp(10, 100, 30, 300)) {
					bosmove(2, 40);
				}
				if (scenef % 300 == 10) {
					for (int i = 0; i < 5; i++) {
						mahou(bosx, bosy, 0, deg(i * 72), 0, 200);
					}
				}
				if (scenef % 300 == 160) {
					for (int i = 0; i < 30; i++) {
						mahou(bosx, bosy, 8 + i / 5, deg(i * 33), 1);
					}
				}
			}
			if (scene == 3) {
				setscene(1800, 240);
				bosbrake(150, 120, 350, 160, 0.04);
				if (scenef % 8 == 0) {
					mahou(bosx, bosy, 6, degd(15.0 + scenef * 3.75), 2);
					bullet(bosx, bosy, 6, degd(15.0 + scenef * 3.75), 0);
				}
				if (scenef % 200 == 60) {
					bosmove(2, 45);
				}
			}
			if (scene == 4) {
				setscene(800);
				bosx = 250;
				bosy = -80;
				if (stp(20, 80, 20)) {
					enemy(rnd(50, 200), 0, 3, deg(90), 0, 8, 8);
				}
				if (stp(10, 120, 5)) {
					enemy(300, 0, 4, deg(90), 1, 5);
				}
				if (stp(120, 200, 20)) {
					enemy(rnd(300, 450), 0, 3, deg(90), 0, 8, 8);
				}
				if (stp(120, 240, 5)) {
					enemy(200, 0, 4, deg(90), 1, 6);
				}
				if (stp(240, 320, 20)) {
					enemy(rnd(50, 200), 0, 3, deg(90), 0, 8, 8);
				}
				if (stp(240, 360, 5)) {
					enemy(300, 0, 4, deg(90), 1, 5);
				}
				if (stp(360, 440, 20)) {
					enemy(rnd(300, 450), 0, 3, deg(90), 0, 8, 8);
				}
				if (stp(360, 480, 5)) {
					enemy(200, 0, 4, deg(90), 1, 6);
				}
				if (stp(510, 700, 5)) {
					enemy(rnd(400, 500), 0, 5, deg(90), 1, 0);
					enemy(rnd(0, 100), 0, 5, deg(90), 1, 2);
				}
			}
			if (scene == 5) {
				setscene(1800, 240);
				bosbrake(200, 240, 300, 480, 0.05, 8);
				if (scenef > 30) {
					if (scenef % 15 == 0) {
						way(bosx, bosy, 6, degrnd2(180), 1, 60, deg(6), 5);
					}
				}
			}
			if (scene == 6) {
				setscene(3600, 300);
				if (scenef == 1) {
					bosang = deg(-180);
					bosy = 240;
				}
				bosang += degd(0.5 + scenef / 28.0);
				if (bosang > 0)bosang -= deg(180);
				for (int i = 0; i < 3; i++) {
					bullet(bosx + rnd2(20), bosy + rnd2(20), rnd(16, 23) / 3.0, bosang + degrnd2(5), 3, 7, 150);
					bullet(bosx, bosy, rnd(16, 23) / 3.0, bosang + degrnd2(5), 1, 7, 150);
				}
				if (scenef % 2 == 0) {
					bullet(bosx, bosy, rnd(16, 23) / 3.0, bosang, 0, 7, 150);
				}
			}
			if (scene == 7) {
				setscene(1800, 220);
				if (scenef == 1)bosy = 240;
				bosbrake(200, 158, 300, 160, 0.04, 1);
				if (scenef % 36 == 15) {
					bosmove(0.8, 45);
				}
				if (scenef % 72 == 1) {
					mahou(bosx, bosy, 0, deg(90), 3);
				}
				if (scenef % 72 == 37) {
					mahou(bosx, bosy, 0, deg(90), 4);
				}
				if (scenef % 15 == 0) {
					way(bosx, bosy, 4, aim(bosx, bosy) + deg(2), 3, 15, deg(24));
					way(bosx, bosy, 4, aim(bosx, bosy) - deg(2), 3, 15, deg(24));
					way(bosx, bosy, 4.1, aim(bosx, bosy), 3, 15, deg(24));
				}
			}
			if (scene == 8) {
				setscene(750);
				if (scenef == 1)bosy = 160;
				bosbrake(100, -120, 400, 160, 0.04);
				bosy -= 0.25;
				if (scenef % 50 == 5 && scenef < 450) {
					bosmove(2, 0);
				}
				if (scenef > 450) {
					bosy -= 0.5;
				}
				if (scenef % 15 == 0) {
					way(bosx, bosy, 4, aim(bosx, bosy) + deg(2), 3, 15, deg(24));
					way(bosx, bosy, 4, aim(bosx, bosy) - deg(2), 3, 15, deg(24));
					way(bosx, bosy, 4.1, aim(bosx, bosy), 3, 15, deg(24));
				}
				if (stp(20, 70, 13)) {
					enemy(50, 0, 4, deg(90), 0, 9, 3);
				}
				if (stp(70, 120, 13)) {
					enemy(450, 0, 4, deg(90), 0, 10, 3);
				}
				if (stp(120, 220, 13)) {
					enemy(50, 0, 4, deg(90), 0, 11, 3);
				}
				if (stp(220, 320, 13)) {
					enemy(450, 0, 4, deg(90), 0, 12, 3);
				}
				if (stp(320, 370, 13)) {
					enemy(50, 0, 4, deg(90), 0, 9, 3);
				}
				if (stp(370, 420, 13)) {
					enemy(450, 0, 4, deg(90), 0, 10, 3);
				}
				if (stp(440, 650, 5)) {
					enemy(50, 0, 4, deg(90), 1, 5);
					enemy(450, 0, 4, deg(90), 1, 6);
				}
			}
			if (scene == 9) {
				setscene(3600, 120);
				bossp -= 0.1;
				if (scenef % 200 == 1) {
					bosy = 0;
					bossp = 10;
					bosang = deg(90);
					for (int i = 0; i < 7; i++) {
						way(bosx, bosy, 2.0 + i / 2.0, deg(i * 10), 0, 24, deg(20));
					}
					for (int i = 0; i < 80; i++) {
						bullet(bosx + rnd2(30), bosy, rnd(20, 120) / 10.0, deg(90), 3);
						bullet(bosx + rnd2(60), bosy, rnd(20, 90) / 10.0, deg(90), 3);
						bullet(bosx + rnd2(250), bosy, rnd(40, 60) / 10.0, deg(90), 3);
						bullet(bosx + rnd2(400), bosy, rnd(20, 40) / 10.0, deg(90), 3);
					}
				}
				if (stp(0, 100, 1, 200)) {
					way(bosx, bosy, 2.0 + (scenef % 200) / 40.0, bosang + degrnd2(30), 2, 3, deg(30));
				}
				else {
					bosbrake(px - 3, -80, px + 3, 1000, 0, 2.25);
				}
			}
			if (scene == 10) {
				setscene(3600, 400);
				if (scenef > 100) {
					if (stp(0, 11, 1, 50)) {
						way(bosx, bosy, 6, deg(scenef * 90), 0, 6, deg(15));
					}
					if (stp(160, 200, 1, 200) && gamef % 7 == 3) {
						way(bosx, bosy, 6, degrnd2(180), 3, 72, deg(5), 8);
					}
					if (stp(10, 50, 1, 200) && gamef % 7 == 5) {
						way(bosx, bosy, 6, degrnd2(180), 3, 72, deg(5), 9);
					}
					if (stp(60, 100, 1, 200) && gamef % 7 == 0) {
						way(bosx, bosy, 6, degrnd2(180), 3, 72, deg(5), 10);
					}
					if (stp(110, 150, 1, 200) && gamef % 7 == 1) {
						way(bosx, bosy, 6, degrnd2(180), 3, 72, deg(5));
					}
				}
				else {
					bosbrake(245, 95, 255, 105, 0.1, 6);
					if (stp(0, 11, 1, 25) && scenef >= 50) {
						way(bosx, bosy, 6, deg(scenef * 90), 0, 6, deg(15));
					}
				}
			}
			if (scene == 11) {
				scene = 100;
			}
		}

		//st3
		if (stage == 2) {
			if (gamef == 1) {
				setbgm(3);
			}
			if (scene == 0) {
				setscene(1160, 0, 0);
				if (stp(50, 90, 5)) {
					enemy(40, 480, 12, deg(-90), 1, 0);
				}
				if (stp(100, 140, 5)) {
					enemy(460, 480, 12, deg(-90), 1, 1);
				}
				if (stp(150, 190, 5)) {
					enemy(40, 480, 12, deg(-90), 1, 0);
				}
				if (stp(200, 240, 5)) {
					enemy(460, 480, 12, deg(-90), 1, 1);
				}
				if (stp(250, 290, 5)) {
					enemy(0, 60, 14, deg(0), 1, 2);
				}
				if (stp(300, 340, 5)) {
					enemy(500, 60, 14, deg(180), 1, 2);
				}
				if (stp(350, 420, 5)) {
					enemy(0, 80, 14, deg(0), 1, 4);
					enemy(500, 80, 14, deg(180), 1, 4);
				}
				if (stp(450, 490, 5)) {
					enemy(460, 480, 12, deg(-90), 1, 1);
				}
				if (stp(500, 540, 5)) {
					enemy(40, 480, 12, deg(-90), 1, 0);
				}
				if (stp(550, 590, 5)) {
					enemy(500, 60, 14, deg(180), 1, 2);
				}
				if (stp(600, 640, 5)) {
					enemy(0, 60, 14, deg(0), 1, 2);
				}
				if (stp(650, 690, 5)) {
					enemy(500, 60, 14, deg(180), 1, 2);
				}
				if (stp(700, 740, 5)) {
					enemy(0, 60, 14, deg(0), 1, 2);
				}
				if (stp(750, 820, 5)) {
					enemy(0, 80, 14, deg(0), 1, 4);
					enemy(500, 80, 14, deg(180), 1, 4);
				}
				if (stp(850, 1000, 5)) {
					enemy(40, 480, 12, deg(-90), 1, 0);
					enemy(460, 480, 12, deg(-90), 1, 1);
				}
			}
			if (scene == 1) {
				setscene(1800, 240, 0, 1);
				bosbrake(200, 180, 300, 200, 0.1, 7);
				if (scenef == 40) {
					for (int i = 0; i < 18; i++) {
						mahou(bosx, bosy, 5, deg(i * 20), 0, 20);
						for (int n = 0; n < 30; n++) {
							bullet(bosx + rnd2(10), bosy + rnd2(10), rnd(40, 50) / 10.0, deg(i * 20), 3);
						}
						for (int n = 0; n < 8; n++) {
							bullet(bosx + rnd2(15), bosy + rnd2(15), rnd(40, 50) / 10.0, deg(i * 20), 1);
						}
						bullet(bosx, bosy, 5, deg(i * 20), 0);
					}
				}
				if (scenef > 70 && scenef % 12 == 0) {
					bosmove(1.5, 40);
					mahou(bosx, bosy, 5, degrnd(0, 90), 0, 20);
					mahou(bosx, bosy, 5, degrnd(90, 180), 0, 20);
					mahou(bosx, bosy, 5, degrnd(180, 270), 0, 20);
					mahou(bosx, bosy, 5, degrnd(270, 360), 0, 20);
				}
			}
			if(scene==2){
				setscene(540);
				bosx=250;
				bosy=-80;
				if(stp(20,100,5)){
					enemy(500,60,8,deg(180),1,3);
				}
				if(stp(100,180,5)){
					enemy(0,60,8,0,1,3);
				}
				if(stp(180,260,5)){
					enemy(500,60,8,deg(180),1,3);
				}
				if(stp(260,340,5)){
					enemy(0,60,8,0,1,3);
				}
				if(stp(340,420,5)){
					enemy(500,60,8,deg(180),1,3);
				}
				if(stp(420,500,5)){
					enemy(0,60,8,0,1,3);
				}
			}
			if(scene==3){
				setscene(1800,180);
				bosbrake(150,120,350,160,0.1,6);
				if(scenef%80==30){
					for(int i=0;i<4;i++){
						mahou(bosx,bosy,5,deg(i*60),1);
					}
				}
				if(scenef%80==70){
					bosmove(3.0,30);
				}
			}
			if(scene==4){
				setscene(550);
				bosx=-40;
				bosy=80;
				if(stp(0,550,3)){
					enemy(rnd(30,470),0,4,deg(90),1,5);
				}
			}
			if(scene==5){
				setscene(1800,40);
				if(scenef==1){
					bosx=-40;
					bossp=8;
					eraceobj(1);
					eraceobj(2);
				}
				if(bossp<120){
					bossp+=0.1;
				}
				if(bosx>520){
					bosy=200;
					bosang=degrnd(180,200);
				}
				if(bosx<-20){
					bosy=200;
					bosang=degrnd(-20,0);
				}
				if(scenef%6==0){
					for(int n=0;n<4;n++){
						bullet(bosx,bosy,5,degrnd(-180,0),1,1,180);
					}
				}
			}
			if(scene==6){
				setscene(3600,220);
				bosbrake(245,155,255,165,0,8);
				if(scenef>50){
					if(stp(0,50,1,100)){
						way(bosx,bosy,6.0,deg(scenef*20+rnd2(10)),1,4,deg(90),2);
						bullet(bosx,bosy,1.5,deg(scenef*20+rnd2(10)),0,2);
					}
					if(stp(50,100,1,100)){
						way(bosx,bosy,6.0,deg(scenef*-20+rnd2(10)),1,4,deg(90),3);
						bullet(bosx,bosy,1.5,deg(scenef*-20+rnd2(10)),0,3);
					}
				}
			}
			if(scene==7){
				setscene(660);
				bosx=250;
				bosy=-80;
				if (stp(0, 240, 5)) {
					enemy(0, 60, 15.0, deg(0), 1, 4);
					enemy(500, 60, 15.0, deg(180), 1, 4);
				}
				if(stp(300,380,5)){
					enemy(0,60,8,0,1,3);
				}
				if(stp(380,460,5)){
					enemy(500,60,8,deg(180),1,3);
				}
				if(stp(460,540,5)){
					enemy(0,60,8,0,1,3);
				}
				if(stp(540,620,5)){
					enemy(500,60,8,deg(180),1,3);
				}
			}
			if(scene==8){
				setscene(1800,180,0,2);
				bosbrake(150,100,350,140,0.1,8);
				if(scenef%40==0){
					bosmove(2.0,30);
				}
				if(scenef%6==0){
					for(int i=0;i<12;i++){
						bullet(bosx+rnd2(20),bosy+rnd2(20),3.0,degrnd2(180),1,5);
					}
				}
				if(scenef%25==0&&scenef>80){
					way(bosx,bosy,3.0,degrnd2(180),0,24,deg(15),5);
				}
			}
			if(scene==9){
				setscene(1050);
				bosx=250;
				bosy=-80;
				if(stp(0,780,5)){
					enemy(0,80,8,deg(0),1,9);
					enemy(500,80,8,deg(180),1,9);
				}
				if(stp(0,740,20)){
					enemy(250+sin(deg(scenef-20))*100,0,6,deg(90),0,8,80);
				}
				if(stp(780,1000,5)){
					int iaa=scenef-700;
					enemy(0,iaa,8,deg(0),1,9);
					enemy(500,iaa,8,deg(180),1,9);
				}
			}
			if(scene==10){
				setscene(1800,230);
				bosbrake(240,120,260,140,0,12);
				if(scenef%5==0){
					way(bosx+rnd2(20),bosy+rnd2(20),4.5,deg(scenef),3,18,deg(20));
					way(bosx+rnd2(20),bosy+rnd2(20),3.5,deg(-scenef),3,18,deg(20));
				}
				if(scenef%8==0&&scenef>60){
					for(int n=0;n<40;n++){
						bullet(bosx+rnd2(60),bosy+rnd2(60),rnd(8,32),deg(-90+scenef*10),0);
					}
				}
			}
			if(scene==11){
				setscene(930);
				if(stp(0,179,1)){
					bosx=250;
					bosy=80;
					if(scenef%5==0){
					int ddd=scenef;
					if(scenef>130)ddd=130;
					mahou(0+ddd,rnd(-20,0),18,deg(90),2);
					mahou(500-ddd,rnd(-20,0),18,deg(90),2);
					}
				}
				if(stp(180,899,1)){
					bosx=250-sin(deg(-scenef))*80;
					if(scenef%5==0){
					mahou(130-sin(deg(-scenef))*80,rnd(-20,0),18,deg(90),2);
					mahou(370-sin(deg(-scenef))*80,rnd(-20,0),18,deg(90),2);
					}
				}
				if(stp(900,930,1)){
					bosx=250;
					if(scenef%5==0){
					mahou(130,rnd(-20,0),18,deg(90),2);
					mahou(370,rnd(-20,0),18,deg(90),2);
					}
				}
			}
			if(scene==12){
				setscene(1800,240);
				bosy=80;
				if(stp(0,130,1)){
					if(scenef%5==0){
					int ddd=scenef;
					mahou(130-ddd,rnd(-20,0),18,deg(90),2);
					mahou(370+ddd,rnd(-20,0),18,deg(90),2);
					mahou(rnd(-20,0),bosy,18,deg(0),2);
					}
				}else{
					if(scenef%5==0){
					mahou(0,rnd(-20,0),18,deg(90),2);
					mahou(500,rnd(-20,0),18,deg(90),2);
					mahou(rnd(-20,0),bosy,18,deg(0),2);
					}
				}
			}
			if(scene==13){
				setscene(7200,900);
				bosy=80;
				for(int n=0;n<4;n++){
				way(bosx+rnd2(10),bosy+rnd2(10),rnd(18,24),deg(-10),5,15,deg(1));
				way(bosx+rnd2(10),bosy+rnd2(10),rnd(18,24),deg(190),5,15,deg(1));
				way(bosx+rnd2(10),bosy+rnd2(10),rnd(18,30),deg(-10),5,15,deg(1));
				way(bosx+rnd2(10),bosy+rnd2(10),rnd(18,30),deg(190),5,15,deg(1));
				}
				if(scenef<40){
					for(int n=0;n<4;n++){
					way(bosx,bosy+rnd2(10),rnd(18,24),deg(-80),5,7,deg(1));
					way(bosx,bosy+rnd2(10),rnd(18,24),deg(260),5,7,deg(1));
					way(bosx,bosy+rnd2(10),rnd(18,30),deg(-80),5,7,deg(1));
					way(bosx,bosy+rnd2(10),rnd(18,30),deg(260),5,7,deg(1));
					}
				}
				if(scenef==40){
					mahou(bosx,bosy,0,deg(-80),3);
					mahou(bosx,bosy,0,deg(260),4);
				}
				if(scenef==120){
					mahou(bosx,bosy,0,deg(0),3);
					mahou(bosx,bosy,0,deg(180),4);
				}
				if(scenef>=200){
				if(stp(0,20,1,200)){
					way(bosx,bosy,18,deg(scenef*28),0,5,deg(72),4,100);
				}
				if(scenef%160==40){
					mahou(bosx,bosy,0,deg(100),3);
					mahou(bosx,bosy,0,deg(80),4);
				}
				if(scenef%160==120){
					mahou(bosx,bosy,0,deg(0),3);
					mahou(bosx,bosy,0,deg(180),4);
				}
				if(scenef%8==0){
					int aaa=scenef/150;
					if(aaa>10)aaa=10;
					for(int n=0;n<aaa;n++){
						bullet(rnd(0,500),rnd(0,80),rnd(30,40)/10.0,degrnd(80,100),1);
					}
				}
				}
			}
			if (scene == 14) {
				scene = 100;
			}
		}

		//st4
		if(stage==3){
			if(gamef==1){
				setbgm(4);
			}
			if(scene==0){
				setscene(1080,0,0);
				if(scenef==60){
					enemy(250,-20,6,deg(90),0,0,300);
				}
				if(stp(540,820,30)){
					mahou(rnd(100,400),rnd(40,240),0,0,0);
				}
				if(stp(840,950,15)){
					mahou(rnd(100,400),rnd(40,240),0,0,0);
				}
				if(stp(960,1019,15,0,0)||stp(960,1019,15,0,7)){
					mahou(rnd(100,400),rnd(40,240),0,0,0);
				}
				if(stp(1020,1049,4)){
					mahou(rnd(100,400),rnd(40,240),0,0,0);
				}
				if(stp(1050,1080,2)){
					mahou(rnd(100,400),rnd(40,240),0,0,0);
				}
			}
			if(scene==1){
				setscene(1800,280,0,3);
				bosy=160;
				if(stp(120,240,2,240)){
					mahou(rnd(100,400),rnd(40,200),0,0,2);
				}
			}
			if(scene==2){
				setscene(1800,220);
				bosbrake(150,160,350,200,0.1,2);
				if(scenef==1){
					bosy=160;
				}
				if(scenef%40==0){
					bosmove(2.5,30);
				}
				if(scenef%6==0){
					mahou(bosx,bosy,8,deg(-15),1);
					mahou(bosx,bosy,8,deg(195),1);
				}
			}
			if(scene==3){
				setscene(1800,240);
				bosbrake(150,160,350,200,0.1,2);
				if(scenef==1){
					bosy=160;
				}
				if(scenef%120==0){
					bosmove(2.5,30);
				}
				if(scenef%8==0){
					mahou(bosx,bosy,8,deg(scenef*2),3);
					mahou(bosx,bosy,8,deg(scenef*2+180),3);
				}
			}
			if(scene==4){
				setscene(1800,200);
				bosbrake(200,160,300,200,0.1,2);
				if(scenef%40==0){
					bosmove(2.0,30);
				}
				if(scenef%30==0){
					for(int n=rnd2(35);n<500;n+=70){
						mahou(n,10,0.5,deg(0),4);
					}
				}
				if(scenef%30==15){
					for(int n=rnd2(35);n<500;n+=70){
						mahou(500-n,10,0.5,deg(180),4);
					}
				}
			}
			if(scene==5){
				if(gamef<3240){
					setscene(3240-gamef);
				}else{
					setscene(1);
				}
				bosx=250;
				bosy=-80;
				if(gamef<3210){
				for(int i=0;i<6;i++){
					double xxx=rnd(20,480);
					double yyy=rnd(10,470);
					if(!cols(px,py,xxx,yyy,30)){
						bullet(xxx,yyy,0.15,degrnd2(180),4,3);
					}
				}
				}
			}
			if(scene==6){
				setscene(7200,700);
				bosbrake(200,160,300,200,0.1,4);
				if(scenef==1){
					for(int i=0;i<12;i++){
						way(250,-20,3.0+i/2.0,deg(i*5),0,36,deg(10));
					}
				}
				if(scenef==60){
					for(int i=0;i<10;i++){
						mahou(0,0,0,deg(i*36),5);
						mahou(0,0,0,deg(i*36+18),6);
					}
				}
				if(scenef%240==0){
					way(bosx,bosy,7.5,aim(bosx,bosy)+degrnd(178,182),0,24,deg(15),4);
				}
				if(boshp<500&&scenef%240==120){
					way(bosx,bosy,7.5,aim(bosx,bosy)+degrnd(178,182),0,24,deg(15),4);
				}
				if(boshp<300&&scenef%120==60){
					way(bosx,bosy,7.5,aim(bosx,bosy)+degrnd(178,182),0,24,deg(15),4);
				}
				if(boshp<100&&scenef%60==30){
					way(bosx,bosy,7.5,aim(bosx,bosy)+degrnd(178,182),0,24,deg(15),4);
				}
			}
			if(scene==7){
				setscene(7200,700);
				bosbrake(200,110,300,120,0.1,1);
				if(scenef==1){
					double dd=0;
					for(int i=1;i<30;i++){
						mahou(bosx,bosy,i/2.0,dd,7,100);
						mahou(bosx,bosy,i/2.0,dd+deg(180),8,100);
					}
				}
				if(scenef>30){
				if(scenef%60==0){
					double dd=degrnd(15,45);
					for(int i=1;i<30;i++){
						mahou(bosx,bosy,i/2.0,dd,7,100);
						mahou(bosx,bosy,i/2.0,dd+deg(180),8,100);
					}
				}
				if(scenef%60==30){
					double dd=degrnd(-45,-15);
					for(int i=1;i<30;i++){
						mahou(bosx,bosy,i/2.0,dd,7,100);
						mahou(bosx,bosy,i/2.0,dd+deg(180),8,100);
					}
				}
				}
			}
			if(scene==8){
				setscene(3600,100);
				bosy=120;
				if(scenef==1){
					double dd=degrnd2(180);
					way(bosx,bosy,8,dd,3,30,deg(12));
					way(bosx,bosy,8.06,dd,5,30,deg(12));
				}
				if(scenef>60){
					double dd=degrnd2(180);
					double xx=bosx+rnd2(20);
					double yy=bosy+rnd2(5);
					way(xx,yy,8,dd,3,30,deg(12));
					way(xx,yy,8.06,dd,5,30,deg(12));
				}
			}
			if (scene == 9) {
				scene = 100;
			}
		}


		if (scene == 100) {
			stopbgm();
			setscene(80000);
			if (stp(0, 70, 10)) {
				shake = 10;
				for (int i = 0; i < 3; i++) {
					effectd(bosx + rnd2(120), bosy + rnd2(120));
				}
				if (scenef == 70) {
					shake = 30;
					effectd(bosx, bosy);
					for (int i = 0; i < 5; i++) {
						effectd(bosx + rnd2(120), bosy + rnd2(120));
					}
					bosy = -80;
				}
				playse(enemyse[1]);
			}
			if (scenef == 150) {
				if (replaymode == 0) {
					clearcount[stage]++;
					if (score > hiscore[stage]) {
						hiscore[stage] = score;
					}
					if (clearcount[stage] == 1) {
						lowmiss[stage] = miss;
					}
					if (miss < lowmiss[stage]) {
						lowmiss[stage] = miss;
					}
				}
				gamemode = 11;
			}
		}
	}
	void eraceobj(int getobjnum) {
		for (int i = 0; i < objmax; i++) {
			if (objgroup[i] == getobjnum) {
				objgroup[i] = 0;
			}
		}
	}
	void nextscene() {
		scene++;
		scenef = 1;
		if (bosflag == 1) {
			eraceobj(1);
			eraceobj(2);
			eraceobj(3);
			score += kill * bostime * 10;
			bosbonus = kill * bostime * 10;
			bonusdrawf = 100;
			shake = 20;
			effectd(bosx, bosy);
			for (int i = 0; i < 8; i++) {
				effectd(bosx + rnd2(120), bosy + rnd2(40));
			}
			playse(enemyse[1]);
		}
		bosflag = 0;
		bossp = 0;
		if (reachscene[stage] < scene && replaymode == 0) {
			reachscene[stage] = scene;
		}
	}
	void changescene() {
		if (bosflag == 1) {
			if (boshp <= 0) {
				nextscene();
				return;
			}
		}
		if (bostime <= 0) {
			bosflag = 0;
			nextscene();
		}
	}
	void setscene(int gettimer, int getboshp = 0, int getbg = 130, int getbosgraph = 9) {
		if (scenef == 1) {
			bostime = gettimer;
			if (getbg < 128) {
				bggraph = getbg;
			}
			bosflag = 0;
			if (getboshp > 0) {
				bosflag = 1;
				boshp = getboshp;
				boshpmax = getboshp;
				if (getbosgraph < 7) {
					bosgraph = getbosgraph;
				}
			}
		}
	}
	int stp(int sectstart, int sectend, int sectrapid, int sectloop = 0, int secttime = 0) {
		int sectf = scenef;
		if (sectloop > 0) {
			sectf %= sectloop;
		}
		if (sectf >= sectstart && sectf <= sectend) {
			if ((sectf % sectrapid) == secttime) {
				return 1;
			}
		}
		return 0;
	}
	void pausetask() {
		stopbgm();
		drawobj();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
		DrawBox(20, 10, 480, 470, GetColor(27, 2, 29), 1);
		DrawBox(40, 120, 250, 246, GetColor(0, 0, 0), 1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawBox(50, 178 + (menubar * 20), 240, 198 + (menubar * 20), GetColor(45, 25, 53), 1);
		DrawBox(50, 178 + (menubar * 20), 240, 198 + (menubar * 20), GetColor(145, 125, 153), 0);
		DrawString(60, 130, "Pause", white);

		DrawString(60, 180, "Resume", white);
		DrawString(60, 200, "Quit", white);
		DrawString(60, 220, "Retry", white);

		barmovetask(2);
		if (backtask()) {
			menubar = 1;
		}
		if (clicktask()) {
			switch (menubar) {
			case 0:
				gamemode = 10;
				break;
			case 1:
				setbgm(0);
				if(replaymode){
					gamemode = 3;
				}else{
					gamemode = 1;
				}
				break;
			case 2:
				startgame();
				break;
			}
			menubar = 0;
		}
	}
	void drawobj() {
		drawbg();


		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);

		if(effecton){
		for (int i = 0; i < objmax; i++) {
			if (objgroup[i] == 4) {
				double efexr = 1;
				double efang = 0;
				char efgraph = 0;
				switch (objtype[i]) {
				case 1:
					efexr = 0.5 - objf[i] / 100.0;
					efang = objangle[i] + deg(objf[i] * 5 + gamef * 4);
					break;
				case 2:
					efexr = 0.8 - objf[i] / 50.0;
					efgraph = 1;
					break;
				}
				draw(objx[i], objy[i], efexr, efang, objimg[efgraph][objgraph[i]]);
			}
		}
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 191);

		for (int i = 0; i < objmax; i++) {
			if (objgroup[i] == 2) {
				int ecolor = objgraph[i] % 7;
				if (objgraph[i] < 7) {
					draw(objx[i], objy[i], 0.3, deg(objf[i] * 5), objimg[0][ecolor]);
					ecolor += 1;
					ecolor %= 7;
					draw(objx[i], objy[i], 0.45, deg(objf[i] * -4), objimg[0][ecolor]);
					ecolor += 1;
					ecolor %= 7;
					draw(objx[i], objy[i], 0.6, deg(objf[i] * 3), objimg[0][ecolor]);
				}
				else {
					draw(objx[i], objy[i], 0.3, deg(objf[i] * 5), objimg[0][ecolor]);
					ecolor += 1;
					ecolor %= 7;
					draw(objx[i], objy[i], 0.45, deg(objf[i] * -4), objimg[0][ecolor]);
				}
			}
		}

		if (bosgraph >= 0) {
			int boscolor = bosgraph % 7;
			draw(bosx, bosy, 0.4, deg(gamef * -5), objimg[0][boscolor]);
			boscolor += 1;
			boscolor %= 7;
			draw(bosx, bosy, 0.6, deg(gamef * 4), objimg[0][boscolor]);
			boscolor += 1;
			boscolor %= 7;
			draw(bosx, bosy, 0.8, deg(gamef * -3), objimg[0][boscolor]);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);

		for (int i = 0; i < objmax; i++) {
			if (objgroup[i] == 3) {
				double mexr = 0.5 + (sin(deg(gamef * 8)) * 0.1);
				draw(objx[i], objy[i], mexr, deg(objf[i] * 6), objimg[2][objgraph[i]]);
			}
		}

		for (int i = 0; i < shotmax; i++) {
			if (shottype[i] > 0) {
				int scolor = gamef / 4 % 8;
				if (shottype[i] == 1) {
					draw(shotx[i], shoty[i], 1.25, 0, shotimg[0][scolor]);
				}
				else {
					draw(shotx[i], shoty[i], 1.5, 0, shotimg[1][scolor]);
				}
			}
		}

		if (deathflag==0) {
			if (star == 0) {
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
			else {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 191);
			}
			draw(px, py, 1, 0, playerimg[1]);
			draw(px, py, 1, deg(pangler * 2), playerimg[2], 0);
			draw(px, py, 1, deg(panglel * 2), playerimg[2], 1);

			if(isdmode){
				if(power>=1000){
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
					for (int i = 0; i < 40; i++) {
						double markdeg = deg(i * 9 - 90);
						draw(px + cos(markdeg) * 30, py + sin(markdeg) * 30, 0.75, markdeg + deg(90), playerimg[3]);
					}
				}
			}else{
				if (shield > 0) {
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					for (int i = 0; i < 39 - shield / 12; i++) {
						double markdeg = deg(i * 9 - 90);
						draw(px + cos(markdeg) * 30, py + sin(markdeg) * 30, 1, markdeg + deg(90), playerimg[3]);
					}
				}
				else {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
					for (int i = 0; i < 40; i++) {
						double markdeg = deg(i * 9 - 90);
						draw(px + cos(markdeg) * 30, py + sin(markdeg) * 30, 0.75, markdeg + deg(90), playerimg[3]);
					}
				}
			}
		}

		for (int n = 0; n < 6; n++) {
			for (int i = 0; i < objmax; i++) {
				if (objgroup[i] == 1 && objgraph[i] == n) {
					double erate = 1.0;
					if (objf[i] < 4) {
						erate = 2.0 - objf[i] * 0.25;
					}
					double bangle = 0;
					switch (objgraph[i]) {
					case 0:
						bangle = 0;
						break;
					case 3:
						bangle = deg(objf[i] * 3 + gamef * 3);
						break;
					case 1:
					case 5:
						bangle = deg(objf[i] * -3 + gamef * -3);
						break;
					case 2:
					case 4:
						bangle = objangle[i];
						break;
					}
					int bcolor = (gamef - objf[i]) % 7;
					switch (objgraph[i]) {
					case 0:
						SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
						break;
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
						break;
					}
					draw(objx[i], objy[i], erate, bangle, bulletimg[objgraph[i]][bcolor]);
				}
			}
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		draw(px, py, slowpush / 5.0, deg(gamef / 8 * 45), playerimg[0]);

		if (starsc > 0) {
			SetDrawBlendMode(DX_BLENDMODE_INVDESTCOLOR, 255);
			if (starsc > 30) {
				DrawCircle((int)px, (int)py, (50 - starsc) * 30, white);
				if (starsc < 40) {
					DrawCircle((int)px, (int)py, (40 - starsc) * 60, white);
				}
			}
			else if (starsc < 20 && deathflag == 0) {
				DrawCircle((int)px, (int)py, starsc * 30, white);
				if (starsc > 10) {
					DrawCircle((int)px, (int)py, (starsc - 10) * 60, white);
				}
			}
		}

		if(star>0&&scene==8&&stage==3){
			SetDrawBlendMode(DX_BLENDMODE_INVDESTCOLOR, 255);
			DrawCircle((int)bosx,(int)bosy,50,white);
		}

		if (deathflag == 1 && gamemode != 11) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
			DrawBox(180, 220, 320, 260, GetColor(80, 0, 80), 1);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawString(200, 232, "Game Over", white);
		}

		sidewindow();
	}
	void drawbg() {
		switch (bggraph) {
		case 0:
			break;
		}
	}
	void objtask() {

		for (int i = 0; i < 3; i++) {
			if (bulletsewait[i] > 0) {
				bulletsewait[i]--;
			}
		}

		if (shake > 0) {
			shakex = rnd2(10);
			shakey = rnd2(10);
			shake--;
		}
		else {
			shakex = 0;
			shakey = 0;
		}


		for (int i = 0; i < shotmax; i++) {
			if (shottype[i] == 2) {
				shottype[i] = 0;
			}
			if (shottype[i] == 1) {
				shoty[i] -= 30;
				if (cols(shotx[i], shoty[i], bosx, bosy, 40)) {
					boshit();
					shottype[i] = 2;
				}
				for (int n = 0; n < objmax; n++) {
					if (objgroup[n] == 2 && shottype[i] == 1) {
						int enemycols = 20;
						if (objgraph[i] < 7) {
							enemycols = 30;
						}
						if (cols(shotx[i], shoty[i], objx[n], objy[n], enemycols)) {
							ehit(n);
							shottype[i] = 2;
						}
					}
				}
				if (shoty[i] < -10) {
					shottype[i] = 0;
				}
			}
		}

		bosx += bossp * cos(bosang);
		bosy += bossp * sin(bosang);

		for (int i = 0; i < objmax; i++) {
			if (objgroup[i] == 2) {
				objf[i]++;
				objx[i] += objspeed[i] * cos(objangle[i]);
				objy[i] += objspeed[i] * sin(objangle[i]);

				//enemy
				if (stage == 0) {
					switch (objtype[i]) {
					case 0:
						if (objf[i] < 20) {
							objspeed[i] -= 0.3;
						}
						else {
							objspeed[i] -= 0.05;
						}
						if (objf[i] % 35 == 0) {
							way(objx[i], objy[i], 4, degrnd2(180), 1, 5, deg(45), 0);
						}
						break;
					case 1:
						if (objf[i] < 30) {
							objspeed[i] -= 0.4;
						}
						else if (objf[i]<120) {
							objspeed[i] -= 0.05;
							way(objx[i], objy[i], 3, deg(objf[i] * 23), 3, 3, deg(28), 0);
						}
						break;
					case 2:
						if (objf[i] < 20) {
							objspeed[i] -= 0.3;
						}
						else {
							objspeed[i] -= 0.02;
						}
						if (objf[i] % 15 == 0) {
							way(objx[i], objy[i], 5, aimrnd(objx[i], objy[i], 8), 1, 5, deg(45), 0);
						}
						break;
					case 3:
						if(objf[i]<40){
							if(objf[i]%4==0){
								way(objx[i],objy[i],2.5+objf[i]/30.0,degrnd2(180),3,2,deg(1));
							}
							objspeed[i]-=0.2;
						}else if(objf[i]==40){
							objangle[i]=aim(objx[i],objy[i]);
							way(objx[i],objy[i],3.5,deg(90),1,15,deg(24));
						}else{
							objspeed[i]+=0.2;
							bullet(objx[i],objy[i],rnd(40,80)/10.0,objangle[i]-degrnd(160,200),2);
						}
						break;
					case 4:
						if(objf[i]<40){
							if(objf[i]%5==0){
								for(int n=0; n<24; n++){
								way(objx[i],objy[i],2.0+objf[i]/8.0,deg(n*15)+aim(objx[i],objy[i]),3,2,deg(2));
								}
							}
							objspeed[i]-=0.2;
						}else if(objf[i]==40){
							objangle[i]=aim(objx[i],objy[i]);
						}else{
							objspeed[i]+=0.2;
							bullet(objx[i],objy[i],rnd(40,80)/10.0,objangle[i]-degrnd(160,200),2);
						}
						break;
					}
				}

				if (stage == 1) {
					switch (objtype[i]) {
					case 0:
						objx[i] -= objf[i] / 8.0;
						objspeed[i] -= 0.08;
						if (objf[i] % 22 == 0) {
							way(objx[i], objy[i], 4, aimrnd(objx[i], objy[i], 26), 4, 3, deg(30));
						}
						break;
					case 1:
						if (objf[i] == 40) {
							objspeed[i] = 0;
						}
						if (objf[i] % 8 == 0) {
							way(objx[i], objy[i], 1, aim(objx[i],objy[i]), 2, 9, deg(26), 1);
						}
						break;
					case 2:
						if (objf[i] == 20) {
							objspeed[i] = 0;
						}
						if (objf[i] == 30) {
							objspeed[i] = 6;
							objangle[i] = degrnd(0, 45);
						}
						if (objf[i] % 15 == 0) {
							way(objx[i], objy[i], 5, deg(90), 3, 5, deg(60));
						}
						break;
					case 3:
						objspeed[i] -= 0.12;
						if (objf[i] % 20 == 0 && objf[i] > 20) {
							bullet(objx[i], objy[i], 4.5, aimrnd(objx[i], objy[i], 5), 2);
							bullet(objx[i], objy[i], 4, degrnd2(180), 3);
						}
						break;
					case 4:
						objspeed[i] -= 0.1;
						if (objf[i] < 120) {
							way(objx[i], objy[i], 8, deg(225 - objf[i]), 4, 6, deg(1));
							way(objx[i], objy[i], 8, deg(-45 + objf[i]), 4, 6, deg(1));
						}
						break;
					case 5:
						objx[i] += objf[i] / 6.0;
						if (objf[i] % 10 == 0) {
							way(objx[i], objy[i], 3.5 + objf[i] / 60.0, degrnd(0, 180), 5, 3, deg(1));
							way(objx[i], objy[i], 4, degrnd(-180, 0), 5, 3, deg(1));
						}
						break;
					case 6:
						objx[i] -= objf[i] / 6.0;
						if (objf[i] % 10 == 0) {
							way(objx[i], objy[i], 3.5 + objf[i] / 60.0, degrnd(0, 180), 5, 3, deg(1));
							way(objx[i], objy[i], 4, degrnd(-180, 0), 5, 3, deg(1));
						}
						break;
					case 7:
						if (objf[i] == 60) {
							objspeed[i] = 0;
						}
						if (objf[i] > 100) {
							bullet(objx[i], objy[i], rnd(2, 12), degrnd(88, 92), 0);
						}
						if (objf[i] > 130)objspeed[i] -= 0.1;
						if (objf[i] == 100) {
							for (int n = 0; n < 12; n++) {
								way(objx[i], objy[i], (n + 3.0) / 3.0, degrnd2(180), 1, 10, deg(36));
							}
						}
						break;
					case 8:
						if (objf[i] < 30) {
							objspeed[i] -= 0.1;
						}
						else {
							objspeed[i] += 0.1;
							if (objf[i] % 5 == 0) {
								bullet(objx[i] + rnd2(20), objy[i] + rnd2(20), 4, aim(objx[i], objy[i]), 2);
							}
						}
						break;
					case 9:
						objx[i] += objf[i] / 8.0;
						if (gamef % 10 == 0) {
							way(objx[i], objy[i], 1.6, aim(objx[i], objy[i]), 2, 4, deg(3), 1);
							way(objx[i], objy[i], 2.0, aim(objx[i], objy[i]), 2, 4, deg(2), 1);
							way(objx[i], objy[i], 2.4, aim(objx[i], objy[i]), 2, 4, deg(1), 1);
						}
						break;
					case 10:
						objx[i] -= objf[i] / 8.0;
						if (gamef % 10 == 0) {
							way(objx[i], objy[i], 1.6, aim(objx[i], objy[i]), 2, 4, deg(3), 1);
							way(objx[i], objy[i], 2.0, aim(objx[i], objy[i]), 2, 4, deg(2), 1);
							way(objx[i], objy[i], 2.4, aim(objx[i], objy[i]), 2, 4, deg(1), 1);
						}
						break;
					case 11:
						objx[i] += objf[i] / 8.0;
						if (objf[i] % 4 == 0) {
							way(objx[i], objy[i], 3.5, degrnd(0, 180), 4, 5, deg(1));
							way(objx[i], objy[i], 3.5, degrnd(180, 360), 4, 5, deg(1));
						}
						break;
					case 12:
						objx[i] -= objf[i] / 8.0;
						if (objf[i] % 4 == 0) {
							way(objx[i], objy[i], 3.5, degrnd(0, 180), 4, 5, deg(1));
							way(objx[i], objy[i], 3.5, degrnd(180, 360), 4, 5, deg(1));
						}
						break;
					}
				}

				if (stage == 2) {
					switch (objtype[i]) {
					case 0:
						if (objf[i] >= 30 && objf[i] < 40) {
							objangle[i] += deg(9);
						}
						if (objf[i] % 2 == 0 && objf[i] > 20) {
							for (int n = 0; n < 30; n++) {
								way(objx[i], objy[i], objf[i] / 4.0, aim(objx[i], objy[i]) + deg(n * 12), 5, 3, deg(1));
							}
						}
						break;
					case 1:
						if (objf[i] >= 30 && objf[i] < 40) {
							objangle[i] -= deg(9);
						}
						if (objf[i] % 2 == 0 && objf[i] > 20) {
							for (int n = 0; n < 30; n++) {
								way(objx[i], objy[i], objf[i] / 4.0, aim(objx[i], objy[i]) + deg(n * 12), 5, 3, deg(1));
							}
						}
						break;
					case 2:
						if(objf[i]%3==0){
						for(int n=0;n<4;n++){
						double dds=degrnd2(180);
						int ssd=rnd(80,100);
						way(objx[i], objy[i], (ssd+objf[i]) / 20.0, dds, 5, 3, deg(1));
						way(objx[i], objy[i], (ssd+1+objf[i]) / 20.0, dds, 5, 3, deg(1));
						}
						}
						break;
					case 3:
						for(int n=0;n<3;n++){
							bullet(objx[i],objy[i],9.0+n/6.0,deg(90),1);
						}
						if(scenef%10==0){
							for(int n=0;n<3;n++){
							way(objx[i],objy[i],12.0+n/5.0,aim(objx[i],objy[i]),1,7,deg(45));
							}
						}
						break;
					case 4:
						if(objf[i]%3==0){
						for(int n=0;n<2;n++){
						double dds=degrnd2(180);
						int ssd=rnd(80,100);
						way(objx[i], objy[i], (ssd+objf[i]) / 20.0, dds, 5, 3, deg(1));
						way(objx[i], objy[i], (ssd+1+objf[i]) / 20.0, dds, 5, 3, deg(1));
						}
						}
						break;
					case 5:
						if(objf[i]==10)objspeed[i]=0;
						if(objf[i]%190==1){
							for(int n=0;n<16;n++){
								//way(objx[i],objy[i],5.0+n/20.0,deg(90),5,n+1,degd(0.5));
								bullet(objx[i]+n,objy[i],5.0+n/60.0,deg(90),5);
								bullet(objx[i]-n,objy[i],5.0+n/60.0,deg(90),5);
							}
							way(objx[i],objy[i],5.0,aim(objx[i],objy[i]),5,2,deg(1));
							bullet(objx[i],objy[i],5.07,aim(objx[i],objy[i]),5);
						}
						break;
					case 6:
						break;
					case 7:
						break;
					case 8:
						objspeed[i] -= 0.1;
						if (objf[i] < 110) {
							way(objx[i], objy[i], 12, deg(210 - objf[i]), 1, 9, deg(1));
							way(objx[i], objy[i], 12, deg(-30 + objf[i]), 1, 9, deg(1));
							way(objx[i], objy[i], 11, deg(210 - objf[i]), 1, 9, deg(1));
							way(objx[i], objy[i], 11, deg(-30 + objf[i]), 1, 9, deg(1));
						}
						break;
					case 9:
						if(objf[i]%2==0&&objf[i]<20){
							bullet(objx[i],objy[i],(500-objy[i])/100.0,degrnd2(180),3);
						}
						break;
					}
				}

				if (stage == 3) {
					switch (objtype[i]) {
					case 0:
						if(objf[i]<61){
							objspeed[i]-=0.1;
						}else if(objf[i]%2==0){
							if(objf[i]<420){
								for(int n=0;n<360;n+=30){
									bullet(objx[i]+cos(deg(gamef+n))*(420-objf[i])/4,objy[i]+sin(deg(gamef+n))*(420-objf[i])/8,rnd(30,50)/10.0,deg(gamef+n+rnd2(objf[i]/28)),1);
								}
							}else{
								for(int n=0;n<360;n+=30){
									bullet(objx[i],objy[i],rnd(30,60)/10.0,deg(gamef+n+rnd2(15)),1);
								}
							}
						}
						if(cols(objx[i],objy[i],px,py,20)){
							bullet(px,py,0,0,0);
						}
						break;
					}
				}


				if (objf[i] > 60 && outwindow(objx[i], objy[i], 24)) {
					objgroup[i] = 0;
				}
			}
		}

		for (int i = 0; i < objmax; i++) {
			if (objgroup[i] == 3) {
				objf[i]++;
				objx[i] += objspeed[i] * cos(objangle[i]);
				objy[i] += objspeed[i] * sin(objangle[i]);

				//mahou
				if(stage==0){
					switch(objtype[i]){
					case 0:
						if(objf[i]%5==0){
							way(objx[i],objy[i],rnd(30,50)/10.0,objangle[i]-degrnd(120,240),1,5,deg(2));
						}
						if(objf[i]==60)objhp[i]=0;
						break;
					case 1:
						objspeed[i]-=0.3;
						if(objf[i]==30){
							way(objx[i],objy[i],8,aim(objx[i],objy[i]),0,5,deg(72),99,30);
							for(int n=0;n<12;n++){
							way(objx[i],objy[i],5,deg(scenef+n*30),2,2,deg(2),2,30);
							way(objx[i]+rnd2(20),objy[i]+rnd2(20),rnd(50,80)/10.0,aim(objx[i],objy[i]),2,5,deg(72),99,30);
							}
							objhp[i]=0;
						}
						break;
					case 2:
						bullet(objx[i],objy[i],0,0,3,4);
						if(objf[i]<100){
						if(objx[i]<20||objx[i]>480){
							objangle[i]=deg(180)-objangle[i];
							for(int n=0;n<10;n++){
								bullet(objx[i],objy[i],rnd(45,50)/10.0,degrnd2(180),4,5);
							}
						}
						if(objy[i]<10||objy[i]>470){
							objangle[i]=-objangle[i];
							for(int n=0;n<10;n++){
								bullet(objx[i],objy[i],rnd(45,50)/10.0,degrnd2(180),4,5);
							}
						}
						}
						break;
					case 3:
						objangle[i]+=deg(6);
					case 4:
						if(objtype[i]==4){
							objangle[i]-=deg(6);
						}
						objx[i]=bosx+cos(objangle[i])*objspeed[i];
						objy[i]=bosy+sin(objangle[i])*objspeed[i];
						way(objx[i],objy[i],6,objangle[i],2,8,deg(3));
						if(objf[i]<20){
							objspeed[i]-=14;
						}else if(objf[i]>80){
							objspeed[i]+=14;
						}
						if(objf[i]%20==0){
							bullet(objx[i],objy[i],3,objangle[i]+deg(180),0,120,100);
						}
						if(objf[i]==99)objhp[i]=0;
						break;
					}
				}

				if (stage == 1) {
					switch (objtype[i]) {
					case 0:
						objangle[i] += deg(3);
						if (objf[i] < 30) {
							objx[i] = bosx + cos(objangle[i]) * objf[i] * 4;
							objy[i] = bosy + sin(objangle[i]) * objf[i] * 4;
						}
						else {
							objx[i] = bosx + cos(objangle[i]) * 120;
							objy[i] = bosy + sin(objangle[i]) * 120;
						}
						if (stp(8, 11, 1, 12)) {
							bullet(objx[i], objy[i], 0, deg(90), 4, 3);
							bullet(objx[i], objy[i], 0, degrnd2(180), 3, 2);
						}
						if (objf[i] == 150) {
							way(objx[i], objy[i], 2, degrnd2(180), 1, 30, deg(12), 3);
							objhp[i] = 0;
						}
						break;
					case 1:
						if (objspeed[i] > 0) {
							objspeed[i] -= 0.2;
						}
						if (objf[i] == 40) {
							way(objx[i], objy[i], 0, degrnd2(180), 4, 24, deg(15), 4);
							objhp[i] = 0;
						}
						break;
					case 2:
						bullet(objx[i], objy[i], 3.75, objangle[i] + degrnd(160, 200), 2);
						if (objf[i] % 3 == 0) {
							bullet(objx[i], objy[i], 0, degrnd2(180), 3, 4);
						}
						break;
					case 3:
						objangle[i] += degd(2.5);
					case 4:
						if (objtype[i] == 4)objangle[i] -= degd(2.5);
						objy[i] += 4;
						way(objx[i], objy[i], rnd(12, 24), objangle[i], 1, 2, deg(180));
						way(objx[i], objy[i], rnd(12, 36), objangle[i], 1, 2, deg(180));
						break;
					}
				}

				if (stage == 2) {
					switch (objtype[i]) {
					case 0:
						if (outwindow(objx[i], objy[i], 32)) {
							for (int n = 0; n < 30; n++) {
							way(objx[i] + rnd2(10), objy[i] + rnd2(10), rnd(40, 50) / 10.0, deg(45), 3, 4, deg(90));
							}
							for (int n = 0; n < 8; n++) {
							way(objx[i] + rnd2(15), objy[i] + rnd2(15), rnd(40, 50) / 10.0, deg(45), 1, 4, deg(90));
							}
							way(objx[i], objy[i], 5, deg(45), 0, 4, deg(90));
							objhp[i] = 0;
						}
						break;
					case 1:
						if(objspeed[i]>0.5){
							objspeed[i]-=0.15;
						}
						if(objf[i]%6==0){
							double dds=degrnd2(180);
							for(int n=0;n<3;n++){
								way(objx[i],objy[i],4.5+(n/20.0),dds,4,7,degd(0.5),0);
							}
						}
						if(objf[i]==80)objhp[i]=0;
						break;
					case 2:
						if(gamef%12==0){
							way(objx[i],objy[i],2.75,objangle[i],1,2,deg(180));
						}
						break;
					case 3:
						objangle[i]+=deg(1);
						if(objf[i]>80)objhp[i]=0;
						for(int n=0;n<4;n++){
						way(objx[i],objy[i]+rnd2(10),rnd(18,24),objangle[i],5,7,deg(1));
						way(objx[i],objy[i]+rnd2(10),rnd(18,30),objangle[i],5,7,deg(1));
						}
						break;
					case 4:
						objangle[i]-=deg(1);
						if(objf[i]>80)objhp[i]=0;
						for(int n=0;n<4;n++){
						way(objx[i],objy[i]+rnd2(10),rnd(18,24),objangle[i],5,7,deg(1));
						way(objx[i],objy[i]+rnd2(10),rnd(18,30),objangle[i],5,7,deg(1));
						}
						break;
					case 5:
						if(objf[i]<30){
							objspeed[i]-=0.4;
						}else if(objf[i]==30){
							objangle[i]=aim(objx[i],objy[i]);
							for(int n=0;n<3;n++){
								bullet(objx[i],objy[i],8+n,aim(objx[i],objy[i]),0,100,100);
							}
						}else{
							objspeed[i]-=0.1;
							int aaa=360-objf[i]*6;
							way(objx[i],objy[i],10,objangle[i]+deg(aaa),2,9,deg(1),100,100);
							way(objx[i],objy[i],10,objangle[i]-deg(aaa),2,9,deg(1),100,100);
							way(objx[i],objy[i],11,objangle[i]+deg(aaa),2,9,deg(1),100,100);
							way(objx[i],objy[i],11,objangle[i]-deg(aaa),2,9,deg(1),100,100);
						}
						if(objf[i]==58)objhp[i]=0;
						break;
					}
				}

				if(stage==3){
					switch(objtype[i]){
					case 0:
						if(objf[i]==1){
							way(objx[i],objy[i],8,degrnd2(180),2,30,deg(12),1);
						}
						if(scene==1){
							way(objx[i],objy[i],9,degrnd2(180),2,12,deg(30),0,80);
							objhp[i]=0;
						}
						break;
					case 1:
						if(outwindow(objx[i],objy[i],-20)){
							int dd=rnd2(180);
							for(int n=0;n<360;n+=72){
								way(objx[i],objy[i],0.8+objf[i]/12.0,deg(n+dd),5,3,deg(1));
							}
						}
						if(outwindow(objx[i],objy[i],0)){
							objhp[i]=0;
						}
						break;
					case 2:
						if(objf[i]==1){
							way(objx[i],objy[i],8,degrnd2(180),2,15,deg(24),1);
						}
						if(scene==1){
							way(objx[i],objy[i],3,degrnd2(180),2,15,deg(24),2,80);
							objhp[i]=0;
						}
						break;
					case 3:
						if(outwindow(objx[i],objy[i],0)){
							int dd=rnd2(180);
							for(int n=0;n<360;n+=120){
								way(objx[i],objy[i],5.0-objf[i]/11.0,deg(n+dd),5,3,deg(1));
							}
						}
						if(outwindow(objx[i],objy[i],20)){
							objhp[i]=0;
						}
						break;
					case 4:
						objy[i]+=objf[i]/6.0;
						objspeed[i]+=0.01;
						if(objf[i]%5==0){
							way(objx[i],objy[i],3,0,4,2,deg(180));
						}
						if(objf[i]==60){
							objhp[i]=0;
						}
						break;
					case 5:
						objangle[i]+=deg(2);
						objx[i]=bosx+cos(objangle[i])*160;
						objy[i]=bosy+sin(objangle[i])*40;
						if(objf[i]%2==0)bullet(objx[i],objy[i],rnd(28,32)/10.0,objangle[i]+degrnd2(2),1);
						break;
					case 6:
						objangle[i]-=deg(2);
						objx[i]=bosx+cos(objangle[i])*160;
						objy[i]=bosy+sin(objangle[i])*40;
						if(objf[i]%2==0)bullet(objx[i],objy[i],rnd(28,32)/10.0,objangle[i]+degrnd2(2),1);
						break;
					case 7:
						if(objspeed[i]>0.1){
							objspeed[i]-=0.125;
							objangle[i]-=degd(0.8);
						}else{
							objtype[i]=9;
							if(py>bosy){
								objangle[i]+=deg(90);
							}else{
								objangle[i]-=deg(90);
							}
						}
						break;
					case 8:
						if(objspeed[i]>0.1){
							objspeed[i]-=0.125;
							objangle[i]+=degd(0.8);
						}else{
							objtype[i]=9;
							if(py>bosy){
								objangle[i]-=deg(90);
							}else{
								objangle[i]+=deg(90);
							}
						}
						break;
					case 9:
						objspeed[i]+=0.1+(700-boshp)/7000.0;
						bullet(objx[i],objy[i],-1,objangle[i],3,6);
						break;
					}
				}


				if (outwindow(objx[i], objy[i], 20)) {
					objhp[i]--;
				}
				if (objhp[i] <= 0) {
					objgroup[i] = 0;
				}
			}
		}

		bulletcount = 0;
		for (int i = 0; i < objmax; i++) {
			if (objgroup[i] == 1) {
				objf[i]++;
				objx[i] += objspeed[i] * cos(objangle[i]);
				objy[i] += objspeed[i] * sin(objangle[i]);

				bulletcount++;

				//bullet
				if (stage == 0) {
					switch (objtype[i]) {
					case 0:
						if (objf[i] < 30) {
							objspeed[i] += 0.05;
						}
						break;
					case 1:
						if (objf[i] < 10) {
							objspeed[i] -= 3.2;
						}
						break;
					case 2:
						if(objf[i]<20){
							objspeed[i]-=0.1;
						}
						break;
					case 3:
						if(outwindow(objx[i],objy[i],10)){
							objangle[i]=degrnd2(180);
							objtype[i]=125;
							objspeed[i]=1.0+objspeed[i]/2.0;
							if(objy[i]>480)objhp[i]=0;
						}
						break;
					case 4:
						if(objf[i]>15)objhp[i]=0;
						break;
					case 5:
						if(objf[i]<9)objspeed[i]-=0.5;
						break;
					}
				}

				if (stage == 1) {
					switch (objtype[i]) {
					case 0:
						objspeed[i] += 0.05;
						if (objf[i] < 8) {
							objspeed[i] -= 0.8;
						}
						else if (objf[i] == 8) {
							objspeed[i] = 0.5;
						}
						break;
					case 1:
						objspeed[i] += 0.07;
						break;
					case 2:
						if (objspeed[i] < 3) {
							objspeed[i] += 0.05;
						}
						break;
					case 3:
						if (objspeed[i] < 4) {
							objspeed[i] += 0.1;
						}
						break;
					case 4:
						if (objspeed[i] < 3) {
							objspeed[i] += 0.015;
						}
						break;
					case 5:
						if (objf[i] < 40) {
							objspeed[i] -= 0.15;
						}
						else {
							objspeed[i] = 1.5;
							if (scenef % 2 == 0) {
								objangle[i] += deg(30);
							}
							else {
								objangle[i] -= deg(30);
							}
						}
						break;
					case 6:
						if (objf[i] > 45)objhp[i] = 0;
						break;
					case 7:
						objy[i] += objf[i] / 8.0;
						break;
					case 8:
						if (objf[i] == 20) {
							objangle[i]+=deg(30);
						}
						break;
					case 9:
						if (objf[i] == 20) {
							objangle[i]-=deg(30);
						}
						break;
					case 10:
						if (objf[i] == 20) {
							if(scenef%2==0){
								objangle[i]+=deg(15);
							}else{
								objangle[i]-=deg(15);
							}
						}
						break;
					}
				}

				if(stage==2){
					switch(objtype[i]){
					case 0:
						if(objx[i]<20||objx[i]>480){
							objangle[i]=deg(180)-objangle[i];
							objtype[i]=125;
						}
						if(objy[i]<10||objy[i]>470){
							objangle[i]=-objangle[i];
							objtype[i]=125;
						}
						if(objy[i]>470)objhp[i]=0;
						break;
					case 1:
						if(objf[i]<60){
							objspeed[i]-=0.1;
						}else if(objf[i]==60){
							objgraph[i]=0;
							objspeed[i]=rnd(60,80)/10.0;
							objangle[i]=degrnd(75,105);
						}
						break;
					case 2:
						objangle[i]+=deg(1);
						break;
					case 3:
						objangle[i]-=deg(1);
						break;
					case 4:
						if(objf[i]<30){
							objspeed[i]-=0.6;
						}else if(objf[i]==30){
							objangle[i]=aim(objx[i],objy[i]);
						}else{
							objspeed[i]+=0.6;
						}
						break;
					case 5:
						objspeed[i]+=0.2;
						break;
					}
				}

				if(stage==3){
					switch(objtype[i]){
					case 0:
						if(objf[i]<60){
							objspeed[i]-=0.3;
						}else{
							objspeed[i]=3.5;
						}
						break;
					case 1:
						if(objspeed[i]>3){
							objspeed[i]-=0.15;
						}
						break;
					case 2:
						if(objf[i]<120){
							objspeed[i]-=0.05;
						}else{
							objspeed[i]=4;
						}
						break;
					case 3:
						if(gamef>3240){
							objspeed[i]=12;
						}else if(gamef>3210){
							objangle[i]=aim(objx[i],objy[i])+deg(180);
							objspeed[i]+=0.12;
						}
						break;
					case 4:
						if(objspeed[i]>3)objspeed[i]-=0.1;
						break;
					case 5:
						objspeed[i]=4.0+(700-boshp)/300.0;
						break;
					case 6:
						if(objf[i]>10)objhp[i]=0;
						break;
					}
				}


				int bulletcols = 0;
				switch (objgraph[i]) {
				case 0:
					bulletcols = 12;
					break;
				case 1:
					bulletcols = 3;
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					bulletcols = 2;
					break;
				}
				if (cols(px, py, objx[i], objy[i], bulletcols)) {
					phit();
					objhp[i] = 0;
				}

				if (outwindow(objx[i], objy[i], 32)) {
					objhp[i]--;
				}
				if (objhp[i] <= 0) {
					objgroup[i] = 0;
				}
			}
		}

		for (int i = 0; i < objmax; i++) {
			if (objgroup[i] == 4) {
				objf[i]++;
				objx[i] += objspeed[i] * cos(objangle[i]);
				objy[i] += objspeed[i] * sin(objangle[i]);

				switch (objtype[i]) {
				case 0:
					break;
				case 1:
					if (objspeed[i] > 0) {
						objspeed[i] -= 0.02;
					}
					if (objf[i] == 50) {
						objhp[i] = 0;
					}
					break;
				case 2:
					if (objspeed[i] > 0) {
						objspeed[i] -= 0.15;
					}
					if (objf[i] == 40) {
						objhp[i] = 0;
					}
					break;
				}

				if (outwindow(objx[i], objy[i], 32)) {
					objhp[i]--;
				}
				if (objhp[i] <= 0) {
					objgroup[i] = 0;
				}
			}
		}
		
	}
	void startgame() {
		gamemode = 10;
		px = 250;
		py = 420;
		star = 0;
		starsc = 0;
		shield = 0;
		autoshot = 0;
		slowpush = 0;
		pangler = 0;
		panglel = 0;
		bosx = 250;
		bosy = -80;
		bossp = 0;
		bosang = 0;
		bosgraph = 0;
		bostime = 10;
		boshp = 10;
		bosflag = 0;
		bosbonus = 0;
		bonusdrawf = 0;
		score = 0;
		miss = 0;
		kill = 0;
		shake = 0;
		shakex = 0;
		shakey = 0;
		power = 0;
		gamef = 0;
		scenef = 0;
		scene = 0;
		deathwaitf = 0;
		deathflag = 0;
		pausecount = 0;
		//randnum = 0x5c396a7e;
		if(replaymode){
			randnum=rseed;
			playsaved=1;
		}else{
			rseed=randnum;
			playsaved=-1;
		}
		for (int i = 0; i < objmax; i++) {
			objgroup[i] = 0;
		}
		for (int i = 0; i < shotmax; i++) {
			shottype[i] = 0;
		}
		if (replaymode == 0) {
			playcount[stage]++;
		}
	}
	int getkey(char keytype) {
		char shiftcount = keytype - 1;
		char inputbit = 0b00000001;
		inputbit <<= shiftcount;
		if (inputbit & nowkey) {
			return 1;
		}
		else {
			return 0;
		}
	}
	void sidewindow() {

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		int sidecolor = 0;
		if (deathflag == 0) {
			if (shield == 0) {
				sidecolor = GetColor(70, 60, 120);
			}
			else {
				sidecolor = GetColor(100, 30, 100);
			}
			if(isdmode&&power>=1000){
				int ddd=gamef*12;
				int rr=64+sin(deg(ddd))*8;
				int gg=64+sin(deg(ddd+120))*8;
				int bb=64+sin(deg(ddd+240))*8;
				sidecolor=GetColor(rr,gg,bb);
			}
		}
		else {
			sidecolor = GetColor(50, 0, 20);
		}

		DrawBox(0, 0, 640, 10, sidecolor, 1);
		DrawBox(0, 470, 640, 480, sidecolor, 1);

		if (bosflag) {
			DrawBox(bosx - 20, 472, bosx + 20, 480, GetColor(156, 34, 34), 1);
			DrawBox(bosx - 20, 472, bosx + 20, 490, GetColor(36, 4, 4), 0);
		}

		DrawBox(0, 0, 20, 480, sidecolor, 1);
		DrawBox(480, 0, 640, 480, sidecolor, 1);

		if (bosflag) {
			DrawBox(4, 470 - (460 * boshp / boshpmax), 16, 470, GetColor(246, 243, 25), 1);
			DrawBox(4, 10, 16, 470, GetColor(35, 36, 2), 0);
			for (int i = 0; i < 1 + bostime / 60; i++) {
				draw(560 + cos(deg(i * 6 - 90)) * 60, 280 + sin(deg(i * 6 - 90)) * 60, 1.25, deg(i * 6), playerimg[3]);
			}
			DrawFormatString(532, 240, white, "%.2f", bostime / 60.0);
			DrawString(515, 262, "BONUS", white);
			DrawFormatString(504, 282, white, "+%d", bostime * kill * 10);
		}

		if (bonusdrawf > 0) {
			bonusdrawf--;
			DrawFormatString(40, 450, white, "BONUS +%d", bosbonus);
		}

		if (gamemode >= 9) {
			DrawFormatString(525, 8, white, "STAGE-%d", stage + 1);
		}

		DrawString(497, 30, "Score", white);
		DrawLine(500, 67, 630, 67, white);
		DrawLine(500, 68, 630, 68, GetColor(0, 0, 0));
		int scoredigit = score;
		if (scoredigit > 0) {
			scoredigit = (int)log10(score);
		}
		DrawFormatString(620 - (scoredigit * 11), 50, white, "%d", score);


		DrawFormatString(500, 90, white, "Miss %d", miss);
		DrawFormatString(500, 115, white, "Kill %d", kill);
		DrawFormatString(500, 165, white, "Bullet %d", bulletcount);
		if(isdmode){
			int moziclr;
			if(power>=1000){
				int ddd=rnd(0,360);
				int rr=191+sin(deg(ddd))*64;
				int gg=191+sin(deg(ddd+120))*64;
				int bb=191+sin(deg(ddd+240))*64;
				moziclr=GetColor(rr,gg,bb);
			}else{
				moziclr=white;
			}
			DrawFormatString(500,380,moziclr,"POWER %d",power);
		}

		if(getkey(1)){
			DrawBox(500,450,508,468,GetColor(127,196,144),1);
		}
		if(getkey(2)){
			DrawBox(490,450,498,468,GetColor(127,196,144),1);
		}
		if(getkey(3)){
			DrawBox(520,450,528,458,GetColor(127,196,144),1);
		}
		if(getkey(4)){
			DrawBox(520,460,528,468,GetColor(127,196,144),1);
		}
		if(getkey(5)){
			DrawBox(510,460,518,468,GetColor(127,196,144),1);
		}
		if(getkey(6)){
			DrawBox(530,460,538,468,GetColor(127,196,144),1);
		}
	}
	void titletask() {
		titleback();

		DrawBox(50, 178 + (menubar * 20), 240, 198 + (menubar * 20), GetColor(45, 25, 53), 1);
		DrawBox(50, 178 + (menubar * 20), 240, 198 + (menubar * 20), GetColor(145, 125, 153), 0);
		DrawString(60, 180, "Start", white);
		DrawString(60, 200, "Replay", white);
		DrawString(60, 220, "Records", white);
		DrawString(60, 240, "Music", white);
		DrawString(60, 260, "Option", white);
		DrawString(60, 280, "Exit", white);

		if(!isdwait&&CheckHitKey(KEY_INPUT_I)){
			if(isdmode){
				isdmode=0;
			}else{
				isdmode=1;
			}
			isdwait=1;
			playse(enemyse[1]);
		}else if(!CheckHitKey(KEY_INPUT_I)){
			isdwait=0;
		}

		barmovetask(5);
		if (backtask()) {
			menubar = 5;
		}
		if (clicktask()) {
			switch (menubar) {
			case 0:
				gamemode = 2;
				break;
			case 1:
				gamemode = 3;
				replaylineread();
				break;
			case 2:
				gamemode = 4;
				break;
			case 3:
				gamemode = 5;
				break;
			case 4:
				gamemode = 6;
				break;
			case 5:
				gamemode = 0;
				break;
			}
			menubar = 0;
		}

	}
	void stageselect() {
		titleback();
		DrawString(60, 130, "Stage Select", white);

		DrawBox(50, 178 + (menubar * 20), 240, 198 + (menubar * 20), GetColor(45, 25, 53), 1);
		DrawBox(50, 178 + (menubar * 20), 240, 198 + (menubar * 20), GetColor(145, 125, 153), 0);

		char playable = 0;
		for (int i = 0; i < 4; i++) {
			if (clearcount[i] > 0) {
				playable++;
			}
			if (playable >= i) {
				DrawFormatString(60, 180 + (20 * i), white, "Stage %d", i + 1);
			}
			else {
				DrawFormatString(60, 180 + (20 * i), GetColor(127, 127, 127), "Stage %d", i + 1);
			}
		}
		if (playable > 3) {
			playable = 3;
		}

		barmovetask(playable);
		if (backtask()) {
			gamemode = 1;
			menubar = 0;
		}
		if (clicktask()) {
			stage = menubar;
			menubar = 0;
			replaymode = 0;
			startgame();
		}
	}
	void replaylineread(){
		for(int i=0;i<30;i++){
			rpline[i]=replayread(i);
		}
	}
	void replaylineup() {
		titleback();
		DrawBox(30, 98 + (menubar * 20), 540, 118 + (menubar * 20), GetColor(45, 25, 53), 1);
		DrawBox(30, 98 + (menubar * 20), 540, 118 + (menubar * 20), GetColor(145, 125, 153), 0);
		DrawString(40,70,"STAGE",white);
		DrawString(320,70,"SCORE",white);
		DrawString(420,70,"Miss",white);
		DrawString(470,70,"FPSavg",white);
		for(int i=0;i<16;i++){
			DrawFormatString(10, 100 + (20 * i), white, "%d", i);
			if(rpline[i].ver!=0){
				DrawFormatString(40, 100 + (20 * i), white, "St-%d", rpline[i].stage+1);
				DrawFormatString(100, 100 + (20 * i), white, "%4d/%2d/%2d", rpline[i].date.Year,rpline[i].date.Mon,rpline[i].date.Day);
				DrawFormatString(220, 100 + (20 * i), white, "%2d:%2d:%2d", rpline[i].date.Hour,rpline[i].date.Min,rpline[i].date.Sec);
				DrawFormatString(320, 100 + (20 * i), white, "%d", rpline[i].score);
				DrawFormatString(430, 100 + (20 * i), white, "%d", rpline[i].miss);
				DrawFormatString(470, 100 + (20 * i), white, "%.2f", rpline[i].avgfps);
				if(rpline[i].isd){
					int ddd=nrnd(0,360);
					int rr=191+sin(deg(ddd))*64;
					int gg=191+sin(deg(ddd+120))*64;
					int bb=191+sin(deg(ddd+240))*64;
					DrawString(540,100+(20*i),"ISD MODE",GetColor(rr,gg,bb));
				}
			}
		}

		barmovetask(15);
		if (backtask()) {
			gamemode = 1;
			menubar = 1;
		}
		if(clicktask()){
			if(rpline[menubar].ver>0){
				gamemode=10;
				replaymode=1;
				replayreadall(menubar);
				rpfast=rpline[menubar].fastsp;
				rpslow=rpline[menubar].slowsp;
				isdmode=rpline[menubar].isd;
				rseed=rpline[menubar].rseed;
				stage=rpline[menubar].stage;
				menubar=0;
				startgame();
			}
		}
	}
	void watchrecord() {
		titleback();
		DrawString(60, 130, "Play Record", white);

		DrawString(30,200,"HISCORE",white);
		DrawString(30,220,"LowMiss",white);
		DrawString(30,240,"PlayCount",white);
		DrawString(30,260,"ClearCount",white);
		DrawString(30,280,"PlayTime",white);
		for(int i=0;i<4;i++){
			DrawFormatString(160+i*120,170,white,"STAGE-%d",i+1);
			DrawFormatString(160+i*120,200,white,"%d",hiscore[i]);
			DrawFormatString(160+i*120,220,white,"%d",lowmiss[i]);
			DrawFormatString(160+i*120,240,white,"%d",playcount[i]);
			DrawFormatString(160+i*120,260,white,"%d",clearcount[i]);
			DrawFormatString(160+i*120,280,white,"%d:%2d:%2d",playtime[i]/216000,(playtime[i]%216000)/3600,(playtime[i]%3600)/60);
		}

		DrawFormatString(30,400,white,"Total UpTime %d:%2d:%2d",windowtime/216000,(windowtime%216000)/3600,(windowtime%3600)/60);
		DrawFormatString(30,420,white,"Total Miss %d",allmiss);
		DrawFormatString(30,440,white,"Total Kill %d",allkill);

		if (backtask()) {
			gamemode = 1;
			menubar = 2;
		}
	}
	void musicroomtask() {
		titleback();
		DrawString(60, 130, "Music Room", white);

		DrawBox(50, 178 + (menubar * 20), 400, 198 + (menubar * 20), GetColor(45, 25, 53), 1);
		DrawBox(50, 178 + (menubar * 20), 400, 198 + (menubar * 20), GetColor(145, 125, 153), 0);

		DrawString(60, 180, "TITLE    Phantom", white);
		DrawString(60, 200, "STAGE-1  Initiadigm/Shift", white);
		DrawString(60, 220, "STAGE-2  BreakDown", white);
		DrawString(60, 240, "STAGE-3  Scenario 3rd", white);
		DrawString(60, 260, "STAGE-4  End of IT.", white);

		barmovetask(4);
		if(clicktask()){
			setbgm(menubar);
		}
		if (backtask()) {
			gamemode = 1;
			menubar = 3;
		}
	}
	void titleback(){
		resumebgm();
		DrawBox(0, 0, 640, 480, GetColor(17, 0, 19), 1);
		DrawString(40, 40, "Phantom Bullet Dimension.", white);
	}
	void settingtask() {
		titleback();
		DrawString(60, 130, "Option", white);

		DrawBox(50, 178 + (menubar * 20), 370, 198 + (menubar * 20), GetColor(45, 25, 53), 1);
		DrawBox(50, 178 + (menubar * 20), 370, 198 + (menubar * 20), GetColor(145, 125, 153), 0);
		DrawString(60, 180, "Music Volume", white);
		DrawString(60, 200, "SE Volume", white);
		DrawString(60, 220, "Move Speed Fast", white);
		DrawString(60, 240, "Move Speed Slow", white);
		DrawFormatString(255, 180, white, "%d", musicvol);
		DrawFormatString(255, 200, white, "%d", sevol);
		DrawFormatString(255, 220, white, "%d", fastspeed);
		DrawFormatString(255, 240, white, "%d", slowspeed);
		DrawString(249, 260, "Save", white);

		barmovetask(4);
		if (menubar < 4) {
			DrawString(239, 180 + (menubar * 20), "<    >", white);
		}
		if (backtask()) {
			gamemode = 1;
			menubar = 4;
			configread();
			setsoundvol();
		}
		if (clicktask()) {
			if (menubar == 4) {
				gamemode = 1;
				menubar = 4;
				configwrite();
				setsoundvol();
			}
		}
		if (getkey(5) && barmove == 0) {
			switch (menubar) {
			case 0:
				musicvol -= 5;
				if (musicvol < 0) {
					musicvol = 0;
				}
				setsoundvol();
				break;
			case 1:
				sevol -= 5;
				if (sevol < 0) {
					sevol = 0;
				}
				setsoundvol();
				break;
			case 2:
				fastspeed--;
				if (fastspeed < 4) {
					fastspeed = 4;
				}
				break;
			case 3:
				slowspeed--;
				if (slowspeed < 4) {
					slowspeed = 4;
				}
				break;
			}
			if (menubar < 4) {
				barmove = 1;
				playse(menuse[2]);
			}
		}
		if (getkey(6) && barmove == 0) {
			switch (menubar) {
			case 0:
				musicvol += 5;
				if (musicvol > 100) {
					musicvol = 100;
				}
				setsoundvol();
				break;
			case 1:
				sevol += 5;
				if (sevol > 100) {
					sevol = 100;
				}
				setsoundvol();
				break;
			case 2:
				fastspeed++;
				if (fastspeed > 40) {
					fastspeed = 40;
				}
				break;
			case 3:
				slowspeed++;
				if (slowspeed > 40) {
					slowspeed = 40;
				}
				break;
			}
			if (menubar < 4) {
				barmove = 1;
				playse(menuse[2]);
			}
		}
	}
	void keycontask() {
		titleback();
		DrawString(60, 130, "Not yet sorry..", white);
		if (backtask()) {
			gamemode = 6;
			menubar = 4;
		}
	}
	void savereplay(){
		if(playsaved<0){
			rpmeta data;
			playsaved=0;
			while(playsaved<100){
				data=replayread(playsaved);
				if(data.ver==0){
					data.ver=ver;
					GetDateTime(&data.date);
					data.rseed=rseed;
					data.isd=isdmode;
					data.fastsp=fastspeed;
					data.slowsp=slowspeed;
					data.score=score;
					data.kill=kill;
					data.miss=miss;
					data.time=gamef;
					data.stage=stage;
					double sum=0;
					for(int n=0;n<(gamef/60);n++){
						sum+=avgfps[n];
					}
					data.avgfps=sum/(gamef/60);
					replaywrite(data,playsaved);
					break;
				}
				playsaved++;
			}
		}
	}
	void scoreboard() {
		stopbgm();
		drawobj();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
		DrawBox(20, 10, 480, 470, GetColor(27, 2, 29), 1);
		DrawBox(40, 120, 250, 246, GetColor(0, 0, 0), 1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawBox(50, 178 + (menubar * 20), 240, 198 + (menubar * 20), GetColor(45, 25, 53), 1);
		DrawBox(50, 178 + (menubar * 20), 240, 198 + (menubar * 20), GetColor(145, 125, 153), 0);

		if (deathflag == 0) {
		DrawFormatString(60, 130, white, "STAGE-%d CLEAR!", stage + 1);
		DrawString(60, 180, "Save Replay", white);
		DrawString(60, 200, "Quit", white);
		DrawString(60, 220, "Retry", white);
		if(playsaved>=100){
			DrawString(200, 180, "ERROR", GetColor(255,0,0));
		}else if(playsaved>=0){
			DrawFormatString(200,180,white,"SAVED");
		}
		barmovetask(2);
		if (backtask()) {
			menubar = 1;
		}
		if (clicktask()) {
			switch (menubar) {
			case 0:
				savereplay();
				break;
			case 1:
				setbgm(0);
				if(replaymode){
					gamemode = 3;
				}else{
					gamemode = 1;
				}
				break;
			case 2:
				startgame();
				break;
			}
			menubar = 0;
		}
		}
		else {
		DrawString(60, 130, "Game Over", white);
		DrawString(60, 180, "Retry", white);
		DrawString(60, 200, "Quit", white);
		DrawString(60, 220, "Save Replay", white);
		if(playsaved>=100){
			DrawString(200, 180, "ERROR", GetColor(255,0,0));
		}else if(playsaved>=0){
			DrawFormatString(200,220,white,"SAVED");
		}
		barmovetask(2);
		if (backtask()) {
			menubar = 1;
		}
		if (clicktask()) {
			switch (menubar) {
			case 0:
				startgame();
				break;
			case 1:
				setbgm(0);
				if(replaymode){
					gamemode = 3;
				}else{
					gamemode = 1;
				}
				break;
			case 2:
				savereplay();
				break;
			}
			menubar = 0;
		}
		}
	}
	int clicktask() {
		if (clickwait == 0) {
			if (getkey(1)) {
				clickwait = 1;
				playse(menuse[2]);
				return 1;
			}
		}
		else {
			if (!getkey(1)) {
				clickwait = 0;
			}
		}
		return 0;
	}
	int backtask() {
		if (backwait == 0) {
			if (getkey(2)) {
				backwait = 1;
				playse(menuse[1]);
				return 1;
			}
		}
		else {
			if (!getkey(2)) {
				backwait = 0;
			}
		}
		return 0;
	}
	void barmovetaskex(char barmovemax, char sidemove) {
		if (barmove == 0) {
			if (getkey(3)) {
				barmove = 1;
				menubar -= sidemove;
				playse(menuse[0]);
				if (menubar < 0) {
					menubar += barmovemax + 1;
				}
			}
			if (getkey(4)) {
				barmove = 1;
				menubar += sidemove;
				playse(menuse[0]);
				if (menubar > barmovemax) {
					menubar -= barmovemax + 1;
				}
			}
			if (getkey(5)) {
				barmove = 1;
				menubar--;
				playse(menuse[0]);
				if (menubar < 0) {
					menubar = barmovemax;
				}
			}
			if (getkey(6)) {
				barmove = 1;
				menubar++;
				playse(menuse[0]);
				if (menubar > barmovemax) {
					menubar = 0;
				}
			}
		}
		else {
			if (!getkey(3) && !getkey(4) && !getkey(5) && !getkey(6)) {
				barmove = 0;
			}
		}
	}
	void barmovetask(char barmovemax) {
		if (barmove == 0) {
			if (getkey(3)) {
				barmove = 1;
				menubar--;
				playse(menuse[0]);
				if (menubar < 0) {
					menubar = barmovemax;
				}
			}
			if (getkey(4)) {
				barmove = 1;
				menubar++;
				playse(menuse[0]);
				if (menubar > barmovemax) {
					menubar = 0;
				}
			}
		}
		else {
			if (!getkey(3) && !getkey(4) && !getkey(5) && !getkey(6)) {
				barmove = 0;
			}
		}
	}
	void setsoundvol() {
		ChangeVolumeSoundMem(255 * musicvol / 100, bgm);
		for (int i = 0; i < 3; i++) {
			ChangeVolumeSoundMem(255 * sevol / 100, menuse[i]);
		}
		for (int i = 0; i < 2; i++) {
			ChangeVolumeSoundMem(255 * sevol / 100, bulletse[i]);
		}
		for (int i = 0; i < 3; i++) {
			ChangeVolumeSoundMem(255 * sevol / 100, playerse[i]);
		}
		for (int i = 0; i < 2; i++) {
			ChangeVolumeSoundMem(255 * sevol / 100, enemyse[i]);
		}
	}
	void stopbgm() {
		if (CheckSoundMem(bgm)) {
			StopSoundMem(bgm);
		}
	}
	void resumebgm() {
		if (CheckSoundMem(bgm) == 0) {
			PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, 0);
		}
	}
	void screenshottask() {
		if (screenshotwait == 0) {
			if (CheckHitKey(KEY_INPUT_P)) {
				if (_mkdir("snapshot") == 0) {
					screenshotwait = 1;
				}
				char snapshotname[] = "snapshot/0000.bmp";
				for (int i = 0; i < 4; i++) {
					int snapnameup = screenshotcount;
					for (int n = 0; n < i; n++) {
						snapnameup /= 10;
					}
					snapnameup %= 10;
					snapshotname[12 - i] += snapnameup;
				}
				SaveDrawScreenToBMP(0, 0, 640, 480, snapshotname);
				screenshotcount++;
				screenshotwait = 1;
			}
		}
		else {
			if (!CheckHitKey(KEY_INPUT_P)) {
				screenshotwait = 0;
			}
		}
	}
	void fpstask() {
		if (windowtime % 60 == 0) {
			fps = GetNowCount() - frametime;
			drawfps = 1000.0 / fps * 60.0;
			frametime = GetNowCount();
		}
		DrawFormatString(560, 460, white, "%.1fFPS", drawfps);
		DrawFormatString(560, 442, GetColor(127, 127, 127), "Ver %.1f", ver / 10.0);
		if(gamemode==10&&gamef%60==59){
			avgfps[gamef/60]=drawfps;
		}
		if(isdmode){
			int ddd=nrnd(0,360);
			int rr=191+sin(deg(ddd))*64;
			int gg=191+sin(deg(ddd+120))*64;
			int bb=191+sin(deg(ddd+240))*64;
			DrawString(549,424,"ISD MODE",GetColor(rr,gg,bb));
		}
	}
	void debugtask() {
		
		for (int i = 0; i < 7; i++) {
			//DrawGraph(300 + i * 20, 400 + i * 10, objimg[1][i], 1);
		}
		DrawBox(500, 300, 610, 320, GetColor(122, 12, 189), 1);
		DrawString(500, 300, "xdxdxiixxO", GetColor(255, 255, 255));
		DrawString(500, 310, "x", GetColor(255, 255, 255));
		DrawString(510, 310, "d", GetColor(255, 255, 255));
		DrawGraph(400, 100, playerimg[0], 0);
		DrawFormatString(10, 20, GetColor(255, 255, 255), "windowtime %d", windowtime);
		DrawFormatString(10, 40, GetColor(255, 255, 255), "allmiss %d", allmiss);
		DrawFormatString(10, 60, GetColor(255, 255, 255), "allkill %d", allkill);
		DrawFormatString(10, 200, GetColor(255, 255, 255), "sc %d", screenshotcount);
		DrawFormatString(10, 360, GetColor(255, 255, 255), "%d", nowkey);
		DrawFormatString(500, 360, GetColor(255, 255, 255), "%d", gamemode);
		DrawFormatString(490, 380, GetColor(255, 255, 255), "%d %d %d %d %d %d", getkey(1),getkey(2),getkey(3),getkey(4),getkey(5),getkey(6));
		DrawFormatString(500, 360, GetColor(255, 255, 255), "%d", menubar);
		DrawFormatString(500, 380, GetColor(255, 255, 255), "%d", barmove);
		DrawFormatString(500, 400, GetColor(255, 255, 255), "%d", clickwait);
		DrawFormatString(500, 100, GetColor(255, 255, 255), "%d", fps);
		DrawFormatString(100, 420, GetColor(255, 255, 255), "star %d", star);
		DrawFormatString(100, 440, GetColor(255, 255, 255), "starsc %d", starsc);
		DrawFormatString(100, 460, GetColor(255, 255, 255), "shied %d", shield);
		DrawFormatString(100, 360, GetColor(255, 255, 255), "scenef %d", scenef);
		DrawFormatString(100, 380, GetColor(255, 255, 255), "bosf %d", bostime);
		DrawFormatString(500, 460, GetColor(255, 255, 255), "bit %d", GetScreenBitDepth());
		DrawFormatString(500, 260, GetColor(255, 255, 255), "death %d", deathwaitf);
		DrawFormatString(500, 280, GetColor(255, 255, 255), "in01 %d", GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_1);
		
		for (int i = 0; i < 4; i++) {
			DrawFormatString(10 + 40 * i, 80, GetColor(255, 255, 255), "%d", hiscore[i]);
			DrawFormatString(10 + 40 * i, 100, GetColor(255, 255, 255), "%d", lowmiss[i]);
			DrawFormatString(10 + 40 * i, 120, GetColor(255, 255, 255), "%d", reachscene[i]);
			DrawFormatString(10 + 40 * i, 140, GetColor(255, 255, 255), "%d", playcount[i]);
			DrawFormatString(10 + 40 * i, 160, GetColor(255, 255, 255), "%d", clearcount[i]);
			DrawFormatString(10 + 40 * i, 180, GetColor(255, 255, 255), "%d", playtime[i]);
		}
		for(int i=0;i<30;i++){
			DrawFormatString(410 , 10+20*i, GetColor(255, 255, 255), "avg %f", avgfps[i]);
		}
	}
	void debugst(int dbgstset) {
		shield = 0;
		if (scene == 0) {
			scene = dbgstset;
		}
	}
};


class FrameTask {
	int wait = 0;
	int basetime = 0;
	int passtime = 0;
	int nowframe = 0;
	int nowtime = 0;
	int waitcount = 0;
public:
	void initframe() {
		basetime = GetNowCount();
		passtime = 0;
		nowframe = 0;
		nowtime = 0;
		wait = 0;
		waitcount = 0;
	}
	void setframe() {
		passtime = GetNowCount() - nowtime;
		waitcount = wait - passtime;

		if (waitcount > 0) {
			if (sleeptype == 0) {
				Sleep(waitcount);
			}
			else {
				WaitTimer(waitcount);
			}
		}

		ScreenFlip();

		nowframe++;
		nowtime = GetNowCount();
		passtime = nowtime - basetime;
		wait = nowframe * 100 / 6 - passtime;

		if (wait < -100) {
			initframe();
		}

		if (taskprepare >= 0) {
			waitcount = wait - taskprepare;
			if (waitcount > 0) {
				if (sleeptype == 0) {
					Sleep(waitcount);
				}
				else {
					WaitTimer(waitcount);
				}
			}
		}

		ClearDrawScreen();
	}
};


int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	configread();
	recordread();
	SetOutApplicationLogValidFlag(FALSE);
	SetGraphMode(640, 480, 32);
	SetMainWindowText("Phantom Bullet Dimension.");
	SetMainWindowClassName("pbdstg");
	ChangeWindowMode(winmode);
	SetWindowSizeExtendRate(winsizex / 640.0, winsizey / 480.0);
	SetWindowPosition(windowx, windowy);
	SetWaitVSyncFlag(vsync);
	if (DxLib_Init() == -1) {
		return 0;
	}
	SetAlwaysRunFlag(TRUE);
	SetUseDXArchiveFlag(TRUE);
	SetDXArchiveExtension("dat");
	SetDrawScreen(DX_SCREEN_BACK);
	ChangeFontType(DX_FONTTYPE_EDGE);
	SetJoypadInputToKeyInput(DX_INPUT_PAD1, PAD_INPUT_11, KEY_INPUT_LSHIFT);
	SetJoypadInputToKeyInput(DX_INPUT_PAD1, PAD_INPUT_12, KEY_INPUT_LCONTROL);
	SetJoypadDeadZone(DX_INPUT_PAD1, joypadzone / 100.0);
	if(bilinearon){
		SetDrawMode(DX_DRAWMODE_BILINEAR);
	}

	FrameTask* frame = new FrameTask;
	GameTask* game = new GameTask;

	frame->initframe();

	while (ProcessMessage() != -1 && gamemode != 0) {
		game->maintask();
		frame->setframe();
	}
	delete(game);
	delete(frame);
	DxLib_End();
	recordwrite();
	return 0;
}
