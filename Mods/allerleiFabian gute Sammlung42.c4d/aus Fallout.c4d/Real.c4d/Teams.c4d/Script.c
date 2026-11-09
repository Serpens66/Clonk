/*-- Clonks können mehr tragen --*/

#strict

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
  while(MaxTeamNum>1)
  {
	MaxTeamNum--;
    var TxtOwner= GetTaggedPlayerName(Global(10000+MaxTeamNum*2-1)-1);
    if(!TxtOwner) TxtOwner= "";
	AddMenuItem(Format("Bündnis %d, %s",MaxTeamNum,TxtOwner), Format("JoinTeam(%d,%d)",iPlr,MaxTeamNum), 4, pClonk);
  }
  return(1);
}

func JoinTeam(iPlr,iTeam)
{
  Leave(iPlr);
  if(!Global(10000+iTeam*2-1)) Global(10000+iTeam*2-1) = iPlr+1;//Besitzer
  Global(1000+iPlr)= iTeam;
  Global(10000+iTeam*2)+=2^(iPlr+1);//Teammitgleid hinzufügen
  return(1);
}

func Leave(iPlr)
{
  var iTeam= Global(1000+iPlr);
  if(Global(10000+iTeam*2-1)=iPlr+1) Global(10000+iTeam*2-1) = 0;//keinen Besitzer
  Global(1000+iPlr)= 0;
  Global(10000+iTeam*2)-=2^(iPlr+1);//Teammitgleid herusnehmen
  return(1);
}

/*Auswirkungen*/
func Timer()
{
  for(var iPlr= GetPlayerCount(); iPlr; iPlr--)
  {
   if(!Global(1000+iPlr-1))
   {
    for(var iCrewNum= GetCrewCount(iPlr-1);iCrewNum;iCrewNum--) SetColorDw(GetPlrColorDw(iPlr-1),GetCrew(iPlr-1,iCrewNum-1));
    return(1);
   }
   for(var iCrewNum= GetCrewCount(iPlr-1);iCrewNum;iCrewNum--) SetColor(Global(1000+iPlr-1),GetCrew(iPlr-1,iCrewNum-1));
  }
  return(1);
}