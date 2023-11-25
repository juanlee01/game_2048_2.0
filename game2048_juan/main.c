//게임2048
//이현규 timer 추가 버전

//2.1 랜덤생성숫자 추가.
//2.2 메모장 이름 시간 저장.
//2.3 메모장 이름 시간 불러옴, 게임 종료 순위 정렬하여 후 저장.
//2.4 순위표 출력 함수 작성.
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

typedef struct rank
{
	char name[20];
	//int rankNumber;
	double timeRecord;
}RANK;

time_t start_time;
double get_current_time();
void start_timer();
double elapsed_time();

void random_set();
void print_2048();
void print_board();
void print_Rank(RANK arr[]);
void gotoxy(int x, int y);

int check_gameover();
int check_2048();

void slide_board(int direction);
void board_sort(int direction);
void board_add(int* array1, int* array2);

void Bubble_Sort(RANK arr[]);
void Stuctrank_Reset(RANK arr[]);
void save_Rank(FILE* RT, RANK arr[]);

#define SIZE 4		//보드크기
#define XBOARD 15	//보드위치 X,Y
#define YBOARD 5	

#define UP 72		//방향키 입력값
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#define ESC 27

//보드 배열
int board[SIZE][SIZE] = {
	{1024,1024,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};

int main()
{
	//파일 포인터 선언
	FILE* ranktxt;

	char name[100];
	int key = 0;
	double end_time = 0;
	RANK ranking[12];
	
	//랭크 파일 오픈
	//파일이 끝날 때까지 랭킹 구조체에 읽어옴.
	ranktxt = fopen("ranking.txt", "r");
	Stuctrank_Reset(ranking);

	int i = 0;
	for (int i = 1; i < 12; i++)
	{
		fscanf(ranktxt, "%s %lf\n", ranking[i].name, &ranking[i].timeRecord);
	}
	fclose(ranktxt);
	
	//랭킹 출력
	system("cls");
	print_2048();
	print_Rank(ranking);
	gotoxy(XBOARD - 4, YBOARD + 13);
	printf("시작하려면 아무키나 눌러주세요.");
	while (!_kbhit())
	{

	}
	//게임 시작 셋팅
	random_set();
	random_set();
	system("cls");
	print_board();

	start_timer();//타이머 시작

	
	_getch();
	while (1)
	{
		key = _getch();
		//ESC이면 게임 종료
		if (key == ESC)
			break;

		//방향키 확인
		key = _getch();
		switch (key)
		{
		case LEFT:
			slide_board(LEFT);
			break;
		case RIGHT:
			slide_board(RIGHT);
			break;
		case UP:
			slide_board(UP);
			break;
		case DOWN:
			slide_board(DOWN);
			break;
		default:
			break;
		}
		random_set();
		system("cls"); //다음보드 출력을 위한 콘솔 지움
		print_2048();
		print_board();
		gotoxy(XBOARD, YBOARD+10);
		printf("%.2f sec\n", elapsed_time());
		//게임종료확인
		if (check_gameover() == 1)
		{
			end_time = elapsed_time();
			printf("G A M E O V E R!\n\n");
			printf("종료하려면 ESC를 눌러주세요. ");
		}
		if (check_2048() == 1)
		{
			ranktxt = fopen("ranking.txt", "w");
			end_time = elapsed_time();
			ranking[11].timeRecord = end_time; 
			gotoxy(XBOARD, YBOARD + 11);
			printf("congratulations! 2048!\n");
			gotoxy(XBOARD, YBOARD + 12);
			printf("랭킹에 저장할 이름을 입력해주세요 >> ");
			gets(ranking[11].name);
			

			//fprintf(ranktxt, "%s %f", name, end_time);
			Bubble_Sort(ranking);
			//fputs(name, ranktxt);
			system("cls"); 
			print_2048();
			print_Rank(ranking);
			save_Rank(ranktxt, ranking);
			gotoxy(XBOARD-4, YBOARD + 13);
			printf("종료하려면 ESC를 눌러주세요. ");
			fclose(ranktxt);
		}
		
		
	}
	
}
 
//게임 이름 출력 함수
void print_2048()
{
	
	gotoxy(XBOARD + 6, 2);
	printf("┌─┐┌─┐│ │┌─┐\n");
	gotoxy(XBOARD + 6, 3);
	printf("┌─┘│ │└─┤├─┤\n");
	gotoxy(XBOARD + 6, 4);
	printf("└──└─┘  │└─┘\n");
}
//보드를 출력해주는 함수
void print_board()
{
	int z = 1;
	print_2048();
	//보드 출력
	gotoxy(XBOARD, YBOARD);
	printf("┌─────┬─────┬─────┬─────┐\n");
	for (int i = 0; i < 4; i++)
	{
		gotoxy(XBOARD, YBOARD + z);
		for (int j = 0; j < 4; j++)
		{

			if (board[i][j] == 0)
				printf("│  ·  ");
			else
				printf("│ %4d", board[i][j]);
		}
		printf("│");
		z += 1;
		if (i == 3)
		{
			gotoxy(XBOARD, YBOARD + z);
			printf("└─────┴─────┴─────┴─────┘");
			printf("\n");
			break;
		}
		gotoxy(XBOARD, YBOARD + z);
		printf("├─────┼─────┼─────┼─────┤\n");
		z += 1;
	}
}
//랭킹 출력해주는 함수
void print_Rank(RANK arr[])
{
	gotoxy(XBOARD-4, YBOARD);
	printf("┌────────────────────────────────┐\n");
	for (int i = 1; i < 11; i++)
	{
		if (arr[i].timeRecord != 9999999999999999999)
		{
			gotoxy(XBOARD-4, YBOARD+i);
			printf("│ %2d# %10s %10.2lf min  │\n", i, arr[i].name, arr[i].timeRecord/60);
		}
		else
		{
			gotoxy(XBOARD-4, YBOARD + i);
			printf("│                                │\n");
		}
	}
	gotoxy(XBOARD-4, YBOARD + 11);
	printf("└────────────────────────────────┘\n");
}

//보드에 2, 4를 랜덤한 위치에 넣어주는 함수
void random_set()
{
	int width = 0;
	int length = 0;
	int check_0 = 0;
	while (1)
	{
		//랜덤으로 넣기 전에 보드가 다 차있는지 확인
		check_0 = 0;
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				if (board[i][j] != 0)
					check_0 += 1;
			}
		}
		if (check_0 == 16)
			break;

		width = rand() % SIZE;
		length = rand() % SIZE; //랜덤한 위치 하나를 지정
		if (board[length][width] == 0) //이 위치가 0일 경우 2를 넣고서 반복을 나감
		{
			srand(time(NULL));
			if (70 > rand() % 100)
				board[length][width] = 2;
			else
				board[length][width] = 4;

			break;
		}

	}

}

