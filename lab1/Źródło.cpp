
#include <math.h>
#include <stdio.h>

// Do³¹cz definicje biblioteki Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const float FPS = 60;		//obraz bêdzie aktualizowany co 1/FPS sekundy
const int SCREEN_W = 640;	//szerokoœæ okna
const int SCREEN_H = 480;	//wysokoœæ okna
const int BOKI_WIELOKATA = 5;

void init(float points[], float& xm, float& ym, int& R)
{
	float ffi = (ALLEGRO_PI * 2) / BOKI_WIELOKATA;
	xm = SCREEN_W / 2;
	ym = SCREEN_H / 2;
	R = 100;

	for (int i = 0; i < BOKI_WIELOKATA; i++)
	{
		double kat = ffi * i;
		points[2 * i] = R * cos(kat) + SCREEN_W / 2;
		points[2 * i + 1] = R * sin(kat) + SCREEN_H / 2;
	}
}

// Funkcja g³ówna
int main()
{
	ALLEGRO_DISPLAY* display = NULL;			//okno
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;	//kolejka zdarzen
	ALLEGRO_TIMER* timer = NULL;				//timer, od ktorego bêdziemy odbierac zdarzenia (potrzebny do animacji)
	bool redraw = true;
	float xm, ym;
	int R;
	float dx = 0, dy = 0;
	float skalowanie = 1;
	int R2 = 100;

	if (!al_init()) {							//inicjalizacja biblioteki Allegro
		fprintf(stderr, "Nie zainicjalizowano allegro!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);	//utworznie okna
	timer = al_create_timer(1.0 / FPS);					//utworzenie timera
	al_install_keyboard();								//inicjalizacja obs³ugi klawiatury
	event_queue = al_create_event_queue();				//utworzenie kolejki zdarzeñ

	al_init_primitives_addon();							//inicjalizacja obs³ugi prostych elementów (punkty, linie, prostok¹ty, elipsy itd.)

	//Rejestracja ¿róde³ zdarzeñ (okno, timer, klawiatura ...)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//Kolory rysowania
	ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

	//Definicja wielok¹ta
	const int N = BOKI_WIELOKATA;


	//Tablice na przetworzone wspó³rzêdna punktów
	float points[2 * N];
	init(points, xm, ym, R);

	//Uruchamiamy timer, który bêdzie z zadan¹ czêstotliwoœci¹ wysy³a³ zdarzenia
	al_start_timer(timer);

	//Pêtla g³ówna programu - obs³uga zdarzeñ.
	//Dzia³amy, dopóki u¿ytkownik nie wciœnie Esc.
	while (true)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {	//zdarzenie timera -> odœwie¿enie obrazu 
			redraw = true;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {	//zdarzenie klawiatury -> jeœli Esc to koñczymy
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				break;
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //zdarzenie zamkniêcia okna
			break;
		}

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
			al_clear_to_color(black); //czyszczenie okna na zadany kolor

			R2 += skalowanie;

			if (R2 > 150)
				skalowanie *= -1;
			if (R2 < 20)
				skalowanie *= -1;

			dx += ALLEGRO_PI * 2 * 0.001;
			dy += ALLEGRO_PI * 2 * 0.001;

			if (dx >= ALLEGRO_PI * 2)
				dx -= ALLEGRO_PI * 2;
			if (dy >= ALLEGRO_PI * 2)
				dy -= ALLEGRO_PI * 2;



			//Obrót figury
			for (int i = 0; i < N; i++)
			{
				float ffi = ALLEGRO_PI * 2 / BOKI_WIELOKATA;
				double kat = ffi * i;
				points[2 * i] = (100 + R2) * cos(kat + dx) + (xm - sin(dx) * 2 * (100 + R2));
				points[2 * i + 1] = (100 + R2) * sin(kat + dy) + (ym - cos(dy) * 2 * (100 + R2));
			}

			//Narysuj wype³niony wielokat
			al_draw_circle(xm, ym, 100, yellow, 1);
			al_draw_circle((xm - sin(dx) * 2 * (100 + R2)), (ym - cos(dy) * 2 * (100 + R2)), 100 + R2, yellow, 1);
			al_draw_polygon(points, N, ALLEGRO_LINE_JOIN_NONE, white, 1, 1);

			//Wyœwietl w oknie to, co narysowano w buforze
			al_flip_display();
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	return 0;
}
