#include <stdio.h>
#include "life.h"

// When completed this file will contain several versions of the
// life() function and conditional compilation will be used to
// determine which version runs.  Each version will be named life1,
// life2, and so on. To select the version of the life function to
// use, change the following declaration to the version of the life()
// to be used.  All versions of life() must produce a correct result.

// The provided Makefile also includes the possibility of creating an
// executable called "timelife-lifeN" (replacing lifeN with any
// version you'd like to test). You may compile it with "make
// timelife-lifeN" then run "./timelife-lifeN".

#ifndef LIFE_VERSION
#define LIFE_VERSION life8
#endif

// You are only allowed to change the contents of this file with
// respect to improving the performance of this program. You may not
// change the data structure or parameters for the function life.


// If you need to initialize some data structures for your implementation
// of life then make changes to the following function, otherwise
// do not change this function.

void initLife() {
}

// You are required to document the changes you make in the README.txt
// file. For each entry in the README.txt file there is to be a
// version of the matching life() function here such that the markers
// can see, and run if needed, the actual code that you used to
// produce your results for the logged change to the life function.

static inline int life0(long oldWorld[N][N], long newWorld[N][N]) {
  return base_life(oldWorld, newWorld);
}

// For each version of life you are testing duplicate the function
// below, rename it to a different name, and make your changes. To use
// the new version, change the #define above to use the current version.

static inline int life1(long oldWorld[N][N], long newWorld[N][N]) {

  int i, j;
  int col, row;

  //clear the new world
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
      newWorld[j][i] =  0;
    }

  // Count the cells to the top left
  for (i = 0; i < N; i++) {
    col = (i -1 + N) % N;
    for (j = 0; j < N; j++) {
      row = (j - 1 + N ) % N;
      newWorld[j][i] += oldWorld[row][col];
    }
  }

  // Count the cells immediately above
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      row = (j - 1 + N ) % N;
      newWorld[j][i] += oldWorld[row][i];
    }
  }

  // Count the cells to the top right
  for (i = 0; i < N; i++) {
    col = (i + 1 + N) % N;
    for (j = 0; j < N; j++) {
      row = (j - 1  + N ) % N;
      newWorld[j][i] += oldWorld[row][col];
    }
  }

  // Count the cells to the immediate left
  for (i = 0; i < N; i++) {
    col = (i -1 + N) % N;
    for (j = 0; j < N; j++) {
      newWorld[j][i] += oldWorld[j][col];
    }
  }

  // Count the cells to the immediate right
  for (i = 0; i < N; i++) {
    col = (i + 1 + N) % N;
    for (j = 0; j < N; j++) {
      newWorld[j][i] += oldWorld[j][col];
    }
  }

  // Count the cells to the bottom left
  for (i = 0; i < N; i++) {
    col = (i - 1 + N) % N;
    for (j = 0; j < N; j++) {
      row = (j + 1 + N ) % N;
      newWorld[j][i] += oldWorld[row][col];
    }
  }

  // Count the cells immediately below
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      row = (j + 1  + N ) % N;
      newWorld[j][i] += oldWorld[row][i];
    }
  }

  // Count the cells to the bottom right
  for (i = 0; i < N; i++) {
    col = (i + 1 + N) % N;
    for (j = 0; j < N; j++) {
      row = (j + 1  + N ) % N;
      newWorld[j][i] += oldWorld[row][col];
    }
  }

  // Check each cell to see if it should come to life, continue to live, or die
  int alive = 0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
      newWorld[j][i] = checkHealth(newWorld[j][i], oldWorld[j][i]);
      alive += newWorld[j][i] ? 1:0;
    }

  return alive;
}

