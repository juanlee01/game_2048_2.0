//����2048
//������ timer �߰� ����

//2.1 ������������ �߰�.
//2.2 �޸��� �̸� �ð� ����.
//2.3 �޸��� �̸� �ð� �ҷ���, ���� ���� ���� �����Ͽ� �� ����.
//2.4 ����ǥ ��� �Լ� �ۼ�.
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

#define SIZE 4		//����ũ��
#define XBOARD 15	//������ġ X,Y
#define YBOARD 5	

#define UP 72		//����Ű �Է°�
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#define ESC 27

//���� �迭
int board[SIZE][SIZE] = {
	{1024,1024,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};

int main()
{
	//���� ������ ����
	FILE* ranktxt;

	char name[100];
	int key = 0;
	double end_time = 0;
	RANK ranking[12];
	
	//��ũ ���� ����
	//������ ���� ������ ��ŷ ����ü�� �о��.
	ranktxt = fopen("ranking.txt", "r");
	Stuctrank_Reset(ranking);

	int i = 0;
	for (int i = 1; i < 12; i++)
	{
		fscanf(ranktxt, "%s %lf\n", ranking[i].name, &ranking[i].timeRecord);
	}
	fclose(ranktxt);
	
	//��ŷ ���
	system("cls");
	print_2048();
	print_Rank(ranking);
	gotoxy(XBOARD - 4, YBOARD + 13);
	printf("�����Ϸ��� �ƹ�Ű�� �����ּ���.");
	while (!_kbhit())
	{

	}
	//���� ���� ����
	random_set();
	random_set();
	system("cls");
	print_board();

	start_timer();//Ÿ�̸� ����

	
	_getch();
	while (1)
	{
		key = _getch();
		//ESC�̸� ���� ����
		if (key == ESC)
			break;

		//����Ű Ȯ��
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
		system("cls"); //�������� ����� ���� �ܼ� ����
		print_2048();
		print_board();
		gotoxy(XBOARD, YBOARD+10);
		printf("%.2f sec\n", elapsed_time());
		//��������Ȯ��
		if (check_gameover() == 1)
		{
			end_time = elapsed_time();
			printf("G A M E O V E R!\n\n");
			printf("�����Ϸ��� ESC�� �����ּ���. ");
		}
		if (check_2048() == 1)
		{
			ranktxt = fopen("ranking.txt", "w");
			end_time = elapsed_time();
			ranking[11].timeRecord = end_time; 
			gotoxy(XBOARD, YBOARD + 11);
			printf("congratulations! 2048!\n");
			gotoxy(XBOARD, YBOARD + 12);
			printf("��ŷ�� ������ �̸��� �Է����ּ��� >> ");
			gets(ranking[11].name);
			

			//fprintf(ranktxt, "%s %f", name, end_time);
			Bubble_Sort(ranking);
			//fputs(name, ranktxt);
			system("cls"); 
			print_2048();
			print_Rank(ranking);
			save_Rank(ranktxt, ranking);
			gotoxy(XBOARD-4, YBOARD + 13);
			printf("�����Ϸ��� ESC�� �����ּ���. ");
			fclose(ranktxt);
		}
		
		
	}
	
}
 
//���� �̸� ��� �Լ�
void print_2048()
{
	
	gotoxy(XBOARD + 6, 2);
	printf("�������������� ��������\n");
	gotoxy(XBOARD + 6, 3);
	printf("�������� ��������������\n");
	gotoxy(XBOARD + 6, 4);
	printf("������������  ��������\n");
}
//���带 ������ִ� �Լ�
void print_board()
{
	int z = 1;
	print_2048();
	//���� ���
	gotoxy(XBOARD, YBOARD);
	printf("��������������������������������������������������\n");
	for (int i = 0; i < 4; i++)
	{
		gotoxy(XBOARD, YBOARD + z);
		for (int j = 0; j < 4; j++)
		{

			if (board[i][j] == 0)
				printf("��  ��  ");
			else
				printf("�� %4d", board[i][j]);
		}
		printf("��");
		z += 1;
		if (i == 3)
		{
			gotoxy(XBOARD, YBOARD + z);
			printf("��������������������������������������������������");
			printf("\n");
			break;
		}
		gotoxy(XBOARD, YBOARD + z);
		printf("��������������������������������������������������\n");
		z += 1;
	}
}
//��ŷ ������ִ� �Լ�
void print_Rank(RANK arr[])
{
	gotoxy(XBOARD-4, YBOARD);
	printf("��������������������������������������������������������������������\n");
	for (int i = 1; i < 11; i++)
	{
		if (arr[i].timeRecord != 9999999999999999999)
		{
			gotoxy(XBOARD-4, YBOARD+i);
			printf("�� %2d# %10s %10.2lf min  ��\n", i, arr[i].name, arr[i].timeRecord/60);
		}
		else
		{
			gotoxy(XBOARD-4, YBOARD + i);
			printf("��                                ��\n");
		}
	}
	gotoxy(XBOARD-4, YBOARD + 11);
	printf("��������������������������������������������������������������������\n");
}

//���忡 2, 4�� ������ ��ġ�� �־��ִ� �Լ�
void random_set()
{
	int width = 0;
	int length = 0;
	int check_0 = 0;
	while (1)
	{
		//�������� �ֱ� ���� ���尡 �� ���ִ��� Ȯ��
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
		length = rand() % SIZE; //������ ��ġ �ϳ��� ����
		if (board[length][width] == 0) //�� ��ġ�� 0�� ��� 2�� �ְ� �ݺ��� ����
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

//���忡 2048�� �ִ��� Ȯ���ϴ� �Լ� 
//������ 1, ������ 0�� ��ȯ
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

//���� ����
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
			//������ �迭 �ʱ�ȭ
			for (int j = 0; j < SIZE; j++)
			{
				array[j] = 0;
			}
			//���忡�� �迭�� ����
			for (int j = 0; j < SIZE; j++)
			{
				if (board[i][j] != 0)
				{
					array[x] = board[i][j];
					x++;
				}
			}
			//�迭 ���� �ٽ� ����� �̵�
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

//�迭�� �ִ� ���� ���ڸ� ����
void board_add(int* array1, int* array2)
{
	//0ĭ 1ĭ�� ���� ��
	if (array1[0] == array1[1])
	{
		array2[0] = array1[0] + array1[1];
		//2ĭ 3ĭ�� ���� ��
		if (array1[2] == array1[3])
		{
			array2[1] = array1[2] + array1[3];
		}
		//2ĭ 3ĭ�� �ٸ� ��
		else
		{
			array2[1] = array1[2];
			array2[2] = array1[3];
		}
	}
	//0ĭ 1ĭ�� �ٸ� ��
	else
	{
		array2[0] = array1[0];
		//1ĭ 2ĭ�� ���� ��
		if (array1[1] == array1[2])
		{
			array2[1] = array1[1] + array1[2];
			array2[2] = array1[3];
		}
		//1ĭ 2ĭ�� �ٸ� ��
		else
		{
			array2[1] = array1[1];
			//2ĭ 3ĭ�� ���� ��
			if (array1[2] == array1[3])
			{
				array2[2] = array1[2] + array1[3];
			}
			//2ĭ 3ĭ�� �ٸ� ��
			else
			{
				array2[2] = array1[2];
				array2[3] = array1[3];
			}
		}
	}
}

//����Ű �Է´�� ���� �� ���� ����	
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

//ȭ�� ��� ��ġ ����
void gotoxy(int x, int y)
{
	//x, y ��ǥ ����
	COORD pos = { x,y };
	//Ŀ�� �̵�
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//gameover �Ǻ�
//over ������ 1, �ƴϸ� 0�� ��ȯ 
int check_gameover()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE - 1; j++)
		{
			//�̿��� �� ���� ���ų� 0�� ��
			// X�� Y��
			if ((board[i][j] == board[i][j + 1]) || board[i][j] == 0 || board[i][j + 1] == 0)
				return 0;
			if ((board[j][i] == board[j + 1][i]) || board[j][i] == 0 || board[j + 1][i] == 0)
				return 0;
		}
	}
	return 1;
}

//���� �ð� ��ȯ
double get_current_time()
{
	return(double)clock() / CLOCKS_PER_SEC;
}

//Ÿ�̸� ����
void start_timer() {
	start_time = time(NULL);
}

//���� ��� �ð��� ��ȯ�ϴ� �Լ�
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

//txt ���Ϸ� ������ �����ϴ� �Լ�
//�迭�� 10��°, 1���� 10������ ������.
void save_Rank(FILE* RT, RANK arr[])
{
	for (int i = 1; i < 11; i++)
	{
		if(arr[i].timeRecord != 9999999999999999999)
			fprintf(RT, "%s %lf\n", arr[i].name, arr[i].timeRecord);
	}
}
