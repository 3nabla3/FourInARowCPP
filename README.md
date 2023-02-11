# Four In a Row C++

## Brief
The program contains a graphical version of the game "Connect Four" as well as a
minimax algorithm that can play the game. If both players are
human, it is just a normal game of Connect Four, if both players are minimax, then
just sick back and watch the computer play itself. Humans can also play against the 
computer by setting only one player to minimax. Before compiling the program, 
the `main.c` file can be modified to ensure a human or minimax plays Player1 or Player2.

## How to run
Tested on Ubuntu 22.04 LTS
```
sudo apt update
sudo apt install libxinerama-dev libxcursor-dev cmake ninja-build
git clone https://github.com/3nabla3/FourInARowCPP.git
cd FourInARowCPP
./setup.bash
```

## How to play
To select a column for the piece:
* click on the column with the cursor; or
* enter the number of the column on the keyboard (0 is the left-most column)

To restart the game at any point:
* press `r` on the keyboard

To quit the game:
* click the close button on the window

To view the minimax algorithm's though process
* look at the terminal from which the `./setup.bash` command was run


## How minimax works
The algorithm looks far into the future and decides what move is the best to play
making the hypothesis that the opponent will play their optimal move. The algorithm can
evaluate a board by assigning it a score centered around 0. The bigger the score is, 
the more likely Player1 is to win; the lower the score is, the more likely Player2 is
to win.

1) generate a tree of all possible boards up to a certain depth
   1) bigger depth = more insight = smarter minimax
   2) bigger depth = more boards to analyze = slower execution
2) assign a score to each leaf nodes using what is called a static analysis
   1) This analysis does not look at any future moves, just the board and the 
   pieces arranged on that board
   2) With two minimax algorithm of the same depth, the intelligence is determined
   by the accuracy of the static analysis
3) For each non-leaf node, assign its score to be the score of the best child
   1) If it's Player1's turn, the best child is the one with the biggest score
   2) If it's Player2's turn, the best child is the one with the lowest score
4) From the tree's head, chose the best child, now that the scores are known
   1) If it's Player1's turn, the best child is the one with the biggest score
   2) If it's Player2's turn, the best child is the one with the lowest score