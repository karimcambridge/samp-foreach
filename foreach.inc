/**--------------------------------------------------------------------------**\
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
	
	For extensive documentation on writing and using iterators, see this topic:
	
		http://forum.sa-mp.com/showthread.php?t=481877
	
Legal:
	Version: MPL 1.1
	
	The contents of this file are subject to the Mozilla Public License Version 
	1.1 (the "License"); you may not use this file except in compliance with 
	the License. You may obtain a copy of the License at 
	[url]http://www.mozilla.org/MPL/[/url]
	
	Software distributed under the License is distributed on an "AS IS" basis,
	WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
	for the specific language governing rights and limitations under the
	License.
	
	The Original Code is the YSI foreach include.
	
	The Initial Developer of the Original Code is Alex "Y_Less" Cole.
	Portions created by the Initial Developer are Copyright (C) 2011
	the Initial Developer. All Rights Reserved.
	
	Contributors:
		ZeeX, koolk, JoeBullet/Google63, g_aSlice/Slice
	
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
	0.4
Changelog:
	26/12/13:
		Added sections.
	17/10/12:
		Fixed a bug that was here but not in "foreach.inc".
	04/10/12:
		Added a tiny tweak to detect tag-returning iterator functions.
		Added Iter_InternalSize.
	13/01/12:
		Fixed the count thanks to AndreT.
	05/01/12:
		Fixed multi-dimensional iterators.
		Fixed "FOREACH_NO_BOTS".
		Made "Iterator:" support multi-dimensional arrays.
	24/12/11:
		Added _YSI_SPECIAL_DEBUG support.
		Added fix for function iterators.
		Ported back to YSI.
		Changed to use internal YSI "__" natives.
		Fixed calls order by using ALS.
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
		Iter_ShowArray - Displays the contents of the array.
		Iter_AddInternal - Add a value to an iterator.
		Iter_RemoveInternal - Remove a value from an iterator.
		Iter_RandomInternal - Get a random item from an iterator.
		Iter_FreeInternal - Gets the first free slot in the iterator.
		Iter_InitInternal - Initialises a multi-dimensional iterator.
	Static:
		-
	Inline:
		Iter_Create - Create a new iterator value set.
		Iter_Add - Wraps Iter_AddInternal.
		Iter_Remove - Wraps Iter_RemoveInternal.
		Iter_Random - Wraps Iter_RandomInternal.
		Iter_Count - Gets the number of items in an iterator.
		Iter_Debug - Wraps around Iter_ShowArray.
		Iter_Free - Wraps around Iter_FreeInternal.
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
	foreach - Command to loop an iterator.
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
\**--------------------------------------------------------------------------**/

/*

     ad88888ba                                              
    d8"     "8b              ,d                             
    Y8,                      88                             
    `Y8aaaaa,    ,adPPYba, MM88MMM 88       88 8b,dPPYba,   
      `"""""8b, a8P_____88   88    88       88 88P'    "8a  
            `8b 8PP"""""""   88    88       88 88       d8  
    Y8a     a8P "8b,   ,aa   88,   "8a,   ,a88 88b,   ,a8"  
     "Y88888P"   `"Ybbd8"'   "Y888  `"YbbdP'Y8 88`YbbdP"'   
                                               88           
                                               88           
    
*/

// "y_iterate" is always higher than "foreach".
#define _FOREACH_LOCAL_VERSION        30

// Foreach is testing us.
#if defined _FOREACH_INC_TEST
	#define _FOREACH_CUR_VERSION _FOREACH_LOCAL_VERSION
	#endinput
#endif
#if !defined _FOREACH_NO_TEST
	#define _FOREACH_INC_TEST
	#tryinclude "YSI\y_iterate"
	#undef _FOREACH_INC_TEST
	// "y_iterate" exists - test which is newer.
	#if defined _Y_ITERATE_LOCAL_VERSION
		#if _Y_ITERATE_LOCAL_VERSION > _FOREACH_LOCAL_VERSION
			// "y_iterate" is newer.
			#if defined _inc_y_iterate
				#undef _inc_y_iterate
			#endif
			#define _FOREACH_NO_TEST
			// Include one of the two.
			#tryinclude "YSI\y_iterate"
			#endinput
		#endif
	#elseif defined foreach
		#error Old y_iterate.inc files are no longer compatible with foreach.
	#endif
#endif
#define _FOREACH_INC_TEST

#if !defined _samp_included
	#error "Please include a_samp or a_npc before foreach"
#endif

#tryinclude "YSI\y_utils"

#define _FOREACH_TEST_IS_UNSET(%0) ((2*%0-1+1)==-1)

// Resolve compiler options.
#if !defined FOREACH_NO_PLAYERS
	// No define.
	#define _FOREACH_PLAYER_ITERATORS (1)
#elseif defined FOREACH_NO_PLAYERS
	// #define FOREACH_NO_PLAYERS 1
	#define _FOREACH_PLAYER_ITERATORS (0)
#else
	// #define FOREACH_NO_PLAYERS 0
	#define _FOREACH_PLAYER_ITERATORS (1)
#endif

// Resolve compiler options.
#if !defined FOREACH_NO_BOTS
	// No define.
	#define _FOREACH_BOT_ITERATORS (1)
#elseif defined FOREACH_NO_BOTS
	// #define FOREACH_NO_BOTS 1
	#define _FOREACH_BOT_ITERATORS (0)
#else
	// #define FOREACH_NO_BOTS 0
	#define _FOREACH_BOT_ITERATORS (1)
#endif

#if defined GetDistanceFromMeToPoint
	#define _FOREACH_SETUP_MODE (1) // "OnNPCModeInit" (NPC).
#elseif defined GameModeExit
	#define _FOREACH_SETUP_MODE (2) // "OnScriptInit" (GM/FS).
#else
	#define _FOREACH_SETUP_MODE (0) // "main" (other).
#endif

#if _FOREACH_SETUP_MODE == 3
	// Need a guarantee of certain things we don't have.
	#undef _FOREACH_PLAYER_ITERATORS
	#define _FOREACH_PLAYER_ITERATORS (0)
#endif

#if !defined IsPlayerConnected
	// Can't have player iterators even if we wanted them.
	#undef _FOREACH_PLAYER_ITERATORS
	#define _FOREACH_PLAYER_ITERATORS (0)
#endif

