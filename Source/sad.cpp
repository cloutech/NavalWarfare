/* Cpp File - Search and Destroy Algorithmus
Project:		Schiffeversenken Advanced
Author:			Christoph Clouser
Beschreibung:	Diese Klasse beschreibt einen Search and Destroy Algorithmus (sad). Er ist ausgelegt auf
ein Koordinatensysem von x aus [0,9] und y aus [A,J]

Changelog/Version:
0.1 File created, to-dos
0.2 get/set Methoden
0.3 create Koordinatenvektoren
0.4 Konstruktor
0.5 sad_core
0.6 Pointerueberlauf in sad_core korrigiert 
0.7 
*/
#include "stdafx.h"
#include "sad.h"
// Konstruktor: initialisiert Flags und Vektoren
sad::sad() 
{
	this->sad::set_s(0);									// Spalte auf Null
	this->sad::set_z(0);									// Spalte auf Null
	this->sad::set_nrhits(1);								// Wenn Algo startet wurde bereits ein Treffer erziehlt
	this->sad::lasthit = "00";								// Init, beinhaltet sonst Koordinaten des letzten Treffers
	this->sad::_av.resize(16,"00");							// 4 Angriffsvektoren (Richtungsgebunden) mit 4 Eintraegen = 16 Elemente
	this->sad::set_stopmuster(false);						// Statusverwaltung (flags), Initialisierung
	this->sad::set_aav(false);								// Wurde Angriffsvektor schon erstellt -> nein
	this->sad::set_hitflag(false);							// Wurde ein Hit durch Algo erzielt -> nein
	this->sad::set_orientation(UNKNOWN);					// Ist die Ausrichtung des Schiffes bekannt -> nein
}

