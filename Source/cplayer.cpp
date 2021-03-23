/* class player - Methodendefinition
Project:		Schiffeversenken Advanced
Author:			Christoph Clouser
Description:	Diese Klasse definiert die Spieler mit Schiffen und Methoden zur Eingabe und zum Zeichnen von Schiffen, Treffer und Spielernamen.
				Regelkonformes Platzieren zwischen Schiffen wird ebenfalls kontrolliert.

Changelog/Version:
0.1 File created, to-dos
0.2 _nr,_isalive,_canmove
0.3 get/Set Methoden
0.4 Print Methode
0.5 draw-ships Methode, mit BMPs erweitert
0.6 draw Hit, input Mensch
0.7 Test auf regelkonforme Schiffsplatzierung 
0.8 Fehler behoben: draw_blank zeichnete fuer zerstoerer falsches BMP
0.9 
*/
#include "stdafx.h"
#include "cplayer.h"

// Konstruktor:
player::player(char inr)
{
	this->set_nr(inr);									// Setze Spielernr
	this->player::_ships.resize(MAX);					// Vektor wird fuer Standard-Spiel, 10 Schiffe, gesetzt
	this->player::_nogo.resize(LASTHITS);
	//this->player::_hits.resize(0,"00");						// Alle Trefferpunkte der 10 Schiffe
	this->player::create_ships();						// Erstelle Schiffe
	this->player::set_isalive(true);					// Neues Spiel, neues Glueck
}
	
