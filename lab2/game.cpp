#define XK_LATIN1
//#define DEBUG_ZCU
#include<X11/Xlib.h>
#include<unistd.h>
#include<stdlib.h>
#include<X11/keysymdef.h>
#include<iostream>
#include<time.h>
#include<string>
#include<sstream>
#include<mutex>
#include<thread>
#include<stdio.h>

#ifdef DEBUG_ZCU
	static std::mutex cout_mutex;
#endif

class rectangle_flag
{
	public:
	int x;
	int y;
	int width;
	int height;
	bool on;
	void draw(Display* display,  const Window& w, const GC& gc)
	{
		if(on)
		{
		XLockDisplay(display);
		XDrawLine(display, w, gc, x, y, x+width, y+height);
		XDrawLine(display, w, gc, x, y+height, x+width,y);
		XUnlockDisplay(display); 
		}
	}
	void erase(Display* display, const Window& w, const GC& gc)
	{
		draw(display, w, gc);
		on = false;
	}
};

class face
{
	public:
	int x;
	int y;
	int width;
	int height;
	bool smile;
	bool on;
	int position;
	void draw(Display* display, const Window& w, const GC& gc)
	{
		if(on)
		{
			XLockDisplay(display);
			XDrawArc(display, w, gc, x, y, width, height, 0, 360*64);
			XDrawArc(display, w, gc, x+width/3 - width/10, y + height/3 - height/10,width/5, height/5, 0, 360*64);
			XDrawArc(display, w, gc, x+width*2/3 - width/10 , y+height/3 - height/10, width/5, height/5, 0, 360*64);
			XDrawLine(display, w, gc, x+width/2, y+height/3, x+width/2, y+height/2);
			if(smile)
			{
				XDrawArc(display, w, gc, x+width/4, y+height/4+height/10,width/2, height/2, 180*64, 180*64);
			}else
			{
				XDrawArc(display, w, gc, x+width/4, y+height*2/3, width/2, height/2, 0, 180*64);
			}
			XUnlockDisplay(display);
		}
	}
	void erase(Display* display, const Window& w, const GC& gc)
	{
		draw(display, w, gc);
		on = false;
	}
};

class text
{
public:
	int x;
	int y;
	std::string m_text;
	bool on;
	void draw(Display* display, const Window& w, const GC& gc)
	{
		if(on)
		{
		XLockDisplay(display);
		XDrawImageString(display, w, gc, x, y, m_text.c_str(), m_text.length());
		XUnlockDisplay(display);
		}
	}
	void erase(Display* display,const Window& w, const GC& gc)
	{
		XLockDisplay(display);
		XDrawImageString(display, w, gc, x, y, m_text.c_str(),m_text.length()); 
		XUnlockDisplay(display);
		on = false;
	}
};


class area
{
public:
	int x;
	int y;
	int width;
	int height;
	bool on;
	int selection;
	void draw(Display* display, const Window& w, const GC& gc)
	{
		if(on)
		{
		XLockDisplay(display);
		XFillRectangle(display, w, gc, x, y, width, height);
		XUnlockDisplay(display);
		}
	} 
	void erase(Display* display, const Window& w, const GC& gc)
	{
		XLockDisplay(display);
		XFillRectangle(display, w, gc, x, y, width, height);
		XUnlockDisplay(display);
		on = false;
	}
};


