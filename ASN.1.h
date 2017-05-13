#pragma once
#include"Exceptions.h"
#include<iostream>
#include<string>
#include<vector>
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


class ASNobject:public IDisplayable, IStorable//!klasa interfejsu kodujacego i dekodujacego
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



