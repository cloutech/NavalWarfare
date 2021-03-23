// TESTSHIPS.cpp: TEST DATEI fuer Schiffe
//
//
//
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "user.h"
#include "graphics\graphicfunctions.h"
#include "math.h"

#include <iostream>
#include <fstream>
#include "ships.h" 
//#include "cboard.h"
//#include "cplayer.h"

#ifndef _USE_OLD_OSTREAMS
using namespace std;
#endif


COLORREF Colref[] = { BLACK,RED,GREEN,BLUE,YELLOW,BROWN };
int Colind = 0;

void user_main()
{
	ships uboot1(KREUZER, P1);
	char typ, own, ori;
	char type;
	//uboot1.set_type(KREUZER);
	//uboot1.set_owner(P1);
	uboot1.set_orientation(SUED);
	uboot1.set_position('2', 'G');
	type = uboot1.get_type();
	uboot1.display_position();
	uboot1.set_hits('2', 'I');
	uboot1.display_hits();
	cout << endl << "Is alive? ";
	if (uboot1.get_isalive())
		cout << "Ja" << endl;
	else
		cout << "Nein" << endl;
	typ = uboot1.get_type();
	own = uboot1.get_owner();
	ori = uboot1.get_orientation();
	cout << "Type: " << typ << "\n" << "Owner: " << own << "\n" << "Orientation: " << ori << endl;
	uboot1.display_position();
	if (!(uboot1.boundcheck(*(uboot1.get_position()), *(uboot1.get_position() + 1), SUED)))
		cout << "\n check fail" << endl;
}