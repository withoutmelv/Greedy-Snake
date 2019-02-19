#include<stdio.h>
#include<time.h>
#include<windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include<stdlib.h>
#define U 1
#define D 2
#define L 3
#define R 4     
//蛇的状态，U：上 ；D：下；L:左 R：右
typedef struct SNAKE //蛇身的一个节点
{
	int x;
	int y;
	struct SNAKE *next;
}snake;

struct LIST
{
	int x;
	int y;
};
//全局变量//
int n = 1;
int count = 51;
int ducao = 6;
int score = 0, add = 10;//总得分与每次吃食物得分。
int status, sleeptime = 200;//每次运行的时间间隔
snake *head;
struct LIST food;
struct LIST boom;
struct LIST poison1;
struct LIST poison2;
struct LIST poison3;
snake *q;//遍历蛇的时候用到的指针
int endgamestatus = 0; //游戏结束的情况，1：撞到墙；2：咬到自己；3：主动退出游戏；4：被炸死了；5:毒草
int chapterstatus;
int top_score[5];//记录和比较分数的数组 
//声明全部函数//
void Pos(); 
void BubbleSort();
void SaveRecord(); 
void LoadRecord();
void creatMap();
void initsnake();
int biteself();
void createfood_1();
void createboom();
void createpoison();
void cantcrosswall();
void snakemove();
void pause();
void check();//防止刷新时重合
void gamecircle();
void welcometogame();
void endgame();
void gamestart();
void chapter();//关卡选择

void Pos(int x, int y)//设置光标位置
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}


void BubbleSort(int a[], int size)//分数进行排序
 {
	int i,j,t;
	for(j=0;j<size-1;j++)
		for(i=0;i<size-j-1;i++)
			if(a[i]<a[i+1])
			{t=a[i];a[i]=a[i+1];a[i+1]=t;}
 }

void LoadRecord()
{//读入以前存在的数据（分数），并存入数组中
	FILE *record;
	int i;
	if((record=fopen("Record.txt","r"))==NULL)
	{
		record=fopen("Record.txt","w");
		for(i=0;i<5;i++)
		{
			fprintf(record,"0 ",NULL);
		}
		fclose(record);
	}
	record=fopen("Record.txt","r+");
	for(i=0;i<5;i++)
		fscanf(record,"%d",&top_score[i]);
	fclose(record);
	
	for(i=0;i<5;i++)
	{Pos(40,10+i);
	printf("%d",top_score[i]);
	}
}

void SaveRecord(int score1){//记录该次成绩的分数
	FILE *record;int i;
	Pos(21,21);
	if(score1<=top_score[0]){
		if(score1==top_score[0])
			printf("恭喜你达到了本游戏的最高纪录,再加把劲儿吧！\n");
		else printf("加油哦，你还没有打破最高纪录⊙﹏⊙");	
	}
	else printf("恭喜你打破本游戏最高纪录！*^_^*");
	if(score1>top_score[4]){
		top_score[4]=score1;
		BubbleSort(top_score,5);
		record=fopen("Record.txt","w");
		for(i=0;i<5;i++)
			fprintf(record,"%d ",top_score[i]);
		fclose(record);
	}
}



