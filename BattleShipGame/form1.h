// Form1.h
#pragma once // Standard preprocessor directive to ensure this header file is included only once during compilation.

#include <cstdlib> // Includes the C standard library for general utilities, e.g., rand(), srand() for random number generation.
#include <ctime>   // Includes the C time library, often used with <cstdlib> to seed the random number generator (srand(time(0))).
#include "BattleshipGame.h" // Includes a custom header file, likely containing the core game logic class (BattleshipGameLogic).
#include <msclr/marshal_cppstd.h> // Includes MSCLR (Microsoft C++ Language Runtime) utilities for marshalling (converting) between .NET System::String and C++ std::string.
#include <msclr/lock.h>         // Includes MSCLR utility for simplified locking, often used for thread synchronization with a critical section.

using namespace System; // Brings the System namespace into scope, allowing direct use of its classes (e.g., String, EventArgs) without System:: prefix.
using namespace System::ComponentModel; // Brings types for component model services (e.g., Container, BackgroundWorker).
using namespace System::Collections; // Brings types for collections (e.g., ArrayList, Hashtable - though Generic::List is often preferred).
using namespace System::Collections::Generic; // Brings types for generic collections (e.g., List<T>, Queue<T>, Dictionary<T, U>).
using namespace System::Windows::Forms; // Brings types for creating Windows Forms applications (e.g., Form, Button, Label).
using namespace System::Data; // Brings types for ADO.NET data access (not heavily used here it seems, but common in WinForms).
using namespace System::Drawing; // Brings types for GDI+ graphics (e.g., Point, Size, Color, Bitmap).
using namespace System::Net; // Brings types for network programming (e.g., IPAddress, Dns).
using namespace System::Net::Sockets; // Brings types for network sockets (e.g., TcpClient, TcpListener, NetworkStream).
using namespace System::Threading; // Brings types for multithreading (e.g., Thread, Mutex, Monitor).
using namespace System::Text; // Brings types for text encoding and manipulation (e.g., Encoding, StringBuilder).
using namespace System::IO; // Brings types for file and data stream I/O (e.g., File, StreamReader, StreamWriter).
using namespace System::Media; // Brings types for playing system sounds and .wav files (e.g., SoundPlayer).

namespace BattleshipGame { // Declares a namespace for the game to organize code and prevent naming conflicts.

