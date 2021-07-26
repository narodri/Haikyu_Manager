#define WINDOWS_IGNORE_PACKING_MISMATCH
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<stdlib.h>
#include<time.h>

#define MAP_WIDTH 20 // �׵θ��� ���� ����
#define MAP_HEIGHT 29 // �׵θ��� ���� ����
#define MAP_X 12 // �׵θ��� ���۵Ǵ� ��ġ (����)
#define MAP_Y 5 // �׵θ��� ���۵Ǵ� ��ġ (����)
#define MX MAP_X + (MAP_WIDTH / 2) - 10 // �� �� ������ ��ġ��ǥ ����(X)
#define MY MAP_Y + MAP_HEIGHT - 2 // �� �� ������ ��ġ��ǥ ����(Y)
#define YX MAP_X + (MAP_WIDTH / 2) + 7 // ��� �� ������ ��ġ��ǥ ����(X)
#define YY MAP_Y + 1 // ��� �� ������ ��ġ��ǥ ����(Y)

void title(); // ���� ���� ȭ��
void draw_map(); // �׵θ� �׸���
void gotoxy(int, int , char*); // �ش� ��ǥ�� Ŀ���� �̵� + ���� �ֱ�
void reset(); // ����
void reset2(); // ���� + ��Ʈ �׸���
void reset_up(); // �� ��Ʈ ����
void reset_down(); // �Ʒ� ��Ʈ ����
void select_team(); // �� ���� - my, your�� ���� ����, ������� ������ȣ�� �Էµȴ�.
void input_player(int, int); // ����ü �迭 MyTeam[6], YourTeam[6]�� ���������� �Է�(���õ� ���� ���� �� ���Ͽ��� �о�´�.)
void start(); // �����غ�, ��Ʈ�׸���
void entry(int, int); // ��Ʈ ���ʿ� ������� �ۼ�
void rotate_m(); // �� ���� �����̼� �� �� ����
void rotate_y(); // ��� ���� �����̼� �� �� ����
void serve_change(); // ����� ����
void rotation_m(); // �� �� - ����/���� �����̼�
void rotation_y(); // ��� �� - ����/���� �����̼�
void rotation_m_a(); // �� �� - ���� �����̼�
void rotation_y_a(); // ��� �� - ���� �����̼�
void playball(); // �÷��̺�. �� �� ���� �� score_m �� +1, ��� �� ���� �� score_y�� +1
int atk_1m(int, int); // �� �� ����, �������� �����̼� ��ȣ , ���� ���� ���� �Է�
int atk_1y(int, int); // ��� �� ����, �������� �����̼� ��ȣ , ���� ���� ���� �Է�
int atk_2m(int, int); // �� �� ����(��Ʈ, ��� ����), �������� �����̼� ��ȣ , ���� ���� ���� �Է�
int atk_2y(int, int); // ��� �� ����(��Ʈ, ��� ����), �������� �����̼� ��ȣ , ���� ���� ���� �Է�
int vs_m(int, int); // �� vs �� (�� �� ����)
int vs_y(int, int); // �� vs �� (��� �� ����)
int vs2_m(int, int); // �� vs �� (�� �� ����)
int vs2_y(int, int); // �� vs �� (��� �� ����)
void select_atk_m(int); // �� �� ������ ����, �������� �����̼� �Է�
void select_atk_y(int); // ��� �� ������ ���� ����, �������� �����̼� �Է�

int key = 0;
int my, your; // ����, ������� ������ȣ�� ����ȴ�.(select_team���� ������)
int rt_m, rt_y; // ����, ������� �����̼� ��ȣ
int serve; // ���� ���� : 1, ����� ���� : -1
int score_m = 0, score_y = 0; // ���� ����
int set_score_m = 0, set_score_y = 0; // ��Ʈ ���� ����
int atk_name = 0, def_name = 0; // ������, �������� ���� ����
int b1_name, b2_name, b3_name; // ���ŷ 3���� ���� ����


typedef struct player {
	char name[30]; // �̸�
	int num;       // ���ȣ
	char pos;      // ������ (S : ���� , L : ����Ʈ , R : ����Ʈ , C : ���� , I : ������)
	int srv;       // ���� �ɷ�ġ
	int atk;       // ���� �ɷ�ġ
	int rcv;       // ���ú� �ɷ�ġ
	int blc;       // ���ŷ �ɷ�ġ
	int set;       // ��Ʈ �ɷ�ġ
	int stm;       // ü�� �ɷ�ġ (������ �ҷ� ����)
	int rt;        // ����
}pl;

typedef struct team {
	int num;       // �� ������ȣ
	char name[40]; // �� �̸�
	char phrase[100]; // �� ���� �� ��Ʈ
}tm;

