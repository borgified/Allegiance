/*-------------------------------------------------------------------------
  pch.h
  
  Precompiled header for Lobby
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _LOBBY_PCH_
#define _LOBBY_PCH_

// #include "appWeb.h" //Imago: HTTP Library for ASGS services

#include <windows.h>
#include <stdlib.h>
#include <memory.h>
#include <dplobby.h>
#include <mmsystem.h> // wlp 2006 added = somehow appweb.h excludes timegetTime in this module 



// This also includes <atlbase.h> and <atlcom.h>
#include "LobbyModule.h"
#include <stdio.h>

// identifier was truncated to '255' characters in the debug information
#pragma warning(disable : 4786)

// 'identifier' : decorated name length exceeded, name was truncated
#pragma warning(disable : 4503)

#include <map>

#define __MODULE__ "Lobby"
#include <zlib.h>
#include <utility.h>


#include <TCLib.h>
#include <TCAtl.h>
#include <..\TCAtl\ObjectMap.h>
#include <..\TCAtl\UserAcct.h>
#include <..\TCLib\TCThread.h>

#ifdef USEAUTH
#include "LobQueries.h" // and allegdb.h
#endif
#include <guids.h>
#include "resource.h"
#include "messagecore.h"
#include "messagesAll.h"
#include "messagesLS.h"
#include "messagesLC.h"
#include "sharemem.h"
#include "counters.h"
#include "zauth.h"
#include "broadcast.h"
#include "zgameinfo.h"
#include "LobbyApp.h"
#include "mission.h"
#include "client.h"
#include "server.h"
#include "AutoUpdate.h"
#include "LobbyEvents.h"


/////////////////////////////////////////////////////////////////////////////
// This is included very last since it conflicts with <atlwin.h>, included
// indirectly by <TCAtl.h>.
//
#include <windowsx.h> // For GlobalAllocPtr and GlobalFreePtr


/////////////////////////////////////////////////////////////////////////////

#endif
