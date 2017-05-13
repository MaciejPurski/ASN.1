#include "ASN.1.h"
#include "Osoba.h"
#include <iostream>
#include<string>
#include<vector>
#include <sstream>
int main()
{

	bool tab[4] = { true, true, false, false };
	Osoba a("Jan","Kowalski", 22, true, tab);
	tab[0] = 1; tab[1] = 0; tab[2] = 0; tab[3] = 1;
	Osoba b("Anna", "Kwiatkowska", 35, false, tab);
	tab[0] = 0; tab[1] = 1; tab[2] = 1; tab[3] = 0;
	Osoba c("Andrzej", "Nowak", 45, true, tab);
	std::stringstream ss;
	a.serialize(ss);


	std::cout << std::endl << "-------------------------" << std::endl;
	a.display();
	std::cout << std::endl << "-------------------------" << std::endl;
	b.display();
	std::cout << std::endl << "-------------------------" << std::endl;
	c.display();

    return 0;
}

