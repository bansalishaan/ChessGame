#ifndef BOARD
#define BOARD
#include <iostream>
#include <map>
#include <vector>
#include <memory>
using namespace std;

class Board {
    class Display {
        protected:
            map<int, char> *whitePieces;
            map<int, char> *blackPieces;
            Display(map<int, char> *whitePieces, map<int, char> *blackPieces);
        public:
            virtual ~Display() = default;
            virtual void display() = 0;
    };
    
    class TextDisplay: public Display {
        public:
            TextDisplay(map<int, char> *whitePieces, map<int, char> *blackPieces);
            void display() override;
    };

    class GraphicsDisplay: public Display {
        public:
            GraphicsDisplay(map<int, char> *whitePieces, map<int, char> *blackPieces);
            void display() override;
    };


    map<int, char> whitePieces;
    map<int, char> blackPieces;
    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteLRookMoved = false;
    bool whiteRRookMoved = false;
    bool blackLRookMoved = false;
    bool blackRRookMoved = false;
    vector<int> enPassantPawns;
    unique_ptr<Display> d;

    bool movePutsKingInCheck(int pieceLoc, int moveLoc, int col,
        map<int, char> *ownPieces, map<int, char> *oppPieces);

    public:
        Board(int boardType);
        bool init(bool normal = true, char piece='\0', int location = 0, 
            bool remove = false);
        bool validInit();
        bool makeMove(vector<int> move, int col);
        bool noMoves(int col);
        bool inCheck(int col);
        void display();
};

#endif
