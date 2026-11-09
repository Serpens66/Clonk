#strict

// indizierte Locals werden als Array für Regierungsformen genutzt

protected func Activate(player)
{
  CreateMenu(RDW6,GetCursor(player),this());
  if(!FindObjectOwner(REVO,player))
  	if(	GetPlrKnowledge(player, _BBA) ||
		GetPlrKnowledge(player, _BAY) ||
		GetPlrKnowledge(player, _BAZ) )
		{
		  AddMenuItem("%s","Liste",CXIN,GetCursor(player),0,player);
			AddMenuItem("%s","Revolution",REVO,GetCursor(player),0,player);
			return 1;
		}
	return Liste(0,player);
}

private func Liste(lassmichinruh,showplayer)
{
  var string,government;
  string="";
  for(var i = 0,player; i < GetPlayerCount(); i++)
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
  AddMenuItem("Tyrannei","NeueReg",ROCK,GetCursor(showplayer),0,showplayer);
  if(GetPlrKnowledge(showplayer, _BAY)) AddMenuItem("%s","NeueReg",_BAY,GetCursor(showplayer),0,showplayer);
  if(GetPlrKnowledge(showplayer, _BAZ)) AddMenuItem("%s","NeueReg",_BAZ,GetCursor(showplayer),0,showplayer);
  if(GetPlrKnowledge(showplayer, _BBA)) AddMenuItem("%s","NeueReg",_BBA,GetCursor(showplayer),0,showplayer);
  return(1);
}

private func NeueReg(reg,showplayer)
{
  var revolution=CreateObject(REVO,0,0,showplayer);
  revolution->REVO::StarteRevolution(showplayer,reg);
  return(1);
}
  

global func SetGovernment(iPlayer, c4idReg)
{
  var regel=FindObject(RDW6);
  if(!regel) return(0);
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
  if(!regel) return(0);

  if(Local(iPlayer,regel)==_BAY) return("Faschismus");
  if(Local(iPlayer,regel)==_BAZ) return("Kommunismus");
  if(Local(iPlayer,regel)==_BBA) return("Demokratie");
  if(Local(iPlayer,regel)==ROCK) return(0);
  if(Local(iPlayer,regel)==REVO) return("Revolution");
  return(0);
}

private func GetGovernmentLOKAL(iPlayer)
{
  if(Local(iPlayer)==_BAY) return("Faschismus");
  if(Local(iPlayer)==_BAZ) return("Kommunismus");
  if(Local(iPlayer)==_BBA) return("Demokratie");
  if(Local(iPlayer)==ROCK) return("Tyrannei");
  if(Local(iPlayer)==REVO) return("Revolution");
  return(0);
}
