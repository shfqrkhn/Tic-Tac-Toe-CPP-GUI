#pragma once
class CTicTacToe
{
public:
	//Methods
	BOOL GetGameBoardRect(HWND hwnd, RECT* pRect);
	int GetCellRect(HWND hWnd, int index, RECT* pRect);
	void DrawLine(HDC hdc, int x1, int y1, int x2, int y2);
	int GetCellNumberFromPoint(HWND hwnd, int x, int y);
	int GetWinner(int wins[3]);
	void ShowTurn(HWND hWnd, HDC hdc);

	int gameBoard[9] = { 0,0,0,0,0,0,0,0,0 };
	int wins[3];

	//Getters
	int getCellSize() { return cellSize; }
	int getPlayerTurn() { return playerTurn; }
	int getWinner() { return winner; }

private:
	const int cellSize = 100;
	HBRUSH hbr1, hbr2;
	int playerTurn = 1;
	int winner = 0;
};