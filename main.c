//1791085 임경익
//테트리스 게임 (1p, 2p)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>



// 색상 정의
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
//#define RED1	4
#define MAGENTA1 5
#define ORANGE	6 //YELLOW1
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15
#define RED1 8

#define ESC 0x1b //  ESC 누르면 종료
#define ENTER 0x0D
#define SPACE 0x20

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define WIDTH 150
#define HEIGHT 50

#define TETRIS_WIDTH  11		//게임 너비 10
#define TETRIS_HEIGHT  21		//게임 높이 20

#define ROTATE_CLOCK 0
#define ROTATE_COUNTERCLOCK 1 

#define FILE_NAME "SCORE"
typedef struct Score {
	int score;
	char name[4];
}Score;
//block 관련
//block 좌표 LCS
const int block_O[4][2] = { {0,1}, {1,1}, {0,0}, {1,0} };
const int block_L[4][2] = { {0,0}, {-1,0},{1,1},{1,0} };
const int block_J[4][2] = { {0,0}, {1,0}, {-1,1}, {-1,0} };
const int block_I[4][2] = { {0,0}, {-1,0}, {1,0}, {2,0} };
const int block_T[4][2] = { {0,0}, {-1,0}, {1,0}, {0,1} };
const int block_Z[4][2] = { {0,0}, {0,1}, {-1,1}, {1,0} };
const int block_S[4][2] = { {0,0}, {-1,0}, {0,1}, {1,1} };
const int rotateClock[2][2] = { { 0,1 }, {-1,0} };
const int rotateCounterClock[2][2] = { { 0,-1 }, {1, 0} };
//block 관련 끝

int Delay = 100; // 100 msec delay, 이 값을 줄이면 속도가 빨라진다.
int scoreP1 = 0;
int scoreP2 = 0;

void removeCursor(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void switchTextColor(int block_num) {
	textcolor(block_num, BLACK);
}

// 화면 지우기고 원하는 배경색으로 설정한다.
void cls(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf_s(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);

}

//x, y 받아 이동 후 width 만큼 row 그리기
void draw_row(int x, int y, int color, int width) {
	textcolor(color, BLACK);
	int posX;
	for (posX = x; posX < x + width * 2; posX += 2) {
		gotoxy(posX, y);
		printf("■");
	}
}

//x, y 받아 이동 후 width 만큼 col 그리기
void draw_col(int x, int y, int color, int height) {
	textcolor(color, BLACK);
	int posY;
	for (posY = y; posY < y + height; posY++) {
		gotoxy(x, posY);
		printf("■");
	}
}

//메뉴 박스 생성, 글자 생성
void drawScreenMenu(int width, int height, int color) {
	int x, y;
	//cls(BLACK, RED1);
	textcolor(color, BLACK);
	for (x = 0; x < width; x += 2) {
		gotoxy(x, 0);
		printf("■");
		gotoxy(x, height - 1);
		printf("■");
	}
	for (y = 0; y < height; y++) {
		gotoxy(0, y);
		printf("■");
		gotoxy(width - 2, y);
		printf("■");
	}

	int moveX, moveY;
	moveX = 24;
	moveY = 0;

	draw_row(10 + moveX, 6 + moveY, RED2, 5);	//T
	draw_col(14 + moveX, 6, RED2, 7);

	draw_row(24 + moveX, 6, ORANGE, 5);		//E
	draw_row(24 + moveX, 9, ORANGE, 5);
	draw_row(24 + moveX, 12, ORANGE, 5);
	draw_col(24 + moveX, 6, ORANGE, 7);

	draw_row(38 + moveX, 6, YELLOW2, 5);	//T
	draw_col(42 + moveX, 6, YELLOW2, 7);

	draw_row(52 + moveX, 6, GREEN2, 5);		//R
	draw_col(52 + moveX, 6, GREEN2, 7);
	draw_col(60 + moveX, 6, GREEN2, 4);
	draw_row(52 + moveX, 9, GREEN2, 5);
	draw_col(56 + moveX, 10, GREEN2, 1);
	draw_col(58 + moveX, 11, GREEN2, 1);
	draw_col(60 + moveX, 12, GREEN2, 1);

	draw_row(66 + moveX, 6, BLUE2, 5);		//I
	draw_col(70 + moveX, 6, BLUE2, 7);
	draw_row(66 + moveX, 12, BLUE2, 5);

	draw_row(80 + moveX, 6, MAGENTA2, 5);	//S
	draw_row(80 + moveX, 9, MAGENTA2, 5);
	draw_row(80 + moveX, 12, MAGENTA2, 5);
	draw_col(80 + moveX, 6, MAGENTA2, 3);
	draw_col(88 + moveX, 9, MAGENTA2, 3);


}

//flag 받아 메뉴 선택 하이라이트
void menuSelect(int width, int height, int color, int mFlag, int oFlag) {
	textcolor(color, BLACK);
	if (mFlag == 0)
		textcolor(color, GRAY1);
	else
		textcolor(color, BLACK);
	gotoxy(width / 2 - 10, height / 3 * 2);
	printf("START GAME");
	textcolor(color, BLACK);

	if (mFlag == 1)
		textcolor(color, GRAY1);
	else
		textcolor(color, BLACK);
	gotoxy(width / 2 - 10, height / 3 * 2 + 2);
	printf("                     ");
	gotoxy(width / 2 - 10, height / 3 * 2 + 2);
	if (oFlag == 0)
		printf("OPTION : SINGLE PLAY");
	else if (oFlag == 1)
		printf("OPTION : P1 VS P2");

	else if (oFlag == 2)
		printf("OPTION : VS COM");
	textcolor(color, BLACK);

	if (mFlag == 2)
		textcolor(color, GRAY1);
	else
		textcolor(color, BLACK);
	gotoxy(width / 2 - 10, height / 3 * 2 + 4);
	printf("SHOW SCORE");
	textcolor(color, BLACK);

	if (mFlag == 3)
		textcolor(color, GRAY1);
	gotoxy(width / 2 - 10, height / 3 * 2 + 6);
	printf("EXIT");
	textcolor(color, BLACK);
	gotoxy(0, 0);
	Sleep(100);
}

//메뉴 동작
int onMenuFunc() {
	unsigned char ch;
	int menuFlag = 0;
	int optionFlag = 0;
	drawScreenMenu(WIDTH, HEIGHT, RED1);
	while (1) {
		menuSelect(WIDTH, HEIGHT, RED1, menuFlag, optionFlag);
		if (_kbhit() == 1) {  // 키보드가 눌려져 있으면
			ch = _getch(); // key 값을 읽는다
			if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = _getch();
				switch (ch) {
				case UP:
					if (menuFlag > 0)
						menuFlag--;
					break;
				case DOWN:
					if (menuFlag < 3)
						menuFlag++;
					break;

				}
			}
			else
				switch (ch) {
				case ENTER:
					if (menuFlag == 0) {
						if (optionFlag == 0)
							return 0;
						else if (optionFlag == 1)
							return -1;
						else if (optionFlag == 2)
							return -2;
					}

					else if (menuFlag == 1) {
						optionFlag++;
						optionFlag %= 2;
					}
					else if (menuFlag == 2)
						return 2;
					else if (menuFlag == 3)
						return 3;

					break;
				}
		}
		else;
	}
}

