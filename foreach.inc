/*----------------------------------------------------------------------------*\
					===========================
					 foreach efficient looping
					===========================
Description:
	Provides efficient looping through sparse data sets, such as connected
	players.  Significantly improved from the original version to be a generic
	loop system, rather then purely a player loop system.  When used for
	players this has constant time O(n) for number of connected players (n),
	unlike standard player loops which are O(MAX_PLAYERS), regardless of the
	actual number of connected players.  Even when n is MAX_PLAYERS this is
	still faster.
Legal:
	Version: MPL 1.1
	
	The contents of this file are subject to the Mozilla Public License Version 
	1.1 (the "License"); you may not use this file except in compliance with 
	the License. You may obtain a copy of the License at 
	http://www.mozilla.org/MPL/
	
	Software distributed under the License is distributed on an "AS IS" basis,
	WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
	for the specific language governing rights and limitations under the
	License.
	
	The Original Code is the YSI foreach include.
	
	The Initial Developer of the Original Code is Alex "Y_Less" Cole.
	Portions created by the Initial Developer are Copyright (C) 2011
	the Initial Developer. All Rights Reserved.
	
	Contributors:
		ZeeX, koolk, JoeBullet/Google63, g_aSlice/Slice, Kar
	
	Thanks:
		JoeBullet/Google63 - Handy arbitrary ASM jump code using SCTRL.
		ZeeX - Very productive conversations.
		koolk - IsPlayerinAreaEx code.
		TheAlpha - Danish translation.
		breadfish - German translation.
		Fireburn - Dutch translation.
		yom - French translation.
		50p - Polish translation.
		Zamaroht - Spanish translation.
		Dracoblue, sintax, mabako, Xtreme, other coders - Producing other modes
			for me to strive to better.
		Pixels^ - Running XScripters where the idea was born.
		Matite - Pestering me to release it and using it.
	
	Very special thanks to:
		Thiadmer - PAWN, whose limits continue to amaze me!
		Kye/Kalcor - SA:MP.
		SA:MP Team past, present and future - SA:MP.
	
Version:
	19
Changelog:
	14/05/15:
		Cleaned up the entire include.
	16/07/12:
		Fixed a bug with "loadfs" blocking callbacks.
		Slightly reduced the memory consumption of constant strings.
	25/05/12:
		Added a delay timer to remove players for "Kick" crashes.
	05/01/12:
		Fixed multi-dimensional iterators.
		Fixed "FOREACH_NO_BOTS".
		Made "Iterator:" support multi-dimensional arrays.
	07/12/11:
		Underlying implementation tweak for more consistent code.
		Added Iter_Contains.
	06/12/11:
		Minor fixes.
	31/10/11:
		Changed the underlying loop code to be slightly faster.
		Added support for Iter_SafeRemove, prompting refactoring.
	17/09/11:
		Fixed arrays under the new syntax.
	28/04/11:
		Moved iterator identifiers to end of variables.
		Rewrote "foreach" to accept two syntaxes for "foreach (new i : Iter)".
	16/08/10:
		Removed all the "2" versions of the functions.
	14/08/10:
		Added Iter_Clear to reset an array.
	06/08/10:
		Added special array declaration format.
	18/12/09:
		Added Iter_Func2 functions for multi-dimensional iterators.
		Renamed foreact et al as keywords in the documentation.
		Changed licensing from GPL to MPL.
	02/09/09:
		Fixed (again) for 0.3.
		Added free slot finding.
	21/08/09:
		Updated to include random functions.
		Made entirely stand alone.
		Ported to 0.3 (separate version).
		Added automatic callback hook code.
		Removed debug information from stand alone version.
	06/01/08:
		Added debug information.
	09/10/07:
		Moved to system.
	16/09/07:
		Added list sorting.
		Made this part of Y SeRver Includes, not Y Sever Includes.
		Made list sorting optional.
		Fixed version number.
	08/09/07:
		First version.
Functions:
	Public:
		OnPlayerDisconnect - Called when a player leaves to remove them.
		OnPlayerConnect - Called when a player connects to add them.
	Core:
		-
	Stock:
		Iter_ShowArray - Displays the contents of the array. // Disabled
		Iter_AddInternal - Add a value to an iterator.
		Iter_RemoveInternal - Remove a value from an iterator.
		Iter_RandomInternal - Get a random item from an iterator.
		Iter_FreeInternal - Gets the first free slot in the iterator.
		Iter_InitInternal - Initialises a multi-dimensional iterator.
		Iter_ContainsInternal - Checks if a value is in an iterator.
	Static:
		-
	Inline:
		Iter_Create - Create a new iterator value set.
		Iter_Add - Wraps Iter_AddInternal.
		Iter_SafeRemove - Wraps Iter_SafeRemoveInternal.
		Iter_Remove - Wraps Iter_RemoveInternal.
		Iter_Random - Wraps Iter_RandomInternal.
		Iter_Count - Gets the number of items in an iterator.
		Iter_Debug - Wraps around Iter_ShowArray. // Disabled
		Iter_Free - Wraps around Iter_FreeInternal.
		Iter_Contains - Wraps around Iter_ContainsInternal.
	API:
		-
Callbacks:
	-
Hooks:
	Iter_OnPlayerConnect - Hook for the OnPlayerConnect callback.
	Iter_OnPlayerDisconnect - Hook for the OnPlayerDisconnect callback.
	Iter_OnGameModeInit - Only exists to make the code compile correctly...
Definitions:
	-
Enums:
	-
Macros:
	-
Keywords:
	foreach - Function to loop an iterator.
	foreachex - Like foreach but without a new variable.
Tags:
	Iterator - Declare an iterator.
Variables:
	Global:
		-
	Static:
		YSI_g_OPC - Records whether Iter_OnPlayerConnect exists for speed.
		YSI_g_OPDC - Records whether Iter_OnPlayerDisconnect exists for speed.
Commands:
	-
Compile options:
	FOREACH_NO_BOTS - Remove the bot iterators for smaller code.
	FOREACH_NO_PLAYERS - Remove all default code for player iteration.
Operators:
	-
Iterators:
	Player - List of all players connected.
	Bot - List of all bots (npcs) connected.
	NPC - Alias of Bot.
	Character - All players and bots.
\*----------------------------------------------------------------------------*/

