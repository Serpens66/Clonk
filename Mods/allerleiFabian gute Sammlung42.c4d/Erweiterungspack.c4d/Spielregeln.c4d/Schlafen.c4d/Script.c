/*-- Schlafen --*/

#strict

protected func Activate(iPlr)
{
  var pClonk= GetCursor(iPlr);
  CreateMenu(GetID(),pClonk,this(),0,"$TxtSleep$",0,1);
  if(!GetCrewEnabled(pClonk))
    AddMenuItem(Format("$TxtWakeUp$",GetName(pClonk)),"WakeUp",GetID(pClonk),pClonk,0,pClonk,0,4,pClonk);
  else
    AddMenuItem(Format("$TxtYes$",GetName(pClonk)),"GoSleep",GetID(pClonk),pClonk,0,pClonk,0,4,pClonk);
  var pCrew;
  while(pCrew= FindObjectOwner(0,iPlr,0,0,0,0,OCF_CrewMember(),0,0,pCrew))
   if(pCrew != pClonk)
    if(!GetCrewEnabled(pCrew)) 
     AddMenuItem(Format("$TxtWakeUp$",GetName(pCrew)),"WakeUp",GetID(pCrew),pClonk,0,pCrew,0,4,pCrew);
    else
     AddMenuItem(Format("$TxtYes$",GetName(pCrew)),"GoSleep",GetID(pCrew),pClonk,0,pCrew,0,4,pCrew);
}

private func WakeUp(idItem,pClonk)
{
  EffectCall(pClonk,GetEffect("Sleep",pClonk),"Stop");
}

private func GoSleep(idItem,pClonk)
{
  if(GetRemainingCrew(GetOwner(pClonk)) <= 1) return(Message("$TxtAtLeastOneClonk$",pClonk));
  AddCommand(pClonk,"Call",this(),ObjectNumber(pClonk),0,0,0,"Sleep");
  if(FindBase(GetOwner(pClonk))) AddCommand(pClonk,"Home");
}

private func Sleep(Par0,iClonk)
{
  var pClonk= Object(iClonk);
  SetCrewEnabled(0,pClonk);
  AddEffect("Sleep",pClonk,250,25,this());
}

protected func FxSleepStart(pTarget,iEffectNumber)
{
  // Scheintoter Zustand
  ObjectSetAction(pTarget,"Dead");
  // mit schnarchen anfangen
  Sound("Snore",0,pTarget,0,0,+1);
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
  ObjectSetAction(pTarget,"FlatUp",0,0,1);
  // mit schnarchen aufhören
  Sound("Snore",0,pTarget,0,0,-1);
  // stehenbleiben
  SetComDir(COMD_Stop(),pTarget);
  // Effekt löschen
  if(GetEffect("Sleep",pTarget)) RemoveEffect(0,pTarget,iEffectNumber);  
  // Fertig
  return(1);
}

protected func FxSleepTimer(pTarget,iEffectNumber,iTime)
{
  // Zzz-Partikel casten
  CreateParticle("Zzz",-GetX()+GetX(pTarget),-GetY()+GetY(pTarget),2+(111%50)*3/25+RandomX(-1,+1),-5,60,RGB(255,255,255));
}

protected func FxSleepDamage(pTarget,iEffectNumber,iDmgEngy)
{
  // Bei Damage aufwachen
  RemoveEffect(0,pTarget,iEffectNumber);
  return(iDmgEngy);
}

private func GetRemainingCrew(iPlr)
{
  var pCrew,i;
  while(pCrew= FindObjectOwner(0,iPlr,0,0,0,0,OCF_CrewMember(),0,0,pCrew))
   if(GetCrewEnabled(pCrew))
    i++;
  return(i);
}