#strict 3

public func Researched(iPlayer,idTech) // int iPlayer; c4id Tech
{
  if(regelkontrolle) 
    if(regelkontrolle->~IsActive())
	   return(1);
  if(Equal(GetPlayerCount(), 1)) return(nil);
  for(var i=nil; (i ?? 0)<(GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
   if(GetPlrKnowledge(i,idTech))
    return(nil);
  DoScore(150,iPlayer);
  DoWealth(iPlayer,50);
  Log("%s entwickelt %s als erster!",GetPlayerName(iPlayer),GetName(nil,idTech));
  return(1);
}
  
func Activate(...) {
  if(Equal(GetPlayerCount(), 1)) return(MessageWindow("Keine Konkurrenten",Par(nil)));
  return(MessageWindow("Jeder Innovation wird mit 150 Punkten belohnt",Par(nil)));
}
