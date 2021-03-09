#ifndef mumja_h
#define mumja_h
#include <stdio.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <list>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <concurrent_vector.h>

using namespace std;
using namespace concurrency;
class PasekZdrowia
{
	ALLEGRO_BITMAP* raz;
	ALLEGRO_BITMAP* dwa;
	ALLEGRO_BITMAP* trzy;
public:
	PasekZdrowia();
	PasekZdrowia(string hp1, string hp2,string hp3);
	void rysuj(int x, int y, float progres, float wielkosc);
	int getWidth();
	int getHeight();
};
enum TypObiektu
{
	unDef=-1,
	gracz=0,
	przeszkoda=1,
	przeciwnik=2,
	pocisk=3,
	endLevelItem=4,
	pociskPoProstej=5
};
enum TypPerka
{
	brak=-1,
	szybkosc=0,
	zasieg=1,
	hp=2,
	obrazenia=3
};
enum SiType//sprawdzic czy sie zgadza
{
	siGracz=0,
	siObiektStatyczny=1,
	siPrzeciwnik=2,
	siPocisk=3,
	siPrzeciwnikWieza=4
};
enum Kierunek
{
	zaden=0,
	lewo=1,
	prawo=2,
	gora=3,
	dol=4
};
struct TargetType
{
	bool gracz;
	bool przeciwnik;
	bool statyczny;
	TargetType()
	{
		gracz=false;
		przeciwnik=false;
		statyczny=false;
	}
};
struct Punkt
{
	int x;
	int y;
	Punkt();
	Punkt(int _x, int _y);
	bool operator ==(Punkt p)
	{
		if (p.x == x && p.y==y) return true;
		else return false;
	}
	bool operator !=(Punkt p)
	{
		if (p.x == x && p.y==y) return false;
		else return true;
	}
};
struct Wektor//wartosci x i y (rzuty na osi odpowiadajace wektorowi)
{
	float x;
	float y;
	Wektor();
	Wektor(float _x, float _y);
};
class Graf
{
private:
	struct Docelowy
	{
		int gdzie;
		float wartosc;
		Docelowy();
		Docelowy(int _gdzie, float _wartosc);
	};
	struct Wezel
	{
		float g;//koszt start-aktualny
		float h;//koszt (szacowany) aktualny-koniec
		float f;//g+h
		int indeks;//numer wierzcholka ktory ejst reprezentowany przez wezel
		int ojciec;//potrzebne aby odtworzyc droge
		Wezel();
		Wezel(int _h, int _g, int _indeks);
		Wezel(int _h, int _g, int _indeks, int _ojciec);
	};
	struct PorownajWezel
	{
		bool operator () (const Wezel &w1, const Wezel &w2)
		{
			if (w1.f < w2.f) return false;
			return true;
		}
	};
	std::vector <Punkt> wierzcholki;
	std::vector <std::vector <Docelowy>> krawedzie;
	Wezel stworzWezel(int _indeks, Punkt punktWezla, Punkt docelowy, float drogaAktualna, int _rodzic);//zwraca obiekt typu wezel utworzony na podstawie wierzcholka o zadanym indeksie
	float liczH(Punkt a, Punkt b);//obliczenie heurystyki dla A*
	Punkt getPunkt(int indeks);//zwraca wspolzedne punktu z zadanego indeksu
	bool aSistnieje(int indeks, list<Wezel> &lista);//zwraca true jezeli istnieje wezel o indeksie w liscie
	void Graf::push(list <Graf::Wezel>& l, Graf::Wezel w);
public:
	void algorytmAS(Punkt poczatek, Punkt koniec, list<Punkt> &lista);//algorytm A*
	Graf();
	int krawedzIstnieje(Punkt a, Punkt b);//jezeli istnieje krawedz z a do b to zwraca jej dlugosc. Jak nie istnieje to -1;
	void dodaj(int _x, int _y);//dodaje wierzcholek
	void usun(int _x, int _y);//usuwa wierzcholek
	void clear();//usuwa wierzcholki i krawedzie
	void clearKrawedzie();//usuwa krawedzie
	void dodajKrawedz(Punkt _x, Punkt _y);//dodaje krawedz
	void rysuj();
	void initKrawedzie();//inicjuje liste "krawedzie" na podstawie aktualnych wierzcholkow (nie zapelnia jej wartosciami)
	Punkt operator [](int indeks)
	{
		if (indeks < 0 || indeks >= iloscWierzcholkow())
		{
			return Punkt(-1, -1);
		}
		else
		{
			std::vector<Punkt>::iterator it = wierzcholki.begin();
			std::advance(it, indeks);
			return Punkt(it->x, it->y);
		}
	}
	int iloscWierzcholkow();
	int iloscKrawedzi();
	int punktIstnieje(Punkt p);//zwraca indeks jezeli punkt jest wierzcholkiem a -1 gdy nie
	void usunWierzcholek(Punkt p);
	bool wierzcholekIstnieje(Punkt a);
};
class Mapa
{
public:
	struct Pole//pole na mapie x/y
	{
		bool wolne;//nic na nim nie ma
		bool istnieje;//jezeli mapa nie jest prostokatem to pole moze nie istniec
		bool stale;//czy mozna zmieniac jego stan
		Pole(bool _wolne, bool _istnieje);
		Pole();
	};
	struct Wierzcholek//wierzcholek obiektu do latwego przeksztalecenia na wierzcholek grafu
	{
		int x;
		int y;
		bool obiektWPrawo;//wierzcholek ma obiekt po swojej prawej stronie 
		bool obiektWLewo;//wierzcholek ma obiekt po swojej lewej stronie 
		bool obiektWGore;//wierzcholek ma obiekt nad soba
		bool obiektWDol;//wierzcholek ma obiekt pod soba
		Wierzcholek();
		Wierzcholek(int _x, int _y);
		Wierzcholek(int _x, int _y, bool _lewo, bool _prawo, bool _gora, bool _dol);
	};
	class Obiekt
	{
		int pozycjaX;
		int pozycjaY;
	public:
		Obiekt();
		Obiekt(int _x, int _y);
		int getPozycjaX();
		int getPozycjaY();
	};
	struct PunktWartosc//Punkt z informacja o koszcie dojscia do niego
	{
		Punkt p;
		int wartosc;
		PunktWartosc();
		PunktWartosc(Punkt _p, int w);
	};
	struct Wezel
	{
		float g;//koszt start-aktualny
		float h;//koszt (szacowany) aktualny-koniec
		float f;//g+h
		Punkt indeks;//mylaca nazwa; chodzi o zidentyfikowanie (po prostu jest to punkt)
		Punkt ojciec;//potrzebne aby odtworzyc droge
		Wezel();
		Wezel(int _h, int _g, Punkt _indeks);
		Wezel(int _h, int _g, Punkt _indeks, Punkt _ojciec);
	};
private:
	bool isSet;//gdyby ktos uzyl konstruktora domyslnego to obiekt nie dziala
	int rozmiarX;
	int rozmiarY;
	Pole** mapa;
	//wywalic
	bool dodatkowyBorder;//mapa na krawedziach ma dodatkowy niedostepny teren
	//wywalic
	void prostokatWolne(int _xPocz, int _yPocz, int _xKon, int _yKon, bool _pelny, bool _wolny);//ustawia parametr "wolny" na prostokacie, oraz jego wnetrzu gdy _pelny = true;
	//na luj to???
	void usun();// usuwa Pole** mapa
	list <Obiekt> listaObiektow;
	Graf grafStatyczny;//wierzcholki opisujace mozliwe drogi oparte na obiektach, ktore z reguly sie nie zmieniaja
	void stworzGrafStatyczny();//metoda niszczy stary graf statyczny i tworzy nowy, oparty na aktualnym stanie mapy
	void rysujGraf();
	bool jestDroga(Punkt a, Punkt b);//zwraca true jezeli punkty sie "widza" - nie ma na mapie miedzy nimi przeszkod
	float odleglosc (Punkt punktPocz, Punkt punktKon, Punkt punkt3);//zwraca odlegosc punkt3 od prostej opartej na punktPocz i punktKon
	bool pozaOdcinkiem(Punkt punktPocz, Punkt punktKon, Punkt punkt3);//zwwraca true, jezeli punkt3 nie znajduje sie na odcinku opartym na dwoch punktach pierwszych
	bool ASpunkt(int x, int y);//zwraca true, jezeli mozna "wejsc" na podany punkt (do alg a*)
	bool ASpunkt(Punkt a);//to samo
	float liczH(Punkt a, Punkt b);
	void push(list <Wezel>& l, Wezel w);
public:
	Mapa(int _rozmiarX, int _rozmiarY);//dodatkowy niedostepny teren na brzegach
	Mapa();
	~Mapa();
	void stworz(int _rozmiarX, int _rozmiarY);
	Pole getPole(int x, int y);//NIE UZYWAC NA NIEPOPRAWNYCH POLACH
	int getRozmiarX();
	int getRozmiarY();
	void dodajObiekt(Obiekt a);
	void przeliczKrawedzie();//od nowa tworzy wszystkie krawedzie na podstawie aktualnego grafu
	void przeliczWierzcholki();//od nowa tworzy wszystkie wierzcholki na podstawie aktualnego grafu
	void rysuj();
	bool poleWolne(int _x, int _y);//zwraca informacje czy na polu nic nie ma (i jest fizycznie dostepne)
	//void droga(list <Punkt> &lista, Punkt poczatek, Punkt koniec, int rozmiar);//droga z punktu x do y na podstawie aktualnej mapy
	void droga(Punkt poczatek, Punkt koniec, list<Punkt> &lista);//metoda na podstawie grafu oraz punktow pocz i kon dodaje do listy "lista" kolejno punkty poruszania (algorytm A*)
	void algorytmAS(Punkt poczatek, Punkt koniec, list<Punkt> &lista);//algorytm A*
	void reset();//kiedy mapa (gry) sie zmienia nalezy usunac wszystkie elementy starej mapy
	void przeliczMape();
};
class Element;
struct Akcja
{
	bool ruch;
	bool createPocisk;
	bool zniszcz;//obiekt ma byc zniszczony
	Element *cel;
	float ruchX;
	float ruchY;
	Akcja();
};
class IObiekt
{
protected:
	float predkoscBazowa;
	int wytrzymalosc;
	ALLEGRO_BITMAP* bitmapa;
	void ustawPrzezroczystosc();
	int szybkoscAtaku;
	int hp;
	int zasieg;
	TargetType typCelu;
public:
	float getPredkosc();
	void setBitmapa(ALLEGRO_BITMAP* _bitmapa);
	virtual void rysuj(int _x, int _y, float _skala, float kierunek, int klatka) = 0;
	float getWidth();
	float getHeight();
	void zniszcz();
	int getSzybkoscAtaku();
	int getMaxHp();
	int getZasieg();
	TargetType getTypCelu();
	void setTargetType(TargetType t);
};
/*class IPrzedmiot : public IObiekt
{

};*/
class IPostac: public IObiekt
{
protected:
public:
	virtual void zadajObrazenia(int _obrazenia);
};
class IPocisk: public IObiekt
{
protected:
	Element *cel;
public:
	TargetType typCelu;
	void setTargetType(TargetType typ);
	TargetType getTypCelu();
};
class Pocisk: public IPocisk
{
public:
	void rysuj(int _x, int _y, float _skala, float kierunek, int klatka);
	Pocisk();
	Pocisk(Element *_cel, TargetType typ);
	Pocisk(string _sciezka, TargetType typ);
	Pocisk(string _sciezka, bool alphaDisabled, TargetType typ);
	~Pocisk();
};
class Element
{
public:
	struct Stan
	{
		bool poruszaSie;//element porusza sie
		bool ghostPostacie;//mozliwosc przechodzenia przez przeciwnikow etc
		bool ghostPrzeszkody;//mozliwosc przechodzenia przez sciany etc
		bool attackMove;//attackMove on
		bool potrzebnePrzeliczenieDrogi;
		bool drogaZostalaPrzeliczona;
		Stan();
	};
private:
	Stan stan;
	float pozycjaX;//pozycja
	float pozycjaY;
	Kierunek kierunek;//kierunek w ktory jest obrocony obiekt
	IObiekt *obiekt;
	TypObiektu typ;//0-gracz, 1-obiekt Statyczny, 2 przeciwnik
	int siType;//0-gracz 1-przeciwnik
	bool drogaUpdate;//(potencjalne) aktualizowanie drogi co petle jest bez sensu, wiec aby ustalic nowa trase ta zmienna musi byc takze true;
	int licznikOdOstatniegoUpdate;//ilosc cykli od ostatniego wyznaczenia drogi
	float skala;
	float srodekX;
	float srodekY;
	void obliczSrodek();//pakuje do srodekX/srodekY aktualny srodek bitmapy
	void przeliczPredkosc(); //po kazdej zmianie predkosci nalezy uruchomic ta funkcje aby zaktualizowala zmienna "predkoscTickowa"
	int aktualnyTick;
	int predkoscTickowa;//co ile tickow wykonuje swoj ruch (po uwzglednieniu predkosci obiektu)
	int iloscTickowDefault;//predkosc domyslna
	list <Punkt> droga;//lista punktow na podstawie ktorej obiekt ma sie poruszac
	Wektor drogaDocelowy;//wspolzedne konca drogi ("ostatni punkt" na liscie "droga")
	float odleglosc (Wektor a , Wektor b);//zwraca odleglosc pomiedzy punktami
	Element *cel;
	bool celSet;//zwraca true, jezeli *cel zostal ustawiony
	int ostatniAtak;//ilosc cykli od ostatniego ataku
	int id;
	int hpAktualne;
	Akcja akcja;//aktualna akcja obiektu (potrzebna dla drugiego watku)
	bool needDrogaUpdate;//jezeli jest true, to nalezy ponownie obliczyc droge
	int buffZasieg;
	int buffObrazenia;
	int buffSzybkoscAtaku;
	int buffHP;
	bool rotacja;//jezeli true, to obiekt obraca siê w kierunku przemieszczania
	int licznikAnimacji;
	int cykleNaZmianeAnimacji;
public:
	ALLEGRO_MUTEX *mutex;

