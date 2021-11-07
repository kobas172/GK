
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

int red, green, blue, yellow, black, white;
unsigned long foreground, background;

//*************************************************************************************************************************
//funkcja przydzielania kolorow

int AllocNamedColor(char *name, Display* display, Colormap colormap)
  {
    XColor col;
    XParseColor(display, colormap, name, &col);
    XAllocColor(display, colormap, &col);
    return col.pixel;
  } 

//*************************************************************************************************************************
// inicjalizacja zmiennych globalnych okreslajacych kolory

int init_colors(Display* display, int screen_no, Colormap colormap)
{
  background = WhitePixel(display, screen_no);  //niech tlo bedzie biale
  foreground = BlackPixel(display, screen_no);  //niech ekran bedzie czarny
  red=AllocNamedColor("red", display, colormap);
  green=AllocNamedColor("green", display, colormap);
  blue=AllocNamedColor("blue", display, colormap);
  yellow=AllocNamedColor("yellow", display, colormap);
  black=AllocNamedColor("black", display, colormap);
  white=AllocNamedColor("white", display, colormap);
}

int translatePoints(int dx, int dy, XPoint tab[], int len)
{
  for (int i = 0; i < len; i++)
  {
  	tab[i].x += dx;
  	tab[i].y += dy;
  }
}

int resizePoints(int dx, int dy, XPoint tab[], int len)
{
  for (int i = 0; i < len; i++)
  {
  	tab[i].x *= dx;
  	tab[i].y *= dy;
  }
}
//*************************************************************************************************************************
// Funkcja rysujaca literki
void drawP(Display* display, Window window, GC gc, int dx, int dy, float scaleX, float scaleY, int colorLitery, int colorObwodki)
{
	int P = 16, P1 = 10, P2 = 7;

	XPoint literkaP[] = {{2, 4}, {2, 24}, {6, 24}, {6, 16}, {10, 16}, {10, 14}, {4, 14}, {4, 6}, {10, 6}, {12, 8}, {12, 12}, {10, 14}, {10, 16}, {14, 12}, {14, 8}, {10, 4}};

	XPoint obwodkaP1[] = {{2, 4}, {2, 24}, {6, 24}, {6, 16}, {10, 16}, {10, 16}, {14, 12}, {14, 8}, {10, 4}, {2, 4}};

	XPoint obwodkaP2[] = {{10, 14}, {4, 14}, {4, 6}, {10, 6}, {12, 8}, {12, 12}, {10, 14}}; 

	for(int i=0; i<P; i++){ 
		literkaP[i].x *= scaleX;
		literkaP[i].y *= scaleY;
        	literkaP[i].x += dx;
        	literkaP[i].y += dy; 	  
    	}

	for(int i=0; i<P1; i++){ 
		obwodkaP1[i].x *= scaleX;
		obwodkaP1[i].y *= scaleY;
        	obwodkaP1[i].x += dx;
        	obwodkaP1[i].y += dy; 	  
    	}
    	
	for(int i=0; i<P2; i++){ 
		obwodkaP2[i].x *= scaleX;
		obwodkaP2[i].y *= scaleY;
        	obwodkaP2[i].x += dx;
        	obwodkaP2[i].y += dy; 	  
    	}
    	XSetLineAttributes(display, gc, 3, LineSolid, 1, 1);
 	XSetForeground(display, gc, colorLitery);
   	XFillPolygon(display, window, gc, literkaP, P, Complex, CoordModeOrigin);
   	XSetForeground(display, gc, colorObwodki);
   	XDrawLines(display, window, gc, obwodkaP1, P1, LineSolid);
   	XDrawLines(display, window, gc, obwodkaP2, P2, LineSolid);   	
}

void drawG(Display* display, Window window, GC gc, int dx, int dy, float scaleX, float scaleY, int colorLitery, int colorObwodki)
{
	int G = 20, G1 = 21;

	XPoint literkaG[] = {{34, 10}, {36, 10}, {32, 6}, {26, 6}, {24, 8}, {24, 22}, {26, 24}, {34, 24}, {36, 22}, {36, 16}, {30, 16}, {30, 18}, {34, 18}, {34, 20}, {32, 22}, {28, 22}, {26, 20}, {26, 10}, {28, 8}, {32, 8}};

	XPoint obwodkaG1[] = {{34, 10}, {36, 10}, {32, 6}, {26, 6}, {24, 8}, {24, 22}, {26, 24}, {34, 24}, {36, 22}, {36, 16}, {30, 16}, {30, 18}, {34, 18}, {34, 20}, {32, 22}, {28, 22}, {26, 20}, {26, 10}, {28, 8}, {32, 8}, {34, 10}};

	for(int i=0; i<G; i++){ 
		literkaG[i].x *= scaleX;
		literkaG[i].y *= scaleY;
        	literkaG[i].x += dx;
        	literkaG[i].y += dy; 	  
    	}

	for(int i=0; i<G1; i++){ 
		obwodkaG1[i].x *= scaleX;
		obwodkaG1[i].y *= scaleY;
        	obwodkaG1[i].x += dx;
        	obwodkaG1[i].y += dy; 	  
    	}
    	
    	XSetLineAttributes(display, gc, 3, LineSolid, 1, 1);
 	XSetForeground(display, gc, colorLitery);
   	XFillPolygon(display, window, gc, literkaG, G, Complex, CoordModeOrigin);
   	XSetForeground(display, gc, colorObwodki);
   	XDrawLines(display, window, gc, obwodkaG1, G1, LineSolid);
}