void creatMap()//创建地图
{
	int i;
	if (chapterstatus == 1)
	{
		system("color 6");
		for (i = 0; i < 66; i += 2)//打印上下边框
		{
			Pos(i, 0);
			printf("■");
			Pos(i, 29);
			printf("■");
		}
		for (i = 1; i < 29; i++)//打印左右边框
		{
			Pos(0, i);
			printf("■");
			Pos(64, i);
			printf("■");
		}
	}
	else if (chapterstatus == 2)
	{
		system("color 7");
		for (i = 0; i < 66; i += 2)//打印上下边框
		{
			Pos(i, 0);
			printf("■");
			Pos(i, 29);
			printf("■");
		}

		for (i = 1; i < 29; i++)//打印左右边框
		{
			Pos(0, i);
			printf("■");
			Pos(64, i);
			printf("■");
		}
		for (i = 4; i < 20; i += 2)
		{
			Pos(i, 8);
			printf("■");
			Pos(i, 21);
			printf("■");
		}
		for (i = 2; i < 9; i++)
		{
			Pos(20, i);
			printf("■");
			Pos(46, i);
			printf("■");
		}
		for (i = 46; i < 62; i += 2)
		{
			Pos(i, 8);
			printf("■");
			Pos(i, 21);
			printf("■");
		}
		for (i = 21; i < 28; i++)
		{
			Pos(20, i);
			printf("■");
			Pos(46, i);
			printf("■");
		}
	}
	else if (chapterstatus == 3)
	{
		system("color 3");
		for (i = 0; i < 66; i += 2)//打印上下边框
		{
			Pos(i, 0);
			printf("■");
			Pos(i, 29);
			printf("■");
		}
		for (i = 4; i < 62; i += 2)
		{
			Pos(i, 15);
			printf("■");
		}
		for (i = 1; i < 29; i++)//打印左右边框
		{
			Pos(0, i);
			printf("■");
			Pos(64, i);
			printf("■");
		}

		for (i = 4; i < 20; i += 2)
		{
			Pos(i, 8);
			printf("■");
			Pos(i, 21);
			printf("■");
		}
		for (i = 2; i < 9; i++)
		{
			Pos(20, i);
			printf("■");
			Pos(46, i);
			printf("■");
		}
		for (i = 46; i < 62; i += 2)
		{
			Pos(i, 8);
			printf("■");
			Pos(i, 21);
			printf("■");
		}
		for (i = 21; i < 28; i++)
		{
			Pos(20, i);
			printf("■");
			Pos(46, i);
			printf("■");
		}
	}
}
void initsnake()//初始化蛇身  
{
	snake*tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));//从蛇尾开始，头插法，以x,y设定开始的位置// 
	tail->x = 24;
	tail->y = 10;
	tail->next = NULL;
	for (i = 1; i <= 4; i++)
	{
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 24 + 2 * i;
		head->y = 10;
		tail = head;

	}
	while (tail != NULL)//从头到为，输出蛇身
	{
		Pos(tail->x, tail->y);
		printf("■");
		tail = tail->next;
	}
}
int biteself()//判断是否咬到了自己
{
	snake *self;
	self = head->next;
	while (self != NULL)
	{
		if (self->x == head->x && self->y == head->y)
		{
			return 1;
		}
		self = self->next;
	}
	return 0;
}


void createfood_1()//随机出现食物
{
	int i;
	srand((unsigned)time(NULL));
loop:	food.x = rand() % 56 + 2;
	while ((food.x % 2) != 0)    //保证其为偶数，使得食物能与蛇头对其
	{
		food.x = food.x + 1;
	}
	food.y = rand() % 28 + 1;
	q = head;
	while (q!= NULL)
	{
		if (q->x == food.x && q->y == food.y) //判断蛇身是否与食物重合
		{
			goto loop;
		}
		q = q->next;
		
	}

	if (chapterstatus == 2)
	{
		for (i = 2; i <= 8; i++)
		{
			for (int j = 4; j < 20; j += 2)
			{
				for (int k = 46; k < 62; k += 2)
				{
					for (int m = 21; m < 28; m++)
					{
						if ((food.x == 20 && food.y == i) || (food.x == 46 && food.y == i) || (food.x == j&&food.y == 8) || (food.x == j&&food.y == 21) || (food.x == k&&food.y == 8) || (food.x == k&&food.y == 21) || (food.x == 20 && food.y == m) || (food.x == 46 && food.y == m))
						{
							goto loop;
						}
					}
				}
			}

		}


	}

	if (chapterstatus == 3)
	{
		for (i = 2; i <= 8; i++)
		{
			for (int j = 4; j < 20; j += 2)
			{
				for (int k = 46; k < 62; k += 2)
				{
					for (int m = 21; m < 28; m++)
					{
						for (int p = 4; p < 62; p += 2)
						{
							if ((food.x == 20 && food.y == i) || (food.x == 46 && food.y == i) || (food.x == j&&food.y == 8) || (food.x == j&&food.y == 21) || (food.x == k&&food.y == 8) || (food.x == k&&food.y == 21) || (food.x == 20 && food.y == m) || (food.x == 46 && food.y == m) || (food.x == p&&food.y == 15))
							{
								goto loop;
							}
						}
					}
				}
			}

		}
	}

	Pos(food.x, food.y);
	printf("●");
}