	Element(int x, int y);
	Element(IObiekt* _obiekt);
	Element(IObiekt* _obiekt, float _pozX, float _pozY, TypObiektu _typ, int _siType, int _tickNaRuch, int _id);
	Element(IObiekt* _obiekt, Element *_cel, float _pozX, float _pozY, TypObiektu _typ, int _siType, int _tickNaRuch, int _id);
	~Element();
	void rysuj();
	void setPozycjaX(float _pozycjaX);
	void setPozycjaY(float _pozycjaY);
	void zmienPozycjaX(float _wartosc);//dodaje do aktualnej pozycji _wartosc
	void zmienPozycjaY(float _wartosc);
	void setSkala(float _skala);
	int getTyp();
	float getPozycjaX();
	float getPozycjaY();
	Punkt getPozycja();
	int getSiType();
	float getSkala();
	float getRozmiarX();
	float getRozmiarY();
	float getRozmiar();//zwraca promien okregu opartego na sredniej z rozmiaru x i y
	float getOdlegloscOdSrodkaObiektu(float _x, float _y);//zwraca odleglosc punktu od srodka obiektu
	float getPromien();//zwraca promien okregu opartego na sprit'cie obiektu (czyli wielkosc)
	bool getRuch();//zwraca czy element jest w ruchu
	float getPredkosc();
	bool czasNaRuch();//
	Wektor getDroga(Punkt pozycja);//zwraca miejsce do ktorego ma udac sie element. Jako parametry pozycja na mapie grafu (13x7)
	bool setDocelowy(float _x, float _y);//ustawia punkt docelowy, do ktorego obiekt ma zmierzac. Zwraca true jezeli punkt zostal zmieniony a false jezeli pozostal taki sam
	void setListaDroga(list<Punkt> lista);//tworzy droge
	bool punktFinalny();//zwraca informacje czy metoda getDroga podaje ostateczny punkt (czy lista "droga" jest pusta)
	void setGhostPostacie(bool _true);
	void setGhostPrzeszkody(bool _true);
	void setPoruszaSie(bool _true);
	void setPotrzebnePrzeliczenieDrogi(bool _true);
	void setDrogaZostalaPrzeliczona(bool _true);
	Stan getStan();//zwraca obiekt stan
	void zwiekszLicznikOdOstantiegoUpdate();//inkrementuje zmienna licznikOdOstatniegoUpdate w celu sledzenia ile cykli temu byl update drogi
	int getLicznikOdOstatniegoUpdate();
	void zerujLicznikOdOstatniegoUpdate();
	bool punktZawieraSie(float x, float y);//zwraca true jezeli punkt znajduje sie na obiekcie (wykrywanie klikniecia)
	Element *getCel();
	int getSzybkoscAtaku();
	int getOstatniAtak();
	int getObrazenia();
	bool moznaAtakowac();
	void atak();
	void wyczyscDroga();
	void licznik();//zwieksza liczniki
	void setDrogaDocelowy(float x, float y);
	int getId();
	int getHpMax();
	int getHpAktualne();
	void onHit();//metoda wywolywana jezeli cos zaatakuje Element (moment zderzenia pocisku)
	void onHitCel();//wywolanie onHit na celu (*cel)
	bool onDelete();//kiedy obiekt ma byc usuniety moze byc np wyswietlana animacja (czyli nie jest usuwany od razu i dopoki cos sie dzieje to metoda zwraca false). Kiedy metoda zwroci true obiekt zostanie usuniety
	Wektor getDrogaDocelowy();//zwraca finalny punkt drogi
	void setAttackMove(bool _true);//ustawia akcja.attackMove na podana wartosc
	bool celIsSet();//zwraca celSet
	void setCelSet(bool _true);
	void setCel(Element* _cel);//ustawia *cel na _cel
	int getZasieg();
	void setAkcja(Akcja a);
	Akcja getAkcja();
	bool getNeedDrogaUpdate();//zwraca wartosc zmiennej
	void drogaUpdated();//ustawiwa flage, ze droga odnowiona
	void addBuffZasieg(int ile);
	void addBuffObrazenia(int ile);
	void addBuffSzybkoscAtaku(int ile);
	void addBuffHP(int ile);
	TargetType getTargetType();
	void setTargetType(TargetType t);
	void dotkniety(TypObiektu t);//kiedy dwa obiekty sie dotkna (np zadanie obrazen)
	void setKierunek(Kierunek k);
	Kierunek getKierunek();
	void setRozmiar(int pix);//ustala skale obiektu aby rozmiar odpowiadal ilosci pikseli
	void setRotacja(bool b);
	bool getRotacja();
	void restartHP();//odnawia cale zycie
	//void poruszony();//wywolywane kiedy obiekt bedzie sie poruszal
};
class Protagonista : public IPostac
{
public:
	Protagonista();
	Protagonista(string _sciezka);
	Protagonista(string _sciezka, bool alphaDisabled);//alphaDisabled - jezeli piksele nie posiadaja kanalu alfa to nastapi ich stworzenie na kolorze (255,0,255)
	void rysuj(int _x, int _y, float _skala, float kierunek, int klatka);
	~Protagonista();
};
class Przeciwnik : public IPostac
{
public:
	Przeciwnik();
	Przeciwnik(string _sciezka);
	Przeciwnik(string _sciezka, bool alphaDisabled);
	void rysuj(int _x, int _y, float _skala, float kierunek, int klatka);
	~Przeciwnik();
};
class PrzeciwnikDzialko : public IPostac
{
public:
	PrzeciwnikDzialko();
	PrzeciwnikDzialko(string _sciezka);
	PrzeciwnikDzialko(string _sciezka, bool alphaDisabled);
	void rysuj(int _x, int _y, float _skala, float kierunek, int klatka);
	~PrzeciwnikDzialko();
};
class Przeszkoda : public IObiekt
{
public:
	Przeszkoda(string _sciezka);
	Przeszkoda();
	Przeszkoda(string _sciezka, bool alphaDisabled);
	void rysuj(int _x, int _y, float _skala, float kierunek, int klatka);
	~Przeszkoda();
};
class Silnik
{
public:
	struct argumentWatek
	{
		concurrent_vector <Element> &lista;
		ALLEGRO_MUTEX *&mut;
		Mapa &mapa;
		int &bohaterPozycjaDocelowaX;
		int &bohaterPozycjaDocelowaY;
		bool &koniec;
		argumentWatek(concurrent_vector<Element> &l, ALLEGRO_MUTEX *&m, Mapa &ma, int &x, int &y, bool &b) : lista(l), mut(m), mapa(ma), bohaterPozycjaDocelowaX(x), bohaterPozycjaDocelowaY(y), koniec(b){}
	};
	/*struct Mutexy
	{
		int id;
		ALLEGRO_MUTEX *mutex;
		ALLEGRO_COND *cond;
		Mutexy() : mutex(al_create_mutex()),
				cond(al_create_cond()),
				id(-1){};
		~Mutexy() {
			al_destroy_mutex(mutex);
			al_destroy_cond(cond);
		}
	};*/
private:
	struct Perk
	{
		int pozycjaX;
		int pozycjaY;
		TypPerka typ;
		bool remove;//jak true to usunac
		Perk();
		Perk(int x, int y, TypPerka t);
	};
	int nonie;
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_BITMAP *mysz;
	ALLEGRO_BITMAP *perkRed;
	ALLEGRO_BITMAP *perkBlue;
	ALLEGRO_BITMAP *perkYellow;
	ALLEGRO_BITMAP *perkGreen;
	ALLEGRO_BITMAP *tlo;
	float FPS;
	float iloscTickow;//wewnetrzny zegar gry (max ilosc)
	float aktualnyTick;
	float tickowNaRuch;//przy bazowej predkosci; co ile tickow obiekt wykonuje ruch
	int SCREEN_W;
	int SCREEN_H;
	ALLEGRO_FONT *font;
	ALLEGRO_EVENT ev;
	int mapaWielkoscX;//obiektow statycznych (skaly etc) nie mozna umieszczac dowolnie, a tyko na mapie okreslanej przez te zmienne
	int mapaWielkoscY;
	int currentElementObliczenieDrogi;//id obiektu dla ktorego w tym cyklu zostanie przeliczona droga
	bool endLevelItem;//flaga czy na planszy znajduje sie przedmiot pozwalajacy przejsc na nastepna
	PasekZdrowia pasek;
	int dlugoscGryMax;
	int dlugoscGryAktualna;
	bool przegrana;
	bool wygrana;