#if !defined IsPlayerNPC
	// Can't have bot iterators even if we wanted them.
	#undef _FOREACH_BOT_ITERATORS
	#define _FOREACH_BOT_ITERATORS (0)
#endif

#if !_FOREACH_PLAYER_ITERATORS
	// Fixed dependency.
	#undef _FOREACH_BOT_ITERATORS
	#define _FOREACH_BOT_ITERATORS (0)
#endif

forward Iter_OPDCInternal(playerid);

forward Iter_PlayerConnectionCheck();

static stock
	YSI_gsConnectionTimer = -1;

/*

    88      a8P                                                                           88            
    88    ,88'                                                                            88            
    88  ,88"                                                                              88            
    88,d88'      ,adPPYba, 8b       d8 8b      db      d8  ,adPPYba,  8b,dPPYba,  ,adPPYb,88 ,adPPYba,  
    8888"88,    a8P_____88 `8b     d8' `8b    d88b    d8' a8"     "8a 88P'   "Y8 a8"    `Y88 I8[    ""  
    88P   Y8b   8PP"""""""  `8b   d8'   `8b  d8'`8b  d8'  8b       d8 88         8b       88  `"Y8ba,   
    88     "88, "8b,   ,aa   `8b,d8'     `8bd8'  `8bd8'   "8a,   ,a8" 88         "8a,   ,d88 aa    ]8I  
    88       Y8b `"Ybbd8"'     Y88'        YP      YP      `"YbbdP"'  88          `"8bbdP"Y8 `"YbbdP"'  
                               d8'                                                                      
                              d8'                                                                       

*/

/**--------------------------------------------------------------------------**\
Array:
	Iterator
</returns>
<remarks>
	Creates a new iterator start/array pair.
</remarks>
\**--------------------------------------------------------------------------**/

#define Iterator:%1<%2> _Y_ITER_C9:_Y_ITER_C3:%1$YSII_Cg,_Y_ITER_C3:%1$YSII_Ag[(%2)+1]={0,1,...}
#define iterator%0<%1> new Iterator:%0<%1>

/**--------------------------------------------------------------------------**\
<summary>foreach</summary>
<param name="data">Data to iterate through.</param>
<param name="as">Variable to set value to.</param>
<returns>
	-
</returns>
<remarks>
	Not exactly the same as PHP foreach, just iterates through a list and
	returns the value of the current slot but uses that slot as the next index
	too.  Variables must be in the form @YSII_<gname>S for the start index and
	@YSII_<gname>A for the data array where <name> is what's entered in data.
</remarks>
\**--------------------------------------------------------------------------**/

#define foreach%1(%0) for(new Y_FOREACH_SECOND|||Y_FOREACH_THIRD|||%0|||)

/*

    88b           d88            88                       db        88888888ba  88  
    888b         d888            ""                      d88b       88      "8b 88  
    88`8b       d8'88                                   d8'`8b      88      ,8P 88  
    88 `8b     d8' 88 ,adPPYYba, 88 8b,dPPYba,         d8'  `8b     88aaaaaa8P' 88  
    88  `8b   d8'  88 ""     `Y8 88 88P'   `"8a       d8YaaaaY8b    88""""""'   88  
    88   `8b d8'   88 ,adPPPPP88 88 88       88      d8""""""""8b   88          88  
    88    `888'    88 88,    ,88 88 88       88     d8'        `8b  88          88  
    88     `8'     88 `"8bbdP"Y8 88 88       88    d8'          `8b 88          88  

*/

/**--------------------------------------------------------------------------**\
<summary>Iter_Init</summary>
<param name="iter">Name of the iterator array to initialise.</param>
<returns>
	-
</returns>
<remarks>
	Wrapper for Iter_InitInternal.

native Iter_Init(IteratorArray:Name[]<>);

</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Init(%1) Iter_InitInternal(_Y_ITER_ARRAY$%1$YSII_Ag,_Y_ITER_MAYBE_ARRAY(%1)+1,_Y_ITER_MAYBE_ARRAY(%1[]))

/**--------------------------------------------------------------------------**\
<summary>Iter_Add</summary>
<param name="iter">Name of the iterator to add the data to.</param>
<param name="value">Value to add to the iterator.</param>
<returns>
	-
</returns>
<remarks>
	Wrapper for Iter_AddInternal.

native Iter_Add(Iterator:Name<>, value);

</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Add(%1,%2) Iter_AddInternal(_Y_ITER_ARRAY$%1$YSII_Cg,_Y_ITER_ARRAY$%1$YSII_Ag,%2,_Y_ITER_MAYBE_ARRAY(%1))

/**--------------------------------------------------------------------------**\
<summary>Iter_Remove</summary>
<param name="iter">Name of the iterator to remove data from.</param>
<param name="value">Data to remove.</param>
<returns>
	-
</returns>
<remarks>
	Wrapper for Iter_RemoveInternal.

native Iter_Remove(Iterator:Name<>, value);

</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Remove(%1,%2) Iter_RemoveInternal(_Y_ITER_ARRAY$%1$YSII_Cg,_Y_ITER_ARRAY$%1$YSII_Ag,%2,_Y_ITER_MAYBE_ARRAY(%1))

/**--------------------------------------------------------------------------**\
<summary>Iter_Free</summary>
<param name="iter">Name of the iterator to get the first free slot in.</param>
<returns>
	-
</returns>
<remarks>
	Wrapper for Iter_FreeInternal.

native Iter_Free(Iterator:Name<>);

</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Free(%1) Iter_FreeInternal(_Y_ITER_ARRAY$%1$YSII_Ag,_Y_ITER_MAYBE_ARRAY(%1))

/**--------------------------------------------------------------------------**\
<summary>Iter_Contains</summary>
<param name="iter">Name of the iterator to check membership of.</param>
<param name="value">Value to check.</param>
<returns>
	-
</returns>
<remarks>
	Checks if the given value is in the given iterator.

native Iter_Remove(Iterator:Name<>, value);

	I wanted to make this a pure macro, but it would use the second parameter 3
	times, which can mess up code.  The first parameter is safe to use though.
</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Contains(%1,%2) Iter_ContainsInternal(_Y_ITER_ARRAY$%1$YSII_Ag,%2,_Y_ITER_MAYBE_ARRAY(%1))

/**--------------------------------------------------------------------------**\
<summary>Iter_SafeRemove</summary>
<param name="iter">Name of the iterator to remove data from.</param>
<param name="value">Data to remove.</param>
<param name="next">Container for the pointer to the next element.</param>
<returns>
	-
</returns>
<remarks>
	Wrapper for Iter_SafeRemoveInternal.  Common use:
	
	Iter_SafeRemove(iter, i, i);

native Iter_SafeRemove(Iterator:Name<>, value, &next);

</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_SafeRemove(%1,%2,%3) Iter_SafeRemoveInternal(_Y_ITER_ARRAY$%1$YSII_Cg,_Y_ITER_ARRAY$%1$YSII_Ag,%2,%3,_Y_ITER_MAYBE_ARRAY(%1))

/**--------------------------------------------------------------------------**\
<summary>Iter_Random</summary>
<param name="iter">Name of the iterator to get a random slot from.</param>
<returns>
	-
</returns>
<remarks>
	Wrapper for Iter_RandomInternal.

native Iter_Random(Iterator:Name<>);

</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Random(%1) Iter_RandomInternal(_Y_ITER_ARRAY$%1$YSII_Cg,_Y_ITER_ARRAY$%1$YSII_Ag,_Y_ITER_MAYBE_ARRAY(%1))

/**--------------------------------------------------------------------------**\
<summary>Iter_Count</summary>
<param name="iter">Name of the iterator to get a random slot from4.</param>
<returns>
	-
</returns>
<remarks>
	Returns the number of items in this iterator.

native Iter_Count(Iterator:Name<>);

</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Count(%1) (_Y_ITER_ARRAY$%1$YSII_Cg)

/**--------------------------------------------------------------------------**\
<summary>Iter_Clear</summary>
<param name="iter">Name of the iterator empty.</param>
<returns>
	-
</returns>
<remarks>
	Wrapper for Iter_ClearInternal.

native Iter_Clear(IteratorArray:Name[]<>);

</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Clear(%1) Iter_ClearInternal(_Y_ITER_ARRAY$%1$YSII_Cg,_Y_ITER_ARRAY$%1$YSII_Ag,_Y_ITER_MAYBE_ARRAY(%1))

/*

    88888888888                                                     db        88888888ba  88  
    88                        ,d                                   d88b       88      "8b 88  
    88                        88                                  d8'`8b      88      ,8P 88  
    88aaaaa     8b,     ,d8 MM88MMM 8b,dPPYba, ,adPPYYba,        d8'  `8b     88aaaaaa8P' 88  
    88"""""      `Y8, ,8P'    88    88P'   "Y8 ""     `Y8       d8YaaaaY8b    88""""""'   88  
    88             )888(      88    88         ,adPPPPP88      d8""""""""8b   88          88  
    88           ,d8" "8b,    88,   88         88,    ,88     d8'        `8b  88          88  
    88888888888 8P'     `Y8   "Y888 88         `"8bbdP"Y8    d8'          `8b 88          88  

*/

