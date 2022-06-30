#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#define MAX 4
int g[MAX][MAX];
int score;
int in;
int count;
int win;
int stop;
void filecorrect(FILE *f,int *des,int def)
{
	if(fscanf(f,"%d",des)!=1)
		*des=def;
}
int log2(int a)
{
	if (a<=1)
		return 0;
	else return log2(a/2)+1;
}
void screen()
{
	int i,j;
	system("cls");
	printf("Score: %d\n",score);
	for (i=0;i<MAX;i++)
	{
		for (j=0;j<MAX;j++)
		{
			if (g[i][j]==0)
				printf("  _  ");
			else if(g[i][j]<10)
				printf("  %d  ",g[i][j]);
			else if(g[i][j]<100)
				printf(" %d  ",g[i][j]);
			else if(g[i][j]<1000)
				printf(" %d ",g[i][j]);
			else
				printf("%d ",g[i][j]);
		}
		printf("\n\n");
	}
	if (win==1)
	{
		printf("\nYou win!\n");
		printf("You have created 2048.\n");
	}
}
void intro()
{
	printf("Instruction:\n\n");
	printf("Use arrow keys to move.\n");
	printf("Press Space to open the pause menu.\n");
	printf("\nPress any key to continue.\n");
	getch();
}
int process(int in)
{
	int i,j,k;
	int stack[MAX];
	int top;
	int pop;
	int ok=0;
	int tmp;
	switch(in*in)
	{
		case 1:
			for (i=0;i<MAX;i++)
			{
				switch (in)
				{
					case 1: j=0; k=1; break;
					case -1: j=MAX-1; k=-1; break;
				}
				top=-1;
				pop=MAX-1-j;
				tmp=0;
				for (;j>=0&&j<MAX;j+=k)
				{
					if(g[i][j]!=0)
					{
						top++;
						stack[top]=g[i][j];
						tmp=1;
					}
					else if (tmp==1)
						ok=1;
				}
				while (top!=-1)
				{
					if (top>0&&stack[top]==stack[top-1])
					{
						score+=log2(stack[top])*5;
						count--;
						stack[top]*=2;
						if (stack[top]==2048)
							win=1;
						g[i][pop]=stack[top];
						pop-=k;
						top-=2;
						ok=1;
					}
					else
					{
						g[i][pop]=stack[top];
						pop-=k;
						top--;
					}
				}
				while(pop>=0&&pop<MAX)
				{
					g[i][pop]=0;
					pop-=k;
				}
			}
			break;
		case 4:
			for (j=0;j<MAX;j++)
			{
				switch (in)
				{
					case 2: i=0; k=1; break;
					case -2: i=MAX-1; k=-1; break;
				}
				top=-1;
				pop=MAX-1-i;
				tmp=0;
				for (;i>=0&&i<MAX;i+=k)
				{
					if (g[i][j]!=0)
					{
						top++;
						stack[top]=g[i][j];
						tmp=1;
					}
					else if (tmp==1)
						ok=1;
				}
				while (top!=-1)
				{
					if (top>0&&stack[top]==stack[top-1])
					{
						score+=log2(stack[top])*5;
						count--;
						stack[top]*=2;
						if (stack[top]==2048)
							win=1;
						g[pop][j]=stack[top];
						pop-=k;
						top-=2;
						ok=1;
					}
					else
					{
						g[pop][j]=stack[top];
						pop-=k;
						top--;
					}
				}
				while(pop>=0&&pop<MAX)
				{
					g[pop][j]=0;
					pop-=k;
				}
			}
			break;
	}
	return ok;
}
int poss()
{
	int i,j;
	for (i=0;i<MAX;i++)
		for (j=0;j<MAX-1;j++)
			if (g[i][j]==g[i][j+1]||g[j][i]==g[j+1][i])
				return 1;
	return 0;
}
void setup()
{
	int i,j;
	FILE * save = fopen("saveFile.txt","w+");
	for(i=0;i<MAX;i++)
	{
		for(j=0;j<MAX;j++)
			fprintf(save,"%d ",0);
		fprintf(save,"\n");
	}
	for (i=0;i<3;i++)
		fprintf(save,"%d\n",0);
	fprintf(save,"%d\n",-1);
	fclose(save);
}
void gameover()
{
	int hi[5];
	int i,j;
	printf("\nGame over!\n");
	FILE * rank = fopen("scoreFile.txt","r");
	for (i=0;i<5;i++)
		filecorrect(rank,&hi[i],0);
	fclose(rank);
	for (i=0;i<5;i++)
		if (score>=hi[i])
		{
			for (j=4;j>i;j--)
				hi[j]=hi[j-1];
			hi[i]=score;
			break;
		}
	printf("High score: %d\n",hi[0]);
	rank = fopen("scoreFile.txt","w+");
	for (i=0;i<5;i++)
		fprintf(rank,"%d\n",hi[i]);
	fclose(rank);
	setup();
	Sleep(2000);
}
void savefile()
{
	int i,j;
	FILE * save = fopen("saveFile.txt","w+");
		for(i=0;i<MAX;i++)
		{
			for(j=0;j<MAX;j++)
				fprintf(save,"%d ",g[i][j]);
			fprintf(save,"\n");
		}
		fprintf(save,"%d\n",score);
		fprintf(save,"%d\n",count);
		fprintf(save,"%d\n",win);
		fprintf(save,"%d\n",0);
		fclose(save);
}
int pause()
{
	char c;
	system("cls");
	printf("Pause\n");
	printf("\t1. End the game without saving\n");
	printf("\t2. Save the game and go back to the menu\n");
	printf("\t3. Back\n");
	do
	{
		c=getch();
	} while (c!='1' && c!='2' && c!='3');
	switch(c)
	{
		case '1':
			return 1;
		case '2':
			savefile();
			return 2;
	}
	return 0;
}
void rd()
{
	int x,y;
	do
	{
		x=rand()%MAX;
		y=rand()%MAX;
	} while (g[x][y]!=0);
	g[x][y]=2;
	count++;
}
void play()
{
	int ctrl;
	int ok;
	intro();
	while(1)
	{
		if (stop==-1)
			rd();
		screen();
		if (stop==1)
		{
			gameover();
			break;
		}
		else stop=-1;
		if (count==MAX*MAX)
		{
			if (poss()==0)
			{
				gameover();
				break;
			}
		}
		do
		{
			do
			{
				ok=0;
				ctrl=getch();
				if (ctrl==' ')
					break;
				if (ctrl==0||ctrl==224)
				{
					ok=1;
					ctrl=getch();
				}
			} while (ok==0||(ctrl!=75&&ctrl!=77&&ctrl!=72&&ctrl!=80));
			if (ctrl==' ')
			{	
				stop=pause();
				break;
			}
			switch(ctrl)
			{
				case 75: in=-1; break;
				case 77: in=1; break;
				case 72: in=-2; break;
				case 80: in=2; break;
			}
			ok=process(in);
		} while (ok==0);
		if (stop==2)
			break;
	}
}
void cont()
{
	int i,j;
	FILE * save = fopen("saveFile.txt","r");
	for(i=0;i<MAX;i++)
		for(j=0;j<MAX;j++)
			filecorrect(save,&g[i][j],0);
	filecorrect(save,&score,0);
	filecorrect(save,&count,0);
	filecorrect(save,&win,0);
	filecorrect(save,&stop,-1);
	fclose(save);
	play();
}
void hiprint()
{
	int hi;
	int i;
	FILE * rank = fopen("scoreFile.txt","r");
	for (i=0;i<5;i++)
	{
		filecorrect(rank,&hi,0);
		printf("\t%d.\t%d\n",i+1,hi);
	}
	fclose(rank);
}
void menu()
{
	int o;
	system("cls");
	printf("Menu: \n");
	printf("\t1. Continue\n");
	printf("\t2. Play\n");
	printf("\t3. High score\n");
	printf("\t4. Exit\n");
	do
		o=getch();
	while (o!='1' && o!='2' && o!='3' && o!='4');
	system("cls");
	switch(o)
	{
		case '2':
			setup();
		case '1':
			cont();
			break;
		case '3':
			printf("High score:\n");
			hiprint();
			printf("Press any key to continue.\n");
			getch();
			break;
		case '4':
			printf("Press any key to exit.\n");
			getch();
			exit(0);
	}
}
int main()
{
	while(1)
		menu();
	return 0;
}
