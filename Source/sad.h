/* Header File - Search and Destroy Algorithmus
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
0.6 
*/

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#define UNKNOWN 'u'
#define VERTICAL 'v'
#define HORIZONTAL 'h'
#define NRSHIPS 5
#define LARGESTTYPE 5
class sad
{
private:
	int _s,_z,_nrhits;							// Spalte und Zeile in Matrix, Anzahl an Treffern durch Algo
	bool flag_stopmuster;						// Wurde Treffer durch Mustersequenz erzielt? - Flag
	bool flag_aav;								// Wurde Angriffsvektor schon erstellt? - Flag
	bool flag_hit;								// War letzter Angriff durch Algo-Koordinaten ein Treffer? - Flag
	std::vector <std::string> _av;				// Beinhaltet Koordinaten, die anzugreifen sind (Angriffskreuz): 1dim String-Matrix 
	std::string lasthit;						// Beinhaltet Koordinaten des letzten Treffers (durch Mustersequenz)
	char orientation;							// UNKNOWN, HORIZONTAL, VERTICAL; speichert was ueber Ausrichtung des Schiffs bekannt
public:
	// Konstruktor:
	sad();
	// RO Methoden:
	int get_s() const { return _s; };
	int get_z() const { return _z; };
	int get_nrhits() const { return _nrhits; };
	bool get_hitflag() const { return flag_hit; };
	bool get_aav() const { return flag_aav; };
	bool get_stopmuster() const { return flag_stopmuster; };					
	const std::string *get_avr() const { return _av.data(); };					// Read Only(RO) Referenz auf Angriffsmuster
	const std::string get_lasthit() const { return lasthit; };					// RO Referenz auf letzten Treffer
	const char get_orientation() const { return orientation; };					// RO Ausrichtung
	// R/W:
	std::string *get_avrw() { return _av.data(); };					// R/W Referenz auf Angriffsmuster
	// Set Methoden: (siehe .cpp)
	void set_s();
	void set_s(int is);
	void set_z();
	void set_z(int iz);
	void set_nrhits();
	void set_nrhits(int inrhits);
	void set_stopmuster(bool ival);
	void set_aav(bool ival);
	void set_hitflag(bool ival);
	void set_lasthit(std::string icoor);
	void set_orientation(char iori);
	// Algo:
	void create_coor();									// Berechnet Angriffsvektor
	std::string sad_core();								// Fuehrt Angriffsvektor aus

};