/**--------------------------------------------------------------------------**\
<summary>Iter_Begin</summary>
<param name="iter">Name of the iterator to get the start of.</param>
<returns>
	-
</returns>
<remarks>
	Gets a point BEFORE the start of the iterator (the theoretical beginning).
</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Begin(%1) Iter_Size(%1)
#define Iter_Start Iter_Begin

/**--------------------------------------------------------------------------**\
<summary>Iter_End</summary>
<param name="iter">Name of the iterator to get the end of.</param>
<returns>
	-
</returns>
<remarks>
	Gets a point AFTER the end of the iterator (think "MAX_PLAYERS").
</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_End(%1) Iter_Size(%1)
#define Iter_Finish Iter_End

/**--------------------------------------------------------------------------**\
<summary>Iter_First</summary>
<param name="iter">Name of the iterator to get the first valid element in.</param>
<returns>
	-
</returns>
<remarks>
	Gets the first element in an iterator.
</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_First(%1) Iter_Next(%1,Iter_Begin(%1))

/**--------------------------------------------------------------------------**\
<summary>Iter_Last</summary>
<param name="iter">Name of the iterator to</param>
<returns>
	-
</returns>
<remarks>
	Gets the last element in an iterator.  Works by getting the previous item
	from the one BEFORE the first element (i.e. the one before the sentinel).
</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Last(%1) Iter_Prev(%1,Iter_End(%1))

/**--------------------------------------------------------------------------**\
<summary>Iter_Next</summary>
<param name="iter">Name of the iterator to get the next element in.</param>
<param name="cur">The current element.</param>
<returns>
	-
</returns>
<remarks>
	Gets the element in an interator after the current one.
</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Next(%1,%2) (_Y_ITER_ARRAY$%1$YSII_Ag[(%2)])

/**--------------------------------------------------------------------------**\
<summary>Iter_Prev</summary>
<param name="iter">Name of the iterator to get the previous element in.</param>
<param name="cur">The current element.</param>
<returns>
	-
</returns>
<remarks>
	Gets the element in an iterator before the current one.  Now uses a better
	method, but does an extra "mod" that isn't needed in all cases.  That's just
	to make this code work:

for (new i = Iter_Finish(Player); (i = Iter_Prev(Player, i)) != Iter_Start(Player); )

	Which is the reverse equivalent to "foreach":

for (new i = Iter_Start(Player); (i = Iter_Next(Player, i)) != Iter_Finish(Player); )

</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Prev(%1,%2) ((_Y_ITER_ARRAY$%1$YSII_Ag[((%2)-1)%Iter_InternalSize(%1)]-1)%Iter_InternalSize(%1))
#define Iter_Previous Iter_Prev

/**--------------------------------------------------------------------------**\
<summary>Iter_Back</summary>
<param name="iter">Name of the iterator to get the previous element in.</param>
<param name="cur">The current element.</param>
<returns>
	-
</returns>
<remarks>
	Slightly different to "Iter_Prev" - this returns "-1" and the end instead of
	"Iter_Start":

for (new i = Iter_Finish(Player); (i = Iter_Back(Player, i)) != -1; )

	This makes it slightly faster, but not strictly the reverse of "Iter_Next".
</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Back(%1,%2) (_Y_ITER_ARRAY$%1$YSII_Ag[((%2)-1)%Iter_InternalSize(%1)]-1)

/**--------------------------------------------------------------------------**\
<summary>Iter_InternalArray</summary>
<param name="iter">Name of the iterator to get the true name of.</param>
<returns>
	-
</returns>
<remarks>
	Accesses the internal array of an iterator.
</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_InternalArray(%1) (_Y_ITER_ARRAY$%1$YSII_Ag)

/**--------------------------------------------------------------------------**\
<summary>Iter_InternalSize</summary>
<param name="iter">Name of the iterator to get the true size of.</param>
<returns>
	-
</returns>
<remarks>
	Accesses the internal size of an iterator.
</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_InternalSize(%1) (Iter_Size(%1)+1)

/**--------------------------------------------------------------------------**\
<summary>Iter_Size</summary>
<param name="iter">Name of the iterator to get the true size of.</param>
<returns>
	-
</returns>
<remarks>
	Accesses the internal size of an iterator.
</remarks>
\**--------------------------------------------------------------------------**/

