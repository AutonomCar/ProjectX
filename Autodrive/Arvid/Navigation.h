/*
* Arvid Englund
* 2017
*
* Positioning classes and functions
* "Autonoma Elbilar"
*/


class Pos {
public:
	char intsec;
	char dir;
	bool leave;
	bool available = true;
};

class Intersection { // first try, might be better?
public:
	bool n;
	bool s;
	bool w;
	bool e;

	Intersection * north;
	Intersection * south;
	Intersection * west;
	Intersection * east;

};



Pos nextPos(Pos current) {

	Pos ret;
	ret.intsec = current.intsec;
	ret.dir = current.dir;

	//when leaving intersection

	if (current.leave) {
		ret.leave = false;
		switch (current.intsec) {
		
		case 'a':
			switch (current.dir) {
	
				case 'n':
					ret.intsec = 'b';
					ret.dir = 'n';
					break;
	
				case 's':
					ret.intsec = 'c';
					ret.dir = 'n';
					break;
	
				case 'e':
					ret.intsec = 'b';
					ret.dir = 'w';
					break;
	
				case 'w':
					ret.intsec = 'c';
					ret.dir = 'w';
					break;
				default:
					ret.available = false;
			}
			break;
		
		case 'b':
			switch (current.dir) {

				case 'n':
					ret.intsec = 'a';
					ret.dir = 'n';
					break;

				case 's':
					ret.intsec = 'c';
					ret.dir = 'e';
					break;

				case 'e':
					ret.available = false;
					break;

				case 'w':
					ret.intsec = 'a';
					ret.dir = 'e';
					break;
				default:
					ret.available = false;
			}
			break;
		
		case 'c':
			switch (current.dir) {

				case 'n':
					ret.intsec = 'a';
					ret.dir = 's';
					break;

				case 's':
					ret.available = false;
					break;

				case 'e':
					ret.intsec = 'b';
					ret.dir = 's';
					break;

				case 'w':
					ret.intsec = 'a';
					ret.dir = 'w';
					break;
				default:
					ret.available = false;
			}
			break;

		default:
			ret.available = false;
		}
	}



	return ret;
}
