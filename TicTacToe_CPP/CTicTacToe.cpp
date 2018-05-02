#include "stdafx.h"
#include "CTicTacToe.h"
#include "Windowsx.h"

BOOL CTicTacToe::GetGameBoardRect(HWND hwnd, RECT* pRect)
{
	RECT rc;	/*
				RECT Structure1
				left -- Specifies the x-coordinate of the upper-left corner of a rectangle.
				top -- Specifies the y-coordinate of the upper-left corner of a rectangle.
				right -- Specifies the x-coordinate of the lower-right corner of a rectangle.
				bottom -- Specifies the y-coordinate of the lower-right corner of a rectangle.
				RECT rctA;
				rctA.left = 20;
				rctA.top = 30;
				rctA.right  = 180;
				rctA.bottom = 230;
				RECT rctB = {20,30,180,230};
				*/
	if (GetClientRect(hwnd, &rc)) {
		int width = rc.right - rc.left; // right is width
		int height = rc.bottom - rc.top;// bottom is height
										// rc.left and rc.top would be useful when using GetWindowRect
										// find the four corners of the 3x3 table
		pRect->left = (width - cellSize * 3) / 2;
		pRect->top = (height - cellSize * 3) / 2;
		pRect->right = pRect->left + cellSize * 3;
		pRect->bottom = pRect->top + cellSize * 3;
		return TRUE;
	}
	SetRectEmpty(pRect);
	return FALSE;
}

//gives the coordinate values of the rectangle
int CTicTacToe::GetCellRect(HWND hWnd, int index, RECT* pRect) {

	RECT rcBoard;

	SetRectEmpty(pRect);

	if (index < 0 || index>8) {
		return false;
	}

	if (GetGameBoardRect(hWnd, &rcBoard)) {
		//convert index into x,y pair
		int y = index / 3; //row number
		int x = index % 3; //col number

		pRect->left = rcBoard.left + x * cellSize + 1;
		pRect->top = rcBoard.top + y * cellSize + 1;

		pRect->right = pRect->left + cellSize - 1;
		pRect->bottom = pRect->top + cellSize - 1;

		return true;
	}
}

// Draws a line given two sets of points
void CTicTacToe::DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	//Drawing lines
	MoveToEx(hdc, x1, y1, NULL); // The MoveToEx function updates the current position to the specified point and optionally returns the previous position.
	LineTo(hdc, x2, y2); // The LineTo function draws a line from the current position up to, but not including, the specified point.
}

int CTicTacToe::GetCellNumberFromPoint(HWND hwnd, int x, int y)
{
	RECT rc;
	POINT pt; // The POINT structure defines the x- and y-coordinates of a point
	pt.x = x; // Specifies the x-coordinate of a point
	pt.y = y; // Specifies the y-coordinate of a point
			  // POINT pt = {x,y}; // Alternate ways to initialize a POINT structure

	if (GetGameBoardRect(hwnd, &rc))
	{
		if (PtInRect(&rc, pt))
		{
			//user clicked inside gameboard
			//normalize (0 to 3*cellSize)
			x = pt.x - rc.left;
			y = pt.y - rc.top;

			int column = x / cellSize;
			int row = y / cellSize;

			//convert to index (0-8)
			return column + row * 3;
		}
	}
	return -1; //outside gameboard
}

int CTicTacToe::GetWinner(int wins[3]) {
	int cells[] = { 0,1,2,3,4,5,6,7,8,0,3,6,1,4,7,2,5,8,0,4,8,2,4,6 };

	//check for winner
	for (int i = 0; i < sizeof(cells) / sizeof(cells[0]); i += 3) {
		if (0 != gameBoard[cells[i]] && gameBoard[cells[i]] == gameBoard[cells[i + 1]] && gameBoard[cells[i]] == gameBoard[cells[i + 2]]) {
			wins[0] = cells[i];
			wins[1] = cells[i + 1];
			wins[2] = cells[i + 2];

			return gameBoard[cells[i]];
		}
	}

	for (int i = 0; i < sizeof(gameBoard) / sizeof(gameBoard[0]); i++) {
		if (0 == gameBoard[i]) {
			return 0; //at least one cell is empty; continue to play
		}
	}

	return 3; //it's a draw
}

void CTicTacToe::ShowTurn(HWND hWnd, HDC hdc) {
	RECT rc;

	static const WCHAR szTurn1[] = L"Turn: Player 1";
	static const WCHAR szTurn2[] = L"Turn: Player 2";

	const WCHAR* pszTurnText = NULL;

	switch (winner) {
	case 0: //continue to play
		pszTurnText = (playerTurn == 1) ? szTurn1 : szTurn2;
		break;

	case 1: //continue to play
		pszTurnText = L"Player 1 Wins!";
		break;

	case 2: //continue to play
		pszTurnText = L"Player 1 Wins!";
		break;

	case 3: //continue to play
		pszTurnText = L"It's a draw!";
		break;

	default:
		pszTurnText = L"";
		break;
	}

	if (NULL != pszTurnText && GetClientRect(hWnd, &rc)) {
		rc.top = rc.bottom - 48;
		FillRect(hdc, &rc, (HBRUSH)GetStockObject(GRAY_BRUSH));
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, pszTurnText, lstrlen(pszTurnText), &rc, DT_CENTER);
	}
}