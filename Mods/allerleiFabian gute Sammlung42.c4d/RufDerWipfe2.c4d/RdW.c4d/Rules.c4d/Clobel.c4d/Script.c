#strict

public func Researched(iPlayer,idTech) // int iPlayer; c4id Tech
{
  if(regelkontrolle) 
    if(regelkontrolle->~IsActive())
	   return(1);
  if(GetPlayerCount()==1) return(0);
  for(var i=0; i<GetPlayerCount(); i++) 
   if(GetPlrKnowledge(i,idTech))
    return(0);
  DoScore(150,iPlayer);
  DoWealth(iPlayer,50);
  Log("%s entwickelt %s als erster!",GetPlayerName(iPlayer),GetName(0,idTech));
  return(1);
}
  
Activate:
  if(GetPlayerCount()==1) return(MessageWindow("Keine Konkurrenten",Par(0)));
  return(MessageWindow("Jeder Innovation wird mit 150 Punkten belohnt",Par(0)));
