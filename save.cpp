#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

void saveVideo(const vector<Mat>& frames,const string& name,int fps,int w,int h){
    int fourcc;
    if(name.find(".avi")!=string::npos)fourcc=VideoWriter::fourcc('M','J','P','G');
    else if(name.find(".webm")!=string::npos)fourcc=VideoWriter::fourcc('V','P','8','0');
    else fourcc=VideoWriter::fourcc('a','v','c','1');
    VideoWriter writer(name,fourcc,fps,Size(w,h));
    for(auto& f:frames)writer.write(f);
    writer.release();
}