class player_window
{ 
public:
	explicit player_window(Display* display_ptr,int window_width = 500, int window_height = 500);
	void handle_expose_event();
	void draw_face(int position, bool smile);
	void remove_face();
	void display_score(const int& score);
	void display_round(const int& round);
	void display_message(const std::string& msg);
	void clear_message();
	void show_flag();
	void remove_flag();
	virtual ~player_window();
	int get_area_number(int x, int y);
	void mark_area(int area_number);
	void unmark_area();
	Window get_window() const;
private:
	Window m_window;
	GC m_gc;
	GC m_gcXor;
	Display* m_display;
	XWindowAttributes m_attributes;
	int m_black_color;
	int m_white_color;
	face m_face;
	rectangle_flag m_flag;
	area m_area;
	text m_score;
	text m_round;
	text m_message;
	void draw_face();
	void display_score();
	void display_round();
	void display_message();
	void redraw_flag();
	void redraw_area();
	
};
void player_window::handle_expose_event()
{
	#ifdef DEBUG_ZCU
		{
			std::lock_guard<std::mutex> guard(cout_mutex);
			std::cout<<__LINE__<<"\tplayer_window::handle_expose_event\n"<<std::flush;
		}
	#endif
	XLockDisplay(m_display);
	XGetWindowAttributes(m_display, m_window, &m_attributes);
	XUnlockDisplay(m_display);
	int window_width = m_attributes.width;
	int window_height = m_attributes.height;
	XLockDisplay(m_display);
	XDrawRectangle(m_display, m_window, m_gc, window_width/10, window_height/10, window_width*4/5, window_height*4/5);
	XDrawLine(m_display, m_window, m_gc, window_width/2, window_height/10, window_width/2, window_height*9/10);
	XDrawLine(m_display, m_window, m_gc, window_width/10, window_height/2, window_width*9/10, window_height/2);
	XDrawRectangle(m_display, m_window, m_gc, window_width*37/40, window_height/10, window_width/20, window_height*4/5);  // flag rectangle
	XUnlockDisplay(m_display);
	redraw_flag();
	redraw_area();
	display_message();
	display_round();
	display_score();
	draw_face();
	XLockDisplay(m_display);
	XFlush(m_display);
	XUnlockDisplay(m_display);
}

Window player_window::get_window() const
{
	return m_window;
}

player_window::player_window(Display* display_ptr, int window_width, int window_height)
{
	this->m_display = display_ptr;
	XLockDisplay(m_display);
	m_black_color = BlackPixel(m_display, DefaultScreen(m_display));
	m_white_color = WhitePixel(m_display, DefaultScreen(m_display));
	m_window = XCreateSimpleWindow(m_display, DefaultRootWindow(m_display), 0, 0, window_width, window_height, 0, m_white_color, m_black_color);  
	this->m_gc = XCreateGC(m_display, m_window,0, NULL);
	this->m_gcXor = XCreateGC(m_display,m_window, 0, NULL);
	XSetFunction(m_display, m_gcXor, GXxor);
	XSetForeground(m_display, m_gc, m_white_color);
	XSetForeground(m_display, m_gcXor, m_white_color);
	XSelectInput(m_display, m_window, ExposureMask | ButtonPressMask | KeyPressMask);
	XGetWindowAttributes(m_display, m_window, &m_attributes); 
	m_face.on = false;
	m_area.on = false;
	m_score.on = true;
	m_round.on = true;
	m_message.on = false;
	XMapWindow(m_display, m_window);
	XUnlockDisplay(m_display);
}

void player_window::draw_face(int position, bool smile)
{
	m_face.erase(m_display, m_window, m_gcXor);
	m_face.x = 0;
	m_face.y = 0;
	m_face.smile = smile;
	m_face.on = true;
	int window_width = m_attributes.width;
	int window_height = m_attributes.height;
	m_face.position = position;
	switch (position)
	{
		case 1:
		{
		m_face.x = window_width/4;
		m_face.y = window_height/4;
		}
		break;
		case 2:
		{
		m_face.x = window_width * 3 / 4;
		m_face.y = window_height / 4;
		}
		break;
		case 3:
		{
		m_face.x = window_width / 4;
		m_face.y = window_height *3 / 4;
		}
		break;
		case 4:
		{
		m_face.x = window_width * 3 /4;
		m_face.y = window_height * 3 /4;
		}
		break;
	}
	m_face.width = m_attributes.width/10;
	m_face.height = m_attributes.height/10;
	m_face.draw(m_display, m_window, m_gcXor);
	XLockDisplay(m_display);
	XFlush(m_display);
	XUnlockDisplay(m_display);
}
void player_window::draw_face()
{
	if(m_face.on)
	{
	//m_face.erase(m_display, m_window, m_gcXor);
	int window_width = m_attributes.width;
	int window_height = m_attributes.height;
	switch (m_face.position)
	{
		case 1:
		{
		m_face.x = window_width/4;
		m_face.y = window_height/4;
		}
		break;
		case 2:
		{
		m_face.x = window_width * 3 / 4;
		m_face.y = window_height / 4;
		}
		break;
		case 3:
		{
		m_face.x = window_width / 4;
		m_face.y = window_height *3 / 4;
		}
		break;
		case 4:
		{
		m_face.x = window_width * 3 /4;
		m_face.y = window_height * 3 /4;
		}
		break;
	}
	m_face.width = m_attributes.width/10;
	m_face.height = m_attributes.height/10;
	m_face.draw(m_display, m_window, m_gcXor);
	XLockDisplay(m_display);
	XFlush(m_display);
	XUnlockDisplay(m_display);
	}	
}

