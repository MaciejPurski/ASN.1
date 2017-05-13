#pragma once
#include<string>
#include <iostream>
#include<string>
#include<vector>
#include <sstream>
#include <fstream>
enum except_ids { INVALID_TAG, UNKNOWN_TYPE, FILE_NOT_OPEN, ARRAY_OUT_OF_BOUNDS, BAD_ENUM, LENGTH_BYTES };

class Exception
{
private:
	int id;
public:
	Exception(int id) : id(id) {}
	std::string what()
	{
		switch (id)
		{
		case(INVALID_TAG):
			return "Unkown tag";
			break;
		case(LENGTH_BYTES):
			return "Data takes too much space";
			break;
		case(UNKNOWN_TYPE):
			return "Wrong type";
			break;
		case(FILE_NOT_OPEN):
			return "Couldn't open file for reading/writing.";
			break;
		case(ARRAY_OUT_OF_BOUNDS):
			return "Tried to access an element out of the array's bounds.";
		case(BAD_ENUM):
			return "Inappropriate enum state";
		default:
			return "Undefined exception thrown.";
		}

	}
};

class IDisplayable //!klasa interfejsu wyswietlajacego na ekran 
{
public:
	virtual void display() = 0;
};

class IStorable//!Klasa interfejsu zapisujacego do pliku
{
public:
	virtual void saveTofile(const char* filename) = 0;
};


class ASNobject :public IDisplayable, IStorable//!klasa interfejsu kodujacego i dekodujacego
{
protected:
	unsigned char tag;
public:
	virtual int countLength() = 0;//!Funkcja zwracajaca dlugosc zakodowanej danej
	virtual void codeValue(std::ostream& dataOut) = 0;//!Funkcja kodujaca wartosc 
	virtual void decodeValue(std::istream& dataIn, int length) = 0;//!Funkcja dekodujaca wartosc
	void codeLength(std::ostream& dataOut);
	int countLengthBytes(int length);
	void deserialize(std::istream& dataIn);//!Funkcja czytajaca ze strumienia dane
	void serialize(std::ostream& dataOut);//!Funkcja wyrzucajaca zakodowana dana na strumien
};

class integerASN : public ASNobject//!
{
protected:
	int value;
public:
	integerASN(int x = 0) { tag = 2;  value = x; }
	int countLength();
	void codeValue(std::ostream& dataOut);
	void decodeValue(std::istream& dataIn, int length);
	void setvalue(int b);
	int getvalue();
	void display();
	void saveTofile(const char* filename);

};

class stringASN : public ASNobject//!
{
	std::string value;
public:
	stringASN(std::string n = "") { tag = 12;  value = n; }
	int countLength();
	void codeValue(std::ostream& dataOut);
	void decodeValue(std::istream& dataIn, int length);
	void setvalue(std::string n);
	std::string getstring();
	void display();
	void saveTofile(const char* filename);

};

class bitstringASN : public ASNobject//!
{
	bool* bitstring;
	int size;
	std::string* content;
public:
	bitstringASN(int nsize = 0) { tag = 3;  size = nsize;  bitstring = new bool[size]; }
	void setvalue(bool* arg, int nsize);
	bool* getstring();
	~bitstringASN() { delete[]content; }
	int countLength();
	void codeValue(std::ostream& dataOut);
	void decodeValue(std::istream& dataIn, int length);
	void display();
	void saveTofile(const char* filename);
	void fill(std::string* ntab, int nsize); //funkcja wypelniajaca liste wielowybory


};

class enumeratedASN : public integerASN//!
{
	std::string* content;
	int size;
public:

	enumeratedASN(int nv = 0) { tag = 10; value = nv; }
	~enumeratedASN() { delete[] content; }
	void display();
	void saveTofile(const char* filename);
	void fill(std::string* ntab, int nsize);


};

class sequenceASN : public ASNobject//!
{
protected:
	std::vector<ASNobject*> tab;
public:
	sequenceASN() { tag = 16; }
	~sequenceASN() {}
	sequenceASN(std::vector<ASNobject*> ntab) { tag = 16;  tab = ntab; }
	int countLength();
	void codeValue(std::ostream& dataOut);
	void decodeValue(std::istream& dataIn, int length);
	void display();
	void saveTofile(const char* filename);


};

class Osoba :public sequenceASN//!Klasa przechowujaca dane osoby 
{
	stringASN imie;
	stringASN nazwisko;
	integerASN wiek;
	enumeratedASN plec;
	bitstringASN zainteresowania;
public:
	Osoba();
	Osoba(std::string nimie, std::string nnazwisko, int nwiek, bool nplec, bool* nzainteresowania);
	void setImie(std::string nimie);
	std::string getImie();
	void setNazwisko(std::string nnazwisko);
	std::string getNazwisko();
	void setWiek(int nwiek);
	int getWiek();
	void setPlec(bool nplec);
	bool getPlec();
	void setZainteresowania(bool[], int nsize);
	bool* getZainteresowania();
};

