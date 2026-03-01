#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <deque>

using namespace cv;
using namespace std;

extern Mat applyEffects(const Mat&, int);
extern void saveVideo(const vector<Mat>&, const string&, int, int, int);

deque<Mat> undoStack;
deque<Mat> redoStack;
vector<Mat> timeline;
int effectValue=0;
int frameIndex=0;

void onSlider(int v, void*){effectValue=v;}

int main(int argc,char** argv){
    string path=argc>1?argv[1]:"";
    VideoCapture cap;
    Mat frame;
    if(path.find(".png")!=string::npos||path.find(".jpg")!=string::npos){
        frame=imread(path);
        timeline.push_back(frame.clone());
    }else{
        cap.open(path);
        while(cap.read(frame))timeline.push_back(frame.clone());
    }
    namedWindow("Editor",WINDOW_NORMAL);
    createTrackbar("Effect","Editor",&effectValue,100,onSlider);
    while(true){
        Mat current=timeline[frameIndex].clone();
        Mat processed=applyEffects(current,effectValue);
        imshow("Editor",processed);
        int k=waitKey(30);
        if(k==27)break;
        if(k=='z'&&!undoStack.empty()){redoStack.push_back(timeline[frameIndex]);timeline[frameIndex]=undoStack.back();undoStack.pop_back();}
        if(k=='y'&&!redoStack.empty()){undoStack.push_back(timeline[frameIndex]);timeline[frameIndex]=redoStack.back();redoStack.pop_back();}
        if(k=='c')undoStack.push_back(timeline[frameIndex]);
        if(k=='v'&&!undoStack.empty())timeline[frameIndex]=undoStack.back();
        if(k=='x'){timeline.erase(timeline.begin()+frameIndex);frameIndex=0;}
        if(k=='n')frameIndex=(frameIndex+1)%timeline.size();
        if(k=='p')frameIndex=(frameIndex-1+timeline.size())%timeline.size();
        if(k=='s')saveVideo(timeline,"output.mp4",30,processed.cols,processed.rows);
    }
    return 0;
}