static inline int life2(long oldWorld[N][N], long newWorld[N][N]) {

  int i, j;
  int col, row;

  //clear the new world
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
      // IMPROVEMENT 1: row-major order
      newWorld[i][j] =  0;
    }

  // Count the cells to the top left
  for (i = 0; i < N; i++) {
    col = (i -1 + N) % N;
    for (j = 0; j < N; j++) {
      row = (j - 1 + N ) % N;
      newWorld[j][i] += oldWorld[row][col];
    }
  }

  // Count the cells immediately above
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      row = (j - 1 + N ) % N;
      newWorld[j][i] += oldWorld[row][i];
    }
  }

  // Count the cells to the top right
  for (i = 0; i < N; i++) {
    col = (i + 1 + N) % N;
    for (j = 0; j < N; j++) {
      row = (j - 1  + N ) % N;
      newWorld[j][i] += oldWorld[row][col];
    }
  }

  // Count the cells to the immediate left
  for (i = 0; i < N; i++) {
    col = (i -1 + N) % N;
    for (j = 0; j < N; j++) {
      newWorld[j][i] += oldWorld[j][col];
    }
  }

  // Count the cells to the immediate right
  for (i = 0; i < N; i++) {
    col = (i + 1 + N) % N;
    for (j = 0; j < N; j++) {
      newWorld[j][i] += oldWorld[j][col];
    }
  }

  // Count the cells to the bottom left
  for (i = 0; i < N; i++) {
    col = (i - 1 + N) % N;
    for (j = 0; j < N; j++) {
      row = (j + 1 + N ) % N;
      newWorld[j][i] += oldWorld[row][col];
    }
  }

  // Count the cells immediately below
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      row = (j + 1  + N ) % N;
      newWorld[j][i] += oldWorld[row][i];
    }
  }

  // Count the cells to the bottom right
  for (i = 0; i < N; i++) {
    col = (i + 1 + N) % N;
    for (j = 0; j < N; j++) {
      row = (j + 1  + N ) % N;
      newWorld[j][i] += oldWorld[row][col];
    }
  }

  // Check each cell to see if it should come to life, continue to live, or die
  int alive = 0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
      newWorld[j][i] = checkHealth(newWorld[j][i], oldWorld[j][i]);
      alive += newWorld[j][i] ? 1:0;
    }

  return alive;
}

static inline int life3(long oldWorld[N][N], long newWorld[N][N]) {

  int i, j;
  int col, row;

  //clear the new world
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
      // IMPROVEMENT 1: row-major order
      newWorld[i][j] =  0;
    }

  // Count the cells to the top left
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    row = (i - 1 + N) % N;
    for (j = 0; j < N; j++) {
      col = (j - 1 + N) % N;
      newWorld[i][j] += oldWorld[row][col];
    }
  }

  // Count the cells immediately above
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    row = (i - 1 + N) % N;
    for (j = 0; j < N; j++) {
      newWorld[i][j] += oldWorld[row][j];
    }
  }

  // Count the cells to the top right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    row = (i - 1 + N) % N;
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      newWorld[i][j] += oldWorld[row][col];
    }
  }

  // Count the cells to the immediate left
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    for (j = 0; j < N; j++) {
      col = (j - 1 + N) % N;
      newWorld[i][j] += oldWorld[i][col];
    }
  }

  // Count the cells to the immediate right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      newWorld[i][j] += oldWorld[i][col];
    }
  }

  // Count the cells to the bottom left
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    row = (i + 1 + N) % N;
    for (j = 0; j < N; j++) {
      col = (j - 1 + N) % N;
      newWorld[i][j] += oldWorld[row][col];
    }
  }

  // Count the cells immediately below
  for (i = 0; i < N; i++) {
  // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    row = (i + 1 + N) % N;
    for (j = 0; j < N; j++) {
      newWorld[i][j] += oldWorld[row][j];
    }
  }

  // Count the cells to the bottom right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    row = (i + 1 + N) % N;
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      newWorld[i][j] += oldWorld[row][col];
    }
  }

  // Check each cell to see if it should come to life, continue to live, or die
  int alive = 0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
      newWorld[i][j] = checkHealth(newWorld[i][j], oldWorld[i][j]);
      alive += newWorld[i][j] ? 1:0;
    }

  return alive;
}