#define Iter_Size(%1) (_Y_ITER_MAYBE_ARRAY(%1))

/*

    88b           d88                                                         
    888b         d888                                                         
    88`8b       d8'88                                                         
    88 `8b     d8' 88 ,adPPYYba,  ,adPPYba, 8b,dPPYba,  ,adPPYba,  ,adPPYba,  
    88  `8b   d8'  88 ""     `Y8 a8"     "" 88P'   "Y8 a8"     "8a I8[    ""  
    88   `8b d8'   88 ,adPPPPP88 8b         88         8b       d8  `"Y8ba,   
    88    `888'    88 88,    ,88 "8a,   ,aa 88         "8a,   ,a8" aa    ]8I  
    88     `8'     88 `"8bbdP"Y8  `"Ybbd8"' 88          `"YbbdP"'  `"YbbdP"'  

*/

// This now uses "$" not ":" because it is ALWAYS stripped out (it isn't a tag
// based macro that may not be detected), and that way we can detect the "$" in
// "C2" even when there is a custom tag involved.  The second "$" allows us to
// pick up all array subscripts at once without recursion.  I'm not overly happy
// about the trailing ")" on the macros, but I can't see any other way atm.
#define _Y_ITER_ARRAY$ _:_Y_ITER_C0:_Y_ITER_C3:
#define _Y_ITER_C0:%0[%1$YSII_%4g%5) %0$YSII_%4g[%1%5)
#define _Y_ITER_C3:%0$ %0@

// Find out if the passed iterator is multi-dimensional or an old function.
#define _Y_ITER_FOREACH_SIZE(%1) _:_Y_ITER_C1:_Y_ITER_C2:(sizeof %1@YSII_Ag-1)

// This macro detects multi-dimensional iterator arrays and moves the square
// brackets to the correct location AFTER the iterator name (not in the middle).
// If we get anything in "%8" don't bother putting it back as we can no longer
// match against it because we have matched against "C1" instead.
#define _Y_ITER_C1:%8(%0[%1]%2@YSII_Ag%9-1) _Y_ITER_C1:(%0%2@YSII_Ag[]%9-1)

// I will deprecate these later.  The code to do so is in revision c9ca7fd.
#define _Y_ITER_C2:(sizeof%0(%1)@YSII_Ag-1);_:(%2=%9$%3(%4)$YSII_Ag[%5])!=_Y_ITER_MAYBE_ARRAY(%6);) _Y_ITER_C4:(sizeof %3@YSII_Ag-1);(sizeof %3@YSII_Ag-1)!=_:(%2=%3@YSII_Ag[%4][%5]);)

#define _Y_ITER_C4:%9;%8!=_:(%2=%3[][%5]);) %9;%9!=_:(%2=%3[%5]);)

#define _Y_ITER_C5:%9[%1]%2(%0(%5));) (%0(%5%1));)
#define _Y_ITER_C6:_Y_ITER_C5:%9[%1]%2[%3]%4(%0(%5));) _Y_ITER_C6:_Y_ITER_C5:%2[%3]%4(%0(%5%1,));)

#define @iterfunc%1()%7:(%0=%8)%9) YSI_NULL_ITERATOR-3);-1!=_:_Y_ITER_C6:_Y_ITER_C5:%8(%0=%1@YSII_Ag());)
#define iterfunc%0(%1) %0@YSII_Ag(%1)

// Check if this is an array or a single variable.
#define _Y_ITER_MAYBE_ARRAY(%1) _:_Y_ITER_C1:(sizeof %1@YSII_Ag-1)

// Used in array declaration.  Need to keep the "_Y_ITER_C9:" as a tag, not as a
// recursive macro.  "_Y_ITER_C3:" strips the "$" from normal declarations.
#define _Y_ITER_C9:_Y_ITER_C3:%0[%1$YSII_Cg,_Y_ITER_C3:%2[%3$YSII_Ag[%4]={%5} _Y_ITER_C9:%0@YSII_Cg[%1,%0@YSII_Ag[%1[%4]

/**--------------------------------------------------------------------------**\
<summary>Iter_Create2</summary>
<param name="name">Iterator identifier.</param>
<param name="size0">Number of iterators.</param>
<param name="size1">Number of items per iterator.</param>
<returns>
	-
</returns>
<remarks>
	Creates a new array of iterator start/array pair.
</remarks>
\**--------------------------------------------------------------------------**/

// If this ever changes, update the size reference in y_users.

#define IteratorArray Iterator //:%1[%2]<%3> %1@YSII_Cg[%2],%1@YSII_Ag[%2][%3+1]

/**--------------------------------------------------------------------------**\
	The workings of these macros are very extensively documented at:
	
		http://forum.sa-mp.com/showpost.php?p=2823668
	
\**--------------------------------------------------------------------------**/

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
#define Y_FOREACH_SEVENTH|||%9Y_FOREACH_SIXTH;%0|||%1|||%2||| new %0:%1=%0:_Y_ITER_FOREACH_SIZE(%2);_:(%1=%0:_Y_ITER_ARRAY$%2$YSII_Ag[%1])!=_Y_ITER_MAYBE_ARRAY(%2);

// This is called when there aren't.
#define Y_FOREACH_SIXTH;%0|||Y_FOREACH_SEVENTH|||%2||| %0=_Y_ITER_FOREACH_SIZE(%2);_:(%0=_Y_ITER_ARRAY$%2$YSII_Ag[%0])!=_Y_ITER_MAYBE_ARRAY(%2);

