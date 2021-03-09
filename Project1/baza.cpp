#include "baza.h" 
//concurrent_vector <Element> listaElementow;
//concurrent_vector <Element> lista;
//todo:
//zmienic w mapa::liczh na inta z floata (cialo metody)







//WAAAAAA¯¯¯¯¯¯NNNNNNNNNNEEEEEEEEEEEEE 
//mo¿e zadzia³a zamiast dekonstruktorów zwalniac pamiêæ z poziomu funkcji usuwaj¹cej 









//dodac getObrazenia do element i zrobic zeby za ich pomoca zadawac obrazenia
//rozmiar perkow z dupy

//siMobekWie¿a zasiêg w pikselach zamiast proporcjonalnie do rozmiaru mapy

//pytanie czy nie trzeba mutexow np na bohaterPozycjaX/Y (moga sie zmienic w trakcie liczenia drogi przez drugi watek)

//Punkt przeliczRozdzMapy(int _x, int _y, int SCREEN_W, int SCREEN_H, int mapaWielkoscX, int mapaWielkoscY) uogólniæ (generalnie syf z metodami statycznymi poprawiæ)

//problemy z dekonstruktorami mog¹ siê braæ st¹d, ¿e obiekty inicjowane sa przez referencje/wskaŸniki, wiêc usuwanie mo¿e nie dotyczyæ tego co myœlê, ¿e dotyczy

//silnik::porysz zwieksza e.licznik(). pytanie czy ten licznik ma byc zalezny od predkosci tickowej elementu (wtedy jest ok) czy tez nie(wtedy licznik musi byc zwiekszany niezaleznie od niej)
//usuwanie obiektow ????????????????????? Silnik::ruch dodaje na koncu do usuniecia, a Silnik::porusz tez sprawdza warunek i tez dodaje ?????????????????????????

//zasieg jest w pikselach (zmiana rozdzielczonsci zmienia zasieg)
//sprawdzic czy lista mutexow poprawnie sie usuwa/dodaje

//pociski maja liczona droge???? dziwnie leca na poczatku
//do A* trzeba przekazac wspolzedne mapy a nie pikseli jako parametry
//algorytm A* i poruszanie na podstawie jego drogi
//zmienic nazwy metod dodaj na dodajWierzcholek w Graf
//metody grafu (zwlaszcza krawedzi) sa nowe i moga koniec koncow nie dzialac
//void Mapa::droga wywalic drugie przeliczenie krawedzi (do debuga ok, a bez niego puste liczenie)


//DEKONSTRUKTORY
//aktualnie jest zamieszanie; dodawanie obiektow nie tworzy nowych obiektow (przeciwnik ?protagonista ?przeszkoda) a tylko korzysta z istniejacego obiektu przez referencje. W takim wypadku uzycie dekonstruktora usuwa pierwotna bitmape dla wszystkich elementow??? ogarnac to lol bo crashe przez to i smieszne rzeczy. 
//pocisk ma swoj normalny dekonstruktor


//optymalizacja:
//wywalic funkcje typu obrotWektora do osobnego pliku
//wierzcholki grafu dla mapy generowane bezposrednio dla kazdego obiektu przy tworzeniu, a nie na podstawie mapy "ktore pola sa zajete". rozroznianie obiektow bardzo upraszcza analize otoczenia obiektu
//Graf::algorytmAS nie przeszukiwac kolejki priorytetowej (nie jest to operacja domyslna), tylko utworzyc dodatkowa liste z samymi indeksami w celu szybkiej iteracji
//Silnik::usuwanieObiektow() moze pakowac na liste zwolnione indeksy, a metoda Silnik::getId przydzielac z niej

//bugi:
//element.setDocelowy = element.setDrogadocelowy naprawic
//po zniszczeniu obiektu na mapie przebudowac graf!
//jezeli algorytmAS wpadnie w nieskonczona petle to mozliwe ze indeks ojca = indeks wlasny. Odkomentowac porownanie ze indeks ojca nie moze byc rowny indeksowi wlasnemu
//Graf::algorytmAS przy tworzeniu niektore wezly mialy siebie za ojca (nieskonczona petla) pomoglo dodanie sprawdzenia ojciec!=aktualnyIndeks ale cos jest zbugowane, ze to powstaje
//porobic dekonstruktory
//krawedzie sa nadmiarowe (lezace na tej samej linii). obiekty przeszkadzajace brac nie tylko z mapy ale tez dodatkowo traktowac kazdy wierzcholek jako przeszkode
//Silnik::wektorPrzeciwny i jego zastosowanie (Silnik::porusz) jest z dupy ale dziala w sumie spoko LUL.  NAPRAWDE DOBRZE DZIALA EleGiggle

//wazne:
//Mapa::droga wywolanie przeliczKrawedzie() (na koncu metody) jest niepotrzebne i czasochlonne, jezeli nie chcemy wyswietlac grafu. Przy debug off wystarczy wywolac clearKrawedzie i initKrawedzie(nieaktualne?)

Punkt przeliczRozdzMapy(int _x, int _y, int SCREEN_W, int SCREEN_H, int mapaWielkoscX, int mapaWielkoscY)
{
	int rozdzX=SCREEN_W/mapaWielkoscX;
	int rozdzY=SCREEN_H/mapaWielkoscY;
	Punkt wynik;
	wynik.x=_x/rozdzX;
	wynik.y=_y/rozdzY;
	return wynik;
}

PasekZdrowia::PasekZdrowia()
{
	raz=NULL;
	dwa=NULL;
	trzy=NULL;
}
PasekZdrowia::PasekZdrowia(string hp1, string hp2,string hp3)
{
	raz = al_load_bitmap(hp1.c_str());
	dwa = al_load_bitmap(hp2.c_str());
	trzy = al_load_bitmap(hp3.c_str());
}
void PasekZdrowia::rysuj(int x, int y, float progres, float wielkosc)
{
	if (progres<0) progres=0;
	if (progres>1) progres=1;
	al_draw_scaled_bitmap(raz, 0,0,al_get_bitmap_width(raz), al_get_bitmap_height(raz), x,y,al_get_bitmap_width(raz)*wielkosc, al_get_bitmap_height(raz)*wielkosc, 0);
	al_draw_scaled_bitmap(dwa, 0,0,al_get_bitmap_width(dwa), al_get_bitmap_height(dwa), x,y,al_get_bitmap_width(dwa)*wielkosc*progres, al_get_bitmap_height(dwa)*wielkosc, 0);
	al_draw_scaled_bitmap(trzy, 0,0,al_get_bitmap_width(trzy), al_get_bitmap_height(trzy), x,y,al_get_bitmap_width(trzy)*wielkosc, al_get_bitmap_height(trzy)*wielkosc, 0);
}
int PasekZdrowia::getWidth()
{
	return al_get_bitmap_width(raz);
}
int PasekZdrowia::getHeight()
{
	return al_get_bitmap_height(raz);
}
Akcja::Akcja()
{
	ruch=false;
	createPocisk=false;
	zniszcz=false;
	cel=NULL;
	ruchX=0;
	ruchY=0;
}

Punkt::Punkt()
{
	x=-1;
	y=-1;
}
Punkt::Punkt(int _x, int _y)
{
	x=_x;
	y=_y;
}

Wektor::Wektor()
{
	x=0;
	y=0;
}
Wektor::Wektor(float _x, float _y)
{
	x=_x;
	y=_y;
}

Graf::Docelowy::Docelowy()
{
	gdzie=0;
	wartosc=0;
}
Graf::Docelowy::Docelowy(int _gdzie, float _wartosc)
{
	gdzie=_gdzie;
	wartosc=_wartosc;
}
Graf::Graf()
{

}
void Graf::dodaj(int _x, int _y)
{
	std::vector <Docelowy> temp;//tablica ktora zostanie dodana w przypadku dolaczenia nowego punktu
	temp.clear();
	bool istnieje=false;
	for (vector<Punkt>::iterator it=wierzcholki.begin(); it != wierzcholki.end(); ++it)
	{
		if (it->x == _x && it->y == _y)
		{
			istnieje=true;
			break;
		}
	}
	if (!istnieje)
	{
		krawedzie.push_back(temp);//stworzenie listy na krawedzie na kolejnym indeksie
		wierzcholki.push_back(Punkt(_x, _y));//dodanie wierzcholka do listy wierzcholkow
	}
}
//wyglada ok
//!usuwa tylko wierzcholki; lista z krawedziami jest wtedy bledna i trzeba ja przeliczyc
void Graf::usun(int _x, int _y)
{
	wierzcholki.erase(std::remove(wierzcholki.begin(), wierzcholki.end(), Punkt(_x, _y)), wierzcholki.end());
}
void Graf::clear()
{
	wierzcholki.clear();
	krawedzie.clear();
}
void Graf::clearKrawedzie()
{
	krawedzie.clear();
}
int Graf::iloscKrawedzi()
{
	int ilosc=0;
	for (int i=0;i<iloscWierzcholkow();i++)
	{
		ilosc += krawedzie[i].size();
	}
	return ilosc;
}
int Graf::punktIstnieje(Punkt p)
{
	int index=0;
	for (vector<Punkt>::iterator it=wierzcholki.begin(); it != wierzcholki.end(); ++it)
	{
		if (p.x== it->x && p.y == it->y)
		{
			return index;
		}
		index++;
	}
	return -1;
}
int Graf::krawedzIstnieje(Punkt a, Punkt b)
{
	int indexA = punktIstnieje(a);
	int indexB = punktIstnieje(b);
	int wynik=-1;//-1 jezeli krawedz nie istnieje. Zmieni sie na poprawna wartosc jezeli krawedz zostanie znaleziona
	for (vector<Docelowy>::iterator it=krawedzie[indexA].begin(); it != krawedzie[indexA].end();it++)
	{
		if (it->gdzie==indexB)
		{
			wynik=it->wartosc;
			break;
		}
	}
	return wynik;
}
void Graf::dodajKrawedz(Punkt _x, Punkt _y)
{
	int punktX=punktIstnieje(_x);
	if (punktX == -1) return;
	int punktY=punktIstnieje(_y);
	if (punktY == -1) return;
	if (krawedzIstnieje(_x, _y) == -1)//-1 znaczy ze nie ma takiej krawedzi
	{
		float odleglosc=sqrt((_y.x-_x.x)*(_y.x-_x.x)+(_y.y-_x.y)*(_y.y-_x.y));
		krawedzie[punktX].push_back(Docelowy(punktY, odleglosc));
		krawedzie[punktY].push_back(Docelowy(punktX, odleglosc));
	}	
}
//sprawdzic czy dziala poprawnie i nie krzaczy kolekcji
//ZAKTUALIZOWAC LISTE KRAWEDZI
void Graf::usunWierzcholek(Punkt p)
{
	for (vector<Punkt>::iterator it=wierzcholki.begin(); it != wierzcholki.end(); ++it)
	{
		if (it->x == p.x && it->y==p.y)
		{
			wierzcholki.erase(it);
			break;
		}
	}
}
//nalezy uogolnic dla wszystkich rozmiarow map i rozdzielczosci
void Graf::rysuj()
{
	int iterator=0;
	int rozX=640;
	int rozY=480;
	int mapaX=13;
	int mapaY=7;
	for (std::vector <std::vector <Docelowy>>::iterator it=krawedzie.begin(); it != krawedzie.end(); ++it)
	{
		for (std::vector <Docelowy>::iterator it=krawedzie[iterator].begin(); it != krawedzie[iterator].end(); ++it)
		{
			al_draw_line((wierzcholki[iterator].x*((float)640/13))+20, (wierzcholki[iterator].y*((float)480/7))+20,(wierzcholki[it->gdzie].x*((float)640/13))+20, (wierzcholki[it->gdzie].y*((float)480/7))+20, al_map_rgb(0,0,0), 1);
		}
		iterator++;
	}
	for (int i=0;i<mapaX;i++)
	{
		for (int j=0;j<mapaY;j++)
		{
			al_draw_filled_circle(((i)*((float)640/13))+20, (j*((float)480/7))+20, 10, al_map_rgba(255,0,0, 128));
		}
	}
	for (vector<Punkt>::iterator it=wierzcholki.begin(); it != wierzcholki.end(); ++it)
	{
		al_draw_filled_circle(((it->x)*((float)640/13))+20, (it->y*((float)480/7))+20, 10, al_map_rgba(0,255,0, 200));
	}
}
int Graf::iloscWierzcholkow()
{
	return wierzcholki.size();
}
//!nie sprawdzone ale nie zcrashowalo i dziala a jest wywolywane
void Graf::initKrawedzie()
{
	std::vector <Docelowy> temp;//obiekt listy krawedzi; puszniecie go na odpowiedni indeks odpowiadajacy wierzhcolkowi
	for (vector<Punkt>::iterator it=wierzcholki.begin(); it != wierzcholki.end(); ++it)//przerost formy nad trescia ale chodzi o ilosc wierzcholkow
	{
		krawedzie.push_back(temp);//akt puszniecia
	}
}
Graf::Wezel Graf::stworzWezel(int _indeks, Punkt punktWezla, Punkt docelowy, float drogaAktualna, int _rodzic)
{
	Wezel temp;
	if (_indeks <0 || _indeks > wierzcholki.size()) return temp;
	temp.g=drogaAktualna;
	temp.h=liczH(punktWezla, docelowy);
	temp.f=temp.g+temp.h;
	temp.indeks=_indeks;
	temp.ojciec=_rodzic;
}
//nie wiadomo czy dziala
bool Graf::aSistnieje(int indeks, list<Wezel> &lista)
{
	for (list<Wezel>::iterator it=lista.begin(); it != lista.end(); ++it)
	{
		if (it->indeks == indeks) return true;
	}
	return false;
}
void Graf::push(list <Graf::Wezel>& l, Graf::Wezel w)
{
	int wielkoscListy=l.size();
	list <Graf::Wezel>::iterator pozycja=l.begin();
	for (int i=0;i<wielkoscListy;i++)
	{
		if (w.f < pozycja->f)
		{
			break;
		}
		pozycja++;
	}
	l.insert(pozycja, w);
}
void Graf::algorytmAS(Punkt poczatek, Punkt koniec, list<Punkt> &lista)
{
	if (punktIstnieje(poczatek) == -1)
	{
		return;
	}
	bool bylNaClosed;//czy wierzcholek byl na liscie closed
	bool bylNaOpen;//czy wierzcholek byl na liscie open
	Wezel aktualny;//wezel n aktorym aktualnie pracuje algorytm
	Wezel aktualnyDrugi;//potrzebny przy iteracji
	list <Graf::Wezel> open;
	list <Graf::Wezel> closed;
	Wezel xd(liczH(poczatek,koniec), 0, punktIstnieje(poczatek),-1);
	push(open, xd);
	while (open.size() >0)
	{
		aktualny=open.front();
		open.pop_front();
		closed.push_front(aktualny);
		if (aktualny.indeks==punktIstnieje(koniec))//jezeli jest to wezel ktorego szukamy to konczymy algorytm
		{
			int aktualnyIndex=aktualny.indeks;
			int aktualnyOjciec=aktualny.ojciec;
			//list <Punkt> wynik;
			lista.clear();
			lista.push_front(getPunkt(aktualnyIndex));
			while(lista.front()!=poczatek)
			{
				for(std::list <Graf::Wezel>::iterator it=closed.begin();it!=closed.end();++it)
				{
					if (it->indeks==aktualnyOjciec) 
					//if (it->indeks==aktualnyOjciec && it->indeks != it->ojciec)//jakby byla nieskonczona petla
					{
						lista.push_front(getPunkt(it->indeks));
						aktualnyOjciec=it->ojciec;
						aktualnyIndex=it->indeks;
						break;
					}
				}
			}
			if (lista.size() >0) lista.pop_front();//w punkt poczatku i konca jest niepotrzebny w wyniku
			if (lista.size() >0) lista.pop_back();
			break;
		}
		for(std::vector <Docelowy>::iterator it=krawedzie[aktualny.indeks].begin();it!=krawedzie[aktualny.indeks].end();++it)
		{
			bylNaClosed=false;
			for (std::list<Graf::Wezel>::iterator iter=closed.begin();iter!=closed.end();++iter)//przeszukanie listy closed czy istnieje na niej ten wierzcholek
			{
				if (iter->indeks==it->gdzie)//jezeli istnieje to nic nie robimy
				{
					bylNaClosed=true;//ustawiamy flage
					break;
				}
			}
			if (!bylNaClosed)//jakby byl na closed to pomijamy go
			{
				bylNaOpen=false;
				for (std::list<Graf::Wezel>::iterator iter=open.begin();iter!=open.end();++iter)//przeszukanie listy open czy istnieje na niej ten wierzcholek
				{
					if (iter->indeks==it->gdzie)//jezeli istnieje
					{
						float nowaG=aktualny.g+it->wartosc;//nowa wartosc g
						if (nowaG<iter->g)//jezeli nowa wartosc g do tego punktu jest mniejsza niz dotychczasowa to akceptujemy ta droge zamiast tamtej
						{
							aktualnyDrugi.ojciec=aktualny.indeks;//stworzenie wezla do dodania
							aktualnyDrugi.g=nowaG;
							aktualnyDrugi.f=aktualnyDrugi.g+aktualnyDrugi.h;
							open.erase(iter);//usuniecie starego elementu
							push(open, aktualnyDrugi);//dodanie lepszego
						}
						bylNaOpen=true;//ustawiamy flage
						break;
					}
				}
				if (!bylNaOpen)
				{
					aktualnyDrugi.indeks=it->gdzie;
					aktualnyDrugi.ojciec=aktualny.indeks;
					aktualnyDrugi.g=aktualny.g+it->wartosc;
					aktualnyDrugi.h=liczH(getPunkt(aktualnyDrugi.indeks),koniec);//chyba dobrze
					aktualnyDrugi.f=aktualnyDrugi.g+aktualnyDrugi.h;
					push(open, aktualnyDrugi);
				}
			}
		}
	}
}
float Graf::liczH(Punkt a, Punkt b)
{
	//float wynik=abs(a.x-b.x)+abs(a.y-b.y);//wtf???
	float wynik=sqrt((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y));
	return wynik;
}
Graf::Wezel::Wezel()
{
	g=-1;
	h=-1;
	f=-1;
	indeks=-1;
	ojciec=-1;
}
Graf::Wezel::Wezel(int _h, int _g, int _indeks)
{
	g=_g;
	h=_h;
	f=g+h;
	indeks=_indeks;
	ojciec=-1;
}
Graf::Wezel::Wezel(int _h, int _g, int _indeks, int _ojciec)
{
	g=_g;
	h=_h;
	f=g+h;
	indeks=_indeks;
	ojciec=_ojciec;
}
Punkt Graf::getPunkt(int indeks)
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
bool Graf::wierzcholekIstnieje(Punkt a)
{
	for(std::vector <Punkt>::iterator it=wierzcholki.begin();it!=wierzcholki.end();++it)
	{
		if (it->x==a.x && it->y==a.y) return true;
	}
	return false;
}

