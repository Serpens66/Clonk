/*-- Relaunch --*/

#strict
#appendto CLNK

protected func Activate(Par1,a,b,c)
{
  if(GetID()== RLNC)
    {
    if(GameCall("MaxRelaunchs"))
      {
      var M= GameCall("MaxRelaunchs");
      var R= LocalN("Relaunchs",FindRelaunchHelper(Par1,this()));
      MessageWindow(Format("%s|Max. Relaunchs: %d|Relaunchs: %d",GetDesc(),M,R),Par1);
      return(1);
      }
    MessageWindow(GetDesc(),Par1);
    return(1);
    }
  _inherited(Par1,a,b,c);
}

protected func Death(a,b,c)
{
  var ok;
  if(ObjectCount(RLNC))
    {
    var Max_R= GameCall("MaxRelaunchs");
    if(!FindRelaunchHelper(GetOwner()))
      {
      var hlp= CreateContents(RHLP,FindObject(RLNC));
      LocalN("Player",hlp)= GetOwner();    
      }
    var Relaunchs= LocalN("Relaunchs",FindRelaunchHelper(GetOwner()));
    if(!Max_R || Relaunchs <= Max_R)
     if(GetOwner()!= NO_OWNER())
      {
      ok= true;
      var wipf= PlaceAnimal(WIPF),clonk= CreateObject(GetID(),0,0,GetOwner()),energy;
      SetPosition(GetX(wipf),GetY(wipf),clonk); 
      if(wipf) RemoveObject(wipf);
      MakeCrewMember(clonk,GetOwner());
      DoEnergy(+(GetPhysical("Energy",1) /1000 -GetEnergy()),clonk);
      Log("$Relaunched$",GetDefCoreVal("Name","DefCore",GetID()),GetName(),GetPlayerName(GetOwner()));
      if(Max_R)
        {
        LocalN("Relaunchs",FindRelaunchHelper(GetOwner()))++;
        Log("$Remaining$",GetPlayerName(GetOwner()),Max_R -Relaunchs);
        }
      GameCall("Relaunching",clonk,this());
      }
    }
  _inherited(a,b,c);
  // Das muss am Schluss stehen, da es sonst Probleme mit der DeathAnnounce() gibt...
  if(ok) GrabObjectInfo(this(),clonk);
  return(1);
}

private func FindRelaunchHelper(iPlr,pInto)
{
  if(!pInto) pInto= FindObject(RLNC);
  var i= ContentsCount(0,pInto);
  while(i--)
   if(GetID(Contents(i,pInto))== RHLP)
    if(LocalN("Player",Contents(i,pInto))== iPlr)
     return(Contents(i,pInto));
  return(0);
}