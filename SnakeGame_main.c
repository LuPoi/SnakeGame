#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h> //Import windows keyboard detection

// Define Grid Size
#define Width 25
#define Height 25

int PrintSomething[Width][Height];
int FoodX;
int FoodY;

struct LocationX{
	int NodeX;
	struct LocationX *next;
};

struct LocationY{
	int NodeY;
	struct LocationY *next;
};

void AssignListToPrintSomthing(struct  LocationX *X, struct LocationY *Y)
{
	int tempX;
	int tempY;
	int count = 0;

	while (X != NULL)
	{
		tempX = X->NodeX;
		tempY = Y->NodeY;

		if (count == 0)
			PrintSomething[tempX][tempY] = 3;
		else
			PrintSomething[tempX][tempY] = 2;

		X = X->next;
		Y = Y->next;

		count = count + 1;
	}
}

void AddNewNodeToFirst(struct LocationX** HeadOfLocationX, int NewLocationX, struct LocationY** HeadOfLocationY, int NewLocationY)
{
	/* 1. allocate node */
	struct LocationX* NewNodeX = (struct LocationX*) malloc(sizeof(struct LocationX));
	struct LocationY* NewNodeY = (struct LocationY*) malloc(sizeof(struct LocationY));

	/* 2. put in the data  */
	NewNodeX->NodeX = NewLocationX;
	NewNodeY->NodeY = NewLocationY;

	/* 3. Make next of new node as head */
	NewNodeX->next = (*HeadOfLocationX);
	NewNodeY->next = (*HeadOfLocationY);

	/* 4. move the head to point to the new node */
	(*HeadOfLocationX) = NewNodeX;
	(*HeadOfLocationY) = NewNodeY;
}

void DeleteLastNode(struct LocationX *X, struct LocationY *Y)
{

	struct LocationX *Xtemp;
	struct LocationY *Ytemp;

	while (X->next != NULL)
	{
		Xtemp = X;
		X = X->next;
		Ytemp = Y;
		Y = Y->next;
	}

	int tempX;
	int tempY;
	tempX = X->NodeX;
	tempY = Y->NodeY;
	PrintSomething[tempX][tempY] = 0;

	free(Xtemp->next);
	free(Ytemp->next);

	Xtemp->next = NULL;
	Ytemp->next = NULL;

}

void CreatRandomFood(struct LocationX* HeadOfLocationX, struct LocationY* HeadOfLocationY)
{
	int MatchFoodAndSnakeFlag = 1;

	struct LocationX *Xtemp;
	struct LocationY *Ytemp;

	while (MatchFoodAndSnakeFlag == 1) {
		FoodX=(rand() % 23) +1;
		FoodY=(rand() % 23) +1;

		Xtemp = HeadOfLocationX;
		Ytemp = HeadOfLocationY;
		while( Xtemp != NULL && Ytemp != NULL){
			if( Xtemp->NodeX == FoodX && Ytemp->NodeY == FoodY)
			{
				MatchFoodAndSnakeFlag = 1;
				break;
			}
			Xtemp = Xtemp->next;
			Ytemp = Ytemp->next;
			MatchFoodAndSnakeFlag = 0;
		}
	}

	PrintSomething[FoodX][FoodY] = 4;

}

int CheckMovement(struct LocationX* HeadOfLocationX, struct LocationY* HeadOfLocationY, int LocationXofHead, int LocationYofHead, int failflag)
{
	struct LocationX *Xtemp;
	struct LocationY *Ytemp;

	Xtemp = HeadOfLocationX;
	Ytemp = HeadOfLocationY;
	while( Xtemp != NULL && Ytemp != NULL){
		if( Xtemp->NodeX == LocationXofHead && Ytemp->NodeY == LocationYofHead)
		{
			failflag = 1;
			break;
		}
		Xtemp = Xtemp->next;
		Ytemp = Ytemp->next;
	}

	return failflag;

}

void PrintFromLocation()
{
	for (int j=0 ; j<Height ; j++)
	{
		for (int i=0 ; i<Width ; i++)
		{
			if (PrintSomething[i][j] == 1)
				printf(" #");
			else if (PrintSomething[i][j] == 2)
				printf(" *");
			else if (PrintSomething[i][j] == 0)
				printf("  ");

		}
	printf("\n");
	}
}