#define _FOREACH_LOCAL_VERSION		19

// Foreach is testing us.
#if defined _FOREACH_INC_TEST
	#define _FOREACH_CUR_VERSION _FOREACH_LOCAL_VERSION
	#endinput
#endif
#if !defined _FOREACH_NO_TEST
	#define _FOREACH_INC_TEST
	#tryinclude <YSI\y_iterate>
	#undef _FOREACH_INC_TEST
	// <foreach> exists - test which is newer.
	#if defined _inc_y_iterate
		#if _FOREACH_CUR_VERSION > _FOREACH_LOCAL_VERSION
			// Foreach is newer.
			#undef _inc_y_iterate
			#define _FOREACH_NO_TEST
			#include <YSI\y_iterate>
			#endinput
		#endif
	#endif
#endif

#if !defined _samp_included
	#error "Please include a_samp or a_npc before foreach"
#endif

#if defined SendChat || defined FOREACH_NO_PLAYERS
	#define BOTSYNC_IS_BOT		(true)
#endif

#if defined IsPlayerNPC
	#define _FOREACH_BOT
#endif

#define _Y_ITER_ARRAY: _:_Y_ITER_C0:
#define _Y_ITER_ARRAY_SIZE(%1) _:_Y_ITER_C1:_Y_ITER_C2:sizeof %1@YSII_Ag-1

#define _Y_ITER_C0:%0[%1]@YSII_%4g%3) %0@YSII_%4g[%1]%3)
#define _Y_ITER_C1:_Y_ITER_C2:%0[%1]@YSII_Ag%3) %0@YSII_Ag[]%3)
#define _Y_ITER_C2:sizeof%0(%1)@YSII_Ag-1;_:(%2=_Y_ITER_ARRAY:%3(%4)@YSII_Ag[%5])!=_Y_ITER_ARRAY_SIZE(%6);) -1;_:(%2=%3@YSII_Ag(%4,%5))!=-1;)

#define _Y_ITER_C3:%0[%1]@YSII_Cg,%2[%3]@YSII_Ag[%4]={%5} _Y_ITER_C3:%0@YSII_Cg[%1],%0@YSII_Ag[%1][%4]

// foreach 19 C2 and C3
//#define _Y_ITER_C2:sizeof%0(%1)@YSII_Ag-1;_:(%2=_Y_ITER_ARRAY:%3(%4)@YSII_Ag[%5])!=%9_Y_ITER_ARRAY_SIZE(%6);) -1;_:(%2=%3@YSII_Ag(%4,%5))!=-1;)
 
//#define _Y_ITER_C3:%0[%1]@YSII_Cg,%2[%3]@YSII_Ag[%4]={%5} _Y_ITER_C3:%0@YSII_Cg[%4-1],%0@YSII_Ag[%1][%4]


#if !defined BOTSYNC_IS_BOT
	static stock
		YSI_g_sCallbacks = 0;
#endif

#if !defined BOTSYNC_IS_BOT
	forward Iter_OPDCInternal(playerid);
#endif

/*----------------------------------------------------------------------------*\
Function:
	Iter_Create2 (unused)
Params:
	name - Iterator identifier.
	size0 - Number of iterators.
	size1 - Number of items per iterator.
Return:
	-
Notes:
	Creates a new array of iterator start/array pair.
\*----------------------------------------------------------------------------*/

