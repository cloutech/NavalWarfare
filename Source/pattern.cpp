/* class class pattern
Project:		Schiffeversenken Advanced
Author:			Christoph Clouser
Description:	Diese Klasse stellt Methoden zur Generierung von Koordinaten in einer txt-Datei zur Verfuegung.

Changelog/Version:
0.1 File created, to-dos
0.2 Get/Set-Methoden eingefuegt
0.3 create ... erstellt
0.4 
*/
#include "stdafx.h"
#include "pattern.h"

// Konstruktor: default
pattern::pattern() 
{
	;
}

/* : Übersicht:
create_rndcf(filename,0=neu/1=erweitern): erstellt eine pseudo-zufaellige Koordinatensequenz (Seed von time(NULL)) in ifilename Datei
create_cf(filename,0=neu/1=erweitern): erstellt eine geordnete Koordinatensequenz (0A1A2A..0B1B2B...9J) in ifilename Datei
create_muster(filename, iempty, ival): filename = dateiname, int iempty = Anzahl der Stellen, die uebersprungen werden sollen bei Angriff
create_rnda(): erstellt pseudo-zufaellige Aktionsmuster-Datei
*/
void pattern::create_rndcf(std::string ifilename, int ival) // ifilename = Dateiname, ival: 0=Datei neu schreiben, 1=exitierende Datei erweitern
{ // create random coordinate file (Bsp: 1D7F...)
	int r1, r2, i;
	char y, x;
	std::fstream file_out;
	char *ptr_buffer = new char[2 * FIELDLENGTH];	// Speicherreservierung fuer Buffer
	srand(time(NULL));							// neuer Seed fuer rand
	for (i = 0; i < FIELDLENGTH; i++)			// Generiert pseudo-random Koordinatensequenz
	{
		r1 = rand() % MAX;							// Nummer zw 0-9
		r2 = rand() % MAX;
		x = r1 + '0';								// Erstelle gueltige X,Y Koordinaten, ASCII Offset -> typ conversion int to char
		y = r2 + 'A';
		*(ptr_buffer + (2 * i)) = x;
		*(ptr_buffer + ((2 * i) + 1)) = y;
	}
	if (ival)
		file_out.open(ifilename, std::ios::out | std::ios::app);	// outStream erstellen, append -> Data anhaengen
	else
		file_out.open(ifilename, std::ios::out);					// outStream erstellen, truncate -> Datei auf zero -> neu schreiben
	if (!file_out.good())											// Filestream wird geprueft
	{
		std::cout << "Fehler: Datei " << ifilename << " konnte nicht geoeffnet werden!" << std::endl;
		exit(0);
	}
	else
	{
		file_out.write(ptr_buffer, (2 * FIELDLENGTH));							// Stream in Datei schreiben
		file_out.close();															// Datei schliesen
	}
	delete[] ptr_buffer;															// Speicherplatz freigeben
}
void pattern::create_cf(std::string ifilename, int ival) // ifilename = Dateiname, ival: 0=Datei neu schreiben, 1=exitierende Datei erweitern
{ // create coordinate file (Bsp: A0A1...)
	int i, ii;
	char y, x;
	std::fstream file_out;
	char *ptr_buffer = new char[2 * FIELDLENGTH];	// Speicherreservierung fuer Buffer
	for (i = 0, ii = 0; i < FIELDLENGTH; i++)				// Generiert Koordinatensequenz
	{
		x = (i%MAX) + '0';							// Erstelle gueltige X,Y Koordinaten, ASCII Offset -> typ conversion int to char
		y = (ii%MAX) + 'A';
		*(ptr_buffer + (2 * i)) = x;
		*(ptr_buffer + ((2 * i) + 1)) = y;
		if ((i%MAX) == 9)
			ii++;
	}
	if (ival)
		file_out.open(ifilename, std::ios::out | std::ios::app);	// outStream erstellen, append -> Data anhaengen
	else
		file_out.open(ifilename, std::ios::out);					// outStream erstellen, truncate -> Datei auf zero -> neu schreiben
	if (!file_out.good())											// Filestream wird geprueft
	{
		std::cout << "Fehler: Datei " << ifilename << " konnte nicht geoeffnet werden!" << std::endl;
		exit(0);
	}
	else
	{
		file_out.write(ptr_buffer, (2 * FIELDLENGTH));				// Stream in Datei schreiben
		file_out.close();											// Datei schliesen
	}
	delete[] ptr_buffer;											// Speicherplatz freigeben
}
void pattern::create_muster(std::string ifilename, int iempty, int ival) // ifilename = Dateiname, iempty: freilassen, dann Treffer, ival: 0=Datei neu schreiben, 1=exitierende Datei erweitern
{ // create coordinate file (Bsp: A0A1...)
	unsigned short index, i, ii, iii, iiii, empty, lim, val;
	char y, x;
	std::fstream file_out;
	char *ptr_buffer = new char[4 * FIELDLENGTH];				// Speicherreservierung fuer Buffer
	val = ival;
	empty = iempty%MAX;											// Overflow-Abfang
	lim = 2 * (FIELDLENGTH / (empty + 1));
	for (iiii = 0; iiii < (empty + 1); iiii++)
	{
		for (index = 0, i = 0, ii = 0; index < lim; i++, index++)				// Generiert Koordinatensequenz
		{

			x = (i + iiii) % MAX + '0';								// Erstelle gueltige X,Y Koordinaten, ASCII Offset -> typ conversion int to char
			y = (ii%MAX) + 'A';
			*(ptr_buffer + ((2 * index) + (iiii*lim))) = x;
			*(ptr_buffer + ((2 * index) + 1 + (iiii*lim))) = y;
			for (iii = 0; iii < empty; iii++)						// Felder werden ausgelassen -> nicht in Datei geschrieben
			{
				i++;												// Also so lange Luecke lassen bis Anzahl empty Felder erreicht
				if ((i%MAX) == 9)
				{
					ii++;											// Falls in naechste Zeile gelaufen wird, abfangen
				}
			}
			if (((i + iiii) % MAX) == 9)
			{
				ii++;
			}
		}
	}
	if (val)
		file_out.open(ifilename, std::ios::out | std::ios::app);	// outStream erstellen, append -> Data anhaengen
	else
		file_out.open(ifilename, std::ios::out);					// outStream erstellen, truncate -> Datei auf zero -> neu schreiben
	if (!file_out.good())											// Filestream wird geprueft
	{
		std::cout << "Fehler: Datei " << ifilename << " konnte nicht geoeffnet werden!" << std::endl;
		exit(0);
	}
	else
	{
		file_out.write(ptr_buffer, 2 * FIELDLENGTH);				// Stream in Datei schreiben
		file_out.close();
	}
	delete[] ptr_buffer;										// Speicherplatz freigeben
}
void pattern::create_rnda(std::string ifilename, int ival) // ifilename = Dateiname, ival: 0=Datei neu schreiben, 1=exitierende Datei erweitern
{ // create random coordinate file (Bsp: 1D7F...)
	int r, i;
	char a, arr_a[] = { NORD,SUED,OST,WEST,UZ,GZ };	// Moegliche Aktionen
	std::fstream file_out;
	char *ptr_buffer = new char[2 * FIELDLENGTH];		// Speicherreservierung fuer Buffer
	srand(time(NULL));								// neuer Seed fuer rand
	for (i = 0; i < 2 * FIELDLENGTH; i++)				// Generiert pseudo-random Aktionssequenz
	{
		r = rand() % 6;								// 6 moegliche Aktionen
		a = arr_a[r];
		*(ptr_buffer + i) = a;
	}
	if (ival)
		file_out.open(ifilename, std::ios::out | std::ios::app);	// outStream erstellen, append -> Data anhaengen
	else
		file_out.open(ifilename, std::ios::out);					// outStream erstellen, truncate -> Datei auf zero -> neu schreiben
	if (!file_out.good())											// Filestream wird geprueft
	{
		std::cout << "Fehler: Datei " << ifilename << " konnte nicht geoeffnet werden!" << std::endl;
		exit(0);
	}
	else
	{
		file_out.write(ptr_buffer, (2 * FIELDLENGTH));							// Stream in Datei schreiben
		file_out.close();															// Datei schliesen
	}
	delete[] ptr_buffer;															// Speicherplatz freigeben
}