struct player MyTeam[7]; // ���� 7���� ������ ������ ����ü �迭
struct player YourTeam[7]; // ����� 7���� ������ ������ ����ü �迭

// ���� �߰��� �� ���� ������Ʈ �ʿ�
/************************************************************************/
#define team_number 2
struct team T[team_number] = {
	{ 1, "ī�󽺳�", "���ƶ�, ī�󽺳�!" },
	{ 2, "�ƿ����һ���", "��Ʈ�� �����϶�, �ƿ����һ���!" }
};
/************************************************************************/

void main() {
	srand(time(NULL));
	title();
	select_team();
	input_player(my, your);
	start();
	entry(my, your);
	reset_up(); reset_down();
	rt_m = 1; rt_y = 1;
	serve = 1;
	rotation_m();
	rotation_y();
	while(score_m != 15 && score_y != 15) {
		playball();
	}
	
}

void gotoxy(int x, int y, char* s) { //x���� 2x�� ����, ��ǥ���� �ٷ� ���ڿ��� �Է��� �� �ֵ��� printf�Լ� ����  
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);
}
void title() {
	draw_map();
	int i, j;
	for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) { // �� �׵θ� ������ ��ĭ���� ä�� 
		for (j = MAP_X + 1; j < MAP_X + MAP_WIDTH - 1; j++) 
			gotoxy(j, i, "  ");
	}
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 10, "+-----------------------------+");
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 11, "|       H A I K Y U  !!       |");
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 12, "|        M A N A G E R        |");
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 13, "+-----------------------------+");
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 16, "   < PRESS ANY KEY TO START >  ");
	while(key != 1){
		key = kbhit();
		gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 16, "   < PRESS ANY KEY TO START >  ");
		Sleep(300);
		gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 16, "                               ");
		Sleep(300);
	}
	while (kbhit()) getch();
	reset();
}
void draw_map() {
	int i;
	for (i = 0; i < MAP_WIDTH; i++) {
		gotoxy(MAP_X + i, MAP_Y, "��");
	}
	for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) {
		gotoxy(MAP_X, i, "��");
		gotoxy(MAP_X + MAP_WIDTH - 1, i, "��");
	}
	for (i = 0; i < MAP_WIDTH; i++) {
		gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "��");
	}
}
void reset() {
	system("cls");
	draw_map(); 
}
void reset2() {
	system("cls");
	draw_map();
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y + 14, "������������������������������������������������������������������������");
}
void reset_up() {
	int i, j;
	for (i = 0; i < 13; i++) {
		for (j = 0; j < 18; j++) {
			gotoxy(MAP_X + 1 + j, MAP_Y + 1 + i, "  ");
		}
	}
}
void reset_down() {
	int i, j;
	for (i = 0; i < 13; i++) {
		for (j = 0; j < 18; j++) {
			gotoxy(MAP_X + 1 + j, MAP_Y + 15 + i, "  ");
		}
	}
}
void select_team() {
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 4, "   <  T E A M   L I S T  >   ");
	int i;
	for (i = 0; i < team_number; i++) {  // �� ��� �����ֱ�
		gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 6 + i * 2, " ");
		printf("%d. %s", T[i].num, T[i].name);
		gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 6 + i * 2 + 1, " ");
	}
	gotoxy(MAP_X + (MAP_WIDTH / 2) + 11, MAP_Y + 5, "�� ���� �����ϼ���(���ڼ��� �� ����) : ");
	scanf_s("%d",&my);
	gotoxy(MAP_X + (MAP_WIDTH / 2) + 11, MAP_Y + 6, " ");
	printf("%s", T[my - 1].phrase);
	Sleep(1000);
	gotoxy(MAP_X + (MAP_WIDTH / 2) + 11, MAP_Y + 8, "��� ���� �����ϼ���(���ڼ��� �� ����) : ");
	scanf_s("%d", &your);
	gotoxy(MAP_X + (MAP_WIDTH / 2) + 11, MAP_Y + 9, " ");
	printf("%s", T[your - 1].phrase);
	Sleep(1000);
}
// ���� �߰��� �� ���� ������Ʈ �ʿ�
void input_player(int a, int b) { // a:���� ������ȣ, b:����� ������ȣ
	FILE* f1 = NULL, * f2 = NULL;
	int i;
	switch (a) {
	case 1:
		f1 = fopen("KRS.txt", "r");
		break;
	case 2:
		f1 = fopen("ABJ.txt", "r");
		break;
	}
	if (f1 == NULL) { printf("error\n"); exit(0); }
	for (i = 0; i < 7; i++) {
		fscanf(f1, "%s %d %c %d %d %d %d %d %d %d", &MyTeam[i].name, &MyTeam[i].num, &MyTeam[i].pos,
			&MyTeam[i].srv, &MyTeam[i].atk, &MyTeam[i].rcv, &MyTeam[i].blc, &MyTeam[i].set, &MyTeam[i].stm, &MyTeam[i].rt);
	}
	switch (b) {
	case 1:
		f2 = fopen("KRS.txt", "r");
		break;
	case 2:
		f2 = fopen("ABJ.txt", "r");
		break;
	}
	for (i = 0; i < 7; i++) {
		fscanf(f2, "%s %d %c %d %d %d %d %d %d %d", &YourTeam[i].name, &YourTeam[i].num, &YourTeam[i].pos,
			&YourTeam[i].srv, &YourTeam[i].atk, &YourTeam[i].rcv, &YourTeam[i].blc, &YourTeam[i].set, &YourTeam[i].stm, &YourTeam[i].rt);
	}
	if (f2 == NULL) { printf("error\n"); exit(0); }
	fclose(f1); fclose(f2);
}
void start() {
	reset();
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y + 14, "������������������������������������������������������������������������");
	Sleep(1000);
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 19, "�� �� : ");
	Sleep(1000);
	printf("%s", T[my - 1].name);
	Sleep(1000);
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 9, "��� �� : ");
	Sleep(1000);
	printf("%s", T[your - 1].name);
	Sleep(1000);
	gotoxy(MAP_X + (MAP_WIDTH / 2) + 11, MAP_Y + 14, "GAME START !!");
	Sleep(2000);
	reset2();
}
void entry(a, b) { // a:���� ������ȣ, b:����� ������ȣ
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 21, MAP_Y + 17, ""); printf("< %s �� >", T[a-1].name);
	gotoxy(MAP_X + (MAP_WIDTH / 2) - 21, MAP_Y + 4, ""); printf("< %s �� >", T[b-1].name);
	Sleep(1000);
	int i;
	for (i = 0; i < 7; i++) {
		gotoxy(MAP_X + (MAP_WIDTH / 2) - 21, MAP_Y + 19 + i, ""); printf("%2d %s", MyTeam[i].num, MyTeam[i].name);
		gotoxy(MAP_X + (MAP_WIDTH / 2) - 21, MAP_Y + 6 + i, ""); printf("%2d %s", YourTeam[i].num, YourTeam[i].name);
		Sleep(300);
	}
}
void rotate_m() {
	if (rt_m == 6) rt_m = 1;
	else rt_m += 1;
}
void rotate_y() {
	if (rt_y == 6) rt_y = 1;
	else rt_y += 1;
}
void serve_change() {
	serve = serve * -1;
}
void rotation_m() {
	switch (rt_m) {
	case 1:
		gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos);
		gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos); gotoxy(MX + 3, MY - 9, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos);
		gotoxy(MX + 8, MY - 9, ""); printf("%2d(%c)", MyTeam[2].num, MyTeam[2].pos); gotoxy(MX + 14, MY - 9, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos);
		break;
	case 2:
		gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos);
		gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos); gotoxy(MX + 3, MY - 9, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos);
		gotoxy(MX + 8, MY - 9, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos); gotoxy(MX + 14, MY - 9, ""); printf("%2d(%c)", MyTeam[2].num, MyTeam[2].pos);
		break;
	case 3:
		if (serve == 1) {
			gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[2].num, MyTeam[2].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos);
			gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos); gotoxy(MX + 3, MY - 9, ""); printf("%2d(%c)", MyTeam[5].num, MyTeam[5].pos);
			gotoxy(MX + 8, MY - 9, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos); gotoxy(MX + 14, MY - 9, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos);
		}
		else if (serve == -1) {
			gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos);
			gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos); gotoxy(MX + 3, MY - 9, ""); printf("%2d(%c)", MyTeam[5].num, MyTeam[5].pos);
			gotoxy(MX + 8, MY - 9, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos); gotoxy(MX + 14, MY - 9, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos);
		}
		break;
	case 4:
		gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos);
		gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos); gotoxy(MX + 3, MY - 9, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos);
		gotoxy(MX + 8, MY - 9, ""); printf("%2d(%c)", MyTeam[5].num, MyTeam[5].pos); gotoxy(MX + 14, MY - 9, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos);
		break;
	case 5:
		gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos);
		gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos); gotoxy(MX + 3, MY - 9, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos);
		gotoxy(MX + 8, MY - 9, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos); gotoxy(MX + 14, MY - 9, ""); printf("%2d(%c)", MyTeam[5].num, MyTeam[5].pos);
		break;
	case 6:
		if (serve == 1) {
			gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[5].num, MyTeam[5].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos);
			gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos); gotoxy(MX + 3, MY - 9, ""); printf("%2d(%c)", MyTeam[2].num, MyTeam[2].pos);
			gotoxy(MX + 8, MY - 9, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos); gotoxy(MX + 14, MY - 9, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos);
		}
		else if (serve == -1) {
			gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos);
			gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos); gotoxy(MX + 3, MY - 9, ""); printf("%2d(%c)", MyTeam[2].num, MyTeam[2].pos);
			gotoxy(MX + 8, MY - 9, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos); gotoxy(MX + 14, MY - 9, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos);
		}
		break;
	}
}
void rotation_y() {
	switch (rt_y) {
	case 1:
		gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[6].pos);
		gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos); gotoxy(YX - 3, YY + 9, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos);
		gotoxy(YX - 8, YY + 9, ""); printf("%2d(%c)", YourTeam[2].num, YourTeam[2].pos); gotoxy(YX - 14, YY + 9, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos);
		break;
	case 2:
		gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos);
		gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[6].pos); gotoxy(YX - 3, YY + 9, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos);
		gotoxy(YX - 8, YY + 9, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos); gotoxy(YX - 14, YY + 9, ""); printf("%2d(%c)", YourTeam[2].num, YourTeam[2].pos);
		break;
	case 3:
		if (serve == -1) {
			gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[2].num, YourTeam[2].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos);
			gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos); gotoxy(YX - 3, YY + 9, ""); printf("%2d(%c)", YourTeam[5].num, YourTeam[5].pos);
			gotoxy(YX - 8, YY + 9, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos); gotoxy(YX - 14, YY + 9, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos);
		}
		else if (serve == 1) {
			gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[5].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos);
			gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos); gotoxy(YX - 3, YY + 9, ""); printf("%2d(%c)", YourTeam[5].num, YourTeam[3].pos);
			gotoxy(YX - 8, YY + 9, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos); gotoxy(YX - 14, YY + 9, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[5].pos);
		}
		break;
	case 4:
		gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[6].pos);
		gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos); gotoxy(YX - 3, YY + 9, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos);
		gotoxy(YX - 8, YY + 9, ""); printf("%2d(%c)", YourTeam[5].num, YourTeam[5].pos); gotoxy(YX - 14, YY + 9, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos);
		break;
	case 5:
		gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos);
		gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[6].pos); gotoxy(YX - 3, YY + 9, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos);
		gotoxy(YX - 8, YY + 9, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos); gotoxy(YX - 14, YY + 9, ""); printf("%2d(%c)", YourTeam[5].num, YourTeam[5].pos);
		break;
	case 6:
		if (serve == -1) {
			gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[5].num, YourTeam[5].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos);
			gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos); gotoxy(YX - 3, YY + 9, ""); printf("%2d(%c)", YourTeam[2].num, YourTeam[2].pos);
			gotoxy(YX - 8, YY + 9, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos); gotoxy(YX - 14, YY + 9, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos);
		}
		else if (serve == 1) {
			gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[6].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos);
			gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos); gotoxy(YX - 3, YY + 9, ""); printf("%2d(%c)", YourTeam[2].num, YourTeam[2].pos);
			gotoxy(YX - 8, YY + 9, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos); gotoxy(YX - 14, YY + 9, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos);
		}
		break;
	}

}
void rotation_m_a() {
	switch (rt_m) {
	case 1:
		gotoxy(MX + 8, MY - 11, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos); gotoxy(MX + 10, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos);
		gotoxy(MX + 4, MY - 3, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos); gotoxy(MX + 2, MY - 10, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos);
		gotoxy(MX + 11, MY - 11, ""); printf("%2d(%c)", MyTeam[2].num, MyTeam[2].pos); gotoxy(MX + 15, MY - 10, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos);
		break;
	case 2:
		gotoxy(MX + 10, MY - 3, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos); gotoxy(MX + 8, MY - 11, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos);
		gotoxy(MX + 4, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos); gotoxy(MX + 2, MY - 10, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos);
		gotoxy(MX + 15, MY - 10, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos); gotoxy(MX + 11, MY - 11, ""); printf("%2d(%c)", MyTeam[2].num, MyTeam[2].pos);
		break;
	case 3:
		if (serve == 1) {
			gotoxy(MX + 10, MY - 3, ""); printf("%2d(%c)", MyTeam[2].num, MyTeam[2].pos); gotoxy(MX + 4, MY - 3, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos);
			gotoxy(MX + 8, MY - 11, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos); gotoxy(MX + 11, MY - 11, ""); printf("%2d(%c)", MyTeam[5].num, MyTeam[5].pos);
			gotoxy(MX + 2, MY - 10, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos); gotoxy(MX + 15, MY - 10, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos);
		}
		else if (serve == -1) {
			gotoxy(MX + 10, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos); gotoxy(MX + 4, MY - 3, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos);
			gotoxy(MX + 8, MY - 11, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos); gotoxy(MX + 11, MY - 11, ""); printf("%2d(%c)", MyTeam[5].num, MyTeam[5].pos);
			gotoxy(MX + 2, MY - 10, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos); gotoxy(MX + 15, MY - 10, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos);
		}
		break;
	case 4:
		gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos);
		gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos); gotoxy(MX + 8, MY - 11, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos);
		gotoxy(MX + 11, MY - 11, ""); printf("%2d(%c)", MyTeam[5].num, MyTeam[5].pos); gotoxy(MX + 2, MY - 10, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos);
		break;
	case 5:
		gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos);
		gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos); gotoxy(MX + 2, MY - 10, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos);
		gotoxy(MX + 8, MY - 11, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos); gotoxy(MX + 11, MY - 11, ""); printf("%2d(%c)", MyTeam[5].num, MyTeam[5].pos);
		break;
	case 6:
		if (serve == 1) {
			gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[5].num, MyTeam[5].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos);
			gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos); gotoxy(MX + 11, MY - 11, ""); printf("%2d(%c)", MyTeam[2].num, MyTeam[2].pos);
			gotoxy(MX + 2, MY - 10, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos); gotoxy(MX + 8, MY - 11, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos);
		}
		else if (serve == -1) {
			gotoxy(MX + 14, MY - 3, ""); printf("%2d(%c)", MyTeam[6].num, MyTeam[6].pos); gotoxy(MX + 8, MY - 3, ""); printf("%2d(%c)", MyTeam[4].num, MyTeam[4].pos);
			gotoxy(MX + 3, MY - 3, ""); printf("%2d(%c)", MyTeam[3].num, MyTeam[3].pos); gotoxy(MX + 11, MY - 11, ""); printf("%2d(%c)", MyTeam[2].num, MyTeam[2].pos);
			gotoxy(MX + 2, MY - 10, ""); printf("%2d(%c)", MyTeam[1].num, MyTeam[1].pos); gotoxy(MX + 8, MY - 11, ""); printf("%2d(%c)", MyTeam[0].num, MyTeam[0].pos);
		}
		break;
	}
}
void rotation_y_a() {
	switch (rt_y) {
	case 1:
		gotoxy(YX - 8, YY + 11, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos); gotoxy(YX - 10, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[6].pos);
		gotoxy(YX - 4, YY + 3, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos); gotoxy(YX - 2, YY + 10, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos);
		gotoxy(YX - 11, YY + 11, ""); printf("%2d(%c)", YourTeam[2].num, YourTeam[2].pos); gotoxy(YX - 15, YY + 10, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos);
		break;
	case 2:
		gotoxy(YX - 10, YY + 3, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos); gotoxy(YX - 8, YY + 11, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos);
		gotoxy(YX - 4, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[6].pos); gotoxy(YX - 2, YY + 10, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos);
		gotoxy(YX - 15, YY + 10, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos); gotoxy(YX - 11, YY + 11, ""); printf("%2d(%c)", YourTeam[2].num, YourTeam[2].pos);
		break;
	case 3:
		if (serve == -1) {
			gotoxy(YX - 10, YY + 3, ""); printf("%2d(%c)", YourTeam[2].num, YourTeam[2].pos); gotoxy(YX - 4, YY + 3, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos);
			gotoxy(YX - 8, YY + 11, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos); gotoxy(YX - 11, YY + 11, ""); printf("%2d(%c)", YourTeam[5].num, YourTeam[5].pos);
			gotoxy(YX - 2, YY + 10, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos); gotoxy(YX - 15, YY + 10, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos);
		}
		else if (serve == 1) {
			gotoxy(YX - 10, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[6].pos); gotoxy(YX - 4, YY + 3, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos);
			gotoxy(YX - 8, YY + 11, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos); gotoxy(YX - 11, YY + 11, ""); printf("%2d(%c)", YourTeam[5].num, YourTeam[5].pos);
			gotoxy(YX - 2, YY + 10, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos); gotoxy(YX - 15, YY + 10, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos);
		}
		break;
	case 4:
		gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[6].pos);
		gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos); gotoxy(YX - 8, YY + 11, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos);
		gotoxy(YX - 11, YY + 11, ""); printf("%2d(%c)", YourTeam[5].num, YourTeam[5].pos); gotoxy(YX - 2, YY + 10, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos);
		break;
	case 5:
		gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos);
		gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[6].pos); gotoxy(YX - 2, YY + 10, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos);
		gotoxy(YX - 8, YY + 11, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos); gotoxy(YX - 11, YY + 11, ""); printf("%2d(%c)", YourTeam[5].num, YourTeam[5].pos);
		break;
	case 6:
		if (serve == -1) {
			gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[5].num, YourTeam[5].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos);
			gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos); gotoxy(YX - 11, YY + 11, ""); printf("%2d(%c)", YourTeam[2].num, YourTeam[2].pos);
			gotoxy(YX - 2, YY + 10, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos); gotoxy(YX - 8, YY + 11, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos);
		}
		else if (serve == 1) {
			gotoxy(YX - 14, YY + 3, ""); printf("%2d(%c)", YourTeam[6].num, YourTeam[6].pos); gotoxy(YX - 8, YY + 3, ""); printf("%2d(%c)", YourTeam[4].num, YourTeam[4].pos);
			gotoxy(YX - 3, YY + 3, ""); printf("%2d(%c)", YourTeam[3].num, YourTeam[3].pos); gotoxy(YX - 11, YY + 11, ""); printf("%2d(%c)", YourTeam[2].num, YourTeam[2].pos);
			gotoxy(YX - 2, YY + 10, ""); printf("%2d(%c)", YourTeam[1].num, YourTeam[1].pos); gotoxy(YX - 8, YY + 11, ""); printf("%2d(%c)", YourTeam[0].num, YourTeam[0].pos);
		}
		break;
	}
}
void playball() {
	int state = 0;
	if (serve == 1) {
		reset_up(); reset_down();
		rotation_m(); rotation_y();
		atk_name = rt_m - 1;
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ����.", &MyTeam[atk_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                      ");
		state = atk_1m(rt_y, rt_m - 1);
	}
	else if (serve == -1) {
		reset_up(); reset_down();
		rotation_m(); rotation_y();
		atk_name = rt_y - 1;
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ����.", &YourTeam[atk_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                      ");
		state = atk_1y(rt_m, rt_y - 1);
	}

	while(state != 1 && state != -1) {
		switch (state) {
		case 2:
			reset_down();  rotation_m_a();
			select_atk_m(rt_m); state = atk_2m(rt_y, atk_name);
			break;
		case -2:
			reset_up();  rotation_y_a();
			select_atk_y(rt_y); state = atk_2y(rt_m, atk_name);
			break;
		}
	}
	if (state == 1) {
		score_m += 1;
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 18, ""); printf("%2d", score_m); Sleep(3000);
		if(serve == -1)
			rotate_m();
		serve = 1;		
	}	
	else if (state == -1) {
		score_y += 1;
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 10, ""); printf("%2d", score_y); Sleep(3000);
		if (serve == 1)
			rotate_y();
		serve = -1;
	}
}
int atk_1m(int b, int atk) { // b : �޴� �� �����̼� , atk : �������� ����
	int c, def; // def : �������� ����
	atk_name = atk;
	srand(time(NULL));
	switch (b) {
	case 1:
		c = rand() % 2;
		switch (c) {
		case 0:
			def = 4; break;
		case 1:
			def = 6; break;
		}
		def_name = def;
		return vs_m(atk, def);
	case 2:
		c = rand() % 2;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 1; break;
		}
		def_name = def;
		return vs_m(atk, def);
	case 3:
		c = rand() % 2;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 1; break;
		}
		def_name = def;
		return vs_m(atk, def);
	case 4:
		c = rand() % 3;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 1; break;
		case 2:
			def = 3; break;
		}
		def_name = def;
		return vs_m(atk, def);
	case 5:
		c = rand() % 3;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 4; break;
		case 2:
			def = 3; break;
		}
		def_name = def;
		return vs_m(atk, def);
	case 6:
		c = rand() % 3;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 3; break;
		case 2:
			def = 4; break;
		}
		def_name = def;
		return vs_m(atk, def);
	}
}
int atk_1y(int b, int atk) { // b : �޴� �� �����̼� , atk : �������� ����
	int c, def; // def : �������� ����
	atk_name = atk;
	srand(time(NULL));
	switch (b) {
	case 1:
		c = rand() % 2;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 4; break;
		}
		def_name = def;
		return vs_y(atk, def);
	case 2:
		c = rand() % 2;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 1; break;
		}
		def_name = def;
		return vs_y(atk, def);
	case 3:
		c = rand() % 2;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 1; break;
		}
		def_name = def;
		return vs_y(atk, def);
	case 4:
		c = rand() % 3;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 1; break;
		case 2:
			def = 3; break;
		}
		def_name = def;
		return vs_y(atk, def);
	case 5:
		c = rand() % 3;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 4; break;
		case 2:
			def = 3; break;
		}
		def_name = def;
		return vs_y(atk, def);
	case 6:
		c = rand() % 3;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 3; break;
		case 2:
			def = 4; break;
		}
		def_name = def;
		return vs_y(atk, def);
	}
}
int atk_2m(int b, int atk) { // b : �޴� �� �����̼� , atk : �������� ����
	int c, def; // def : ������(���ú�)�� ����
	atk_name = atk;
	srand(time(NULL));
	switch (b) {
	case 1:
		c = rand() % 2;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 4; break;
		}
		def_name = def;
		b1_name =1, b2_name =2, b3_name = 3;
		return vs2_m(atk, def);
	case 2:
		c = rand() % 2;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 1; break;
		}
		def_name = def;
		b1_name =2, b2_name =3, b3_name = 4;
		return vs2_m(atk, def);
	case 3:
		c = rand() % 2;
		if (serve == 1) {
			switch (c) {
			case 0:
				def = 6; break;
			case 1:
				def = 1; break;
			}
		}
		else {
			switch (c) {
			case 0:
				def = 2; break;
			case 1:
				def = 1; break;
			}
		}
		def_name = def;
		b1_name =3, b2_name =4, b3_name = 5;
		return vs2_m(atk, def);
	case 4:
		c = rand() % 3;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 1; break;
		case 2:
			def = 3; break;
		}
		def_name = def;
		b1_name =4, b2_name =5, b3_name = 0;
		return vs2_m(atk, def);
	case 5:
		c = rand() % 3;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 4; break;
		case 2:
			def = 3; break;
		}
		def_name = def;
		b1_name =5, b2_name =0, b3_name = 1;
		return vs2_m(atk, def);
	case 6:
		c = rand() % 3;
		if (serve == 1) {
			switch (c) {
			case 0:
				def = 6; break;
			case 1:
				def = 3; break;
			case 2:
				def = 4; break;
			}
		}
		else {
			switch (c) {
			case 0:
				def = 5; break;
			case 1:
				def = 3; break;
			case 2:
				def = 4; break;
			}
		}
		def_name = def;
		b1_name =0, b2_name =1, b3_name = 2;
		return vs2_m(atk, def);
	}
}
int atk_2y(int b, int atk) { // b : �޴� �� �����̼� , atk : �������� ����
	int c, def; // def : ������(���ú�)�� ����
	atk_name = atk;
	srand(time(NULL));
	switch (b) {
	case 1:
		c = rand() % 2;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 4; break;
		}
		def_name = def;
		b1_name = 1, b2_name = 2, b3_name = 3;
		return vs2_y(atk, def);
	case 2:
		c = rand() % 2;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 1; break;
		}
		def_name = def;
		b1_name = 2, b2_name = 3, b3_name = 4;
		return vs2_y(atk, def);
	case 3:
		c = rand() % 2;
		if (serve == 1) {
			switch (c) {
			case 0:
				def = 6; break;
			case 1:
				def = 1; break;
			}
		}
		else {
			switch (c) {
			case 0:
				def = 2; break;
			case 1:
				def = 1; break;
			}
		}
		def_name = def;
		b1_name = 3, b2_name = 4, b3_name = 5;
		return vs2_y(atk, def);
	case 4:
		c = rand() % 3;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 1; break;
		case 2:
			def = 3; break;
		}
		def_name = def;
		b1_name = 4, b2_name = 5, b3_name = 0;
		return vs2_y(atk, def);
	case 5:
		c = rand() % 3;
		switch (c) {
		case 0:
			def = 6; break;
		case 1:
			def = 4; break;
		case 2:
			def = 3; break;
		}
		def_name = def;
		b1_name = 5, b2_name = 0, b3_name = 1;
		return vs2_y(atk, def);
	case 6:
		c = rand() % 3;
		if (serve == 1) {
			switch (c) {
			case 0:
				def = 6; break;
			case 1:
				def = 3; break;
			case 2:
				def = 4; break;
			}
		}
		else {
			switch (c) {
			case 0:
				def = 5; break;
			case 1:
				def = 3; break;
			case 2:
				def = 4; break;
			}
		}
		def_name = def;
		b1_name = 0, b2_name = 1, b3_name = 2;
		return vs2_y(atk, def);
	}
}
int vs_m(int a, int b) { // a : �� �� �������� ����, b : ��� �� �������� ����
	srand(time(NULL));
	int num = rand() % 100;
	if (num < MyTeam[a].srv - YourTeam[b].rcv + 10) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ���꿡�̽�! ", &MyTeam[atk_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		return 1; // �� �� ����!
	}
	else if (num < MyTeam[a].srv - YourTeam[b].rcv + 25) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� �Ҿ��� ���ú�, ���ƿ´�! ������! ", &YourTeam[def_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                                            ");
		reset_down(); rotation_m_a();
		return 2; // �� �� ������!
	}
	else if (num < MyTeam[a].srv - YourTeam[b].rcv + 35) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ����̽�.", &MyTeam[atk_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		return -1; // �� �� ����̽�
	}
	else {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s, �Ϻ��� �޾Ƴ´�..!", &YourTeam[def_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		return -2; // ��� ���� ���ú� ����
	}
}
int vs_y(int a, int b) { // a : ��� �� �������� ����, b : �� �� �������� ����
	srand(time(NULL));
	int num = rand() % 100;
	if (num < YourTeam[a].atk - MyTeam[b].rcv + 10) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ���꿡�̽�! ", &YourTeam[atk_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		return -1; // ��� �� ����!
	}
	else if (num < YourTeam[a].atk - MyTeam[b].rcv + 25) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� �Ҿ��� ���ú�, ��Ʈ�� �Ѿ� ��� ���� ������! ", &MyTeam[def_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                                                     ");
		reset_up(); rotation_y_a();
		return -2; // ��� �� ������!
	}
	else if (num < MyTeam[a].atk - YourTeam[b].rcv + 35) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ����̽�.", &YourTeam[atk_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		return 1; // ��� �� ����̽�
	}
	else {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s, �Ϻ��� �޾Ƴ´�..!", &MyTeam[def_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		return 2; // �� ���� ���ú� ����
	}
}
int vs2_m(int a, int b) { // a : �� �� �������� ����, b : ��� �� �������� ����
	srand(time(NULL));
	int num = rand() % 100;
	if (num < MyTeam[0].set + MyTeam[a].atk - YourTeam[b].rcv - (YourTeam[b1_name].blc+ YourTeam[b3_name].blc+ YourTeam[b2_name].blc)/3 + 50) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ����! ", &MyTeam[atk_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		return 1; // �� �� ����!
	}
	else if (num < MyTeam[0].set + MyTeam[a].atk - YourTeam[b].rcv - (YourTeam[b1_name].blc + YourTeam[b3_name].blc + YourTeam[b2_name].blc) / 3 + 70) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� �Ҿ��� ���ú�, ���ƿ´�! ������! ", &YourTeam[def_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                                         ");
		reset_up(); rotation_y();
		return 2; // �� �� ������!
	}
	else if (num < MyTeam[0].set + MyTeam[a].atk - YourTeam[b].rcv - (YourTeam[b1_name].blc + YourTeam[b3_name].blc + YourTeam[b2_name].blc) / 3 + 90) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ��� ����!", &YourTeam[def_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		return -2; // ��� ���� ���ú� ����!
	}
	else {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ���ŷ, �˾ƿ�..!", &YourTeam[b2_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		reset_down(); rotation_m();
		return -1; // ��� ���� ���ŷ ����!
	}
}
int vs2_y(int a, int b) { // a : ��� �� �������� ����, b : �� �� �������� ����
	srand(time(NULL));
	int num = rand() % 100;
	if (num < YourTeam[0].set + YourTeam[a].atk - MyTeam[b].rcv - MyTeam[b].blc + 50) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ����! ", &YourTeam[atk_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		return -1; // ��� �� ����!
	}
	else if (num < YourTeam[0].set + YourTeam[a].atk - MyTeam[b].rcv - MyTeam[b].blc + 70) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ���ú�, ������ ��Ʈ�� �Ѿ� ����� ������! ", &MyTeam[def_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                                               ");
		reset_down(); rotation_m();
		return -2; // ��� �� ������!
	}
	else if (num < YourTeam[0].set + YourTeam[a].atk - MyTeam[b].rcv - MyTeam[b].blc + 90) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ��� ����!! ��������!", &MyTeam[def_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		return 2; // �� ���� ���ú� ����!
	}
	else {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� �˾ƿ�! ���ŷ ����!!", &MyTeam[b2_name].name); Sleep(3000);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
		reset_up(); rotation_y();
		return 1; // �� ���� ���ŷ ����!
	}
}
void select_atk_m(int rt) { // ������ ������, ������ �����, ��������display�� ���Խ�Ű���� ������Ʈ �ʿ�
	int a, b = -1, i;
	if (rt < 3 || rt == 6) {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "���� ������ ���� ��ȣ : "); printf("%d %d %d %d                        ", MyTeam[1].num, MyTeam[2].num, MyTeam[3].num, MyTeam[4].num);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 15, "��ȣ �Է� �� ���� : "); scanf_s("%d", &a);
	}
	else {
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "���� ������ ���� ��ȣ : "); printf("%d %d %d %d                        ", MyTeam[1].num, MyTeam[5].num, MyTeam[3].num, MyTeam[4].num);
		gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 15, "��ȣ �Է� �� ���� : "); scanf_s("%d", &a);
	}
	for (i = 0; i < 6; i++) {
		if (MyTeam[i].num == a) 
			b = i;
	}
	atk_name = b;
	Sleep(1000);
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ����!", MyTeam[atk_name].name);
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 15, "                                         ");
	Sleep(3000);
}
void select_atk_y(int rt) {
	int a, i;
	srand(time(NULL));
	a = rand() % 4 + 1;
	if (rt > 2 && rt < 6) {
		if (a == 2)
			a = 5;
	}
	atk_name = a;
	Sleep(3000);
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, "                                         ");
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 14, ""); printf("%s�� ����!", YourTeam[atk_name].name);
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 15, "                                         ");
	Sleep(3000);
}