#include<iostream>
#include<fstream>
#include"ASN.1.h"
#include"Exceptions.h"



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
	if (dataIn.eof())
		throw(EndofStream("Stream suddenly ended"));
}

void ASNobject::codeLength(std::ostream& dataOut)//!Funkcja kodujaca dlugosc obiektu. Maksymalnie dlugosc moze byc zapisana na 127 bajtach
{
	int length = countLength();
	int comp = 128, comp2 = 255;
	int nbytes; 
	nbytes=countLengthBytes(length);
	if (length > 127)
	{

		dataOut.put( 128 | nbytes);
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
		throw (TooLong("Data takes too many bytes"));
	return nbytes;
}
