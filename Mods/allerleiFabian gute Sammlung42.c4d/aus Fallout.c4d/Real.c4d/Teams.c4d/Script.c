/*-- Clonks können mehr tragen --*/

#strict 3

protected func Activate(iByPlayer)
{
  var pClonk= GetCursor(iByPlayer);
  CreateMenu(GetID(), pClonk, this());
  // Menüeinträge erzeugen
  //AddMenuItem("Mitglied kicken", Format("Kick(%d)",iByPlayer), 4, pClonk);
  AddMenuItem("Bündins eingehen", Format("Join(%d)",iByPlayer), 4, pClonk);
  AddMenuItem("Bündins brechen", Format("Leave(%d)",iByPlayer), 4, pClonk);
  return(1);
}

func Join(iPlr)
{
  var MaxTeamNum= GameCall("MaxTeamNum");
  if(!MaxTeamNum) MaxTeamNum=10;

  var pClonk= GetCursor(iPlr);

  CreateMenu(GetID(), pClonk, this());
  while((MaxTeamNum ?? 0)>1)
  {
	S42MigrationStep(MaxTeamNum, -1, true);
    var TxtOwner= GetTaggedPlayerName((Global(10000+(MaxTeamNum ?? 0)*2-1) ?? 0)-1);
    if(!TxtOwner) TxtOwner= "";
	AddMenuItem(Format("Bündnis %d, %s",MaxTeamNum,TxtOwner), Format("JoinTeam(%d,%d)",iPlr,MaxTeamNum), 4, pClonk);
  }
  return(1);
}

func JoinTeam(iPlr,iTeam)
{
  Leave(iPlr);
  if(!Global(10000+(iTeam ?? 0)*2-1)) Global(10000+(iTeam ?? 0)*2-1) = (iPlr ?? 0)+1;//Besitzer
  Global(1000+(iPlr ?? 0))= iTeam;
  S42MigrationAssign(Global(10000+(iTeam ?? 0)*2), "+=", 2^((iPlr ?? 0)+1));//Teammitgleid hinzufügen
  return(1);
}

func Leave(iPlr)
{
  var iTeam= Global(1000+(iPlr ?? 0));
  if(Global(10000+(iTeam ?? 0)*2-1)=(iPlr ?? 0)+1) Global(10000+(iTeam ?? 0)*2-1) = nil;//keinen Besitzer
  Global(1000+(iPlr ?? 0))= nil;
  S42MigrationAssign(Global(10000+(iTeam ?? 0)*2), "-=", 2^((iPlr ?? 0)+1));//Teammitgleid herusnehmen
  return(1);
}

/*Auswirkungen*/
func Timer()
{
  for(var iPlr= GetPlayerCount(); iPlr; S42MigrationStep(iPlr, -1, true))
  {
   if(!Global(1000+(iPlr ?? 0)-1))
   {
    for(var iCrewNum= GetCrewCount((iPlr ?? 0)-1);iCrewNum;S42MigrationStep(iCrewNum, -1, true)) SetColorDw(GetPlrColorDw((iPlr ?? 0)-1),GetCrew((iPlr ?? 0)-1,(iCrewNum ?? 0)-1));
    return(1);
   }
   for(var iCrewNum= GetCrewCount((iPlr ?? 0)-1);iCrewNum;S42MigrationStep(iCrewNum, -1, true)) SetColor(Global(1000+(iPlr ?? 0)-1),GetCrew((iPlr ?? 0)-1,(iCrewNum ?? 0)-1));
  }
  return(1);
}