	bool debugMode;
	bool zmianaMapy;//ustawia sie na true jezeli ma byc wylosowana nowa plansza
	Mapa mapa;
	list <Element> listaElementow;
	//ALLEGRO_MUTEX *mutexLista;
	list <Element> listaPociskow;
	list <Perk> listaPerkow;
	//list <Mutexy> mutexy;
	list <int> doUsuniecia;//indeksy objektow ktore maja byc usuniete (podczas wykrycia potrzeby usuniecia wykonywana jest operacja w petli na liscie, wiec nie wskazane jest jej modyfikowanie)
	ALLEGRO_MOUSE_STATE mouseState;
	ALLEGRO_KEYBOARD_STATE keyboardState;
	float skalaGlobalna;
	Protagonista protagonista;
	Przeciwnik przeciwnik;
	Przeciwnik przeciwnikWieza;
	Przeciwnik przeciwnikWidmo;
	Przeszkoda przeszkoda;
	int idCounter;//kazdy element dostaje id
	bool koniecGry;//sygnal dla dodatkowego watku, aby sie skonczyl 

	int bohaterPozycjaDocelowaX;
	int bohaterPozycjaDocelowaY;
	ALLEGRO_MOUSE_STATE myszKopia;
	bool klikniecie;

	Akcja siDefault(Element &_element, bool obliczenieDrogi);
	Akcja SiGracz(Element &_element, bool obliczenieDrogi);
	Akcja siMobekMeele(Element &_element, bool obliczenieDrogi);
	Akcja siMobekWieza(Element &_element, bool obliczenieDrogi);
	Akcja siPocisk(Element &_element, bool obliczenieDrogi);//auto namierzanie celu
	Akcja siPociskPoProstej(Element &_element, bool obliczenieDrogi);
	Akcja kolizja(Akcja _akcja);//funkcja sprawdza czy miejsce docelowe jest dostepne i w razie potrzeby probuje zmienic trase
	void wyswietlDebug();
	void skaluj (float _skala);//zmienia rozmiar obiektow
	void korygujPozycje(Element* e);//sprawdza czy pozycja elementu miesci sie w oknie gry
	bool porusz(Element &e, Akcja a, bool rekurencja);//metoda porusza obiektem jezeli jest to dopuszczalne. rekurencja = true oznacza ze funkcja moze wywolac sama siebie (przydatne przy modyfikowaniu trasy w celu ominiecia przeszkody). zwraca true, jezeli ruch nastapi
	float odleglosc (Punkt a, Punkt b);//zwraca odleglosc pomiedzy punktami
	bool wektorPrzeciwny(Wektor w, Punkt pocz, Punkt kon);//funkcja zwraca true, jezeli wektory maja w (sporym) przyblizeniu przeciwne zwroty. Pozwala na przyklad na wychodzenie z siebie nastackowanych obiektow
	float dlugoscWektora(Wektor w);//zwraca dlugosc wektora
	void dodajPrzeszkoda(IObiekt &a, int x, int y);//dodaje obiekt jako przeszkode//pozycja na podstawie rozmiaru mapy (13x7) a nie ilosci pikseli
	void dodajEndLevelItem(int x, int y);
	void dodajPrzeciwnik(Punkt gdzie);
	void dodajPrzeciwnikWidmo(Punkt gdzie);
	void dodajPrzeciwnikWieza(Punkt gdzie);
	Punkt przeliczRozdzMapa(int _x, int _y);//mapa ma rozmiar x/y(13x7) a rozdzielczosc x2/y2(1920x1080). Metoda przelicza jednego na drugie. przelicza 13x7 -> 1080p
	Punkt przeliczRozdzMapa2(int _x, int _y);//w druga strone 1080p->13x7
	int init();//inicjuje potrzebne moduly allegro
	void deinit();//czysci bitmapy
	void stworzMape();
	void obliczDroga(Element &e);//wymusza obliczenie nowej trasy dla obiektu
	void dodajPocisk(Element &cel, float poczatekX, float poczatekY, TargetType typcelu);
	void dodajPociskPoProstej(int startX, int startY, int celX, int celY, TargetType typcelu);
	int getId();//zwraca pierwsze wolne id i inkrementuje licznik
	void clearId();//zeruje licznik id
	void usuwanieObiektow();//metoda analizuje liste doUsuniecia i usuwa obiekty
	void pushDoUsuniecia(int _id);
	Element* najblizszyPrzeciwnik(Punkt p);//zwraca najblizszego przeciwnika do punktu
	Element* attackMove(Punkt poczatek, Punkt dokad, int zasieg);
	friend void *Func_Thread(ALLEGRO_THREAD *thr, void *arg);
	void mapTypeBoss();
	void mapType1(int _ilosc);
	void mapType2(int _ilosc);
	void mapType3(int _ilosc);
	void mapType4(int _ilosc);
	void mapType5(int _ilosc);
	void mapType6(int _ilosc);
	void mapType7(int _ilosc);
	void nowaMapa();//tworzy nowa instancje planszy
	int losuj(int _min, int _max);//zwraca liczbe z zakresu
	bool endLevel();//zwraca true jezeli zostaly spelnione warunki do zmiany poziomu
	void osbslugaPerkow();//sprawdza czy jest interakcja z perkiem i ja obsluguje
	void dodajPerk(TypPerka t, int x, int y);
	void wyswietlPerki();
	void wyswietlGUI();
	void obliczKierunek(Element &e, float x, float y);
	void wyswietlWygrana();//metoda zawiera zakonczenie w przypadku sukcesu
	void wyswietlPrzegrana();//metoda zawiera zakonczenie w przypadku porazki
	void restart();//zaczyna gre od poczatku
public:
	Silnik();
	~Silnik();
	int gra();
	//void dodajElement(Element a);
	void ruch();//metoda wywoluje metode "porusz" dla wszystkich elementow oraz przekazuje do jej argumentu odpowiednie si
	void rysuj();
	Wektor obrotWektora(Wektor w, double radiany);//obraca wektor o kat zadany w radianach
	//void generuj();
};
#endif