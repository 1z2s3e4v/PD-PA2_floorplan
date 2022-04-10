#include <fstream>
#include <sstream>
#include "fp.h"
#include "drawHtml.h"

void drawSvg(string);
void rotate(Block*);
void updateXY();

void readBlock(char* fileName){
    ifstream file(fileName);
    string line;
    int count_block = 0;
    while(getline(file,line)){
        stringstream ss(line);
        string title;
        ss >> title;
        if(title == "Outline:"){
            ss >> outLine_x >> outLine_y;
            outline_aspect_ratio = (double)outLine_x/outLine_y;
        }
        else if(title == "NumBlocks:")
            ss >> block_num;
        else if(title == "NumTerminals:")
            ss >> terminal_num;
        else if(title != ""){ // Block and terminal
            if(count_block < block_num){ // Block
                int w,h;
                ss >> w >> h;
                Block *block = new Block(title,w,h);
                all_Blocks.emplace(title,block);
                vec_all_Blocks.push_back(block);
                count_block++;
            }
            else{ // terminal
                string tmp;
                int x,y;
                ss >> tmp >> x >> y;
                Terminal *terminal = new Terminal(title,x,y);
                all_Terminals.emplace(title,terminal);
                vec_all_Terminals.push_back(terminal);
                count_block++;
            }
        }
        ss.clear(); ss.str("");
    }
    file.close();
    /*cout << "blocks:\n";
    for(auto it = all_Blocks.begin();it != all_Blocks.end();it++){
        cout << it->second->name << " " << it->second->w << " " << it->second->h << endl;
    }
    cout << "\nterminals:\n";
    for(auto it = all_Terminals.begin();it != all_Terminals.end();it++){
        cout << it->second->name << " " << it->second->x << " " << it->second->y << endl;
    }*/
}

void readNet(char* fileName){
    ifstream file(fileName);
    string line;
    int count_net = 0;
    while(getline(file,line)){
        Net* net;
        stringstream ss(line);
        string obj;
        string netName;
        ss >> obj;
        if(obj == "NumNets:")
            ss >> net_num;
        else if(obj == "NetDegree:"){
            int degree;
            ss >> degree;
            netName = "n" + to_string(count_net);
            net = new Net(netName,degree);
            all_Nets.emplace(netName,net);
            vec_all_Nets.push_back(net);
            count_net++;
        }
        else if(obj != ""){
            auto it = all_Blocks.find(obj);
            if(it != all_Blocks.end()){ // Block
                net->addBlock(it->second);
            }
            else{ // Terminal
                net->addTerminal(all_Terminals[obj]);
            }
        }
    }
    file.close();
}

void drawSvg(string fileName){
    string output_fileName = fileName + ".svg.html";
    string metal_color[7] = {"black", "blue", "yellow", "green", "purple", "gray", "orange"};
    Drawer_C* draw_svg = new Drawer_C(output_fileName);
    draw_svg->setting(outLine_x,outLine_y,1,0,0); // outline_x outline_y scaling offset_x offset_y
    draw_svg->start_svg();
    draw_svg->drawRect("Outline", bBox(Pos(0,0),Pos(outLine_x,outLine_y)), "gray");
    for(unsigned int i=0;i<vec_all_Blocks.size();i++){
        Block *b = vec_all_Blocks[i];
        draw_svg->drawRect(b->name, bBox(Pos(b->x,b->y),Pos(b->x+b->w,b->y+b->h)), "yellow");
        draw_svg->drawText(b->name+"label", Pos(b->mid_x,b->mid_y), b->name);
    }
    draw_svg->end_svg();
}

void writeRpt(char* fileName,clock_t start){
    double runtime = (double)(clock()-start)/(double)CLOCKS_PER_SEC;
    ofstream fout(fileName);
    // Cost, W, A, w&h, runtime, (each Block's lower-left corner and upper-right corner)
    double cost = alpha*A + (1-alpha)*W;
    fout << cost << "\n";
    fout << W << "\n";
    fout << A << "\n";
    fout << MAX_X << " " << MAX_Y << "\n";
    fout << runtime << "\n";

    for(unsigned int i=0;i<vec_all_Blocks.size();i++){
        Block *b = vec_all_Blocks[i];
        fout << b->name << " " << b->x << " " << b->y << " " << b->x+b->w << " " << b->y+b->h << "\n";
    }
    cout << "-------------write output----------------" << "\n";
    cout << "Cost: " << cost << "\n";
    cout << "Area: " << MAX_X << " * " << MAX_Y << "\n";
    cout << "Runtime: " << runtime << "\n";
    cout << "-----------------------------------------" << "\n";
}