// This is called if "%1" didn't have "new" at the start.
#define Y_FOREACH_FIFTH|||Y_FOREACH_FOURTH|||%1:%2||| _Y_ITER_FOREACH_SIZE(%2);_:(%1=_Y_ITER_ARRAY$%2$YSII_Ag[%1])!=_Y_ITER_MAYBE_ARRAY(%2);

// This is the old version, but DON'T add "new" because that already exists from
// the failed "new" macro call above.
#define Y_FOREACH_SECOND|||Y_FOREACH_THIRD|||%1,%2||| %2=_Y_ITER_FOREACH_SIZE(%1);_:(%2=_Y_ITER_ARRAY$%1$YSII_Ag[%2])!=_Y_ITER_MAYBE_ARRAY(%1);

/**--------------------------------------------------------------------------**\
<summary>foreachex</summary>
<param name="data">Data to iterate through.</param>
<param name="as">Variable to set value to.</param>
<returns>
	-
</returns>
<remarks>
	Similar to foreach but doesn't declare a new variable for the iterator.
</remarks>
\**--------------------------------------------------------------------------**/

#define foreachex(%1,%2) foreach(%2:%1)

/*

    88                                                                                  
    88   ,d                                      ,d                                     
    88   88                                      88                                     
    88 MM88MMM ,adPPYba, 8b,dPPYba, ,adPPYYba, MM88MMM ,adPPYba,  8b,dPPYba, ,adPPYba,  
    88   88   a8P_____88 88P'   "Y8 ""     `Y8   88   a8"     "8a 88P'   "Y8 I8[    ""  
    88   88   8PP""""""" 88         ,adPPPPP88   88   8b       d8 88          `"Y8ba,   
    88   88,  "8b,   ,aa 88         88,    ,88   88,  "8a,   ,a8" 88         aa    ]8I  
    88   "Y888 `"Ybbd8"' 88         `"8bbdP"Y8   "Y888 `"YbbdP"'  88         `"YbbdP"'  

*/

/**--------------------------------------------------------------------------**\
Create the internal iterators.
\**--------------------------------------------------------------------------**/

#if _FOREACH_PLAYER_ITERATORS
	new
		#if _FOREACH_BOT_ITERATORS
			Iterator:Character<MAX_PLAYERS>,
			Iterator:Bot<MAX_PLAYERS>,
		#endif
			Iterator:Player<MAX_PLAYERS>;
	
	// Alternate name, "NPC" == "Bot".
	#define NPC@YSII_Cg Bot@YSII_Cg
	#define NPC@YSII_Ag Bot@YSII_Ag
#endif

/*

    88                                                                88  
    88              ,d                                                88  
    88              88                                                88  
    88 8b,dPPYba, MM88MMM ,adPPYba, 8b,dPPYba, 8b,dPPYba,  ,adPPYYba, 88  
    88 88P'   `"8a  88   a8P_____88 88P'   "Y8 88P'   `"8a ""     `Y8 88  
    88 88       88  88   8PP""""""" 88         88       88 ,adPPPPP88 88  
    88 88       88  88,  "8b,   ,aa 88         88       88 88,    ,88 88  
    88 88       88  "Y888 `"Ybbd8"' 88         88       88 `"8bbdP"Y8 88  

*/

/**--------------------------------------------------------------------------**\
	Variables to optimise memory usage by only having one copy of each string.
	Note that only strings used more than once are put here because only they
	have any gain to being located in only one place.
\**--------------------------------------------------------------------------**/

static stock
	YSI_gsOnGameModeInit[] = "Iter_OnGameModeInit",
	YSI_gsSpecifier@[] = "";

// This is a hack to build the constant number "-1" with "sizeof".
stock const
	YSI_NULL_ITERATOR[2]; // NULL target only used for "sizeof".

/**--------------------------------------------------------------------------**\
<summary>Iter_OPDCInternal</summary>
<param name="playerid">Player who left.</param>
<returns>
	-
</returns>
<remarks>
	Called AFTER "OnPlayerDisconnect" so that using "Kick" inside a "foreach"
	loop doesn't crash the server due to an OOB error.
</remarks>
\**--------------------------------------------------------------------------**/

#if _FOREACH_PLAYER_ITERATORS && _FOREACH_SETUP_MODE == 1
	public Iter_OPDCInternal(playerid)
	{
			if (IsPlayerConnected(playerid))
			{
				return;
			}
		#if _FOREACH_BOT_ITERATORS
			Iter_Remove(Character, playerid);
			if (IsPlayerNPC(playerid))
			{
				Iter_Remove(Bot, playerid);
			}
			else
		#endif
			{
				Iter_Remove(Player, playerid);
			}
	}
#endif

/**--------------------------------------------------------------------------**\
<summary>Iter_SetupPlayers</summary>
<returns>
	-
</returns>
<remarks>
	When a new mode starts, this function sets up the internal iterators with
	all the currently connected players.  Written to modify the internal
	structure of the arrays, rather than using the standard "Iter_" functions.
</remarks>
\**--------------------------------------------------------------------------**/