void player_window::remove_face()
{
	if(m_face.on)
	{
	m_face.erase(m_display, m_window, m_gcXor);
	m_face.on = false;
	}
	XLockDisplay(m_display);
	XFlush(m_display);
	XUnlockDisplay(m_display);
}

void player_window::display_score(const int& score)
{
	m_score.erase(m_display, m_window, m_gcXor);
	std::ostringstream score_stream;
	score_stream<<"Score:"<<score;
	m_score.m_text = score_stream.str();
	m_score.on = true;
	m_score.x = m_attributes.width/10;
	m_score.y = m_attributes.height/25;
	m_score.draw(m_display, m_window, m_gcXor);
	XLockDisplay(m_display);
	XFlush(m_display);
	XUnlockDisplay(m_display);
}

void player_window::display_score()
{
	if(m_score.on)
	{
	//m_score.erase(m_display, m_window, m_gcXor);
	//m_score.on = true;
	m_score.x = m_attributes.width/10;
	m_score.y = m_attributes.height/25;
	m_score.draw(m_display, m_window, m_gcXor);
	XLockDisplay(m_display);
	XFlush(m_display);
	XUnlockDisplay(m_display);
	}	
}
void player_window::display_round(const int& round)
{
	m_round.erase(m_display, m_window, m_gcXor);
	std::ostringstream round_stream;
	round_stream<<"Round: "<<round;
	m_round.m_text = round_stream.str();
	m_round.on = true;
	m_round.x = m_attributes.width/2;
	m_round.y = m_attributes.height/25;
	m_round.draw(m_display, m_window, m_gcXor);
	XLockDisplay(m_display);
	XFlush(m_display);
	XUnlockDisplay(m_display);
}

void player_window::display_round()
{
	if(m_round.on)
	{
	m_round.erase(m_display, m_window, m_gcXor);
	m_round.on = true;
	m_round.x = m_attributes.width/2;
	m_round.y = m_attributes.height/25;
	m_round.draw(m_display, m_window, m_gcXor);
	}
}
void player_window::display_message(const std::string& msg)
{
	m_message.erase(m_display, m_window, m_gcXor);
	m_message.m_text = msg;
	m_message.on = true;
	display_message();
	
}
void player_window::display_message()
{
	if(m_message.on)
	{
	//m_message.erase(m_display, m_window, m_gcXor);
	//m_message.on = true;
	m_message.x = m_attributes.width/10;
	m_message.y = m_attributes.height/15;
	m_message.draw(m_display, m_window, m_gcXor);
	XLockDisplay(m_display);
	XFlush(m_display);
	XUnlockDisplay(m_display);
	}
}
void player_window::clear_message()
{
	if(m_message.on) 
	{
		m_message.erase(m_display, m_window, m_gcXor);
		m_message.on = false;
	}
}
void player_window::show_flag()
{
	m_flag.on = true;
	redraw_flag();
}

void player_window::redraw_flag()
{
	if(m_flag.on)
	{
	//m_flag.erase(m_display, m_window, m_gcXor);
	//m_flag.on = true;
	int window_width = m_attributes.width;
	int window_height = m_attributes.height;
	m_flag.x = window_width*37/40;
	m_flag.y = window_height/10;
	m_flag.width = window_width/20;
	m_flag.height = window_height*4/5;
	m_flag.draw(m_display, m_window, m_gcXor); 
	}
}

