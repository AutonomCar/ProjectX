//The Intersection Object, used for navigation in Map.


class Intersection{
public:
	char name;
	//Next intersection (memory adress) zero if not available(default)
	Intersection * north = 0;
	Intersection * south = 0;
	Intersection * east  = 0;
	Intersection * west  = 0;
};