int drawC(Display* display, Window window, GC gc, int x, int y, int r, int color)
{
    XSetForeground(display, gc, color);
    XFillArc(display, window, gc, x, y, r, r, 0, 360 * 64); 
}

//*************************************************************************************************************************
// Glowna funkcja zawierajaca petle obslugujaca zdarzenia */

int main(int argc, char *argv[])
{
  char            icon_name[] = "Grafika";
  char            title[]     = "Grafika komputerowa";
  Display*        display;    //gdzie bedziemy wysylac dane (do jakiego X servera)
  Window          window;     //nasze okno, gdzie bedziemy dokonywac roznych operacji
  GC              gc;         //tu znajduja sie informacje o parametrach graficznych
  XEvent          event;      //gdzie bedziemy zapisywac pojawiajace sie zdarzenia
  KeySym          key;        //informacja o stanie klawiatury 
  Colormap        colormap;
  int             screen_no;
  XSizeHints      info;       //informacje typu rozmiar i polozenie ok
  
  char            buffer[8];  //gdzie bedziemy zapamietywac znaki z klawiatury
  int             hm_keys;    //licznik klawiszy
  int             to_end;
  int             skala = 1, mnoznik = 1;
  
  char            info1[]     = "LEWY przycisk  - PROSTOKATY";
  XPoint mousePosition;
  
  display    = XOpenDisplay("");                //otworz polaczenie z X serverem pobierz dane od zmiennej srodowiskowej DISPLAY ("")
  screen_no  = DefaultScreen(display);          //pobierz domyslny ekran dla tego wyswietlacza (0)
  colormap = XDefaultColormap(display, screen_no);
  init_colors(display, screen_no, colormap);

  //okresl rozmiar i polozenie okna
  info.x = 100;
  info.y = 150;
  info.width = 500*2;
  info.height = 300*2;
  info.flags = PPosition | PSize;

  //majac wyswietlacz, stworz okno - domyslny uchwyt okna
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),info.x, info.y, info.width, info.height, 7/* grubosc ramki */, foreground, background);
  XSetStandardProperties(display, window, title, icon_name, None, argv, argc, &info);
  //utworz kontekst graficzny do zarzadzania parametrami graficznymi (0,0) domyslne wartosci
  gc = XCreateGC(display, window, 0, 0);
  XSetBackground(display, gc, background);
  XSetForeground(display, gc, foreground);

  //okresl zdarzenia jakie nas interesuja, np. nacisniecie klawisza
  XSelectInput(display, window, (KeyPressMask | ExposureMask | ButtonPressMask| ButtonReleaseMask | Button1MotionMask));
  XMapRaised(display, window);  //wyswietl nasze okno na samym wierzchu wszystkich okien
      
  to_end = FALSE;

 /* petla najpierw sprawdza, czy warunek jest spelniony
     i jesli tak, to nastepuje przetwarzanie petli
     a jesli nie, to wyjscie z petli, bez jej przetwarzania */
  while (to_end == FALSE)
  {
    XNextEvent(display, &event);  // czekaj na zdarzenia okreslone wczesniej przez funkcje XSelectInput
    switch(event.type)
    {
      case Expose:
        if (event.xexpose.count == 0)
        {
    		drawC(display, window, gc, 0,  10, 250, yellow);
    		drawP(display, window, gc, 50, 0, 10, 10, blue, black);
    		drawG(display, window, gc, 0, 40, 4, 4, red, black);
        }
        break;

      case MappingNotify:
        XRefreshKeyboardMapping(&event.xmapping); // zmiana ukladu klawiatury - w celu zabezpieczenia sie przed taka zmiana trzeba to wykonac
        break;

      case ButtonPress:
        if (event.xbutton.button == Button1)  // sprawdzenie czy wcisnieto lewy przycisk		
        {
   		              
        }
        break;
        
      case MotionNotify:
        skala += mnoznik;
        if (skala > 20)
        	mnoznik *= -1;
        if (skala < 1)
        	mnoznik *= -1;
        XSetForeground(display, gc, white);
    	XFillRectangle(display, window, gc, 0, 0, 1000, 600);
    	drawC(display, window, gc, 0,  10, 250, yellow);
    	drawG(display, window, gc, 0, 40, 4, 4, red, black);
      	mousePosition.x = event.xmotion.x;
        mousePosition.y = event.xmotion.y; 
        drawP(display, window, gc, mousePosition.x-50, mousePosition.y-50, 10+skala, 10+skala, blue, black);
	//drawC(display, window, gc, -50+mousePosition.x,  10+mousePosition.y, 250, green);
	//drawC(display, window, gc, 180,  10, 250, blue);
	//drawP(display, window, gc, mousePosition.x, mousePosition.y, 10, 10, red, black);
	//drawG(display, window, gc, 0, 0, 10, 10, red, black);              
      
      	break;


      case KeyPress:
        hm_keys = XLookupString(&event.xkey, buffer, 8, &key, 0);
        if (hm_keys == 1)
        {
          if (buffer[0] == 'q') to_end = TRUE;        // koniec programu
          
        }

      default:
        break;
    }
  }

  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return 0;
}
