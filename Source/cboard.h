/* Header File - class Board
Project:		Schiffeversenken Advanced
Author:			Christoph Clouser
Beschreibung:	Diese Klasse definiert die Spielbretter mit ihren Anzeigemoeglichkeiten
	Das Spielbrett ist 1-10 horizontal und A-J vertikal. Legende: o = 0 = miss, x = 1 = hit

Changelog/Version:
0.1 File created, to-dos
0.2 Konstruktor
0.3 

*/
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "cplayer.h"

class cboard
{
	private:
		unsigned short _round = 0;
		
	public:
	// Konstruktor:
	cboard();
	// Get Methoden:
	unsigned short get_round() { return _round; };
	// Set Methoden:
	void set_round();
};