//x,y 좌표에 테트리스 틀, 박스 생성
void draw_tetris_box(int x, int y, int color)
{
	int info_box_width = 7;

	//game box start
	textcolor(color, BLACK);
	int posX, posY;
	for (posX = x * 2; posX <= x * 2 + TETRIS_WIDTH * 2; posX += 2) {
		gotoxy(posX, y);
		printf("■");
		gotoxy(posX, y + TETRIS_HEIGHT);
		printf("■");;
	}
	for (posY = y; posY <= y + TETRIS_HEIGHT; posY++) {
		gotoxy(x * 2, posY);
		printf("■");
		gotoxy(x * 2 + TETRIS_WIDTH * 2, posY);
		printf("■");
	}
	//game box end

	//info box start
	draw_row(x * 2 + TETRIS_WIDTH * 2, y, RED1, info_box_width);
	draw_col(x * 2 + TETRIS_WIDTH * 2 + info_box_width * 2, y, RED1, TETRIS_HEIGHT);
	draw_row(x * 2 + TETRIS_WIDTH * 2 + 2, y + TETRIS_HEIGHT, RED1, info_box_width);
	//info box end
}

//테트리스 박스 옆 정보 표시
void onInfoFunc(int startX, int startY, int score, int block_shape[][2], int block, char* player) {
	gotoxy((startX + TETRIS_WIDTH + 2) * 2, startY + 2);
	printf("?PLAYER?");
	gotoxy((startX + TETRIS_WIDTH + 3) * 2, startY + 3);
	printf("%3s", player);
	gotoxy((startX + TETRIS_WIDTH + 3) * 2, startY + 5);
	printf("NEXT");
	for (int i = 0; i < 4; i++) {
		gotoxy((startX + TETRIS_WIDTH + 2) * 2, startY + 7 + i);
		printf("        ");
	}
	switchTextColor(block);
	for (int i = 0; i < 4; i++) {
		gotoxy((startX + TETRIS_WIDTH + 3 + block_shape[i][0]) * 2, startY + 8 - block_shape[i][1]);
		printf("■");
	}
	switchTextColor(RED1);
	gotoxy((startX + TETRIS_WIDTH + 3) * 2, startY + 10);
	printf("SCORE");
	gotoxy((startX + TETRIS_WIDTH + 3.5) * 2, startY + 11);
	printf("%03d", score);

	switchTextColor(RED1);
}

//블럭 choice
int selectBlock() {
	return rand() % 7 + 1;
}

//블럭을 반시계 90도 회전
void rotateBlock(int block[][2]) {
	int result[4][2];
	memcpy(result, block, sizeof(block_O));
	result[0][0] = block[0][0] * rotateCounterClock[0][0] + block[0][1] * rotateCounterClock[0][1];
	result[0][1] = block[0][0] * 1 + block[0][1] * 0;
	result[1][0] = block[1][0] * 0 + block[1][1] * (-1);
	result[1][1] = block[1][0] * 1 + block[1][1] * 0;
	result[2][0] = block[2][0] * 0 + block[2][1] * (-1);
	result[2][1] = block[2][0] * 1 + block[2][1] * 0;
	result[3][0] = block[3][0] * 0 + block[3][1] * (-1);
	result[3][1] = block[3][0] * 1 + block[3][1] * 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 2; j++)
			block[i][j] = result[i][j];
}

