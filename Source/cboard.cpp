/* class Board
Project:		Schiffeversenken Advanced
Author:			Christoph Clouser
Description:	Diese Klasse definiert die Spielbretter mit ihren Anzeigemoeglichkeiten
	Das Spielbrett ist 1-10 horizontal und A-J vertikal. Legende: o = 0 = miss, x = 1 = hit

	
Changelog/Version:
0.1 File created, to-dos
0.2 Konstruktor
0.3 

*/

#include "stdafx.h"
#include "cboard.h"
#include ".\\Graphics\\Graphicfunctions.h"

// Konstruktor:
cboard::cboard()
{
	;
}
// Set Methoden:

void cboard::set_round()
{
	_round = (cboard::get_round() + 1);
}