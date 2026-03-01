#include <opencv2/opencv.hpp>

using namespace cv;

Mat applyEffects(const Mat& src,int v){
    Mat dst=src.clone();
    if(v<10)cvtColor(dst,dst,COLOR_BGR2GRAY);
    if(v>=10&&v<20)GaussianBlur(dst,dst,Size(9,9),v*0.1);
    if(v>=20&&v<30)blur(dst,dst,Size(15,15));
    if(v>=30&&v<40)bitwise_not(dst,dst);
    if(v>=40&&v<50){
        Mat mapx(dst.size(),CV_32F),mapy(dst.size(),CV_32F);
        for(int y=0;y<dst.rows;y++)for(int x=0;x<dst.cols;x++){
            float dx=x-dst.cols/2;
            float dy=y-dst.rows/2;
            float r=sqrt(dx*dx+dy*dy);
            mapx.at<float>(y,x)=x+dx*0.001f*v*r;
            mapy.at<float>(y,x)=y+dy*0.001f*v*r;
        }
        remap(dst,dst,mapx,mapy,INTER_LINEAR);
    }
    if(v>=50&&v<60){
        Mat noise(dst.size(),dst.type());
        randn(noise,0,v);
        add(dst,noise,dst);
    }
    if(v>=60&&v<70){
        Mat hsv;cvtColor(dst,hsv,COLOR_BGR2HSV);
        vector<Mat> ch;split(hsv,ch);
        ch[1]+=v;
        merge(ch,hsv);
        cvtColor(hsv,dst,COLOR_HSV2BGR);
    }
    if(v>=70&&v<80){
        flip(dst,dst,1);
    }
    if(v>=80){
        Mat small;
        resize(dst,small,Size(),0.5,0.5);
        resize(small,dst,src.size());
    }
    return dst;
}