#if _FOREACH_PLAYER_ITERATORS
	static stock Iter_SetupPlayers()
	{
		#if defined _YSI_SPECIAL_DEBUG
				for (new i = 0; i != MAX_PLAYERS; ++i)
				{
					#if _FOREACH_BOT_ITERATORS
						Character@YSII_Ag[i] = Bot@YSII_Ag[i] = i,
					#endif
						Player@YSII_Ag[i] = i + 1;
				}
			#if _FOREACH_BOT_ITERATORS
				Character@YSII_Cg = Bot@YSII_Cg = 0,
				Character@YSII_Ag[MAX_PLAYERS] = Bot@YSII_Ag[MAX_PLAYERS] = MAX_PLAYERS,
			#endif
				Player@YSII_Ag[MAX_PLAYERS] = 0,
				Player@YSII_Cg = _Y_ITER_C9:MAX_PLAYERS;
		#else
			// Set the iterator as empty.
			#if _FOREACH_BOT_ITERATORS
				Character@YSII_Ag[MAX_PLAYERS] = Bot@YSII_Ag[MAX_PLAYERS] =
			#endif
					Player@YSII_Ag[MAX_PLAYERS] = MAX_PLAYERS;
			// Set the count to 0.
			#if _FOREACH_BOT_ITERATORS
				Character@YSII_Cg = Bot@YSII_Cg =
			#endif
					Player@YSII_Cg = _Y_ITER_C9:0;
				new
					#if _FOREACH_BOT_ITERATORS
						lastCharacter = MAX_PLAYERS,
						lastBot = MAX_PLAYERS,
					#endif
						lastPlayer = MAX_PLAYERS;
				for (new i = 0; i != MAX_PLAYERS; ++i)
				{
					if (IsPlayerConnected(i))
					{
						#if _FOREACH_BOT_ITERATORS
							Character@YSII_Ag[(i - 1) % (MAX_PLAYERS + 1)] = (lastCharacter + 1) % (MAX_PLAYERS + 1),
							lastCharacter = Character@YSII_Ag[lastCharacter] = i,
							++Character@YSII_Cg;
							if (IsPlayerNPC(i))
							{
								Bot@YSII_Ag[(i - 1) % (MAX_PLAYERS + 1)] = (lastBot + 1) % (MAX_PLAYERS + 1),
								lastBot = Bot@YSII_Ag[lastBot] = i,
								++Bot@YSII_Cg;
							}
							else
						#endif
							{
								Player@YSII_Ag[(i - 1) % (MAX_PLAYERS + 1)] = (lastPlayer + 1) % (MAX_PLAYERS + 1),
								lastPlayer = Player@YSII_Ag[lastPlayer] = i,
								++Player@YSII_Cg;
							}
					}
					else
					{
						#if _FOREACH_BOT_ITERATORS
							Character@YSII_Ag[i] = Bot@YSII_Ag[i] =
						#endif
								Player@YSII_Ag[i] = i;
					}
				}
			#if _FOREACH_BOT_ITERATORS
				Character@YSII_Ag[lastCharacter] = Bot@YSII_Ag[lastBot] = 
			#endif
					Player@YSII_Ag[lastPlayer] = MAX_PLAYERS,

			#if _FOREACH_BOT_ITERATORS
				Character@YSII_Ag[MAX_PLAYERS - 1] = (lastCharacter + 1) % (MAX_PLAYERS + 1),
				Bot@YSII_Ag[MAX_PLAYERS - 1] = (lastBot + 1) % (MAX_PLAYERS + 1),
			#endif
				Player@YSII_Ag[MAX_PLAYERS - 1] = (lastPlayer + 1) % (MAX_PLAYERS + 1);
		#endif
	}
#endif

/**--------------------------------------------------------------------------**\
<summary>Iter_PlayerConnectionCheck</summary>
<returns>
	-
</returns>
<remarks>
	This function is used by NPC scripts to detect when a new player connects.
	Sadly there is no native way to do this AFAIK (could be done with plugins),
	so we need to set a timer running.  I'm not sure if having this run very
	frequently will actually make "foreach" worse or not...
</remarks>
\**--------------------------------------------------------------------------**/

#if _FOREACH_PLAYER_ITERATORS && _FOREACH_SETUP_MODE == 2
	#if defined SetTimer
		public Iter_PlayerConnectionCheck()
	#else
		static stock _Iter_NeverUsedFunction()
	#endif
		{
			for (new i = 0; i != MAX_PLAYERS; ++i)
			{
				if (IsPlayerConnected(i))
				{
					if (!Iter_Contains(Player, i))
					{
						// Is connected, but not recorded as such.
						#if _FOREACH_BOT_ITERATORS
							Iter_Add(Character, i);
							if (IsPlayerNPC(i))
							{
								Iter_Add(Bot, i);
							}
							else
						#endif
							{
								Iter_Add(Player, i);
							}
						#if defined Iter_OnPlayerConnect
							Iter_OnPlayerConnect(i);
						#endif
					}
				}
				else
				{
					if (Iter_Contains(Player, i))
					{
						// Is not connected, but is recorded as being.
						#if defined Iter_OnPlayerDisconnect
							Iter_OnPlayerDisconnect(i, 0);
						#endif
						#if _FOREACH_BOT_ITERATORS
							Iter_Remove(Character, i);
							if (IsPlayerNPC(i))
							{
								Iter_Remove(Bot, i);
							}
							else
						#endif
							{
								Iter_Remove(Player, i);
							}
					}
				}
			}
		}
#endif

/*

    88        88                         88                   
    88        88                         88                   
    88        88                         88                   
    88aaaaaaaa88  ,adPPYba,   ,adPPYba,  88   ,d8  ,adPPYba,  
    88""""""""88 a8"     "8a a8"     "8a 88 ,a8"   I8[    ""  
    88        88 8b       d8 8b       d8 8888[      `"Y8ba,   
    88        88 "8a,   ,a8" "8a,   ,a8" 88`"Yba,  aa    ]8I  
    88        88  `"YbbdP"'   `"YbbdP"'  88   `Y8a `"YbbdP"'  

*/

/**--------------------------------------------------------------------------**\
<summary>Iter_OnNPCModeInit</summary>
<returns>
	-
</returns>
<remarks>
	Set up the arrays in an NPC mode.  More involved than other types - we need
	to set up a timer to detect "OnPlayerConnect" and "OnPlayerDisconnect".
</remarks>
\**--------------------------------------------------------------------------**/

#if _FOREACH_PLAYER_ITERATORS && _FOREACH_SETUP_MODE == 1
	public OnNPCModeInit()
	{
		Iter_SetupPlayers();
		#if defined Iter_OnNPCModeInit
			Iter_OnNPCModeInit();
		#endif
		return 1;
	}
	
	#if defined _ALS_OnNPCModeInit
		#undef OnNPCModeInit
	#else
		#define _ALS_OnNPCModeInit
	#endif
	
	#define OnNPCModeInit Iter_OnNPCModeInit
	
	#if defined Iter_OnNPCModeInit
		forward Iter_OnNPCModeInit();
	#endif
#endif

/**--------------------------------------------------------------------------**\
<summary>Iter_OnFilterScriptInit</summary>
<returns>
	-
</returns>
<remarks>
	If this is a GM this is just never used.
</remarks>
\**--------------------------------------------------------------------------**/

#if _FOREACH_PLAYER_ITERATORS && _FOREACH_SETUP_MODE == 2
	public OnFilterScriptInit()
	{
		Iter_SetupPlayers();
		#if defined Iter_OnFilterScriptInit
			Iter_OnFilterScriptInit();
		#endif
		return 1;
	}
	
	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	
	#define OnFilterScriptInit Iter_OnFilterScriptInit
	
	#if defined Iter_OnFilterScriptInit
		forward Iter_OnFilterScriptInit();
	#endif
