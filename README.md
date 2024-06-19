![ShallowMind: ChessAI](./Resources/banner.png)

## ShallowMind ChessAI

* [**NOTE** ***CASTLING*** has **not** *yet* been implemented](#castling).

### Description
* ShallowMind ChessAI is a one and two player MacOS desktop application.

### How to Play?
* **One** player: Click one player, select your colour, and begin playing against ShallowMind!
* **Two** player: Click two player and pass the computer to the person playing with the white pieces.

### Controls
#### Keyboard
*  `CMD + Z`: undo move.
#### Mouse
* `LEFT CLICK`: Select Piece / Move Piece.

### Media
![players](./Resources/players.png)
![board](./Resources/board.png)

### Links
 - youtube.com: [YouTube Video](https://www.youtube.com/channel/UCnr_HtDpLTYRbjR-2wZvfKQ)
 - itch.io: [ShallowMind Chess AI](https://badongo.itch.io/shallowmind-chessai)

### Developers
 - Brendan Michael Lynch - Application Designer.
 - Chess assets are courtesy of [Wildlife Studios](https://wildlifestudios.itch.io/chess-set-pixel-art)

> *Copyright (c) 2024 Brendan Lynch github/brendanml*

## Notes On Development

### Inspiration
- Growing up an avid chess player I was motivated to write my own Chess AI following an introductory course on AI at university.

### How I built it...
- C++, raylib, statically built
- apple application bundle

### Things I've Learned
- Things are always more challenging to implement than to plan...
- git forking
- C++ object structuring
- command pattern

#### Castling
* Castling is not currently possible... I began coding with an incomplete understanding of the problem, I will try and detail this below.
  * I implemented the majority of the code logic by selecting a piece (origin) & another square (desination) and if the piece is capable of moving there... move the origin piece to the desintation square.
  * Castling, obviously, does not follow this pattern...
    * The majority of the code logic runs on the origin & destination pattern, and altering it breaks: person movements, AI movements and decision logic & check scenarios.

### What's next for ShallowMind
- Plan on creating a fork re-writing a large portion of the code base in order to both clean-up the codebase as well as make moves like castling possible.

### ShallowMind
- ShallowMind is a heuristic-based AI that is only concerned with the current board state (does not consider results of it's actions).