//테트리스 박스의 시작 좌표, 테트리스 블럭 중심의 좌표, 블럭 상태, 블럭 번호를 받아 블럭 그림
void drawBlock(int startX, int startY, int posX, int posY, const int block_shape[][2], int block_state, int block) {
	switchTextColor(block);
	int result[4][2];
	memcpy(result, block_shape, sizeof(block_O));
	for (int i = 0; i < block_state; i++)
		rotateBlock(result);
	for (int i = 0; i < 4; i++) {
		if (posY - result[i][1] > 0) {
			gotoxy((startX + posX + result[i][0]) * 2, startY + posY - result[i][1]);
			printf("■");
		}
	}
	switchTextColor(RED1);
}

//테트리스 박스의 시작 좌표, 테트리스 블럭 중심의 좌표, 블럭 상태를 받아 블럭 삭제
void eraseBlock(int startX, int startY, int posX, int posY, const int block_shape[][2], int block_state) {
	int position[4];
	int result[4][2];
	int left = 0, right = 0, up = 0, down = 0;
	memcpy(result, block_shape, sizeof(block_O));
	for (int i = 0; i < block_state; i++)
		rotateBlock(result);
	for (int i = 0; i < 4; i++) {
		if (posY - result[i][1] > 0) {
			gotoxy((startX + posX + result[i][0]) * 2, startY + posY - result[i][1]);
			printf("  ");
		}
	}
}

//테트리스 블럭의 중심 좌표를 받고, 상태 정보 받아 맵과 비교 좌측 이동 가능한지.
int checkLeft(int posX, int posY, int block_shape[][2], int block_state, int map[][TETRIS_WIDTH - 1]) {
	int result[4][2];
	memcpy(result, block_shape, sizeof(block_O));
	for (int i = 0; i < block_state; i++)
		rotateBlock(result);
	for (int i = 0; i < 4; i++) {
		if (result[i][0] + posX - 1 == 0)
			return 0;
		else if (result[i][0] + posX > 0)
			if (posY - result[i][1] > 0)
				if (map[posY - result[i][1] - 1][posX + result[i][0] - 2] != 0)
					return 0;
				else;
	}
	return 1;
}

//테트리스 블럭의 중심 좌표를 받고, 상태 정보 받아 맵과 비교 이측 이동 가능한지.
int checkRight(int posX, int posY, int block_shape[][2], int block_state, int map[][TETRIS_WIDTH - 1]) {
	int result[4][2];
	memcpy(result, block_shape, sizeof(block_O));
	for (int i = 0; i < block_state; i++)
		rotateBlock(result);
	for (int i = 0; i < 4; i++) {
		if (result[i][0] + posX == TETRIS_WIDTH - 1)
			return 0;
		else if (result[i][0] + posX < TETRIS_WIDTH - 1)
			if (posY - result[i][1] > 0)
				if (map[posY - result[i][1] - 1][posX + result[i][0]] != 0)
					return 0;
				else;
	}
	return 1;
}

//테트리스 블럭의 중심 좌표를 받고, 상태 정보 받아 맵과 비교 반시계회전가능한지
int checkRotateCounterClock(int posX, int posY, int block_shape[][2], int block_state, int map[][TETRIS_WIDTH - 1]) {
	int result[4][2];
	memcpy(result, block_shape, sizeof(block_O));
	for (int i = 0; i < block_state + 1; i++)
		rotateBlock(result);
	for (int i = 0; i < 4; i++) {
		if (result[i][0] + posX <= 0)
			return 0;
		else if (result[i][0] + posX > TETRIS_WIDTH - 1)
			return 0;
		if (posY - result[i][1] <= 0)
			return 0;
		else if (map[posY - result[i][1] - 1][result[i][0] - 1 + posX] != 0)
			return 0;
		if (posY - result[i][1] > TETRIS_HEIGHT - 2)
			return 0;
	}
	return 1;
}

//테트리스 블럭의 중심 좌표를 받고, 상태 정보 받아 맵과 비교 시계회전가능한지
int checkRotateClock(int posX, int posY, int block_shape[][2], int block_state, int map[][TETRIS_WIDTH - 1]) {
	int result[4][2];
	memcpy(result, block_shape, sizeof(block_O));
	for (int i = 0; i < block_state - 1; i++)
		rotateBlock(result);
	for (int i = 0; i < 4; i++) {
		if (result[i][0] + posX <= 0)
			return 0;
		else if (result[i][0] + posX >= TETRIS_WIDTH)
			return 0;
		if (posY - result[i][1] <= 0)
			return 0;
		else if (map[posY - result[i][1] - 1][result[i][0] - 1 + posX] != 0)
			return 0;
		if (posY - result[i][1] > TETRIS_HEIGHT - 2)
			return 0;
	}
	return 1;
}


