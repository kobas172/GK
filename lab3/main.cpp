/******************************************************************
 Grafika komputerowa, środowisko MS Windows - program  przykładowy
 *****************************************************************/

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

POINT heart[13];
int vertex = -1;

//deklaracja funkcji obslugi okna
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void createLetter(POINT* first, POINT* second)
{
	first[0].x = 1, first[0].y = 1;
	first[1].x = 1, first[1].y = 7;
	first[2].x = 2, first[2].y = 7;
	first[3].x = 2, first[3].y = 5;
	first[4].x = 4, first[4].y = 4;
	first[5].x = 5, first[5].y = 3;
	first[6].x = 5, first[6].y = 2;
	first[7].x = 4, first[7].y = 1;

	second[0].x = 2, second[0].y = 2;
	second[1].x = 2, second[1].y = 4;
	second[2].x = 4, second[2].y = 3;
	second[3].x = 4, second[3].y = 2;
}

void createHeart(POINT* heart)
{
	heart[0].x = 5, heart[0].y = 4;
	heart[1].x = 4, heart[1].y = 3;
	heart[2].x = 3, heart[2].y = 2;
	heart[3].x = 1, heart[3].y = 3;
	heart[4].x = 2, heart[4].y = 6;
	heart[5].x = 4, heart[5].y = 9;
	heart[6].x = 5, heart[6].y = 11;
	heart[7].x = 6, heart[7].y = 9;
	heart[8].x = 8, heart[8].y = 6;
	heart[9].x = 9, heart[9].y = 3;
	heart[10].x = 7, heart[10].y = 2;
	heart[11].x = 6, heart[11].y = 3;
	heart[12].x = 5, heart[12].y = 4;
}

void setLetter(POINT* first, POINT* second, int dx, int dy, int scaleX, int scaleY)
{
	for (int i = 0; i < 8; i++)
	{
		first[i].x *= scaleX;
		first[i].y *= scaleY;
		first[i].x += dx;
		first[i].y += dy;
	}
	for (int i = 0; i < 4; i++)
	{
		second[i].x *= scaleX;
		second[i].y *= scaleY;
		second[i].x += dx;
		second[i].y += dy;
	}
}

void setHeart(POINT* heart, int dx, int dy, int scaleX, int scaleY)
{
	for (int i = 0; i < 13; i++)
	{
		heart[i].x *= scaleX;
		heart[i].y *= scaleY;
		heart[i].x += dx;
		heart[i].y += dy;
	}
}

