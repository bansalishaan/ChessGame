#ifndef DISPLAY
#define DISPLAY

#include "window.h"
#include <map>
using namespace std;

class BoardDisplay {
    protected:
        map<int, char> *whitePieces;
        map<int, char> *blackPieces;
        BoardDisplay(map<int, char> *whitePieces, map<int, char> *blackPieces);
    public:
        virtual ~BoardDisplay() = default;
        virtual void display() = 0;
};

class TextDisplay: public BoardDisplay {
    public:
        TextDisplay(map<int, char> *whitePieces, map<int, char> *blackPieces);
        void display() override;
};

class GraphicsDisplay: public BoardDisplay {
    Xwindow window;
    public:
        GraphicsDisplay(map<int, char> *whitePieces,
                        map<int, char> *blackPieces);
        void display() override;
};

#endif
