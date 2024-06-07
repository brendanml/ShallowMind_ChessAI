# Development Process

## TODO
- web asm uploadable
- AI a bit smarter of an algorithm??
  - determine danger/kill squares of opposing player prior to ranking moves
  - prioritize AI putting opponent in safe checks
  - rewrite determine moves to insert a 3 if its a piece its protecting
- re-implement movement commands to make special moves possible
  - implement castling
  - en passant
- update when piece moves
  - special move stored in the move
    - reset this var end of turn/undo
- write read me

- fix stalemates
- add size/rescaling button
- sounds
- developer console

## DONE CHECKLIST
`checklist is reverse order (higher -> more recent)`
- pawns now auto upgrade to queens when passing the board
- fixed problem where pawns wasn't properly putting kings in check if on the last row
- game now checks for checkmate scenarios prompting end of game screen
- fixed attempting to restart game when piece selected causing game to crash
- made game rescaleable based on single variable
- refactored buttons that reset game state into a single function that resets all game state variables instead of being spread out everywhere
- fixed some UI bugs in regards to the win % chance bar
- reformatted draw functions
- Constructed command to validate that moves AI attempts to choose don't put itself in check
- Implemented AI to play against: constructed priority queue to add ranked moves, then AI picks a random move of the highest move value
- changed potentialmoves int array to std::vector array of moves so the AI can use the existing methods for choosing his moves
- rewrote all moves to use 'xy' structs instead of arbitrary std::vector indicies
- cleaned up code structure for readability and modularity
- fixed pawns jumping pieces on starting square
- fixed bug where selecting white king on anything other than the first move would cause the game to crash
- black king can now only make moves that puts him out of check (if in check)
- basing logic around whether next move gets player out of check (figured out it doesn't matter if you're in check)
- Fixed oversight where I was failing to re-copy the board state each turn to look for check scenarios
- implemented supplementray grids that use the now generic piece move determining functions to calculate whether a move would get a player out of check
- made commands update position of king indicies kept in board (for check scenarios)
- made move determining functions generic to handle multiple uses (bots using them with advanced state board, potential moves, etc)
- fixed bug where taking turn and undoing it wouldn't cause the game state of who's turn it was to change
- limited pieces to only move to valid positions
- made it possible to select different colors
- added title screen
- added highlight for selected piece
- fixed bug where you could move to the square your piece started (putting the piece back down)
- made score bar display who is currently winning
- pieces move and update score to std::out
- limited players to select their own pieces
- allowed undoing moves with cmd Z
- made commands to initialize pieces
- Render pieces from spritesheet on the board

## LEARNED
- implementing command pattern
- next time create game object and encompass everything within that, looks a bit messy