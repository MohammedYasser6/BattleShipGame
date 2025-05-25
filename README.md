# Battleship PvP Game

## Project Description

This is a classic Battleship game implemented in C++/CLI with Windows Forms for the user interface. The game supports Player vs. Player (PvP) gameplay over a local network (TCP/IP). Players can host a game or join an existing game hosted by another player on the same network.

The objective is to sink all of the opponent's ships before they sink yours.

## Features

*   **Player vs. Player (PvP) Mode:** Play against another human opponent over the network.
*   **Network Play:**
    *   Host a game and wait for an opponent to connect.
    *   Join an existing game by specifying the host's IP address and port.
*   **Classic Battleship Gameplay:**
    *   Standard 10x10 game grids.
    *   Default fleet of 5 ships: Carrier (5), Battleship (4), Cruiser (3), Submarine (3), Destroyer (2).
    *   Ships are placed randomly for both players at the start of a network game.
*   **Graphical User Interface:** Built with Windows Forms for intuitive interaction.
*   **Turn-based Combat:** Players take turns firing shots at the opponent's grid.
*   **Visual Feedback:** Hits, misses, and sunk ships are clearly indicated on the boards.
*   **Game Status Updates:** A status label provides information about game progress and whose turn it is.
*   **Background Music:** Includes a looping background audio track (requires `background_music.wav`).

## Technologies Used

*   **Language:** C++/CLI
*   **Framework:** .NET Framework (specifically for Windows Forms and Networking libraries)
*   **User Interface:** Windows Forms
*   **Networking:** `System::Net::Sockets` (TCP/IP) for client-server communication.
*   **IDE:** Microsoft Visual Studio

## Project Structure

The project consists of the following main C++ source and header files:

*   **`Form1.h` / `Form1.cpp`:** Manages the main game window, UI interactions, network communication handling, and overall game flow coordination.
*   **`BattleshipGame.h` / `BattleshipGame.cpp`:** Contains the core game logic for a Battleship match, including managing players, processing attacks, and determining game state (win/loss). This is primarily used by the Host player.
*   **`Player.h` / `Player.cpp`:** Defines the `Player` class, which manages a player's own game board, their tracking board for the opponent, their ships, and handles ship placement and attack processing.
*   **`Ship.h` / `Ship.cpp`:** Defines the `Ship` class, representing individual ships with properties like name, size, and hit status.
*   **`main.cpp`:** The entry point for the Windows Forms application.

## How to Compile and Run

1.  **Prerequisites:**
    *   Microsoft Visual Studio (e.g., 2017, 2019, 2022) with the "Desktop development with C++" workload installed.
    *   Ensure the C++/CLI support component is included in your Visual Studio installation.
    *   .NET Framework (the version targeted by the project, likely 4.x).

2.  **Open the Project:**
    *   Open the `.sln` (solution) file in Visual Studio.

3.  **Build the Project:**
    *   From the Visual Studio menu, select `Build -> Build Solution`.
    *   This will compile the code and create an executable (e.g., `BattleShipGame.exe`) in the output directory (typically `bin\Debug\` or `bin\Release\` relative to your project folder).

4.  **Music File (Important):**
    *   Ensure you have a `.wav` file named `background_music.wav`.
    *   This file must be placed in the **same directory as the compiled `BattleShipGame.exe` file** for the background music to play. If you followed the steps to include it as "Content" with "Copy to Output Directory", this should happen automatically when you build.

5.  **Running the Game:**
    *   Navigate to the output directory (e.g., `YourProjectPath\BattleShipGame\bin\Debug\`).
    *   Run `BattleShipGame.exe`.
    *   To play PvP:
        *   One player starts the application and clicks "Host Game". They should note their IP address if not playing on `localhost`.
        *   The second player starts the application on another computer (or the same computer for testing with `localhost`).
        *   The second player enters the Host's IP address (or `localhost`) and the same port number (default is 12345), then clicks "Join Game".
        *   Once connected, both players click the "Ready" button.
        *   The game will start, typically with the Host taking the first turn.

## Gameplay Instructions

1.  **Setup:**
    *   Enter your player name.
    *   **To Host:** Click "Host Game".
    *   **To Join:** Enter the Host's IP and Port, then click "Join Game".
2.  **Ready Up:** Once connected, both players click the "Ready" button. Your ships will be placed randomly.
3.  **Taking Turns:**
    *   The status label will indicate whose turn it is.
    *   If it's your turn, click on a cell in the **"Attacking: [Opponent's Name]"** grid (your tracking grid) to fire a shot.
    *   **H** indicates a Hit.
    *   **M** indicates a Miss.
    *   Colors on the grid also indicate hits (Red) and misses (Blue).
4.  **Winning:** The first player to sink all of their opponent's ships wins the game.

## Author

*   **Mohammed Yasser**