#endif

/**--------------------------------------------------------------------------**\
<summary>Iter_OnGameModeInit</summary>
<returns>
	-
</returns>
<remarks>
	If this is a filterscript this function may be called multiple times,
	resetting the player iterators each time, but this is not a major concern.
	Detecting and acconting for that case is more hassle than it's worth.
</remarks>
\**--------------------------------------------------------------------------**/

#if _FOREACH_PLAYER_ITERATORS && _FOREACH_SETUP_MODE == 2
	public OnGameModeInit()
	{
		Iter_SetupPlayers();
		#if defined Iter_OnGameModeInit
			Iter_OnGameModeInit();
		#endif
		return 1;
	}
	
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	
	#define OnGameModeInit Iter_OnGameModeInit
	
	#if defined Iter_OnGameModeInit
		forward Iter_OnGameModeInit();
	#endif
#endif

/*

    88888888ba  88                                                         
    88      "8b 88                                                         
    88      ,8P 88                                                         
    88aaaaaa8P' 88 ,adPPYYba, 8b       d8  ,adPPYba, 8b,dPPYba, ,adPPYba,  
    88""""""'   88 ""     `Y8 `8b     d8' a8P_____88 88P'   "Y8 I8[    ""  
    88          88 ,adPPPPP88  `8b   d8'  8PP""""""" 88          `"Y8ba,   
    88          88 88,    ,88   `8b,d8'   "8b,   ,aa 88         aa    ]8I  
    88          88 `"8bbdP"Y8     Y88'     `"Ybbd8"' 88         `"YbbdP"'  
                                  d8'                                      
                                 d8'                                       

*/

/**--------------------------------------------------------------------------**\
<summary>Iter_OnPlayerConnect</summary>
<param name="playerid">Player who joined.</param>
<returns>
	-
</returns>
<remarks>
	Adds a player to the loop data.  Now sorts the list too.  Note that I found
	the most bizzare bug ever (I *think* it may be a compiler but, but it
	requires further investigation), basically it seems that multiple variables
	were being treated as the same variable (namely @YSII_EgotS and
	@YSII_CgharacterS were the same and @YSII_EgotC and @YSII_CgharacterC were the
	same).  Adding print statements which reference these variables seem to fix
	the problem, and I've tried to make sure that the values will never actually
	get printed.
</remarks>
\**--------------------------------------------------------------------------**/

#if _FOREACH_PLAYER_ITERATORS
	public OnPlayerConnect(playerid)
	{
		#if _FOREACH_SETUP_MODE == 2
			#if defined KillTimer
				if (YSI_gsConnectionTimer != -1)
				{
					KillTimer(YSI_gsConnectionTimer);
					YSI_gsConnectionTimer = -1;
				}
			#endif
		#endif
		#if _FOREACH_BOT_ITERATORS
			Iter_Add(Character, playerid);
			if (IsPlayerNPC(playerid))
			{
				Iter_Add(Bot, playerid);
			}
			else
		#endif
			{
				Iter_Add(Player, playerid);
			}
		#if defined Iter_OnPlayerConnect
			return Iter_OnPlayerConnect(playerid);
		#else
			return 1;
		#endif
	}
	
	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	
	#define OnPlayerConnect Iter_OnPlayerConnect
	
	#if defined Iter_OnPlayerConnect
		forward Iter_OnPlayerConnect(playerid);
	#endif
#endif

/**--------------------------------------------------------------------------**\
<summary>Iter_OnPlayerDisconnect</summary>
<param name="playerid">Player who left.</param>
<returns>
	-
</returns>
<remarks>
	Removes a player from the loop data.  No longer uses "hook" to ENSURE that
	this is always last.  Previously I think that the order of evaluation in
	y_hooks meant that this got called before the user "OnPlayerDisconnect".
</remarks>
\**--------------------------------------------------------------------------**/

#if _FOREACH_PLAYER_ITERATORS
	public OnPlayerDisconnect(playerid, reason)
	{
		#if _FOREACH_SETUP_MODE == 2
				if (YSI_gsConnectionTimer != -1)
				{
					KillTimer(YSI_gsConnectionTimer);
					YSI_gsConnectionTimer = -1;
				}
			#if _FOREACH_BOT_ITERATORS
				Iter_Remove(Character, playerid);
				if (IsPlayerNPC(playerid))
				{
					Iter_Remove(Bot, playerid);
				}
				else
			#endif
				{
					Iter_Remove(Player, playerid);
				}
		#else
				SetTimerEx("Iter_OPDCInternal", 0, false, "i", playerid);
		#endif
		#if defined Iter_OnPlayerDisconnect
				return Iter_OnPlayerDisconnect(playerid, reason);
		#else
				return 1;
		#endif
	}
	
	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif
	
	#define OnPlayerDisconnect Iter_OnPlayerDisconnect
	
	#if defined Iter_OnPlayerDisconnect
		forward Iter_OnPlayerDisconnect(playerid, reason);
	#endif
#endif

/*

           db        88888888ba  88      ,ad8888ba,                       88             
          d88b       88      "8b 88     d8"'    `"8b                      88             
         d8'`8b      88      ,8P 88    d8'                                88             
        d8'  `8b     88aaaaaa8P' 88    88             ,adPPYba,   ,adPPYb,88  ,adPPYba,  
       d8YaaaaY8b    88""""""'   88    88            a8"     "8a a8"    `Y88 a8P_____88  
      d8""""""""8b   88          88    Y8,           8b       d8 8b       88 8PP"""""""  
     d8'        `8b  88          88     Y8a.    .a8P "8a,   ,a8" "8a,   ,d88 "8b,   ,aa  
    d8'          `8b 88          88      `"Y8888Y"'   `"YbbdP"'   `"8bbdP"Y8  `"Ybbd8"'  

*/

/**--------------------------------------------------------------------------**\
<summary>Iter_AddInternal</summary>
<param name="&start">Array start index.</param>
<param name="&count">Number of items in the iterator.</param>
<param name="array[]">Iterator data.</param>
<param name="value">Item to add.</param>
<returns>
	-
</returns>
<remarks>
	Adds a value to a given iterator set.  Now detects when you try and add the
	last item multiple times, as well as all the other items.  Now simplified even
	further with the new internal representation.
	
	Now does reverse iteration as well.
</remarks>
\**--------------------------------------------------------------------------**/