void player_window::remove_flag()
{
	if(m_flag.on)
	{
		m_flag.erase(m_display, m_window, m_gcXor);
		m_flag.on = false;
	}
}


player_window::~player_window()
{
	XLockDisplay(m_display);
	XFreeGC(m_display, m_gc);
	XFreeGC(m_display, m_gcXor);
	XDestroyWindow(m_display, m_window);
	XUnlockDisplay(m_display);
	XCloseDisplay(m_display);	
}


int player_window::get_area_number(int x, int y)
{
	XLockDisplay(m_display);
        XGetWindowAttributes(m_display, m_window, &m_attributes);
        XUnlockDisplay(m_display);

		int selected = 0;
		int window_width = m_attributes.width;
		int window_height = m_attributes.height;
		if((x > static_cast<int>(window_width/10)) && (x < static_cast<int>(window_width*9/10)) 
			&& (y > static_cast<int>(window_height/10)) && (y< static_cast<int>(window_height*9/10)))
		{
	#ifdef DEBUG_ZCU
		std::lock_guard<std::mutex> guard(cout_mutex);
		std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!INSIDE "<<std::flush;
	#endif
			if(x < window_width/2)
			{
				if(y<window_height/2)
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
				if(y < window_height/2)
				{
					selected = 2;
				}
				else
				{
					selected = 4;
				}
			}
		}
	#ifdef DEBUG_ZCU
		if(!selected)
		{
			std::lock_guard<std::mutex> guard(cout_mutex);
			std::cout<<"width:"<<window_width<<"\theight:"<<window_height<<"\tx:"<<x<<" y:"<<y<<"\t wrong selection !!!!!"
			<<"["<<static_cast<int>(window_width/10)<<", "<<static_cast<int>(window_width*9/10)<<"]\n"<<
			"["<<static_cast<int>(window_height/10)<<", "<<static_cast<int>(window_height*9/10)<<"]\n"<<std::flush;
		}
	#endif
	return selected;
}
void player_window::mark_area(int area_number)
{
	m_area.on = true;
	m_area.selection = area_number;
	redraw_area();
}

void player_window::redraw_area()
{
	if(m_area.on)
	{
	//m_area.erase(m_display, m_window, m_gcXor);
	//m_area.on = true;
	int window_width = m_attributes.width;
	int window_height = m_attributes.height;
	m_area.width = window_width*8/25;
	m_area.height = window_height*8/25;
		switch(m_area.selection)
		{
			case 1:
			{
				m_area.x = window_width/10 + window_width/25;
				m_area.y = window_height/10 + window_height/25;
			}
			break;
			case 2:
			{
				m_area.x = window_width/2 + window_width/25;
				m_area.y = window_height/10 + window_height/25;
			}
			break;
			case 3:
			{
				m_area.x = window_width/10 + window_width/25;
				m_area.y = window_height/2 + window_height/25;
			}
			break;
			case 4:
			{
				m_area.x = window_width/2 + window_width/25;
				m_area.y = window_height/2 + window_height/25;
			}
			break;
			default:
			break;
		}
	m_area.draw(m_display, m_window, m_gcXor);
	}
}
void player_window::unmark_area()
{
	if(m_area.on)
	{
		m_area.erase(m_display, m_window, m_gcXor);
		m_area.on = false;
	}
}

