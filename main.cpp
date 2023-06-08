#include <windows.h>
#include <ctime>
#include <iostream>
#include <list>

using namespace std;

HANDLE hStdout;
COORD cord;

int playerLocX = 50;
int playerLocY = 10;

int momentX = 0;
int momentY = 0;
int appleX = 0;
int appleY = 0;

int score = 0;

bool lose = false;

void PrintScore()
{
	cord.X = 110;
	cord.Y = 12;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdout, cord);
	cout << score;
};

void AppleSpawn(int x, int y)
{
	cord.X = x;
	cord.Y = y;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdout, cord);
	cout << "a";
}

void RandomAppleSpawn()
{
	srand(time(0));
	appleX = 10 + (std::rand() % (75 - 10 + 1));
	appleY = 10 + (std::rand() % (24 - 10 + 1));
	cord.X = appleX;
	cord.Y = appleY;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdout, cord);
	cout << "a";
}

void SetPlayer(int x, int y)
{
	cord.X = x;
	cord.Y = y;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdout, cord);
	cout << "s";
}

void SetTheMap()
{
	int x, y;
	x = 100;
	y = 25;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < x; i++)
	{
		cord.Y = 0;
		cord.X = i;
		SetConsoleCursorPosition(hStdout, cord);
		cout << "*";
		cord.Y = y;
		cord.X = i;
		SetConsoleCursorPosition(hStdout, cord);
		cout << "*";
	}
	for (int i = 0; i < y; i++)
	{
		cord.X = 0;
		cord.Y = i;
		SetConsoleCursorPosition(hStdout, cord);
		cout << "*";
		cord.X = x;
		cord.Y = i;
		SetConsoleCursorPosition(hStdout, cord);
		cout << "*";
	}
}

int main()
{
	list<int> tailX, tailY;
	while (true)
	{
		system("CLS");
		SetTheMap();

		// Kontroller
		if (GetKeyState('A') & 0x8000 && momentX != 1)
		{
			momentX = -1;
			momentY = 0;
		}

		if (GetKeyState('D') & 0x8000 && momentX != -1)
		{
			momentX = 1;
			momentY = 0;
		}

		if (GetKeyState('S') & 0x8000 && momentY != -1)
		{
			momentX = 0;
			momentY = 1;
		}

		if (GetKeyState('W') & 0x8000 && momentY != 1)
		{
			momentX = 0;
			momentY = -1;
		}
		// Kuyruğun takip etmesi
		if (tailX.size() > 0)
		{
			tailX.pop_front();
			tailY.pop_front();
			tailX.push_back(playerLocX);
			tailY.push_back(playerLocY);
		}

		// Elma yeme kontrolü
		if (appleX == playerLocX && appleY == playerLocY)
		{
			appleX = 0;
			appleY = 0;
			score++;
			if (tailX.size() >= 1)
			{
				auto x_front = tailX.begin();
				auto y_front = tailY.begin();
				advance(y_front, tailY.size() - 1);
				advance(x_front, tailX.size() - 1);
				tailX.push_back(*x_front);
				tailY.push_back(*y_front);
			}
			else
			{
				tailX.push_back(playerLocX);
				tailY.push_back(playerLocY);
			}
		}
		playerLocX = playerLocX + momentX;
		playerLocY = playerLocY + momentY;

		SetPlayer(playerLocX, playerLocY);

		// Elma yoksa elmayı spawnlama
		if (appleX == 0 && appleY == 0)
		{
			RandomAppleSpawn();
		}
		// elma mevcutsa elmayı render etme
		if (appleX != 0 || appleY != 0)
		{
			AppleSpawn(appleX, appleY);
		}

		// Kenarlara çarpma kontrolü
		if (playerLocX >= 100 || playerLocY >= 25 || playerLocX <= 0 || playerLocY <= -1)
		{
			break;
		}
		if (tailX.size() > 0)
		{
			for (int i = 0; i < tailX.size(); i++)
			{
				auto x_front = tailX.begin();
				auto y_front = tailY.begin();
				advance(x_front, i);
				advance(y_front, i);
				cord.X = *x_front;
				cord.Y = *y_front;
				hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleCursorPosition(hStdout, cord);
				cout << "s";
				if ((playerLocX == cord.X && tailX.size() > 1) && (playerLocY == cord.Y && tailY.size() > 1))
					lose = true;
			}
		}
		if (lose)
			break;
		PrintScore();
		Sleep(100);
	}
}