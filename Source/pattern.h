/* Header File - class pattern
Project:		Schiffeversenken Advanced
Author:			Christoph Clouser
Description:	Diese Klasse stellt Methoden zur Generierung von Koordinaten in einer txt-Datei zur Verfuegung.

Changelog/Version:
0.1 File created, to-dos
0.2 Get/Set-Methoden eingefuegt
0.3 create ... erstellt
0.4
*/

#include <iostream>
#include <fstream>
#include <string>
#include <limits>								// Limit beim Lesen des Buffers -> cin 
#include <vector>
#include <time.h>								// -> Seed fuer srand, damit nicht immer gleicher Seed fuer rand
#undef max										// Wird von VS gesetzt, fuehrt aber zu Fehler bei <limits>

// Definitionen:
#define MAX 10
#define NOHIT '0'
#define HIT '1'
// Schiffstypen:
#define SCHLACHTSCHIFF '5'						// Typ in ASCII; Anzahl: 1
#define KREUZER '4'								// Anzahl: 2
#define ZERSTOERER '3'							// Anzahl: 3
#define UBOOT '2'								// Anzahl: 4
// Dateinamen:
#define FIELDLENGTH 100							// Groesse des Rasters (10x10)
#define SEQUENCELENGTH 30						// Startsequenzlaenge des Strings zur Platzierung der Schiffe aus Datei (3 Char * 10 Schiffe) 
#define FILENAME_START "input\\start.txt"		// Name Startsequenzdatei
#define FILENAME_HITS1 "input\\hits1.txt"		// Name der Hitsequenzdatei
#define FILE_LENGTH1 1000
#define FILENAME_HITS2 "input\\hits2.txt"		// Name der 2. Hitsequenzdatei
#define FILE_LENGTH2 200
#define FILENAME_ACTION "input\\action.txt"		// Name der Datei mit Aktionen
#define FILENAME_MUSTER "input\\muster.txt"		// Name der Datei mit Muster-Sequenz fuer Angriffe
// weitere Def										// ASCII:
#define NORD 'n'								// 110
#define SUED 's'								// 115
#define WEST 'w'								// 119
#define OST 'o'									// 111
#define UZ 'u'									// Im Uhrzeigersinn
#define GZ 'z'									// Gegen Uhrzeigersinn
#define P1 '1'									// Spielernr 1
#define P2 '2'									// Spielernr 2
class pattern
{
private:
	
public:
	// Konstruktor:
	pattern();
	// RO Methoden:
	void create_rnda(std::string ifilename, int ival);					// Random Action-Datei
	// Dateien mit Sequenzen:
	void create_rndcf(std::string ifilename, int ival);					// Random Koordinaten-Datei
	void create_cf(std::string ifilename, int ival);					// Nicht random Koordinaten-Datei
	void create_muster(std::string ifilename, int iempty, int ival);	// Parametrierbares Angriffsmuster
};