void CreateGrid()
{

	for (int i=0 ; i<Width ; i++)
	{
		PrintSomething[i][0]=1;
		PrintSomething[i][Height-1]=1;
	}

	for (int i=0 ; i<Height ; i++)
	{
		PrintSomething[0][i]=1;
		PrintSomething[Width-1][i]=1;
	}

}

void deleteLinkedList(struct LocationX** HeadOfLocationX, struct LocationY** HeadOfLocationY)
{
	struct LocationX *Xtemp;
	struct LocationY *Ytemp;
	while(*HeadOfLocationX) {
		Xtemp = *HeadOfLocationX;
		Ytemp = *HeadOfLocationY;
		*HeadOfLocationX = Xtemp->next;
		*HeadOfLocationY = Ytemp->next;
		free(Xtemp);
		free(Ytemp);
	}

	*HeadOfLocationX = NULL;
	*HeadOfLocationY = NULL;
}

void SetPrintDefault()
{
	for (int j=0 ; j<Height ; j++)
	{
		for (int i=0 ; i<Width ; i++)
		{
			PrintSomething[i][j] = 0;
		}
	}

	PrintSomething[12][13] = '5';


}

HANDLE hOutput, hOutBuf;//Control Screen Buffer Handle
COORD coord = { 0,0 };
//Display double buffer
DWORD bytes = 0;
char Output[Width][Height*2];

void display()
{

	for (int j=0 ; j<Height ; j++)
	{
		for (int i=0 ; i<Width ; i++)
		{

			if (PrintSomething[i][j] == 1)
			{
				Output[j][2*i]=' ';
				Output[j][2*i+1]='#';
			}
            else if (PrintSomething[i][j] == 2)
            {
				Output[j][2*i]=' ';
				Output[j][2*i+1]='O';
			}
			else if (PrintSomething[i][j] == 3)
			{
				Output[j][2*i]=' ';
				Output[j][2*i+1]='@';
			}
			else if (PrintSomething[i][j] == 4)
			{
				Output[j][2*i]=' ';
				Output[j][2*i+1]='+';
			}
			else if (PrintSomething[i][j] == 0)
			{
				Output[j][2*i]=' ';
				Output[j][2*i+1]=' ';
			}

		}
	}

	for (int i = 0; i < Height; i++)
	{
		coord.Y = i;
		WriteConsoleOutputCharacterA(hOutBuf, Output[i], Width*2, coord, &bytes);
	}
	SetConsoleActiveScreenBuffer(hOutBuf);
	// Sleep(10);

	for (int i = 0; i < Height; i++)
	{
		coord.Y = i;
		WriteConsoleOutputCharacterA(hOutput, Output[i], Width*2, coord, &bytes);
	}
	SetConsoleActiveScreenBuffer(hOutput);
	// Sleep(10);

}

void GameOverDisplay()
{

	for (int j=0 ; j<Height ; j++)
	{
		for (int i=0 ; i<Width ; i++)
		{

			if (PrintSomething[i][j] == 1)
			{
				Output[j][2*i]=' ';
				Output[j][2*i+1]='#';
			}
			else if (PrintSomething[i][j] == 2)
			{
				Output[j][2*i]=' ';
				Output[j][2*i+1]='O';
			}
			else if (PrintSomething[i][j] == 3)
			{
				Output[j][2*i]=' ';
				Output[j][2*i+1]='@';
			}
			else if (PrintSomething[i][j] == 4)
			{
				Output[j][2*i]=' ';
				Output[j][2*i+1]='+';
			}
			else if (PrintSomething[i][j] == 0)
			{
				Output[j][2*i]=' ';
				Output[j][2*i+1]=' ';
			}
		}
	}

	Output[13][8]='G';
	Output[13][9]='A';
	Output[13][10]='M';
	Output[13][11]='E';
	Output[13][12]='O';
	Output[13][13]='V';
	Output[13][14]='E';
	Output[13][15]='R';

	Output[13][16]=',';
	Output[13][17]=' ';

	Output[13][18]='P';
	Output[13][19]='R';
	Output[13][20]='E';
	Output[13][21]='S';
	Output[13][22]='S';

	Output[13][23]=' ';

	Output[13][24]='A';
	Output[13][25]='N';
	Output[13][26]='Y';

	Output[13][27]=' ';

	Output[13][28]='K';
	Output[13][29]='E';
	Output[13][30]='Y';

	Output[13][31]=' ';

	Output[13][32]='T';
	Output[13][33]='O';

	Output[13][34]=' ';

	Output[13][35]='C';
	Output[13][36]='O';
	Output[13][37]='N';
	Output[13][38]='T';
	Output[13][39]='I';
	Output[13][40]='N';
	Output[13][41]='U';
	Output[13][42]='E';

	for (int i = 0; i < Height; i++)
	{
		coord.Y = i;
		WriteConsoleOutputCharacterA(hOutBuf, Output[i], 50, coord, &bytes);
	}
	SetConsoleActiveScreenBuffer(hOutBuf);
	// Sleep(10);

	for (int i = 0; i < Height; i++)
	{
		coord.Y = i;
		WriteConsoleOutputCharacterA(hOutput, Output[i], 50, coord, &bytes);
	}
	SetConsoleActiveScreenBuffer(hOutput);
	// Sleep(10);
}