void print_info(){
    cout << "nets-------------------------\n";
    for(auto it = all_Nets.begin();it!=all_Nets.end();it++){
        cout << it->second->name << " " << it->second->m_vec_Block.size() << " " << it->second->m_vec_Terminal.size() << endl;
    }
    cout << "blocks-----------------------\n";
    for(auto it = all_Blocks.begin();it!=all_Blocks.end();it++){
        cout << it->second->name << " " << it->second->m_map_net.size()<< endl;
    }
    cout << "terminals--------------------\n";
    for(auto it =   all_Terminals.begin();it!=all_Terminals.end();it++){
        cout << it->second->name << " " << it->second->m_map_net.size() << endl;
    }
    cout << "\n";
}

bool inside_Outline(){
    if(MAX_X < outLine_x && MAX_Y < outLine_y){
        return true;
    }
    else if(MAX_X < outLine_y && MAX_Y < outLine_x){
        reverse(pos.begin(),pos.end());
        for(unsigned int i=0;i<pos.size();i++){
            rotate(pos[i]);
        }
        updateXY();
    }
    return (MAX_X < outLine_x && MAX_Y < outLine_y);
}

int outLine_distence(){
    int distence_x = MAX_X - outLine_x;
    int distence_y = MAX_Y - outLine_y;
    if(distence_x < 0) distence_x = 0;
    if(distence_y < 0) distence_y = 0;
    return distence_x + distence_y ;
}

void compute_allNetCost(){
    MAX_X = -1;
    MAX_Y = -1;
    W = 0;
    for(auto it = all_Nets.begin();it != all_Nets.end();it++){
        W += it->second->updateCost();
    }
    A = MAX_X*MAX_Y;
}

void compute_overArea(){
    overA = 0;
    for(unsigned int i=0;i<pos.size();i++){
        overA += pos[i]->compute_OverA();
    }
}

void init_Loci(){
    for(int i=0;i<10000;++i){
        random_shuffle(vec_all_Blocks.begin(),vec_all_Blocks.end());
        pos = vec_all_Blocks;
        random_shuffle(vec_all_Blocks.begin(),vec_all_Blocks.end());
        neg = vec_all_Blocks;
        updateXY();

        double aspect_ratio = (double)MAX_X/MAX_Y;
        if(abs(aspect_ratio - outline_aspect_ratio) < 0.2){
            cout << "\nInit Aspect Ratio = " << aspect_ratio << " in " << i << "-th round.\n";
            break;
        }
    }
}

void updateXY(){
    // initial
    W=0;
    OutOfRange = 0;
    for(unsigned int k=0;k<pos.size();k++){
        pos[k]->setX(0);
        pos[k]->setY(0);
    }

    int i=0,i_n=0,j=0,j_n=0;
    for(i=0;i<pos.size();i++){ // for each pos[i]
        while(pos[i]->name != neg[i_n]->name) 
            i_n++; // get index of "pos[i]" in neg
        for(j=i+1;j<pos.size();j++){ // for each obj behind pos[i]
            for(j_n=i_n+1;j_n<neg.size();j_n++){ // for each obj behind neg[i_n]
                if(pos[j]->name == neg[j_n]->name){ // find
                    if(pos[j]->x < (pos[i]->x + pos[i]->w)){
                        pos[j]->setX(pos[i]->x + pos[i]->w);
                    }
                    break;
                }
            }
        }
        i_n = 0;
    }

    i=0,i_n=0,j=0,j_n=0;
    for(i=pos.size()-1;i>=0;i--){ // for each pos'[i]
        while(pos[i]->name != neg[i_n]->name) 
            i_n++; // get index of "pos[i]" in neg
        for(j=i-1;j>=0;j--){ // for each obj behind pos[i]
            for(j_n=i_n+1;j_n<neg.size();j_n++){ // for each obj behind neg[i_n]
                if(pos[j]->name == neg[j_n]->name){ // find
                    if(pos[j]->y < (pos[i]->y + pos[i]->h)){
                        pos[j]->setY(pos[i]->y + pos[i]->h);
                    }
                    break;
                }
            }
        }
        i_n = 0;
    }

    compute_allNetCost();
    compute_overArea();
}

void print_Loci(){
    cout << "pos:\n";
    for(unsigned int i=0;i<pos.size();i++){
        cout << pos[i]->name << " ";
    }
    cout << "\nneg:\n";
    for(unsigned int i=0;i<neg.size();i++){
        cout << neg[i]->name << " ";
    }
    cout << "\n";
}

void swap_one(int choice, int index1, int index2){
    if(choice == 0){
        swap(pos[index1],pos[index2]);
    }
    else{
        swap(neg[index1],neg[index2]);
    }
}