// If this ever changes, update the size reference in y_users.
/*#define Iter_Create2(%1,%2,%3) \
	new \
		%1@YSII_Sg[%2] = {-1, ...}, \
		%1@YSII_Cg[%2] = {0}, \
		%1@YSII_Ag[%2][%3]*/

/*----------------------------------------------------------------------------*\
Array:
	IteratorArray
Notes:
	Creates a new iterator array start/array pair.
\*----------------------------------------------------------------------------*/

#define IteratorArray:%1[%2]<%3> %1@YSII_Cg[%2],%1@YSII_Ag[%2][%3+1]//,%1@YSII_Rg[%2][%3+1]

/*----------------------------------------------------------------------------*\
Array:
	Iterator
Notes:
	Creates a new iterator start/array pair.
\*----------------------------------------------------------------------------*/

#define Iterator:%1<%2> _Y_ITER_C3:%1@YSII_Cg,%1@YSII_Ag[(%2)+1]={(%2)*2,(%2)*2-1,...}
#define iterator%0<%1> new Iterator:%0<%1>

/*----------------------------------------------------------------------------*\
Function:
	Iter_Init
Params:
	iter - Name of the iterator array to initialise.
Return:
	-
Notes:
	Wrapper for Iter_InitInternal.

native Iter_Init(IteratorArray:Name[]<>);

\*----------------------------------------------------------------------------*/

#define Iter_Init(%1) \
	Iter_InitInternal(%1@YSII_Ag,sizeof %1@YSII_Ag,sizeof %1@YSII_Ag[]-1)

/*----------------------------------------------------------------------------*\
Function:
	Iter_Create
Params:
	name - Iterator identifier.
	size - Number of values.
Return:
	-
Notes:
	Creates a new iterator start/array pair.
\*----------------------------------------------------------------------------*/

// If this ever changes, update the size reference in y_users.
/*
#define Iter_Create(%1,%2) \
	new \
		%1@YSII_Sg = -1, \
		%1@YSII_Cg = 0, \
		%1@YSII_Ag[%2] = {-1, ...}*/

/*----------------------------------------------------------------------------*\
Function:
	Iter_Add
Params:
	iter - Name of the iterator to add the data to.
	value - Value to add to the iterator.
Return:
	-
Notes:
	Wrapper for Iter_AddInternal.

native Iter_Add(Iterator:Name<>, value);

\*----------------------------------------------------------------------------*/

#define Iter_Add(%1,%2) Iter_AddInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,%2,_Y_ITER_ARRAY_SIZE(%1))

/*
#define Iter_AddSafe(%1,%2) \
	Iter_AddSafeInternal(%1@YSII_Sg, %1@YSII_Cg, %1@YSII_Ag, %1@YSII_Rg, %2)*/

/*----------------------------------------------------------------------------*\
Function:
	Iter_Free
Params:
	iter - Name of the iterator to get the first free slot in.
Return:
	-for
Notes:
	Wrapper for Iter_FreeInternal.

native Iter_Free(Iterator:Name<>);

\*----------------------------------------------------------------------------*/

#define Iter_Free(%1) Iter_FreeInternal(_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_Remove
Params:
	iter - Name of the iterator to remove data from.
	value - Data to remove.
Return:
	-
Notes:
	Wrapper for Iter_RemoveInternal.

native Iter_Remove(Iterator:Name<>, value);

\*----------------------------------------------------------------------------*/

#define Iter_Remove(%1,%2) Iter_RemoveInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,%2,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_Contains
Params:
	iter - Name of the iterator to check membership of.
	value - Value to check.
Return:
	-
Notes:
	Checks if the given value is in the given iterator.

native Iter_Remove(Iterator:Name<>, value);

\*----------------------------------------------------------------------------*/

#define Iter_Contains(%1,%2) Iter_ContainsInternal(_Y_ITER_ARRAY:%1@YSII_Ag,%2,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_SafeRemove
Params:
	iter - Name of the iterator to remove data from.
	value - Data to remove.
	next - Container for the pointer to the next element.
Return:
	-
Notes:
	Wrapper for Iter_SafeRemoveInternal.  Common use:
	
	Iter_SafeRemove(iter, i, i);

native Iter_SafeRemove(Iterator:Name<>, value, &next);

\*----------------------------------------------------------------------------*/

#define Iter_SafeRemove(%1,%2,%3) Iter_SafeRemoveInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,%2,%3,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_Random
Params:
	iter - Name of the iterator to get a random slot from.
Return:
	-
Notes:
	Wrapper for Iter_RandomInternal.

native Iter_Random(Iterator:Name<>);

\*----------------------------------------------------------------------------*/

#define Iter_Random(%1) Iter_RandomInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_Debug
Params:
	iter - Name of the iterator to output debug information from.
Return:
	-
Notes:
	Wrapper for Iter_ShowArray.
\*----------------------------------------------------------------------------*/

//#define Iter_Debug(%1) Iter_ShowArray(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag)

