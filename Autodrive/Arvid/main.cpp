/*
* Arvid Englund 
* 2017
* 
* Main and testing
*/

//Att göra: en för att lämna korsning (bara ge ingångsläget för nästa) och en för att hitta en tillåten väg i korsningen(inte samma tillbaka eller en väg som inte finns.)
//Enklast att kanske göre en kort lista med förutbestämt men random rutt. 

//Att göra: koppla höger/vänster/rakt till korrekt väderstreck vid varje korsning. **DETTA ÄR DET STÖRSTA PROBLEMET**
//Lösning:hårdkoda in det i kartan/navigeringen. Finns det inget enklare sätt?


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