//보드에 2048이 있는지 확인하는 함수 
//있으면 1, 없으면 0를 반환
int check_2048()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (board[i][j] == 2048)
			{
				return 1;
			}
		}
	}
	return 0;
}

//보드 정렬
void board_sort(int direction)
{
	int x = 0;
	int array[SIZE] = { 0 };
	switch (direction)
	{
	case LEFT:
		for (int i = 0; i < SIZE; i++)
		{
			x = 0;
			//정렬할 배열 초기화
			for (int j = 0; j < SIZE; j++)
			{
				array[j] = 0;
			}
			//보드에서 배열로 정렬
			for (int j = 0; j < SIZE; j++)
			{
				if (board[i][j] != 0)
				{
					array[x] = board[i][j];
					x++;
				}
			}
			//배열 값을 다시 보드로 이동
			for (int j = 0; j < SIZE; j++)
			{
				board[i][j] = 0;
				board[i][j] = array[j];
			}
		}
		break;
	case RIGHT:
		for (int i = 0; i < SIZE; i++)
		{
			x = 3;
			for (int j = 0; j < SIZE; j++)
			{
				array[j] = 0;
			}
			for (int j = SIZE - 1; j > -1; j--)
			{
				if (board[i][j] != 0)
				{
					array[x] = board[i][j];
					x--;
				}
			}
			for (int j = SIZE - 1; j > -1; j--)
			{
				board[i][j] = 0;
				board[i][j] = array[j];
			}
		}
		break;
	case UP:
		for (int i = 0; i < SIZE; i++)
		{
			x = 0;
			for (int j = 0; j < SIZE; j++)
			{
				array[j] = 0;
			}
			for (int j = 0; j < SIZE; j++)
			{
				if (board[j][i] != 0)
				{
					array[x] = board[j][i];
					x++;
				}
			}
			for (int j = 0; j < SIZE; j++)
			{
				board[j][i] = 0;
				board[j][i] = array[j];
			}
		}
		break;
	case DOWN:
		for (int i = 0; i < SIZE; i++)
		{
			x = 3;
			for (int j = 0; j < SIZE; j++)
			{
				array[j] = 0;
			}
			for (int j = SIZE - 1; j > -1; j--)
			{
				if (board[j][i] != 0)
				{
					array[x] = board[j][i];
					x--;
				}
			}
			for (int j = SIZE - 1; j > -1; j--)
			{
				board[j][i] = 0;
				board[j][i] = array[j];
			}
		}
		break;
	default:
		break;
	}
}

