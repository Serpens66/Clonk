#strict

global func GetWarlevel(iPlayer) // int 
{
  var warlevel;
  for(var i=0;i<GetPlayerCount();i++)
   if(GetPlayerName(i))
   if(Hostile(iPlayer,i) && GetGovernment(i) ne "Faschismus")
    warlevel++;
  return(warlevel);
}

Dawn:
  if(GetPlayerCount()==1) return(0);
  var warlevel;
  for(var i=0;i<GetPlayerCount();i++) 
  {
	warlevel=GetWarlevel(i);
 	var faktor=1;
 	if(GetGovernment(i)S="Demokratie") faktor=2;
	if(!warlevel) DoScore(i,+50*faktor);
		else  DoScore(i,-30*warlevel*faktor);
  }
  return(1);
  
Activate:
  if(GetPlayerCount()==1) return(MessageWindow("Keine Konkurrenten",Par(0)));
  return(MessageWindow(Format("Du bist mit %d Spielern im Krieg",GetWarlevel(Par(0))),Par(0)));