class SynchronizationMonitor
{
public:	
	SynchronizationMonitor();
	int get_selected();
	void set_selected(const int& inp);
	bool get_selection_flag();
	void set_selection_flag(const bool& inp);
	int get_hit();
	void set_hit(const int& inp);
	bool get_hit_flag();
	void set_hit_flag(const bool& inp);
	int get_round();
	void up_round();
	void up_score();
	int get_score();
	bool check_selector_event();
	void signal_selector();
	bool check_hiter_event();
	void signal_hiter();
	bool get_hit_stop();
	bool get_select_stop();
	void set_hit_stop();
	void set_select_stop();
private:
	std::mutex selection_mutex;
	int m_selected;
	bool m_selected_flag;
	std::mutex hit_mutex;
	int m_hit;
	bool m_hit_flag;
	std::mutex round_mutex;
	int round;
	std::mutex score_mutex;
	int score;
	std::mutex event_mutex;
	bool event_selector;
	bool event_hiter;
	std::mutex stop_mutex;
	bool hit_stop;
	bool select_stop;
};
void SynchronizationMonitor::signal_selector()
{
	std::lock_guard<std::mutex> guard(event_mutex);
	event_selector = true;
}
void SynchronizationMonitor::signal_hiter()
{
	std::lock_guard<std::mutex> guard(event_mutex);
	event_hiter = true;
}

bool SynchronizationMonitor::check_selector_event()
{
	std::lock_guard<std::mutex> guard(event_mutex);
	if(event_selector) 
	{
		event_selector = false;
		return true;
	}
	return false;
}
bool SynchronizationMonitor::check_hiter_event()
{
	std::lock_guard<std::mutex> guard(event_mutex);
	if(event_hiter)
	{
		event_hiter = false;
		return true;
	}
	return false;
}
SynchronizationMonitor::SynchronizationMonitor()
{
	round = 1;
	score = 0;
	m_selected = 0;
	m_selected_flag = false;
	m_hit = 0;
	m_hit_flag = false;
	event_selector = false;
	event_hiter = false;
	select_stop = false;
	hit_stop = false;
}
bool SynchronizationMonitor::get_hit_stop()
{
	std::lock_guard<std::mutex> guard(stop_mutex);
	return hit_stop;
}
void SynchronizationMonitor::set_hit_stop()
{
	std::lock_guard<std::mutex> guard(stop_mutex);
	hit_stop = true;
}
bool SynchronizationMonitor::get_select_stop()
{
	std::lock_guard<std::mutex> guard(stop_mutex);
	return select_stop;
}
void SynchronizationMonitor::set_select_stop()
{
	std::lock_guard<std::mutex> guard(stop_mutex);
	select_stop = true;
}

void SynchronizationMonitor::up_score()
{
	std::lock_guard<std::mutex> guard(score_mutex);
	++score;
}
int SynchronizationMonitor::get_score()
{
	std::lock_guard<std::mutex> guard(score_mutex);
	return score;
}

int SynchronizationMonitor::get_round()
{
	std::lock_guard<std::mutex> guard(round_mutex);
	return round;
}
void SynchronizationMonitor::up_round()
{
	std::lock_guard<std::mutex> guard(round_mutex);
	++round;
}

int SynchronizationMonitor::get_selected()
{
	std::lock_guard<std::mutex> guard(selection_mutex);
	return m_selected;
}
void SynchronizationMonitor::set_selected(const int& inp)
{
	std::lock_guard<std::mutex> guard(selection_mutex);
	m_selected = inp;
	m_selected_flag = true;
}
bool SynchronizationMonitor::get_selection_flag()
{
	std::lock_guard<std::mutex> guard(selection_mutex);
	return m_selected_flag;
}
void SynchronizationMonitor::set_selection_flag(const bool& inp)
{
	std::lock_guard<std::mutex> guard(selection_mutex);
	m_selected_flag = inp;
}
int SynchronizationMonitor::get_hit()
{
	std::lock_guard<std::mutex> guard(hit_mutex);
	return m_hit ;
}
void SynchronizationMonitor::set_hit(const int& inp)
{
	std::lock_guard<std::mutex> guard(hit_mutex);
	m_hit = inp;
}
bool SynchronizationMonitor::get_hit_flag()
{
	std::lock_guard<std::mutex> guard(hit_mutex);
	return m_hit_flag;
}
void SynchronizationMonitor::set_hit_flag(const bool& inp)
{
	std::lock_guard<std::mutex> guard(hit_mutex);
	m_hit_flag = inp;
}

