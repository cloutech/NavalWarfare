/* class ships
Project:		Schiffeversenken Advanced
Author:			Christoph Clouser
Beschreibung:	Diese Klasse beschreibt die verschiedenen Schiffstypen und ihre Methoden. 

Changelog/Version:
0.1 File created, to-dos
0.2 Get/Set-Methoden eingefuegt
0.3 get_position, get_hits: Abfang des Ueberlaufs, Zwischenspeicher
0.4 statisches array jetzt dynamisch erzeugt
0.5 display position eingefuegt
0.6 ueberladbare boundchecks-methode eingefuegt, Ueberlaufabfang in set_position eingefuegt
0.7 set_position erweitert um Ausrichtungsvektor (n,s,w,o)
0.8 Korrektur boundchecks zur Rotation/Bewegung
0.9 create_positionstring zur Abfrage zwischen Schiffen, "using namespace std" entfernt (bad practice) 
1.0
*/
#include "stdafx.h"
#include "ships.h"

//Kontrtuktor:
ships::ships() { ; }						 // default
ships::ships(char itype, char iowner)		 // itype: SCHLACHTSCHIFF,...,UBOOT; iowner: P1, P2
{
	this->ships::set_type(itype);
	this->ships::set_owner(iowner);
	unsigned short size_pos = (2 * (this->ships::get_type() - '0'));	// Type mit Offsetkorrektur
	unsigned short size_hits = (this->ships::get_type() - '0');
	//unsigned short size_umkreis = ((size_hits+2)*3);					// (R001) Anzahl der Kaestchen des Schiffs mit Umkreis von 1 Kaestchen
	this->ships::_hits.resize(size_hits, NOHIT);						// Resize mit Initialisierung '0'
	this->ships::set_isalive(true);
	this->ships::_position.resize(size_pos, '0');						// Vektor wird an benoetigte Groesse angepasst, sodass
																		// kein Access Violation aufkommt, weil sonst der iterator und pointer invalid werden (neue Speicherposition)
	//this->_positionstring.resize(size_umkreis,"00");					// (R001) s.o.
	str_position.resize(size_hits);										// Initialisiert str_position

}

