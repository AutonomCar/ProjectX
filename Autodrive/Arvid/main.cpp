/*
* Arvid Englund 
* 2017
* 
* Main and testing
*/

//Att g�ra: en f�r att l�mna korsning (bara ge ing�ngsl�get f�r n�sta) och en f�r att hitta en till�ten v�g i korsningen(inte samma tillbaka eller en v�g som inte finns.)
//Enklast att kanske g�re en kort lista med f�rutbest�mt men random rutt. 

//Att g�ra: koppla h�ger/v�nster/rakt till korrekt v�derstreck vid varje korsning. **DETTA �R DET ST�RSTA PROBLEMET**
//L�sning:h�rdkoda in det i kartan/navigeringen. Finns det inget enklare s�tt?


#include <iostream>
#include "Navigation.h"

using namespace std;


int  main() {

	char list[10] = { 'n','s', 'w', 'e', 'w', 's', 'n', 'e', 'w', 'n' }; //Example of premade route

	Pos now;
	now.intsec = 'b';
	now.leave = true;

	for (int i = 0; i<=10; i++) {
		//randomly pick direction
		now.dir = list[i];
		

		now = nextPos(now);
		while (now.available == false) {
			//randomly repick new direction
			cout << "wrong" <<endl;
			i++;
			now.dir = list[i];
			now = nextPos(now);
		}

	cout << i << " dir: " << list[i] << endl;
	cout << "intsec: " << now.intsec << endl;

	}



	return 0;
}
