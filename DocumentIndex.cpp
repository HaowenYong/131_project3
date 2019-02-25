//****************************************************************************************
//
//	INCLUDE FILES
//
//****************************************************************************************
#include	<fstream>
#include	<iostream>
#include	<string>
#include    <sstream>

#include <set>
#include <map>

//#include	<unistd.h>

#include	"DocumentIndex.h"
#include	"GetLine.h"

using namespace std;

//****************************************************************************************
//
//	CONSTANT DEFINITIONS
//
//****************************************************************************************

//****************************************************************************************
//
//	CLASSES, TYPEDEFS AND STRUCTURES
//
//****************************************************************************************
typedef	string::size_type	StringSize;
typedef map<string, set<int>> MyMap;
typedef MyMap::value_type MyMapValue;
typedef MyMap::iterator MyMapIterator;

//****************************************************************************************
//
//	PUBLIC DATA
//
//****************************************************************************************

//****************************************************************************************
//
//	PRIVATE DATA
//
//****************************************************************************************

//****************************************************************************************
//
//	FUNCTION PROTOTYPES
//
//****************************************************************************************

//****************************************************************************************
//
//	DocumentFile::Close
//
//****************************************************************************************
void	DocumentFile::Close()
{
	//************************************************************************************
	//	LOCAL DATA
	
	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	
	file_.close();
	file_.clear();


	return;
}

//****************************************************************************************
//
//	DocumentFile::GetPageNumber
//
//****************************************************************************************
int	DocumentFile::GetPageNumber()
{
	//************************************************************************************
	//	LOCAL DATA
	
	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	
	return(pageNumber_);
}

//****************************************************************************************
//
//	DocumentFile::GetWord
//
//****************************************************************************************
string	DocumentFile::GetWord()
{
	//************************************************************************************
	//	LOCAL DATA
	string	word;
	string temp = "";
	string line= "";
	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	if (text_ == "")
		return "";
	istringstream iss(text_);
	iss >> word;
	if (word.length() == text_.length())
	{
		text_ = "";
	}
	else
	{
		for (int i = word.length() + 1; i < text_.length(); i++)
		{
			line = line + text_[i];
		}
		text_ = line;
		line = "";
	}
	//cout << "GW" << text_ << "GW" << endl;

	temp = "";
	int i = 0;
	for (i = 0; i < word.length(); i++)
	{
		if (word[i] == '\'')
		{
			if (i != 0 && i != word.length() - 1)
			{
				if (!(word[i + 1] == 's' && word[i + 1] == word.length() - 1))
				{
					/*
					i = word.length() + 10;
					continue;
					*/
					temp = GetWord();
					break;
				}
			}
		}
		else if (isalpha(word[i]) == false)
		{
			if (word.length() == 1)
			{
				temp = GetWord();
				break;
			}
			if (i != word.length() - 1 && i != 0)
			{
				/*
				i = word.length() + 10;
				continue;
				*/
				temp = GetWord();
				break;
			}
		}
		else temp = temp + word[i];
	}
	/*
	cout << "GW i=" << i << "i GW" << endl;
	if ((i < word.length()) || (i== (word.length() + 10 )) || (i == 1)) 
		GetWord();
	*/
	for (vector<string>::iterator naranja = v.begin(); naranja != v.end(); ++naranja)
	{
		if (temp == *naranja)
			temp = "exclude";
	}
	
	while (temp == "exclude")
	{
		temp = GetWord();
	}
	/*
	while (temp == "")
	{
		if (temp == "end")
			return "end";
		temp = GetWord();
	}
	*/
	//cout << "WORD" << temp << "WORD" << endl;
	return(temp);
}

//****************************************************************************************
//
//	DocumentFile::LoadExclusions
//
//****************************************************************************************

bool	DocumentFile::LoadExclusions(const string& name)
{
	//************************************************************************************
	//	LOCAL DATA
	bool	success;

	exclusion.open(name);
	if (exclusion.fail())
		success = false;
	else success = true;

	string s;
	string line = "";
	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	
	while (!exclusion.eof())
	{
		GetLine(exclusion, line);
		if (line == "") break;

		istringstream iss(line);
		while (!iss.eof())
		{
			iss >> s;
			v.push_back(s);
		}
	}

	
	//for (vector<string>::iterator l = v.begin(); l != v.end(); ++l)
	//{
		//cout << *l << endl;
	//}
	
	return(success);
}


//****************************************************************************************
//
//	DocumentFile::Open
//
//****************************************************************************************
bool	DocumentFile::Open(const string& name)
{
	//************************************************************************************
	//	LOCAL DATA
	//bool	success;


	file_.open(name, ios::in);
	if (!file_.fail())
	{
		//	You may add any useful initialization here.
		return(true);
	}
	else
	{
		return(false);
	}


	//************************************************************************************
	//	EXECUTABLE STATEMENTS

	//return(success);
}

