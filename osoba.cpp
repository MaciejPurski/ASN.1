
#include"Osoba.h"


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