void ASNobject::serialize(std::ostream& dataOut)//!Funkcja, ktora wywoluje odpowiednie metody kodujace poszczegolne elementy 
{

	dataOut << tag;
	codeLength(dataOut);
	codeValue(dataOut);
}

void ASNobject::deserialize(std::istream& dataIn)//!Funkcja, ktora zapisuje w odpowiednim polu klasy zdekodowana wartosc
{
	unsigned char buf;
	int length, nbytes, temp;
	buf = dataIn.get();
	if (buf != tag)
		throw (Exception(INVALID_TAG));
	buf = dataIn.get();
	if ((buf & 128) != 128)//sprawdzenie czy dlugosc jest zakodowana na wiecej niz 1 bajcie
		length = buf;
	else
	{
		length = 0;
		nbytes = buf & 127;
		for (int i = 0; i < nbytes; i++)
		{
			temp = dataIn.get();
			length = temp << (8 * (nbytes - i - 1));
		}
	}

	decodeValue(dataIn, length);
}

void ASNobject::codeLength(std::ostream& dataOut)//!Funkcja kodujaca dlugosc obiektu. Maksymalnie dlugosc moze byc zapisana na 127 bajtach
{
	int length = countLength();
	int comp = 128, comp2 = 255;
	int nbytes;
	nbytes = countLengthBytes(length);
	if (length > 127)
	{

		dataOut.put(128 | nbytes);
		for (int i = 0; i < nbytes; i++)
		{
			comp = 255;
			comp = comp << (8 * (nbytes - 1 - i));
			dataOut.put(length & comp);

		}
	}
	else
	{
		dataOut.put((unsigned char)length);
	}

}

int ASNobject::countLengthBytes(int length)
{
	int nbytes = 1;//trzeba jednego bajta do trzymania informacji na ilu bajtach jest zakodowana dlugosc
	int comp = 128;
	while (comp - 1 < length)
	{
		nbytes++;
		comp = comp << 8;
	}
	if (nbytes > 127)
		throw (Exception(LENGTH_BYTES));
	return nbytes;
}

int sequenceASN::countLength()//!Funkcja liczaca dlugosc sequenceASN'a
{
	unsigned char length = 0;
	unsigned int i;
	for (i = 0; i < tab.size(); i++)
	{
		length = length + tab[i]->countLength();
	}
	length = length + 2 * ((unsigned char)tab.size());

	return length;
}

void sequenceASN::codeValue(std::ostream& dataOut)//!Funkcja kodujaca wartosc sequenceASN'a
{
	for (auto i = tab.begin(); i != tab.end(); i++)
	{
		(*i)->serialize(dataOut);
	}
}

void sequenceASN::decodeValue(std::istream& dataIn, int length)//!Funkcja dekodujaca wartosc sequenceASN'a
{
	int k = 0;
	for (auto i = tab.begin(); i != tab.end(); i++)
	{
		(*i)->deserialize(dataIn);
		k = k + 1 + (*i)->countLength();
		if (k == length) break;
	}
}

void sequenceASN::display()//!Funkcja wyswietlajaca sequenceASN'a
{
	std::cout << std::dec << "sequenceASN: " << std::endl;
	for (unsigned int i = 0; i < tab.size(); i++)
	{
		tab[i]->display();
	}
}

void sequenceASN::saveTofile(const char* filename)//!Funkcja zapisujaca do pliku sequenceASN'a
{

	std::fstream file;
	file.open(filename, std::ios::in | std::ios::binary);
	if (!file.good())
		throw (Exception(FILE_NOT_OPEN));
	file << std::dec << "sequenceASN: " << std::endl;
	for (unsigned int i = 0; i < tab.size(); i++)
	{
		tab[i]->serialize(file);
	}
	file.close();
}

void stringASN::setvalue(std::string n)//!Funkcja wprowadzajaca nowa wartosc
{
	value = n;
}

int stringASN::countLength()//!Funkcja liczaca dlugosc stringa
{
	unsigned char length;
	length = static_cast<unsigned char>(value.length());
	return length;
}

void stringASN::codeValue(std::ostream& dataOut)//!Funkcja kodujaca stringa
{
	for (unsigned int i = 0; i < value.length(); i++)
	{
		dataOut.put(value[i]);
	}

}


void stringASN::decodeValue(std::istream& dataIn, int length)//!Funkcja dekodujaca stringa
{
	std::string nvalue;
	char buf;
	value = "";
	for (int i = 0; i < length; i++)
	{
		buf = dataIn.get();
		value = value + buf;
	}
}

std::string stringASN::getstring()//!Funkcja zwracajaca stringa
{
	return value;
}

