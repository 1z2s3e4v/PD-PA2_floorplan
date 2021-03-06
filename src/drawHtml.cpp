#include "drawHtml.h"

//string pos2str(Pos pos) {return "("+to_string(get<0>(pos))+","+to_string(get<1>(pos))+")";}
Drawer_C::Drawer_C(){}
Drawer_C::Drawer_C(string s){
    fileName = s;
    string cmd = "rm -f ";
    cmd += fileName;
    system(cmd.c_str());
}

// svg
void Drawer_C::start_svg(){
    fout.open(fileName.c_str(), ofstream::out);
    fout << "<svg height=\"" <<outline_y  << "\" width=\"" << outline_x  << "\">\n";
}
void Drawer_C::end_svg(){
    fout << "</svg>\n";
    fout.close();
}
void Drawer_C::setting(int p_outline_x,int p_outline_y,int p_scaling,int p_offset_x,int p_offset_y){
    outline_x = p_outline_x;
    outline_y = p_outline_y;
    scaling = p_scaling;
    offset_x = p_offset_x;
    offset_y = p_offset_y;
}
void Drawer_C::drawRect(string name, bBox box, string color){
    float w_ori = get<0>(get<1>(box)) - get<0>(get<0>(box));
    float h_ori = get<1>(get<1>(box)) - get<1>(get<0>(box));
    
    int w = w_ori*scaling;
    int h = h_ori*scaling;
    int x = get<0>(get<0>(box)) * scaling + offset_x;
    int y = outline_y - (get<1>(get<0>(box)) * scaling + offset_y) - h;
    fout << "   <rect name=\""<< name << "\" x=\"" << x << "\" y=\"" << y 
        << "\" width=\"" << w << "\" height=\"" << h << "\""
        << " style=\"fill:"<< color << ";stroke:red;stroke-width:1;fill-opacity:0.1;stroke-opacity:0.9\" />\n";
}
void Drawer_C::drawLine(string name, Pos p1, Pos p2, string color, float width, float opacity){
    int x1 = get<0>(p1) * scaling + offset_x;
    int y1 = outline_y - (get<1>(p1) * scaling + offset_y);
    int x2 = get<0>(p2) * scaling + offset_x;
    int y2 = outline_y - (get<1>(p2) * scaling + offset_y);
    int w = width * scaling;
    fout << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" style=\"stroke:" << color << ";stroke-width:" << w << ";stroke-opacity:" << opacity << "\" />\n";
}
void Drawer_C::drawLine(string name, Pos p1, Pos p2, int* rgb, float width, float opacity){
    int x1 = get<0>(p1) * scaling + offset_x;
    int y1 = outline_y - (get<1>(p1) * scaling + offset_y);
    int x2 = get<0>(p2) * scaling + offset_x;
    int y2 = outline_y - (get<1>(p2) * scaling + offset_y);
    int w = width * scaling;
    fout << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" style=\"stroke:rgb(" << rgb[0] <<","<< rgb[1] <<"," << rgb[2] << ");stroke-width:" << w << ";stroke-opacity:" << opacity << "\" />\n";
}
void Drawer_C::drawLine(string name, Pos p1, Pos p2, string color, float width){
    int x1 = get<0>(p1) * scaling + offset_x;
    int y1 = outline_y - (get<1>(p1) * scaling + offset_y);
    int x2 = get<0>(p2) * scaling + offset_x;
    int y2 = outline_y - (get<1>(p2) * scaling + offset_y);
    int w = width * scaling;
    fout << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" style=\"stroke:" << color << ";stroke-width:" << w << "\" />\n";
}
void Drawer_C::drawLine(string name, Pos p1, Pos p2, int* rgb, float width){
    int x1 = get<0>(p1) * scaling + offset_x;
    int y1 = outline_y - (get<1>(p1) * scaling + offset_y);
    int x2 = get<0>(p2) * scaling + offset_x;
    int y2 = outline_y - (get<1>(p2) * scaling + offset_y);
    int w = width * scaling;
    fout << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" style=\"stroke:rgb(" << rgb[0] <<","<< rgb[1] <<"," << rgb[2] << ");stroke-width:" << w << "\" />\n";
}
void Drawer_C::drawText(string name, Pos pos, string str){
    int x = get<0>(pos) * scaling + offset_x;
    int y = outline_y - (get<1>(pos) * scaling + offset_y);
    fout << "   <text name=\""<< name << "\" x=\"" << x << "\" y=\"" << y << "\" fill=\"black\">" << str << "</text>\n";
}  