//funkcja Main - dla Windows
 int WINAPI WinMain(HINSTANCE hInstance,
               HINSTANCE hPrevInstance,
               LPSTR     lpCmdLine,
               int       nCmdShow)
{

	 createHeart(heart);
	 setHeart(heart, 500, 175, 20, 20);



	MSG meldunek;		  //innymi slowy "komunikat"
	WNDCLASS nasza_klasa; //klasa głównego okna aplikacji
	HWND okno;
	static char nazwa_klasy[] = "Podstawowa";
	
	GdiplusStartupInput gdiplusParametry;// parametry GDI+; domyślny konstruktor wypełnia strukturę odpowiednimi wartościami
	ULONG_PTR	gdiplusToken;			// tzw. token GDI+; wartość uzyskiwana przy inicjowaniu i przekazywana do funkcji GdiplusShutdown
   
	// Inicjujemy GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusParametry, NULL);

	//Definiujemy klase głównego okna aplikacji
	//Okreslamy tu wlasciwosci okna, szczegoly wygladu oraz
	//adres funkcji przetwarzajacej komunikaty
	nasza_klasa.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	nasza_klasa.lpfnWndProc   = WndProc; //adres funkcji realizującej przetwarzanie meldunków 
 	nasza_klasa.cbClsExtra    = 0 ;
	nasza_klasa.cbWndExtra    = 0 ;
	nasza_klasa.hInstance     = hInstance; //identyfikator procesu przekazany przez MS Windows podczas uruchamiania programu
	nasza_klasa.hIcon         = 0;
	nasza_klasa.hCursor       = LoadCursor(0, IDC_ARROW);
	nasza_klasa.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
	nasza_klasa.lpszMenuName  = "Menu" ;
	nasza_klasa.lpszClassName = nazwa_klasy;

    //teraz rejestrujemy klasę okna głównego
    RegisterClass (&nasza_klasa);
	
	/*tworzymy okno główne
	okno będzie miało zmienne rozmiary, listwę z tytułem, menu systemowym
	i przyciskami do zwijania do ikony i rozwijania na cały ekran, po utworzeniu
	będzie widoczne na ekranie */
 	okno = CreateWindow(nazwa_klasy, "Grafika komputerowa", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	
	/* wybór rozmiaru i usytuowania okna pozostawiamy systemowi MS Windows */
   	ShowWindow (okno, nCmdShow) ;
    
	//odswiezamy zawartosc okna
	UpdateWindow (okno) ;

	// GŁÓWNA PĘTLA PROGRAMU
	while (GetMessage(&meldunek, NULL, 0, 0))
     /* pobranie komunikatu z kolejki; funkcja GetMessage zwraca FALSE tylko dla
	 komunikatu WM_QUIT; dla wszystkich pozostałych komunikatów zwraca wartość TRUE */
	{
		TranslateMessage(&meldunek); // wstępna obróbka komunikatu
		DispatchMessage(&meldunek);  // przekazanie komunikatu właściwemu adresatowi (czyli funkcji obslugujacej odpowiednie okno)
	}

	GdiplusShutdown(gdiplusToken);
	
	return (int)meldunek.wParam;
}

/********************************************************************
FUNKCJA OKNA realizujaca przetwarzanie meldunków kierowanych do okna aplikacji*/
LRESULT CALLBACK WndProc (HWND okno, UINT kod_meldunku, WPARAM wParam, LPARAM lParam)
{
	HMENU mPlik, mInfo, mGlowne;
    	
/* PONIŻSZA INSTRUKCJA DEFINIUJE REAKCJE APLIKACJI NA POSZCZEGÓLNE MELDUNKI */
	switch (kod_meldunku) 
	{
	case WM_CREATE:  //meldunek wysyłany w momencie tworzenia okna
		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Zapiszcz...");
		AppendMenu(mPlik, MF_SEPARATOR, 0, "");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
		mInfo = CreateMenu();
		AppendMenu(mInfo, MF_STRING, 200, "&Autor...");
		mGlowne = CreateMenu();
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mPlik, "&Plik");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mInfo, "&Informacja");
		SetMenu(okno, mGlowne);
		DrawMenuBar(okno);

	case WM_COMMAND: //reakcje na wybór opcji z menu
		switch (wParam)
		{
		case 100: if(MessageBox(okno, "Zapiszczeć?", "Pisk", MB_YESNO) == IDYES)
					MessageBeep(0);
                  break;
		case 101: DestroyWindow(okno); //wysylamy meldunek WM_DESTROY
        		  break;
		case 200: MessageBox(okno, "Imię i nazwisko:\nNumer indeksu: ", "Autor", MB_OK);
		}
		return 0;
	
	case WM_LBUTTONDOWN: //reakcja na lewy przycisk myszki
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			

			for (int i = 0; i < 13; i++)
			{
				if (x > heart[i].x - 3 && x < heart[i].x + 3 && y > heart[i].y - 3 && y < heart[i].y + 3)
				{
					vertex = i;
					break;
				}
			}
			InvalidateRect(okno, NULL, true);

			return 0;
		}
	case WM_LBUTTONUP: 
	{
		vertex = -1;
		InvalidateRect(okno, NULL, true);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (vertex != -1 && vertex != 0 && vertex != 12)
		{
			heart[vertex].x = x;
			heart[vertex].y = y;
		}
		else if (vertex == 0 || vertex == 12)
		{
			heart[0].x = x;
			heart[0].y = y;
			heart[12].x = x;
			heart[12].y = y;
		}

		InvalidateRect(okno, NULL, true);
		return 0;
	}

	case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HDC kontekst;

			kontekst = BeginPaint(okno, &paint);

			// MIEJSCE NA KOD GDI

			HBRUSH pedzle[6] = { CreateSolidBrush(RGB(0, 0, 255)), CreateSolidBrush(RGB(0, 255, 255)), 
				CreateSolidBrush(RGB(255, 0, 255)), CreateSolidBrush(RGB(255, 0, 0)), 
				CreateSolidBrush(RGB(255, 100, 150)), CreateSolidBrush(RGB(125, 125, 255)) };


			SelectObject(kontekst, pedzle[0]);
			Pie(kontekst, 200, 200, 400, 400, 300, 200, 200, 300);
			DeleteObject(pedzle[0]);

			SelectObject(kontekst, pedzle[1]);
			Pie(kontekst, 200, 200, 400, 400, 200, 300, 300, 400);
			DeleteObject(pedzle[1]);

			SelectObject(kontekst, pedzle[2]);
			Pie(kontekst, 200, 200, 400, 400, 300, 400, 400, 300);
			DeleteObject(pedzle[2]);

			SelectObject(kontekst, pedzle[3]);
			Pie(kontekst, 200, 200, 400, 400, 400, 300, 300, 200);
			DeleteObject(pedzle[3]);


			POINT first[8], second[4];
			createLetter(first, second);
			setLetter(first, second, 250, 220, 20, 20);

			SelectObject(kontekst, pedzle[4]);
			Polygon(kontekst, first, 8);
			DeleteObject(pedzle[4]);

			SelectObject(kontekst, pedzle[5]);
			Polygon(kontekst, second, 4);
			DeleteObject(pedzle[5]);

			HBRUSH kolor = CreateSolidBrush(RGB(0, 0, 0));

			SelectObject(kontekst, kolor);
			PolyBezier(kontekst, heart, 13);
			for (int i = 0; i < 14; i++) {
				Ellipse(kontekst, heart[i].x - 3, heart[i].y - 3, heart[i].x + 3, heart[i].y + 3);
			}
			DeleteObject(kolor);
		
			// utworzenie obiektu umożliwiającego rysowanie przy użyciu GDI+
			// (od tego momentu nie można używać funkcji GDI
			Graphics grafika(kontekst);
			
			// MIEJSCE NA KOD GDI+


			// utworzenie czcionki i wypisanie tekstu na ekranie
			FontFamily  fontFamily(L"Times New Roman");
			Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
			PointF      pointF(100.0f, 400.0f);
			SolidBrush  solidBrush(Color(255, 0, 0, 255));

			grafika.DrawString(L"To jest tekst napisany za pomocą GDI+.", -1, &font, pointF, &solidBrush);

			EndPaint(okno, &paint);

			return 0;
		}
  	
	case WM_DESTROY: //obowiązkowa obsługa meldunku o zamknięciu okna
		PostQuitMessage (0) ;
		return 0;
    
	default: //standardowa obsługa pozostałych meldunków
		return DefWindowProc(okno, kod_meldunku, wParam, lParam);
	}
}
