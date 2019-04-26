#include "EquiFrame.h"
#include "EquiConsole.h"
#include "EquiInterface.h"

EquiFrame::EquiFrame()
{
	map<string, EquiObject*>* tok = new map<string, EquiObject*>;

	EquiVoid* vd = new EquiVoid;
	(*tok)["void"] = vd;

	loadInterface(tok);
	loadConsoleStd(tok);
	tokens.push_back(tok);

	map<string, EquiObject*>* typ = new map<string, EquiObject*>;
	(*typ)["int"] = new EquiPrimitive<int>;
	(*typ)["bool"] = new EquiPrimitive<bool>;
	(*typ)["float"] = new EquiPrimitive<float>;
	(*typ)["double"] = new EquiPrimitive<double>;
	(*typ)["long"] = new EquiPrimitive<long>;
	(*typ)["string"] = new EquiString;
	(*typ)["()"] = new EquiTuple;
	(*typ)["function"] = new EquiFunctionHandle;
	types.push_back(typ);
}

EquiFrame::EquiFrame(const EquiFrame& o)
{
	setTokens(o.tokens);
	setTypes(o.types);
}

bool EquiFrame::isToken(string n)
{
	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i]->count(n) != 0)
		{
			return true;
		}
	}
	return false;
}

void EquiFrame::emplaceToken(string n, EquiObject* o)
{
	if (tokens[tokens.size() - 1]->count(n) != 0)
		throwError("Token " + n + " already defined");

	o->setTemp(false);
	(*tokens[tokens.size() - 1])[n] = o;
}

bool EquiFrame::isType(string n)
{
	for (int i = 0; i < types.size(); i++)
	{
		if (types[i]->count(n) != 0)
		{
			return true;
		}
	}
	return false;
}

void EquiFrame::emplaceType(string n, EquiObject* o)
{
	if (types[types.size() - 1]->count(n) != 0)
		throwError("Token " + n + " already defined");

	o->setTemp(false);
	(*types[types.size() - 1])[n] = o;
}

EquiFrame& EquiFrame::operator=(const EquiFrame& o)
{
	setTokens(o.tokens);
	setTypes(o.types);

	return *this;
}

EquiFrame::~EquiFrame()
{
	clear();
}

void EquiFrame::clear()
{
	for (auto x : tokens)
	{
		for (auto y : *x)
			delete y.second;

		delete x;
	}

	tokens.clear();

	for (auto x : types)
	{
		for (auto y : *x)
			delete y.second;

		delete x;
	}

	types.clear();
}

void EquiFrame::setTokens(const deque<map<string, EquiObject*>*>& o)
{
	for (auto x : tokens)
	{
		for (auto y : *x)
			delete y.second;

		delete x;
	}

	tokens.clear();

	for (auto x : o)
	{
		map<string, EquiObject*>* tok = new map<string, EquiObject*>;
		for (auto y : *x)
		{
			EquiObject* o = y.second->spawnMyType();
			*o = *y.second;
			o->setTemp(false);
			(*tok)[y.first] = o;
		}

		tokens.push_back(tok);
	}
}

void EquiFrame::setTypes(const deque<map<string, EquiObject*>*>& o)
{
	for (auto  x : types)
	{
		for (auto  y : *x)
			delete y.second;

		delete x;
	}

	types.clear();

	for (auto  x : o)
	{
		map<string, EquiObject*>* tok = new map<string, EquiObject*>;
		for (auto y : *x)
		{
			EquiObject* o = y.second->spawnMyType();
			*o = *y.second;
			o->setTemp(false);
			(*tok)[y.first] = o;
		}

		types.push_back(tok);
	}
}

EquiObject* EquiFrame::getToken(string n)
{
	for (int i = tokens.size() - 1; i >= 0; i--)
	{
		if (tokens[i]->count(n) != 0)
		{
			return (*tokens[i])[n];
		}
	}
	throwError("Token " + n + " not found");
	return NULL;
}

EquiObject* EquiFrame::getType(string n)
{
	for (int i = types.size() - 1; i >= 0; i--)
	{
		if (types[i]->count(n) != 0)
		{
			return (*types[i])[n];
		}
	}
	throwError("Type " + n + " not found");
	return NULL;
}