void swap_two(int index1, int index2){
    swap(pos[index1],pos[index2]);
    swap(neg[index1],neg[index2]);
}

void rotate(Block* b){
    swap(b->w,b->h);
}

void op(int choice,int b, int r1, int r2){
    switch(choice){
        case 0: // Rotate a module
            rotate(vec_all_Blocks[r1]);
            break;
        case 1: // Swap two module names in only one sequence
            swap_one(b,r1,r2);
            break;
        case 2: // Swap two module names in both sequences.
            swap_two(r1,r2);
            break;
    }
}

bool rand_access(double p){
    int i = 100000000*p;
    if(p == 0) i = 2;
    return rand()%100000000 < i ;
}

void FloorPlan(char* outFileName, clock_t start){
    clock_t fp_start = clock();
    int epoch = 0;
    int state = -1;
    double T = (double)INT_MAX; // init temperature
    int count_not_update = 0;
    while((double)(clock()-start)/(double)CLOCKS_PER_SEC < 290){ // origin: !inside_Outline()
        if(epoch % 10000 == 0 && state != epoch/10000){
            state = epoch/10000;
            cout << "epoch " << epoch << "\n";
            cout << "area: "<< MAX_X << " * " << MAX_Y << " = " << A << "\n";
            cout << "over area: "<< overA << "\n";
            cout << "\n";
            if(count_not_update > all_Blocks.size()*1000 && minCost != (double)INT_MAX){
                cout << "Break for local optimal.\n";
                break;
            }
            if(count_not_update > all_Blocks.size()*1000 && minCost == (double)INT_MAX){
                cout << "Break for dead solution.\n";
                break;
            }
        }
        
        int ori_A=A,ori_W=W,ori_OutOfRange=OutOfRange;
        int ori_disXY = outLine_distence();
        int ori_overA = overA;
        double ori_cost = alpha*A+(1-alpha)*W;

        int choice = rand()%3;
        int b = rand()%2;
        int r1 = rand()%vec_all_Blocks.size();
        int r2 = rand()%vec_all_Blocks.size();
        op(choice,b,r1,r2);
        updateXY();
        if(T != 0.0)
            T *= 0.99;
        double delta_C = (overA - ori_overA);//abs(outLine_distence() - ori_disXY);
        //if()
        //cout << "T=" << T << " delta_C=" << delta_C<< " delta_C/T=" << delta_C/T << "\n";
        //cout << "SA p=" << exp(-delta_C/T) << "\n";

        double new_cost = alpha*A+(1-alpha)*W;
        
        if(inside_Outline() && new_cost < minCost){ // find smaller cost, write
            minCost = new_cost;
			writeRpt(outFileName,start);
            drawSvg(outFileName);
            count_not_update = 0;
		}
        else{
            count_not_update++;
        }
        //overA > ori_overAoutLine_distence() > ori_disXY
        if(overA > ori_overA && !rand_access(exp(-delta_C/T))){ 
            // recovery
            op(choice,b,r1,r2);
            updateXY();
            //cout << "recovery\n";
        }
        else if(inside_Outline() && (new_cost < ori_cost || W < ori_W)){
            //epoch++;
        }
        else if(T < 0.0000000000001 && overA > ori_overA){
            // recovery
            op(choice,b,r1,r2);
            updateXY();
            //cout << "recovery\n";
        }
        else{ // smaller area or SA
            //T *= 1.15;
            //epoch++;
        }
        epoch++;
    }
    cout << "final epoch: " << epoch << "\n";
}

int main(int argc,char** argv){
    clock_t start=clock();
    auto seed = (unsigned)time(NULL);
    srand(seed);
    string argv1 = argv[1];
    alpha = stod(argv1);
	readBlock(argv[2]);
    readNet(argv[3]);
    //print_info();

    cout << "Outline Aspect Ratio = " << outline_aspect_ratio << "\n";
    while(count_cost_notUpdate < 3 || minCost == (double)INT_MAX){
		if(minCost != (double)INT_MAX) {cout << "--------------------- FP Reset -----------------------\n";}
        if(minCost >= fp_minCost) {
            count_cost_notUpdate++;
        }
		else {
            count_cost_notUpdate = 0;
            fp_minCost = minCost;
        }
        
        init_Loci();
        //updateXY();
        FloorPlan(argv[4],start);
        

        if((clock()-start)/(double)CLOCKS_PER_SEC > 28 && minCost != (double)INT_MAX){
            cout << "Time out break.\n";
            break;
        }
    }
    ofstream fout("random_seed.txt");

    cout << "------------------------ Finished -------------------------------\n";
    cout << "output rpt: " << argv[4] << "\n";
    cout << "visualization: " << argv[4] << ".svg.html\n";
}