static inline int life4(long oldWorld[N][N], long newWorld[N][N]) {

  int i, j;
  // IMPROVEMENT 3: combine all accesses that are in the same row
  // add more local variables for left column and right column divisions for the rows
  int row, colL, colR;

  //clear the new world
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
      // IMPROVEMENT 1: row-major order
      newWorld[i][j] =  0;
    }

  // Count the cells to the top left, immediately above, and top right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    row = (i - 1 + N) % N;
    for (j = 0; j < N; j++) {
      colL = (j - 1 + N) % N;
      colR = (j + 1 + N) % N;
      newWorld[i][j] += oldWorld[row][colL] + oldWorld[row][j] + oldWorld[row][colR];
    }
  }

  // Count the cells to the immediate left and right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    for (j = 0; j < N; j++) {
      colL = (j - 1 + N) % N;
      colR = (j + 1 + N) % N;
      newWorld[i][j] += oldWorld[i][colL] + oldWorld[i][colR];
    }
  }

  // Count the cells to the bottom left, immediately below, and bottom right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    row = (i + 1 + N) % N;
    for (j = 0; j < N; j++) {
      colL = (j - 1 + N) % N;
      colR = (j + 1 + N) % N;
      newWorld[i][j] += oldWorld[row][colL] + oldWorld[row][j] + oldWorld[row][colR];
    }
  }

  // Check each cell to see if it should come to life, continue to live, or die
  int alive = 0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
      newWorld[i][j] = checkHealth(newWorld[i][j], oldWorld[i][j]);
      alive += newWorld[i][j] ? 1:0;
    }

  return alive;
}

static inline int life5(long oldWorld[N][N], long newWorld[N][N]) {

  int i, j;
  // IMPROVEMENT 3: combine all accesses that are in the same row
  // add more local variables for left column and right column divisions for the rows
  // IMPROVEMENT 4: change to variables saved within the loops
  int row, col;

  //clear the new world
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
      // IMPROVEMENT 1: row-major order
      newWorld[i][j] =  0;
    }

  // Count the cells to the top left, immediately above, and top right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    // IMPROVEMENT 4: store elements that have been read for the next iteration
    row = (i - 1 + N) % N;
    int prevLeft;
    int prevMid = oldWorld[row][N-1];
    int prevRight = oldWorld[row][0];
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      int temp = oldWorld[row][col];
      newWorld[i][j] += prevMid + prevRight + temp;
      prevLeft = prevMid;
      prevMid = prevRight;
      prevRight = temp; // current (temp) becomes prevRight after use in inner loop
    }
  }

  // Count the cells to the immediate left and right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    // IMPROVEMENT 4: store elements that have been read for the next iteration
    int prevMid = oldWorld[i][N-1];
    int prevRight = oldWorld[i][0];
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      int temp = oldWorld[i][col];
      newWorld[i][j] += prevMid + temp;
      prevMid = prevRight;
      prevRight = temp;
    }
  }

  // Count the cells to the bottom left, immediately below, and bottom right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    row = (i + 1 + N) % N;
    int prevLeft;
    int prevMid = oldWorld[row][N-1];
    int prevRight = oldWorld[row][0];
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      int temp = oldWorld[row][col];
      newWorld[i][j] += prevMid + prevRight + temp;
      prevLeft = prevMid;
      prevMid = prevRight;
      prevRight = temp;
    }
  }

  // Check each cell to see if it should come to life, continue to live, or die
  int alive = 0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
      newWorld[i][j] = checkHealth(newWorld[i][j], oldWorld[i][j]);
      alive += newWorld[i][j] ? 1:0;
    }

  return alive;
}

static inline int life6(long oldWorld[N][N], long newWorld[N][N]) {

  int i, j;
  // IMPROVEMENT 3: combine all accesses that are in the same row
  // add more local variables for left column and right column divisions for the rows
  // IMPROVEMENT 4: change to variables saved within the loops
  int row, col;

  // IMPROVEMENT 5: no need for clear, initialize in first loop

  // Count the cells to the top left, immediately above, and top right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    // IMPROVEMENT 4: store elements that have been read for the next iteration
    row = (i - 1 + N) % N;
    int prevLeft;
    int prevMid = oldWorld[row][N-1];
    int prevRight = oldWorld[row][0];
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      int temp = oldWorld[row][col];
      newWorld[i][j] = prevMid + prevRight + temp;
      prevLeft = prevMid;
      prevMid = prevRight;
      prevRight = temp; // current (temp) becomes prevRight after use in inner loop
    }
  }

  // Count the cells to the immediate left and right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    // IMPROVEMENT 4: store elements that have been read for the next iteration
    int prevMid = oldWorld[i][N-1];
    int prevRight = oldWorld[i][0];
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      int temp = oldWorld[i][col];
      newWorld[i][j] += prevMid + temp;
      prevMid = prevRight;
      prevRight = temp;
    }
  }

  // Count the cells to the bottom left, immediately below, and bottom right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    row = (i + 1 + N) % N;
    int prevLeft;
    int prevMid = oldWorld[row][N-1];
    int prevRight = oldWorld[row][0];
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      int temp = oldWorld[row][col];
      newWorld[i][j] += prevMid + prevRight + temp;
      prevLeft = prevMid;
      prevMid = prevRight;
      prevRight = temp;
    }
  }

  // Check each cell to see if it should come to life, continue to live, or die
  int alive = 0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
      newWorld[i][j] = checkHealth(newWorld[i][j], oldWorld[i][j]);
      alive += newWorld[i][j] ? 1:0;
    }

  return alive;
}