//테트리스 정보 받아 아래 표시 블럭 그림
void drawHelpBlock(int startX, int startY, int posX, int posY, int block_shape[][2], int block_state, int map[][TETRIS_WIDTH - 1], int block_num) {
	int min = 10;
	int maxRow[2] = { 0, TETRIS_HEIGHT - 2 };
	int isChecked = 0;
	int result[4][2];
	int flag;
	memcpy(result, block_shape, sizeof(block_O));
	for (int i = 0; i < block_state; i++)
		rotateBlock(result);

	for (int i = 0; i < 4; i++) {
		for (int j = posY; j < TETRIS_HEIGHT - 1; j++) {
			if (map[j][result[i][0] - 1 + posX] != 00 && j < maxRow[1]) {
				maxRow[1] = j;
				maxRow[0] = i;
				isChecked = 1;
				break;
			}
		}
	}

	if (maxRow[1] == 0 && !isChecked)
		maxRow[1] = TETRIS_HEIGHT - 2;

	switchTextColor(block_num);

	for (int i = maxRow[1]; i >= 0; i--) {
		flag = 0;
		for (int j = 0; j < 4; j++) {
			if (map[i - result[j][1]][result[j][0] - 1 + posX] != 0
				|| i - result[maxRow[0]][1] > maxRow[1]) {
				flag = 1;
			}
			if (i - result[j][1] > TETRIS_HEIGHT - 2)
				flag = 1;
		}
		if (!flag) {
			for (int j = 0; j < 4; j++) {
				gotoxy((result[j][0] + posX + startX) * 2, (i - result[j][1]) + startY + 1);
				printf("□");
			}
			break;
		}
	}
	switchTextColor(RED1);
}

//테트리스 정보 받아 아래 표시 블럭 지움
void eraseHelpBlock(int startX, int startY, int posX, int posY, int block_shape[][2], int block_state, int map[][TETRIS_WIDTH - 1]) {
	int min = 10;
	int maxRow[2] = { 0, TETRIS_HEIGHT - 2 };
	int isChecked = 0;
	int result[4][2];
	int flag;
	memcpy(result, block_shape, sizeof(block_O));
	for (int i = 0; i < block_state; i++)
		rotateBlock(result);

	for (int i = 0; i < 4; i++) {
		for (int j = posY; j < TETRIS_HEIGHT - 1; j++) {
			if (map[j][result[i][0] - 1 + posX] != 00 && j < maxRow[1]) {
				maxRow[1] = j;
				maxRow[0] = i;
				isChecked = 1;
				break;
			}
		}
	}

	if (maxRow[1] == 0 && !isChecked)
		maxRow[1] = TETRIS_HEIGHT - 2;



	for (int i = maxRow[1]; i >= 0; i--) {
		flag = 0;
		for (int j = 0; j < 4; j++) {
			if (map[i - result[j][1]][result[j][0] - 1 + posX] != 0
				|| i - result[maxRow[0]][1] > maxRow[1]) {
				flag = 1;
			}
			if (i - result[j][1] > TETRIS_HEIGHT - 2)
				flag = 1;
		}
		if (!flag) {
			for (int j = 0; j < 4; j++) {
				gotoxy((result[j][0] + posX + startX) * 2, (i - result[j][1]) + startY + 1);
				printf("  ");
			}
			break;
		}
	}
}

//테트리스 정보 받아 블럭을 놓는다. 블럭 놓기 실패시 0 반환
int putBlock(int startX, int startY, int posX, int posY, int block_shape[][2], int block_state, int map[][TETRIS_WIDTH - 1], int block) {
	int min = 10;
	int maxRow[2] = { 0, TETRIS_HEIGHT - 2 };
	int isChecked = 0;
	int result[4][2];
	int flag;
	memcpy(result, block_shape, sizeof(block_O));
	for (int i = 0; i < block_state; i++)
		rotateBlock(result);

	for (int i = 0; i < 4; i++) {
		for (int j = posY; j < TETRIS_HEIGHT - 1; j++) {
			if (map[j][result[i][0] - 1 + posX] != 00 && j < maxRow[1]) {
				maxRow[1] = j;
				maxRow[0] = i;
				isChecked = 1;
				break;
			}
		}
	}

	if (maxRow[1] == 0 && !isChecked)
		maxRow[1] = TETRIS_HEIGHT - 2;

	for (int i = maxRow[1]; i >= 0; i--) {
		flag = 0;
		for (int j = 0; j < 4; j++) {
			if (map[i - result[j][1]][result[j][0] - 1 + posX] != 0
				|| i - result[maxRow[0]][1] > maxRow[1]) {
				flag = 1;
			}
			if (i - result[j][1] > TETRIS_HEIGHT - 2)
				flag = 1;
		}
		if (!flag) {
			for (int j = 0; j < 4; j++)
				map[i - result[j][1]][result[j][0] - 1 + posX] = block;
			break;
		}
	}
	if (flag)
		return 1;
	else
		return 0;
}

//블럭을 아래에 놓을수 있는지, 블럭의 posY 감소시킬 때 사용
int checkBelow(int posX, int posY, int block_shape[][2], int block_state, int map[][TETRIS_WIDTH - 1], int block) {
	int min = 10;
	int maxRow[2] = { 0, TETRIS_HEIGHT - 2 };
	int isChecked = 0;
	int result[4][2];
	int flag;
	memcpy(result, block_shape, sizeof(block_O));
	for (int i = 0; i < block_state; i++)
		rotateBlock(result);

	for (int j = 0; j < 4; j++) {
		if (map[posY - result[j][1]][result[j][0] - 1 + posX] != 0)
			return 1;
	}
	return 0;
}

int isGameEnd(int map[][TETRIS_WIDTH - 1]) {
	for (int i = 0; i < TETRIS_WIDTH - 1; i++)
		if (map[0][i] != 0)
			return 1;
	return 0;
}