Mapa::Wierzcholek::Wierzcholek(int _x, int _y, bool _lewo, bool _prawo, bool _gora, bool _dol)
{
	x=_x;
	y=_y;
	obiektWDol = _dol;
	obiektWGore = _gora;
	obiektWLewo = _lewo;
	obiektWPrawo = _prawo;
}
Mapa::Wierzcholek::Wierzcholek()
{
	x=0;
	y=0;
	obiektWDol = false;
	obiektWGore = false;
	obiektWLewo = false;
	obiektWPrawo = false;
}
Mapa::Wierzcholek::Wierzcholek(int _x, int _y)
{
	x=_x;
	y=_y;
	obiektWDol = false;
	obiektWGore = false;
	obiektWLewo = false;
	obiektWPrawo = false;
}
Mapa::Pole::Pole(bool _wolne, bool _istnieje)
{
	wolne=_wolne;
	istnieje=_istnieje;
	stale=false;
}
Mapa::Pole::Pole()
{
	wolne=true;
	istnieje=true;
	stale=false;
}

Mapa::PunktWartosc::PunktWartosc()
{
	p.x=-1;
	p.y=-1;
	wartosc=-1;
}
Mapa::PunktWartosc::PunktWartosc(Punkt _p, int w)
{
	p=_p;
	wartosc=w;
}
Mapa::Mapa(int _rozmiarX, int _rozmiarY)
{
	isSet=true;
	rozmiarX=_rozmiarX;
	rozmiarY=_rozmiarY;
	dodatkowyBorder=true;
	stworz(rozmiarX, rozmiarY);
}
Mapa::Mapa()
{
	isSet=false;
	dodatkowyBorder=true;
	mapa=NULL;
}
Mapa::~Mapa()
{
	usun();
}
void Mapa::prostokatWolne(int _xPocz, int _yPocz, int _xKon, int _yKon, bool _pelny, bool _wolny)
{
	if (_pelny)
	{
		for (int i=_xPocz;i<=_xKon;i++)
		{
			for (int j=_yPocz;j<=_yKon;j++)
			{
				if (!mapa[i][j].stale) mapa[i][j].wolne=_wolny;
			}
		}
	}
	else
	{
		for (int i=_xPocz;i<=_xKon;i++)
		{
			mapa[i][_yPocz].wolne=_wolny;
			mapa[i][_yKon].wolne=_wolny;
		}
		for (int i=_yPocz;i<=_yKon;i++)
		{
			mapa[_xPocz][i].wolne=_wolny;
			mapa[_xKon][i].wolne=_wolny;
		}
	}
}
Mapa::Pole Mapa::getPole(int x, int y)
{
	if (x>=0 && y >=0 && x < rozmiarX && y<rozmiarY) return mapa[x][y];
	else
	{
		Pole temp(false,false);
		temp.wolne=false;
		return temp;
	}
}
void Mapa::usun()
{
	isSet=false;
	if (mapa!= NULL)
	{
		for (int i=0; i<rozmiarX;i++)
		{
			delete[] mapa[i];
		}
		delete[] mapa;
	}
}
void Mapa::stworz(int _rozmiarX, int _rozmiarY)
{
	usun();
	rozmiarX=_rozmiarX;
	rozmiarY=_rozmiarY;
	mapa = new Pole* [rozmiarX];
	for (int i=0;i<rozmiarX;i++)
	{
		mapa[i]= new Pole [rozmiarY];
	}
	prostokatWolne(0,0, rozmiarX-1, rozmiarY-1, false, false);
	isSet=true;
	fprintf(stderr, "stworzono mape %d x %d\n", rozmiarX, rozmiarY);
	/*for (int i=0;i<rozmiarY;i++)
	{
		for (int j=0;j<rozmiarX;j++)
		{
			if (mapa[j][i].wolne) fprintf(stderr, "1");
			else fprintf(stderr, "0");
		}
		fprintf(stderr, "\n");
	}*/
}
int Mapa::getRozmiarX()
{
	if (isSet) return rozmiarX;
	else return -1;
}
int Mapa::getRozmiarY()
{
	if (isSet) return rozmiarY;
	else return -1;
}
float Mapa::odleglosc (Punkt punktPocz, Punkt punktKon, Punkt punkt3)
{
	int A=punktKon.y-punktPocz.y;//rownanie prostej jako Ax+By+C=0
	int B=punktPocz.x-punktKon.x;
	int C=punktPocz.y*punktKon.x-punktKon.y*punktPocz.x;
	float odleglosc = (A*punkt3.x + B*punkt3.y + C)/(sqrt(A*A + B*B));
	return abs(odleglosc);
}
//edytowane, chyba ok
void Mapa::przeliczKrawedzie()
{
	grafStatyczny.clearKrawedzie();//usuniecie krawedzi
	grafStatyczny.initKrawedzie();//stworzenie list na krawedzie
	for (int i=0;i<grafStatyczny.iloscWierzcholkow();i++)//dla kazdego wierzcholka
	{
		Punkt jeden = grafStatyczny[i];//wierzcholek a
		for (int j=i;j<grafStatyczny.iloscWierzcholkow();j++)//chyba ok
		//for (int j=0;j<grafStatyczny.iloscWierzcholkow();j++)//porownaj z pozostalymi wierzcholkami. j=i bo graf nie skierowany wiec drogi "wczesniejsze" istnieja
		{
			Punkt dwa = grafStatyczny[j];//wierzcholek b
			if (jestDroga(jeden,dwa))
			{
				grafStatyczny.dodajKrawedz(jeden, dwa);
			}
		}
	}
	//fprintf(stderr, "Liczba krawedzi: %d\n", grafStatyczny.iloscKrawedzi());
}
bool Mapa::pozaOdcinkiem(Punkt punktPocz, Punkt punktKon, Punkt punkt3)
{
	if (punktPocz.x<punktKon.x)//sprawdzenie ktory punkt jest "dalszy/blizszy"
	{
		if (punkt3.x>=punktPocz.x && punkt3.x <=punktKon.x)//jezeli rzuty x i y sa w przedzialach to punkt lezy pomiedzy punktami = drogi nie ma
		{
			if (punktPocz.y<punktKon.y)
			{
				if (punkt3.y>=punktPocz.y && punkt3.y <=punktKon.y)
				{
					return false;
				}
			}
			else
			{
				if (punkt3.y<=punktPocz.y && punkt3.y >=punktKon.y)
				{
					return false;
				}
			}
		}
	}
	else
	{
		if (punkt3.x<=punktPocz.x && punkt3.x >=punktKon.x)
		{
			if (punktPocz.y<punktKon.y)
			{
				if (punkt3.y>=punktPocz.y && punkt3.y <=punktKon.y)
				{
					return false;
				}
			}
			else
			{
				if (punkt3.y<=punktPocz.y && punkt3.y >=punktKon.y)
				{
					return false;
				}
			}
		}
	}
	return true;
}
bool Mapa::jestDroga(Punkt a, Punkt b)
{
	if (a == b) return false;
	for (int i=0;i<rozmiarX;i++)
	{
		for (int j=0;j<rozmiarY;j++)
		{
			if (!mapa[i][j].wolne)//jezeli pole zajete
			{
				if (odleglosc(a,b,Punkt(i,j))<1 && !pozaOdcinkiem(a,b,Punkt(i,j)))//dodatkowo jezeli pole lezy na prostej (moze byc przeszkoda) i miesci sie w odcinku
				{
					return false;
				}
			}
		}
	}
	Punkt temp;
	for (int i=0;i<grafStatyczny.iloscWierzcholkow();i++)
	{
		temp=grafStatyczny[i];
		if (temp != a && temp != b)
		{			
			if (odleglosc(a,b,temp)<0.1 && !pozaOdcinkiem(a,b,temp)) return false;
		}
	}
	return true;
}
//////////////
void Mapa::rysuj()
{
	//grafStatyczny.rysuj();
	int iterator=0;
	int rozX=640;
	int rozY=480;
	int mapaX=13;
	int mapaY=7;
	for (int i=0;i<13;i++)
	{
		for (int j=0;j<7;j++)
		{
			if (mapa[i][j].wolne) al_draw_filled_circle(((i)*((float)640/13))+20, (j*((float)480/7))+20, 10, al_map_rgba(0,0,0, 255));
		}
	}
	for (list<Obiekt>::iterator it=listaObiektow.begin(); it != listaObiektow.end(); ++it)
	{
		al_draw_filled_circle(((it->getPozycjaX())*((float)640/13))+20, (it->getPozycjaY()*((float)480/7))+20, 10, al_map_rgba(0,0,0, 255));
	}
}
//do wywalenia
bool Mapa::poleWolne(int _x, int _y)
{
	if (_x<0 || _y<0 || _x>=rozmiarX || _y>=rozmiarY) return false;
	if (mapa[_x][_y].istnieje && mapa[_x][_y].wolne) return true;
	else return false;
}
Mapa::Obiekt::Obiekt()
{
	pozycjaX=-1;
	pozycjaY=-1;
}
Mapa::Obiekt::Obiekt(int _x, int _y)
{
	pozycjaX=_x;
	pozycjaY=_y;
}
int Mapa::Obiekt::getPozycjaX()
{
	return pozycjaX;
}
int Mapa::Obiekt::getPozycjaY()
{
	return pozycjaY;
}
//graf jest aktualizowany jednorazowo po dodaniu wszystkich wierzcholkow, a nie przy kazdym pojedynczym dodaniu
void Mapa::dodajObiekt(Obiekt a)
{
	listaObiektow.push_back(a);
	//mapa[a.getPozycjaX()][a.getPozycjaY()].wolne=false;
}
//operacja na nowym grafie zamiast aktualnym moze byc ok
void Mapa::reset()
{
	Pole temp(true,true);
	temp.stale=false;
	listaObiektow.clear();
	for (int i=0;i<rozmiarX;i++)
	{
		for (int j=0;j<rozmiarY;j++)
		{
			mapa[i][j]=temp;
		}
	}
	//przeliczKrawedzie();
	//przeliczWierzcholki();
}
void Mapa::droga(Punkt poczatek, Punkt koniec, list<Punkt> &lista)
{	
	//Graf nowy=grafStatyczny;
	//Graf temp=grafStatyczny;
	//grafStatyczny.dodaj(poczatek.x, poczatek.y);
	//grafStatyczny.dodaj(koniec.x, koniec.y);
	//przeliczKrawedzie();//powinno byc ok po wywolaniu





	//grafStatyczny.algorytmAS(poczatek, koniec, lista);
	algorytmAS(poczatek, koniec, lista);
	//algorytm





	//grafStatyczny=temp;
	//grafStatyczny.usun(poczatek.x, poczatek.y);
	//grafStatyczny.usun(koniec.x, koniec.y);
	//przeliczKrawedzie();//aby graf sie wyswietlal to odkomentowac i zakomentowac dwie dolne. 
	//grafStatyczny.clearKrawedzie();//usuniecie krawedzi
	//grafStatyczny.initKrawedzie();//stworzenie list na krawedzie
}
void Mapa::stworzGrafStatyczny()
{
	grafStatyczny.clear();
	Punkt temp;
	for (list<Mapa::Obiekt>::iterator it=listaObiektow.begin(); it != listaObiektow.end(); ++it)
	{
		temp.x=it->getPozycjaX();
		temp.y=it->getPozycjaY();
		temp.x+=1;
		temp.y+=1;
		if (poleWolne(temp.x, temp.y)) grafStatyczny.dodaj(temp.x, temp.y);
		temp.x-=2;
		if (poleWolne(temp.x, temp.y)) grafStatyczny.dodaj(temp.x, temp.y);
		temp.y-=2;
		if (poleWolne(temp.x, temp.y)) grafStatyczny.dodaj(temp.x, temp.y);
		temp.x+=2;
		if (poleWolne(temp.x, temp.y)) grafStatyczny.dodaj(temp.x, temp.y);
	}
}
//nalezy uogolnic dla roznych wielkosci map i rozdzielczosci
void Mapa::rysujGraf()
{
	grafStatyczny.rysuj();
}
//zbugowane (crash przy wyjsciu z programu + duze obciazenie) (zaczelo dzialac???!!)
void Mapa::przeliczMape()
{
	for (int i=0;i<rozmiarX;i++)
	{
		for (int j=0;j<rozmiarY;j++)
		{
			if (mapa[i][j].istnieje && !mapa[i][j].stale) mapa[i][j].wolne=true;
		}
	}
	for (list<Obiekt>::iterator it=listaObiektow.begin(); it != listaObiektow.end();it++)
	{
		if (mapa[it->getPozycjaX()][it->getPozycjaY()].istnieje)
		{
			mapa[it->getPozycjaX()][it->getPozycjaY()].wolne=false;
		}
	}
}
//mapa jest czyszczona i nanoszone sa nowe obiekty, wiec jezeli ma byc cos wiecej na niej (obramowka?) to trzeba odtworzyc
//todo:w przypadku wiekszej mapy wyznaczyc kluczowe wierzcholki a nie wszystkie dostepne 
//zle dzialanie: przed usunieciem sprawdzic dodatkowo czy nie jest ostatnim punktem w drugiej plaszczyznie(w sensie dla igrekow jezeli badamy iksy i vice versa)
//chyba cos jeszcze (zobaczyc stan aktualny i z dupy usuniete punkty)
//wykomentowany kawalek z czyszczeniem
void Mapa::przeliczWierzcholki()
{
	przeliczMape();
	for (int i=0;i<rozmiarX;i++)
	{
		for (int j=0;j<rozmiarY;j++)
		{
			if (mapa[i][j].wolne) grafStatyczny.dodaj(i,j);
		}
	}
	fprintf(stderr, "Liczba wierzcholkow: %d\n", grafStatyczny.iloscWierzcholkow());
}
float Mapa::liczH(Punkt a, Punkt b)
{
	//float wynik=abs(a.x-b.x)+abs(a.y-b.y);//wtf???
	float wynik=sqrt((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y));
	return wynik;
}
Mapa::Wezel::Wezel()
{
	g=-1;
	h=-1;
	f=-1;
	indeks=Punkt(-1,-1);
	ojciec=Punkt(-1,-1);
}
Mapa::Wezel::Wezel(int _h, int _g, Punkt _indeks)
{
	g=_g;
	h=_h;
	f=g+h;
	indeks=_indeks;
	ojciec=Punkt(-1,-1);
}
Mapa::Wezel::Wezel(int _h, int _g, Punkt _indeks, Punkt _ojciec)
{
	g=_g;
	h=_h;
	f=g+h;
	indeks=_indeks;
	ojciec=_ojciec;
}
bool Mapa::ASpunkt(int x, int y)
{
	if (x < 0 || x >= rozmiarX || y<0 || y>=rozmiarY)
	{
		return false;
	}
	if (mapa[x][y].wolne == true) return true;
	else return false;
}
bool Mapa::ASpunkt(Punkt a)
{
	if (a.x < 0 || a.x >= rozmiarX || a.y<0 || a.y>=rozmiarY)
	{
		return false;
	}
	if (mapa[a.x][a.y].wolne == true) return true;
	else return false;
}
void Mapa::push(list <Wezel>& l, Wezel w)
{
	int wielkoscListy=l.size();
	list <Wezel>::iterator pozycja=l.begin();
	for (int i=0;i<wielkoscListy;i++)
	{
		if (w.f < pozycja->f)
		{
			break;
		}
		pozycja++;
	}
	l.insert(pozycja, w);
}
void Mapa::algorytmAS(Punkt poczatek, Punkt koniec, list<Punkt> &lista)
{
	if (ASpunkt(poczatek) ==false)//jak punkt poza mapa to koniec
	{
		return;
	}
	if (ASpunkt(koniec)==false) return;//jezeli nie mozna sie dostac do celu
	bool bylNaClosed;//czy wierzcholek byl na liscie closed
	bool bylNaOpen;//czy wierzcholek byl na liscie open
	Wezel aktualny;//wezel n aktorym aktualnie pracuje algorytm
	Wezel aktualnyDrugi;//potrzebny przy iteracji
	list <Wezel> open;
	list <Wezel> closed;
	vector <PunktWartosc> nowePunkty;//otoczenie dla aktualnego punktu
	Wezel xd(liczH(poczatek,koniec), 0, poczatek);
	push(open, xd);
	while (open.size() >0)
	{
		aktualny=open.front();
		open.pop_front();
		closed.push_front(aktualny);
		if (aktualny.indeks==koniec)//jezeli jest to wezel ktorego szukamy to konczymy algorytm
		{
			Punkt aktualnyIndex=aktualny.indeks;
			Punkt aktualnyOjciec=aktualny.ojciec;
			//list <Punkt> wynik;
			lista.clear();
			lista.push_front(aktualnyIndex);
			while(lista.front()!=poczatek)
			{
				for(auto it=closed.begin();it!=closed.end();++it)
				{
					if (it->indeks==aktualnyOjciec) 
					//if (it->indeks==aktualnyOjciec && it->indeks != it->ojciec)//jakby byla nieskonczona petla
					{
						lista.push_front(it->indeks);
						aktualnyOjciec=it->ojciec;
						aktualnyIndex=it->indeks;
						break;
					}
				}
			}
			if (lista.size() >0) lista.pop_front();//w punkt poczatku i konca jest niepotrzebny w wyniku
			if (lista.size() >0) lista.pop_back();
			break;
		}
		nowePunkty.clear();//jezeli pole jest wolne to dodajemy
		if (ASpunkt(aktualny.indeks.x, aktualny.indeks.y+1))	nowePunkty.push_back(PunktWartosc(Punkt(aktualny.indeks.x, aktualny.indeks.y+1), 10));//dodanie istniejacych sasiedztw (gdzie mozna sie poruszyc)
		if (ASpunkt(aktualny.indeks.x, aktualny.indeks.y-1))	nowePunkty.push_back(PunktWartosc(Punkt(aktualny.indeks.x, aktualny.indeks.y-1), 10));
		if (ASpunkt(aktualny.indeks.x+1, aktualny.indeks.y-1))
		{
			if (ASpunkt(aktualny.indeks.x+1, aktualny.indeks.y) && ASpunkt(aktualny.indeks.x, aktualny.indeks.y-1))//droge po skosie dopuszczamy jezeli pola nie-skosne sa takze wolne aby nie zacinac sie na krawedziach obiektow
			nowePunkty.push_back(PunktWartosc(Punkt(aktualny.indeks.x+1, aktualny.indeks.y-1), 15));
		}
		if (ASpunkt(aktualny.indeks.x+1, aktualny.indeks.y))	nowePunkty.push_back(PunktWartosc(Punkt(aktualny.indeks.x+1, aktualny.indeks.y), 10));
		if (ASpunkt(aktualny.indeks.x+1, aktualny.indeks.y+1))
		{
			if (ASpunkt(aktualny.indeks.x+1, aktualny.indeks.y) && ASpunkt(aktualny.indeks.x, aktualny.indeks.y+1))
			nowePunkty.push_back(PunktWartosc(Punkt(aktualny.indeks.x+1, aktualny.indeks.y+1), 15));
		}
		if (ASpunkt(aktualny.indeks.x-1, aktualny.indeks.y-1))
		{
			if (ASpunkt(aktualny.indeks.x-1, aktualny.indeks.y) && ASpunkt(aktualny.indeks.x, aktualny.indeks.y-1))
			nowePunkty.push_back(PunktWartosc(Punkt(aktualny.indeks.x-1, aktualny.indeks.y-1), 15));
		}
		if (ASpunkt(aktualny.indeks.x-1, aktualny.indeks.y))	nowePunkty.push_back(PunktWartosc(Punkt(aktualny.indeks.x-1, aktualny.indeks.y), 10));
		if (ASpunkt(aktualny.indeks.x-1, aktualny.indeks.y+1))
		{
			if(ASpunkt(aktualny.indeks.x-1, aktualny.indeks.y) && ASpunkt(aktualny.indeks.x, aktualny.indeks.y+1))
			nowePunkty.push_back(PunktWartosc(Punkt(aktualny.indeks.x-1, aktualny.indeks.y+1), 15));
		}
		for(auto it=nowePunkty.begin();it!=nowePunkty.end();++it)
		{
			bylNaClosed=false;
			for (auto iter=closed.begin();iter!=closed.end();++iter)//przeszukanie listy closed czy istnieje na niej ten wierzcholek
			{
				if (iter->indeks==it->p)//jezeli istnieje to nic nie robimy
				{
					bylNaClosed=true;//ustawiamy flage
					break;
				}
			}
			if (!bylNaClosed)//jakby byl na closed to pomijamy go
			{
				bylNaOpen=false;
				for (auto iter=open.begin();iter!=open.end();++iter)//przeszukanie listy open czy istnieje na niej ten wierzcholek
				{
					if (iter->indeks==it->p)//jezeli istnieje
					{
						float nowaG=aktualny.g+it->wartosc;//nowa wartosc g
						if (nowaG<iter->g)//jezeli nowa wartosc g do tego punktu jest mniejsza niz dotychczasowa to akceptujemy ta droge zamiast tamtej
						{
							aktualnyDrugi.ojciec=aktualny.indeks;//stworzenie wezla do dodania
							aktualnyDrugi.g=nowaG;
							aktualnyDrugi.f=aktualnyDrugi.g+aktualnyDrugi.h;
							open.erase(iter);//usuniecie starego elementu
							push(open, aktualnyDrugi);//dodanie lepszego
						}
						bylNaOpen=true;//ustawiamy flage
						break;
					}
				}
				if (!bylNaOpen)
				{
					aktualnyDrugi.indeks=it->p;
					aktualnyDrugi.ojciec=aktualny.indeks;
					aktualnyDrugi.g=aktualny.g+it->wartosc;
					aktualnyDrugi.h=liczH(aktualnyDrugi.indeks,koniec);//chyba dobrze
					aktualnyDrugi.f=aktualnyDrugi.g+aktualnyDrugi.h;
					push(open, aktualnyDrugi);
				}
			}
		}
	}
}