/*----------------------------------------------------------------------------*\
Function:
	Iter_Count
Params:
	iter - Name of the iterator to get a random slot from4.
Return:
	-
Notes:
	Returns the number of items in this iterator.

native Iter_Count(Iterator:Name<>);

\*----------------------------------------------------------------------------*/

#define Iter_Count(%1) (_Y_ITER_ARRAY:%1@YSII_Cg)

/*----------------------------------------------------------------------------*\
Function:
	Iter_Clear
Params:
	iter - Name of the iterator empty.
Return:
	-
Notes:
	Wrapper for Iter_ClearInternal.

native Iter_Clear(IteratorArray:Name[]<>);

\*----------------------------------------------------------------------------*/

#define Iter_Clear(%1) Iter_ClearInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Create the internal iterators.
\*----------------------------------------------------------------------------*/

#if !defined BOTSYNC_IS_BOT
	new
		Iterator:Player<MAX_PLAYERS>;
	
	#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
		new
			Iterator:Bot<MAX_PLAYERS>,
			Iterator:Character<MAX_PLAYERS>;
		
		#define NPC@YSII_Cg Bot@YSII_Cg
		#define NPC@YSII_Ag Bot@YSII_Ag
	#endif
#endif

/*----------------------------------------------------------------------------*\
	Variables to optimise memory usage by only having one copy of each string.
	Note that only strings used more than once are put here because only they
	have any gain to being located in only one place.
\*----------------------------------------------------------------------------*/

static stock
	YSI_gsOnPlayerConnect[] = "Iter_OnPlayerConnect",
	YSI_gsOnPlayerDisconnect[] = "Iter_OnPlayerDisconnect",
	YSI_gsOnGameModeInit[] = "Iter_OnGameModeInit",
	YSI_gsSpecifier@[] = "",
	YSI_gsSpecifier@i[] = "i";

/*----------------------------------------------------------------------------*\
Function:
	foreach
Params:
	data - Data to iterate through.
	as - Variable to set value to.
Return:
	-
Notes:
	Not exactly the same as PHP foreach, just iterates through a list and
	returns the value of the current slot but uses that slot as the next index
	too.  Variables must be in the form @YSII_<gname>S for the start index and
	@YSII_<gname>A for the data array where <name> is what's entered in data.
\*----------------------------------------------------------------------------*/

//#define foreach(%1,%2)
//	for (new %2 = %1@YSII_Sg; _:%2 != -1; %2 = %1@YSII_Ag[%2])
#define foreach%1(%0) for(new Y_FOREACH_SECOND|||Y_FOREACH_THIRD|||%0|||)
// This allows us to use "new" multiple times - stripping off ONLY whole words.
#define new%0|||%9|||%1:%2||| %9|||%0|||%1|||%2|||

// This one is called if the new syntax is required, but the state of "new" is
// as-yet unknown.  This attempts to call "%1" as a macro, if it starts with
// "new" as a whole word then it will (and will also helpfully strip off the
// "new" keyword for us).
#define Y_FOREACH_THIRD|||%0|||%1|||%2||| %1=Y_FOREACH_FIFTH|||Y_FOREACH_FOURTH|||%1:%2|||

// This is called if the "new" macro is called for a second time.
#define Y_FOREACH_FOURTH|||%0=Y_FOREACH_FIFTH|||%1|||%2||| new Y_FOREACH_SIXTH;%0|||Y_FOREACH_SEVENTH|||%2|||

// This is called when there are tags on the "new" declaration.
#define Y_FOREACH_SEVENTH|||%9Y_FOREACH_SIXTH;%0|||%1|||%2||| new %0:%1=%0:_Y_ITER_ARRAY_SIZE(%2);_:(%1=_Y_ITER_ARRAY:%2@YSII_Ag[%1])!=_Y_ITER_ARRAY_SIZE(%2);

// This is called when there aren't.
#define Y_FOREACH_SIXTH;%0|||Y_FOREACH_SEVENTH|||%2||| %0=_Y_ITER_ARRAY_SIZE(%2);_:(%0=_Y_ITER_ARRAY:%2@YSII_Ag[%0])!=_Y_ITER_ARRAY_SIZE(%2);
//hta:%0=hta:%2@YSII_Sg;_:%0!=-1;%0=hta:%2@YSII_Ag[%0]
//#define Y_FOREACH_FOURTH|||%0=Y_FOREACH_FIFTH|||%1|||%2||| new hta:%0=hta:%2@YSII_Sg;_:%0!=-1;%0=hta:%2@YSII_Ag[%0]

// Move any tags from the second half to the first half.
//#define hta:%0=hta:%1:%2;_:%3!=-1;%4=hta:%5:%6[%7] %0:%1=%2;_:%1!=-1;%1=%6[%1]

// This is called if "%1" didn't have "new" at the start.
#define Y_FOREACH_FIFTH|||Y_FOREACH_FOURTH|||%1:%2||| _Y_ITER_ARRAY_SIZE(%2);_:(%1=_Y_ITER_ARRAY:%2@YSII_Ag[%1])!=_Y_ITER_ARRAY_SIZE(%2);