void stringASN::display()//!Funkcja wyswietlajaca stringa
{
	std::cout << "stringASN " << value << std::endl;
	std::cout << "ASN.1 code(hex): " << std::endl << std::hex;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	std::cout << std::showbase << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		std::cout << (int)(y[i]) << " ";
	}
	std::cout << std::endl;
}

void stringASN::saveTofile(const char* filename)//!Funkcja zapisujaca do pliku stringa
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out);
	if (!file.good())
		throw (Exception(FILE_NOT_OPEN));
	file << "STRING: " << value << std::endl;
	file << "ASN.1 code(hex): " << std::endl << std::hex;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	file << std::showbase << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		file << (int)(y[i]) << " ";
	}
	file << std::endl;
	file.close();
}

Osoba::Osoba(std::string nimie, std::string nnazwisko, int nwiek, bool nplec, bool* nzainteresowania)
{
	imie.setvalue(nimie);
	nazwisko.setvalue(nnazwisko);
	wiek.setvalue(nwiek);
	std::string arr[] = { "Kobieta", "Mezczyzna" };
	plec.fill(arr, 1);
	plec.setvalue(1);
	std::string arr2[] = { "Narty", "lyzwy", "Ksiazki", "Plywanie" };
	zainteresowania.fill(arr2, 4);
	zainteresowania.setvalue(nzainteresowania, 4);
	tab = { &imie, &nazwisko, &wiek, &plec, &zainteresowania };
}

Osoba::Osoba() {
	tab = { &imie, &nazwisko, &wiek, &plec, &zainteresowania };
	std::string arr[] = { "Kobieta", "Mezczyzna" };
	plec.fill(arr, 1);
	std::string arr2[] = { "Narty", "lyzwy", "Ksiazki", "Plywanie" };
	zainteresowania.fill(arr2, 4);
};

void Osoba::setImie(std::string nimie)
{
	imie.setvalue(nimie);
}
std::string Osoba::getImie()
{
	return imie.getstring();
}
void Osoba::setNazwisko(std::string nnazwisko)
{
	nazwisko.setvalue(nnazwisko);
}
std::string Osoba::getNazwisko()
{
	return nazwisko.getstring();
}
void Osoba::setWiek(int nwiek)
{
	wiek.setvalue(nwiek);
}
int Osoba::getWiek()
{
	return wiek.getvalue();
}
void Osoba::setPlec(bool nplec)
{
	plec.setvalue(nplec);
}
bool Osoba::getPlec()
{
	return plec.getvalue();
}
void Osoba::setZainteresowania(bool* tab, int nsize)
{
	zainteresowania.setvalue(tab, nsize);
}
bool* Osoba::getZainteresowania()
{
	return zainteresowania.getstring();
}

void enumeratedASN::display()//!Funkcja Wyswietlajaca na ekran enuma
{
	std::cout << "enumeratedASN -state(decimal): " << value << std::endl;
	std::cout << "- " << content[value] << std::endl;
	std::cout << "ASN.1 code(hex): " << std::endl << std::hex;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	std::cout << std::showbase << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		std::cout << (int)(y[i]) << " ";
	}
	std::cout << std::endl;
}

void enumeratedASN::saveTofile(const char* filename)//!Funkcja zapisujaca do pliku 
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out);
	if (!file.good())
		throw (Exception(FILE_NOT_OPEN));
	file << "enumeratedASN -state(decimal): " << value << std::endl;
	file << "ASN.1 code(hex): " << std::endl << std::hex;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	file << std::showbase << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		file << (int)(y[i]) << " ";
	}
	file << std::endl;
	file.close();
}

void enumeratedASN::fill(std::string* ntab, int nsize)//!Funkcja przypisujaca nazwy poszczegolnym wartosciom
{
	size = nsize;
	content = new std::string[size + 1];
	for (int i = 0; i < nsize + 1; i++)
	{
		content[i] = ntab[i];
	}
}

void bitstringASN::setvalue(bool* arg, int nsize)
{
	if (nsize != size)
	{
		delete[] bitstring;
		bitstring = new bool[nsize];
		size = nsize;
	}
	for (int i = 0; i < size; i++)
	{
		bitstring[i] = arg[i];
	}
}

void bitstringASN::decodeValue(std::istream& dataIn, int length)
{
	unsigned char comp;
	int k = 0;
	unsigned char buf = dataIn.get();
	if (size != (8 * length - buf - 8))
	{
		delete[] bitstring;
		bitstring = new bool[8 * length - buf - 8];
	}

	for (int i = 1; i < length; i++)
	{
		buf = dataIn.get();
		comp = 128;
		for (int j = 0; j < 8; j++)
		{
			if ((buf & comp) == 0)
				bitstring[k] = 0;
			else
				bitstring[k] = 1;
			comp = comp >> 1;
			k++;
		}
	}
}