void IObiekt::setBitmapa(ALLEGRO_BITMAP* _bitmapa)
{
	bitmapa=_bitmapa;
}
void IObiekt::ustawPrzezroczystosc()
{
	al_convert_mask_to_alpha(bitmapa, al_map_rgb(255,0,255));
}
float IObiekt::getPredkosc()
{
	return predkoscBazowa;
}
float IObiekt::getWidth()
{
	return al_get_bitmap_width(bitmapa);
}
float IObiekt::getHeight()
{
	return al_get_bitmap_height(bitmapa);
}
void IObiekt::zniszcz()
{
	al_destroy_bitmap(bitmapa);
}
int IObiekt::getSzybkoscAtaku()
{
	return szybkoscAtaku;
}
int IObiekt::getMaxHp()
{
	return hp;
}
int IObiekt::getZasieg()
{
	return zasieg;
}
TargetType IObiekt::getTypCelu()
{
	return typCelu;
}
void IObiekt::setTargetType(TargetType t)
{
	typCelu=t;
}

Element::Stan::Stan()
{
	poruszaSie=false;
	ghostPostacie=false;
	ghostPrzeszkody=false;
	attackMove=false;
	potrzebnePrzeliczenieDrogi=false;
	drogaZostalaPrzeliczona=false;
}
void Element::dotkniety(TypObiektu t)
{
	if (t==TypObiektu::przeciwnik && typ==TypObiektu::gracz) onHit();
}
Element::Element(IObiekt*_obiekt)
{
	obiekt = _obiekt;
	pozycjaX=10;
	pozycjaY=10;
	typ=unDef;
	//mutex=al_create_mutex();
	buffZasieg=0;
	buffObrazenia=0;
	buffSzybkoscAtaku=0;
	buffHP=0;
	rotacja=true;
	licznikAnimacji=0;
	cykleNaZmianeAnimacji=50;
}
Element::Element(int x, int y)
{
	obiekt = NULL;
	pozycjaX=x;
	pozycjaY=y;
	typ=unDef;
	hpAktualne=1;
	//mutex=al_create_mutex();
	buffZasieg=0;
	buffObrazenia=0;
	buffSzybkoscAtaku=0;
	buffHP=0;
	typ=TypObiektu::unDef;
	rotacja=true;
	licznikAnimacji=0;
	cykleNaZmianeAnimacji=50;
}
Element::Element(IObiekt* _obiekt, float _pozX, float _pozY, TypObiektu _typ, int _siType, int _tickNaRuch, int _id)
{
	id=_id;
	ostatniAtak=0;
	obiekt = _obiekt;
	pozycjaX=_pozX;
	pozycjaY=_pozY;
	typ= _typ;
	siType=_siType;
	skala=1.0;
	obliczSrodek();
	stan.poruszaSie=false;
	iloscTickowDefault=_tickNaRuch;
	aktualnyTick=0;
	przeliczPredkosc();
	drogaUpdate=true;
	licznikOdOstatniegoUpdate=0;
	celSet=false;
	hpAktualne=obiekt->getMaxHp();
	//mutex=al_create_mutex();
	buffZasieg=0;
	buffObrazenia=0;
	buffSzybkoscAtaku=0;
	buffHP=0;
	rotacja=true;
	licznikAnimacji=0;
	cykleNaZmianeAnimacji=50;
}
Element::Element(IObiekt* _obiekt, Element *_cel, float _pozX, float _pozY, TypObiektu _typ, int _siType, int _tickNaRuch, int _id)
{
	id=_id;
	ostatniAtak=0;
	obiekt = _obiekt;
	pozycjaX=_pozX;
	pozycjaY=_pozY;
	typ= _typ;
	siType=_siType;
	skala=1.0;
	obliczSrodek();
	stan.poruszaSie=false;
	iloscTickowDefault=_tickNaRuch;
	aktualnyTick=0;
	przeliczPredkosc();
	drogaUpdate=true;
	licznikOdOstatniegoUpdate=0;
	cel=_cel;
	hpAktualne=obiekt->getMaxHp();
	celSet=false;
	//mutex=al_create_mutex();
	buffZasieg=0;
	buffObrazenia=0;
	buffSzybkoscAtaku=0;
	buffHP=0;
	rotacja=true;
	licznikAnimacji=0;
	cykleNaZmianeAnimacji=50;
}
Element::~Element()
{
	//al_unlock_mutex(mutex);
	//al_destroy_mutex(mutex);

}
void Element::rysuj()
{
	if (rotacja==true)
	{
		double angleInRadians = std::atan2(akcja.ruchX, akcja.ruchY);
		obiekt->rysuj(pozycjaX-srodekX, pozycjaY-srodekY, skala, angleInRadians, 0);
	}
	else
	{
		if (typ== TypObiektu::gracz)
		{
			if (licznikAnimacji < cykleNaZmianeAnimacji/2) obiekt->rysuj(pozycjaX-srodekX, pozycjaY-srodekY, skala, 0, 0);
			else obiekt->rysuj(pozycjaX-srodekX, pozycjaY-srodekY, skala, 0, 1);
		}
		else obiekt->rysuj(pozycjaX-srodekX, pozycjaY-srodekY, skala, 0, 0);
	}
	
}
int Element::getTyp()
{
	return typ;
}
float Element::getPozycjaX()
{
	return pozycjaX;
}
float Element::getPozycjaY()
{
	return pozycjaY;
}
int Element::getSiType()
{
	return siType;
}
void Element::setSkala(float _skala)
{
	skala=_skala;
	obliczSrodek();
}
float Element::getSkala()
{
	return skala;
}
float Element::getRozmiarX()
{
	return obiekt->getWidth()*skala;
}
float Element::getRozmiarY()
{
	return obiekt->getHeight()*skala;
}
void Element::obliczSrodek()
{
	srodekX=(obiekt->getWidth()*skala)/2;
	srodekY=(obiekt->getHeight()*skala)/2;
}
void Element::setPozycjaX(float _pozycjaX)
{
	pozycjaX=_pozycjaX;
}
void Element::setPozycjaY(float _pozycjaY)
{
	pozycjaY=_pozycjaY;
}
float Element::getOdlegloscOdSrodkaObiektu(float _x, float _y)
{
	return sqrt(abs(_x-pozycjaX)*abs(_x-pozycjaX)+abs(_y-pozycjaY)*abs(_y-pozycjaY));
}
float Element::getPromien()
{
	return (getRozmiarX()+getRozmiarY())/4;
}
bool Element::getRuch()
{
	return stan.poruszaSie;
}
float Element::getPredkosc()
{
	return obiekt->getPredkosc();
}
bool Element::czasNaRuch()
{
	if (aktualnyTick>=predkoscTickowa)
	{
		aktualnyTick=0;
		licznikAnimacji++;
		if (licznikAnimacji>=cykleNaZmianeAnimacji) licznikAnimacji=0;
		return true;
	}
	else
	{
		aktualnyTick++;
		return false;
	}
}
void Element::przeliczPredkosc()
{
	float wynik=iloscTickowDefault/obiekt->getPredkosc();
	predkoscTickowa=(int)wynik;
}
void Element::setPoruszaSie(bool _true)
{
	stan.poruszaSie=_true;
}
void Element::zmienPozycjaX(float _wartosc)
{
	pozycjaX+=_wartosc;
}
void Element::zmienPozycjaY(float _wartosc)
{
	pozycjaY+=_wartosc;
}
float Element::getRozmiar()
{
	return (getRozmiarX()+getRozmiarY())/4;//srednia+polowa
}
Wektor Element::getDroga(Punkt pozycja)
{
	Punkt temp;
	if (droga.size() > 0)//jezeli lista z droga nie jest pusta
	{
		temp=droga.front();
		if (odleglosc(Wektor(pozycja.x, pozycja.y), Wektor(temp.x, temp.y)) < 0.05)//sprawdzenie czy osiagnieto kolejny punkt drogi
		{
			droga.pop_front();//jezeli tak to usuwamy go z listy
			if (droga.size()==0) return drogaDocelowy;//jezeli nie istnieje kolejny punkt na liscie to docelowy znajduje sie w drogaDocelowy
			else
			{
				temp=droga.front();
				return Wektor(temp.x, temp.y);//jezeli istnieje to go zwracamy
			}
		}
		else
		{
			return Wektor(temp.x, temp.y);
		}
	}
	else return drogaDocelowy;
}
float Element::odleglosc (Wektor a , Wektor b)
{
	return sqrt((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y));
}
void Element::addBuffZasieg(int ile)
{
	buffZasieg+=ile;
}
void Element::addBuffObrazenia(int ile)
{
	buffObrazenia+=ile;
}
void Element::addBuffSzybkoscAtaku(int ile)
{
	buffSzybkoscAtaku+=ile;
}
void Element::addBuffHP(int ile)
{
	buffHP+=ile;
}
bool Element::setDocelowy(float _x, float _y)
{
	if (abs(drogaDocelowy.x-_x)<0.1 && abs(drogaDocelowy.y-_y)<0.1) return false;//jezeli punkty sa bardzo blisko siebie to nic nie zmieniamy
	else
	{
		drogaDocelowy.x=_x;//jezeli punkty sa oddalone to zmieniamy
		drogaDocelowy.y=_y;
		needDrogaUpdate=true;
		return true;
	}
}
void Element::setListaDroga(list<Punkt> lista)
{
	droga.clear();
	droga=lista;
}
bool Element::punktFinalny()
{
	if (droga.size()==0) return true;
	else return false;
}
void Element::setGhostPostacie(bool _true)
{
	stan.ghostPostacie=_true;
}
void Element::setGhostPrzeszkody(bool _true)
{
	stan.ghostPrzeszkody=_true;
}
Element::Stan Element::getStan()
{
	return stan;
}
void Element::zwiekszLicznikOdOstantiegoUpdate()
{
	if (licznikOdOstatniegoUpdate < INT_MAX) licznikOdOstatniegoUpdate++;//coaby potencjalnie sie integer nie przekrecil
}
int Element::getLicznikOdOstatniegoUpdate()
{
	return licznikOdOstatniegoUpdate;
}
void Element::zerujLicznikOdOstatniegoUpdate()
{
	licznikOdOstatniegoUpdate=0;
}
bool Element::punktZawieraSie(float x, float y)
{
	if (getOdlegloscOdSrodkaObiektu(x,y) < getPromien()) return true;
	else return false;
}
Element *Element::getCel()
{
	return cel;
}
int Element::getSzybkoscAtaku()
{
	return obiekt->getSzybkoscAtaku()+50*buffSzybkoscAtaku;
}
int Element::getOstatniAtak()
{
	return ostatniAtak;
}
bool Element::moznaAtakowac()
{
	if (ostatniAtak >=obiekt->getSzybkoscAtaku()) return true;
	else return false;
}
void Element::atak()
{
	ostatniAtak=0;
}
void Element::wyczyscDroga()
{
	droga.clear();
}
void Element::licznik()
{
	ostatniAtak++;
}
void Element::setDrogaDocelowy(float x, float y)
{
	drogaDocelowy.x=x;
	drogaDocelowy.y=y;
}
int Element::getId()
{
	return id;
}
int Element::getHpMax()
{
	return obiekt->getMaxHp()+buffHP;
}
int Element::getHpAktualne()
{
	return hpAktualne+buffHP;
}
void Element::onHit()
{
	hpAktualne--;
}
void Element::onHitCel()
{
	cel->onHit();
}
bool Element::onDelete()
{
	return true;
}
Wektor Element::getDrogaDocelowy()
{
	return drogaDocelowy;
}
void Element::setAttackMove(bool _true)
{
	stan.attackMove=_true;
}
bool Element::celIsSet()
{
	return celSet;
}
void Element::setCel(Element* _cel)
{
	cel=_cel;
}
void Element::setCelSet(bool _true)
{
	celSet=_true;
}
int Element::getZasieg()
{
	return obiekt->getZasieg()+50*buffZasieg;
}
int Element::getObrazenia()
{
	return buffObrazenia;
}
Punkt Element::getPozycja()
{
	return Punkt(pozycjaX, pozycjaY);
}
void Element::setAkcja(Akcja a)
{
	akcja=a;
}
Akcja Element::getAkcja()
{
	return akcja;
}
void Element::setPotrzebnePrzeliczenieDrogi(bool _true)
{
	stan.potrzebnePrzeliczenieDrogi=_true;
}
void Element::setDrogaZostalaPrzeliczona(bool _true)
{
	stan.drogaZostalaPrzeliczona=_true;
}
bool Element::getNeedDrogaUpdate()
{
	return needDrogaUpdate;
}
void Element::drogaUpdated()
{
	needDrogaUpdate=false;
}
TargetType Element::getTargetType()
{
	return obiekt->getTypCelu();
}
void Element::setTargetType(TargetType t)
{
	obiekt->setTargetType(t);
}
void Element::setKierunek(Kierunek k)
{
	kierunek=k;
}
Kierunek Element::getKierunek()
{
	return kierunek;
}
void Element::restartHP()
{
	hpAktualne=obiekt->getMaxHp();
}
void Element::setRozmiar(int pix)
{
	float wielk=getRozmiar();
	float docelow=pix;
	setSkala(docelow/wielk);
}
void Element::setRotacja(bool b)
{
	rotacja=b;
}
bool Element::getRotacja()
{
	return rotacja;
}