//%1=%2@YSII_Sg;_:Y_FOREACH_NONEW:%1!=-1;%1=%2@YSII_Ag[%1]
// This is the old version, but DON'T add "new" because that already exists from
// the failed "new" macro call above.
#define Y_FOREACH_SECOND|||Y_FOREACH_THIRD|||%1,%2||| %2=_Y_ITER_ARRAY_SIZE(%1);_:(%2=_Y_ITER_ARRAY:%1@YSII_Ag[%2])!=_Y_ITER_ARRAY_SIZE(%1);
//#define Y_FOREACH_NONEW:new%0!=-1;new%1=%2[new%3] %0!=-1;%1=%2[%3]

//#define Y_FOREACH_EIGHTH:%0[%1]@YSII_Sg;%2;%3=%4[%5]@YSII_Ag[%6] %0@YSII_Sg[%1];%2;%3=%4@YSII_Ag[%5][%6]

/*----------------------------------------------------------------------------*\
Function:
	foreachex
Params:
	data - Data to iterate through.
	as - Variable to set value to.
Return:
	-
Notes:
	Similar to foreach but does not declare a new variable for the iterator.
\*----------------------------------------------------------------------------*/

#define foreachex(%1,%2) foreach(%2:%1)
	//for (%2=_Y_ITER_ARRAY_SIZE(%1);(%2=_Y_ITER_ARRAY:%1@YSII_Ag[%2])!=_Y_ITER_ARRAY_SIZE(%1);)

/*----------------------------------------------------------------------------*\
Function:
	Iter_OnPlayerConnect
Params:
	playerid - Player who joined.
Return:
	-
Notes:
	Adds a player to the loop data.  Now sorts the list too.  Note that I found
	the most bizzare bug ever (I *think* it may be a compiler but, but it
	requires further investigation), basically it seems that multiple variables
	were being treated as the same variable (namely @YSII_EgotS and
	@YSII_CgharacterS were the same and @YSII_EgotC and @YSII_CgharacterC were the
	same).  Adding print statements which reference these variables seem to fix
	the problem, and I have tried to make sure that the values will never actually
	get printed.
\*----------------------------------------------------------------------------*/

#if !defined BOTSYNC_IS_BOT
	public OnPlayerConnect(playerid)
	{
		#if defined _FOREACH_BOT
			if (!IsPlayerNPC(playerid))
			{
				Iter_Add(Player, playerid);
			}
			#if !defined FOREACH_NO_BOTS
				else
				{
					Iter_Add(Bot, playerid);
				}
				#pragma tabsize 4
				Iter_Add(Character, playerid);
			#endif
		#else
			Iter_Add(Player, playerid);
		#endif
		if (YSI_g_sCallbacks & 2)
		{
			CallLocalFunction(YSI_gsOnPlayerConnect, YSI_gsSpecifier@i, playerid);
		}
		return 1;
	}
	
	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	#define OnPlayerConnect Iter_OnPlayerConnect
	forward OnPlayerConnect(playerid);
#endif

/*----------------------------------------------------------------------------*\
Function:
	Iter_OnFilterScriptInit
Params:
	-
Return:
	-
Notes:
	Fixes a bug where callbacks are not detected when "loadfs" is used after the
	GM has already started.  If this is a GM this is just never used called.
\*----------------------------------------------------------------------------*/

#if !defined BOTSYNC_IS_BOT
	public OnFilterScriptInit()
	{
		if (funcidx(YSI_gsOnPlayerDisconnect) != -1)
		{
			YSI_g_sCallbacks |= 1;
		}
		if (funcidx(YSI_gsOnPlayerConnect) != -1)
		{
			YSI_g_sCallbacks |= 2;
		}
		#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
			Bot@YSII_Cg = _Y_ITER_C3:0;
			Character@YSII_Cg = _Y_ITER_C3:0;
			new
				lastBot = MAX_PLAYERS,
				lastCharacter = MAX_PLAYERS;
		#endif
		Player@YSII_Cg = _Y_ITER_C3:0;
		new
			lastPlayer = MAX_PLAYERS;
		for (new i = 0; i != MAX_PLAYERS; ++i)
		{
			if (IsPlayerConnected(i))
			{
				#if defined _FOREACH_BOT
					// Had to do "if ! else" due to compile options.
					if (!IsPlayerNPC(i))
					{
						Player@YSII_Ag[lastPlayer] = i;
						++Player@YSII_Cg;
						lastPlayer = i;
					}
					#if !defined FOREACH_NO_BOTS
						else
						{
							Bot@YSII_Ag[lastBot] = i;
							++Bot@YSII_Cg;
							lastBot = i;
						}
						#pragma tabsize 4
						Character@YSII_Ag[lastCharacter] = i;
						++Character@YSII_Cg;
						lastCharacter = i;
					#endif
				#else
					Player@YSII_Ag[lastPlayer] = i;
					++Player@YSII_Cg;
					lastPlayer = i;
				#endif
			}
			else
			{
				#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
					Bot@YSII_Ag[i] = MAX_PLAYERS + 1;
					//Bot@YSII_Rg[i] = -1;
					Character@YSII_Ag[i] = MAX_PLAYERS + 1;
					//Character@YSII_Rg[i] = -1;
				#endif
				Player@YSII_Ag[i] = MAX_PLAYERS + 1;
				//Player@YSII_Rg[i] = -1;
			}
		}
		#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
			Bot@YSII_Ag[lastPlayer] = MAX_PLAYERS;
			Character@YSII_Ag[lastPlayer] = MAX_PLAYERS;
		#endif
		Player@YSII_Ag[lastPlayer] = MAX_PLAYERS;
		CallLocalFunction("Iter_OnFilterScriptInit", YSI_gsSpecifier@);
		return 1;
	}
	
	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit Iter_OnFilterScriptInit
	forward OnFilterScriptInit();
