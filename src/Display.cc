#include "Display.h"

BoardDisplay::BoardDisplay(map<int, char> *whitePieces, map<int, char> *blackPieces): whitePieces{whitePieces}, blackPieces{blackPieces} {}

TextDisplay::TextDisplay(map<int, char> *whitePieces, map<int, char> *blackPieces): BoardDisplay{whitePieces, blackPieces} {}

GraphicsDisplay::GraphicsDisplay(map<int, char> *whitePieces, map<int, char> *blackPieces): BoardDisplay{whitePieces, blackPieces}, window{400, 400} {}

void TextDisplay::display() {
    for(int i = 7; i >= 0; --i) {
        std::cout << i+1 << " ";
        for(int j = 0; j < 8; ++j) {
            // Checks if white has a piece at that location and prints it if
            // true
            if(whitePieces->count(j*10 + i) != 0) std::cout << 
                                                  (*whitePieces)[j*10 + i];
            // Checks if black has a piece at that location and prints it if
            // true
            else if(blackPieces->count(j*10 + i) != 0) std::cout << 
                                                       (*blackPieces)[j*10 + i];
            // Square is empty
            else if((i + j) % 2 == 0) std::cout << "_";
            else std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n  abcdefgh\n";
}

void GraphicsDisplay::display() {
    const int squareSize = 50; // Adjust square size as needed

    for(int i = 7; i >= 0; --i) { // Start from the bottom of the board
        for(int j = 0; j < 8; ++j) { // Loop through each column
            int x = j * squareSize;
            int y = i * squareSize; // No need to invert since we are starting from bottom
            int color = (i + j) % 2 == 0 ? Xwindow::White : Xwindow::Black;
            int textColor = (i + j) % 2 == 0 ? Xwindow::Black : Xwindow::White;

            // Draw square
            window.fillRectangle(x, y, squareSize, squareSize, color);

            // Draw piece if present
            int location = j * 10 + (7 - i); // Convert board coordinates to array index
            if(whitePieces->count(location) != 0) {
                window.drawString(x + squareSize/4, y + squareSize/4, string(1, (*whitePieces)[location]), textColor);
            } else if(blackPieces->count(location) != 0) {
                window.drawString(x + squareSize/4, y + squareSize/4, string(1, (*blackPieces)[location]), textColor);
            }
        }
    }
}