void WindowsPrintSetting(/* arguments */) {
	//Create New Control Buffer
	hOutBuf = CreateConsoleScreenBuffer(
		GENERIC_WRITE,//Buffer Data
		FILE_SHARE_WRITE,//Buffer Share Authority
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	hOutput = CreateConsoleScreenBuffer(
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	//Hide Buffer Cursor
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = 0;
	cci.dwSize = 1;
	SetConsoleCursorInfo(hOutput, &cci);
	SetConsoleCursorInfo(hOutBuf, &cci);
}

int main()
{
	WindowsPrintSetting();

	int NewX = 1;
	int NewY = 1;
	int GameOverTag = 0;
	int keyboardInput = 0;
	int monitor = 1; // Fixing monitor flash
	int FoodEat = 0;
	int failflag = 0;
	int KeyFlag = 0;

	struct LocationX *HeadOfLocationX = NULL;
	struct LocationY *HeadOfLocationY = NULL;
	CreateGrid();
	AddNewNodeToFirst(&HeadOfLocationX, 12, &HeadOfLocationY, 13);
	AssignListToPrintSomthing(HeadOfLocationX, HeadOfLocationY);
	CreatRandomFood(HeadOfLocationX,HeadOfLocationY);
	while (GameOverTag == 0)
	{

		if (kbhit()) {
			keyboardInput = getch();
		}

		if (keyboardInput == 75)
		{
			NewX = HeadOfLocationX -> NodeX - 1;
			NewY = HeadOfLocationY -> NodeY;
		}
		else if (keyboardInput == 77)
		{
			NewX = HeadOfLocationX -> NodeX + 1;
			NewY = HeadOfLocationY -> NodeY;
		}
		else if (keyboardInput == 72)
		{
			NewX = HeadOfLocationX -> NodeX;
			NewY = HeadOfLocationY -> NodeY - 1;
		}
		else if (keyboardInput == 80)
		{
			NewX = HeadOfLocationX -> NodeX ;
			NewY = HeadOfLocationY -> NodeY + 1;
		}

		if (keyboardInput == 75 || keyboardInput == 77 || keyboardInput == 72 || keyboardInput == 80)
		{
			failflag = CheckMovement(HeadOfLocationX, HeadOfLocationY, NewX, NewY, failflag);
			AddNewNodeToFirst(&HeadOfLocationX, NewX, &HeadOfLocationY, NewY);

			if (NewX == FoodX && NewY == FoodY)
				CreatRandomFood(HeadOfLocationX,HeadOfLocationY);
			else
				DeleteLastNode(HeadOfLocationX, HeadOfLocationY);

			AssignListToPrintSomthing(HeadOfLocationX, HeadOfLocationY);
		}

		if (NewX == 0 || NewY == 0 || NewX == 24 || NewY == 24 || failflag == 1)
		{
			deleteLinkedList(&HeadOfLocationX, &HeadOfLocationY);
			GameOverDisplay();
			keyboardInput = getch();
			if (keyboardInput == 27)
				break;
			NewX = 1;
			NewY = 1;
			keyboardInput = 1;
			failflag = 0;
			SetPrintDefault();
			CreateGrid();
			AddNewNodeToFirst(&HeadOfLocationX, 12, &HeadOfLocationY, 13);
			AssignListToPrintSomthing(HeadOfLocationX, HeadOfLocationY);
			CreatRandomFood(HeadOfLocationX,HeadOfLocationY);
		}

		if (monitor == 0) // monitor flash
		{
			system("CLS");
			PrintFromLocation();
			Sleep(50);
		}
		else
		{
			display();
			Sleep(90);
		}

	}


return 0;
}
