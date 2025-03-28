# Tetris_Game
# 🎮 Tetris Game (C++)

## Authors
- PRIYANSH
- NAISARGI
- NIRAJ
- VYOM

## 📜 Table of Contents
- Project Description  
- Key Features  
- Installation  
- Gameplay Controls  
- How To Play  
- Game Mechanics  
- Code Structure  
- License  
- Prerequisites  
- Contact  

## ✨ Project Description  
A classic Tetris game implemented in C++ with colorful console graphics. Features all 7 standard tetromino pieces, dynamic difficulty progression, score tracking, and smooth gameplay mechanics. Built for Windows using console functions.

## 🌟 Key Features
- **7 Tetromino Types**: I, O, T, S, Z, J, and L pieces with distinct colors  
- **Progressive Difficulty**: Game speeds up as you level up (every 3 lines cleared)  
- **Scoring System**: Points awarded for line clears (100-800 points depending on lines)  
- **Next Piece Preview**: See what's coming next  
- **Responsive Controls**: Supports both arrow keys and WASD  
- **Game States**: Pause, restart, and exit functionality  
- **Clean Console UI**: Colorful blocks with borders and info display  


## 🎮 Gameplay Controls  
| Key          | Action                  |  
|--------------|-------------------------|  
| ← or A       | Move piece left         |  
| → or D       | Move piece right        |  
| ↓ or S       | Move piece down         |  
| ↑ or W       | Rotate piece            |  
| Space        | Hard drop (instant fall)|  
| P            | Pause game              |  
| R            | Restart game            |  
| X            | Exit game               |  

## 🎮 How To Play  
1. Pieces fall from the top - guide them to create complete horizontal lines  
2. Completing lines makes them disappear and awards points  
3. Every 3 lines cleared increases your level (and game speed)  
4. Game ends when pieces stack to the top  
5. Use the preview to plan your next move  

## ⚙️ Game Mechanics  
- **Scoring**:  
  - 1 line: 100 pts  
  - 2 lines: 300 pts  
  - 3 lines: 500 pts  
  - 4 lines: 800 pts  

- **Level Progression**:  
  - Level increases every 3 lines cleared  
  - Each level increases falling speed  

- **Collision Detection**:  
  - Checks for walls, floor, and existing blocks  

## 🏗️ Code Structure  
### Main Components:  
1. **Tetromino Class**:  
   - Handles piece shapes, colors, rotation  
   - Manages current position  

2. **GameBoard Class**:  
   - Manages the 10x20 grid  
   - Handles line clearing and scoring  
   - Renders the game state  

3. **TetrisGame Class**:  
   - Main game loop  
   - Input handling  
   - Game state management  

### Key Methods:  
- `canMove()` - Checks valid piece positions  
- `placeTetromino()` - Locks piece in place  
- `clearLines()` - Handles line clearing logic  
- `rotate()` - Piece rotation logic  
- `hardDrop()` - Instant drop functionality  

## 📜 License  
MIT License - Free for educational and personal use.  

## 🧰 Prerequisites  
- Windows OS  
- C++ compiler (tested with g++)  
- Basic understanding of console applications  

## 📞 Contact  
- Email: [202401213@daiict.ac.in](mailto:202401213@daiict.ac.in)  
- GitHub Issues: [Priyansh-9021](https://github.com/Priyansh-9021/Tetris_Game)  
