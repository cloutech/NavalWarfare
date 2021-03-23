/* Header File - class ships
Project:		Schiffeversenken Advanced
Author:			Christoph Clouser
Description:	This class defines the players with functions to deploy, attack and move ships

Changelog/Version:
0.1 File created, to-dos
0.2 Get/Set-Methoden eingefuegt
0.3 set_hits, set_position eingefuegt
0.4 dynamisches array -> vector
0.5 Definitionen erweitert
0.6 get_position, get_hits eingefuegt
0.7 set_position erweitert um Ausrichtungsvektor (n,s,w,o)
0.8 Korrektur boundchecks zur Rotation/Bewegung
0.9 create_positionstring zur Abfrage zwischen Schiffen, using namespace std entfernt (bad practice)
1.0 
*/
#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#define MAX 10
#define NOHIT '0'
#define HIT '1'
// Schiffstypen:
#define SCHLACHTSCHIFF '5'				// in ASCII; Anzahl: 1
#define KREUZER '4'						// Anzahl: 2
#define ZERSTOERER '3'					// Anzahl: 3
#define UBOOT '2'						// Anzahl: 4
// weitere Def							// ASCII:
#define NORD 'n'						// 110
#define SUED 's'						// 115
#define WEST 'w'						// 119
#define OST 'o'							// 111
#define UZ 'u'							// Im Uhrzeigersinn
#define GZ 'z'							// Gegen Uhrzeigersinn
#define P1 '1'							// Spielernr 1
#define P2 '2'							// Spielernr 2

class ships
{
private:
	char _type;								// 5: Zerstoerer(5x1), 4:Uboot(4x1), 3:Frigatte(3x1), 2:Kanonenboot(2x1)
	char _owner;							// Spielerzugehoerigkeit (Spielernummer)
	char _orientation;						// Ausrichtung: n,s,w,o
	bool _isalive;							// Zerstoert?
	std::vector <char> _hits;				// Treffervektor
	std::vector <char> _position;			// Positionsvektor
	std::vector <std::string> str_position;		// beinhaltet nur die Koordinaten des Schiffes ohne Umkreis! 
		
public:
	// Kontruktor:
	ships();
	ships(char itype, char iowner);
	// Read-Only Funktionen:
	char get_owner() const { return _owner; };
	char get_type() const { return _type; };
	char get_orientation() const { return _orientation; };
	bool get_isalive() const { return _isalive; };
	const char *get_position() const { return this->_position.data(); };
	const char *get_hits() const { return this->_hits.data(); };
	const std::string *get_strposition() const { return this->str_position.data(); };
	// Print/Display Fuktionen:
	void display_position() const; 
	void display_hits() const;
	// Schreibfunktionen:
	bool set_type(char itype);
	bool set_owner(char iowner);
	bool set_orientation(char iorientation);
	void set_isalive(bool isalive);
	bool set_hit(char ix, char iy);
	bool set_position(char ix1, char iy1);
	bool set_position(char ix1, char iy1, char val);
	void set_strposition();
	// Create:
	std::vector <std::string> create_strposradius(char ix, char iy, char iorientation);		// Gibt Testvektor mit Koordinaten zurueck
	std::string create_newposition(char ix, char iy, char iaction);

	// Testfunktionen:
	bool boundcheck(char ix, char iy, char ori) const;					
	bool boundcheck_action(char ix, char iy, char val) const;
	bool check_isalive();
};