TargetType IPocisk::getTypCelu()
{
	return typCelu;
}
void IPocisk::setTargetType(TargetType typ)
{
	typCelu=typ;
}
Pocisk::Pocisk()
{
	typCelu=TargetType();
}
Pocisk::Pocisk(string _sciezka, TargetType typ)
{
	bitmapa = al_load_bitmap(_sciezka.c_str());
	predkoscBazowa=3;
	typCelu=typ;
}
Pocisk::Pocisk(Element *_cel, TargetType typ)
{
	bitmapa = al_load_bitmap("pocisk.png");
	predkoscBazowa=3;
	ustawPrzezroczystosc();
	typCelu=typ;
}
Pocisk::Pocisk(string _sciezka, bool alphaDisabled, TargetType typ)
{
	bitmapa = al_load_bitmap(_sciezka.c_str());
	predkoscBazowa=3;
	if (alphaDisabled) ustawPrzezroczystosc();
	typCelu=typ;
}
Pocisk::~Pocisk()
{
	al_unlock_bitmap(bitmapa);
}
void Pocisk::rysuj(int _x, int _y, float _skala, float kierunek, int klatka)
{
	al_draw_scaled_bitmap(bitmapa, 0, 0, al_get_bitmap_width(bitmapa), al_get_bitmap_height(bitmapa), _x, _y, al_get_bitmap_width(bitmapa)*_skala, al_get_bitmap_height(bitmapa)*_skala, 0);
}

Protagonista::Protagonista()
{
	hp=10;
	zasieg=150;
}
Protagonista::Protagonista(string _sciezka)
{
	bitmapa = al_load_bitmap(_sciezka.c_str());
	predkoscBazowa=3;
	szybkoscAtaku=20;
	hp=10;
	zasieg=150;
}
Protagonista::Protagonista(string _sciezka, bool alphaDisabled)
{
	bitmapa = al_load_bitmap(_sciezka.c_str());
	predkoscBazowa=3;
	if (alphaDisabled) ustawPrzezroczystosc();
	szybkoscAtaku = 20;
	hp=10;
	zasieg=150;
}
Protagonista::~Protagonista()
{
	//al_destroy_bitmap(bitmapa);
}

void IPostac::zadajObrazenia(int _obrazenia)
{
	wytrzymalosc = wytrzymalosc - _obrazenia;
	if (wytrzymalosc < 0) wytrzymalosc = 0; 
}

void Protagonista::rysuj(int _x, int _y, float _skala, float kierunek, int klatka)
{
	switch (klatka)
	{
	case 0:
		{
			al_draw_scaled_bitmap(bitmapa, 0, 0, al_get_bitmap_width(bitmapa)/2, al_get_bitmap_height(bitmapa), _x, _y-0.25*al_get_bitmap_height(bitmapa)*_skala, al_get_bitmap_width(bitmapa)*_skala, al_get_bitmap_height(bitmapa)*_skala*3/2, 0);
			break;
		}
	case 1:
		{
			al_draw_scaled_bitmap(bitmapa, al_get_bitmap_width(bitmapa)/2, 0, al_get_bitmap_width(bitmapa)/2, al_get_bitmap_height(bitmapa), _x, _y-0.25*al_get_bitmap_height(bitmapa)*_skala, al_get_bitmap_width(bitmapa)*_skala, al_get_bitmap_height(bitmapa)*_skala*3/2, 0);
			break;
		}
	default:
		{
			al_draw_scaled_bitmap(bitmapa, 0, 0, al_get_bitmap_width(bitmapa)/2, al_get_bitmap_height(bitmapa), _x, _y-0.25*al_get_bitmap_height(bitmapa)*_skala, al_get_bitmap_width(bitmapa)*_skala, al_get_bitmap_height(bitmapa)*_skala*3/2, 0);
			break;
		}
	}
}

Przeciwnik::Przeciwnik()
{
	hp=5;
	zasieg=1;
}
Przeciwnik::Przeciwnik(string _sciezka)
{
	bitmapa = al_load_bitmap(_sciezka.c_str());
	predkoscBazowa=3;
	hp=5;
	zasieg=1;
	szybkoscAtaku=50;
}
Przeciwnik::Przeciwnik(string _sciezka, bool alphaDisabled)
{
	bitmapa = al_load_bitmap(_sciezka.c_str());
	predkoscBazowa=1;
	if (alphaDisabled) ustawPrzezroczystosc();
	hp=5;
	zasieg=1;
	szybkoscAtaku=50;
}
void Przeciwnik::rysuj(int _x, int _y, float _skala, float kierunek, int klatka)
{
	//al_draw_scaled_rotated_bitmap(bitmapa, 0, 0, al_get_bitmap_width(bitmapa)/2, al_get_bitmap_height(bitmapa)/2, _x, _y, al_get_bitmap_width(bitmapa)*_skala, al_get_bitmap_height(bitmapa)*_skala, kierunek, 0);
	al_draw_scaled_rotated_bitmap(bitmapa, al_get_bitmap_width(bitmapa)/2, al_get_bitmap_height(bitmapa)/2, _x+al_get_bitmap_width(bitmapa)*_skala/2, _y+al_get_bitmap_height(bitmapa)*_skala/2, _skala,_skala, -kierunek, 0);
	
}
Przeciwnik::~Przeciwnik()
{
	//al_destroy_bitmap(bitmapa);
}

PrzeciwnikDzialko::PrzeciwnikDzialko()
{
	hp=5;
	zasieg=1;
}
PrzeciwnikDzialko::PrzeciwnikDzialko(string _sciezka)
{
	bitmapa = al_load_bitmap(_sciezka.c_str());
	predkoscBazowa=3;
	hp=5;
	zasieg=1;
}
PrzeciwnikDzialko::PrzeciwnikDzialko(string _sciezka, bool alphaDisabled)
{
	bitmapa = al_load_bitmap(_sciezka.c_str());
	predkoscBazowa=1;
	if (alphaDisabled) ustawPrzezroczystosc();
	hp=5;
	zasieg=1;
}
void PrzeciwnikDzialko::rysuj(int _x, int _y, float _skala, float kierunek, int klatka)
{
	al_draw_scaled_bitmap(bitmapa, 0, 0, al_get_bitmap_width(bitmapa), al_get_bitmap_height(bitmapa), _x, _y, al_get_bitmap_width(bitmapa)*_skala, al_get_bitmap_height(bitmapa)*_skala, 0);
}
PrzeciwnikDzialko::~PrzeciwnikDzialko()
{
	//al_destroy_bitmap(bitmapa);
}
Przeszkoda::Przeszkoda(string _sciezka)
{
	bitmapa = al_load_bitmap(_sciezka.c_str());
	hp=20;
}
//fixed rozmiar do poprawy?
Przeszkoda::Przeszkoda()
{
	hp=20;
}
Przeszkoda::Przeszkoda(string _sciezka, bool alphaDisabled)
{
	bitmapa = al_load_bitmap(_sciezka.c_str());
	if (alphaDisabled) ustawPrzezroczystosc();
	hp=20;
}
Przeszkoda::~Przeszkoda()
{

}
void Przeszkoda::rysuj(int _x, int _y, float _skala, float kierunek, int klatka)
{
	al_draw_scaled_bitmap(bitmapa, 0, 0, al_get_bitmap_width(bitmapa), al_get_bitmap_height(bitmapa), _x, _y, al_get_bitmap_width(bitmapa)*_skala, al_get_bitmap_height(bitmapa)*_skala, 0);
}

