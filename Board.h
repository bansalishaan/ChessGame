#include <iostream>
#include <map>
// #include <string>
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
            virtual ~Display();
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
    vector<int> enPassantPawns;
    unique_ptr<Display> d;

    public:
        Board(int boardType);
        bool init(bool normal = true, char piece='\0', int location = 0);
        bool makeMove(vector<vector<int>> move, int col);
        bool noMoves();
        bool inCheck();
        void display();
};