#include "parse.h"
#include "global.h"

#include <iostream>

vector<string> getLine(istream* in, int& lineNum)
{
  vector<string> line;
 
  string word = "";
  string currentOp = "";
  bool blockComment = false;
  bool lineComment = false;
  bool potentialComment = false;
  bool instring  = false;
  int scope = 0;
  int paren = 0;
  bool oneMore = false;
  bool bs = false;
  bool bsResolv = true;

  do
  {
    bool nextOne = oneMore;
    int pSize = line.size();
    char ln[512];
    in->getline(ln, 512);
    lineComment = false;
    if (in->fail() && !in->eof())
    {
      cerr << "Delimiter not found on line " << lineNum + 1 << ", too long?\n";
      throw 1;
    }
    if (in->eof() && scope > 0)
    {
      cerr << "Unmatched {" << endl;
      throw 1;
    }
    else if (in->eof() && paren > 0)
    {
      cerr << "Unmatched (" << endl;
      throw 1;
    }
    else if (in->eof() && oneMore)
    {
      cerr << "Input ended while expecting another statement." << endl;
      throw 1;
    }

    lineNum++;

    for (int i = 0; i < 256; i++)
    {
      if (ln[i] == '{')
        scope++;
      if (ln[i] == '}')
        scope--;
      if (ln[i] == '(')
        paren++;
      if (ln[i] == ')')
        paren--;

      if (scope == 0 && line.size() != 0 && (line[line.size()-1] == "function" || line[line.size()-1] == "task"))
        oneMore = true;

      if (scope < 0)
      {
        cerr << "Unmatched } on line: " << lineNum << endl;
        throw 1;
      }
      if (paren < 0)
      {
        cerr << "Unmated ) on line: " << lineNum << endl;
        throw 1;
      }

      if (ln[i] == '\n' || ln[i] == '\0')
        break;

      if (lineComment)
        break;
      else if (blockComment)
      {
        static bool escape = false;
        if (ln[i] == '*')
          escape = true;
        else if (escape && ln[i] == '/')
          blockComment = false;
        else
          escape = false;
  
        continue;
      }

     if (potentialComment && ln[i] == '*')
     {
        blockComment = true;
        potentialComment = false;
        continue;
      }
      else if (potentialComment && ln[i] == '/')
      {
        lineComment = true;
        potentialComment = false;
        continue;
      }
      else if (potentialComment)
      {
        if (word != "")
          line.push_back(word);
        line.push_back("/");
        word = "";
        potentialComment = false;
      }

      if (bs)
      {
        bsResolv = false;
        if (instring && ln[i] == '\\')
        {
          word = word + "\\";
        }
        else if (instring && ln[i] == '\"')
        {
          word = word + "\"";
        }
        else if (ln[i] == 'n')
        {
          word = word + "\n";
        }
        else if (ln[i] == 't')
        {
          word = word + "\t";
        }
        else if (ln[i] == 'r')
        {
          word = word + "\r";
        }
        else if (ln[i] == 'a')
        {
          word = word + "\a";
        }
        else
        {
          word = word + "\\";
          bsResolv = true;
        }
        bs = false;
      }
      else
      {
        bsResolv = true;
        if (ln[i] == '\\')
        {
          bs = true;
          bsResolv = false;
        }
      }

      if (!bsResolv)
      {
      }
      else if (iswspace(ln[i]) && !instring)
      {
        if (word != "")
        {
          line.push_back(word);
          word = "";
        }
      }
      else if (ln[i] == '/' && !instring)
      {
        potentialComment = true;
      }
      else if (ln[i] == '"')
      {
        if (!instring)
        {
          if (word != "")
          {
            line.push_back(word);
            word = ln[i];
          }
          else
          {
            word = ln[i];
          }
        }
        else
        {
          line.push_back(word + "\"");
          word = "";
        }
        instring = !instring;
      }
      else if (!instring && !isalnum(ln[i]) && ln[i] != '$' && ln[i] != '_' && ln[i] != '.')
      {
       if (word != "")
           line.push_back(word);
        word = "";

        if (line.size() != 0)
        {
          if (ln[i] == '=' && (line[line.size()-1] == "!" || line[line.size()-1] == "=" || line[line.size()-1] == ">" || line[line.size()-1] == "<"))
            line[line.size()-1] = line[line.size()-1] + ln[i];
          else if (ln[i] == '+' && line[line.size()-1] == "+")
            line[line.size()-1] = line[line.size()-1] + ln[i];
          else if (ln[i] == '-' && line[line.size()-1] == "-")
            line[line.size()-1] = line[line.size()-1] + ln[i];
          else if (ln[i] == ':' && line[line.size()-1] == ":")
            line[line.size()-1] = line[line.size()-1] + ln[i];
          else if (ln[i] == '>' && line[line.size()-1] == "-")
            line[line.size()-1] = line[line.size()-1] + ln[i];
          else if (ln[i] == '|' && line[line.size()-1] == "|")
            line[line.size()-1] = line[line.size()-1] + ln[i];
          else if (ln[i] == '&' && line[line.size()-1] == "&")
            line[line.size()-1] = line[line.size()-1] + ln[i];
          else
          {
            string temp = "";
            temp = temp + ln[i];
            line.push_back(temp);
          }   
        }
        else
        {
          string temp = "";
          temp = temp + ln[i];
          line.push_back(temp);
        } 
      }
      else
      {
        word = word + ln[i];
      }
    }
    if (word != "")
    {
      line.push_back(word); 
      word = "";
    }

    if (nextOne && line.size() > pSize)
      oneMore = false;

    if (line.size() > 0 && !(line[line.size() - 1] == "}" || line[line.size() - 1] == ";"))
      oneMore = true;


  } while (blockComment || scope > 0 || paren > 0 || oneMore);

  return line;
}