void createboom()//炸弹
{
	int i;
loop:	boom.x = rand() % 56 + 2;

	while ((boom.x % 2) != 0)
	{
		boom.x = boom.x + 1;
	}
	boom.y = rand() % 28 + 1;
	q = head;
	while (q != NULL)
	{
		if (q->x == boom.x && q->y == boom.y) //判断蛇身是否与食物重合
		{
			goto loop;
		}
		q = q->next;
	}


	if (chapterstatus == 2)
	{
		for (i = 2; i <= 8; i++)
		{
			for (int j = 4; j < 20; j += 2)
			{
				for (int k = 46; k < 62; k += 2)
				{
					for (int m = 21; m < 28; m++)
					{
						if ((boom.x == 20 && boom.y == i) || (boom.x == 46 && boom.y == i) || (boom.x == j&&boom.y == 8) || (boom.x == j&&boom.y == 21) || (boom.x == k&&boom.y == 8) || (boom.x == k&&boom.y == 21) || (boom.x == 20 && boom.y == m) || (boom.x == 46 && boom.y == m))
						{
							goto loop;
						}
					}
				}
			}
		}


	}

	if (chapterstatus == 3)
	{
		for (i = 2; i <= 8; i++)
		{
			for (int j = 4; j < 20; j += 2)
			{
				for (int k = 46; k < 62; k += 2)
				{
					for (int m = 21; m < 28; m++)
					{
						for (int p = 4; p < 62; p += 2)
						{
							if ((boom.x == 20 && boom.y == i) || (boom.x == 46 && boom.y == i) || (boom.x == j&&boom.y == 8) || (boom.x == j&&boom.y == 21) || (boom.x == k&&boom.y == 8) || (boom.x == k&&boom.y == 21) || (boom.x == 20 && boom.y == m) || (boom.x == 46 && boom.y == m) || (boom.x == p&&boom.y == 15))
							{
								goto loop;
							}
						}
					}
				}
			}

		}
	}
	Pos(boom.x, boom.y);
	printf("▲");
}

void createpoison()//毒草
{
	int i;
loop:	poison1.x = rand() % 56 + 2;
	while ((poison1.x % 2) != 0)    //保证其为偶数，使得食物能与蛇头对其
	{
		poison1.x = poison1.x + 1;
	}
	poison1.y = rand() % 28 + 1;

	poison2.x = rand() % 56 + 2;
	while ((poison2.x % 2) != 0)    //保证其为偶数，使得食物能与蛇头对其
	{
		poison2.x = poison2.x + 1;
	}
	poison2.y = rand() % 28 + 1;

	poison3.x = rand() % 56 + 2;
	while ((poison3.x % 2) != 0)    //保证其为偶数，使得食物能与蛇头对其
	{
		poison3.x = poison3.x + 1;
	}
	poison3.y = rand() % 28 + 1;
	q = head;
	while (q != NULL)
	{
		if ((q->x == poison1.x && q->y == poison1.y)||(q->x==poison2.x&&q->y==poison2.y)||(q->x==poison3.x&&q->y==poison3.y)) //判断蛇身是否与食物重合
		{
			goto loop;
		}
		q = q->next;
	}
	if((poison1.x==poison2.x&&poison1.y==poison2.y)||(poison1.x==poison3.x&&poison1.y==poison3.y)||(poison2.x==poison3.x&&poison2.y==poison3.y))
		goto loop;
	if (chapterstatus == 2)
	{
		for (i = 2; i <= 8; i++)
		{
			for (int j = 4; j < 20; j += 2)
			{
				for (int k = 46; k < 62; k += 2)
				{
					for (int m = 21; m < 28; m++)
					{
						if ((poison1.x == 20 && poison1.y == i) || (poison1.x == 46 && poison1.y == i) || (poison1.x == j&&poison1.y == 8) || (poison1.x == j&&poison1.y == 21) || (poison1.x == k&&poison1.y == 8) || (poison1.x == k&&poison1.y == 21) || (poison1.x == 20 && poison1.y == m) || (poison1.x == 46 && poison1.y == m)||(poison2.x == 20 && poison2.y == i) || (poison2.x == 46 && poison2.y == i) || (poison2.x == j&&poison2.y == 8) || (poison2.x == j&&poison2.y == 21) || (poison2.x == k&&poison2.y == 8) || (poison2.x == k&&poison2.y == 21) || (poison2.x == 20 && poison2.y == m) || (poison2.x == 46 && poison2.y == m)||(poison3.x == 20 && poison3.y == i) || (poison3.x == 46 && poison3.y == i) || (poison3.x == j&&poison3.y == 8) || (poison3.x == j&&poison3.y == 21) || (poison3.x == k&&poison3.y == 8) || (poison3.x == k&&poison3.y == 21) || (poison3.x == 20 && poison3.y == m) || (poison3.x == 46 && poison3.y == m))
						{
							goto loop;
						}
					}
				}
			}

		}


	}

	if (chapterstatus == 3)
	{
		for (i = 2; i <= 8; i++)
		{
			for (int j = 4; j < 20; j += 2)
			{
				for (int k = 46; k < 62; k += 2)
				{
					for (int m = 21; m < 28; m++)
					{
						for (int p = 4; p < 62; p += 2)
						{
							if ((poison1.x == 20 && poison1.y == i) || (poison1.x == 46 && poison1.y == i) || (poison1.x == j&&poison1.y == 8) || (poison1.x == j&&poison1.y == 21) || (poison1.x == k&&poison1.y == 8) || (poison1.x == k&&poison1.y == 21) || (poison1.x == 20 && poison1.y == m) || (poison1.x == 46 && poison1.y == m) || (poison1.x == p&&poison1.y == 15)
								||(poison2.x == 20 && poison2.y == i) || (poison2.x == 46 && poison2.y == i) || (poison2.x == j&&poison2.y == 8) || (poison2.x == j&&poison2.y == 21) || (poison2.x == k&&poison2.y == 8) || (poison2.x == k&&poison2.y == 21) || (poison2.x == 20 && poison2.y == m) || (poison2.x == 46 && poison2.y == m) || (poison2.x == p&&poison2.y == 15)
								||(poison3.x == 20 && poison3.y == i) || (poison3.x == 46 && poison3.y == i) || (poison3.x == j&&poison3.y == 8) || (poison3.x == j&&poison3.y == 21) || (poison3.x == k&&poison3.y == 8) || (poison3.x == k&&poison3.y == 21) || (poison3.x == 20 && poison3.y == m) || (poison3.x == 46 && poison3.y == m) || (poison3.x == p&&poison3.y == 15))
							{
								goto loop;
							}
						}
					}
				}
			}

		}
	}
	Pos(poison1.x, poison1.y);
	printf("★");
	Pos(poison2.x, poison2.y);
	printf("★");
	Pos(poison3.x, poison3.y);
	printf("★");

}