static inline int life7(long oldWorld[N][N], long newWorld[N][N]) {

  int i, j;
  // IMPROVEMENT 3: combine all accesses that are in the same row
  // add more local variables for left column and right column divisions for the rows
  // IMPROVEMENT 4: change to variables saved within the loops
  int row, col;

  // IMPROVEMENT 5: no need for clear, initialize in first loop

  // IMPROVEMENT 6: nest all inner loops into one for loop (rows)
  // Count the cells to the top left, immediately above, and top right
  for (i = 0; i < N; i++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    // IMPROVEMENT 4: store elements that have been read for the next iteration
    row = (i - 1 + N) % N;
    int prevLeft;
    int prevMid = oldWorld[row][N-1];
    int prevRight = oldWorld[row][0];
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      int temp = oldWorld[row][col];
      newWorld[i][j] = prevMid + prevRight + temp;
      prevLeft = prevMid;
      prevMid = prevRight;
      prevRight = temp; // current (temp) becomes prevRight after use in inner loop
    }

    // Count the cells to the immediate left and right
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    // IMPROVEMENT 4: store elements that have been read for the next iteration
    prevMid = oldWorld[i][N-1];
    prevRight = oldWorld[i][0];
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      int temp = oldWorld[i][col];
      newWorld[i][j] += prevMid + temp;
      prevMid = prevRight;
      prevRight = temp;
    }

    // Count the cells to the bottom left, immediately below, and bottom right
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
    // IMPROVEMENT 3: combine all accesses that are in the same row
    row = (i + 1 + N) % N;
    prevMid = oldWorld[row][N-1];
    prevRight = oldWorld[row][0];
    for (j = 0; j < N; j++) {
      col = (j + 1 + N) % N;
      int temp = oldWorld[row][col];
      newWorld[i][j] += prevMid + prevRight + temp;
      prevLeft = prevMid;
      prevMid = prevRight;
      prevRight = temp;
    }
  }

  // Check each cell to see if it should come to life, continue to live, or die
  int alive = 0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++) {
    // IMPROVEMENT 2: row-major order accesses, switch rows and columns
      newWorld[i][j] = checkHealth(newWorld[i][j], oldWorld[i][j]);
      alive += newWorld[i][j] ? 1:0;
    }

  return alive;
}

static inline int life8(long oldWorld[N][N], long newWorld[N][N]) {
  // IMPROVEMENT 7: added "register" and "unsigned"
  register unsigned int i, j, rowTop, rowBtm, colL, colR;
  register unsigned int alive = 0;

  // IMPROVEMENT 5: no need for clear, initialize in first loop
  // IMPROVEMENT 6: nest all inner loops into one for loop
  for (i = 0; i < N; i++) {
    colL = (i - 1 + N) % N;
    colR = (i + 1 + N) % N;

    for (j = 0; j < N; j++) {
      rowTop = (j - 1 + N) % N;
      rowBtm = (j + 1 + N) % N;

  // IMPROVEMENT 2: row-major order accesses, switch rows and columns
  // IMPROVEMENT 8: calculate all surrounding cells together
  // IMPROVEMENT 9: put checkHealth and alive into inner loop
  // IMPROVEMENT 10: input newly calculated newWorld directly into checkHealth, and alive statement
      alive += (checkHealth(oldWorld[colL][rowTop] + oldWorld[i][rowTop] + oldWorld[colR][rowTop]
             + oldWorld[colL][j] + oldWorld[colR][j]
             + oldWorld[colL][rowBtm] + oldWorld[i][rowBtm] + oldWorld[colR][rowBtm], oldWorld[i][j])) ? 1:0;
    }
  }
  return alive;
}

int life(long oldWorld[N][N], long newWorld[N][N]) {
  return LIFE_VERSION(oldWorld, newWorld);
}