/* Uebersicht: Set- Methoden
set_type: zerstoerer/.../uboot uebergeben
set_owner: P1/P2 uebergeben
set_orientation: NORD,SUED,WEST,OST (siehe define) uebergeben
set_isalive: true/false uebergeben
set_position(...): Startwert(x0,y0) und Ausrichtung val (n,s,w,o) uebergeben
set_hits(...): speichert Treffer
create_positionstring(): erstellt Vektor mit Strings "0A" etc. mit Koordinaten fuer Schiff und 1 Kaestchen Umkreis zum Vergleich
*/
bool ships::set_type(char itype)
{
	int i(0);
	if ((itype >= UBOOT) && (itype <= SCHLACHTSCHIFF))
	{
		_type = itype;
		return true;
	}
	else
	{
		std::cout << "Fehler set_type: falscher Wert" << std::endl;
	}
	return false;
}
bool ships::set_owner(char iowner)
{
	if ((iowner == P1) || (iowner == P2))					// Vergleich gegen ASCII 0 (48) und 1 (49)
	{
		_owner = iowner;
		return true;
	}
	else
	{
		std::cout << "Fehler set_owner: falscher Wert" << std::endl;
	}
	return false;
}
bool ships::set_orientation(char iorientation)
{
	if ((iorientation == NORD)||(iorientation == SUED)|| (iorientation == WEST)|| (iorientation == OST))
	{
		_orientation = iorientation;
		return true;
	}
	else
	{
		std::cout << "Fehler set_orientation: falscher Wert" << std::endl;
	}
	return false;
}
void ships::set_isalive(bool isalive)
{
		_isalive = isalive;
}
bool ships::set_hit(char ix, char iy)
{
	unsigned short i, ii;
	int size = this->ships::_position.size();
	this->ships::check_isalive();
	if (!(this->ships::get_isalive()))
	{
		std::cout << "Ist bereits zerstoert!" << std::endl;
		return false;
	}
	if ((ix >= '0') && (ix <= '9') && (iy >= 'A') && (iy <= 'J'))
	{
		for (i = 0, ii = 0; i < size; i++, ii++)
		{
			// cout << "\nIndex i: " << i << endl << "Index ii: " << ii << endl;
			if ((this->ships::get_orientation() == WEST) || (this->ships::get_orientation() == OST))
			{
				if (*(this->ships::_position.data() + i) == ix)
					*(this->ships::_hits.data() + ii) = HIT;
				i++;
			}
			else
			{
				if ((this->ships::get_orientation() == NORD) || (this->ships::get_orientation() == SUED))
				{
					i++;
					if (*(this->ships::_position.data() + i) == iy)
						*(this->ships::_hits.data() + ii) = HIT;
				}
			}
		}
	}
	else
	{
		std::cout << "Fehler set_hits: falscher Wert" << std::endl;
		return false;
	}
	this->ships::check_isalive();
	return true;
}
bool ships::set_position(char ix, char iy)
{
	unsigned short i,ii;
	int size = this->ships::get_type();								// Type mit Offsetkorrektur ergibt Groesse
	if ((ix >= '0') && (ix <= '9') && (iy >= 'A') && (iy <= 'J') && (this->ships::boundcheck(ix, iy, this->ships::get_orientation())))	// erneuter Verlgeich auf richtige Eingabe
	{
		for (i = 0, ii = 0; i < size; i++)
		{
			switch (this->ships::get_orientation())
			{
			case WEST: // Orientation: WEST -> nur x-Wert aendert sich ruecklaufend
				if (i % 2 == 0)
				{
					*(this->ships::_position.data() + i) = ix - ii;
					ii++;
				}
				else
				{
					*(this->ships::_position.data() + i) = iy;
				}
				break;
			case OST: // Orientation: OST -> nur x-Wert aendert sich fortlaufend
				if (i % 2 == 0)
				{
					*(this->ships::_position.data() + i) = ix + ii;
					ii++;
				}
				else
				{
					*(this->ships::_position.data() + i) = iy;
				}
				break;
			case NORD: // Schiff liegt vertikal -> nur y-Wert aendert sich ruecklaufend
				if (i % 2 == 0)
				{
					*(this->ships::_position.data() + i) = ix;
				}
				else
				{
					*(this->ships::_position.data() + i) = iy - ii;
					ii++;
				}
				break;
			case SUED: // Schiff liegt vertikal -> nur y-Wert aendert sich fortlaufend
				if (i % 2 == 0)
				{
					*(this->ships::_position.data() + i) = ix;
				}
				else
				{
					*(this->ships::_position.data() + i) = iy + ii;
					ii++;
				}
				break;
			default:
				//std::cout << "Fehler Ausrichtung" << std::endl;
				return false;
			}
		}
		this->ships::set_strposition();				// Erstellt Vektor mit Koordinatenstrings 
		//if (!(this->create_strradius()))		// Erstellt Vektor mit Koordinatenstrings und Umkreiskoordinaten -> wurde rausgenommen, aktiviere alle (R001)
		//	std::cout << "Fehler: position string!";
		return true;
	}
	else
	{
		//std::cout << "Fehler set_position: falscher Wert" << std::endl;	// Diese Meldung ist auskommentiert, da bei Durchlauf eines zufaelligen Musters ungueltige auftreten werden
	}
	return false;
}
bool ships::set_position(char ix, char iy, char iaction)
{
	unsigned short i, ii;
	int size = this->ships::get_type();								// Type mit Offsetkorrektur ergibt Groesse
	bool vergleich = this->ships::boundcheck_action(ix, iy, iaction);
	if ((ix >= '0') && (ix <= '9') && (iy >= 'A') && (iy <= 'J') && (vergleich))	// erneuter Verlgeich auf richtige Eingabe
	{
		for (i = 0, ii = 0; i < size; i++)
		{
			switch (this->ships::get_orientation())
			{
			case WEST: // Orientation: WEST -> nur x-Wert aendert sich ruecklaufend
				if (i % 2 == 0)
				{
					*(this->ships::_position.data() + i) = ix - ii;
					ii++;
				}
				else
				{
					*(this->ships::_position.data() + i) = iy;
				}
				break;
			case OST: // Orientation: OST -> nur x-Wert aendert sich fortlaufend
				if (i % 2 == 0)
				{
					*(this->ships::_position.data() + i) = ix + ii;
					ii++;
				}
				else
				{
					*(this->ships::_position.data() + i) = iy;
				}
				break;
			case NORD: // Schiff liegt vertikal -> nur y-Wert aendert sich ruecklaufend
				if (i % 2 == 0)
				{
					*(this->ships::_position.data() + i) = ix;
				}
				else
				{
					*(this->ships::_position.data() + i) = iy - ii;
					ii++;
				}
				break;
			case SUED: // Schiff liegt vertikal -> nur y-Wert aendert sich fortlaufend
				if (i % 2 == 0)
				{
					*(this->ships::_position.data() + i) = ix;
				}
				else
				{
					*(this->ships::_position.data() + i) = iy + ii;
					ii++;
				}
				break;
			default:
				//std::cout << "Fehler Ausrichtung" << std::endl;
				return false;
			}
		}
		this->ships::set_strposition();			// Erstellt Vektor mit Koordinatenstrings 
												//if (!(this->create_strradius()))		// Erstellt Vektor mit Koordinatenstrings und Umkreiskoordinaten -> wurde rausgenommen, aktiviere alle (R001)
												//	std::cout << "Fehler: position string!";
		return true;
	}
	else
	{
		//std::cout << "Fehler set_position: falscher Wert" << std::endl;
	}
	return false;
}
void ships::set_strposition()
{
	int i, ii;
	std::string str_insert = "00";
	int type = (this->ships::get_type() - '0');		// Schiff besitzt 5 Paare *2 = 10 Koordinatenkomponenten (x,y)
	for (i = 0, ii = 0; i < type; i++, ii++)
	{
		str_insert[0] = *(this->ships::get_position() + (2*i));		// _position vektor hat doppelt so viele Eintraege wie str_position
		str_insert[1] = *(this->ships::get_position() + ((2*i)+1));
		str_position[ii] = str_insert;							// fuege string in vektorstring ein	 
	}
	//for (i = 0; i < (this->get_type()-'0'); i++)		// kontrolle
	//	std::cout << "In str_position: " << str_position[i]<< std::endl;
}
/*Übersicht: create-Methoden
std::vector <std::string> ships::create_strposradius(char ix, char iy, char iorientation):
in: char: x,y Koordinate, Ausrichtung
parse: berechnet und erstellt Umkreisstringvektor 
out: Stringvektor mit Umkreiskoordinaten

std::string ships::create_newposition(char ix, char iy, char iaction):
in: char: x,y Koordinate, Aktion
parse: berechnet neue Position, die durch Aktion entstehen würde
out: String mit neuer x,y Koordinate und Ausrichtung (zb: "3Dn") 
*/
std::vector <std::string> ships::create_strposradius(char ix, char iy, char iorientation)
{
	std::string str_insert = "00";				// Insert string wird zum Einfuegen der Koordinaten in Vektor-String benutzt
	std::vector <std::string> stringvec;
	stringvec.resize(((this->ships::get_type() - '0')+2)*3); //  Anzahl aller Kaestchen des Schiffs mit 1 Kaestchen Umkreis
	char x0, y0, a, b;
	int i, ii, iii, k, fields;
	x0 = ix;									// Ursprung des Schiffes
	y0 = iy;
	fields = ((this->ships::get_type() - '0') + 2);	// Anzahl der Kaestchen des Schiffs mit je 1 Kaestchen vor und hinter Schiff
	switch (iorientation)						// Auf Grund der Symmetrie werden ix und iy nach Bedarf gesetzt
	{											// so lassen sich alle Faelle mit einer Schleife erledigen
	case NORD:
		a = y0;
		b = x0;
		k = (-1);
		break;
	case SUED:
		a = y0;
		b = x0;
		k = 1;
		break;
	case WEST:
		a = x0;
		b = y0;
		k = (-1);
		break;
	case OST:
		a = x0;
		b = y0;
		k = 1;
		break;
	default:
		std::cout << "Fehler positioncheck: wrong type!" << std::endl;
	}
	for (i = -1, iii = 0; i < 2; i++)			// Es muessen 3 * (type+2) Durchlaeufe gemacht werden
	{											// um alle Kaestchen berechnen zu koennen
		for (ii = -1; ii < (fields-1); ii++, iii++)	// x/y Koordinate (je nach Himmelsrichtung) halten und y/x durchlaufen, dann x/y eins hoch
		{
			if ((iorientation == NORD) || (iorientation== SUED))	// x Wert steht immer zuerst -> sortieren
			{
				str_insert[0] = b + i*k;				// Elemente in insert String, x bleibt erst mal gleich
				str_insert[1] = a + ii*k;				// y laeuft
			}
			else
			{
				str_insert[0] = a + ii*k;				// Elemente in insert String, y bleibt erst mal gleich
				str_insert[1] = b + i*k;				// x laeuft
			}
			//cout << "String: " << str_insert << endl;	// Kontrolle
			stringvec[iii] = str_insert;			// in String-Vektor 
														//cout << "Im Vektor:" << _positionstring[iii] << endl;
		}
	}
	//cout << "Im Vektor-String: "; // Kontrolle
	//for (i = 0; i < ((fields+1) * 3); i++)
	//	cout << _positionstring[i] <<","; 
	return stringvec;
}
std::string ships::create_newposition(char ix, char iy, char iaction)
{
	std::string str_coor = "000";
	char x, y, orientation, action;
	orientation = this->ships::get_orientation();
	action = iaction;
	switch (action)
	{
	case NORD:
	{
		switch (orientation)
		{
		case NORD:		// Faehrt eins vorwaerts
			x = ix;
			y = iy - 1;
			break;
		case SUED:		// Faehrt eins rueckwaerts
			x = ix;
			y = iy + 1;
			break;
		default:		// seitlich Fahren nicht erlaubt
			return  str_coor;
		}
		break;
	}
	case SUED:
	{
		switch (orientation)
		{
		case NORD:		// Faehrt eins rueckwaerts
			x = ix;
			y = iy - 1;
			break;
		case SUED:		// Faehrt eins vorwaerts
			x = ix;
			y = iy + 1;
			break;
		default:		// seitlich Fahren nicht erlaubt
			return  str_coor;
		}
		break;
	}
	case WEST:
	{
		switch (orientation)
		{
		case WEST:		// Faehrt eins vorwaerts
			x = ix - 1;
			y = iy;
			break;
		case OST:		// Faehrt eins rueckwaerts
			x = ix + 1;
			y = iy;
			break;
		default:		// seitlich Fahren nicht erlaubt
			return  str_coor;
		}
		break;
	}
	case OST:
	{
		switch (orientation)
		{
		case WEST:		// Faehrt eins rueckwaerts
			x = ix - 1;
			y = iy;
			break;
		case OST:		// Faehrt eins vorawerts
			x = ix + 1;
			y = iy;
			break;
		default:		// seitlich Fahren nicht erlaubt
			return  str_coor;
		}
		break;
	}
	case UZ:
	{
		switch (orientation)
		{
		case NORD:
			orientation = OST;
			break;
		case SUED:
			orientation = WEST;
			break;
		case WEST:
			orientation = NORD;
			break;
		case OST:
			orientation = SUED;
			break;
		}
		x = ix;
		y = iy;
		break;
	}
	case GZ:
	{
		switch (orientation)		// Drehung aendert entsprechend Ausrichtung
		{
		case NORD:
			orientation = WEST;
			break;
		case SUED:
			orientation = OST;
			break;
		case WEST:
			orientation = SUED;
			break;
		case OST:
			orientation = NORD;
			break;
		}
		x = ix;						// Koordinaten bleiben gleich
		y = iy;
		break;
	}
	default:
	{
		std::cout << "Fehler create_newposition: unzulaessige Aktion!" << std::endl;
		return  str_coor;
	}
	}
	str_coor[0] = x;
	str_coor[1] = y;
	str_coor[2] = orientation;
	return str_coor;
}
/* Uebersicht: Diplay-Methoden
display_position: cout Koordinaten
display_hits: cout Treffer
*/
void ships::display_position() const
{
	unsigned short i;
	std::cout << "Die Koordinaten: ";
	for (i = 0; i <  this->_position.size(); i++)
	{
		std::cout << *(this->_position.data()+i);
		if ((i % 2 == 1) && (i < (this->_position.size()-1)))
			std::cout << ',';
	}
}
void ships::display_hits() const 
{
	unsigned short i;
	std::cout << "Die Treffer: ";
	for (i = 0; i < this->_hits.size(); i++)
		std::cout << *(this->_hits.data() + i);
	
}