#endif

/*----------------------------------------------------------------------------*\
Function:
	Iter_OnGameModeInit
Params:
	-
Return:
	-
Notes:
	There are WIERD bugs in this script, seemingly caused by the compiler, so
	this hopefully fixes them.  The OnGameModeInit code is written to be
	very fast by utilising the internal array structure instead of the regular
	Add functions.
\*----------------------------------------------------------------------------*/

#if !defined BOTSYNC_IS_BOT
	public OnGameModeInit()
	{
		// Clear everything.
		if (funcidx(YSI_gsOnPlayerDisconnect) != -1)
		{
			YSI_g_sCallbacks |= 1;
		}
		if (funcidx(YSI_gsOnPlayerConnect) != -1)
		{
			YSI_g_sCallbacks |= 2;
		}
		if (!Player@YSII_Cg)
		{
			#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
				CallLocalFunction(YSI_gsOnGameModeInit, YSI_gsSpecifier@, Bot@YSII_Cg, Character@YSII_Cg, Player@YSII_Cg);
			#else
				CallLocalFunction(YSI_gsOnGameModeInit, YSI_gsSpecifier@, Player@YSII_Cg);
			#endif
			return 1;
		}
		// Do the forward iterator list.
		#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
			Bot@YSII_Cg = _Y_ITER_C3:0;
			Bot@YSII_Ag[MAX_PLAYERS] = MAX_PLAYERS;
			Character@YSII_Ag[MAX_PLAYERS] = MAX_PLAYERS;
			Character@YSII_Cg = _Y_ITER_C3:0;
			new
				lastBot = MAX_PLAYERS,
				lastCharacter = MAX_PLAYERS;
		#endif
		Player@YSII_Cg = _Y_ITER_C3:0;
		Player@YSII_Ag[MAX_PLAYERS] = MAX_PLAYERS;
		new
			lastPlayer = MAX_PLAYERS;
		for (new i = 0; i != MAX_PLAYERS; ++i)
		{
			if (IsPlayerConnected(i))
			{
				#if defined _FOREACH_BOT
					// Had to do "if ! else" due to compile options.
					if (!IsPlayerNPC(i))
					{
						Player@YSII_Ag[lastPlayer] = i;
						++Player@YSII_Cg;
						lastPlayer = i;
					}
					#if !defined FOREACH_NO_BOTS
						else
						{
							Bot@YSII_Ag[lastBot] = i;
							++Bot@YSII_Cg;
							lastBot = i;
						}
						#pragma tabsize 4
						Character@YSII_Ag[lastCharacter] = i;
						++Character@YSII_Cg;
						lastCharacter = i;
					#endif
				#else
					Player@YSII_Ag[lastPlayer] = i;
					++Player@YSII_Cg;
					lastPlayer = i;
				#endif
			}
			else
			{
				#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
					Bot@YSII_Ag[i] = MAX_PLAYERS + 1;
					//Bot@YSII_Rg[i] = -1;
					Character@YSII_Ag[i] = MAX_PLAYERS + 1;
					//Character@YSII_Rg[i] = -1;
				#endif
				Player@YSII_Ag[i] = MAX_PLAYERS + 1;
				//Player@YSII_Rg[i] = -1;
			}
		}
		#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
			Bot@YSII_Ag[lastPlayer] = MAX_PLAYERS;
			Character@YSII_Ag[lastPlayer] = MAX_PLAYERS;
		#endif
		Player@YSII_Ag[lastPlayer] = MAX_PLAYERS;
		CallLocalFunction(YSI_gsOnGameModeInit, YSI_gsSpecifier@);
		return 1;
	}
	
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit Iter_OnGameModeInit
	forward OnGameModeInit();
#endif