void cantcrosswall()//不能穿墙
{
	int i;
	if (chapterstatus == 1)
	{
		if (head->x == 0 || head->x == 64 || head->y == 0 || head->y == 29)
		{
			endgamestatus = 1;
			endgame();
		}
	}
	if (chapterstatus == 2)
	{
		if (head->x == 0 || head->x == 64 || head->y == 0 || head->y == 29)
		{
			endgamestatus = 1;
			endgame();
		}
		for (i = 2; i <= 8; i++)
		{
			if (head->x == 20 && head->y == i)
			{
				endgamestatus = 1;
				endgame();
			}
			else if (head->x == 46 && head->y == i)
			{
				endgamestatus = 1;
				endgame();
			}
		}
		for (i = 4; i < 20; i += 2)
		{
			if (head->x == i&&head->y == 8)
			{
				endgamestatus = 1;
				endgame();
			}
			else if (head->x == i&&head->y == 21)
			{
				endgamestatus = 1;
				endgame();
			}
		}
		for (i = 46; i < 62; i += 2)
		{
			if (head->x == i&&head->y == 8)
			{
				endgamestatus = 1;
				endgame();
			}
			else if (head->x == i&&head->y == 21)
			{
				endgamestatus = 1;
				endgame();
			}
		}
		for (i = 21; i < 28; i++)
		{
			if (head->x == 20 && head->y == i)
			{
				endgamestatus = 1;
				endgame();
			}
			else if (head->x == 46 && head->y == i)
			{
				endgamestatus = 1;
				endgame();
			}
		}
	}
	if (chapterstatus == 3)
	{
		if (head->x == 0 || head->x == 64 || head->y == 0 || head->y == 29)
		{
			endgamestatus = 1;
			endgame();
		}
		for (i = 2; i <= 8; i++)
		{
			if (head->x == 20 && head->y == i)
			{
				endgamestatus = 1;
				endgame();
			}
			else if (head->x == 46 && head->y == i)
			{
				endgamestatus = 1;
				endgame();
			}
		}
		for (i = 4; i < 20; i += 2)
		{
			if (head->x == i&&head->y == 8)
			{
				endgamestatus = 1;
				endgame();
			}
			else if (head->x == i&&head->y == 21)
			{
				endgamestatus = 1;
				endgame();
			}
		}
		for (i = 46; i < 62; i += 2)
		{
			if (head->x == i&&head->y == 8)
			{
				endgamestatus = 1;
				endgame();
			}
			else if (head->x == i&&head->y == 21)
			{
				endgamestatus = 1;
				endgame();
			}
		}
		for (i = 21; i < 28; i++)
		{
			if (head->x == 20 && head->y == i)
			{
				endgamestatus = 1;
				endgame();
			}
			else if (head->x == 46 && head->y == i)
			{
				endgamestatus = 1;
				endgame();
			}
		}


		for (i = 4; i < 62; i += 2)
		{
			if (head->x == i&&head->y == 15)
			{
				endgamestatus = 1;
				endgame();
			}
		}
	}
}


