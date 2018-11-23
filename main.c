#include <stdio.h>
#include <stdlib.h>

#define ROWS 8
#define COLUMNS 8
#define LENGTH 5  // palindrome length


//For algorithm the matrix is filled from above
// printBoard() function prints the matrix reversed to show inputs from beyond

void boardInit(char board[ROWS][COLUMNS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            board[i][j] = '-';
        }
    }
}

void printBoard(char board[ROWS][COLUMNS]) {
    for (int i = ROWS - 1; i >= 0; i--) {
        for (int j = 0; j < COLUMNS; j++) {
            printf(" %c", board[i][j]);
        }
        printf("\n");
    }
}

int getInput() {
    int usrInput;
    int count = 0;
    int input;

    while ((usrInput = getchar()) != EOF && usrInput != '\n') {
        count++;
        input = usrInput;
    }
    if (usrInput == EOF) return EOF;

    if (count == 0 || count > 1) return '*'; //default invalid input
    return input;
}

int movePlayer(int columnChar, int pawnsInColumns[], char board[ROWS][COLUMNS], char turn) {
    int column = columnChar - 'a';
    int row = pawnsInColumns[column]++;

    board[row][column] = turn;

    return column;
}

int isValidColumn(int columnChar) {
    return columnChar >= 'a' && columnChar <= 'h';
}

int isColumnFilled(int columnChar, int pawnsInColumns[]) {
    int column = columnChar - 'a';
    return pawnsInColumns[column] != COLUMNS;
}


int isValidMove(int columnChar, int pawnsInColumns[]) {
    if (columnChar == '=' || columnChar == EOF)
        return 1;
    if (!isValidColumn(columnChar))
        return 0;
    if (!isColumnFilled(columnChar, pawnsInColumns))
        return 0;
    return 1;
}

int isPalindromeForColumn(int column, int start, int end, char board[ROWS][COLUMNS]) {
    for (int i = 0; i < LENGTH / 2; i++) {
        if (board[i + start][column] != board[end - i][column]) {
            return 0;
        }
    }
    return 1;
}

int isPalindromeForRow(int row, int start, int end, char board[ROWS][COLUMNS]) {
    for (int i = 0; i < LENGTH / 2; i++) {
        if (board[row][i + start] != board[row][end - i]) {
            return 0;
        }
    }
    return 1;
}

int isPalindromeForDiagonal(int startRow, int startColumn, int endRow, int endColumn, char board[ROWS][COLUMNS]) {
    int rowStep = (endRow - startRow) / abs(endRow - startRow);
    int k = 0;

    for (int i = startRow, j = startColumn; i != (startRow + endRow) / 2; i += rowStep, j++) {
        if (board[i][j] != board[endRow - (rowStep * k++)][endColumn - j + startColumn] ||  board[i][j] == '-') {
            return 0;
        }
    }
    if(board[(startRow+endRow)/2][(startColumn+endColumn)/2] == '-')
        return 0;

    return 1;
}

int playerWon(int column, char board[ROWS][COLUMNS], int pawnsInColumns[]) {
    int pawnsOnColumn = pawnsInColumns[column];

    //region column check
    if (pawnsOnColumn >= LENGTH) {
        if (isPalindromeForColumn(column, pawnsOnColumn - LENGTH, pawnsOnColumn - 1, board)) {
            return 1;
        }
    }
    //endregion

    int row = pawnsOnColumn - 1;

    //region row check
    for (int j = column - LENGTH + 1; j <= column; j++) {
        if (j < 0 || (j + LENGTH) > COLUMNS) continue;

        int isRangeFilled = 1;

        for (int k = j; k < j + LENGTH; k++) {
            if (pawnsInColumns[k] < pawnsOnColumn) {
                isRangeFilled = 0;
                break;
            }
        }

        if (isRangeFilled) {
            if (isPalindromeForRow(row, j, j + LENGTH - 1, board)) {
                return 1;
            }
        }
    }
    //end region

    //region BackSlash diagonal (from left upper corner to right lower)
    for (int i = row - LENGTH + 1, j = column - LENGTH + 1; i <= row; i++, j++) {
        if (i < 0 || j < 0 || (i + LENGTH - 1) >= ROWS || (j + LENGTH - 1) >= COLUMNS) {
            continue;
        }
        if(isPalindromeForDiagonal(i, j, i + LENGTH - 1, j + LENGTH - 1, board)) {
            return 1;
        }
    }
    //endregion

    //region Slash diagonal (from left lower corner to right upper)
    for (int i = row + LENGTH - 1, j = column - LENGTH + 1; i >= row; i--, j++) {
        if (i >= ROWS || j < 0 || j + LENGTH - 1 >= COLUMNS) {
            continue;
        }
        if(isPalindromeForDiagonal(i, j, i - LENGTH + 1, j + LENGTH - 1, board)) {
            return 1;
        }
    }
    //endregion

    return 0;
}

void runGame() {
    static char board[ROWS][COLUMNS];
    static int pawnsInColumns[COLUMNS]; // Keep number of inputs for certain column
    static char turn = '1'; //Game starts first player then turn switches to second player and so on

    boardInit(board);
    
    do {
        printBoard(board);
        printf(" a b c d e f g h\n");
        printf("%c:\n", turn);
        int columnChar = getInput();
        if (columnChar == '=' || columnChar == EOF) {
            return;
        }
        if (!isValidMove(columnChar, pawnsInColumns))
            continue;

        int column = movePlayer(columnChar, pawnsInColumns, board, turn);
      
        if (playerWon(column, board, pawnsInColumns)) {
            printBoard(board);
            printf(" a b c d e f g h\n");
            printf("%c.\n", turn);
            return;
        }
        turn = (char) (turn == '1' ? '2' : '1'); // switch turns
    } while (1);
}

int main() {
    runGame();
    return 0;
}
