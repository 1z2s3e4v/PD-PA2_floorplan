#ifndef DRAWHTML_H
#define DRAWHTML_H

#include <string>
#include <tuple>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

typedef tuple<float, float> Pos; // x, y
typedef tuple<Pos, Pos> bBox; // ll, ur

class Drawer_C{
public:
    Drawer_C();
    Drawer_C(string);
    
    
    // svg
    void start_svg();
    void end_svg();
    void setting(int,int,int,int,int); // outline_x outline_y scaling offset_x offset_y
    void drawRect(string name, bBox, string color);
    void drawLine(string name, Pos, Pos, string color,float width);
    void drawLine(string name, Pos, Pos, int* color,float width);
    void drawLine(string name, Pos, Pos, string color,float width,float opacity);
    void drawLine(string name, Pos, Pos, int* color,float width,float opacity);
    void drawText(string name, Pos, string);

    // virtuoso
    void start();
    void end();
    void set_output_cellview(string,string,string);
    void drawInst(Pos, string name);
    void drawPath(Pos, Pos, int layer,float width, string netName);
    void drawVia(Pos, int layer1, int layer2, string netName);
    void drawPin(Pos, int layer, string name);
private:
    string fileName;
    ofstream fout;
    // svg
    int outline_x = 1600;
    int outline_y = 1200;
    int scaling = 200;
    int offset_x = 200;
    int offset_y = 600;
};

#endif