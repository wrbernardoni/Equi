#ifndef EQUI_FRAME_H_
#define EQUI_FRAME_H_

#include "global.h"
#include "AllObj.h"

#include <map>
#include <string>
#include <deque>

using namespace std;

class EquiFrame : public EquiObject
{
private:
	bool delType;
	bool delTok;
	int dTyL;
	int dToL;

	void copyTokens(const deque<map<string, EquiObject*>*>&);
	void copyTypes(const deque<map<string, EquiObject*>*>&);
public:
	virtual inline string getType() { return "FRAME"; };

	deque<map<string, EquiObject*>*> tokens;
	deque<map<string, EquiObject*>*> types;

	bool isToken(string);
	void emplaceToken(string, EquiObject*);
	bool isType(string);
	void emplaceType(string, EquiObject*);

	EquiFrame();
	EquiFrame(const EquiFrame&);
	EquiFrame(EquiFrame*, bool, bool);
	EquiFrame(int);
	~EquiFrame();

	EquiObject* getToken(string);
	EquiObject* getType(string);

	void setTokens(const deque<map<string, EquiObject*>*>&);
	void setTypes(const deque<map<string, EquiObject*>*>&);

	vector<pair<string, EquiObject*>> apparentTokens();

	EquiFrame& operator=(const EquiFrame&);
	void clear();
};

#endif