//****************************************************************************************
//
//	DocumentFile::Read
//
//****************************************************************************************
bool	DocumentFile::Read()
{
	//************************************************************************************
	//	LOCAL DATA
	bool	success = true;

	if (file_.is_open())
		success = true;
	else
		return false;
	string temp;
	int emptyLine = 0;
	//************************************************************************************
	//	EXECUTABLE STATEMENTS
		 GetLine(file_, text_);
		 //cout << "aa" << text_ << "bb" << endl;

		 if (file_.eof())
			 return false;

		 while (!file_.eof() && text_ == "")
		 {
			 emptyLine++;
			 GetLine(file_, text_);
		 }

		 if (file_.eof())
			 return false;

		 if (emptyLine > 1)
		 {
			 pageNumber_++;
			 emptyLine = 0;
		 }
		 //cout << "READ" << text_ << "READ" << endl;
		 return true;
		 
	/*
	 for (map<string, set<int>>::iterator apple = myMap.begin(); apple != myMap.end(); ++apple)
	 {
		 if (apple->second.size() <= 10)
		 {
			 cout << apple->first << endl;
			 cout << "\t";

			 for (set<int>::iterator banana = apple->second.begin(); banana != apple->second.end(); ++banana)
			 {
				 cout << *banana;
				 cout << " ";
			 }

			 cout << "\n";
		 }
	 }
	 */
	
	return(success);
}

//****************************************************************************************
//
//	DocumentIndex::Create
//
//****************************************************************************************
void	DocumentIndex::Create(DocumentFile& documentFile)
{
	//************************************************************************************
	//	LOCAL DATA

	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	while (documentFile.Read() == true)
	{
		string word = documentFile.GetWord();
		//cout << "WW" << word << "11" << endl;
		while (word != "")
		{
			/*
			if (word == "exclude") {
				word = documentFile.GetWord();
				continue;
			}

			if (word == "fa-il")
			{
				word = documentFile.GetWord();
				continue;
			}
			*/
			//string word = documentFile.GetWord();
			int page = documentFile.GetPageNumber();
			bool exist = false;
			for (map<string, set<int>>::iterator i = m.begin(); i != m.end(); ++i)
			{
				if (i->first == word)
				{
					exist = true;
					i->second.insert(page);
				}
			}

			if (exist == false)
			{
				set<int> s;
				s.insert(page);
				m.insert(MyMapValue(word, s));
			}


			bool exist1 = false;
			for (map<string, int>::iterator j = c.begin(); j != c.end(); ++j)
			{
				if (j->first == word)
				{
					exist1 = true;
					j->second = j->second + 1;
				}
			}

			if (exist1 == false)
			{
				c.insert(map<string, int>::value_type(word, 1));
			}





			//cout << "CREATE" << word << "CREATE" << endl;
			word = documentFile.GetWord();

			//cout << "CREATE" << word << "CREATE" << endl;
		}
	}
	
	return;
}

//****************************************************************************************
//
//	DocumentIndex::Write
//
//****************************************************************************************
void	DocumentIndex::Write(ostream& indexStream)
{
	//************************************************************************************
	//	LOCAL DATA
	
	//************************************************************************************
	//	EXECUTABLE STATEMENTS

	/*
	for (map<string, set<int>>::iterator avocado = m.begin(); avocado != m.end(); ++avocado)
	{
	cout << "MAP";
	cout << avocado->first;
	cout << "\t";
	for (set<int>::iterator apricot = avocado->second.begin(); apricot != avocado->second.end(); ++apricot)
	{
	cout << *apricot;
	cout << " ";
	}
	cout << "MAP";
	cout << endl;
	}
	*/

	for (map<string, set<int>>::iterator grape = m.begin(); grape != m.end(); ++grape)
	{
		bool excessive = false;

		if (grape->second.size() > 10)
			continue;

		for (map<string, int>::iterator pear = c.begin(); pear != c.end(); ++pear)
		{
			if (pear->first == grape->first)
				if (pear->second > 10)
					excessive = true;
		}

		if (excessive == false)
		{
			indexStream << grape->first;
			indexStream << " ";
			cout << grape->first;
			cout << " ";
			for (set<int>::iterator peach = grape->second.begin(); peach != grape->second.end(); ++peach)
			{
				indexStream << *peach;
				cout << *peach;
				if (peach == --grape->second.end())
				{
					indexStream << "\n";
					cout << "\n";
				}
				else
				{
					indexStream << ", ";
					cout << ", ";
				}
			}
		}
	}

	return;
}
