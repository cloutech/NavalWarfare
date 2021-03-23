/* Header File - class player
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
0.8 Fehler behoben: draw_blank zeichnete fuer Zerstoerer falsches BMP
0.9
*/
#include <iostream>
#include <fstream>
#include <string>
#include <limits>								// Limit beim Lesen des Buffers -> cin 
#include <vector>
#include <time.h>								// -> Seed fuer srand, damit nicht immer gleicher Seed fuer rand
#include "ships.h"
#include "sad.h"
#include "stdafx.h"
#include "graphics\graphicfunctions.h"
#undef max										// Wird von VS gesetzt, fuehrt aber zu Fehler bei <limits>


// Eingabeoptionen:
#define MUST_TAKE_ACTION 0						// default:0 ;1: So lange Schiffe durchprobieren bis eins eine aktion gemacht hat, 0: eins hat die chance
#define LASTHITS 3								// Wieviele der letzten Treffer sollen Felder blockieren?
#define HITOFFSET 4								// Hitoffset, welche Treffersequenz durchlaufen werden soll
#define POSOFFSET 76							// zwischen 0 und 99, Offset fuer eingelesenen Startkoordinaten in Datei
#define ACTIONOFFSET 0							// Offset fuer Einleseposition in Datei
#define NAME_PLAYER1 "Jean Valjean"				// Name Spieler 1
#define NAME_PLAYER2 "Javert"					// Name Spieler 2
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
// Geometrie:
#define XNAME 90								// X-Wert an dem Name geschrieben wird (Spieler1, Spieler 2 Offset automatisch)
#define YNAME 105								// Y Wert an dem Name geschrieben wird
#define FONTSIZENAME 20							// Fontssize der Namen
#define XSTART 50								// Wo beginnt x der Rasterkarte
#define YSTART 150								// Wo beginnt y der Rasterkarte
#define FIELD 40								// Groesse eines Feldes
#define P2OFFSET 500							// Offset der Rasterkarte fuer Spieler 2
#define SHIPOFFSET 0							// Offset fuer Grafikdateien (falls verschoben)
#define HITOFFSETX 10							// x-Offset wann Hit gezeichnet wird
#define HITOFFSETY -7							// y-Offset wann Hit gezeichnet wird
// Graphikfiles:
#define BG_BMP L"bmps\\Spielfeld.bmp"			// an x=0,y=0
#define BLANK_BMP L"bmps\\Blank.bmp"
#define	BLANKKREUZERNS L"bmps\\KreuzerBN.bmp"
#define	BLANKKREUZEROW L"bmps\\KreuzerBO.bmp"
#define	BLANKUBOOTNS L"bmps\\UbootBN.bmp"
#define	BLANKUBOOTOW L"bmps\\UbootBO.bmp"
#define	BLANKSCHLACHTSCHIFFNS L"bmps\\SchlachtschiffBN.bmp"
#define	BLANKSCHLACHTSCHIFFOW L"bmps\\SchlachtschiffBO.bmp"
#define	BLANKZERSTOERERNS L"bmps\\ZerstoererBN.bmp"
#define	BLANKZERSTOEREROW L"bmps\\ZerstoererBO.bmp"
#define UBOOT_BMPO L"bmps\\UbootO.bmp"
#define UBOOT_BMPS L"bmps\\UbootS.bmp"
#define UBOOT_BMPN L"bmps\\UbootN.bmp"
#define UBOOT_BMPW L"bmps\\UbootW.bmp"
#define KREUZER_BMPO L"bmps\\KreuzerO.bmp"
#define KREUZER_BMPW L"bmps\\KreuzerW.bmp"
#define KREUZER_BMPN L"bmps\\KreuzerN.bmp"
#define KREUZER_BMPS L"bmps\\KreuzerS.bmp"
#define SCHLACHTSCHIFF_BMPN L"bmps\\SchlachtschiffN.bmp"
#define SCHLACHTSCHIFF_BMPS L"bmps\\SchlachtschiffS.bmp"
#define SCHLACHTSCHIFF_BMPW L"bmps\\SchlachtschiffW.bmp"
#define SCHLACHTSCHIFF_BMPO L"bmps\\SchlachtschiffO.bmp"
#define ZERSTOERER_BMPN L"bmps\\ZerstoererN.bmp"
#define ZERSTOERER_BMPS L"bmps\\ZerstoererS.bmp"
#define ZERSTOERER_BMPW L"bmps\\ZerstoererW.bmp"
#define ZERSTOERER_BMPO L"bmps\\ZerstoererO.bmp"
// Schiffstypen:
//#define NRSHIPS 4								// Anzahl an verschiedenen Schiffstypen
#define SCHLACHTSCHIFF '5'						// Typ in ASCII; Anzahl: 1
#define KREUZER '4'								// Anzahl: 2
#define ZERSTOERER '3'							// Anzahl: 3
#define UBOOT '2'								// Anzahl: 4

