/*-- Destroy the banner --*/

#strict 3

private func Execute()
{
  
  // if(regelkontrolle)// Noch am Regelnauswählen
    // if(regelkontrolle->~IsActive())
      // return(1);
  
  // for(var i = 0; i < GetPlayerCount(C4PT_User); i++) // Alle Spieler überprüfen
    // if(GetPlayerName(GetPlayerByIndex(i,C4PT_User)))
      // if(!GetBanner(i)) EliminatePlayer(i);
  return(1);
}

protected  func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
  return(1);
}