void snakemove()//蛇前进,上U,下D,左L,右R
{
	snake * nexthead;
	cantcrosswall();
	nexthead = (snake*)malloc(sizeof(snake));
	if (status == U)
	{
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
		
	}
	if (status == D)
	{
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
		
	}
		
	if (status == L)
	{
		nexthead->x = head->x - 2;
		nexthead->y = head->y;
		
	}
		
	if (status == R)
	{
		nexthead->x = head->x + 2;
		nexthead->y = head->y;
		
	}
	if (head->x == food.x && head->y == food.y)//如果下一个有食物//
		{
			if(status==U)
				system("color C");
			if(status==D)
				system("color A");
			if(status==L)
				system("color D");
			if(status==R)
				system("color E");
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score = score + add;
			createfood_1();
			check();
		}

		else if (head->x == boom.x&&head->y == boom.y)//如果下一个有炸弹
		{
			q = head;
			n = 1;
			while (q->next != NULL)
			{
				n++;
				q = q->next;
			}
			if (n <= 2)
			{
				endgamestatus = 4;
				endgame();
			}
			if (n % 2 == 0)
				n = n / 2;
			else
				n = (n - 1) / 2;
			for (int i = 0; i<n; i++)//擦掉蛇身一半
			{
				q = head;
				while (q->next->next != NULL)
				{
					q = q->next;
				}
				Pos(q->next->x, q->next->y);
				printf("  ");
				q->next = NULL;
			}

			score = score - add / 2;
			createboom();
			check();

		}


		else if ((head->x == poison1.x&&head->y == poison1.y)||(head->x == poison2.x&&head->y == poison2.y)||(head->x == poison3.x&&head->y == poison3.y))//如果下一个有毒草
		{
			n = 1;
			q = head;
			while (q->next != NULL)
			{
				n++;
				q = q->next;
			}
			if (n <= 2)
			{
				endgamestatus = 5;
				endgame();
			}
			q = head;
			while (q->next->next != NULL)
			{
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
			score = score - add / 2;
			Pos(poison1.x,poison1.y);
			printf(" ");
			Pos(poison2.x,poison2.y);
			printf(" ");
			Pos(poison3.x,poison3.y);
			printf(" ");
			createpoison();
			check(); 

		}
		else                                               //如果什么都没有//
		{
			count++;
			ducao++;
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf("  ");
			q->next = NULL;
			
		}
	
	
	
	if (biteself() == 1)       //判断是否会咬到自己
	{
		endgamestatus = 2;
		endgame();
	}
	
}

void check()
{
	if ((food.x == boom.x&&food.y == boom.y) || (food.x == poison1.x&&food.y == poison1.y)|| (food.x == poison2.x&&food.y == poison2.y)|| (food.x == poison3.x&&food.y == poison3.y))
	{
		createfood_1();
	}
	if ((boom.x == poison1.x&&boom.y == poison1.y)||(boom.x == poison2.x&&boom.y == poison2.y)||(boom.x == poison3.x&&boom.y == poison3.y))
	{
		createboom();
	}
}

void pause()//暂停
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))
		{
			break;
		}
	}
}

void chapter()//关卡选择界面
{

	Pos(40, 12);
	printf("1.无聊的第一关。\n");
	Pos(40, 16);
	printf("2.有点改变的第二关。\n");
	Pos(40, 20);
	printf("3.还可以玩玩的第三关。\n");
	Pos(40, 24);
	printf("4.没啥用的排行榜。\n");
	Pos(40, 28);
	printf("你的选择是：");
	scanf("%d", &chapterstatus);
	if (chapterstatus > 4)
	{
		system("cls");
		printf("抱歉，你不能选择不存在的关卡。\n");
		system("pause");

		system("cls");
		chapter();
	}

	else if (chapterstatus == 4)
	{
		system("cls");
		LoadRecord();
		system("pause");
		system("cls");
		chapter();
	}
	else
		system("cls");
}



