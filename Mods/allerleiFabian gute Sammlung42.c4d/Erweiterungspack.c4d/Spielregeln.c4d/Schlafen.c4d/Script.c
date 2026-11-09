/*-- Schlafen --*/

#strict 3

protected func Activate(iPlr)
{
  var pClonk= GetCursor(iPlr);
  CreateMenu(GetID(),pClonk,this(),nil,"$TxtSleep$",nil,1);
  if(!GetCrewEnabled(pClonk))
    AddMenuItem(Format("$TxtWakeUp$",GetName(pClonk)),"WakeUp",GetID(pClonk),pClonk,nil,pClonk,nil,4,pClonk);
  else
    AddMenuItem(Format("$TxtYes$",GetName(pClonk)),"GoSleep",GetID(pClonk),pClonk,nil,pClonk,nil,4,pClonk);
  var pCrew;
  while(pCrew= FindObjectOwner(nil,iPlr,nil,nil,nil,nil,OCF_CrewMember,nil,nil,pCrew))
   if(!Equal(pCrew, pClonk))
    if(!GetCrewEnabled(pCrew)) 
     AddMenuItem(Format("$TxtWakeUp$",GetName(pCrew)),"WakeUp",GetID(pCrew),pClonk,nil,pCrew,nil,4,pCrew);
    else
     AddMenuItem(Format("$TxtYes$",GetName(pCrew)),"GoSleep",GetID(pCrew),pClonk,nil,pCrew,nil,4,pCrew);
}

private func WakeUp(idItem,pClonk)
{
  EffectCall(pClonk,GetEffect("Sleep",pClonk),"Stop");
}

private func GoSleep(idItem,pClonk)
{
  if((GetRemainingCrew(GetOwner(pClonk)) ?? 0) <= 1) return(Message("$TxtAtLeastOneClonk$",pClonk));
  AddCommand(pClonk,"Call",this(),ObjectNumber(pClonk),nil,nil,nil,"Sleep");
  if(FindBase(GetOwner(pClonk))) AddCommand(pClonk,"Home");
}

private func Sleep(Par0,iClonk)
{
  var pClonk= Object(iClonk);
  SetCrewEnabled(nil,pClonk);
  AddEffect("Sleep",pClonk,250,25,this());
}

protected func FxSleepStart(pTarget,iEffectNumber)
{
  // Scheintoter Zustand
  ObjectSetAction(pTarget,"Dead");
  // mit schnarchen anfangen
  Sound("Snore",nil,pTarget,nil,nil,+1);
  // Fertig
  return(1);
}

protected func FxSleepStop(pTarget,iEffectNumber)
{
  // clonk hat den Schlaf nicht überstanden ;)
  if(!GetAlive(pTarget)) return(1);
  // wieder anwählbar machen
  SetCrewEnabled(1,pTarget);
  // aufstehen
  ObjectSetAction(pTarget,"FlatUp",nil,nil,1);
  // mit schnarchen aufhören
  Sound("Snore",nil,pTarget,nil,nil,-1);
  // stehenbleiben
  SetComDir(COMD_Stop,pTarget);
  // Effekt löschen
  if(GetEffect("Sleep",pTarget)) RemoveEffect(nil,pTarget,iEffectNumber);
  // Fertig
  return(1);
}

protected func FxSleepTimer(pTarget,iEffectNumber,iTime)
{
  // Zzz-Partikel casten
  CreateParticle("Zzz",-(GetX() ?? 0)+(GetX(pTarget) ?? 0),-(GetY() ?? 0)+(GetY(pTarget) ?? 0),2+(111%50)*3/25+(RandomX(-1,+1) ?? 0),-5,60,RGB(255,255,255));
}

protected func FxSleepDamage(pTarget,iEffectNumber,iDmgEngy)
{
  // Bei Damage aufwachen
  RemoveEffect(nil,pTarget,iEffectNumber);
  return(iDmgEngy);
}

private func GetRemainingCrew(iPlr)
{
  var pCrew,i;
  while(pCrew= FindObjectOwner(nil,iPlr,nil,nil,nil,nil,OCF_CrewMember,nil,nil,pCrew))
   if(GetCrewEnabled(pCrew))
    S42MigrationStep(i, 1, true);
  return(i);
}