//map 기준 테트리스 정보 그림
void drawMatrix(int startX, int startY, int map[][TETRIS_WIDTH - 1]) {
	for (int i = 0; i < TETRIS_HEIGHT - 1; i++) {
		for (int j = 0; j < TETRIS_WIDTH - 1; j++) {
			if (map[i][j] != 0) {
				switchTextColor(map[i][j]);
				gotoxy(startX * 2 + 2 + j * 2, startY + 1 + i);
				printf("■");
				switchTextColor(RED1);
			}
			else {/*  //사용시 drawBlock과 상반되는 동작, 삭제
				gotoxy(startX * 2 + 2 + j * 2, startY + 1 + i);
				printf("  ");*/
			}
		}
	}
}

//map 정보 업데이트, row 가 full 시 해당 row pop
int popMatrix(int startX, int startY, int map[][TETRIS_WIDTH - 1], int* score) {
	int i, j;
	int isRowFull;
	int isRowEmpty;
	int emptyRowIndex;
	int index = 0;
	int fullRow[TETRIS_HEIGHT - 1] = { 0 };
	for (i = TETRIS_HEIGHT - 2; i >= 0; i--) {
		isRowFull = i;
		isRowEmpty = i;
		for (j = 0; j < TETRIS_WIDTH - 1; j++)
			if (map[i][j] != 0)
				isRowEmpty = 0;
			else
				isRowFull = 0;
		if (isRowFull)
			fullRow[index++] = i;
		if (isRowEmpty) {
			emptyRowIndex = i;
			break;
		}
	}

	if (fullRow[0] != 0) {
		for (i = 0; i < index; i++) {
			for (j = 0; j < TETRIS_WIDTH - 1; j++) {
				gotoxy((startX + 1 + j) * 2, startY + fullRow[i] + 1);
				printf("□");
			}
		}
		Sleep(150);
		for (i = 0; i < index; i++) {
			for (j = 0; j < TETRIS_WIDTH - 1; j++) {
				gotoxy((startX + 1 + j) * 2, startY + fullRow[i] + 1);
				printf("  ");
				map[fullRow[i]][j] = 0;
			}
		}
		Sleep(150);
		for (i = index - 1; i >= 0; i--) {
			for (j = fullRow[i]; j > emptyRowIndex; j--) {
				for (int k = 0; k < TETRIS_WIDTH - 1; k++) {
					map[j][k] = map[j - 1][k];
				}
			}
		}
		for (i = fullRow[index - 1]; i > emptyRowIndex; i--) {
			for (j = 0; j < TETRIS_WIDTH - 1; j++) {
				if (map[i][j] == 0) {
					gotoxy((startX + 1 + j) * 2, startY + 1 + i);
					printf("  ");
				}
			}
		}
		(*score) += index * index;
	}
	return index - 1;
}

//대전 시 상대방에게 블럭 줄 놓아주는 함수
void addMatrix(int startX, int startY, int map[][TETRIS_WIDTH - 1], int index) {
	if (index <= 0)
		return;
	int temp_map[TETRIS_HEIGHT - 1][TETRIS_WIDTH - 1] = { 0, };
	int maxRow = 0;
	int num;
	for (int i = 0; i < TETRIS_HEIGHT - 1; i++) {
		for (int j = 0; j < TETRIS_WIDTH - 1; j++) {
			temp_map[i][j] = map[i][j];
			if (!maxRow)
				if (map[i][j] != 0) {
					maxRow = i;
					continue;
				}
		}
	}
	maxRow = maxRow - index;
	if (maxRow < 0)
		maxRow = 0;
	for (int i = TETRIS_HEIGHT - 2; i > TETRIS_HEIGHT - 2 - index; i--) {
		num = rand() % TETRIS_WIDTH - 1;
		for (int j = 0; j < TETRIS_WIDTH - 1; j++) {
			if (j != num)
				map[i][j] = 11;
			else {
				gotoxy((startX + j + 1) * 2, startY + 1 + i);
				printf("  ");
				map[i][j] = 0;
			}
		}
	}
	for (int i = TETRIS_HEIGHT - 2 - index; i >= 0; i--) {
		for (int j = 0; j < TETRIS_WIDTH - 1; j++) {
			map[i][j] = temp_map[i + index][j];
		}
	}
	for (int i = 0; i < TETRIS_HEIGHT - 1; i++) {
		for (int j = 0; j < TETRIS_WIDTH - 1; j++) {
			if (map[i][j] == 0) {
				gotoxy((startX + 1 + j) * 2, startY + 1 + i);
				printf("  ");
			}
		}
	}
}

void displayScore(int startX, int startY, int score, char* player) {


	draw_row(startX * 2 + 8, startY + 4, RED1, 11);
	draw_row(startX * 2 + 8, startY + 6 + 4, RED1, 11);
	draw_col(startX * 2 + 8, startY + 4, RED1, 7);
	draw_col(startX * 2 + 8 + 20, startY + 4, RED1, 7);

	for (int i = 0; i < 5; i++) {
		gotoxy(startX * 2 + 10, startY + 5 + i);
		printf("                  ");
	}

	gotoxy(startX * 2 + 8 + 10, startY + 6);
	printf("%s", player);
	gotoxy(startX * 2 + 8 + 6, startY + 8);
	printf("SCORE : %03d", score);
}

