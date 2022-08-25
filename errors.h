#pragma once
#include <exception>

class NodeIndexException : public std::exception
{
public:
	NodeIndexException(int index) : exception(), index_(index) {}
	const char* what() 
	{
		return "error when read node index, expected" + index_;
	}
private:
	int index_;
};

class NodeRandException : public std::exception
{
public:
	NodeRandException(int index) : exception(), index_(index) {}
	const char* what()
	{
		return "error when read rand, node index is " + index_;
	}
private:
	int index_;
};

class NodeLenException : public std::exception
{
public:
	NodeLenException(int index) : exception(), index_(index) {}
	const char* what()
	{
		return "error when read len, node index is " + index_;
	}
private:
	int index_;
};

class NodeDataException : public std::exception
{
public:
	NodeDataException(int index) : exception(), index_(index) {}
	const char* what()
	{
		return "error when read len, node index is " + index_;
	}
private:
	int index_;
};

class CountException : public std::exception
{	
	const char* what()
	{
		return "the number of nodes does not match";
	}
};

class EofException : public std::exception
{
	const char* what()
	{
		return "eof not found";
	}
};