class player
{
	private:
		sad algo;								// Search and Destroy Algorithmus algo wird erstellt
		std::string _name;						// Name des Spielers
		char _nr;								// Nummer des Spielers
		bool _isalive;							// Spieler hat noch Schiffe?
		bool _canmove;							// Spieler hat noch mehr als 2 Schiffe?
		std::vector <ships> _ships;				// Schiffe des Spielers
		std::vector <std::string> _nogo;		// Die letzten drei Treffer gegen diesen Spieler
		std::vector <std::string> _hits;		// Alle Koordinaten, die zu einem Treffer gefuerht haben (30)

	public:
		// Konstruktor:
		player(char inr);
		// RO Methoden:
		bool get_stopmuster() const { return algo.sad::get_stopmuster(); };		// Gibt an, ob Stopflag fuer Mustersequenz gesetzt (dann ist Algo an)
		bool get_isalive() const { return _isalive; };							// Gibt an, ob Spieler noch Schiffe hat
		bool get_canmove() const { return _canmove; };							// Gibt an, ob Spieler noch mehr als 2 Schiffe hat
		char get_nr() const { return _nr; };									// Gibt Spielernr zurueck; Read Only Methode
		std::string get_name() const { return _name; };							// Gibt Spielername zurueck; 
		const ships *get_ships() const { return this->player::_ships.data(); };	// Gibt RO Referenz auf vector _ships zurueck
		const std::string *get_hitsr() const { return this->player::_hits.data(); };			// Gibt _hits-Koordinatenvektor zurueck; 
		// R/W Methoden:
		std::string *get_hitsrw() { return this->player::_hits.data(); };			// Gibt _hits-Koordinatenvektor zurueck; 
		std::string *get_nogo() { return this->player::_nogo.data(); };			// Gibt _nogo-Koordinatenvektor zurueck; 
		ships *get_shipsvector() { return this->player::_ships.data(); };		// Gibt R/W Referenz auf vector _ships zurueck
		// Set-Methoden:
		bool set_nr(char inr);								// Setzt Spielenr
		void set_canmove(bool ival);						// Setzt ob Spielerschiffe noch Aktion durchfuehren duerfen
		void set_isalive(bool ival);						// Setzt ob Spieler verloren
		void set_name();									// Setzt Namen
		bool set_name(std::string iname);					// Setzt eingegebenen Namen
		void set_nogo(std::string icoordinates);			// Setzt Treffer, rotiert, hat immer nur die letzten drei Treffer im Speicher
		// Eingabe/Ausgabe:
		void input(int ioffset);											// Fordert Eingabe zur Platzierung
		bool input_action(int iround);										// Fordert Eingabe zur Aktion nach Schuss
		std::string get_input(std::string ifilename, int ipos);				// Dateiname, Nr der Aufstellung (0-9)
		std::string get_input(std::string ifilename, int ipos, int ival);	// Dateiname, Nr der Aufstellung (0-9), ival: 1 -> mit Orientation
		// Create:
		bool create_ships();												// Erstellt alle Schiffe des Spielers
		void create_rnda(std::string ifilename, int ival);					// Random Action-Datei
		// Dateien mit Sequenzen:
		void create_rndcf(std::string ifilename, int ival);					// Random Koordinaten-Datei
		void create_cf(std::string ifilename, int ival);					// Nicht random Koordinaten-Datei
		void create_muster(std::string ifilename, int iempty, int ival);	// Parametrierbares Angriffsmuster
		// Testfunktionen:
		bool check_coor(std::string icoor);									// Testet ob bereits schon einmal angegriffen
		bool check_hit(std::string icoordinates);							// Testet auf Treffer
		bool check_isalive();												// Testet ob Spieler besiegt
		bool positioncheck(char ix, char iy, char iorientation, int inrships);
		bool positioncheck(int inrship, char iaction);		// iorientation = NORD/SUED/WEST/OST, iaction = NORD/SUED/WEST/OST oder UZ/GZ 
		// Print Funktionen:		
		void print_ships();									// Gibt auf Konsole alle Schiffe mit Details aus
		// Draw Methoden:
		void draw_bg();										// Zeichnet nur den Hintergrund
		void draw(char ix, char iy, char type, char iori);	// Zeichnet Schiffe
		void draw_hit(char ix, char iy);					// Zeichnet Treffer
		void draw_hits();									// Zeichnet alle Treffer
		void draw_ships();									// Zeichnet alle Schiffe
		void draw_name();									// Zeichnet Spielernamen
		void draw_blank(char ix, char iy, char itype, char iori);					// Zeichnet leeres Seefeld
};
