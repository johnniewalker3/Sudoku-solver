#include <iostream>
#include <map>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <bitset>
using namespace std;

void NonZeroElementsInRows(int sudoku[][9], bitset<10> constant_values[]){
    for (int i=0;i<9;i++){
        for (int j=0;j<9;j++)
        {
            if (sudoku[i][j] != 0)
               constant_values[i][sudoku[i][j]] = 1;
        }
    }
    
}

void checkColumn(int sudoku[][9], bitset<10> &column_values, int col){
    for (int i=0;i<9;i++){
        if (sudoku[i][col] != 0)
           column_values[sudoku[i][col]] =1;
    }
}

void checkSquare(int sudoku[][9], bitset<10> &square_values, int row, int col){
    int r = (row%3 == 0)?(row):((row-1)%3 == 0)?(row-1):(row-2);
    int c = (col%3 == 0)?(col):((col-1)%3 == 0)?(col-1):(col-2);

    for (int i=r;i<r+3;i++){
        for (int j=c;j<c+3;j++)
        {
            if (sudoku[i][j] != 0)
               square_values[sudoku[i][j]] = 1;
        }
    }
}
bool canEnter(bitset<10> changed_values[], bitset<10> column_values, bitset<10> square_values,int row,int value){
     return ((changed_values[row][value] == 0)  && (column_values[value] == 0) && (square_values[value] == 0));
}




void sudoku_solver(bitset<10> constant_values[], bitset<10> changed_values[], int sudoku[][9], int row,int col, bool &final_flag, bool &flag){
       int count = 0;
       bitset<10> column_values;
       bitset<10> square_values;

       for (int i=0;i<9;i++){
           for (int j=0;j<9;j++){
               if (sudoku[i][j] == 0)
               {
                   count++;
                   break;
               }
           }
           if (count > 0)
              break;
       }
       if (count == 0){
           final_flag = true;
           return;
       }

       row += (col/9);
       col %= 9;

       if (row<=8){
           if (sudoku[row][col] != 0 && constant_values[row][sudoku[row][col]] == 1)
              sudoku_solver(constant_values, changed_values, sudoku, row, col+1, final_flag, flag);
           else if (sudoku[row][col] == 0){
               for (int i=1;i<=9;i++){
                   checkColumn(sudoku, column_values, col);
                   checkSquare(sudoku, square_values, row, col);
                   flag = canEnter(changed_values,column_values,square_values, row, i);
                   if (flag){
                      sudoku[row][col] = i;
                      changed_values[row][sudoku[row][col]] = 1;
                      column_values[sudoku[row][col]] = 1;
                      square_values[sudoku[row][col]] = 1;
                      sudoku_solver(constant_values, changed_values,sudoku, row, col+1, final_flag, flag);
                      if (final_flag)
                         return;
                      if (flag == false){
                          changed_values[row][sudoku[row][col]] = 0;
                          column_values[sudoku[row][col]] = 0;
                          square_values[sudoku[row][col]] = 0;
                          sudoku[row][col] = 0;
                          
                      }
                   }
               }
           }
              
       }

       
        
}


int main(){
    srand(time(0));
    bool final_flag = false, flag =true;
    int sudoku[][9] = {{0, 0, 0, 4, 0, 0, 2, 0, 0},
                        {0, 0, 6, 5, 0, 0, 0, 0, 0},
                        {1, 0, 0, 0, 6, 2, 0, 7, 0},
                        {0, 0, 3, 0, 2, 1, 8, 0, 0},
                        {0, 0, 0, 0, 0, 4, 0, 0, 0},
                        {0, 8, 0, 0, 0, 0, 0, 5, 0},
                        {0, 0, 1, 0, 3, 8, 5, 0, 0},
                        {0, 0, 0, 6, 0, 0, 0, 0, 0},
                        {7, 0, 0, 0, 0, 0, 0, 0, 9}};

   bitset<10> constant_values[9];//non zero elements for every row that are not allowed to change by the user.Of course we ignore the first bit
   bitset<10> changed_values[9];//non zero elements for every row that are allowed to change by the user
   bitset<10> column;

   for (int i=0;i<9;i++)
       constant_values[i][0] = 0;
   
   NonZeroElementsInRows(sudoku, constant_values);
   
   for (int i=0;i<9;i++){
       for (int j=0;j<10;j++)
           changed_values[i][j] = constant_values[i][j];
   }
   

   sudoku_solver(constant_values, changed_values, sudoku, 0, 0, final_flag, flag);

   for (int i=0;i<9;i++){
       for (int j=0;j<9;j++)
           cout<<sudoku[i][j]<<" ";
       cout<<endl;
   }

   if (final_flag == true)
      cout<<"Solution exist";
   else
      cout<<"Solution does not exist"<<endl;

  
}