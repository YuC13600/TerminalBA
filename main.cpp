#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <windows.h>
#include <cstdlib>
using namespace cv;
using namespace std;

int main(void){
    cout<<"resize the terminal window and ..."<<endl;
    system("pause");

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int scr_width, scr_height;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    scr_width = csbi.srWindow.Right - csbi.srWindow.Left +1;
    scr_height = csbi.srWindow.Bottom - csbi.srWindow.Top +1;
    cout<<scr_width<<" * "<<scr_height<<endl;
    system("pause");
    int vwidth=scr_width/2, vheight=scr_height;
    int dp_width, dp_height, real_height;
    int buffer_size;
    if(vwidth/480.f > vheight/360.f){
        dp_height = real_height = scr_height;
        dp_width = scr_height*480/360*2;
        buffer_size = (dp_width+1) * dp_height -1;
    }
    else if(vwidth/480.f < vheight/360.f){
        dp_width = scr_width;
        real_height = dp_width*360/480/2;
        dp_height = scr_height;
        buffer_size = (dp_width+1)*real_height + (scr_height - real_height) -1;
    }
    else{
        dp_height = real_height = scr_height;
        dp_width = scr_width;
        buffer_size = (dp_width+1)*dp_height -1;
    }
    cout<<dp_height<<" "<<dp_width<<" "<<real_height<<endl;
    system("pause");
    int x_step=480/dp_width, y_step=360/real_height;

    char *buffer=(char*)malloc(sizeof(char)*buffer_size);
    VideoCapture cap("C:/Users/georg/Desktop/code/repos/TerminalBA/orivid.webm");
    if(!cap.isOpened()){
        cout<<"Error: source video opening failed.\n";
        return -1;
    }

    Mat frame, greyframe;
    int value;
    unsigned char* p;

    float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

    for(;;){
        cap>>frame;
        if(frame.empty())
            break;
        
        cvtColor(frame, greyframe, COLOR_BGR2GRAY);
        int render_counter=0;
        for(int y=0;y<dp_height;++y){
            if(y>real_height){
                buffer[render_counter++]=10;
            }
            else{
                for(int x=0;x<dp_width;++x){
                    p=greyframe.ptr(y*y_step, x*x_step);
                    value = (int)*p;
                    if(value<40) buffer[render_counter++]=46;
                    else if(value<80) buffer[render_counter++]=45;
                    else if(value<120) buffer[render_counter++]=61;
                    else if(value<160) buffer[render_counter++]=43;
                    else if(value<200) buffer[render_counter++]=35;
                    else if(value<250) buffer[render_counter++]=37;
                    else buffer[render_counter++]=64;
                }
                buffer[render_counter++]=10;
            }
        }
        putchar('\n');
        putchar('\n');
        for(int i=0;i<buffer_size;++i)
            putchar(buffer[i]);
        LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;
        Sleep(20);  //20
    }

    cap.release();
    return 0;
}