class player
{
public:
	void player_thread();
	explicit player(Display* display_ptr, SynchronizationMonitor* synch_ptr );

protected:
	player_window m_w;
	Display* m_display;
	SynchronizationMonitor* const monitor;
	void stop();
private:
	virtual void handle_mouse_left_click(const XEvent& event) = 0;
	virtual void handle_mouse_right_click(const XEvent& event) = 0;
	virtual void handle_key_press(const XEvent& event) = 0;
	virtual void react_on_other() = 0;
	bool m_stopped;
};

void player::handle_mouse_right_click(const XEvent& event)
{
	perror("handle mouse right click!!!");
	stop();
}
void player::handle_mouse_left_click(const XEvent& event)
{
	perror("Handle mouse left click!!!!");
	stop();
}
void player::handle_key_press(const XEvent& event)
{
	perror("Handle key press!!!!");
	stop();
}
void player::react_on_other()
{
	perror("react on other!!!!");
	stop();
}

player::player(Display* display_ptr, SynchronizationMonitor* const synch_ptr)
:monitor(synch_ptr),
m_display(display_ptr),
m_w(display_ptr)
{m_stopped = false;}

void player::stop()
{
	m_stopped = true;
	// we need to somewhow message the second thread
}

Bool predicate_function(Display* display, XEvent* event, XPointer arg)
{
	if(event->xany.window == *reinterpret_cast<Window*>(arg)) return true;
	return false;
}
 
void player::player_thread()
{
	try{
		while(!m_stopped){
		XEvent e;
		bool event_valid;
		XLockDisplay(m_display);
		//Window window_ptr = m_w.get_window();
		event_valid = XCheckWindowEvent(m_display, m_w.get_window(),ExposureMask| ButtonPressMask| KeyPressMask, &e);//predicate_function, reinterpret_cast<char*>(&window_ptr));
		XUnlockDisplay(m_display);
		//std::cout<<"react on other\n"<<std::flush;
		react_on_other();
		if(e.xany.window == m_w.get_window() && event_valid)
		switch(e.type)
		{
			case ButtonPress:
			switch(e.xbutton.button)
			{
				case Button1:
				{
					//std::cout<<"handle_mouse left\n"<<std::flush;
					handle_mouse_left_click(e);
				}
				break;		
				case Button3:
				{
					//std::cout<<"handle mouse right\n"<<std::flush;
					handle_mouse_right_click(e);
				}
				break;
			}
			break;
			case Expose:
			{
				//std::cout<<"handle expose event\n"<<std::flush;
				m_w.handle_expose_event();
			}
			break;
			case KeyPress:
			{
				//std::cout<<"handle key press\n"<<std::flush;
				handle_key_press(e);
			}
			break;
		}// event handling finished
		} // end while loop 	
	}catch(...)
	{
		// we catch every exception here :)
	}
}

class selector_player: public player
{
	public:
	explicit selector_player(Display* display_ptr, SynchronizationMonitor* synch_ptr);
	private:
	virtual void handle_mouse_left_click(const XEvent& event);
	virtual void handle_mouse_right_click(const XEvent& event);
	virtual void handle_key_press(const XEvent& event);
	virtual void react_on_other();
	void handle_selection(const int& inp);
	int score;
};

void selector_player::react_on_other()
{
	#ifdef DEBUG_ZCU
		{
		std::lock_guard<std::mutex> guard(cout_mutex);
		std::cout<<__LINE__<<"\tselector_player::react_on_other\n"<<std::flush;
		}
	#endif
	
	if( monitor->check_selector_event())
	{
		m_w.unmark_area();
		m_w.remove_flag();
		m_w.display_message("It's time to select an area for your opponenet");
		int current_round = monitor->get_round();
		int new_score = current_round - monitor->get_score() - 1;
		int hit = monitor->get_hit();
			if(new_score > this->score)
			{
				this->score = new_score;
				m_w.draw_face(hit,true);
			}else
			{
				m_w.draw_face(hit,false);
			}
		m_w.display_score(this->score);
		m_w.display_round(current_round);
	}
	if(monitor->get_hit_stop())
	{
		m_w.display_message("Other player have stoped the game, You Win!");
	}
}

