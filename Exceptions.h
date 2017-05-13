#pragma once
#include<string>
#include<stdexcept>
class UnkownTag : public std::runtime_error
{
public:
	UnkownTag(const char* what) :runtime_error(what) {};
};

class FileNotopen : public std::runtime_error
{
public:
	FileNotopen(const char* what) :runtime_error(what) {};
};

class ArrayOut: public std::runtime_error
{
public:
	ArrayOut(const char* what) :runtime_error(what) {};
};

class BadEnum : public std::runtime_error
{
public:
	BadEnum(const char* what) :runtime_error(what) {};
};

class TooLong : public std::runtime_error
{
public:
	TooLong(const char* what) :runtime_error(what) {};
};

class EndofStream : public std::runtime_error
{
public:
	EndofStream(const char* what) :runtime_error(what) {};
};