/* Set-Methoden:
set_nr: setzt Spielernummer
set_isalive: setzt Verliererflag
set_name: Setzt Spielername
set_canmove: setzt ob Spieler noch Schiffe bewegen darf (mehr als 2 hat)
*/
bool player::set_nr(char inr)
{
	if ((inr == P1) || (inr == P2))
	{
		_nr = inr;
		this->player::set_name();
		return true;
	}
	else
	{
		std::cout << "Fehler: Spielernummer ungueltig!";
		return false;
	}
}
void player::set_canmove(bool ival)
{
	_canmove = ival;
}
void player::set_isalive(bool ival)
{
	_isalive = ival;
}
void player::set_name()
{
	char nr = this->player::get_nr();
	if (nr == P1)
		this->set_name(NAME_PLAYER1);
	else
		this->set_name(NAME_PLAYER2);
	this->draw_name();
}
bool player::set_name(std::string iname)
{
	if (iname != "")
	{
		_name = iname;
		return true;
	}
	else
	{
		std::cout << "Fehler: der Name darf nicht leer sein!";
		return false;
	}
}
void player::set_nogo(std::string icoordinates)
{	// Baut _nogo Koordinatenvektor auf, der bereits getroffene Koordinaten enthaelt: 0 ist most recent hit, 2 der ablaufende Treffer
	int length, index;
	std::string safe_lasthit, safe_currenthit;
	this->player::_hits.push_back(icoordinates);		// Koordinaten schreiben, Vektor vergroessern
	safe_currenthit = icoordinates;						// Speichere den gerade gemachten Hit
	safe_lasthit = this->player::_nogo[0];
	this->player::_nogo[2] = this->player::_nogo[1];	// dann schiebe die Hits durch
	this->player::_nogo[1] = safe_lasthit;
	this->player::_nogo[0] = safe_currenthit;			// sodass der neuste an Pos 0 steht
}
/* Testfunktionen:
check_hit(string coor): 
in: Koordinatenpaar als String
parse:testet ob ein Treffer bei den eingehenden Koordinaten mit einem Schiff vorliegt, setzt Treffer im Treffervektor des Schiffs, testet ob Schiff oder Spieler besiegt und zeichnet Treffer
out: true/false

check_coor(string icoor):
in: Koordinatenpaar als String 
parse: testet Koordinaten gegen Treffervektor
out: true/false

check_isalive(): 
in: -
parse: testet ob Spieler noch Schiffe bewegen darf oder besiegt ist, setzt flags
out: true/false

positioncheck(x,y,ori,nrships): 
in: char: x Koordinate, y Koordinate, Ausrichtung; int: Index des Schiffes
parse: testet ob Position regelkonform, vergleicht Umkreis des Schiffs mit bereits gesetzten Schiffspositionen
out: true/false

poisitoncheck(nrship,action): bugged, nicht benutzt
*/
bool player::check_hit(std::string icoordinates)
{
	int i,ii, type,vergleich;
	char x, y;
	ships *ptr_ships;
	std::string coor;
	coor = icoordinates;
	ptr_ships = this->player::get_shipsvector();
	if (algo.sad::get_stopmuster())							// Falls Mustersequenz zu einem Treffer gefuehrt hat
	{														// wird SaD Algorithmus ausgefuehrt, bis Algo eine 
		coor = algo.sad::sad_core();						// Abbruchsbedingung erreicht und wieder zum Muster zurueckkehrt
	}
	if (!(coor.compare("00")))
	{
		coor = icoordinates;
	}
	x = coor[0];
	y = coor[1];
	for (i = 0; i < MAX; i++)								// Durchlaeuft den Schiffsvektor des Spielers
	{
		type = (ptr_ships + i)->ships::get_type() - '0';	// Typ mit Offsetkorrektur
		for (ii = 0; ii < type; ii++)						// Durchlaeuft Koordinatenstring eines Schiffes
		{
			vergleich = coor.compare(*((ptr_ships + i)->ships::get_strposition() + ii));	// Vergleicht Angriffskoordinate mit Schiffskoordinate
			if (!vergleich)									// str.compare liefert 0 bei Uebereinstimmung
			{
				std::cout << "Treffer!" << std::endl;		// wurde getroffen
				if (algo.sad::get_stopmuster())
				{
					algo.sad::set_hitflag(true);
				}
				if ((ptr_ships + i)->ships::set_hit(x, y))		// setzte Treffer und teste ob Schiff bereits zerstoert 
				{												// Schiff ist noch nicht zerstoert, also algo starten
					this->player::set_nogo(coor);				// Speichert die letzten drei hits
					this->player::draw_hits();					// Treffer zeichnen
					wait(5);									// Warte bis alle Treffer gezeichnet
					this->player::check_isalive();				// letzter Hit? Spieler besiegt?
					algo.sad::set_stopmuster(true);				// Starte sad Algo
					algo.sad::set_lasthit(coor);				// Uebergebe letzten Treffer an sad Algo
				}
				else											// Schiff bereits zerstoert, also 
				{
					algo.sad::set_hitflag(false);				// Algo-Flag aus
					return false;								// raus
				}
				return true;									// erfolgreicher Treffer, also raus
			}
		}
	}
	std::cout << "Wasser!" << std::endl;					// Kein Schiff getroffen
	algo.sad::set_hitflag(false);							// Algo-Flag aus
	return false;											// Kein Treffer
}
bool player::check_coor(std::string icoor)
{
	int i, ii, size, vergleich;
	std::string coor;
	const std::string *ptr_hits;
	ptr_hits = this->player::get_hitsr();
	size = this->player::_hits.size();
	coor = icoor;
	for(i = 0; i < size; i++)						// Vergleiche Koordinate mit allen Trefferkoordinaten
	{
		vergleich = coor.compare(*(ptr_hits + i));
		if (vergleich == 0)							// Ist die Koordinate gleich einer Trefferkoordinate
		{
			return true;							// return true fuer schon mal genutzt
		}
	}
	return false;									// ansonsten wurde noch nicht getroffen 
}
bool player::check_isalive()
{
	int i, ships_alive = MAX;
	const ships *ptr_ships;
	ptr_ships = this->player::get_ships();
	for (i = 0; i < MAX; i++)							// Gehe Schiffsvektor durch
	{
		if (!(ptr_ships + i)->ships::get_isalive())		// Wieviele Schiffe sind noch _alive?
			ships_alive = ships_alive - 1;				// Wenn nicht, dann ships_alive reduzieren
	}
	if (ships_alive > 2)								// Sind mehr als 2 _alive
	{
		this->player::set_canmove(true);				// Bewegung erlaubt
		this->player::set_isalive(true);				// Spieler _alive
		return true;
	}
	else
	{
		this->player::set_canmove(false);				// 2 oder weniger, dann Bewegung nicht erlaubt
		if (!(ships_alive == 0))						
			return true;
		else
		{
			this->player::set_isalive(false);			// 0 Schiffe uebrig -> Spieler besiegt
			return false;
		}
	}
}
bool player::positioncheck(char ix, char iy, char iorientation, int inrships)	// inrships ist Index des zpS und Anzahl der gesetzten Schiffe
{	// zu platzierenden Schiffs (zpS)
	// bereits platzierte Schiffe (bpS)
	int i, ii, iii, size_toplace, type_placed, vergleich;
	std::vector<std::string> v_shiptoplace;									// beinhaltet alle Koordinaten des zu platzierenden Schiffs (zpS)
	ships *ptr_ships = this->get_shipsvector();
	size_toplace = (((((ptr_ships + (inrships))->get_type()) - '0') + 2) * 3);		// Typ des zu setzenden Schiffs - Offset + 2 Kaestchen mal 3 ergibt Anzal an Kaestchen des Schiffs
	v_shiptoplace.resize(size_toplace, "00");											// Vektor initialisieren, abhaegig vom Typ des zpS
	v_shiptoplace = ptr_ships[inrships].create_strposradius(ix, iy, iorientation);		// Erstellt Vektorstring mit allen Koordinaten des zpS
																						//for (i = 0; i < size_toplace; i++)		// kontrolle, Ausgabe fuer zpS
																						//	std::cout << "In str_position: " << v_shiptoplace[i]<< std::endl;
	for (i = 0; i < MAX; i++)										// Durchlaeuft Schiffsvektor des Spielers, also bpS werden nur betrachtet
	{
		if (i == inrships)
		{
			i++;
		}
		else;
		for (ii = 0; ii < size_toplace; ii++)							// Durchlaeuft Koordinatenstring mit Umkreis des zpS
		{
			type_placed = (((ptr_ships + i)->get_type()) - '0');	// aktualisiert den Typ des bpS fuer die naechste Schleife
			for (iii = 0; iii <type_placed; iii++)						// Durchlaeuft Koordinatenstring ohne Umkreis der bpS
			{
				vergleich = (v_shiptoplace[ii].compare(*(((ptr_ships + i)->get_strposition()) + iii))); // Vergleich von Umkreis des zpS mit Position des bpS
				if (vergleich == 0)
					return false;							// Gibt es eine Uebereinstimmung einer Koordinate, so ist die Platzierung regelwidrig
			}
		}
	}
	return true;
}
bool player::positioncheck(int inrship, char iaction)	// inrshop = Schiffsindex(0-9), iaction = NORD/SUED/WEST/OST oder UZ/GZ 
{
	// zu platzierenden Schiffs (zpS)
	// bereits platzierte Schiffe (bpS)
	int i, ii, iii, size_toplace, type_placed, vergleich1=1, vergleich2=1, nrship;
	char x, y, orientation, action;
	bool vergleich2_erfolgt=false;
	std::vector<std::string> v_shiptoplace;											// beinhaltet alle Koordinaten des zu platzierenden Schiffs (zpS)
	std::string str_coor = "000", str_vergl = "00";
	const std::string *ptr_posvec;
	ships *ptr_ships = this->player::get_shipsvector();								// Zeiger auf Schiffsvektor
	nrship = inrship;
	action = iaction;
	size_toplace = (((((ptr_ships + nrship)->ships::get_type()) - '0') + 2) * 3);	// Typ des zu setzenden Schiffs - Offset + 2 Kaestchen mal 3 ergibt Anzal an Kaestchen des Schiffs
	x = *((ptr_ships + nrship)->ships::get_position());								// Hole momentane Koordinaten
	y = *((ptr_ships + nrship)->ships::get_position()+1);
	// Check ob noch auf Karte:
	if (!((ptr_ships + nrship)->ships::boundcheck_action(x, y, action)))			// Vergleich ob neue Position noch auf Karte
		return false;
	// Berechnung neuer position:
	str_coor = ptr_ships->ships::create_newposition(x, y, action);					// Berechnet neue Koordinaten durch Aktion
	if (!(str_coor.compare("000")))													// Abfang, falls Koordinaten durch Aktion ungueltig
		return false;
	x = str_coor[0];																// Ansonsten zuweisen
	y = str_coor[1];
	orientation = str_coor[2];
	v_shiptoplace.resize(size_toplace, "00");												// Vektor initialisieren, abhaegig vom Typ des zpS
	v_shiptoplace = ptr_ships[nrship].ships::create_strposradius(x, y, orientation);		// Erstellt Vektorstring mit allen neuen Koordinaten des zpS und Umkreis
																							// Check ob neue Position mit Schiffen kollidiert:									
	for (i = 0; i < MAX; i++)											// Durchlaeuft Schiffsvektor des Spielers, also bpS werden nur betrachtet
	{
		if (i == nrship)
		{
			i++;
		}
		ptr_posvec = ptr_ships[i].ships::get_strposition();				// Zeiger auf Koordinatenstring eines bpS mit Index i
		type_placed = (((ptr_ships + i)->ships::get_type()) - '0');		// aktualisiert den Typ des bpS fuer die naechste Schleife
		for (ii = 0; ii < size_toplace; ii++)							// Durchlaeuft Koordinatenstring mit Umkreis des zpS
		{
			for (iii = 0; iii < type_placed; iii++)						// Durchlaeuft Koordinatenstring ohne Umkreis der bpS
			{
				str_vergl = *(ptr_posvec + iii);						// Hole naechsten Koordinatenpaarstring (ohne Umkreis)
				vergleich1 = (v_shiptoplace[ii].compare(str_vergl));	// Vergleich von Umkreis des zpS mit Position des bpS
				if ((!vergleich2_erfolgt) && (iii<3))
				{
					vergleich2 = (v_shiptoplace[ii].compare(*(this->player::get_nogo() + iii)));		// Vergleich mit den drei zuletzt getroffenen Feldern
					if ((ii == (size_toplace-1)) && (iii == 2))												// Test muss nur einmalig fur alle Pos mit Umfang gegen die letzten drei Treffer erfolgen
						vergleich2_erfolgt = true;	
				}
				if (vergleich1 == 0 || vergleich2 == 0)
				{
					return false;
				}											// regelwidrig bei Uebereinstimmung einer der Koordinaten
			}
		}
	}
	return true;											// Keine Uebereinstimmung gefunden, darf platziert werden
}
/* Eingabe:
input(): 
in: int ioffset: Offset für Abfrage in Datei
parse: liest Koordinaten aus Datei zur Platzierung der Schiffe, testet auf regelkonforme Lage
out: -

input_action(): Eingabe fuer Aktion nach Angriff, nimmt eine Nummer auf, die zur generierung eines pseudo-zufaelligen Bewegungsmuster dient
in: int iround: Index zur Auswahl eines Schiffes oder Seed, wenn zufällig
parse: wählt Schiff und Aktion aus und testet ob Schiff durch Aktion bewegt werden kann
out: true/false

get_input(std::string ifilename, int ipos): liest Datei ifilename ab ipos ein, gibt Koordinatenpaar (Bsp: 1A) zurueck
in: std::string ifilename, int ipos: Dateiname, die auszulesen ist, Offset ab dem auszulesen ist
parse: liest aus Datei ab Offset Koordinatenpaar ein
out: string: Koordinatenpaar-string

get_input(std::string ifilename, int ipos, int ival): (nicht benutzt)
in: std::string ifilename, int ipos: Dateiname, die auszulesen ist, Offset ab dem auszulesen ist; ival = 0: nur Koordinatenpaar, ival=1:Koordinatenpaar mit Himmelsrichtung
parse: liest aus Datei ab Offset Koordinatenpaar ein, mit und ohne Himmelsrichtung
out: string: Koordinatenpaar-string
*/
void player::input(int ioffset)
{
	int i,ii,iii, startposition_offset;
	char x, y, orientation, type;
	std::vector<std::string> ship_names = {"NONE","Schlachtschiff","Kreuzer","Zerstörer","Uboot"};
	std::string *ptr_names = ship_names.data();
	std::string str_input = "000";
	ships *ptr_ships;
	ptr_ships = this->player::get_shipsvector();						// Gibt R/W Referenz auf Shipsvektor des Spielers
	startposition_offset = ioffset;
	for (i = 0, ii=1, iii= 0; i < MAX; i++)
	{
		while (true)
		{
			//std::cout << std::endl;
			//std::cout << *(ptr_names + ii) << ":" << std::endl
			//	<< "Bitte Position(X,Y) und Ausrichtung(n,s,w,o) eingeben (Bsp: 5In): ";
			//std::cin >> x >> y >> orientation;							// Manuelle Eingabe 
			str_input = get_input(FILENAME_START, startposition_offset + iii, 1);		// Hol Koordinaten: aus Datei, ab Posoffset, mit Himmelsrichtung
			x = str_input[0];
			y = str_input[1];
			orientation = str_input[2];
			//std::cout << x << y << orientation << std::endl;
			// Kontrollabfrage fuer Eingabe:
			if ((x >= '0') && (x <= '9') && ((y >= 'A') && (y <= 'J')) && (orientation == 'n' || orientation == 's' || orientation == 'w' || orientation == 'o'))
			{
				if ((*(ptr_ships + i)).boundcheck(x, y, orientation) && this->player::positioncheck(x, y, orientation, i)) // Schiffe noch auf Karte? Regelkonform?
				{
					break;	// Alles ok, dann raus aus while(1);
				}
				else
				{
					if (iii < 200)
						iii++;		// Hole naechsten Wert aus Input_File
					else
						iii = 0;
					// Nur manuelle Eingabe:
					//std::cout << "Ungueltige Schiffsplatzierung!" << std::endl;
					//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');	// Stream bis \n ignorieren -> Stream sauber
					//std::cin.clear();											// Flag loeschen -> Einlesen sauber
				}
			}
			else
			{		
				if (iii < 200)
					iii++;		// Hole naechsten Wert aus Input_File
				else
					iii = 0;
				// Nur manuelle Eingabe:
				//std::cout << "Ungueltige Eingabe! Beispiel: 0As" << std::endl;
				//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');	// Stream bis \n ignorieren -> Stream sauber
				//std::cin.clear();											// Flag loeschen -> Einlesen sauber
			}
		}
		(ptr_ships + i)->ships::set_orientation(orientation);		// Ausrichtung wird uebernommen
		(ptr_ships + i)->ships::set_position(x, y);					// Koordinaten werden berechnet und gesetzt
		type=(ptr_ships + i)->ships::get_type();					// Typ wird abgefragt
		std::cout << *(ptr_names + ii) << "(" << i << ")" << std::endl
			<< "Koordinaten: " << x << y << orientation << std::endl << std::endl;
		this->player::draw(x,y,type,orientation);					// fuer diesen Spieler wird dieses Schiff direkt gezeichnet
		updatescr();
		switch (i)											// 1 Schlachtschiff, 2 Kreuzer, 3 Zerstoerer, 4 Uboote; also entsprechend ii hochzaehlen
		{
		case 0:
			ii++;
			break;
		case 2:
			ii++;
			break;
		case 5:
			ii++;
			break;
		default:
			;
		}
	}
	//this->draw_ships();										// Zur Sicherheit am Ende nochmal alle Schiffe zeichnen, ist nicht notwendig
}
bool player::input_action(int iround)
{
	int nrship, ii, fail = 1, round;
	bool vergleich, _alive, once = true;
	char x, a, y, action[6] = { NORD,SUED,OST,WEST,UZ,GZ }, orientation, type, drawblank_type, ori_old, x_old, y_old;
	std::string str_input = "0";
	std::string str_coor = "000";
	ships *ptr_ships;
	ptr_ships = this->player::get_shipsvector();						// Gibt R/W Referenz auf Shipsvektor des Spielers
	if (MUST_TAKE_ACTION)												// Spieler soll nicht auf Action verzichten
	{
		nrship = iround % MAX;												// dann werden alle Schiffe durchgetestet (einmal)
	}
	else																// Ansonsten ein Schiff zufaellig auswaehlen
	{
		round = iround;					
		srand(round);													// Neuer Seed um zufaellig zu bewegendes Schiff auszuwaehlen
		nrship = rand() % MAX;											// Random Schiff wird gewaehlt
	}
	do {
		_alive = (ptr_ships + nrship)->ships::get_isalive();
		if (!_alive)													// Ist Schiff noch am Leben?
		{
			if ((nrship == 9) && once)
				once = false;
			else
				return false;
			nrship = (nrship + 1) % MAX;
		}
		else;
	} while (!_alive);
	x_old = *((ptr_ships + nrship)->ships::get_position());				// Momentane Position des Schiffs wird gespeichert
	y_old = *((ptr_ships + nrship)->ships::get_position() + 1);
	ori_old = (ptr_ships + nrship)->ships::get_orientation();			// Momentane Ausrichtung
	type = (ptr_ships + nrship)->ships::get_type();						// Schiffstyp
																		// Neue Orientation (nur bei Drehung anders)
	for (ii = 0; ii < 5; ii++)											// Durchlaufe alle moeglichen Aktionen
	{
		//std::cout << "Aktion eingeben: Drehung u/g, Bewegung n/s/w/o: ";
		//str_input = get_input(FILENAME_ACTION, ACTIONOFFSET + ii, 1);		// Hol Aktion aus Datei: ab Posoffset, Bewegung oder Drehung
		//= str_input[0];
		//std::cout << action << std::endl;
		a = action[ii];														// Waehle eine moegliche Aktion
		str_coor = (ptr_ships + nrship)->ships::create_newposition(x_old, y_old, a);		// Berechnet neue Koordinaten und Ausrichtung die durch Aktion entstehen
		x = str_coor[0];													// Neue Koordinaten werden gespeichert (abhaengig von auszufuehrender Aktion) 
		y = str_coor[1];
		orientation = str_coor[2];
		if (orientation == '0');
		else
		{
			vergleich = this->player::positioncheck(x, y, orientation, nrship);		// Fuehre einen Positionscheck durch, ob neue Position regelkonform
			if (vergleich)
			{
				fail = 0;										// Neue Position ist regelkonform
				break;											// Alles ok, dann raus;
			}
			else;												// Neue Position nicht regelkonform -> fail = 1 bleibt;
		}
	}
	if (!(fail))												// Nur wenn regelkonform:
	{
		std::cout << "Neue Pos!" << std::endl;
		(ptr_ships + nrship)->ships::set_orientation(orientation);				// Setze neue/alte Ausrichtung des Schiffs
		vergleich = (ptr_ships + nrship)->ships::set_position(x, y);
		if (vergleich)															// Setze neue Koordinaten des Schiffs
		{
			std::cout << "Old: " << x_old << y_old << ori_old << std::endl
				<< "New: " << x << y << orientation << std::endl;
			if (ori_old == a)
				drawblank_type = '1';
			else
				drawblank_type = type;
			this->player::draw_blank(x_old, y_old, drawblank_type, ori_old);	// Ueberschreibt alte Position
			wait(5);
			this->player::draw(x, y, type, orientation);						// Zeichnet Schiff auf neuer Position
			wait(5);
		}
		else
		{
			(ptr_ships + nrship)->ships::set_orientation(ori_old);		// Setze alte Ausrichtung des Schiffs, falls Positiontests in set_position fehlschlagen
			return false;
		}
	}
	else
		return false;
	return true;
}
std::string player::get_input(std::string ifilename, int ipos)	// Dateiname, ipos = startposition in Datei
{
	int pos;
	pos = ipos % (2*FIELDLENGTH);
	std::string stringc = "00";
	char *ptr_buffer;
	ptr_buffer = new char[2];						// Koordinatenpaar
	std::fstream file_start;						// File-Stream zum Lesen definiert
	file_start.open(ifilename, std::ios::in);		// Datei wird geoeffnet, lesen
	if (file_start.good())							// Filestream wird geprueft
	{
		file_start.seekg(pos);						// Aus Datei muss immer erst eine Zahl gelesen werden -> immer 2. Position -> 2*pos immer gerade
		file_start.read(ptr_buffer, 2);
	}
	else
		std::cout << "Fehler: Datei " << ifilename << " konnte nicht geoeffnet werden!" << std::endl;
	file_start.close();								// Datei schliesen
	stringc[0] = *ptr_buffer;
	stringc[1] = *(ptr_buffer + 1);
	//delete[] ptr_buffer; // Fuehrt zu einer Fehlermeldung?!
	return stringc;
}
std::string player::get_input(std::string ifilename, int ipos, int ival)	// Dateiname, Nr der  Aufstellung (0-9), ival: 1 -> mit Orientation
{
	int r, pos;	
	char *ptr_buffer, *ptr_ori;
	std::string stringc = "000";
	ptr_ori = new char[4];
	ptr_buffer = new char[2];						// Koordinatenpaar
	pos = ipos % FIELDLENGTH;
	std::fstream file_start;						// File-Stream zum Lesen definiert
	file_start.open(ifilename, std::ios::in);		// Datei wird geoeffnet, lesen
	if (file_start.good())							// Filestream wird geprueft
	{
		file_start.seekg(2*pos);					// Aus Datei muss immer erst eine Zahl gelesen werden -> immer 2. Position -> 2*pos immer gerade
		file_start.read(ptr_buffer, 2);
	}
	else
		std::cout << "Fehler: Datei " << ifilename << " konnte nicht geoeffnet werden!" << std::endl;
	file_start.close();								// Datei schliesen
	stringc[0] = *ptr_buffer;
	stringc[1] = *(ptr_buffer + 1);
	if (!ival)
	{
		stringc[2] = '\0';
		//delete[] ptr_buffer;
		return stringc;
	}
	else
	{
		*(ptr_ori) = NORD;							// orientation array aufbauen
		*(ptr_ori+1) = SUED;
		*(ptr_ori + 2) = WEST;
		*(ptr_ori + 3) = OST;
		//srand(time(NULL));							// neuer Seed fuer rand
		r = rand() % 4;								// Wert 0-3
		stringc[2] = *(ptr_ori + r);
		//delete[] ptr_buffer;						// Speicherplatz freigeben
		//delete[] ptr_ori;
		return stringc;
	}
}
/* Create:
in: -
create_ships(): erstellt und initialisiert die 10 unterschiedlichen Schiffe
out: true: Erstellen hat geklappt, false: Erstellen ist fehlgeschlagen 
*/
bool player::create_ships()
{
	unsigned short i;
	char type = SCHLACHTSCHIFF;						// 1 Schlachtschiff
	char nr = this->player::get_nr();
	ships* ptr_ships = this->player::get_shipsvector();
	for (i = 0; i < MAX; i++)
	{
		switch (i)
		{
		case 1: // 2 Kreuzer
			type = KREUZER;
			break;
		case 3: // 3 Zerstoerer
			type = ZERSTOERER;
			break;
		case 6: // 4 Uboote
			type = UBOOT;
			break;
		default:;
		}
		*(ptr_ships + i) = *(new ships(type, nr));
	}
	return true;
}
// Print: gibt in Konsole Schiffsvektor aus
void player::print_ships()
{
	unsigned short i;
	const ships* ptr_ships = this->get_ships();
	 std::string coordinates = "00";
	char orientation;
	std::cout.width(30);
	std::cout << "Inhalte des Schiffsvektors des Spielers " << this->get_nr() << ":" << std::endl;
	for (i = 0; i < MAX; i++)
	{
		orientation = (ptr_ships + i)->ships::get_orientation();
		coordinates = *((ptr_ships + i)->ships::get_strposition());
		std::cout <<"\nIndex: "<< i << std::endl
			<<"Owner: "<< (ptr_ships + i)->get_owner() << std::endl
			<< "Type: "<< (ptr_ships + i)->get_type() << std::endl
			<< "Ausrichtung: " << orientation << std::endl
			<< "Koordinaten: " << coordinates << std::endl
			<< std::endl;
	}
	std::cout << std::cout.width(30)
		 << std::cout.fill('-')
		 << std::endl;
}
/* Draw Methoden:
draw_bg():
in: -
parse: zeichnet Hintergrund
out: -

draw(char ix, char iy, char itype, char iori):
in: char: x,y, Koordinate, Schiffstyp, Ausrichtung
parse: berechnet Koordinaten und zeichnet Schiff an Koordinate mit Ausrichtung
out: -

draw_hit(char ix, char iy)
in: char: x,y, Koordinate
parse: berechnet Koordinaten und zeichnet rotes X an Koordinaten
out: -

draw_hits():
in: -
parse: durchlaeuft  Treffervektor, berechnet Koordinaten und zeichnet alle Treffer
out: -

raw_ships():
in: -
parse: durchlaeuft Schiffsvektor, holt Schiffskoordinaten, berechnet Koordinaten und zeichnet alle Schiffe
out: -

draw_name():
in:-
parse: berechnet Koordinaten und zeichnet Namen, die in cplayer.h definiert sind
out: -

draw_blank(char ix, char iy, char itype, char iori):
in: char: x,y, Koordinate, Schiffstyp, Ausrichtung
parse: berechnet Koordinaten und zeichnet leeres Wasserfeld an Koordinate mit Ausrichtung
out: -
*/
void player::draw_bg()
{
	int x1, y1;
	CString filename = L"bmps\\Spielfeld.bmp";
	x1 = 0;
	y1 = 0;
	drawship(filename, x1, y1);
	wait(1);
}
void player::draw(char ix, char iy, char itype, char iori)	// Koordinaten, type, orientation
{
	int x1,y1;
	CString filename;
	switch (iori)
	{
	case (SUED) :					// Ausrichtung bedingt BMP Auswahl und Punkt ab dem gezeichnet wird
		if (this->player::get_nr() == P1)	// Spielerabhaengige Berechnung des Punktes ab dem BMP gezeichnet wird
		{
			x1 = SHIPOFFSET + XSTART + FIELD*(ix - '0');
			y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A');
		}
		else
		{
			x1 = SHIPOFFSET + P2OFFSET + XSTART + FIELD*(ix - '0');
			y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A');
		}
		switch (itype)				// Nach type und orientation abhaeginge Auswahl der BMP Datei
		{
		case UBOOT:
			filename = UBOOT_BMPS;
			break;
		case ZERSTOERER:
			filename = ZERSTOERER_BMPS;
			break;
		case KREUZER:
			filename = KREUZER_BMPS;
			break;
		case SCHLACHTSCHIFF:
			filename = SCHLACHTSCHIFF_BMPS;
			break;
		default:
			std::cout << "draw function: invalid type!" << std::endl;
		}
		break;
	case OST:
		if (this->player::get_nr() == P1)
		{
			x1 = SHIPOFFSET + XSTART + FIELD*(ix - '0');
			y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A');
		}
		else
		{
			x1 = SHIPOFFSET + P2OFFSET + XSTART + FIELD*(ix - '0');
			y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A');
		}
		switch (itype)
		{
		case UBOOT:
			filename = UBOOT_BMPO;
			break;
		case ZERSTOERER:
			filename = ZERSTOERER_BMPO;
			break;
		case KREUZER:
			filename = KREUZER_BMPO;
			break;
		case SCHLACHTSCHIFF:
			filename = SCHLACHTSCHIFF_BMPO;
			break;
		default:
			std::cout << "draw function: invalid type!" << std::endl;
		}
		break;
	case NORD:					// NORD und WEST benoetigen Offsetkorrektur, da nur rechts-runter gezeichnet wird
		if (this->player::get_nr() == P1) // Offsetkorrektur von y um -(((itype-'0') - 1)* FIELD): offsetkorrektur des Typs, und Verschiebung des Punktes ab dem gezeichnet wird um 1-4 Kaestchen
		{
			x1 = SHIPOFFSET + XSTART + FIELD*(ix - '0');
			y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A') - (((itype-'0') - 1)* FIELD);
		}
		else
		{
			x1 = SHIPOFFSET + P2OFFSET + XSTART + FIELD*(ix - '0');
			y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A') - (((itype-'0') - 1)* FIELD);
		}
		switch (itype)
		{
		case UBOOT:
			filename = UBOOT_BMPN;
			break;
		case ZERSTOERER:
			filename = ZERSTOERER_BMPN;
			break;
		case KREUZER:
			filename = KREUZER_BMPN;
			break;
		case SCHLACHTSCHIFF:
			filename = SCHLACHTSCHIFF_BMPN;
			break;
		default:
			std::cout << "draw function: invalid type!" << std::endl;
		}
		break;
	case WEST:
		if (this->player::get_nr() == P1) // Offsetkorrektur von x um -((itype - 1)* FIELD)
		{
			x1 = SHIPOFFSET+XSTART+FIELD*(ix-'0')-(((itype-'0')-1)* FIELD);
			y1 = SHIPOFFSET+YSTART+FIELD*(iy-'A');
		}
		else
		{
			x1 = SHIPOFFSET + P2OFFSET + XSTART + FIELD*(ix - '0') - (((itype-'0')-1)* FIELD);
			y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A');
		}
		switch (itype)
		{
		case UBOOT:
			filename = UBOOT_BMPW;
			break;
		case ZERSTOERER:
			filename = ZERSTOERER_BMPW;
			break;
		case KREUZER:
			filename = KREUZER_BMPW;
			break;
		case SCHLACHTSCHIFF:
			filename = SCHLACHTSCHIFF_BMPW;
			break;
		default:
			std::cout << "Fehler draw: invalid type!" << std::endl;
		}
		break;
	default:
		std::cout << "Fehler draw: invalid orientation!" << std::endl;
		break;
	}
	drawship(filename, x1, y1);	
} 
void player::draw_hit(char ix, char iy)
{
	int x, y;
	if (this->player::get_nr() == P1)
	{
		x = XSTART + HITOFFSETX + FIELD * (ix - '0');
		y = YSTART + HITOFFSETY + FIELD * (iy - 'A');
	}
	else
	{
		x = P2OFFSET + XSTART + HITOFFSETX + FIELD * (ix - '0');
		y = YSTART + HITOFFSETY + FIELD * (iy - 'A');
	}
	text(x, y, 50, RED, "X");
}
void player::draw_hits()
{
	int i,ii;
	const ships *ptr_ships;
	char x,y,type, vergleich;
	ptr_ships = this->player::get_ships();
	for (i = 0; i < MAX; i++)								// Durchlaufe Schiffsvektor des Spielers
	{
		type = (ptr_ships + i)->ships::get_type() - '0';
		for (ii = 0; ii < type; ii++)
		{// Durchlaufe Hitvektor des Schiffs
			vergleich = (*((ptr_ships + i)->ships::get_hits() + ii));
			if (vergleich == HIT)							// Schaue nach Treffern
			{
				x = *((ptr_ships + i)->ships::get_position() + (2*ii));
				y = *((ptr_ships + i)->ships::get_position() + (2*ii + 1));
				this->player::draw_hit(x, y);
			}
			else;
		}
		wait(1);
	}
}
void player::draw_ships()
{
	unsigned short i;
	const char *ptr_pos;			// Zeigt auf Positionsvektor des schiffes (RO)
	char ori,type;					// hat Ausrichtung des Schiffes gespeichert 
	// clrscr();					// Loescht Screen und setzt Hintergrund
	const ships *ptr_ships = this->player::get_ships();				// Zeiger auf Schiffsvektor des Spielers, Anordnung: {S,K1,K2,Z1,Z2,Z3,U1,U2,U3,U4}
	for (i = 0; i < MAX; i++)
	{
		ori=(ptr_ships + i)->ships::get_orientation();				// Hol orientation vom jetzigen Schiff
		ptr_pos=(ptr_ships + i)->ships::get_position();			// Hol Zeiger auf Positionsvektor (RO)
		type = (ptr_ships + i)->ships::get_type();
		draw(*(ptr_pos), *(ptr_pos + 1), type, ori);		// Gib Position an DRAW, sodass Schiff gezeichnet wird,
															// da Vektor, muss nur orientation und Ursprung bekannt sein zum Zeichnen	
	}
	//wait(5);
}
void player::draw_name()
{
	int x = XNAME;
	char *cstr = new char[(this->player::get_name()).length() + 1];		// Speicher reservieren
	strcpy(cstr, (this->player::get_name()).c_str());					// String to char*
	if (this->player::get_nr() == P2)
		x = XNAME + P2OFFSET;
	text(x, YNAME, FONTSIZENAME, WHITE, cstr);						// Text Zeichnen
	wait(1);													// Wait 1ms
	//delete[] cstr;												// Speicherplatz freigeben
}
void player::draw_blank(char ix, char iy, char itype, char iori)	// Koordinaten, type, orientation
{
	int x1, y1;
	CString filename;
	switch (iori)
	{
	case (SUED) :					// Ausrichtung bedingt BMP Auswahl und Punkt ab dem gezeichnet wird
	{
		if (this->player::get_nr() == P1)	// Spielerabhaengige Berechnung des Punktes ab dem BMP gezeichnet wird
		{
			x1 = SHIPOFFSET + XSTART + FIELD*(ix - '0');
			y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A');
		}
		else
		{
			x1 = SHIPOFFSET + P2OFFSET + XSTART + FIELD*(ix - '0');
			y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A');
		}
		switch (itype)				// Nach type und orientation abhaeginge Auswahl der BMP Datei
		{
		case UBOOT:
			filename = BLANKUBOOTNS;
			break;
		case ZERSTOERER:
			filename = BLANKZERSTOERERNS;
			break;
		case KREUZER:
			filename = BLANKKREUZERNS;
			break;
		case SCHLACHTSCHIFF:
			filename = BLANKSCHLACHTSCHIFFNS;
			break;
		default:
			filename = BLANK_BMP;
			break;
		}
		break;
	}
	case OST:
	{
		if (this->player::get_nr() == P1)
		{
			x1 = SHIPOFFSET + XSTART + FIELD*(ix - '0');
			y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A');
		}
		else
		{
			x1 = SHIPOFFSET + P2OFFSET + XSTART + FIELD*(ix - '0');
			y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A');
		}
		switch (itype)
		{
		case UBOOT:
			filename = BLANKUBOOTOW;
			break;
		case ZERSTOERER:
			filename = BLANKZERSTOEREROW;
			break;
		case KREUZER:
			filename = BLANKKREUZEROW;
			break;
		case SCHLACHTSCHIFF:
			filename = BLANKSCHLACHTSCHIFFOW;
			break;
		default:
			filename = BLANK_BMP;
			break;
		}
		break;
	}
	case NORD:					// NORD und WEST benoetigen Offsetkorrektur, da nur rechts-runter gezeichnet wird
	{	if (this->player::get_nr() == P1) // Offsetkorrektur von y um -(((itype-'0') - 1)* FIELD): offsetkorrektur des Typs, und Verschiebung des Punktes ab dem gezeichnet wird um 1-4 Kaestchen
	{
		x1 = SHIPOFFSET + XSTART + FIELD*(ix - '0');
		y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A') - (((itype - '0') - 1)* FIELD);
	}
	else
	{
		x1 = SHIPOFFSET + P2OFFSET + XSTART + FIELD*(ix - '0');
		y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A') - (((itype - '0') - 1)* FIELD);
	}
	switch (itype)
	{
	case UBOOT:
		filename = BLANKUBOOTNS;
		break;
	case ZERSTOERER:
		filename = BLANKZERSTOERERNS;
		break;
	case KREUZER:
		filename = BLANKKREUZERNS;
		break;
	case SCHLACHTSCHIFF:
		filename = BLANKSCHLACHTSCHIFFNS;
		break;
	default:
		filename = BLANK_BMP;
		break;
	}
	break;
	}
	case WEST:
	{	
		if (this->player::get_nr() == P1) // Offsetkorrektur von x um -((itype - 1)* FIELD)
	{
		x1 = SHIPOFFSET + XSTART + FIELD*(ix - '0') - (((itype - '0') - 1)* FIELD);
		y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A');
	}
	else
	{
		x1 = SHIPOFFSET + P2OFFSET + XSTART + FIELD*(ix - '0') - (((itype - '0') - 1)* FIELD);
		y1 = SHIPOFFSET + YSTART + FIELD*(iy - 'A');
	}
	switch (itype)
	{
	case UBOOT:
		filename = BLANKUBOOTOW;
		break;
	case ZERSTOERER:
		filename = BLANKZERSTOEREROW;
		break;
	case KREUZER:
		filename = BLANKKREUZEROW;
		break;
	case SCHLACHTSCHIFF:
		filename = BLANKSCHLACHTSCHIFFOW;
		break;
	default:
		filename = BLANK_BMP;
		break;
	}
	break; 
	}
	default:
		std::cout << "Fehler draw: invalid orientation!" << std::endl;
		break;
	}
	drawship(filename, x1, y1);
}