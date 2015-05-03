#define XK_LATIN1
#include<X11/Xlib.h>
#include<unistd.h>
#include<stdlib.h>
#include<X11/keysymdef.h>
#include<iostream>
#include<time.h>
#include<string>
#include<sstream>


main(int argc, char* argv[])
{
	srand (time(NULL)); // random numbers generator initialization
	int window_width = 500;
	int window_height = 500;
	Display* connection = XOpenDisplay(":0");
	int BlackColor = BlackPixel(connection, DefaultScreen(connection));
	int WhiteColor = WhitePixel(connection, DefaultScreen(connection));
	Window w = XCreateSimpleWindow(connection, DefaultRootWindow(connection), 0, 0, window_width, window_height, 0, BlackColor, BlackColor);
	GC gc = XCreateGC(connection, w, 0, NULL);
	GC gcXor = XCreateGC(connection, w, 0, NULL);
	GC gcXorGreen = XCreateGC(connection, w, 0, NULL);
	XSetFunction(connection, gcXor, GXxor);
	XSetForeground(connection, gc, WhiteColor);
	XSetForeground(connection, gcXor, WhiteColor);
	XMapWindow(connection, w);
	//XMapWindow(connection, w);
	XFlush(connection);
	int done = 0;
	XWindowAttributes w_attr;
	
	int target = rand() % 4 +1;
	int selected = 0;
	int score = 0;
	int round = 0;
	std::string score_string;
	std::string round_string;
	XSelectInput(connection, w, ExposureMask | ButtonPressMask | KeyPressMask);
	int expose_counter= 0;
	while( !done)
	{
	XEvent e;
	XNextEvent(connection, &e);
		switch(e.type)
		{
			case ButtonPress:
			switch(e.xbutton.button)
			{
				case Button1:
				{
					int x = e.xbutton.x;
					int y = e.xbutton.y;
					// now we need to check which area
					if(x>window_width/10 && x<window_width*9/10 && y>window_height/10 && y<window_width*9/10)
					{
						if(x<=window_width/2)
						{
							if(y<=window_height/2)
							{
								selected = 1;
							}
							else
							{
								selected = 3;
							}
						}
						else
						{
							if(y<=window_height/2)
							{
								selected = 2;
							}
							else
							{
								selected = 4;
							}
						}
					} 
				}
				break;		
				case Button3:
				//std::cout<<"Time to finish application"<<std::flush;
				done = 1;
				break;
			}
			break;
			case Expose:
			{
				if(!expose_counter)
				{
					XDrawRectangle(connection, w, gc, window_width/20, window_height/20, window_width*18/20, window_height*18/20);
					++expose_counter;
				}
				std::ostringstream score_stream;
				std::ostringstream round_stream;
				score_stream<<"Score:"<<score;
				round_stream<<"Round:"<<round;
				round_string = round_stream.str();
				score_string = score_stream.str();
				XDrawImageString(connection, w, gcXor, window_width/10, window_height/20, score_string.c_str(), score_string.length());
				XDrawImageString(connection, w, gcXor, window_width/2, window_height/20, round_string.c_str(), round_string.length());
				if(!XGetWindowAttributes(connection, w, &w_attr)){}
				window_width = w_attr.width;
				window_height = w_attr.height;
				XDrawLine(connection, w, gc, window_width/2, window_height/10, window_width/2, window_height*9/10);
				XDrawLine(connection, w, gc, window_width/10, window_height/2, window_width*9/10, window_height/2); 
				XDrawRectangle(connection, w, gc, window_width/10, window_height/10, window_width*4/5, window_height*4/5);
				XFlush( connection);
			
			}
			break;
			case KeyPress:
			{
				int key = XLookupKeysym(&e.xkey, 0); 
				if(key == XK_q)
				{
				//std::cout<<"Q has been pressed:)\n"<<std::flush;
				done =1;
				}
				if(key == XK_1)
				{
					selected = 1;
				}
				if(key == XK_2)
				{
					selected = 2;
				}
				if(key == XK_3)
				{
					selected = 3;
				}
				if(key == XK_4)
				{
					selected = 4;
				}
				
			}
			break;
		}// event handling finished
		if(selected)
		{
			++round;
			int angle = 0;
			int smile = 10;
			if(selected == target)
			{
				++score;
				angle = 180;
				smile = 0;
				//done =1;
			}
			selected = 0;
			//target = rand() % 4 +1;
			XDrawImageString(connection, w, gcXor, window_width/10, window_height/20, score_string.c_str(), score_string.length());
			XDrawImageString(connection, w, gcXor, window_width/2, window_height/20, round_string.c_str(), round_string.length());
			std::ostringstream round_stream;
			std::ostringstream score_stream;
			score_stream<<"Score:"<<score;
			round_stream<<"Round:"<<round;
			round_string = round_stream.str();
			score_string = score_stream.str();
			XDrawImageString(connection, w, gcXor, window_width/10, window_height/20, score_string.c_str(), score_string.length());
			XDrawImageString(connection, w, gcXor, window_width/2, window_height/20, round_string.c_str(), round_string.length());
			int ans_x = 0;
			int ans_y = 0;
			switch (target)
			{
				case 1:
				{
				ans_x = window_width/4;
				ans_y = window_height/4;
				}
				break;
				case 2:
				{
				ans_x = window_width * 3 / 4;
				ans_y = window_height / 4;
				}
				break;
				case 3:
				{
				ans_x = window_width / 4;
				ans_y = window_height *3 / 4;
				}
				break;
				case 4:
				{
				ans_x = window_width * 3 /4;
				ans_y = window_height * 3 /4;
				}
				break;
			}
			XDrawArc(connection, w, gcXor, ans_x, ans_y, 50, 50, 0, 359*64);
				XDrawArc(connection, w, gcXor, ans_x + 10, ans_y + 10 + smile, 30, 30, angle*64, 180*64);
				XDrawArc(connection, w, gcXor, ans_x +10, ans_y + 10, 10, 10, 0, 360*64);
				XDrawArc(connection, w, gcXor, ans_x + 30, ans_y +10, 10, 10, 0, 360*64); 
			XFlush(connection);
			usleep(100000 *3);
			XDrawArc(connection, w, gcXor, ans_x, ans_y, 50, 50, 0, 359*64);
				XDrawArc(connection, w, gcXor, ans_x + 10, ans_y + 10 + smile, 30, 30, angle*64, 180*64);
				XDrawArc(connection, w, gcXor, ans_x +10, ans_y + 10, 10, 10, 0, 360*64);
				XDrawArc(connection, w, gcXor, ans_x + 30, ans_y +10, 10, 10, 0, 360*64); 
			XFlush(connection);
			target = rand() % 4 +1;	
		} // selected
	} // done 
	
	XFreeGC(connection, gc);
	XFreeGC(connection, gcXor);
	XDestroyWindow(connection, w);
	XCloseDisplay(connection);
}
