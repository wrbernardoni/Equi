#include "EquiFrame.h"
#include "EquiConsole.h"
#include "EquiInterface.h"

EquiFrame::EquiFrame()
{
	delType = true;
	delTok = true;
	dTyL = 0;
	dToL = 0;
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

EquiFrame::EquiFrame(int i)
{
	delType = true;
	delTok = true;
	dTyL = 0;
	dToL = 0;

	map<string, EquiObject*>* tok = new map<string, EquiObject*>;
	map<string, EquiObject*>* typ = new map<string, EquiObject*>;
	tokens.push_back(tok);
	types.push_back(typ);
}

EquiFrame::EquiFrame(const EquiFrame& o)
{
	delType = true;
	delTok = true;
	dTyL = 0;
	dToL = 0;

	setTokens(o.tokens);
	setTypes(o.types);
}

EquiFrame::EquiFrame(EquiFrame* o, bool dTo, bool dTy)
{
	delType = dTy;
	delTok = dTo;
	dTyL = 0;
	dToL = 0;

	if (!dTy)
		copyTypes(o->types);
	else
		setTypes(o->types);

	if (!dTo)
		copyTokens(o->tokens);
	else
		setTokens(o->tokens);
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
	int i = 0;
	for (auto x : tokens)
	{
		if (delTok || i > dToL)
		{
			for (auto y : *x)
				delete y.second;
		}
		i++;

		delete x;
	}

	tokens.clear();

	i = 0;
	for (auto x : types)
	{
		if (delType || i > dTyL)
		{
			for (auto y : *x)
				delete y.second;
		}
		i++;

		delete x;
	}
	
	types.clear();
}

void EquiFrame::setTokens(const deque<map<string, EquiObject*>*>& o)
{
	int i = 0;
	for (auto x : tokens)
	{
		if (delTok || i > dToL)
		{
			for (auto y : *x)
				delete y.second;
		}
		i++;

		delete x;
	}
	tokens.clear();

	delTok = true;
	dToL = 0;

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

void EquiFrame::copyTokens(const deque<map<string, EquiObject*>*>& o)
{
	int i = 0;
	for (auto x : tokens)
	{
		if (delTok || i > dToL)
		{
			for (auto y : *x)
				delete y.second;
		}

		i++;

		delete x;
	}
	tokens.clear();

	delTok = false;
	dToL = 0;

	for (auto x : o)
	{
		map<string, EquiObject*>* tok = new map<string, EquiObject*>;
		for (auto y : *x)
		{
			EquiObject* o = y.second;
			(*tok)[y.first] = o;
		}

		tokens.push_back(tok);
		dToL++;
	}
}

void EquiFrame::setTypes(const deque<map<string, EquiObject*>*>& o)
{
	//cout << "Del" << endl;
	int i = 0;
	for (auto x : types)
	{
		if (delType || i > dTyL)
		{
			for (auto y : *x)
				delete y.second;
		}
		i++;

		delete x;
	}
	types.clear();

	delType = true;
	dTyL = 0;
	//cout << "load" << endl;
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

void EquiFrame::copyTypes(const deque<map<string, EquiObject*>*>& o)
{
	int i = 0;
	for (auto x : types)
	{
		if (delType || i > dTyL)
		{
			for (auto y : *x)
				delete y.second;
		}
		i++;

		delete x;
	}
	types.clear();

	delType = false;
	dTyL = 0;

	for (auto  x : o)
	{
		map<string, EquiObject*>* tok = new map<string, EquiObject*>;
		for (auto y : *x)
		{
			EquiObject* o = y.second;
			(*tok)[y.first] = o;
		}

		types.push_back(tok);
		dTyL++;
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

vector<pair<string, EquiObject*>> EquiFrame::apparentTokens()
{
	map<string, EquiObject*> out;
	for (auto x : tokens)
	{
		for (auto y : *x)
		{
			out[y.first] = y.second->clone();
		}
	}

	vector<pair<string, EquiObject*>> toAdd;

	for (auto y : out)
	{
		toAdd.push_back(y);
	}

	return toAdd;
}