void gamecircle()//控制游戏
{
	Pos(70, 15);
	printf("不能穿墙，不能咬到自己\n");
	Pos(70, 17);
	printf("用↑.↓.←.→分别控制蛇的移动.");
	Pos(70, 19);
	printf("右回车键 为加速\n");
	Pos(70, 20);
	printf("右shift键 为减速\n");
	Pos(70, 21);
	printf("ESC ：退出游戏\n");
	Pos(70, 22);
	printf("space：暂停游戏.");
	status = R;
	while (1)
	{
		if (count % 50 == 0)
		{
			Pos(food.x, food.y);
			printf(" ");
			createfood_1();
			check();
		}
		if (ducao % 7 == 0)
		{
			Pos(poison1.x, poison1.y);
			printf(" ");
			Pos(poison2.x, poison2.y);
			printf(" ");
			Pos(poison3.x, poison3.y);
			printf(" ");
			
		}
		if (ducao % 9 == 0)
		{
			Pos(poison1.x, poison1.y);
			printf("★");
			Pos(poison2.x, poison2.y);
			printf("★");
			Pos(poison3.x, poison3.y);
			printf("★");
		}
		Pos(70, 10);
		printf("得分：%d  ", score);
		Pos(70, 11);
		printf("每个食物得分：%d分", add);
		if (GetAsyncKeyState(VK_UP) && status != D)
		{
			status = U;
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != U)
		{
			status = D;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != R)
		{
			status = L;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != L)
		{
			status = R;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{
			pause();
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
		{
			endgamestatus = 3;
			break;
		}
		else if (GetAsyncKeyState(VK_RETURN))
		{
			if (sleeptime >= 50)
			{
				sleeptime = sleeptime - 30;
				add = add + 2;
				if (sleeptime == 320)
				{
					add = 2;//防止减到1之后再加回来有错
				}
			}
		}
		else if (GetAsyncKeyState(VK_RSHIFT))
		{
			if (sleeptime<350)
			{
				sleeptime = sleeptime + 30;
				add = add - 2;
				if (sleeptime == 350)
				{
					add = 1;  //保证最低分为1
				}
			}
		}
		Sleep(sleeptime);
		snakemove();
		if (chapterstatus == 1)
		{
			if (score >= 50 && score <= 100)
			{
				chapterstatus = 2;
		system("cls");
		printf("恭喜你通过第一关\n");
		system("pause");
		system("cls");
		creatMap();
		initsnake();
		createfood_1();
		createboom();
		createpoison();
		gamecircle();
			}
		}
		if (chapterstatus == 2)
		{
			if (score>100)
			{
				chapterstatus = 3;
		system("cls");
		printf("恭喜你通过第二关\n");
		system("pause");
		system("cls");
		creatMap();
		initsnake();
		createfood_1();
		createboom();
		createpoison();
		gamecircle();
			}
		}

	}
}
void welcometogame()//开始界面
{
	Pos(40, 12);
	system("title  吕义的c语言课设");
	Pos(40, 15);
	printf("欢迎来到聪明的小蛇游戏");
	Pos(40, 25);

	system("pause");
	system("cls");

}
void endgame()//结束游戏
{
	system("cls");
	Pos(24, 12);
	if (endgamestatus == 1)
	{
		printf("对不起，您撞到墙了。游戏结束.");
	}
	else if (endgamestatus == 2)
	{
		printf("对不起，您咬到自己了。游戏结束.");
	}
	else if (endgamestatus == 3)
	{
		printf("您的已经结束了游戏。");
	}
	else if (endgamestatus == 4)
	{
		printf("对不起，您被炸死了");
	}
	else if (endgamestatus == 5)
	{
		printf("对不起，您被毒毒毒毒毒死了");
	}
	Pos(24, 13);
	printf("您的得分是%d\n", score);
	SaveRecord(score);
	score = 0;
	system("pause");
	system("cls");
	gamestart();
	gamecircle();

}



void gamestart()//游戏初始化
{
	system("mode con cols=100 lines=30");
	welcometogame();
	chapter();
	creatMap();
	initsnake();
	createfood_1();
	createboom();
	createpoison();
}
int main()
{
	PlaySoundA ("bgm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);


	gamestart();
	gamecircle();



	return 0;

}