Silnik::Silnik()
{
	/*ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *mysz = NULL;
	ALLEGRO_BITMAP *perkBlue=NULL;
	ALLEGRO_BITMAP *perkYellow=NULL;*/
	currentElementObliczenieDrogi=0;
	zmianaMapy=false;
	endLevelItem=false;
	srand(time(NULL));
	koniecGry=false;
	idCounter=0;
	mapaWielkoscX=13;
	mapaWielkoscY=7;
	FPS = 120;
	iloscTickow=900;
	aktualnyTick=0;
	SCREEN_W = 640;
	SCREEN_H = 480;
	bohaterPozycjaDocelowaX=0;
	bohaterPozycjaDocelowaY=0;
	debugMode=0;
	skalaGlobalna=1.0;
	tickowNaRuch=15;
	init();
	protagonista=Protagonista ("Octopus-96.png", true);
	przeciwnik=Przeciwnik("Mite-48.png", true);
	przeszkoda=Przeszkoda("RTS_Crate.png");
	przeciwnikWieza=Przeciwnik("Hornet Hive-50.png", true);
	przeciwnikWidmo=Przeciwnik("Ghost-52.png", true);
	mapa.stworz(mapaWielkoscX,mapaWielkoscY);
	pasek=PasekZdrowia("hp1.png", "hp2.png", "hp3.png");
	dlugoscGryMax=losuj(5,10);
	dlugoscGryAktualna=-1;
	przegrana=false;
	wygrana=false;
}
Silnik::Perk::Perk()
{
	pozycjaX=-1;
	pozycjaY=-1;
	remove=false;
	typ=TypPerka::brak;
}
Silnik::Perk::Perk(int x, int y, TypPerka t)
{
	pozycjaX=x;
	pozycjaY=y;
	typ=t;
	remove=false;
}
Silnik::~Silnik()
{

}
int Silnik::gra()
{
	nonie=0;
	//ALLEGRO_THREAD	*thread_1=NULL;
	bool koniec=false;
	klikniecie=false;
	bool redraw = true;
	int licznikFps=0;//dla 900 tickow/s oraz 60 fps klatka ma byc wiysweitlana co 15 tickow
	//mutexLista=al_create_mutex();
	stworzMape();
	skaluj(2.0);
	Element &e=listaElementow.front();
	e.setRozmiar(40);
	nowaMapa();
	mapa.przeliczWierzcholki();
	mapa.przeliczKrawedzie();
	//concurrent_vector <Punkt> abecadlo;
	//argumentWatek a(listaElementow, mutexLista, mapa, bohaterPozycjaDocelowaX, bohaterPozycjaDocelowaY, koniecGry);
	//a.lista=listaElementow;
	//a.mut=mutexLista;
	//thread_1=al_create_thread(Func_Thread, &a);
	//al_start_thread(thread_1);
	while(!koniec)
	{
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			aktualnyTick++;
			licznikFps++;
			ruch();
			if (aktualnyTick >= iloscTickow) aktualnyTick=0;
			if (licznikFps >= iloscTickow/FPS)
			{
				redraw=true;
				licznikFps=0;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			koniecGry=true;
			//al_destroy_thread(thread_1);
			break;
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
		{
			al_get_mouse_state(&mouseState);
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			al_get_mouse_state(&mouseState);
			for (auto it=listaElementow.begin();it !=listaElementow.end();++it)
			{
				if (it->getTyp()==0)
				{
					//al_lock_mutex(mutexLista);
					it->setAttackMove(false);//klikniecie mysza == ruch zwykly a nie attack move
					//al_unlock_mutex(mutexLista);
					break;
				}
			}
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			al_get_mouse_state(&mouseState);
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			al_get_keyboard_state(&keyboardState);
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_LCTRL:
				for (auto it=listaElementow.begin();it !=listaElementow.end();++it)
				{
					if (it->getTyp()==0)
					{
						//al_lock_mutex(mutexLista);
						it->setGhostPostacie(true);
						//al_unlock_mutex(mutexLista);
						break;
					}
				}
				break;
			case ALLEGRO_KEY_LSHIFT:
				for (auto it=listaElementow.begin();it !=listaElementow.end();++it)
				{
					if (it->getTyp()==0)
					{
						//al_lock_mutex(mutexLista);
						it->setGhostPrzeszkody(true);
						//al_unlock_mutex(mutexLista);
						break;
					}
				}
				break;
			case ALLEGRO_KEY_A:
				for (auto it=listaElementow.begin();it !=listaElementow.end();++it)
				{
					if (it->getTyp()==0)
					{
						//al_lock_mutex(mutexLista);
						it->setAttackMove(true);
						//al_unlock_mutex(mutexLista);
						break;
					}
				}
				break;
			case ALLEGRO_KEY_DELETE:
				nowaMapa();
				break;
			case ALLEGRO_KEY_ENTER:
				{
					if (wygrana==true || przegrana==true)
					{
						restart();
					}
					break;
				}
				
			default:
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			al_get_keyboard_state(&keyboardState);
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_LCTRL:
				for (auto it=listaElementow.begin();it !=listaElementow.end();++it)
				{
					if (it->getTyp()==0)
					{
						//al_lock_mutex(mutexLista);
						it->setGhostPostacie(false);
						//al_unlock_mutex(mutexLista);
						break;
					}
				}
				break;
			case ALLEGRO_KEY_LSHIFT:
				for (auto it=listaElementow.begin();it !=listaElementow.end();++it)
				{
					if (it->getTyp()==0)
					{
						//al_lock_mutex(mutexLista);
						it->setGhostPrzeszkody(false);
						//al_unlock_mutex(mutexLista);
						break;
					}
				}
				break;
			default:
				break;
			}
		}
		if (licznikFps >= iloscTickow/FPS)
		{
			redraw=true;
			licznikFps=0;
		}
		if(redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
			if (mouseState.buttons & 2)
			{
				myszKopia=mouseState;
				klikniecie=true;
				bohaterPozycjaDocelowaX=mouseState.x;
				if (bohaterPozycjaDocelowaX < 0) bohaterPozycjaDocelowaX=0;
				if (bohaterPozycjaDocelowaX > SCREEN_W) bohaterPozycjaDocelowaX=SCREEN_W;
				bohaterPozycjaDocelowaY=mouseState.y;
				if (bohaterPozycjaDocelowaY < 0) bohaterPozycjaDocelowaY=0;
				if (bohaterPozycjaDocelowaY > SCREEN_H) bohaterPozycjaDocelowaY=SCREEN_H;
			}
			if (mouseState.buttons & 4)
			{
				break;
			}
			al_clear_to_color(al_map_rgb(128,128,128));
			if (wygrana==true) wyswietlWygrana();
			else if (przegrana==true) wyswietlPrzegrana();
			else rysuj();
			if (debugMode) wyswietlDebug();
			al_draw_bitmap(mysz, mouseState.x, mouseState.y, 0);
			al_flip_display();
		}
	}
	return 0;
}
void Silnik::dodajPrzeszkoda(IObiekt &a, int x, int y)
{
	int idTemp=getId();
	Punkt temp=przeliczRozdzMapa(x,y);
	//al_lock_mutex(mutexLista);
	Element temp2(&a,temp.x,temp.y,TypObiektu(TypObiektu::przeszkoda),1,0, idTemp);
	temp2.setSkala(skalaGlobalna);
	temp2.setRozmiar(50);
	listaElementow.push_back(temp2);
	//al_unlock_mutex(mutexLista);
	//mutexy.push_back(Mutexy());
	//mutexy.back().id=idTemp;
	mapa.dodajObiekt(Mapa::Obiekt(x,y));
}
void Silnik::ruch()
{
	if (currentElementObliczenieDrogi >= listaElementow.size()) currentElementObliczenieDrogi=0;
	int indeksator=0;//jezeli indeksator==currentelementobliczeniedrogi to ten element ma obliczana na nowo droge
	for(Element &e : listaElementow)
	{
		e.zwiekszLicznikOdOstantiegoUpdate();
		switch(e.getSiType())
		{
		case SiType::siGracz://gracz
			{
				if (e.czasNaRuch())
				{
					porusz(e, SiGracz(e, true), true);
					korygujPozycje(&e);
				}
				break;
			}
		case SiType::siObiektStatyczny://obiekt statyczny
			{
				break;
			}
		case SiType::siPrzeciwnik://przeciwnik
			{
				if (e.czasNaRuch())
				{
					if (indeksator!=currentElementObliczenieDrogi) porusz(e, siMobekMeele(e, false), true);
					else porusz(e, siMobekMeele(e, true), true);
					korygujPozycje(&e);
				}
				break;
			}
		case SiType::siPrzeciwnikWieza://przeciwnik
			{
				if (e.czasNaRuch())
				{
					porusz(e, siMobekWieza(e, true), true);
					korygujPozycje(&e);
				}
				break;
			}
		default:
			{
				if (e.czasNaRuch())
				{
					porusz(e, siDefault(e, true), true);
					korygujPozycje(&e);
				}
				break;
			}
		}
		indeksator++;
	}
	for(Element &e : listaPociskow)
	{
		switch(e.getSiType())
		{
		case SiType::siPocisk:
			if (e.czasNaRuch())
			{
				switch(e.getTyp())
				{
				case TypObiektu::pocisk:
					{
						porusz(e, siPocisk(e, true), true);
						break;
					}
				case TypObiektu::pociskPoProstej:
					{
						porusz(e, siPociskPoProstej(e, true), true);
						for (auto temp=listaElementow.begin();temp!=listaElementow.end();++temp)//sprawdzenie czy jest trafienie
						{
							int raz=temp->getTyp();
							TargetType dwa=e.getTargetType();
							if((temp->getTyp()==TypObiektu::gracz && e.getTargetType().gracz==true) || 
								(temp->getTyp()==TypObiektu::przeciwnik && e.getTargetType().przeciwnik==true) || 
								(temp->getTyp()==TypObiektu::przeszkoda && e.getTargetType().statyczny==true))
							{
								if (odleglosc(temp->getPozycja(), e.getPozycja())<= temp->getRozmiar()+e.getRozmiar())
								{
									doUsuniecia.push_back(e.getId());
									temp->onHit();
								}
							}
						}
						
						break;
					}
				default:
					{
						break;
					}
				}
				
				korygujPozycje(&e);
			}
			break;
		default:
			break;
		}
	}
	for (Element e : listaElementow)
	{
		if(e.getHpAktualne() <= 0)
		{
			if (e.getTyp()==TypObiektu::gracz) continue;
			if (e.onDelete())
			{
				pushDoUsuniecia(e.getId());
			}
		}
	}
	currentElementObliczenieDrogi++;
	auto it=listaElementow.front();//obiekt gracza
	if (it.getHpAktualne()<=0) przegrana=true;
	usuwanieObiektow();
	osbslugaPerkow();
	if (endLevel()) nowaMapa();
}
void Silnik::rysuj()
{
	//al_draw_scaled_bitmap(bitmapa, 0, 0, al_get_bitmap_width(bitmapa), al_get_bitmap_height(bitmapa), _x, _y, al_get_bitmap_width(bitmapa)*_skala, al_get_bitmap_height(bitmapa)*_skala, 0);
	//al_draw_scaled_bitmap(tlo, 0,0,al_get_bitmap_width(tlo), al_get_bitmap_height(tlo), 0,0,SCREEN_W, SCREEN_H, 0);
	al_draw_tinted_scaled_bitmap(tlo, al_map_rgba_f(1, 1, 1, 0.5), 0,0,al_get_bitmap_width(tlo), al_get_bitmap_height(tlo), 0,0,SCREEN_W, SCREEN_H, 0);
	for(Element e : listaElementow)
	{
		e.rysuj();
		//al_draw_textf(font, al_map_rgb(255,0,0), e.getPozycjaX()-10, e.getPozycjaY()-40, ALLEGRO_ALIGN_LEFT, "%d", e.getHpAktualne());
		if (e.getTyp()==TypObiektu::przeciwnik || e.getTyp()==TypObiektu::gracz) pasek.rysuj(e.getPozycjaX()-pasek.getWidth()/4, e.getPozycjaY()-e.getRozmiar()*2, (float)e.getHpAktualne()/e.getHpMax(), 0.5);
	}
	for(Element e: listaPociskow)
	{
		e.rysuj();
	}
	wyswietlPerki();
	wyswietlGUI();
}
Akcja Silnik::siDefault(Element &_element, bool obliczenieDrogi)
{
	Akcja a;
	float celX, celY, x, y;
	for(Element e : listaElementow)
	{
		if (e.getTyp()==0)
		{
			celX=e.getPozycjaX();
			celY=e.getPozycjaY();
			break;
		}
	}
	x=abs(celX-_element.getPozycjaX());
	if (x < 2) x=0;
	y=abs(celY-_element.getPozycjaY());
	if (y < 2) y=0;
	float wynik=0;
	if (!(x==0 && y==0)) wynik=1/(sqrt(x*x+y*y));
	a.ruchX=(celX-_element.getPozycjaX())*wynik;
	a.ruchY=(celY-_element.getPozycjaY())*wynik;
	a.ruch=true;
	return a;
}
Akcja Silnik::siMobekMeele(Element &_element, bool obliczenieDrogi)
{
	Akcja a;
	a.ruch=false;
	a.ruchX=-1;
	a.ruchY=-1;
	float celX, celY, x, y;
	for(Element e : listaElementow)
	{
		if (e.getTyp()==0)
		{
			celX=e.getPozycjaX();
			celY=e.getPozycjaY();
			break;
		}
	}
	if (_element.getStan().ghostPrzeszkody==true)//dla przeciwnika widmo po prostu idziemy do celu
	{
		_element.setDrogaDocelowy(celX, celY);
	}
	else if (_element.getLicznikOdOstatniegoUpdate() > 500+(std::rand() % (1000)))
	{
		if (_element.setDocelowy(celX, celY))//jezeli pozycja docelowa jest nowo ustalona (nastapila zmiana)
		{
			_element.zerujLicznikOdOstatniegoUpdate();
			obliczDroga(_element);
		}
	}
	Wektor docelowy=_element.getDroga(przeliczRozdzMapa2(_element.getPozycjaX(), _element.getPozycjaY()));//do tego punktu zmierzamy (albo w nim jestesmy)
	if (!_element.punktFinalny())
	{
		Punkt korekta=przeliczRozdzMapa(docelowy.x,docelowy.y);
		docelowy.x=korekta.x;
		docelowy.y=korekta.y;
	}
	x=abs(docelowy.x-_element.getPozycjaX());
	if (x < 2) x=0;
	y=abs(docelowy.y-_element.getPozycjaY());
	if (y < 2) y=0;
	if(!(x==0 && y==0))//jezeli wykonujemy ruch
	{
		float wynik=1/(sqrt(x*x+y*y));//aby znormalizowac wektor przemnozymy przez ta wartosc
		a.ruchX=(docelowy.x-_element.getPozycjaX())*wynik;//na nowo bohaterPozycjaDocelowaX-_pozX ¿eby mieæ tak¿e ujemne wartoœci
		a.ruchY=(docelowy.y-_element.getPozycjaY())*wynik;
		a.ruch=true;
	}
	return a;
}
Akcja Silnik::siMobekWieza(Element &_element, bool obliczenieDrogi)
{
	Akcja a;
	a.ruch=false;
	a.ruchX=-1;
	a.ruchY=-1;
	float celX, celY;
	for(Element &e : listaElementow)
	{
		if (e.getTyp()==0)
		{
			celX=e.getPozycjaX();
			celY=e.getPozycjaY();
			break;
		}
	}
	if (odleglosc(_element.getPozycja(), Punkt(celX, celY)) < 200)
	{
		if (_element.moznaAtakowac())
		{
			TargetType typ;
			typ.gracz=true;
			dodajPociskPoProstej(_element.getPozycjaX(), _element.getPozycjaY(), celX, celY, typ);
			_element.atak();
		}
	}
	return a;
}
Akcja Silnik::SiGracz(Element &_element, bool obliczenieDrogi)
{
	Akcja a;
	a.ruch=false;
	bool atak=false;//flaga czy kliknieto na przeciwnika (atakujemy) czy na mape (poruszamy sie)
	if (_element.getStan().attackMove == true)//jezeli padla komenda attackMove
	{
		if (!_element.celIsSet())//sprawdzamy czy jest to pierwszy raz (czy trzeba ustawic nowy cel)
		{
			_element.setCel(najblizszyPrzeciwnik(Punkt(_element.getPozycjaX(),_element.getPozycjaY())));//jezeli tak to ustawiamy
			_element.setCelSet(true);
		}
		bohaterPozycjaDocelowaX=_element.getPozycjaX();
		bohaterPozycjaDocelowaY=_element.getPozycjaY();
		if (_element.moznaAtakowac())
		{
			Element* temp=attackMove(_element.getPozycja(), Punkt(mouseState.x, mouseState.y), _element.getZasieg());
			if (temp != NULL)
			{
				TargetType typ;
				typ.przeciwnik=true;
				dodajPocisk(*temp,_element.getPozycjaX(), _element.getPozycjaY(), typ);
				//dodajPociskPoProstej(_element.getPozycjaX(), _element.getPozycjaY(), temp->getPozycjaX(), temp->getPozycjaY());
				_element.atak();
				atak=true;
			}
			else
			{
				//_element.wyczyscDroga();
				//_element.setDocelowy(mouseState.x, mouseState.y);
				atak=false;
			}
		}
	}
	if (klikniecie && _element.getStan().attackMove == false)
	{
		_element.setCelSet(false);//nie jest to komenda attack move, wiec wylaczamy cel
		for (auto it=listaElementow.begin();it!=listaElementow.end();++it)
		{
			if (it->getTyp()!=TypObiektu::gracz)
			{
				if (it->punktZawieraSie(myszKopia.x,myszKopia.y))
				{
					atak=true;
					bohaterPozycjaDocelowaX=_element.getPozycjaX();
					bohaterPozycjaDocelowaY=_element.getPozycjaY();
					if (_element.moznaAtakowac())
					{
						//dodajPocisk((*it),_element.getPozycjaX(), _element.getPozycjaY());
						//_element.atak();
					}
					break;
				}
			}
			
		}
		klikniecie=false;
	}
	if (!atak)
	{
		_element.setCelSet(false);//nie jest to komenda attack move, wiec wylaczamy cel
		if (_element.getLicznikOdOstatniegoUpdate() > 30)
		{
			if (_element.setDocelowy(bohaterPozycjaDocelowaX, bohaterPozycjaDocelowaY))//jezeli pozycja docelowa jest nowo ustalona (nastapila zmiana)
			{
				_element.zerujLicznikOdOstatniegoUpdate();
				obliczDroga(_element);
			}
		}
		Wektor docelowy;
		if (_element.getStan().ghostPrzeszkody)//jezeli postac moze przechodzic przez przeszkody to poruszamy sie bezposrednio do celu
		{
			docelowy=_element.getDrogaDocelowy();
			_element.wyczyscDroga();
			_element.setDocelowy(docelowy.x, docelowy.y);
		}
		else//jezeli nie moze
		{
			docelowy=_element.getDroga(przeliczRozdzMapa2(_element.getPozycjaX(), _element.getPozycjaY()));//wybieramy kolejny punkt drogi
			if (!_element.punktFinalny())
			{
				Punkt korekta=przeliczRozdzMapa(docelowy.x,docelowy.y);
				docelowy.x=korekta.x;
				docelowy.y=korekta.y;
			}
		}
		float x,y;
		x=abs(docelowy.x-_element.getPozycjaX());//potrzebne do normalizacji wektora
		if (x < 2) x=0;
		y=abs(docelowy.y-_element.getPozycjaY());
		if (y < 2) y=0;
		if(!(x==0 && y==0))//jezeli wykonujemy ruch
		{
			float wynik=1/(sqrt(x*x+y*y));//aby znormalizowac wektor przemnozymy przez ta wartosc
			if (_element.getOdlegloscOdSrodkaObiektu(docelowy.x, docelowy.y)>_element.getPromien() || _element.getRuch())//jezeli stoi i odleglosc jest za mala to sie nie porusza
			{
				a.ruchX=(docelowy.x-_element.getPozycjaX())*wynik;//na nowo bohaterPozycjaDocelowaX-_pozX ¿eby mieæ tak¿e ujemne wartoœci
				a.ruchY=(docelowy.y-_element.getPozycjaY())*wynik;
				a.ruch=true;
			}
		}
	}
	return a;
}
Akcja Silnik::siPocisk(Element &_element, bool obliczenieDrogi)
{
	Akcja a;
	float celX, celY, x, y;
	celX=_element.getCel()->getPozycjaX();
	celY=_element.getCel()->getPozycjaY();
	x=abs(celX-_element.getPozycjaX());
	if (x < 5) x=0;
	y=abs(celY-_element.getPozycjaY());
	if (y < 5) y=0;
	float wynik=0;
	if (!(x==0 && y==0)) wynik=1/(sqrt(x*x+y*y));
	else
	{
		a.zniszcz=true;
		_element.onHitCel();
	}
	float odl=odleglosc(_element.getPozycja(), _element.getCel()->getPozycja());
	if (odl <= _element.getRozmiar()+_element.getCel()->getRozmiar())
	{
		a.zniszcz=true;
		_element.onHitCel();
	}
	a.ruchX=(celX-_element.getPozycjaX())*wynik;
	a.ruchY=(celY-_element.getPozycjaY())*wynik;
	a.ruch=true;
	return a;
}
Akcja Silnik::siPociskPoProstej(Element &_element, bool obliczenieDrogi)
{
	Akcja a;
	float celX, celY, x, y;
	celX=_element.getCel()->getPozycjaX();
	celY=_element.getCel()->getPozycjaY();
	x=abs(celX-_element.getPozycjaX());
	if (x < 5) x=0;
	y=abs(celY-_element.getPozycjaY());
	if (y < 5) y=0;
	float wynik=0;
	if (!(x==0 && y==0)) wynik=1/(sqrt(x*x+y*y));
	else
	{
		a.zniszcz=true;
		_element.onHitCel();
	}
	a.ruchX=(celX-_element.getPozycjaX())*wynik;
	a.ruchY=(celY-_element.getPozycjaY())*wynik;
	a.ruch=true;
	return a;
}
Akcja Silnik::kolizja(Akcja _akcja)
{
	Akcja temp;
	return temp;
}
void Silnik::wyswietlDebug()
{
	al_draw_ellipse(listaElementow.front().getPozycjaX(), listaElementow.front().getPozycjaY(), listaElementow.front().getZasieg(), listaElementow.front().getZasieg(), al_map_rgb(1,0,255), 1);
	al_draw_textf(font, al_map_rgb(0,0,0), 10, SCREEN_H-20, ALLEGRO_ALIGN_LEFT, "Mysz: %d, %d", mouseState.x, mouseState.y);
	al_draw_textf(font, al_map_rgb(0,0,0), 10, SCREEN_H-40, ALLEGRO_ALIGN_LEFT, "BohaterDocelowa: %d, %d", bohaterPozycjaDocelowaX, bohaterPozycjaDocelowaY);
	for (Element e : listaElementow)
	{
		al_draw_ellipse(e.getPozycjaX(), e.getPozycjaY(), e.getPromien(), e.getPromien(), al_map_rgb(1,0,0), 1);
	}
	int x=mapa.getRozmiarX();
	int y=mapa.getRozmiarY();
	mapa.rysuj();
	Element temp=listaElementow.front();
	if (temp.getStan().ghostPostacie == true) al_draw_ellipse(100, 100, 10, 10, al_map_rgb(255,0,0), 1);
	else al_draw_ellipse(100, 100, 10, 10, al_map_rgb(1,0,255), 1);
	al_draw_textf(font, al_map_rgb(0,0,0), 400, SCREEN_H-60, ALLEGRO_ALIGN_LEFT, "kierunek: %d", temp.getKierunek());
	al_draw_textf(font, al_map_rgb(0,0,0), 10, SCREEN_H-60, ALLEGRO_ALIGN_LEFT, "ilosc: %d", temp.getLicznikOdOstatniegoUpdate());
	al_draw_textf(font, al_map_rgb(0,0,0), 10, SCREEN_H-80, ALLEGRO_ALIGN_LEFT, "ostatni atak: %d", temp.getOstatniAtak());
}
void Silnik::skaluj (float _skala)
{
	for(Element &e : listaElementow)
	{
		//al_lock_mutex(mutexLista);//chyba nie potrzebne
		e.setSkala(_skala);
		//al_unlock_mutex(mutexLista);
	}
	//skalaGlobalna=skalaGlobalna*_skala;
	skalaGlobalna=_skala;
}
void Silnik::korygujPozycje(Element* e)
{
	if (e->getPozycjaX()-e->getRozmiarX()/2<0) e->setPozycjaX(0+e->getRozmiarX()/2);
	if (e->getPozycjaY()-e->getRozmiarY()/2<0) e->setPozycjaY(0+e->getRozmiarY()/2);
	if (e->getPozycjaX()+e->getRozmiarX()/2>SCREEN_W) e->setPozycjaX(SCREEN_W-e->getRozmiarX()/2);
	if (e->getPozycjaY()+e->getRozmiarY()/2>SCREEN_H) e->setPozycjaY(SCREEN_H-e->getRozmiarY()/2);
}
float Silnik::odleglosc(Punkt a, Punkt b)
{
	return sqrt((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y));
}
bool Silnik::wektorPrzeciwny(Wektor w, Punkt pocz, Punkt kon)
{
	Wektor W1(w.x/dlugoscWektora(w),w.y/dlugoscWektora(w));//normalizacja wektora
	Wektor W2;
	W2.x=kon.x-pocz.x;
	W2.y=kon.y-pocz.y;
	float dlugosc=dlugoscWektora(W2);//dlugosc do normalizacji
	W2.x=W2.x/dlugosc;//normalizacja
	W2.y=W2.y/dlugosc;
	if (abs(W1.x+W2.x) < 0.3 && abs(W1.y+W2.y) < 0.3) return true;//wartosci na oko
	else return false;
}
float Silnik::dlugoscWektora(Wektor w)
{
	return sqrt(w.x*w.x+w.y*w.y);
}
Wektor Silnik::obrotWektora(Wektor w, double radiany)
{
	Wektor temp;
	temp.x = w.x *cos(radiany) - w.y *sin(radiany);
    temp.y = w.x * sin(radiany) + w.y * cos(radiany);
    return temp;
}
Punkt Silnik::przeliczRozdzMapa(int _x, int _y)
{
	if (_x < 0 || _y < 0 || _x >= mapaWielkoscX || _y >= mapaWielkoscY)//sprawdzenie poprawnosci zakresu
	{
		return (Punkt(0,0));
	}
	int X=SCREEN_W/mapaWielkoscX;
	int Y=SCREEN_H/mapaWielkoscY;
	Punkt wynik;
	wynik.x=_x*X+0.5*X;
	wynik.y=_y*Y+0.5*Y;
	return wynik;
}
Punkt Silnik::przeliczRozdzMapa2(int _x, int _y)
{
	int rozdzX=SCREEN_W/mapaWielkoscX;
	int rozdzY=SCREEN_H/mapaWielkoscY;
	Punkt wynik;
	wynik.x=_x/rozdzX;
	wynik.y=_y/rozdzY;
	return wynik;
}
int Silnik::init()
{
	if(!al_init())
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	if(!al_install_mouse())
	{
		fprintf(stderr, "failed to initialize the mouse!\n");
		return -1;
	}
	if(!al_install_keyboard())
	{
		fprintf(stderr, "failed to initialize keyboard!\n");
		return -1;
	}
	if(!al_init_image_addon())
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	al_init_font_addon();
	al_init_ttf_addon();
	font = al_load_ttf_font("luximb.ttf",15,0 );
	if (!font)
	{
		fprintf(stderr, "Could not load 'pirulen.ttf'.\n");
		return -1;
	}
	timer = al_create_timer(1.0 / iloscTickow);
	if(!timer)
	{
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display)
	{
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}
	mysz = al_load_bitmap("mouse.png");
	if(!mysz)
	{
		fprintf(stderr, "failed to create bouncer bitmap!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}
	tlo=al_load_bitmap("brick_pavement_0132_02_tiled_s.jpg");
	perkBlue=al_load_bitmap("buffBlue.png");
	perkRed=al_load_bitmap("buffRed.png");
	perkYellow=al_load_bitmap("buffYellow.png");
	perkGreen=al_load_bitmap("buffGreen.png");
	al_convert_mask_to_alpha(perkBlue, al_map_rgb(255,0,255));
	al_convert_mask_to_alpha(perkRed, al_map_rgb(255,0,255));
	al_convert_mask_to_alpha(perkYellow, al_map_rgb(255,0,255));
	al_convert_mask_to_alpha(perkGreen, al_map_rgb(255,0,255));
	if(!al_init_primitives_addon()){
        fprintf(stderr,"Couldn't initialize primitives addon!\n");
        return -1;
    }
	al_convert_mask_to_alpha(mysz, al_map_rgb(255,0,255));
	al_hide_mouse_cursor(display);
	al_set_target_bitmap(al_get_backbuffer(display));
	event_queue = al_create_event_queue();
	if(!event_queue)
	{
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_bitmap(mysz);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();
	al_start_timer(timer);
	al_get_mouse_state(&mouseState);//¿eby na pewno by³ bo mo¿e byæ crash
	al_get_keyboard_state(&keyboardState);
	return 1;
}
void Silnik::deinit()
{
	//al_destroy_bitmap(mysz);
	//al_destroy_timer(timer);
	//al_destroy_display(display);
	//al_destroy_event_queue(event_queue);
	//protagonista.zniszcz();
	//przeszkoda.zniszcz();
	//przeciwnik.zniszcz();
}
void Silnik::stworzMape()
{
	int idTemp=getId();
	//al_lock_mutex(mutexLista);
	Element nowy(&protagonista, 0, 0, TypObiektu(TypObiektu::gracz), 0, tickowNaRuch, idTemp);
	nowy.setRotacja(false);
	nowy.setRozmiar(10);
	listaElementow.push_back(nowy);
	//al_unlock_mutex(mutexLista);
	//mutexy.push_back(Mutexy());
	//mutexy.back().id=idTemp;
	/*dodajPrzeszkoda(przeszkoda,2,0);
	dodajPrzeszkoda(przeszkoda,2,1);
	dodajPrzeszkoda(przeszkoda,2,2);
	dodajPrzeszkoda(przeszkoda,2,3);
	dodajPrzeszkoda(przeszkoda,2,4);
	dodajPrzeszkoda(przeszkoda,2,5);
	dodajPrzeszkoda(przeszkoda,3,5);
	dodajPrzeszkoda(przeszkoda,4,5);
	dodajPrzeszkoda(przeszkoda,5,5);
	dodajPrzeszkoda(przeszkoda,6,5);
	dodajPrzeszkoda(przeszkoda,6,4);
	dodajPrzeszkoda(przeszkoda,6,3);
	dodajPrzeszkoda(przeszkoda,6,2);
	dodajPrzeszkoda(przeszkoda,6,1);
	dodajPrzeszkoda(przeszkoda,5,1);
	dodajPrzeszkoda(przeszkoda,4,1);
	dodajPrzeszkoda(przeszkoda,4,2);
	dodajPrzeszkoda(przeszkoda,4,3);
	dodajPrzeciwnik(Punkt(400,200));
	dodajPrzeciwnik(Punkt(500,200));*/
	//dodajPrzeciwnik(Punkt(600,200));
	//dodajPrzeciwnik(Punkt(400,300));
	//dodajPrzeciwnik(Punkt(500,300));
	//dodajPrzeciwnik(Punkt(600,300));
	//nowaMapa();
}
void Silnik::obliczDroga(Element &e)
{
	list<Punkt> wynik;
	float x,y;
	switch (e.getSiType())
	{
	case 0:
		x=bohaterPozycjaDocelowaX;
		y=bohaterPozycjaDocelowaY;
		break;
	case 2:
		for (auto it=listaElementow.begin();it!=listaElementow.end();++it)
		{
			if (it->getTyp() == TypObiektu::gracz)
			{
				x=it->getPozycjaX();
				y=it->getPozycjaY();
				break;
			}
		}
		break;
	default:
		return;//jezeli typ si jest nieustalony to moze byc to przeszkoda (nie ma si)
		break;
	}
	Punkt punkt1=przeliczRozdzMapa2(e.getPozycjaX(), e.getPozycjaY());
	Punkt punkt2=przeliczRozdzMapa2(x, y);
	mapa.droga(punkt1, punkt2, wynik);//utworzenie nowej drogi
	e.setListaDroga(wynik);//ustawienie nowej drogi
}
void Silnik::dodajPrzeciwnik(Punkt gdzie)
{
	int idTemp=getId();
	Element temp(&przeciwnik, gdzie.x, gdzie.y, TypObiektu(TypObiektu::przeciwnik), SiType::siPrzeciwnik, tickowNaRuch, idTemp);
	temp.setSkala(skalaGlobalna);
	temp.setRozmiar(30);
	listaElementow.push_back(temp);
}
void Silnik::dodajPrzeciwnikWidmo(Punkt gdzie)
{
	int idTemp=getId();
	Element temp(&przeciwnikWidmo, gdzie.x, gdzie.y, TypObiektu(TypObiektu::przeciwnik), SiType::siPrzeciwnik, tickowNaRuch, idTemp);
	temp.setSkala(skalaGlobalna);
	temp.setGhostPostacie(true);
	temp.setGhostPrzeszkody(true);
	temp.setRozmiar(70);
	temp.setRotacja(false);
	listaElementow.push_back(temp);
}
void Silnik::dodajPrzeciwnikWieza(Punkt gdzie)
{
	int idTemp=getId();
	Element temp(&przeciwnikWieza, gdzie.x, gdzie.y, TypObiektu(TypObiektu::przeciwnik), SiType::siPrzeciwnikWieza, tickowNaRuch, idTemp);
	temp.setSkala(skalaGlobalna);
	temp.setRozmiar(50);
	temp.setRotacja(false);
	listaElementow.push_back(temp);
}
void Silnik::dodajPocisk(Element &cel, float poczatekX, float poczatekY, TargetType typcelu)
{
	listaPociskow.push_front(Element(new Pocisk(&cel, typcelu), &cel, poczatekX, poczatekY, TypObiektu::pocisk, 3, tickowNaRuch, getId()));
	listaPociskow.front().setGhostPostacie(true);
	listaPociskow.front().setGhostPrzeszkody(true);
	listaPociskow.front().setRozmiar(5);
}
void Silnik::dodajPociskPoProstej(int startX, int startY, int celX, int celY, TargetType typcelu)
{
	Element xs(new Pocisk(NULL, typcelu), new Element(celX,celY), startX, startY, TypObiektu::pociskPoProstej, 3, tickowNaRuch, getId());
	xs.setTargetType(typcelu);//bez tego nie dziala wth
	listaPociskow.push_front(xs);
	listaPociskow.front().setGhostPostacie(true);
	listaPociskow.front().setGhostPrzeszkody(true);
	listaPociskow.front().setRozmiar(5);
}
int Silnik::getId()
{
	idCounter++;
	return idCounter-1;
}
void Silnik::clearId()
{
	idCounter=0;
}
bool Silnik::porusz(Element &e, Akcja a, bool rekurencja)
{
	e.setAkcja(a);
	e.licznik();//zwiekszenie licznika od ostatniego ataku
	if (a.zniszcz == true)
	{
		doUsuniecia.push_back(e.getId());
		return 0;
	}
	if (a.ruch==true)//jezeli akcja zaklada przemieszczenie
	{
		obliczKierunek(e, a.ruchX, a.ruchY);//ustawienie w jakim kierunku sie porusza obiekt
		bool mozna=true;
		if (!(e.getTyp() == TypObiektu::pocisk || e.getTyp() == TypObiektu::pociskPoProstej))//pociski moga sie poruszac po przedmiotach
		{
			float x,y,odl;
			x=e.getPozycjaX();//wspolzedne elementu ktory poruszamy
			y=e.getPozycjaY();
			for (auto it=listaElementow.begin();it!=listaElementow.end();++it)//porownujemy z kazdym obiektem i sprawdzamy czy ruch nie wejdzie na nie
			{
				if (e.getStan().ghostPostacie && it->getTyp()==TypObiektu::przeciwnik) continue;//jezeli obiekt ma mozliwosc przechodzenia przez dany typ obiektow to ignorujemy je
				if (e.getStan().ghostPrzeszkody && it->getTyp()==TypObiektu::przeszkoda) continue;
				if (it->getStan().ghostPostacie) continue;//przeciwnicy moga przechodzic przez przeciwnikow, ktorzy maja ghost
				if (e.getTyp()==TypObiektu::przeciwnik && it->getSiType()==SiType::siPrzeciwnikWieza) continue;//przeciwnicy moga przechodzic przez przeciwnikWieza
				odl=odleglosc(Punkt(x+a.ruchX*e.getPredkosc(),y+a.ruchY*e.getPredkosc()),Punkt(it->getPozycjaX(),it->getPozycjaY()));//odleglosc od pozycji do ktorej chcemy sie dostac do aktualnie badanego obiektu
				if (odl <e.getRozmiar()+it->getRozmiar() && odleglosc(Punkt(x,y),Punkt(it->getPozycjaX(),it->getPozycjaY())) > 0.01)//nie wejdzie na obiekt + sprawdzenie czy nie porownujemy z samym soba
				{
					if (!wektorPrzeciwny(Wektor(a.ruchX,a.ruchY),Punkt(e.getPozycjaX(),e.getPozycjaY()), Punkt(it->getPozycjaX(),it->getPozycjaY())))//pomaga gdy obiekty sa na sobie 
					{
						if (rekurencja)//wywolywane tylko za pierwszym razem (poprawione kierunki nie poprawiaja sie ponownie
						{
							Akcja nowa=a;//proba zmiany kierunku ruchu
							Wektor temp;//metode wywolamy na ten sam obiekt (&e) ze zmienionym kierunkiem ruchu (45 stopni w obie strony) i z parametren false (nie bedzie sie rekurenyjnie wywolywac w nieskonczonosc)
							temp.x=a.ruchX;
							temp.y=a.ruchY;
							temp=obrotWektora(temp,1.57);//1,57=pi/2
							nowa.ruchX=temp.x;
							nowa.ruchY=temp.y;
							if (!porusz(e,nowa,false))//jezeli sie nie uda to probujemy druga strone, jezeli sie uda to rekurencja poruszy obiektem (nie trzeba juz tego robic)
							{
								temp=obrotWektora(Wektor(a.ruchX,a.ruchY),-1.57);//druga strona
								nowa.ruchX=temp.x;
								nowa.ruchY=temp.y;
								if (!porusz(e,nowa,false))//jezeli sie nie uda w druga strone to wychodzimy z petli z odpowiednia flaga
								{
									mozna=false;
									break;
								}
								else
								{
									it->dotkniety((TypObiektu)e.getTyp());
									return true;//obiekt ma ustaiwiony ruch=true oraz zostal poruszony, wiec tylko zwracamy true z metody
								}
							}
							else
							{
								it->dotkniety((TypObiektu)e.getTyp());
								return true;
							}
						}
						mozna=false;
						break;
					}
				}
			}
		}
		if (mozna)
		{
			e.setPoruszaSie(true);
			e.zmienPozycjaX(a.ruchX*e.getPredkosc());
			e.zmienPozycjaY(a.ruchY*e.getPredkosc());
			return true;
		}
		else
		{
			e.setPoruszaSie(false);
			e.setKierunek(Kierunek::zaden);
			return false;
		}
	}
	else
	{
		e.setPoruszaSie(false);
		return false;
	}
}
void Silnik::usuwanieObiektow()
{
	list<int> usuniete;//po usunieciu obiektu indeks trafia na ta liste, zeby po przejsciu petli usunac go z listy doUsuniecia
	int licznikIndeksu=0;
	for (int i: doUsuniecia)
	{
		for (auto it=listaElementow.begin();it!=listaElementow.end();++it)
		{
			if (it->getId() == i)
			{
				for (auto a=listaPociskow.begin();a!=listaPociskow.end();++a)//usuniecie pociskow ktore leca do tego celu
				{
					if (a->getCel()->getId()==i)
					{
						pushDoUsuniecia(a->getId());
					}
				}
				usuniete.push_back(i);
				listaElementow.erase(it);
				break;
			}
		}
	}
	for (int i: doUsuniecia)
	{
		for (auto it=listaPociskow.begin();it!=listaPociskow.end();++it)
		{
			if (it->getId() == i)
			{
				usuniete.push_back(i);
				listaPociskow.erase(it);
				break;
			}
		}
	}
	for (int i : usuniete)
	{
		for (list<int>::iterator it=doUsuniecia.begin();it!=doUsuniecia.end();++it)
		{
			if (*it == i)
			{
				doUsuniecia.erase(it);
				break;
			}
		}
	}
}
void Silnik::pushDoUsuniecia(int _id)
{
	doUsuniecia.push_back(_id);
}
Element* Silnik::najblizszyPrzeciwnik(Punkt p)
{
	Element* wynik=NULL;
	float wartosc=10000000000000000;
	float temp;
	for (auto it=listaElementow.begin();it!=listaElementow.end();++it)
	{
		if (it->getTyp()==TypObiektu::przeciwnik)
		{
			temp=odleglosc(p, Punkt(it->getPozycjaX(), it->getPozycjaY()));
			if (temp < wartosc)
			{
				wynik=&(*it);
				wartosc=temp;
			}
		}
	}
	return wynik;
}
Element* Silnik::attackMove(Punkt poczatek, Punkt dokad, int zasieg)
	{
	Element* wynik=NULL;
	float wartosc=10000000000000000;
	float temp;
	for (auto it=listaElementow.begin();it!=listaElementow.end();++it)//szukamy najblizszego elementu
	{
		if (it->getTyp()==TypObiektu::przeciwnik)//ktory jest przeciwnikiem
		{
			temp=odleglosc(poczatek, Punkt(it->getPozycjaX(), it->getPozycjaY()));
			if (temp-it->getRozmiar() <= zasieg)//jezeli jest w zasiegu ataku to go akceptujemy
			{
				temp=odleglosc(dokad, Punkt(it->getPozycjaX(), it->getPozycjaY()));//szukamy elementu najblizszego kliknieciu, a nie obiektu (wiec ta wartosc porownujemy)
				if (temp < wartosc)
				{
					wynik=&(*it);
					wartosc=temp;
				}
				
			}
		}
	}
	return wynik;
}
void Silnik::nowaMapa()
{
	clearId();
	for (auto it=listaElementow.begin();it != listaElementow.end();++it)//wywalenie z listy wszystkiego poza graczem
	{
		if (it->getTyp() == TypObiektu::gracz)
		{
			Element temp=*it;
			listaElementow.clear();
			listaPociskow.clear();
			listaElementow.push_front(temp);
			getId();//przestawienie licznika id na 1 (0 ma gracz)
			break;
		}
	}
	doUsuniecia.clear();
	mapa.reset();//wyczyszczenie mapy ze starych infomracji
	dlugoscGryAktualna++;
	if (dlugoscGryAktualna>dlugoscGryMax)
	{
		wygrana=true;
		dlugoscGryAktualna--;
		return;
	}
	if (dlugoscGryAktualna==dlugoscGryMax)
	{
		mapTypeBoss();
	}
	else
	{
		int wylosowanyTyp=losuj(0,3);
		switch (wylosowanyTyp)
		{
		case 0:
			{
				mapType1(losuj(1,1));
				mapa.przeliczMape();
				//mapTypeBoss();
				break;
			}
		case 1:
			{
				mapType2(losuj(1,1));
				mapa.przeliczMape();
				//mapTypeBoss();
				break;
			}
		case 2:
			{
				mapType3(losuj(1,1));
				mapa.przeliczMape();
				//mapTypeBoss();
				break;
			}
		case 3:
			{
				mapType4(losuj(1,1));
				mapa.przeliczMape();
				//mapTypeBoss();
				break;
			}
		default:
			{
				break;
			}
		}
		TypPerka t=(TypPerka)losuj(0,3);
		dodajPerk(t, 100, 100);
	}
}
void Silnik::mapTypeBoss()
{
	int idTemp=getId();
	Element temp(&przeciwnikWidmo, 300, 300, TypObiektu(TypObiektu::przeciwnik), SiType::siPrzeciwnik, tickowNaRuch, idTemp);
	temp.setSkala(skalaGlobalna);
	temp.setRotacja(false);
	temp.setSkala(5);
	listaElementow.push_back(temp);
}
void Silnik::mapType1(int _ilosc)
{
	for (int i=0;i<losuj(1,7);i++)
	{
		Punkt temp(losuj(0,SCREEN_W),losuj(0, SCREEN_H));
		dodajPrzeciwnikWidmo(temp);
	}
	for (int i=0;i<_ilosc;i++)
	{
		Punkt temp;
		Punkt temp2;
		do
		{
			temp.x=losuj(0,SCREEN_W);
			temp.y=losuj(0,SCREEN_H);
			temp2=przeliczRozdzMapa2(temp.x, temp.y);
		}
		while (!mapa.poleWolne(temp2.x, temp2.y));
		dodajPrzeciwnik(temp);
	}
}
void Silnik::mapType2(int _ilosc)
{
	dodajPrzeszkoda(przeszkoda, 1,1);
	dodajPrzeszkoda(przeszkoda, 2,1);
	dodajPrzeszkoda(przeszkoda, 1,2);
	dodajPrzeszkoda(przeszkoda, 11,1);
	dodajPrzeszkoda(przeszkoda, 10,1);
	dodajPrzeszkoda(przeszkoda, 11,2);
	dodajPrzeszkoda(przeszkoda, 1,5);
	dodajPrzeszkoda(przeszkoda, 2,5);
	dodajPrzeszkoda(przeszkoda, 1,4);
	dodajPrzeszkoda(przeszkoda, 11,5);
	dodajPrzeszkoda(przeszkoda, 10,5);
	dodajPrzeszkoda(przeszkoda, 11,4);
	for (int i=0;i<losuj(1,5);i++)
	{
		Punkt temp(losuj(0,SCREEN_W),losuj(0, SCREEN_H));
		dodajPrzeciwnikWidmo(temp);
	}
	for (int i=0;i<_ilosc;i++)
	{
		Punkt temp;
		Punkt temp2;
		do
		{
			temp.x=losuj(0,SCREEN_W);
			temp.y=losuj(0,SCREEN_H);
			temp2=przeliczRozdzMapa2(temp.x, temp.y);
		}
		while (!mapa.poleWolne(temp2.x, temp2.y));
		dodajPrzeciwnik(temp);
	}
	dodajPrzeciwnikWieza(przeliczRozdzMapa(1, 3));
	dodajPrzeciwnikWieza(przeliczRozdzMapa(11, 3));
}
void Silnik::mapType3(int _ilosc)
{
	dodajPrzeszkoda(przeszkoda, 1,1);
	dodajPrzeszkoda(przeszkoda, 2,1);
	dodajPrzeszkoda(przeszkoda, 3,1);
	dodajPrzeszkoda(przeszkoda, 4,1);
	dodajPrzeszkoda(przeszkoda, 5,1);
	dodajPrzeszkoda(przeszkoda, 6,1);
	dodajPrzeszkoda(przeszkoda, 7,1);
	dodajPrzeszkoda(przeszkoda, 8,1);
	dodajPrzeszkoda(przeszkoda, 9,1);
	dodajPrzeszkoda(przeszkoda, 10,1);
	dodajPrzeszkoda(przeszkoda, 11,1);
	dodajPrzeszkoda(przeszkoda, 1,5);
	dodajPrzeszkoda(przeszkoda, 2,5);
	dodajPrzeszkoda(przeszkoda, 3,5);
	dodajPrzeszkoda(przeszkoda, 4,5);
	dodajPrzeszkoda(przeszkoda, 5,5);
	dodajPrzeszkoda(przeszkoda, 6,5);
	dodajPrzeszkoda(przeszkoda, 7,5);
	dodajPrzeszkoda(przeszkoda, 8,5);
	dodajPrzeszkoda(przeszkoda, 9,5);
	dodajPrzeszkoda(przeszkoda, 10,5);
	dodajPrzeszkoda(przeszkoda, 11,5);
	dodajPrzeszkoda(przeszkoda, 1,2);
	dodajPrzeszkoda(przeszkoda, 1,4);
	dodajPrzeszkoda(przeszkoda, 11,2);
	dodajPrzeszkoda(przeszkoda, 11,4);
	for (int i=0;i<losuj(1,5);i++)
	{
		Punkt temp(losuj(0,SCREEN_W),losuj(0, SCREEN_H));
		dodajPrzeciwnikWidmo(temp);
	}
	for (int i=0;i<_ilosc;i++)
	{
		Punkt temp;
		Punkt temp2;
		do
		{
			temp.x=losuj(0,SCREEN_W);
			temp.y=losuj(0,SCREEN_H);
			temp2=przeliczRozdzMapa2(temp.x, temp.y);
		}
		while (!mapa.poleWolne(temp2.x, temp2.y));
		dodajPrzeciwnik(temp);
	}
	dodajPrzeciwnikWieza(przeliczRozdzMapa(1, 3));
	dodajPrzeciwnikWieza(przeliczRozdzMapa(11, 3));
}
void Silnik::mapType4(int _ilosc)
{
	dodajPrzeciwnikWieza(przeliczRozdzMapa(11, 3));
}
int Silnik::losuj(int _min, int _max)
{
	return (std::rand() % (_max-_min+1) ) + _min;
}
bool Silnik::endLevel()
{
	bool koniec=true;
	if (endLevelItem==false)
	{
		for (auto it=listaElementow.begin();it!=listaElementow.end();++it)
		{
			if (it->getTyp()==TypObiektu::przeciwnik) 
			{
				koniec=false;
				break;
			}
		}
		if (koniec==true)
		{
			dodajEndLevelItem(12, 6);
			endLevelItem=true;
		}
	}
	koniec=false;
	if (endLevelItem==true)
	{
		for (auto it=listaElementow.begin();it!=listaElementow.end();++it)
		{
			if (it->getTyp()==TypObiektu::endLevelItem) 
			{
				auto gracz=listaElementow.begin();
				if (odleglosc(Punkt(it->getPozycjaX(), it->getPozycjaY()),Punkt(gracz->getPozycjaX(), gracz->getPozycjaY())) < (it->getRozmiar()+gracz->getRozmiar()+10))
				{
					koniec=true;
					endLevelItem=false;
				}
				else koniec=false;
				break;
			}
		}
	}
	return koniec;
}
void Silnik::dodajEndLevelItem(int x, int y)
{
	int idTemp=getId();
	Punkt temp=przeliczRozdzMapa(x,y);
	//al_lock_mutex(mutexLista);
	Element temp2(&przeszkoda,temp.x,temp.y,TypObiektu(TypObiektu::endLevelItem),1,0, idTemp);
	temp2.setSkala(skalaGlobalna);
	temp2.setRozmiar(20);
	listaElementow.push_back(temp2);
	//al_unlock_mutex(mutexLista);
	//mutexy.push_back(Mutexy());
	//mutexy.back().id=idTemp;
	//mapa.dodajObiekt(Mapa::Obiekt(x,y));
}
void Silnik::osbslugaPerkow()
{
	auto gracz=listaElementow.begin();
	for (auto it=listaPerkow.begin();it!=listaPerkow.end();)
	{
		if (odleglosc(gracz->getPozycja(), Punkt(it->pozycjaX, it->pozycjaY)) < 30)
		{
			switch (it->typ)
			{
			case TypPerka::szybkosc:
				{
					gracz->addBuffSzybkoscAtaku(1);
					it->remove=true;
					it=listaPerkow.erase(it);
					break;
				}
			case TypPerka::zasieg:
				{
					gracz->addBuffZasieg(1);
					it->remove=true;
					it=listaPerkow.erase(it);
					break;
				}
			case TypPerka::hp:
				{
					gracz->addBuffHP(1);
					it->remove=true;
					it=listaPerkow.erase(it);
					break;
				}
			case TypPerka::obrazenia:
				{
					gracz->addBuffObrazenia(1);
					it->remove=true;
					it=listaPerkow.erase(it);
					break;
				}
			default:
				{
					break;
				}
			}
		}
		else
		{
			++it;
		}
	}
}
void Silnik::dodajPerk(TypPerka t, int x, int y)
{
	listaPerkow.push_front(Perk(x,y,t));
}
void Silnik::wyswietlPerki()
{
	for (auto it=listaPerkow.begin();it!=listaPerkow.end();++it)
	{
			switch (it->typ)
			{
			case TypPerka::szybkosc:
				{
					al_draw_scaled_bitmap(perkYellow, 0, 0, al_get_bitmap_width(perkYellow), al_get_bitmap_height(perkYellow), it->pozycjaX, it->pozycjaY, al_get_bitmap_width(perkYellow), al_get_bitmap_height(perkYellow), 0);
					break;
				}
			case TypPerka::zasieg:
				{
					al_draw_scaled_bitmap(perkBlue, 0, 0, al_get_bitmap_width(perkBlue), al_get_bitmap_height(perkBlue), it->pozycjaX, it->pozycjaY, al_get_bitmap_width(perkBlue), al_get_bitmap_height(perkBlue), 0);
					break;
				}
			case TypPerka::hp:
				{
					al_draw_scaled_bitmap(perkGreen, 0, 0, al_get_bitmap_width(perkGreen), al_get_bitmap_height(perkGreen), it->pozycjaX, it->pozycjaY, al_get_bitmap_width(perkGreen), al_get_bitmap_height(perkGreen), 0);
					break;
				}
			case TypPerka::obrazenia:
				{
					al_draw_scaled_bitmap(perkRed, 0, 0, al_get_bitmap_width(perkRed), al_get_bitmap_height(perkRed), it->pozycjaX, it->pozycjaY, al_get_bitmap_width(perkRed), al_get_bitmap_height(perkRed), 0);
					break;
				}
			default:
				{
					break;
				}
			}
	}
}
void Silnik::wyswietlGUI()
{
	auto temp=listaElementow.front();
	al_draw_textf(font, al_map_rgb(0,0,0), 10, SCREEN_H-30, ALLEGRO_ALIGN_LEFT, "Szybkosc ataku: %d", temp.getSzybkoscAtaku());
	al_draw_textf(font, al_map_rgb(0,0,0), 10, SCREEN_H-50, ALLEGRO_ALIGN_LEFT, "Buff obrazenia: %d", temp.getObrazenia());
	al_draw_textf(font, al_map_rgb(0,0,0), 10, SCREEN_H-70, ALLEGRO_ALIGN_LEFT, "HP: %d", temp.getHpAktualne());
	al_draw_textf(font, al_map_rgb(0,0,0), 10, SCREEN_H-90, ALLEGRO_ALIGN_LEFT, "Zasieg: %d", temp.getZasieg());
}
void Silnik::obliczKierunek(Element &e, float x, float y)
{
	if (x==y && x==0.0) e.setKierunek(Kierunek::zaden);
	else
	{
		if (x >= 0)
		{
			if (y>=0)
			{
				if (abs(x)>=abs(y))
				{
					e.setKierunek(Kierunek::prawo);
				}
				else
				{
					e.setKierunek(Kierunek::dol);
				}
			}
			else
			{
				if (abs(x)>=abs(y))
				{
					e.setKierunek(Kierunek::prawo);
				}
				else
				{
					e.setKierunek(Kierunek::gora);
				}
			}
		}
		else
		{
			if (y>=0)
			{
				if (abs(x)>=abs(y))
				{
					e.setKierunek(Kierunek::lewo);
				}
				else
				{
					e.setKierunek(Kierunek::dol);
				}
			}
			else
			{
				if (abs(x)>=abs(y))
				{
					e.setKierunek(Kierunek::lewo);
				}
				else
				{
					e.setKierunek(Kierunek::gora);
				}
			}
		}
	}
}
void Silnik::wyswietlWygrana()
{
	al_draw_textf(font, al_map_rgb(0,0,0), 300, SCREEN_H/2, ALLEGRO_ALIGN_LEFT, "GRATULACJE! Wygrales gre!");
	al_draw_textf(font, al_map_rgb(0,0,0), 10, SCREEN_H-50, ALLEGRO_ALIGN_LEFT, "nacisnij ENTER aby zagrac jeszcze raz");
}
void Silnik::wyswietlPrzegrana()
{
	al_draw_textf(font, al_map_rgb(0,0,0), 300, SCREEN_H/2, ALLEGRO_ALIGN_LEFT, "Przegrana :(");
	al_draw_textf(font, al_map_rgb(0,0,0), 10, SCREEN_H-50, ALLEGRO_ALIGN_LEFT, "nacisnij ENTER aby zagrac jeszcze raz");
}
void Silnik::restart()
{
	dlugoscGryMax=losuj(5,10);
	dlugoscGryAktualna=-1;
	nowaMapa();
	Element &e=listaElementow.front();
	e.restartHP();
	wygrana=false;
	przegrana=false;
	endLevelItem=false;
}
void *Func_Thread(ALLEGRO_THREAD *thr, void *arg)
{
	/*Silnik::argumentWatek * a= (Silnik::argumentWatek*) arg;
	int i=0;
	while (true)
	{
		if (a->koniec==true)//flaga konca programu
		{
			return NULL;
		}
		
		try
		{
			al_lock_mutex(a->mut);
			int wielkosc = listaElementow.size();
			fprintf(stderr, "\n\namount: %i\n", wielkosc);
			lista.clear();
			i=0;
			concurrent_vector <Element> *wsk;
			wsk=&listaElementow;
			fprintf(stderr, "adress list1: %x (size: %i)\n", &(listaElementow), listaElementow.size());
			fprintf(stderr, "adress list2: %x (size: %i)\n", &(lista), lista.size());
			auto it=listaElementow.begin();
			fprintf(stderr, "iterator address: %i\n", &it);
			for (;it != listaElementow.end();++it)
			{
				fprintf(stderr, "[%i] ", i);
				fprintf(stderr, "iterator pointing: %x ", &(*it));
				fprintf(stderr, "id: %i ", it->getId());
				lista.push_back(*it);
				fprintf(stderr, " ok\n");
				i++;
			}
			al_unlock_mutex(a->mut);
		}
		catch(exception e)
		{
			i++;
		}
		
		for (auto obiekt : lista)
		{
			if (obiekt.getStan().potrzebnePrzeliczenieDrogi==true)
			{
				list<Punkt> wynik;
				float x,y;
				switch (obiekt.getSiType())
				{
				case 0:
					x=a->bohaterPozycjaDocelowaX;
					y=a->bohaterPozycjaDocelowaY;
					break;
				case 2:
					for (concurrent_vector<Element>::iterator it=a->lista.begin();it!=a->lista.end();++it)
					{
						if (it->getTyp() == TypObiektu::gracz)
						{
							x=it->getPozycjaX();
							y=it->getPozycjaY();
							break;
						}
					}
					break;
				default:
					//return NULL;//jezeli typ si jest nieustalony to moze byc to przeszkoda (nie ma si)
					break;
				}
				Punkt punkt1=przeliczRozdzMapy(obiekt.getPozycjaX(), obiekt.getPozycjaY(), 640, 480, 13, 7);
				Punkt punkt2=przeliczRozdzMapy(x, y, 640, 480, 13, 7);
				a->mapa.droga(punkt1, punkt2, wynik);//utworzenie nowej drogi
				al_lock_mutex(obiekt.mutex);
				if (obiekt.getStan().potrzebnePrzeliczenieDrogi==true)//nie zakladano mutexa podczas pierwszego sprawdzenia, wiec teoretycznie cos sie moglo zmienic; upewniamy sie, ze warunek jest aktualny
				{
					obiekt.setListaDroga(wynik);//ustawienie nowej drogi
					obiekt.setPotrzebnePrzeliczenieDrogi(false);
					obiekt.setDrogaZostalaPrzeliczona(true);
				}
				al_unlock_mutex(obiekt.mutex);
			}
		}
	}*/
	return NULL;
}