char* drawPlayerInputBox(int x, int y, int color) {
	int startX = WIDTH / 2 - 11;
	char name[3] = { '_','_','_' };
	draw_row(startX, y, color, 11);
	draw_row(startX, y + 6, color, 11);
	draw_col(startX, y, color, 7);
	draw_col(startX + 20, y, color, 7);
	gotoxy(startX + 6, y + 2);
	printf("INPUT NAME");
	gotoxy(startX + 9, y + 4);
	printf("_ _ _");
	for (int i = 0; i < 3; i++) {
		name[i] = _getch();
		gotoxy(startX + 9, y + 4);
		printf("%c %c %c", name[0], name[1], name[2]);
	}
	_getch();
	return name;
}

void set_block_shape(int block_shape[][2], int block) {
	switch (block) {
	case 1:
		memcpy(block_shape, block_O, sizeof(block_O));
		break;
	case 2:
		memcpy(block_shape, block_T, sizeof(block_T));
		break;
	case 3:
		memcpy(block_shape, block_S, sizeof(block_S));
		break;
	case 4:
		memcpy(block_shape, block_Z, sizeof(block_Z));
		break;
	case 5:
		memcpy(block_shape, block_L, sizeof(block_L));
		break;
	case 6:
		memcpy(block_shape, block_J, sizeof(block_J));
		break;
	case 7:
		memcpy(block_shape, block_I, sizeof(block_I));
		break;
	}
}

void saveScore(int score, char* player) {
	FILE* fp;
	Score data;
	data.score = score;
	strcpy(data.name, player);
	data.name[3] = NULL;

	if ((fp = fopen(FILE_NAME, "r+b")) == NULL) { // 파일이 없으면
		fp = fopen(FILE_NAME, "w+b"); // 새로 생성
		if (fp == NULL) {
			printf("파일을 열 수 없습니다.\n");
			exit(0);
		}
	}
	fseek(fp, 0, SEEK_END);
	fwrite(&data, sizeof(Score), 1, fp);
	fclose(fp);
}

void showScore() {
	system("cls");
	FILE* fp;
	Score temp;
	Score data[100];
	int startX = WIDTH / 2 - 15;
	int startY = 5;
	draw_row(startX, startY, RED1, 15);
	draw_col(startX, startY, RED1, 20);
	draw_col(startX + 28, startY, RED1, 20);
	draw_row(startX, startY + 20, RED1, 15);
	int num, fileSize;
	int i = 13;

	if ((fp = fopen(FILE_NAME, "r+b")) == NULL) { // 파일이 없으면
		fp = fopen(FILE_NAME, "w+b"); // 새로 생성
		if (fp == NULL) {
			printf("파일을 열 수 없습니다.\n");
		}
	}
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	num = fileSize / sizeof(Score);

	if (i > num)
		i = num;

	fseek(fp, 0, SEEK_SET);
	while (num--) {
		fread(&temp, sizeof(Score), 1, fp);
		data[num] = temp;
	}
	num = fileSize / sizeof(Score);
	for (int j = num - 1; j > 0; j--) {
		for (int k = 0; k < j; k++)
			if (data[k].score < data[k + 1].score) {
				Score temp = data[k];
				data[k] = data[k + 1];
				data[k + 1] = temp;
			}
	}

	gotoxy(startX + 6, startY + 2);
	printf("PLAYER   |   SCORE");
	gotoxy(startX + 2, startY + 4);
	printf("■■■■■■■■■■■■■■");
	for (int j = 0; j < i; j++) {

		gotoxy(startX + 8, startY + 6 + j);
		printf("%s    |    %03d", data[j].name, data[j].score);
	}
	_getch();
}

void playSinglePlay() {
	int map[TETRIS_HEIGHT - 1][TETRIS_WIDTH - 1] = { 0, };
	int startX = 25;		//tetris box start X
	int startY = 10;		//tetris box start Y
	int posX, posY;
	int score = 0;
	int i = 0;
	int block;
	int next_block;
	int next_block_shape[4][2];
	char player[4];
	clock_t start_time;
	clock_t current_time;
	double interval = 0.5;
	system("cls");
	strcpy(player, drawPlayerInputBox(10, 10, RED1));
	player[3] = '\0';

	system("cls");
	block = selectBlock();
	draw_tetris_box(startX, startY, RED1);
	while (1) {
	newBlock:
		start_time = clock();
		next_block = selectBlock();
		set_block_shape(next_block_shape, next_block);
		drawMatrix(startX, startY, map);
		posX = TETRIS_WIDTH / 2, posY = 1;
		int block_shape[4][2];
		int block_state = 0;
		set_block_shape(block_shape, block);
		onInfoFunc(startX, startY, score, next_block_shape, next_block, player);
		drawBlock(startX, startY, posX, posY, block_shape, block_state, block);
		while (1) {
			unsigned char ch;
			if (_kbhit() == 1) {  // 키보드가 눌려져 있으면
				ch = _getch(); // key 값을 읽는다
				if (ch == SPECIAL1 || ch == SPECIAL2) {
					ch = _getch();
					eraseHelpBlock(startX, startY, posX, posY, block_shape, block_state, map);
					eraseBlock(startX, startY, posX, posY, block_shape, block_state);
					switch (ch) {
					case UP:
						if (checkRotateCounterClock(posX, posY, block_shape, block_state, map))
							block_state = (block_state + 1) % 4;
						break;
					case DOWN:
						if (checkRotateClock(posX, posY, block_shape, block_state, map))
							if (block_state == 0)
								block_state = 3;
							else
								block_state--;
						break;
					case LEFT:
						if (checkLeft(posX, posY, block_shape, block_state, map)) {
							posX--;
						}
						break;
					case RIGHT:
						if (checkRight(posX, posY, block_shape, block_state, map)) {
							posX++;
						}
						break;
					}
				}
				else {
					switch (ch) {
					case ENTER:
						eraseBlock(startX, startY, posX, posY, block_shape, block_state);
						eraseHelpBlock(startX, startY, posX, posY, block_shape, block_state, map);
						putBlock(startX, startY, posX, posY, block_shape, block_state, map, block);
						drawMatrix(startX, startY, map);
						popMatrix(startX, startY, map, &score);
						block = next_block;
						goto newBlock;
						break;
					}

				}
				drawMatrix(startX, startY, map);
				drawHelpBlock(startX, startY, posX, posY, block_shape, block_state, map, block);
				drawBlock(startX, startY, posX, posY, block_shape, block_state, block);
			}
			else;
			i++;
			current_time = clock();
			if (current_time - start_time >= (interval - 0.05 * score / 10) * CLOCKS_PER_SEC) {
				eraseBlock(startX, startY, posX, posY, block_shape, block_state);
				eraseHelpBlock(startX, startY, posX, posY, block_shape, block_state, map);
				if (checkBelow(posX, posY, block_shape, block_state, map, block)) {
					if (putBlock(startX, startY, posX, posY, block_shape, block_state, map, block)) {
						drawMatrix(startX, startY, map);
						displayScore(startX, startY, score, player);
						saveScore(score, player);
						_getch();
						return;
					}
					drawMatrix(startX, startY, map);
					popMatrix(startX, startY, map, &score);
					drawMatrix(startX, startY, map);
					block = next_block;
					goto newBlock;
				}
				else {
					start_time = clock();
					posY++;
					drawHelpBlock(startX, startY, posX, posY, block_shape, block_state, map, block);
					drawBlock(startX, startY, posX, posY, block_shape, block_state, block);
					i = 0;
				}
			}
		}
	}
}