//배열에 있는 같은 숫자를 더함
void board_add(int* array1, int* array2)
{
	//0칸 1칸이 같을 때
	if (array1[0] == array1[1])
	{
		array2[0] = array1[0] + array1[1];
		//2칸 3칸이 같을 때
		if (array1[2] == array1[3])
		{
			array2[1] = array1[2] + array1[3];
		}
		//2칸 3칸이 다를 때
		else
		{
			array2[1] = array1[2];
			array2[2] = array1[3];
		}
	}
	//0칸 1칸이 다를 떄
	else
	{
		array2[0] = array1[0];
		//1칸 2칸이 같을 때
		if (array1[1] == array1[2])
		{
			array2[1] = array1[1] + array1[2];
			array2[2] = array1[3];
		}
		//1칸 2칸이 다를 때
		else
		{
			array2[1] = array1[1];
			//2칸 3칸이 같을 때
			if (array1[2] == array1[3])
			{
				array2[2] = array1[2] + array1[3];
			}
			//2칸 3칸이 다를 때
			else
			{
				array2[2] = array1[2];
				array2[3] = array1[3];
			}
		}
	}
}

//방향키 입력대로 정렬 후 수를 더함	
void slide_board(int direction)
{
	int array1[SIZE] = { 0 };
	int array2[SIZE] = { 0 };
	int x = 0;
	switch (direction)
	{
	case LEFT:
		board_sort(LEFT);
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				array2[j] = 0;
			}
			x = 0;
			for (int j = 0; j < SIZE; j++)
			{
				array1[x] = board[i][j];
				x++;
			}

			board_add(array1, array2);
			x = 0;
			for (int j = 0; j < SIZE; j++)
			{
				board[i][j] = array2[x];
				x++;
			}
		}
		break;
	case RIGHT:
		board_sort(RIGHT);
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				array2[j] = 0;
			}
			x = 0;
			for (int j = SIZE - 1; j > -1; j--)
			{
				array1[x] = board[i][j];
				x++;
			}
			board_add(array1, array2);

			x = 0;
			for (int j = SIZE - 1; j > -1; j--)
			{
				board[i][j] = array2[x];
				x++;
			}
		}
		break;
	case UP:
		board_sort(UP);
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				array2[j] = 0;
			}
			x = 0;
			for (int j = 0; j < SIZE; j++)
			{
				array1[x] = board[j][i];
				x++;
			}

			board_add(array1, array2);
			x = 0;
			for (int j = 0; j < SIZE; j++)
			{
				board[j][i] = array2[x];
				x++;
			}
		}

		break;
	case DOWN:
		board_sort(DOWN);
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				array2[j] = 0;
			}
			x = 0;
			for (int j = SIZE - 1; j > -1; j--)
			{
				array1[x] = board[j][i];
				x++;
			}
			board_add(array1, array2);

			x = 0;
			for (int j = SIZE - 1; j > -1; j--)
			{
				board[j][i] = array2[x];
				x++;
			}
		}

		break;
	default:
		break;
	}
}

//화면 출력 위치 변경
void gotoxy(int x, int y)
{
	//x, y 좌표 설정
	COORD pos = { x,y };
	//커서 이동
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//gameover 판별
//over 됐으면 1, 아니면 0을 반환 
int check_gameover()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE - 1; j++)
		{
			//이웃한 두 수가 같거나 0일 때
			// X축 Y축
			if ((board[i][j] == board[i][j + 1]) || board[i][j] == 0 || board[i][j + 1] == 0)
				return 0;
			if ((board[j][i] == board[j + 1][i]) || board[j][i] == 0 || board[j + 1][i] == 0)
				return 0;
		}
	}
	return 1;
}

//현재 시간 반환
double get_current_time()
{
	return(double)clock() / CLOCKS_PER_SEC;
}

//타이머 시작
void start_timer() {
	start_time = time(NULL);
}

//현재 경과 시간을 반환하는 함수
double elapsed_time() {
	return difftime(time(NULL), start_time);
}

void Bubble_Sort(RANK arr[]) {
	RANK tmp;
	int len=13;
	/*for (int i = 0; i < 11; i++)
	{
		if (arr[i].timeRecord == 0)
		{
			len = i - 1;
			break;
		}
	}*/
	for (int i = 1; i < len - 1; ++i) {
		for (int j = 1; j < len - i - 1; ++j) {
			if (arr[j].timeRecord > arr[j + 1].timeRecord) {
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}

void Stuctrank_Reset(RANK arr[]) 
{
	for (int i = 0; i < 12; i++)
	{
		*(arr[i].name) = NULL;
		//arr[i].rankNumber = i;
		arr[i].timeRecord = 9999999999999999999;

	}
}

//txt 파일로 순위를 저장하는 함수
//배열에 10번째, 1부터 10까지만 저장함.
void save_Rank(FILE* RT, RANK arr[])
{
	for (int i = 1; i < 11; i++)
	{
		if(arr[i].timeRecord != 9999999999999999999)
			fprintf(RT, "%s %lf\n", arr[i].name, arr[i].timeRecord);
	}
}