/*----------------------------------------------------------------------------*\
Function:
	Iter_OnPlayerDisconnect
Params:
	playerid - Player who left.
Return:
	-
Notes:
	Removes a player from the loop data.  No longer uses "hook" to ENSURE that
	this is always last.  Previously I think that the order of evaluation in
	y_hooks meant that this got called before the user "OnPlayerDisconnect".
\*----------------------------------------------------------------------------*/

#if !defined BOTSYNC_IS_BOT
	public OnPlayerDisconnect(playerid, reason)
	{
		if (YSI_g_sCallbacks & 1)
		{
			CallLocalFunction(YSI_gsOnPlayerDisconnect, "ii", playerid, reason);
		}
		SetTimerEx("Iter_OPDCInternal", 0, false, YSI_gsSpecifier@i, playerid);
		return 1;
	}
	
	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif
	#define OnPlayerDisconnect Iter_OnPlayerDisconnect
	forward OnPlayerDisconnect(playerid, reason);
#endif

/*----------------------------------------------------------------------------*\
Function:
	Iter_OPDCInternal
Params:
	playerid - Player who left.
Return:
	-
Notes:
	Called AFTER "OnPlayerDisconnect" so that using "Kick" inside a "foreach"
	loop does not crash the server due to an OOB error.
\*----------------------------------------------------------------------------*/

#if !defined BOTSYNC_IS_BOT
	public Iter_OPDCInternal(playerid)
	{
		if (IsPlayerConnected(playerid))
		{
			return;
		}
		#if defined _FOREACH_BOT
			if (!IsPlayerNPC(playerid))
			{
				Iter_Remove(Player, playerid);
			}
			#if !defined FOREACH_NO_BOTS
				else
				{
					Iter_Remove(Bot, playerid);
				}
				#pragma tabsize 4
				Iter_Remove(Character, playerid);
			#endif
		#else
			Iter_Remove(Player, playerid);
		#endif
	}
#endif

/*----------------------------------------------------------------------------*\
Function:
	Iter_ShowArray
Params:
	start - Iterator start point.
	members[] - Iterator contents.
	size - Number of iterator values
Return:
	-
Notes:
	Pure debug function. Has regular prints not debug prints
	as it is only called when debug is on.
\*----------------------------------------------------------------------------*/

/*stock
	Iter_ShowArray(size, members[])
{
	static
		sString[61];
	new
		i,
		j = 10;
	printf("Start: %d", start);
	printf("Size:  %d", size);
	while (i < size)
	{
		sString[0] = '\0';
		while (i < j && i < size)
		{
			format(sString, sizeof (sString), "%s, %d", sString, members[i]);
			i++;
		}
		printf("Array (%d): %s", j, sString);
		j += 10;
	}
}*/

/*----------------------------------------------------------------------------*\
Function:
	Iter_RandomInternal
Params:
	count - Number of items in the iterator.
	array[] - Iterator data.
	size - Size of the iterator.
Return:
	-
Notes:
	Returns a random value from an iterator.
\*----------------------------------------------------------------------------*/