/* Bounds Checking: Boundcheck-Funktion
boundcheck(char ix, char iy): 
in: char: x,y Koordinaten
parse: Funktion testet ob Positionsvektor noch auf Spielflaeche
out: true (gueltig)/false(ungueltig)

boundcheck(char ix, char iy, char val): 
in: val = UZ/GZ (linksrum, rechtsrum), val = n/s/w/o 
parse: Funktion testet ob bei Rotation/Bewegung Positionsvektor danach noch gueltig 
out: true (gueltig)/false(ungueltig)

check_isalive(): 
in: -
parse: testet ob Treffervektor des Schiffs komplett gesetzt ist, setzt _isalive
out: true(nicht versenkt)/false(versenkt)
*/
bool ships::boundcheck(char ix, char iy, char iori) const
{
	int type = this->ships::get_type() - '0';
	int x = ix - '0';
	int y = iy - 'A';
	if ((iori == OST) && ((type + x) <= MAX))
		return true;
	if ((iori == WEST) && ((type + (10 - x - 1)) <= MAX))
		return true;
	if ((iori == NORD) && ((type + (10 - y - 1)) <= MAX))
		return true;
	if ((iori == SUED) && ((type + y) <= MAX))
		return true;
	//cout << "Out of Bounds!" << endl;
	return false;
}
bool ships::boundcheck_action(char ix, char iy, char val) const
{
	int type = this->ships::get_type() - '0';
	int x = ix - '0';								// Offset Ausgleich
	int y = iy - 'A';
	if (this->ships::get_orientation() == OST) 
	{
		switch (val) 
		{
		case UZ: // Uhrzeigersinn
			if ((type + y) <= MAX)					// Bound Sueden
				return true;
			else
				break;
		case GZ: // Gegen Uhrzeigersinn
			if ((type + (10 - y)) <= MAX)			// Bound Norden
				return true;
			else
				break;
		case OST: // +1 nach Osten
			if ((type + x + 1) <= MAX)					// Bound Ost
				return true;
			else
				break;
		case WEST: // +1 nach Westen
			if ((10 - x + 1) <= MAX)			// Bound West
				return true;
			else
				break;
		default: // Eingabecheck fuer val
			//std::cout << "Fehler: keine gueltige Rotation oder Bewegung!" << std::endl;
			return false;
		}
	}
	if (this->get_orientation() == WEST)
	{
		switch (val)
		{
		case GZ: // Uhrzeigersinn
			if ((type + y) <= MAX)					// Bound Sueden
				return true;
			else
				break;
		case UZ: // Gegen Uhrzeigersinn
			if ((type + (10 - y)) <= MAX)			// Bound Norden
				return true;
			else
				break;
		case OST: // +1 nach Osten
			if ((x+1) < MAX)					// Bound Ost
				return true;
			else
				break;
		case WEST: // +1 nach Westen
			if ((type + (10 - x)) <= MAX)			// Bound West
				return true;
			else
				break;
		default: // Eingabecheck fuer val
			//std::cout << "Fehler: keine gueltige Rotation oder Bewegung!" << std::endl;
			return false;
		}
	}
	if (this->get_orientation() == SUED)
	{
		switch (val)
		{
		case GZ: // Uhrzeigersinn
			if ((type + x) <= MAX)					// Bound Osten
				return true;
			else
				break;
		case UZ: // Gegen Uhrzeigersinn
			if ((type + (10 - x)) <= MAX)			// Bound Westen
				return true;
			else
				break;
		case SUED: // +1 nach Sueden
			if ((type + y + 1) <= MAX)					// Bound Sueden
				return true;
			else
				break;
		case NORD: // +1 nach Norden
			if ((10 - y - 1) <= MAX)			// Bound Norden
				return true;
			else
				break;
		default: // Eingabecheck fuer val
			//std::cout << "Fehler: keine gueltige Rotation oder Bewegung!" << std::endl;
			return false;
		}
	}
	if (this->get_orientation() == NORD)
	{
		switch (val)
		{
		case UZ: // Uhrzeigersinn
			if ((type + x) <= MAX)					// Bound Osten
				return true;
			else
				break;
		case GZ: // Gegen Uhrzeigersinn
			if ((type + (10 - x)) <= MAX)			// Bound Westen
				return true;
			else
				break;
		case SUED: // +1 nach Sueden
			if ((y + 1) < MAX)						// Bound Sueden
				return true;
			else
				break;
		case NORD: // +1 nach Norden
			if ((type + (10 - y)) <= MAX)			// Bound Norden
				return true;
			else
				break;
		default: // Eingabecheck fuer val
			//std::cout << "Fehler: keine gueltige Rotation oder Bewegung!" << std::endl;
			return false;
		}
	}
	return false;
}
bool ships::check_isalive()
{
	int i, hp, type;
	const char *ptr_hits;
	ptr_hits = this->ships::get_hits();
	type = (this->ships::get_type() - '0');
	hp = type;
	for (i = 0; i < type; i++)
	{
		if(*(ptr_hits+i)==HIT)
			hp = hp - 1;
	}
	if (hp == 0)
	{
		this->ships::set_isalive(false);
		return false;
	}
	this->ships::set_isalive(true);
	return true;
}