// Set-Methoden:
// Set_s(): zaehlt _s (Spalte) eins rauf
// Set_s(int is): setzt _s = is
void sad::set_s()
{
	_s = this->sad::get_s() + 1;
}
void sad::set_s(int is)
{
	_s = is;
}
// Set_z(): zaehlt _z (Zeile) eins rauf
// Set_z(int iz): setzt _z = iz
void sad::set_z()
{
	_z = this->sad::get_z() + 1;
}
void sad::set_z(int iz)
{
	_z = iz;
}
// Set_nrhits(): zaehlt _nrhits (Anzahl an Treffern) eins rauf
// Set_nrhits(int inrhits): setzt _nrhits = inrhits
void sad::set_nrhits()
{
	_nrhits = this->sad::get_nrhits() + 1;
}
void sad::set_nrhits(int inrhits)
{
	_nrhits = inrhits;
}
// set_stopmuster(bool ival): setzt _stopmuster = ival
void sad::set_stopmuster(bool ival)
{
	flag_stopmuster = ival;
}
// set_aav(bool ival): setzt _aav = ival (aquired attack vector)
void sad::set_aav(bool ival)
{
	flag_aav = ival;
}
// set_hitflag(bool ival): setzt _hitflag = ival (wird true, wenn letzte Algo-Koordinate zu einem Treffer fuehrte)
void sad::set_hitflag(bool ival)
{
	flag_hit = ival;
}
/* set_lasthit: 
input: string Koordinaten (zb: "4G")
parse: setzt Koordinaten _lasthit = icoor
output: none
*/
void sad::set_lasthit(std::string icoor)
{
	lasthit = icoor;
}
/* set_orientation:
input: char Ausrichtung = UNKNOWN, VERTICAL, HORIZONTAL, Def siehe Header
parse: setzt _orientation = iori
output: none
*/
void sad::set_orientation(char iori)
{
	orientation = iori;
}
/* create_coor:
	input: none
	parse: erstellt ausgehend vom letzten Treffer (durch Mustersequenz) ein Angriffskreuz und schreibt dieses in den Vektor _av
	output: none
*/
void sad::create_coor()									// Berechnet Angriffsvektor
{
	int i, s ,z;
	char x, y, x0, y0;
	std::string *ptr_av, lasthit = "00", temp = "00";
	lasthit = this->sad::get_lasthit();					// Holt ersten Treffer durch Mustersequenz
	ptr_av = this->sad::get_avrw();						// R/W Referenz auf Angriffsvektor
	// Berechnung des Angriffsvektors:
	x0 = lasthit[0];
	y0 = lasthit[1];
	for (i = 0, s=0, z=0; i < 16; s++)
	{
		if (s >= 4)
		{
			s = s % 4;
			z++;
			z = z % 5;
		}
		i = 4 * z + s;									// Berechnung des Index in Matrix (durch 1dim Vektor realisiert, da schneller)
		switch (z)
		{
		case 0:
		{
			x = x0;										// x wird gehalten
			y = y0 - (s + 1);							// y laeuft
			break; }
		case 1:
		{
			x = x0 + (s + 1);
			y = y0;
			break; }
		case 2:
		{
			x = x0;
			y = y0 + (s + 1);
			break; }
		case 3:
		{
			x = x0 - (s + 1);
			y = y0;
			break; }
		default:
		{
			std::cout << "Fehler: Erstellung Angriffsvektor" << std::endl;
			break; }
		}
		// Ueber/Unterlauf von Karte abfangen: ein "00" String wird als Abbruch der Richtung gewertet
		if ((x<'0') || (x >'9'))
		{
			x = '0';
			y = '0';
		}
		if ((y<'A') || (y >'J'))
		{
			x = '0';
			y = '0';
		}
		temp[0] = x;											// Koordinaten zusammenfuegen
		temp[1] = y;
		*(ptr_av + i) = temp;									// Koordinaten werden paarweise in Angriffsvektor geschrieben
		if (i == 15)
			break;
	}
	this->sad::set_aav(true);
}
/*sad_core: 
input: none
parse: Algorithmus ruft create_coor auf, laeuft Bedingungen fuer naechsten Angriffspunkt durch und ermittelt eine Angriffsstrategie
output: gibt Koordinantenstring (zB: "5E") aus
*/
std::string sad::sad_core()										// Fuehrt Angriffsvektor aus
{
	bool hitflag, once=false;
	int i, z, s;
	char orientation;
	const std::string *ptr_av;
	std::string lasthit = "00", coor = "00";					// Initialisierung
	orientation = this->sad::get_orientation();					// Aktualisiere Info ueber Ausrichtung
	lasthit = this->sad::get_lasthit();							// Holt Koordinaten des letzten Treffers
	hitflag = this->sad::get_hitflag();							// War letzter Angriff ein Treffer?
	ptr_av = this->sad::get_avr();								// Read only Referenz auf Angriffsvektor
	if (!(this->sad::get_aav()))								// Wurde Angriffsvektor erstellt?
	{															// dann erstelle Angriffsvektor
		this->sad::create_coor();								// Algorithmus wird das erste mal aufgerufen
		coor = *(ptr_av + 0);									// also gib in jedem Fall x1 zurueck
		//this->sad::set_s();										// s hochzaehlen
		this->sad::set_aav(true);								// Angriffsvektor wurde erstellt
		if (!(coor.compare("00")))								// auser es liegt ein Kartenueberlauf vor
		{
			this->sad::set_s(0);								// dann gehe unten in die weiteren Checks, s wieder 0
		}
		else
			return coor;										// kein Ueberlauf, dann raus
	}
	if (_nrhits <= 5)							// Wurden 5 Treffer erzielt, dann ist Schiff sicher zerstoert -> delete Algo
	{
		this->sad::set_stopmuster(false);		// Algo wieder in Anfangszustand
		this->sad::set_aav(false);
		this->sad::set_hitflag(false);
		this->sad::set_s(0);
		this->sad::set_z(0);
		this->sad::set_nrhits(1);
		this->sad::set_orientation(UNKNOWN);
		return "00";							// Also wird algo geloescht
	}					
	do{
		s = this->sad::get_s();									// Aktualisiere Spalte und
		z = this->sad::get_z();									// Zeile
		i = 4 * z + s;											// Index des 1dim Vektors (quasi Matrix) berechnen
		// check auf i==16 ueberlauf
		coor = *(ptr_av + i);
		switch (orientation)									// Stufe 1: Ausrichtung ausfindig machen
		{
		case UNKNOWN:											// Ist die Ausrichtung noch unbekannt
			{
				if (hitflag)									// X1/x5/x9/x13 war Treffer -> Ausrichtung bekannt, von z abhaengig
				{
					this->sad::set_nrhits();					// Zaehle Treffer
					if ((z == 0) || (z == 2))
					{
						this->sad::set_orientation(VERTICAL);
					}
					else
					{
						this->sad::set_orientation(HORIZONTAL);
					}
					this->sad::set_s();
				}
				else
				{	
					s = 0;										// Kein Treffer, dann naechste Zeile ausprobieren, auch wenn x1 Kartenueberlauf waere
					this->sad::set_z();							// also Zeile hochzaehlen
				}
			break; }
		case VERTICAL:
			{
				if (!(coor.compare("00")))
				{
					if (once)
					{
						this->sad::set_stopmuster(false);		// Algo wieder in Anfangszustand
						this->sad::set_aav(false);
						this->sad::set_hitflag(false);
						this->sad::set_s(0);
						this->sad::set_z(0);
						this->sad::set_nrhits(1);
						this->sad::set_orientation(UNKNOWN);
						return "00";							// Also wird algo geloescht
					}
					else
					{
						s = 0;
						this->set_z(2);									// Zeile rauf;
						once = true;
					}
				}
				else
				{
					if (hitflag)									//  war Treffer 
					{
						this->sad::set_nrhits();					// Treffer zaehlen
						this->sad::set_s();							// -> weiter im Angriffsvektor
					}
					else
					{												// Kein Treffer 
						if (this->sad::get_z() != 2)					// Richtung sueden testen, bis kein Treffer mehr
						{
							this->sad::set_z(2);					// -> naechste Zeile ausprobieren
							this->sad::set_s(0);					// dann s wieder von 0 ab
						}
						else
						{											// Kein Treffer mehr und Ausrichtung bekannt => Abbruch des Algo, Schiff versenkt
							this->sad::set_stopmuster(false);		// Algo wieder in Anfangszustand
							this->sad::set_aav(false);
							this->sad::set_hitflag(false);
							this->sad::set_s(0);
							this->sad::set_z(0);
							this->sad::set_nrhits(1);
							this->sad::set_orientation(UNKNOWN);
							return "00";							// Also wird algo geloescht
						}
					}
				}
			break; }
		case HORIZONTAL:
			{
				if (!(coor.compare("00")))
				{
					if (once)
					{
						this->sad::set_stopmuster(false);		// Algo wieder in Anfangszustand
						this->sad::set_aav(false);
						this->sad::set_hitflag(false);
						this->sad::set_s(0);
						this->sad::set_z(0);
						this->sad::set_nrhits(1);
						this->sad::set_orientation(UNKNOWN);
						return "00";							// Also wird algo geloescht
					}
					else
					{
						s = 0;
						this->set_z(3);								// Zeile rauf;
						once = true;
					}
				}
				else
				{
					if (hitflag)									//  war Treffer 
					{
						this->sad::set_nrhits();					// Treffer zaehlen
						this->sad::set_s();							// -> weiter im Angriffsvektor
					}
					else
					{												// Kein Treffer 
						if (this->sad::get_z() != 3)				// Richtung sueden testen, bis kein Treffer mehr
							this->sad::set_z(3);					// -> naechste Zeile ausprobieren
						else
						{											// Kein Treffer mehr und Ausrichtung bekannt => Abbruch des Algo, Schiff versenkt
							this->sad::set_stopmuster(false);		// Algo wieder in Anfangszustand
							this->sad::set_aav(false);
							this->sad::set_hitflag(false);
							this->sad::set_s(0);
							this->sad::set_z(0);
							this->sad::set_nrhits(1);
							this->sad::set_orientation(UNKNOWN);
							return "00";							// Also wird algo geloescht
						}
					}
				}
			break; }
		default:
			std::cout << "Fehler in SaD: Ausrichtung!" << std::endl;
		}
		s = this->sad::get_s();									// Aktualisiere Spalte und
		z = this->sad::get_z();									// Zeile
		i = 4 * z + s;											// Index des 1dim Vektors (quasi Matrix) berechnen
		if (i == 16)
		{
			coor = "00";
			break;
		}
		else
		{
			coor = *(ptr_av + i);
		}
		if (!(coor.compare("00")) && (orientation == UNKNOWN))  // Ist x schon ungueltig -> Kartenrand => naechste Zeile
			{
				s = 0;
				this->sad::set_hitflag(false);					// Treffer zuruecksetzen
			}
	} while (!(coor.compare("00")));							// Bleibe in Schleife bis gueltige Koordinaten in coor
	this->sad::set_hitflag(false);								// Treffer zuruecksetzen
	return coor;
}