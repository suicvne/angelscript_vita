#include <iostream>  // cout
#include <stdio.h>
#include <angelscript.h>
#ifdef _MSC_VER
	#include <crtdbg.h>  // debugging routines
#endif

#include "gamemgr.h"
#include "scriptmgr.h"

#ifndef VITA
using namespace std;
#endif

CScriptMgr *scriptMgr = 0;
CGameMgr   *gameMgr   = 0;

/**
 * ABOUT THIS MODIFICATION
 * 
 * 'using namespace std' is the bane of my existence.
 * 
 * I simply created a custom VitaCout structure to forward std::cout output to my custom object
 * and print over the network using Vita's libdebugnet. 
 * 
 * This object exists in main as a global object. However, in order to access this object and its overrides
 * in the other source files of this project, you must declare the symbols as follows:
 * 
 * #ifdef VITA
 * extern struct VitaCout cout;
 * extern VitaCout& operator<<(VitaCout& out, const char* input);
 * extern VitaCout& operator<<(VitaCout& out, const std::string &msg);
 * extern VitaCout& operator<<(VitaCout& out, std::ostream& (*pf)(std::ostream&));
 * extern VitaCout& operator<<(VitaCout& out, const int &msg);
 * #endif
 * 
 * Doing this will allow you to override cout in a similar way to what I've done here.
 */
#ifdef VITA
#include <debugnet.h>

#define DBG_IP "192.168.0.45"
#define DBG_PORT 18194

struct VitaCout
{
	VitaCout()
	{
		debugNetInit(DBG_IP, DBG_PORT, DEBUG);
	}

	~VitaCout()
	{
		debugNetFinish();
	}

	friend VitaCout& operator<<(VitaCout& out, const int &msg);
	friend VitaCout& operator<<(VitaCout& out, const std::string &msg);
	friend VitaCout& operator<<(VitaCout& out, std::ostream& (*pf)(std::ostream&));
	friend VitaCout& operator<<(VitaCout& out, const char* input);
};

VitaCout& operator<<(VitaCout& out, const int &msg)
{
	debugNetPrintf(DEBUG, "%d\n", msg);
	return out;
}

VitaCout& operator<<(VitaCout& out, const char* input)
{
	if(input == nullptr) debugNetPrintf(DEBUG, "<null>\n");
	else debugNetPrintf(DEBUG, (std::string(input) + "\n").c_str());
	return out;
}

VitaCout& operator<<(VitaCout& out, const std::string &input)
{
	debugNetPrintf(DEBUG, (input + "\n").c_str());
	return out;
}

typedef std::ostream&(*ostream_manipulator)(std::ostream&);
VitaCout& operator<<(VitaCout& out, std::ostream& (*pf)(std::ostream&))
{

	return operator<<(out, "\n");
}

// Actual definition, making this global
VitaCout cout;
#endif

int main(int argc, char **argv)
{
#ifdef _MSC_VER
	// Detect memory leaks
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF);
	_CrtSetReportMode(_CRT_ASSERT,_CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT,_CRTDBG_FILE_STDERR);

	// Use _CrtSetBreakAlloc(n) to find a specific memory leak
#endif

	int r;

	// Make sure the game is being executed with the correct working directory
	// At the very least there should be a 'player.as' script for controlling the 
	// player character.

	/** Modification 10/7/2021 **/
	/** (For this app, anyway) Vita needs app0: to understand that its relative directory is there. **/	
	/** TODO: Properly resolve relative paths? **/
	FILE *f = fopen("app0:player.as", "r");
	if( f == 0 )
	{
		cout << "The game is not executed in the correct location. Make sure you set the working directory to the path where the 'player.as' script is located." << std::endl;
		cout << std::endl;
		cout << "Press enter to exit." << std::endl;
		char buf[10];
		std::cin.getline(buf, 10);
		return -1;
	}
	else
	{
		cout << "Successfully loaded app0:player.as\n";
	}
	fclose(f);

	cout << "Init gameMgr\n";
	// Initialize the game manager
	gameMgr = new CGameMgr();

	cout << "Init scriptMgr\n";
	// Initialize the script manager
	scriptMgr = new CScriptMgr();
	r = scriptMgr->Init();
	if( r < 0 )
	{
		delete scriptMgr;
		delete gameMgr;

		cout << "ERROR: Unable to init scriptMgr. scriptMgr->init returned " << r << "\n";
		return r;
	}

	// Start a new game
	r = gameMgr->StartGame();
	if( r < 0 )
	{
		if(scriptMgr == nullptr)
		{
			cout << "ScriptMgr is NULL!!!!" << std::endl;
		}

		cout << "Failed to initialize the game. Please verify the script errors." << std::endl;
		cout << std::endl;

		cout << "Press enter to exit." << std::endl;
		char buf[10];
		std::cin.getline(buf, 10);
		return -1;
	}
	else
	{
		cout << "Start Game successful...\n";
	}

	cout << "gameMgr->Run();\n";
	// Let the game manager handle the game loop
	gameMgr->Run();
	
	// Uninitialize the game manager
	if( gameMgr )
	{
		delete gameMgr;
		gameMgr = 0;
	}

	// Uninitialize the script manager
	if( scriptMgr )
	{
		delete scriptMgr;
		scriptMgr = 0;
	}

	return r;
}



