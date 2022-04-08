#ifndef SP_H
#define SP_H

#include<string>
#include<map>
#include<vector>
#include<list>
#include<algorithm>
#include<random>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

class Net;
class Terminal;
class Block;

int outLine_x = 0;
int outLine_y = 0;
int block_num = 0;
int terminal_num = 0;
int net_num = 0;
double alpha = 0.0;

int MAX_X=-1;
int MAX_Y=-1;
int A;
int W;
int overA;
int OutOfRange;
double minCost = 9999999;
double fp_minCost = 9999999;
int count_cost_notUpdate = 0;


map<string,Net*> all_Nets;
map<string,Block*> all_Blocks;
map<string,Terminal*> all_Terminals;
vector<Net*> vec_all_Nets;
vector<Block*> vec_all_Blocks;
vector<Terminal*> vec_all_Terminals;

vector<Block*> pos;
vector<Block*> neg;

//-----------------------------------------------------------------------------
class Net{
public:
    Net(string,int);
    void addTerminal(Terminal*);
    void addBlock(Block*);
    double updateCost();

    string name;
    int degree = 0;
    double HPWL = 0.0;
    vector<Terminal*> m_vec_Terminal;
    vector<Block*> m_vec_Block;
    map<string,Terminal*> m_map_Terminal;
    map<string,Block*> m_map_Block;
};
//-----------------------------------------------------------------------------
class Terminal{
public:
    Terminal(string name,int x,int y);
    void addNet(Net*);

    string name;
    int x = 0;
    int y = 0;
    map<string,Net*> m_map_net;
};
//-----------------------------------------------------------------------------
class Block{
public:
    Block(string name,int w,int h);
    void addNet(Net*);
    void setX(int);
    void setY(int);
    int compute_OverA();

    string name;
    int w = 0;
    int h = 0;
    int x = 0;
    int y = 0;
    int mid_x = 0;
    int mid_y = 0;
    int overArea = 0;
    map<string,Net*> m_map_net;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Net::Net(string name,int degree){
    this->name = name;
    this->degree = degree;
}
void Net::addTerminal(Terminal* t){
    m_map_Terminal.emplace(t->name,t);
    m_vec_Terminal.push_back(t);
    t->addNet(this);
}
void Net::addBlock(Block* b){
    m_map_Block.emplace(b->name,b);
    m_vec_Block.push_back(b);
    b->addNet(this);
}
double Net::updateCost(){
    int min_x = 9999999, min_y = 9999999, max_x = -1, max_y = -1;
    for(unsigned int i = 0;i<m_vec_Terminal.size();i++){
        if(m_vec_Terminal[i]->x > max_x)
            max_x = m_vec_Terminal[i]->x;
        if(m_vec_Terminal[i]->y > max_y)
            max_y = m_vec_Terminal[i]->y;
        if(m_vec_Terminal[i]->x < min_x)
            min_x = m_vec_Terminal[i]->x;
        if(m_vec_Terminal[i]->x < min_y)
            min_y = m_vec_Terminal[i]->y;
    }
    for(unsigned int i = 0;i<m_vec_Block.size();i++){
        if(m_vec_Block[i]->mid_x > max_x)
            max_x = m_vec_Block[i]->mid_x;
        if(m_vec_Block[i]->mid_y > max_y)
            max_y = m_vec_Block[i]->mid_y;
        if(m_vec_Block[i]->mid_x < min_x)
            min_x = m_vec_Block[i]->mid_x;
        if(m_vec_Block[i]->mid_y < min_y)
            min_y = m_vec_Block[i]->mid_y;
        // update MAX_X and MAX_Y
        if((m_vec_Block[i]->x + m_vec_Block[i]->w) > MAX_X)
            MAX_X = (m_vec_Block[i]->x + m_vec_Block[i]->w);
        if((m_vec_Block[i]->y + m_vec_Block[i]->h) > MAX_Y)
            MAX_Y = (m_vec_Block[i]->y + m_vec_Block[i]->h);
        // check if out_of_range
        bool isOutOfRange = false;
        if((m_vec_Block[i]->x + m_vec_Block[i]->w) > outLine_x){
            OutOfRange++;
            isOutOfRange = true;
        }
        if((m_vec_Block[i]->y + m_vec_Block[i]->h) > outLine_y && !isOutOfRange)
            OutOfRange++;    
    }
    
    //cout << "("<< max_x << "-" << min_x << ")+(" << max_y << "-" << min_y << ")" << " = " << HPWL<<"\n";

    HPWL = ((max_x-min_x) + (max_y-min_y));
    A = MAX_X*MAX_Y;
    W += HPWL;
    return HPWL;
}
//-----------------------------------------------------------------------------
Terminal::Terminal(string name,int x,int y){
    this->name = name;
    this->x = x;
    this->y = y;
}
void Terminal::addNet(Net* net){
    m_map_net.emplace(net->name,net);
}
//-----------------------------------------------------------------------------
Block::Block(string name,int w,int h){
    this->name = name;
    this->w = w;
    this->h = h;
    setX(0);
    setY(0);
}
void Block::addNet(Net* net){
    m_map_net.emplace(net->name,net);
}
void Block::setX(int _x){
    x = _x;
    mid_x = x+(w/2);
}
void Block::setY(int _y){
    y = _y;
    mid_y = y+(h/2);
}
int Block::compute_OverA(){
    int over_x = w, over_y = h;
    if(x < outLine_x && (x+w)>outLine_x){
        over_x = x+w-outLine_x;
    }
    if(y < MAX_Y && (y+h)>outLine_y){
        over_y = y+h-outLine_y;
    }
    if((x+w)<=outLine_x && (y+h)<=outLine_y){
        over_x = 0;
        over_y = 0;
    }
    overArea = over_x*over_y;
    if((x+w)>outLine_x && (y+h)>outLine_y && x < outLine_x && y < MAX_Y){
        overArea = (x+w-outLine_x)*h + (x+w-outLine_x)*(outLine_x-x) ; 
    }
    
    //cout<< over_x << " * " << over_y << " = " << overArea << "\n";
    return overArea;
}
#endif
