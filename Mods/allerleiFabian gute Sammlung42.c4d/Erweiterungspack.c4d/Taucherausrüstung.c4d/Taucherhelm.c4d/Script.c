/*-- Taucherhelm --*/

#strict 3

protected func Hit() { Sound("ClonkHit*"); }

protected func Activate(pClonk)
{
  [$PutOn$|Image=THLM]
  // if(GetID(pClonk)== HZCK)
  // return(PlayerMessage(GetOwner(pClonk),"Nur von unspezialisierten Clonks verwendbar",pClonk));
  if(FindObject(GetID(),nil,nil,nil,nil,nil,"Wear",pClonk))
   return(PlayerMessage(GetOwner(pClonk),"$AlreadyHasHelmet$",pClonk,GetName(pClonk)));
if(pClonk->~HasGear(GEAR_Armor))
   return(PlayerMessage(GetOwner(pClonk),"Trägt bereits eine Rüstung",pClonk)); // auch nicht mit anderen rüstungen kombinieren
  SetComDir(COMD_Stop,pClonk);
  SetAction("Wear",pClonk);
  // SetPhysical("Walk",35000,2,pClonk);
  // SetPhysical("Jump",15000,2,pClonk);
  // SetPhysical("Scale",5000,2,pClonk);
  // SetPhysical("Hangle",5000,2,pClonk);
  // SetPhysical("Dig",10000,2,pClonk);
  // SetPhysical("Swim",30000,2,pClonk);
  // SetPhysical("Push",5000,2,pClonk);
  // SetPhysical("Fight",15000,2,pClonk);
  SetPhysical("Walk",35000,PHYS_StackTemporary,pClonk);
  SetPhysical("Jump",15000,PHYS_StackTemporary,pClonk);
  SetPhysical("Scale",5000,PHYS_StackTemporary,pClonk);
  SetPhysical("Hangle",5000,PHYS_StackTemporary,pClonk);
  SetPhysical("Dig",10000,PHYS_StackTemporary,pClonk);
  SetPhysical("Swim",40000,PHYS_StackTemporary,pClonk);
  SetPhysical("Push",5000,PHYS_StackTemporary,pClonk);
  SetPhysical("Fight",15000,PHYS_StackTemporary,pClonk);
  Sound("Connect");
  PlayerMessage(GetOwner(pClonk),"$PutOnHelmet$",pClonk);
  return(1);
}

private func Extract(pClonk)
{
  SetComDir(COMD_Stop,pClonk);
  SetAction("Idle");
  if(Contained(pClonk))
    {
    SetVisibility(VIS_All);
    Enter(Contained(pClonk));
    }
  // ResetPhysical(pClonk); 
  
  ResetPhysical(pClonk, "Throw");
  ResetPhysical(pClonk, "Walk");
  ResetPhysical(pClonk, "Scale");
  ResetPhysical(pClonk, "Hangle");
  ResetPhysical(pClonk, "Jump");
  ResetPhysical(pClonk, "Dig");
  ResetPhysical(pClonk, "Swim");
  ResetPhysical(pClonk, "Fight");
  
  Sound("Connect");
  if(GetAlive(pClonk)) PlayerMessage(GetOwner(pClonk),"Taucherhelm abgelegt.",pClonk);
}

private func Check()
{
  var clonk= GetActionTarget();
  if(!GetAlive(clonk)) return(Extract(clonk));
  SetDir(GetDir(clonk));
  if(!!SEqual(GetAction(clonk), "Swim"))
    {
    // if(GetYDir(clonk)> 0) SetPhysical("Swim",60000,3,clonk);
		     // else SetPhysical("Swim",30000,3,clonk);
    if(Equal(GetDir(clonk), DIR_Left)) SetPosition((GetX() ?? 0)-3,(GetY() ?? 0)+1);
			      else SetPosition((GetX() ?? 0)+3,(GetY() ?? 0)+1);
    }
  var pump= GetActionTarget(1,FindObject(PWRL,nil,nil,nil,nil,nil,"Connect",this()));
  if(!pump) return(nil);
  if(!SEqual(GetAction(pump), "Working")) return(nil);
  DoBreath(+1,clonk); 
}

protected func RejectEntrance() { if(!!SEqual(GetAction(), "Wear")) return(1); }

protected func IsAnvilProduct() { return(1); }