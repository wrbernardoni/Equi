#include "parse.h"
#include "global.h"

#include <iostream>
#include <fstream>

vector<string> tokenize(string fn)
{
	P_VERB("Parsing file\n", ALL_STEP_VERB);
	ifstream in(fn);
	in >> std::noskipws;

	vector<string> formatted;
	string currentOp = "";
	string currentWord = "";
	bool blockComment = false;
	bool lineComment = false;
	bool potentialComment = false;
	bool instring = false;
	while (in.good())
	{
		char c = in.get();

		if (lineComment)
		{
			if (c == '\n')
				lineComment = false;

			continue;
		}
		else if (blockComment)
		{
			static bool escape = false;
			if (c == '*')
				escape = true;
			else
			{
				if (escape && c == '/')
					blockComment = false;
				escape = false;
			}

			continue;
		}

		if (potentialComment && c == '*')
		{
			blockComment = true;
			potentialComment = false;
			continue;
		}
		else if (potentialComment && c == '/')
		{
			lineComment = true;
			potentialComment = false;
			continue;
		}
		else if (potentialComment)
		{
			if (currentWord != "")
				formatted.push_back(currentWord);
			formatted.push_back("/");
			currentWord = "";
			potentialComment = false;
		}

		if (iswspace(c) && !instring)
		{
			if (currentWord != "")
			{
				if (currentWord != "")
					formatted.push_back(currentWord);
				currentWord = "";
			}
		}
		else if (c == '/' && !instring)
		{
			potentialComment = true;
		}
		else if (c == '"')
		{
			if (!instring)
			{
				if (currentWord != "")
				{
					if (currentWord != "")
						formatted.push_back(currentWord);
					currentWord = c;
				}
				else
				{
					currentWord = c;
				}
			}
			else
			{
				formatted.push_back(currentWord + "\"");
				currentWord = "";
			}
			instring = !instring;
		}
		else if (!instring && 
		(!isalnum(c) && c != '$' && c != '@' && c != '_' && c != '-'))
		{
			if (currentWord != "")
				formatted.push_back(currentWord);
			string temp = "";
			temp = temp + c;
			formatted.push_back(temp);
			currentWord = "";
		}
		else
		{
			currentWord = currentWord + c;
		}
	}

	if (formatted.size() == 0)
	{
		cerr << "No code found.\n";
		exit(1);
	}

	formatted.pop_back();

	for (int i = 0; i < formatted.size(); i++)
	{
		P_VERB(formatted[i] << "\n", ALL_STEP_VERB);
	}

	return formatted;
}