void playVsPlayer() {
	int p1_map[TETRIS_HEIGHT - 1][TETRIS_WIDTH - 1] = { 0, };
	int p1_startX = 15, p1_startY = 10;
	int p1_posX, p1_posY;
	int p1_score = 0;
	int p1_block;
	int p1_block_next;
	int p1_block_next_shape[4][2];
	clock_t p1_start_time;
	clock_t p1_current_time;
	double p1_interval = 0.5;
	int p1_block_shape[4][2];
	int p1_block_state = 0;

	int p2_map[TETRIS_HEIGHT - 1][TETRIS_WIDTH - 1] = { 0,0, };
	int p2_startX = 45, p2_startY = 10;
	int p2_posX, p2_posY;
	int p2_score = 0;
	int p2_block;
	int p2_block_next;
	int p2_block_next_shape[4][2];
	clock_t p2_start_time;
	clock_t p2_current_time;
	double p2_interval = 0.5;
	int p2_block_shape[4][2];
	int p2_block_state = 0;

	int winner = 0;


	system("cls");

	draw_tetris_box(p1_startX, p1_startY, RED1);
	draw_tetris_box(p2_startX, p2_startY, RED1);

	p1_block = selectBlock();
	p2_block = selectBlock();

	p1_block_next = selectBlock();
	p2_block_next = selectBlock();

	p1_start_time = clock();
	p2_start_time = clock();

	p1_block_state = 0;
	p2_block_state = 0;

	p1_posX = TETRIS_WIDTH / 2, p1_posY = 1;
	p2_posX = TETRIS_WIDTH / 2, p2_posY = 1;
	while (1) {
	newBlock:
		set_block_shape(p1_block_next_shape, p1_block_next);
		set_block_shape(p2_block_next_shape, p2_block_next);

		drawMatrix(p1_startX, p1_startY, p1_map);
		drawMatrix(p2_startX, p2_startY, p2_map);

		set_block_shape(p1_block_shape, p1_block);
		set_block_shape(p2_block_shape, p2_block);

		onInfoFunc(p1_startX, p1_startY, p1_score, p1_block_next_shape, p1_block_next, "P_1");
		onInfoFunc(p2_startX, p2_startY, p2_score, p2_block_next_shape, p2_block_next, "P_2");
		drawBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_block);
		drawBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_block);

		while (1) {
			unsigned char ch;
			if (_kbhit() == 1) {  // 키보드가 눌려져 있으면
				ch = _getch(); // key 값을 읽는다
				if (ch == SPECIAL1 || ch == SPECIAL2) {	//방향키이면
					ch = _getch();
					eraseHelpBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map);
					eraseBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state);
					switch (ch) {
					case UP:
						if (checkRotateCounterClock(p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map))
							p2_block_state = (p2_block_state + 1) % 4;
						break;
					case DOWN:
						if (checkRotateClock(p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map))
							if (p2_block_state == 0)
								p2_block_state = 3;
							else
								p2_block_state--;
						break;
					case LEFT:
						if (checkLeft(p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map)) {
							p2_posX--;
						}
						break;
					case RIGHT:
						if (checkRight(p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map)) {
							p2_posX++;
						}
						break;
					}
				}
				else {	//방향키가 아니면
					switch (ch) {
					case ENTER:
						eraseBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state);
						eraseHelpBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map);
						putBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map, p2_block);
						drawMatrix(p2_startX, p2_startY, p2_map);
						addMatrix(p1_startX, p1_startY, p1_map, popMatrix(p2_startX, p2_startY, p2_map, &p2_score));
						p2_block = p2_block_next;
						p2_block_next = selectBlock();
						p2_block_state = 0;
						p2_posX = TETRIS_WIDTH / 2, p2_posY = 1;
						goto newBlock;
						break;
					case 'A':
					case 'a':
						eraseHelpBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map);
						eraseBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state);
						if (checkLeft(p1_posX, p1_posY, p1_block_shape, p1_block_state, p2_map)) {
							p1_posX--;
						}
						break;
					case 'D':
					case 'd':
						eraseHelpBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map);
						eraseBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state);
						if (checkRight(p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map)) {
							p1_posX++;
						}
						break;
					case 'W':
					case 'w':
						eraseHelpBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map);
						eraseBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state);
						if (checkRotateCounterClock(p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map))
							p1_block_state = (p1_block_state + 1) % 4;
						break;
					case 'S':
					case 's':
						eraseHelpBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map);
						eraseBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state);
						if (checkRotateClock(p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map))
							p1_block_state = (p1_block_state + 1) % 4;
						break;
					case SPACE:
						eraseBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state);
						eraseHelpBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map);
						putBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map, p1_block);
						drawMatrix(p1_startX, p1_startY, p1_map);
						addMatrix(p2_startX, p2_startY, p2_map, popMatrix(p1_startX, p1_startY, p1_map, &p1_score));
						p1_block = p1_block_next;
						p1_block_next = selectBlock();
						p1_block_state = 0;
						p1_posX = TETRIS_WIDTH / 2, p1_posY = 1;
						goto newBlock;
						break;
					}

				}
				drawMatrix(p2_startX, p2_startY, p2_map);
				drawHelpBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map, p2_block);
				drawBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_block);

				drawMatrix(p1_startX, p1_startY, p1_map);
				drawHelpBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map, p1_block);
				drawBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_block);
			}
			else;
			p2_current_time = clock();
			if (p2_current_time - p2_start_time >= (p2_interval - 0.05 * p2_score / 10) * CLOCKS_PER_SEC) {
				eraseBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state);
				eraseHelpBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map);
				if (checkBelow(p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map, p2_block)) {
					if (putBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map, p2_block)) {
						drawMatrix(p2_startX, p2_startY, p2_map);
						displayScore(p2_startX, p2_startY, p2_score, "P_2");
						displayScore(p1_startX, p1_startY, p1_score, "P_1");
						_getch();
						_getch();
						_getch();
						return;
					}
					drawMatrix(p2_startX, p2_startY, p2_map);
					addMatrix(p1_startX, p1_startY, p1_map, popMatrix(p2_startX, p2_startY, p2_map, &p2_score));
					drawMatrix(p2_startX, p2_startY, p2_map);
					p2_block = p2_block_next;
					p2_block_next = selectBlock();
					p2_block_state = 0;
					p2_posX = TETRIS_WIDTH / 2, p2_posY = 1;
					/*if (isGameEnd(p2_map)) {
						drawMatrix(p2_startX, p2_startY, p2_map);
						displayScore(p2_score);
						Sleep(1000);
						return;
					}*/
					goto newBlock;
				}
				else {
					p2_start_time = clock();
					p2_posY++;
					drawHelpBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_map, p2_block);
					drawBlock(p2_startX, p2_startY, p2_posX, p2_posY, p2_block_shape, p2_block_state, p2_block);
				}
			}
			p1_current_time = clock();
			if (p1_current_time - p1_start_time >= (p1_interval - 0.05 * p1_score / 10) * CLOCKS_PER_SEC) {
				eraseBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state);
				eraseHelpBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map);
				if (checkBelow(p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map, p1_block)) {
					if (putBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map, p1_block)) {
						drawMatrix(p1_startX, p1_startY, p1_map);
						displayScore(p1_startX, p1_startY, p1_score, "P_1");
						displayScore(p2_startX, p2_startY, p2_score, "P_2");
						_getch();
						_getch();
						_getch();
						return;
					}
					drawMatrix(p1_startX, p1_startY, p1_map);
					addMatrix(p2_startX, p2_startY, p2_map, popMatrix(p1_startX, p1_startY, p1_map, &p1_score));
					drawMatrix(p1_startX, p1_startY, p1_map);
					p1_block = p1_block_next;
					p1_block_next = selectBlock();
					p1_block_state = 0;
					p1_posX = TETRIS_WIDTH / 2, p1_posY = 1;
					goto newBlock;
				}
				else {
					p1_start_time = clock();
					p1_posY++;
					drawHelpBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_map, p1_block);
					drawBlock(p1_startX, p1_startY, p1_posX, p1_posY, p1_block_shape, p1_block_state, p1_block);
				}
			}
		}
	}
}

int main(void) {
	int selected;
	srand(time(NULL));
	removeCursor();
mainMenu:
	system("cls");
	drawScreenMenu(WIDTH, HEIGHT, RED1);
	selected = onMenuFunc();

	switch (selected) {
	case 0:				//Single Play
		system("cls");
		playSinglePlay();
		goto mainMenu;
		break;
	case -1:			//VS COM
		playVsPlayer();
		goto mainMenu;
		break;
	case -2:			//P! VS P2
		//playVsCom();
		goto mainMenu;
		break;
	case 2:				//SCORE
		showScore();
		goto mainMenu;
		break;
	case 3:				//EXIT
		exit(1);
		break;
	}
}


