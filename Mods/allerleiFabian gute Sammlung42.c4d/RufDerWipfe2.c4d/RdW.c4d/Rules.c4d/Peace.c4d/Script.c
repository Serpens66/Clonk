#strict 3

global func GetWarlevel(iPlayer) // int 
{
  var warlevel;
  for(var i=nil;(i ?? 0)<(GetPlayerCount() ?? 0);S42MigrationStep(i, 1, true))
   if(GetPlayerName(i))
   if(And(Hostile(iPlayer,i), !SEqual(GetGovernment(i), "Faschismus")))
    S42MigrationStep(warlevel, 1, true);
  return(warlevel);
}

func Dawn(...) {
  if(Equal(GetPlayerCount(), 1)) return(nil);
  var warlevel;
  for(var i=nil;(i ?? 0)<(GetPlayerCount() ?? 0);S42MigrationStep(i, 1, true))
  {
	warlevel=GetWarlevel(i);
 	var faktor=1;
        if(!!SEqual(GetGovernment(i), "Demokratie")) faktor=2;
	if(!warlevel) DoScore(i,(+50 ?? 0)*(faktor ?? 0));
		else  DoScore(i,-30*(warlevel ?? 0)*(faktor ?? 0));
  }
  return(1);
}
  
func Activate(...) {
  if(Equal(GetPlayerCount(), 1)) return(MessageWindow("Keine Konkurrenten",Par(nil)));
  return(MessageWindow(Format("Du bist mit %d Spielern im Krieg",GetWarlevel(Par(nil))),Par(nil)));
}