    public ref class Form1 : public System::Windows::Forms::Form // Declares the main form class, Form1. 'public ref class' signifies a managed C++/CLI class. It inherits from System::Windows::Forms::Form.
    {
    public: // Public access specifier for members that follow.
        Form1(void); // Constructor for the Form1 class.

    protected: // Protected access specifier for members that follow.
        ~Form1() { // Destructor for the Form1 class (called when the object is finalized/disposed).
            if (components) { delete components; components = nullptr; } // If the 'components' container (for designer-managed controls) exists, delete it and set to nullptr to prevent dangling pointers.
            if (backgroundMusicPlayer != nullptr) { backgroundMusicPlayer->Stop(); } // If the background music player exists, stop the music.
            CleanUpNetworkResources(); // Calls a custom method to release network-related resources.
            if (gameLogicServer) { delete gameLogicServer; gameLogicServer = nullptr; } // If the game logic object (unmanaged) exists, delete it and set to nullptr.
            if (messageProcessTimer != nullptr) { // If the message processing timer exists...
                if (messageProcessTimer->Enabled) messageProcessTimer->Stop(); // ...and it's enabled, stop it.
                delete messageProcessTimer; messageProcessTimer = nullptr; // Delete the timer object and set to nullptr.
            }
        }

    private: // Private access specifier for members that follow.
        System::ComponentModel::Container^ components; // A container for components used by the form designer. Managed object.
        GroupBox^ networkSetupGroupBox; RadioButton^ pvpRadioButton; // UI: GroupBox for network settings. UI: RadioButton for selecting Player vs. Player mode.
        TextBox^ playerNameTextBox; Label^ playerNameLabel; // UI: TextBox for player name input. UI: Label for the player name textbox.
        TextBox^ serverIpTextBox; Label^ serverIpLabel; // UI: TextBox for server IP input. UI: Label for the server IP textbox.
        TextBox^ portTextBox; Label^ portLabel; // UI: TextBox for port number input. UI: Label for the port textbox.
        Button^ hostGameButton; Button^ joinGameButton; // UI: Button to host a game. UI: Button to join a game.
        Button^ controlButton; Label^ statusLabel; // UI: A multi-purpose control button (e.g., "Start Game", "Ready"). UI: Label to display game status messages.
        TableLayoutPanel^ playerOwnBoardPanel; TableLayoutPanel^ playerTrackingBoardPanel; // UI: Panel to display the player's own ship grid. UI: Panel to display the opponent's grid for tracking shots.
        Label^ ownBoardLabel; Label^ trackingBoardLabel; // UI: Label for the player's own board. UI: Label for the tracking board.

        BattleshipGameLogic* gameLogicServer; // Pointer to an instance of the unmanaged C++ BattleshipGameLogic class, holding the game's rules and state.

        bool isHost; bool isConnected; int myPlayerId; // Game state: True if this instance is hosting the game. True if connected to an opponent/server. Player ID (e.g., 0 or 1).
        String^ myNameInternal; String^ opponentName; // Game state: This player's name. Opponent's name. (Managed System::String).
        bool gameActive; bool isMyTurn; // Game state: True if the game is currently in progress. True if it's this player's turn.
        bool clientSentReady; bool hostAcknowledgedClientReady; // Game state flags for ready synchronization between host and client.

        TcpListener^ tcpListener; TcpClient^ opponentClient; NetworkStream^ opponentStream; // Networking: Listens for incoming TCP connections (for host). Represents the TCP connection to the opponent (for host). Stream for sending/receiving data with the opponent (for host).
        TcpClient^ serverConnection; NetworkStream^ serverStream; // Networking: Represents the TCP connection to the server (for client). Stream for sending/receiving data with the server (for client).
        Thread^ listenThread; Thread^ receiveThread; // Threading: Thread for the host to listen for client connections. Thread for receiving messages from the opponent/server.

        System::Collections::Generic::Queue<String^>^ UIMessageQueue; // A queue to hold messages received from network threads, to be processed by the UI thread.
        Object^ queueLock; // An object used for locking access to the UIMessageQueue to ensure thread safety.
        System::Windows::Forms::Timer^ messageProcessTimer; // A UI timer that periodically checks the UIMessageQueue to process messages on the UI thread.

        array<Button^, 2>^ ownBoardButtons; // A 2D array of Buttons representing the cells on the player's own game board.
        array<Button^, 2>^ trackingBoardButtons; // A 2D array of Buttons representing the cells on the opponent's game board (tracking board).
        SoundPlayer^ backgroundMusicPlayer; // Object to play background music.

        literal int GRID_BUTTON_SIZE = 32; // A compile-time constant defining the size (width/height) of the grid buttons in pixels.

        void InitializeComponent(void); // Standard method generated by the Windows Forms Designer to initialize all UI controls.
        void InitializeGameGrids(void); // Custom method to set up the game board grids (player's and tracking).
        void CreateGridButtonsForPanel(TableLayoutPanel^ panel, array<Button^, 2>^% buttonArray, bool isClickableForAttack); // Helper method to dynamically create and add buttons to a given TableLayoutPanel.
        void UpdateUI(void); // Custom method to refresh the UI elements based on the current game state.
        void RedrawBoardsFromServerData(String^ p1BoardStr, String^ p2BoardStr); // Method to update the visual representation of both game boards based on string data (likely from the server/host).
        void Log(String^ message); // Method to display a message, probably in the statusLabel or a console/debug output.
        void StartHosting(); void ListenForConnections(); void HandleClientConnection(TcpClient^ client); // Networking methods for hosting: Initiate hosting. Start listening for clients. Handle a new client connection.
        void StartJoining(); // Networking method for a client to initiate joining a game.
        void ReceiveMessages(Object^ streamObj); // Method executed on a separate thread to continuously receive messages from the network stream.
        void ProcessUIMessage(String^ message); // Method to process a single message from the UIMessageQueue on the UI thread.
        void SendNetMessage(NetworkStream^ stream, String^ message); // Method to send a message over a given NetworkStream.
        void CleanUpNetworkResources(); // Method to close sockets, streams, and stop threads related to networking.
        void ResetGameAndUI(); // Method to reset the game state and UI elements to their initial state for a new game.

        // Corrected HandleDisconnection structure
        void HandleDisconnection(String^ reason); // Method called when a disconnection is detected, takes a reason string.
        void HandleDisconnection_UIThreadAction();  // Parameterless helper for UI thread: Method to perform UI updates related to disconnection, designed to be invoked on the UI thread.

        delegate void StringArgDelegate(String^ text); void LogOnUIThread(String^ text); // Delegate for methods taking a String^ argument. Method to log messages safely on the UI thread using this delegate.
        delegate void VoidDelegate(); void UpdateUIOnUIThread(); // Delegate for methods with no arguments and no return value. Method to update the UI safely on the UI thread.
        delegate void ControlBoolDelegate(Control^ ctl, bool enable); void EnableControlOnUI(Control^ ctl, bool enable); // Delegate for methods taking a Control^ and a bool. Method to enable/disable a control safely on the UI thread.

        void OnPvpRadioButtonChanged(Object^ sender, EventArgs^ e); // Event handler for when the PvP radio button's checked state changes.
        void OnHostGameClick(Object^ sender, EventArgs^ e); void OnJoinGameClick(Object^ sender, EventArgs^ e); // Event handler for Host Game button click. Event handler for Join Game button click.
        void OnControlButtonClick(Object^ sender, EventArgs^ e); // Event handler for the multi-purpose controlButton click.
        void OnTrackingBoardClick(Object^ sender, EventArgs^ e); // Event handler for when a button on the tracking board is clicked (player making a shot).
        void OnFormClosing(Object^ sender, FormClosingEventArgs^ e); // Event handler for when the form is about to close. Used for cleanup.
        void OnMessageProcessTimerTick(Object^ sender, EventArgs^ e); // Event handler for the messageProcessTimer's Tick event.
        void Form1_Load(Object^ sender, EventArgs^ e); // Event handler for the Form's Load event, which occurs before the form is displayed for the first time.
    }; // End of Form1 class definition.
} // End of BattleshipGame namespace.