stock
	Iter_RandomInternal(count, array[], size)
{
	if (count == 0)
	{
		return -1;
	}
	new
		rnd = random(count),
		cur = array[size];
	while (cur != size)
	{
		if (rnd-- == 0)
		{
			return cur;
		}
		cur = array[cur];
	}
	return -1;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_FreeInternal
Params:
	count - Number of items in the iterator.
	array[] - Iterator data.
	size - Size of the iterator.
Return:
	-
Notes:
	Finds the first free slot in the iterator.  Iterators now HAVE to be
	sorted for this function to work correctly as it uses that fact to decide
	wether a slot is unused or the last one.  If you want to use the slot
	straight after finding it the iterator will need to re-find it to add in
	the data.
\*----------------------------------------------------------------------------*/

stock
	Iter_FreeInternal(array[], size)
{
	for (new i = 0; i != size; ++i)
	{
		if (array[i] > size)
		{
			return i;
		}
	}
	return -1;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_AddInternal
Params:
	&start - Array start index.
	&count - Number of items in the iterator.
	array[] - Iterator data.
	value - Item to add.
Return:
	-
Notes:
	Adds a value to a given iterator set.  Now detects when you try and add the
	last item multiple times, as well as all the other items.  Now simplified even
	further with the new internal representation.
\*----------------------------------------------------------------------------*/

stock
	Iter_AddInternal(&count, array[], value, size)
{
	if (0 <= value < size && array[value] > size)
	{
		new
			last = size,
			next = array[last];
		while (next < value)
		{
			last = next;
			next = array[last];
		}
		array[last] = value;
		array[value] = next;
		++count;
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_RemoveInternal
Params:
	&count - Number of items in the iterator.
	array[] - Iterator data.
	value - Item to remove.
Return:
	-
Notes:
	Removes a value from an iterator.
\*----------------------------------------------------------------------------*/

stock
	Iter_RemoveInternal(&count, array[], value, size)
{
	new
		last;
	return Iter_SafeRemoveInternal(count, array, value, last, size);
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_SafeRemoveInternal
Params:
	&count - Number of items in the iterator.
	array[] - Iterator data.
	back[] - Reverse iterator data.
	value - Item to remove.
	&last - Pointer in which to store the last pointer.
Return:
	-
Notes:
	Removes a value from an iterator safely.
\*----------------------------------------------------------------------------*/

stock
	Iter_SafeRemoveInternal(&count, array[], value, &last, size)
{
	if (0 <= value < size && array[value] <= size)
	{
		last = size;
		new
			next = array[last];
		while (next != value)
		{
			last = next;
			next = array[last];
		}
		array[last] = array[value];
		array[value] = size + 1;
		--count;
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_ContainsInternal
Params:
	array[] - Iterator data.
	value - Item to check.
	size - Size of the iterator.
Return:
	-
Notes:
	Checks if this item is in the iterator.
\*----------------------------------------------------------------------------*/

stock
	Iter_ContainsInternal(array[], value, size)
{
	return 0 <= value < size && array[value] <= size;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_ClearInternal
Params:
	&count - Number of items in the iterator.
	array[] - Iterator data.
	size - Size of the iterator.
Return:
	-
Notes:
	Resets an iterator.
\*----------------------------------------------------------------------------*/

stock
	Iter_ClearInternal(&count, array[], size)
{
	for (new i = 0, t = size + 1; i < size; ++i)
	{
		array[i] = t;
	}
	array[size] = size;
	count = 0;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_InitInternal
Params:
	array[][] - Iterator array to initialise.
	s0 - Size of first dimension.
	s1 - Size of second dimension.
Return:
	-
Notes:
	Multi-dimensional arrays can not be initialised at compile time, so need to be
	done at run time, which is slightly annoying.
\*----------------------------------------------------------------------------*/

stock
	Iter_InitInternal(arr[][], s0, s1)
{
	for (new i = 0, t = s1 + 1; i < s0; ++i)
	{
		for (new j = 0; j < s1; ++j)
		{
			arr[i][j] = t;
		}
		arr[i][s1] = s1;
	}
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_PrevInternal
Params:
	array[] - Iterator data.
	size - Size of the iterator.
	slot - The current slot.
Return:
	-
Notes:
	Gets the element in an iterator that points to the current element.
\*----------------------------------------------------------------------------*/

stock
	Iter_PrevInternal(array[], size, slot)
{
	if (0 <= slot <= size && array[slot] <= size)
	{
		for (new last = slot; last--; )
 		{
 			if (array[last] == slot)
 			{
 			    return last;
 			}
 		}
	}
	return size;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_Begin
Params:
	iter - Name of the iterator to get the start of.
Return:
	-
Notes:
	Gets a point BEFORE the start of the iterator (the theoretical beginning).
\*----------------------------------------------------------------------------*/

#define Iter_Begin(%1) (_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_End
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets a point AFTER the end of the iterator (think "MAX_PLAYERS").
\*----------------------------------------------------------------------------*/

#define Iter_End(%1) (_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_First
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the first element in an iterator.
\*----------------------------------------------------------------------------*/

#define Iter_First(%1) (_Y_ITER_ARRAY:%1@YSII_Ag[_Y_ITER_ARRAY_SIZE(%1)])

/*----------------------------------------------------------------------------*\
Function:
	Iter_Last
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the last element in an iterator.
\*----------------------------------------------------------------------------*/

#define Iter_Last(%1) Iter_PrevInternal(_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1),_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_Next
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the element in an interator after the current one.
\*----------------------------------------------------------------------------*/

#define Iter_Next(%1,%2) (_Y_ITER_ARRAY:%1@YSII_Ag[(%2)])

/*----------------------------------------------------------------------------*\
Function:
	Iter_Prev
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the element in an iterator before the current one.  Slow.
\*----------------------------------------------------------------------------*/

#define Iter_Prev(%1,%2) Iter_PrevInternal(_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1),(%2))

/*----------------------------------------------------------------------------*\
Function:
    Iter_InternalArray
Params:
    iter - Name of the iterator to get the true name of.
Return:
    -
Notes:
    Accesses the internal array of an iterator.
\*----------------------------------------------------------------------------*/

#define Iter_InternalArray(%1) (_Y_ITER_ARRAY:%1@YSII_Ag)

/*----------------------------------------------------------------------------*\
Function:
    Iter_InternalSize
Params:
    iter - Name of the iterator to get the true size of.
Return:
    -
Notes:
    Accesses the internal size of an iterator.
\*----------------------------------------------------------------------------*/

#define _Y_ITER_INT_SIZE:%0(%2[%1]@YSII_Ag)) %0(%2@YSII_Ag[]))

#define Iter_InternalSize(%1) (_:_Y_ITER_INT_SIZE:sizeof (%1@YSII_Ag))
