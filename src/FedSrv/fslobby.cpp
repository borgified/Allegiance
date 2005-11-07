/*-------------------------------------------------------------------------
  fslobby.cpp
  
  site for FedSrvLobbySite
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
#include "pch.h"

HRESULT FedSrvLobbySite::OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)
{
  HRESULT hr = S_OK;
  assert(&g.fmLobby == pthis);

  static CTempTimer timerOnAppMessage("in FedSrvLobbySite::OnAppMessage", .02f);
  timerOnAppMessage.Start();

  switch(pfm->fmid)
  {
    #if !defined(ALLSRV_STANDALONE)
      case FM_L_CREATE_MISSION_REQ:
      {
        CASTPFM(pfmCreateMissionReq, L, CREATE_MISSION_REQ, pfm);
        MissionParams mp;
        mp.Reset();
        lstrcpy(mp.strGameName, ZString(FM_VAR_REF(pfmCreateMissionReq, NameCreator)) + "'s game");
        mp.bScoresCount = true;
        assert(!mp.Invalid());
        FedSrvSite * psiteFedSrv = new FedSrvSite();
        CFSMission * pfsMissionNew = new CFSMission(mp, "", psiteFedSrv, psiteFedSrv, NULL, NULL);
        pfsMissionNew->SetCookie(pfmCreateMissionReq->dwCookie);
      }
      break;
    #endif

    case FM_L_NEW_MISSION_ACK:
    {
      CASTPFM(pfmNewMissionAck, L, NEW_MISSION_ACK, pfm);
      CFSMission * pfsm = CFSMission::GetMissionFromIGCMissionID(pfmNewMissionAck->dwIGCMissionID);
      if (pfsm)
      {
        pfsm->SetCookie(pfmNewMissionAck->dwCookie);
        // If we already have players (e.g. reconnect), tell the lobby who's in the game
        const ShipListIGC * plistShip = pfsm->GetIGCMission()->GetShips();
        for (ShipLinkIGC * plinkShip = plistShip->first(); plinkShip; plinkShip = plinkShip->next())
        {
          IshipIGC * pShip = plinkShip->data();
          CFSShip * pfsShip = GETFSSHIP(pShip);
          if (pfsShip->IsPlayer() && !pShip->IsGhost())
          {
            CFSPlayer* pfsPlayer = pfsShip->GetPlayer();
            BEGIN_PFM_CREATE(g.fmLobby, pfmPlayerJoined, S, PLAYER_JOINED)
              FM_VAR_PARM(pfsPlayer->GetName(), CB_ZTS)
            END_PFM_CREATE
            pfmPlayerJoined->dwMissionCookie = pfsm->GetCookie();
          }
        }
        HRESULT hr = pthis->SendMessages(pthis->GetServerConnection(), FM_GUARANTEED, FM_FLUSH);
      }
    }
    break;

    case FM_L_TOKEN:
    {
      CASTPFM(pfmToken, L, TOKEN, pfm);
      Strcpy(g.m_szToken, FM_VAR_REF(pfmToken, Token));
    }
    break;
    
    case FM_L_REMOVE_PLAYER:
    {
      CASTPFM(pfmRemovePlayer, L, REMOVE_PLAYER, pfm);
      CFSMission * pfsMission = CFSMission::GetMission(pfmRemovePlayer->dwMissionCookie);
      const char* szCharacterName = FM_VAR_REF(pfmRemovePlayer, szCharacterName);
      const char* szMessageParam = FM_VAR_REF(pfmRemovePlayer, szMessageParam);
      
      // try to find the player in question
      if (!pfsMission)
      {        
        debugf("Asked to boot character %s from mission %x by lobby, "
          "but the mission was not found.\n", 
          szCharacterName, pfmRemovePlayer->dwMissionCookie);
      }
      else if (!pfsMission->RemovePlayerByName(szCharacterName, 
          (pfmRemovePlayer->reason == RPR_duplicateCDKey) ? QSR_DuplicateCDKey : QSR_DuplicateRemoteLogon,
          szMessageParam))
      {
        debugf("Asked to boot character %s from mission %x by lobby, "
          "but the character was not found.\n", 
          szCharacterName, pfmRemovePlayer->dwMissionCookie);
      }
    }
    break;

    case FM_L_LOGON_SERVER_NACK:
    {
      char * szReason;

      CASTPFM(pfmLogonNack, L, LOGON_SERVER_NACK, pfm);
      
      szReason = FM_VAR_REF(pfmLogonNack, Reason);

      OnMessageBox(pthis, szReason ? szReason : "Error while try to log onto server.", "Allegiance Server Error", MB_SERVICE_NOTIFICATION);
      // TODO: consider firing out an event message
      PostQuitMessage(-1);
    }
    break;
  }

  timerOnAppMessage.Stop("...for message type %s\n", g_rgszMsgNames[pfm->fmid]);
  return hr;
}


int FedSrvLobbySite::OnMessageBox(FedMessaging * pthis, const char * strText, const char * strCaption, UINT nType)
{
  return g.siteFedSrv.OnMessageBox(pthis, strText, strCaption, nType); // don't need a separate handler
}

HRESULT FedSrvLobbySite::OnSessionLost(FedMessaging * pthis)
{
  _AGCModule.TriggerEvent(NULL, AllsrvEventID_LostLobby, "", -1, -1, -1, 0);
  return S_OK;
}

#ifndef NO_MSG_CRC
void FedSrvLobbySite::OnBadCRC(FedMessaging * pthis, CFMConnection & cnxn, BYTE * pMsg, DWORD cbMsg)
{
  assert(0); // we should never get a bad crc from one of our own servers.
  // todo: something better for people running their own servers, but sufficient for our servers, 
  // since they communicate on a back channel
}
#endif

