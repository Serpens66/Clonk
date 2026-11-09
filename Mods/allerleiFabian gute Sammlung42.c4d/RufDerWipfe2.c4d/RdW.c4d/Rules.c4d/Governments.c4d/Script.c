#strict 3

// indizierte Locals werden als Array für Regierungsformen genutzt

protected func Activate(player)
{
  CreateMenu(RDW6,GetCursor(player),this());
  if(!FindObjectOwner(REVO,player))
        if(	Or(Or(GetPlrKnowledge(player, _BBA),
		GetPlrKnowledge(player, _BAY)),
		GetPlrKnowledge(player, _BAZ)) )
		{
		  AddMenuItem("%s","Liste",CXIN,GetCursor(player),nil,player);
			AddMenuItem("%s","Revolution",REVO,GetCursor(player),nil,player);
			return 1;
		}
	return Liste(nil,player);
}

private func Liste(lassmichinruh,showplayer)
{
  var string,government;
  string="";
  for(var i = nil,player; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
  {
    player=GetPlayerByIndex(i);
  	government=GetGovernmentLOKAL(player);
   	if(government) 	string=Format("%s %s hat die Regierungsform %s.|",string,GetTaggedPlayerName(player),government);
  	else		string=Format("%s %s hat keine Regierung.|",string,GetTaggedPlayerName(player));
  }
  MessageWindow(Format("%s",string),showplayer);
  return(1);
}

private func Revolution(lassmichinruh,showplayer)
{
  CreateMenu(REVO,GetCursor(showplayer),this());
  AddMenuItem("Tyrannei","NeueReg",ROCK,GetCursor(showplayer),nil,showplayer);
  if(GetPlrKnowledge(showplayer, _BAY)) AddMenuItem("%s","NeueReg",_BAY,GetCursor(showplayer),nil,showplayer);
  if(GetPlrKnowledge(showplayer, _BAZ)) AddMenuItem("%s","NeueReg",_BAZ,GetCursor(showplayer),nil,showplayer);
  if(GetPlrKnowledge(showplayer, _BBA)) AddMenuItem("%s","NeueReg",_BBA,GetCursor(showplayer),nil,showplayer);
  return(1);
}

private func NeueReg(reg,showplayer)
{
  var revolution=CreateObject(REVO,nil,nil,showplayer);
  revolution->REVO::StarteRevolution(showplayer,reg);
  return(1);
}
  

global func SetGovernment(iPlayer, c4idReg)
{
  var regel=FindObject(RDW6);
  if(!regel) return(nil);
  Local(iPlayer,regel)=c4idReg;
  // Alle benachrichtigen
  GameCallEx("OnGovernmentChange",iPlayer,c4idReg);
  for(var obj in FindObjects(Find_Owner(iPlayer)))
    obj->~OnGovernmentChange(c4idReg);
  return(1);
}

global func GetGovernment(iPlayer)
{
  var regel=FindObject(RDW6);
  if(!regel) return(nil);

  if(Equal(Local(iPlayer,regel), _BAY)) return("Faschismus");
  if(Equal(Local(iPlayer,regel), _BAZ)) return("Kommunismus");
  if(Equal(Local(iPlayer,regel), _BBA)) return("Demokratie");
  if(Equal(Local(iPlayer,regel), ROCK)) return(nil);
  if(Equal(Local(iPlayer,regel), REVO)) return("Revolution");
  return(nil);
}

private func GetGovernmentLOKAL(iPlayer)
{
  if(Equal(Local(iPlayer), _BAY)) return("Faschismus");
  if(Equal(Local(iPlayer), _BAZ)) return("Kommunismus");
  if(Equal(Local(iPlayer), _BBA)) return("Demokratie");
  if(Equal(Local(iPlayer), ROCK)) return("Tyrannei");
  if(Equal(Local(iPlayer), REVO)) return("Revolution");
  return(nil);
}