bool* bitstringASN::getstring()
{
	return bitstring;
}

int bitstringASN::countLength()
{
	unsigned char length;

	length = size / 8;
	if ((size % 8) != 0)
		length++;
	length++;

	return length;
}
void bitstringASN::codeValue(std::ostream& dataOut)
{
	int length = countLength();
	unsigned char buf;

	if (size % 8 == 0)
		dataOut.put(0);
	else
		dataOut.put(8 - size % 8);
	int k = 0;

	for (int i = 1; i < length; i++)
	{
		int j = 0;
		buf = 0;
		while (j != 8)
		{
			buf = buf << 1;
			buf = buf + bitstring[k];
			j++;
			k++;
			if (k == size)
			{
				buf = buf << (8 - j);
				dataOut.put(buf);
				break;
			}
			if (i > size)
				throw (Exception(ARRAY_OUT_OF_BOUNDS));
		}
		dataOut.put(buf);
	}
}

void bitstringASN::display()
{
	std::cout << "bitstringASN: " << std::dec;
	for (int i = 0; i < size; i++)
		std::cout << bitstring[i];
	std::cout << std::endl;
	for (int i = 0; i < size; i++)
	{
		if (bitstring[i])
			std::cout << content[i] << " ";
	}
	std::cout << std::endl << "ASN.1 code(hex): " << std::endl;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	std::cout << std::showbase << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		std::cout << (unsigned int)(y[i]) << " ";
	}
	std::cout << std::endl;
}

void bitstringASN::saveTofile(const char* filename)
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out);
	if (!file.good())
		throw (Exception(FILE_NOT_OPEN));
	file << "bitstringASN: " << std::dec;
	for (int i = 0; i < size; i++)
		file << bitstring[i];
	file << std::endl << "ASN.1 code(hex): " << std::endl;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	file << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		file << (unsigned int)(y[i]) << " ";
	}
	file << std::endl;
}

void bitstringASN::fill(std::string* ntab, int nsize)
{
	size = nsize;
	content = new std::string[size];
	for (int i = 0; i < nsize; i++)
	{
		content[i] = ntab[i];
	}
}

int integerASN::countLength()//!Funkcja liczaca dlugosc integerASNa
{
	unsigned char length = 1;
	int maxl = 128;
	if (value >= 0)
	{
		while ((maxl - 1) < value)
		{
			length++;
			maxl = maxl << 8;
		}
	}
	else
	{
		while (maxl < (-1)*value)
		{
			length++;
			maxl = maxl << 8;
		}
	}
	return length;
}

void integerASN::codeValue(std::ostream& dataOut)//!Funkcja kodujaca wartosc inta
{
	int length = countLength();
	for (int i = 0; i <length; i++)
	{
		dataOut << (unsigned char)(value >> ((length - 1 - i) * 8) & 0xFF);
	}
}

void integerASN::decodeValue(std::istream& dataIn, int length)//!Funkcja dekodujaca wartosc inta
{
	int temp;
	unsigned char byte;
	unsigned char oldestbyte;
	byte = dataIn.get();
	oldestbyte = byte;
	value = 0;
	for (int i = 0; i < length; i++)
	{
		temp = byte;
		temp = temp << (8 * (length - 1 - i)); //ustawiamy bajt w odpowiednie miejsce
		value = value | temp; //przypisujemy byte do value
		if (i != (length - 1)) //w ostatnim przebiegu petli nie wyciagamy bajtu
			byte = dataIn.get();
	}

	if ((0x80 & oldestbyte) == 128) //jesli pierwszy bit jest rowny 1 to znaczy ze liczba ujemna i trzeba dopelnic jedynkami
	{

		temp = 0xFF;
		temp = temp << 8 * length;
		for (int i = 0; i < (4 - length); i++)
		{
			value = value | temp;
			temp = temp << 8;
		}

	}


}


void integerASN::setvalue(int arg)//!Funkcja przypisujaca to inta nowa wartosc
{
	value = arg;
}


int integerASN::getvalue()//!Funkcja zwracajaca wartosc inta 
{
	return value;
}

void integerASN::display() //!Funkcja wyswietlajaca inta
{
	std::cout << "integerASN(decimal): " << std::dec << value << std::endl;
	std::cout << "ASN.1 code(hex): " << std::endl << std::hex;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	std::cout << std::showbase << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		std::cout << (int)(y[i]) << " ";
	}
	std::cout << std::endl;
}

void integerASN::saveTofile(const char* filename)//!Funkcja zapisujaca do pliku
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out);
	if (!file.good())
		throw (Exception(FILE_NOT_OPEN));
	file << "integerASN(decimal): " << value << std::endl;
	file << "ASN.1 code(hex): " << std::endl << std::hex;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	file << std::showbase << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		file << (int)(y[i]) << " ";
	}
	file << std::endl;
	file.close();

}
