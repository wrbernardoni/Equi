#ifndef EQUI_FRAME_H_
#define EQUI_FRAME_H_

#include "global.h"
#include "AllObj.h"

#include <map>
#include <string>
#include <deque>

using namespace std;

class EquiFrame
{
public:
	deque<map<string, EquiObject*>*> tokens;
	deque<map<string, EquiObject*>*> types;

	EquiFrame();
	EquiFrame(const EquiFrame&);
	~EquiFrame();

	EquiObject* getToken(string);
	EquiObject* getType(string);

	void setTokens(const deque<map<string, EquiObject*>*>&);
	void setTypes(const deque<map<string, EquiObject*>*>&);

	EquiFrame& operator=(const EquiFrame&);
	void clear();
};

#endif