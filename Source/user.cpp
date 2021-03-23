// User.cpp:
//		
//		
//		
//		
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "user.h"

#undef max								// Wird von VS gesetzt, fuehrt aber zu Fehler bei <limits>

COLORREF Colref[]={BLACK,RED,GREEN,BLUE,YELLOW,BROWN};
int Colind=0;

void user_main()
{

	// Initialisierung QPC:
	LARGE_INTEGER frequency;														// ticks per second
	LARGE_INTEGER t1, t2;															// ticks
	double elapsedTime;
	bool vergleich;
	int seed=0, round = 0, ii = 0, iii = 0, turn = 1, startposition_offset;			// turn = 1 -> P1 startet
	std::string coordinates = "00";
	// Initialisierung:
	QueryPerformanceFrequency(&frequency);
	clrscr();																		// clear screen, draw background
	wait(10);																		// Warten auf Buffer und Aktualisierung
	player *ptr_player;
	std::cout << "Willkommen bei Naval Warfare (Version 1.0)!" << std::endl
		<< "Bitte geben Sie einen Startpositionswert zwischen 0-99 ein: ";
	std::cin >> startposition_offset;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');			// Stream bis \n ignorieren -> Stream sauber
	std::cin.clear();															// Flag loeschen -> Einlesen sauber
	startposition_offset = startposition_offset % FIELDLENGTH;
	ptr_player = new player[2]{P1, P2};			
	(ptr_player+0)->player::input(startposition_offset);							// Platzierung der Schiffe von Spieler 1
	(ptr_player+1)->player::input(startposition_offset);							// Platzierung der Schiffe von Spieler 2
	// Spielrunden, bis einer Spieler keine Schiffe mehr hat:
	// start timer
	QueryPerformanceCounter(&t1);
	do
	{
		if (turn >= 1)																// Spielzug Spieler 2
		{
			round++;																// Zaehle die Runden
			std::cout << "Runde: " << round << std::endl;
			do {
				coordinates = (ptr_player + 0)->player::get_input(FILENAME_MUSTER, ii); // Hole Angriffskoordinaten
				vergleich = (ptr_player + 0)->player::check_coor(coordinates);			// Vergleicht ob Koordinate schon mal zu Treffer gefuehrt hat
				if (vergleich)															// Wenn Vergleich true, dann Koordinate gleich einer Trefferkoordinate
				{
					ii++;																// Hole naechste Angriffskoordinate
					ii = ii % (2 * FIELDLENGTH);										
				}
			} while (vergleich);													// so lange bis Koordinate ungleich einer Trefferkoordinate
			if (!((ptr_player + 0)->player::get_stopmuster()))						// Mustersequenz anhalten, coor werden von Algo ueberschrieben in player::check_hit(coordinates) 
			{																		
				ii++;																// Hole naechste Angriffskoordinate
				ii = ii % (2 * FIELDLENGTH);										// Fange Ueberlauf ab
			}
			(ptr_player + 0)->player::check_hit(coordinates);						// Teste auf Treffer bei Spieler 1
			if ((ptr_player + 0)->player::get_canmove())							// Wenn Spieler 2 noch mehr als 2 Schiffe hat
			{
				seed = round;
				if (MUST_TAKE_ACTION)
				{
					seed = 0;
				}
				do																	// darf Spieler 2 noch eine Aktion ausfuehren
				{ 
					vergleich = (ptr_player + 0)->player::input_action(seed);		
					seed++;
				}	
				while((vergleich && MUST_TAKE_ACTION )||(seed%MAX));				// Wird auf Bewegung bestanden, so werden nur EINMAL alle Schiffe durchgetestet (sonst event. loop)
			}
			turn = 0;

		}
		else																		// Spielzug Spieler 1
		{
			coordinates = (ptr_player + 1)->player::get_input(FILENAME_HITS2, iii);	// Hole Angriffskoordinaten
			if (!((ptr_player + 1)->player::get_stopmuster()))						// Mustersequenz anhalten, coor werden von Algo ueberschrieben
			{
				iii++;																// Hole naechste Angriffskoordinate
				iii = iii % (2 * FILE_LENGTH2);										// Fange Ueberlauf ab
			}
			(ptr_player + 1)->player::check_hit(coordinates);						// Teste auf Treffer bei Spieler 
			if ((ptr_player + 1)->player::get_canmove())							// Wenn Spieler 1 noch mehr als 2 Schiffe hat
			{
				seed = round;
				if (MUST_TAKE_ACTION)
					seed = 0;
				do																	// darf Spieler 2 noch eine Aktion ausfuehren
				{
					vergleich = (ptr_player + 1)->player::input_action(seed);		// 
					seed++;
				} while ((vergleich && MUST_TAKE_ACTION) || (seed%MAX));				// Wird auf Bewegung bestanden, so werden nur EINMAL alle Schiffe durchgetestet (sonst event. loop)
			}
			turn = 1;
		}
	} while (ptr_player->player::get_isalive() && (ptr_player + 1)->player::get_isalive()); // Kein Spieler besiegt, dann weiter in while
	QueryPerformanceCounter(&t2);
	elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
	(ptr_player + 1)->player::draw_hits();
	wait(5);																	// warten, damit draw Funktion in Buffer geschrieben hat
	(ptr_player + 0)->player::draw_hits();										// sonst eventuell fehlerhaft
	wait(5);

	if (ptr_player->player::get_isalive())
	{
		std::cout << "------ P1 hat gewonnen ------"
			<< std::endl
			<< "Runden: "
			<< round << std::endl
			<< "Zeit: "
			<< elapsedTime
			<< "ms |"
			<< (elapsedTime / 1000)
			<< "s |"
			<< (elapsedTime / 60000)
			<< "min |"
			<< std::endl << std::endl;
	}								// Kurze Ausgabe: Gewinner, Runden, Zeit
	else
	{
		std::cout << "------ P2 hat gewonnen ------" << std::endl
			<< std::endl
			<< "Runden: "
			<< round << std::endl
			<< "Zeit: "
			<< elapsedTime
			<< "ms |"
			<< (elapsedTime/1000)
			<< "s |"
			<< (elapsedTime/60000)
			<< "min |"
			<< std::endl << std::endl;
	}
}