void selector_player::handle_selection(const int& inp)
{
	if(!monitor->get_selection_flag())
	{
		monitor->set_selected(inp);
		monitor->set_selection_flag(true);
		monitor->signal_hiter();
		m_w.remove_face();
		m_w.mark_area(inp);
		m_w.show_flag();
		m_w.display_message("Now your opponent is guessing!");
	}else
	{
		m_w.display_message("Sorry, you need to wait until second player hit");
	}
}

selector_player::selector_player(Display* display_ptr, SynchronizationMonitor* synch_ptr)
:player(display_ptr, synch_ptr), score(0)
{
	m_w.display_message("Please select an area for your opponent");
	m_w.remove_flag();
	m_w.remove_face();
}

void selector_player::handle_mouse_left_click(const XEvent& event)
{
	#ifdef DEBUG_ZCU
		{
		std::lock_guard<std::mutex> guard(cout_mutex);
		std::cout<<__LINE__<<"\tselector_player::handle_mouse_left_click\n"<<std::flush;
		}
	#endif
	int section = m_w.get_area_number(event.xbutton.x, event.xbutton.y);
	if(section) handle_selection(section);
}
void selector_player::handle_mouse_right_click(const XEvent& event)
{
	#ifdef DEBUG_ZCU
		{
		std::lock_guard<std::mutex> guard(cout_mutex);
		std::cout<<__LINE__<<"\tselector_player::handle_mouse_right_click\n"<<std::flush;
		}
	#endif
	monitor->set_select_stop();
	stop();
}
void selector_player::handle_key_press(const XEvent& event)
{
	#ifdef DEBUG_ZCU
		{
		std::lock_guard<std::mutex> guard(cout_mutex);
		std::cout<<__LINE__<<"\nselector_player::handle_key_press\n"<<std::flush;
		}
	#endif

		XEvent tmp = event;
		XLockDisplay(m_display);
		int key = XLookupKeysym(&tmp.xkey, 0);
		XUnlockDisplay(m_display); 
		if(key == XK_q)
		{
			monitor->set_select_stop();
			stop();
		}
		if(key == XK_1)
		{
			handle_selection(1);
		}
		if(key == XK_2)
		{
			handle_selection(2);
		}
		if(key == XK_3)
		{
			handle_selection(3);
		}
		if(key == XK_4)
		{
			handle_selection(4);
		}
}

class guessing_player: public player
{
	public:
	explicit guessing_player(Display* display_ptr, SynchronizationMonitor* synch_ptr);
	private:
	virtual void handle_mouse_left_click(const XEvent& event);
	virtual void handle_mouse_right_click(const XEvent& event);
	virtual void handle_key_press(const XEvent& event);
	virtual void react_on_other();
	void handle_hit(const int& inp);
	int score;
};

guessing_player::guessing_player(Display* display_ptr, SynchronizationMonitor* synch_ptr)
:player(display_ptr, synch_ptr), score(0)
{	
	m_w.display_message("Please wait until your opponent make target for you");
	m_w.show_flag();
	m_w.remove_face();
	m_w.display_score(0);
	m_w.display_round(1);
}

void guessing_player::react_on_other()
{
	#ifdef DEBUG_ZCU
		{
			std::lock_guard<std::mutex> guard(cout_mutex);
			std::cout<<__LINE__<<"\tguessing_player::react_on_other\n"<<std::flush;
		}
	#endif
	if(monitor->check_hiter_event())
	{
		//std::cout<<"!!!!!!!!!!"<<std::flush;
		m_w.display_message("You can try to hit now");
		m_w.remove_flag();
	}
	if(monitor->get_select_stop())
	{
		m_w.display_message("Other player have stopped the game, You Win!");
	}
}