stock Iter_AddInternal(&count, array[], value, size)
{
	if (0 <= value < size && array[value] <= value)
	{
		new
			last = size,
			next = array[last];
		while (next < value)
		{
			last = next,
			next = array[last];
		}
		return
			array[next - 1] = value + 1,
			array[(value - 1) % (size + 1)] = (last + 1) % (size + 1),
			array[last] = value,
			array[value] = next,
			++count,
			1;
	}
	return 0;
}

/**--------------------------------------------------------------------------**\
<summary>Iter_RemoveInternal</summary>
<param name="&count">Number of items in the iterator.</param>
<param name="array[]">Iterator data.</param>
<param name="value">Item to remove.</param>
<returns>
	-
</returns>
<remarks>
	Removes a value from an iterator.
</remarks>
\**--------------------------------------------------------------------------**/

stock Iter_RemoveInternal(&count, array[], value, size)
{
	new
		last;
	return Iter_SafeRemoveInternal(count, array, value, last, size);
}

/**--------------------------------------------------------------------------**\
<summary>Iter_FreeInternal</summary>
<param name="count">Number of items in the iterator.</param>
<param name="array[]">Iterator data.</param>
<param name="size">Size of the iterator.</param>
<returns>
	-
</returns>
<remarks>
	Finds the first free slot in the iterator.  Iterators now HAVE to be sorted
	for this function to work correctly as it uses that fact to decide whether a
	slot is unused or the last one.  If you want to use the slot straight after
	finding it the iterator will need to re-find it to add in the data.
</remarks>
\**--------------------------------------------------------------------------**/

stock Iter_FreeInternal(array[], size)
{
	for (new i = 0; i != size; ++i)
	{
		if (array[i] <= i)
		{
			return i;
		}
	}
	return -1;
}

/**--------------------------------------------------------------------------**\
<summary>Iter_ContainsInternal</summary>
<param name="array[]">Iterator data.</param>
<param name="value">Item to check.</param>
<param name="size">Size of the iterator.</param>
<returns>
	-
</returns>
<remarks>
	Checks if this item is in the iterator.
</remarks>
\**--------------------------------------------------------------------------**/

stock Iter_ContainsInternal(array[], value, size)
{
	return 0 <= value < size && array[value] <= value;
}

/**--------------------------------------------------------------------------**\
<summary>Iter_SafeRemoveInternal</summary>
<param name="&count">Number of items in the iterator.</param>
<param name="array[]">Iterator data.</param>
<param name="back[]">Reverse iterator data.</param>
<param name="value">Item to remove.</param>
<param name="&last">Pointer in which to store the last pointer.</param>
<returns>
	-
</returns>
<remarks>
	Removes a value from an iterator safely.
</remarks>
\**--------------------------------------------------------------------------**/

stock Iter_SafeRemoveInternal(&count, array[], value, &last, size)
{
	if (0 <= value < size && array[value] > value)
	{
		return
			// Adjustment for easier "mod"ing.
			++size,
			last = (array[(value - 1) % size] - 1) % size,
			// Store the reverse value here as well as in the previous slot.
			array[value] =
				// Copy the next value to the last value.
				array[(array[last] = array[value]) - 1] =
					// Set the reverse iterator value.
					(last + 1) % size,
			--count,
			1;
	}
	return 0;
}

/**--------------------------------------------------------------------------**\
<summary>Iter_RandomInternal</summary>
<param name="count">Number of items in the iterator.</param>
<param name="array[]">Iterator data.</param>
<param name="size">Size of the iterator.</param>
<returns>
	-
</returns>
<remarks>
	Returns a random value from an iterator.
</remarks>
\**--------------------------------------------------------------------------**/

stock Iter_RandomInternal(count, array[], size)
{
	if (count)
	{
		new
			cur = array[size];
		for (new rnd = random(count); rnd--; )
		{
			cur = array[cur];
		}
		return cur;
	}
	return -1;
}

/**--------------------------------------------------------------------------**\
<summary>Iter_ClearInternal</summary>
<param name="&count">Number of items in the iterator.</param>
<param name="array[]">Iterator data.</param>
<param name="back[]">Reverse data.</param>
<param name="size">Size of the iterator.</param>
<returns>
	-
</returns>
<remarks>
	Resets an iterator.
</remarks>
\**--------------------------------------------------------------------------**/

stock Iter_ClearInternal(&count, array[], size)
{
	#if defined memset
		memset(array, 0, size),
	#else
		for (new i = 0; i != size; ++i)
		{
			array[i] = 0;
		}
	#endif
		array[size] = size,
		count = 0;
}

/**--------------------------------------------------------------------------**\
<summary>Iter_InitInternal</summary>
<param name="array[][]">Iterator array to initialise.</param>
<param name="s0">Size of first dimension.</param>
<param name="s1">Size of second dimension.</param>
<returns>
	-
</returns>
<remarks>
	Multi-dimensional arrays can't be initialised at compile time, so need to be
	done at run time, which is slightly annoying.
</remarks>
\**--------------------------------------------------------------------------**/

stock Iter_InitInternal(arr[][], s0, s1)
{
	for (new i = 0; i != s0; ++i)
	{
		#if defined memset
			memset(arr[i], 0, s1),
		#else
			for (new j = 0; j != s1; ++j)
			{
				arr[i][j] = 0;
			}
		#endif
			arr[i][s1] = s1;
	}
}

/*

      ,ad8888ba,  88                                                            
     d8"'    `"8b 88                                                            
    d8'           88                                                            
    88            88  ,adPPYba, ,adPPYYba, 8b,dPPYba,  88       88 8b,dPPYba,   
    88            88 a8P_____88 ""     `Y8 88P'   `"8a 88       88 88P'    "8a  
    Y8,           88 8PP""""""" ,adPPPPP88 88       88 88       88 88       d8  
     Y8a.    .a8P 88 "8b,   ,aa 88,    ,88 88       88 "8a,   ,a88 88b,   ,a8"  
      `"Y8888Y"'  88  `"Ybbd8"' `"8bbdP"Y8 88       88  `"YbbdP'Y8 88`YbbdP"'   
                                                                   88           
                                                                   88           

*/

#undef _FOREACH_TEST_IS_UNSET
#undef _FOREACH_PLAYER_ITERATORS
#undef _FOREACH_BOT_ITERATORS
#undef _FOREACH_SETUP_MODE