void guessing_player::handle_hit(const int& inp)
{
	#ifdef DEBUG_ZCU
		{
			std::lock_guard<std::mutex> guard(cout_mutex);
			std::cout<<__LINE__<<"\tguessing_player::handle_hit\n"<<std::flush;
		}
	#endif
	
	if(monitor->get_selection_flag())
	{
		m_w.show_flag();
		int selection = monitor->get_selected();
		monitor->set_hit(inp);
		monitor->set_hit_flag(true);
		if(selection == inp)
		{
			++score;
			monitor->up_score();
			monitor->set_hit_flag(true);
			m_w.draw_face(inp, true);
			m_w.display_score(score);
		}else
		{
			monitor->set_hit_flag(true);
			m_w.draw_face(inp, false);
			m_w.display_score(score);
		}
		monitor->up_round();
		m_w.display_round(monitor->get_round());
		monitor->set_selection_flag(false);
		monitor->signal_selector();
	}else
	{
		m_w.clear_message();
		m_w.display_message("Wait until user will select target for you");
	}
}

void guessing_player::handle_mouse_left_click(const XEvent& event)
{
	#ifdef DEBUG_ZCU
		{
		std::lock_guard<std::mutex> guard(cout_mutex);
		std::cout<<__LINE__<<"\tguessing_player::handle_mouse_left_click\n"<<std::flush;
		}
	#endif
	int hit = m_w.get_area_number(event.xbutton.x, event.xbutton.y);
	if(hit) handle_hit(hit);
}
void guessing_player::handle_mouse_right_click(const XEvent& event)
{
	#ifdef DEBUG_ZCU
		{
			std::lock_guard<std::mutex> guard(cout_mutex);
			std::cout<<__LINE__<<"\tguessing_player::handle_mouse_right_click\n"<<std::flush;
		}
	#endif
	monitor->set_hit_stop();
	stop();
}
void guessing_player::handle_key_press(const XEvent& event)
{
	#ifdef DEBUG_ZCU
		{
			std::lock_guard<std::mutex> guard(cout_mutex);
			std::cout<<__LINE__<<"\tguessing_player::handle_key_press\n"<<std::flush;
		}
	#endif
	
		XEvent tmp = event;
		XLockDisplay(m_display);
		int key = XLookupKeysym(&tmp.xkey, 0);
		XUnlockDisplay(m_display);
		if(key == XK_q)
		{
			monitor->set_hit_stop();
			stop();
		}
		if(key == XK_1)
		{
			handle_hit(1);
		}
		if(key == XK_2)
		{
			handle_hit(2);
		}
		if(key == XK_3)
		{
			handle_hit(3);
		}
		if(key == XK_4)
		{
			handle_hit(4);
		}	
}


extern int optind;
extern int optopt;
extern int opterr;

int main(int argc, char* argv[])
{
if(!XInitThreads()){exit(EXIT_FAILURE);}
std::cout<<__LINE__<<"\tprogram started"<<std::flush;
opterr = 0; // let me handling passing errors;
const char* selector_display_string = 0;
const char* player_display_string = 0; 
	
const char* const valid_options = "sg";
	int c; // result from getopt
	while((c = getopt(argc, argv, valid_options)) != -1)
	{
		switch(c)
		{
			case 's':
			selector_display_string = argv[optind];
			break;
			case 'g':
			player_display_string = argv[optind];
			default:
			break;
		//	exit(EXIT_FAILURE);
		}
	}
	#ifdef DEBUG_ZCU
	selector_display_string = ":1";//"127.0.0.1:0.0";
	player_display_string = ":1";
	#endif
	std::cout<<"selector display:"<<selector_display_string<<"\t"<<"hiter display string:"<<player_display_string<<"\n"<<std::flush;
	Display* selector_display = XOpenDisplay(selector_display_string);
	Display* player_display = XOpenDisplay(player_display_string);
	SynchronizationMonitor monitor;
	guessing_player guesser(player_display, &monitor);
	selector_player selector(selector_display, &monitor);
	std::thread player1(&selector_player::player_thread, std::ref(selector));
	std::thread player2(&guessing_player::player_thread, std::ref(guesser));
	if(player2.joinable()) player2.join();
	if(player1.joinable()) player1.join();
}
