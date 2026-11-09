#strict 3

local iXDir,iYDir,pThrower;

/* --- Boomerang --- */

protected func Departure(pObj)
{
  if(GetXDir() && ((GetOCF(pObj) ?? 0)&(OCF_Living ?? 0)))
  {
    pThrower=pObj;
    SetAction("Depart");
  }
}

func QueryClonkHit(object clonk)
{
  // Nicht treffen, wenn gerade aus dem Clonk raus
  if (S42MigrationEqual(clonk, pThrower))
    if (S42MigrationEqual(GetAction(), "Depart") || (S42MigrationEqual(GetAction(), "Fly") && (GetActTime() ?? 0)<=3))
      return true;
  // Treffen!
  return nil;
}

protected func Departure2()
{
  if(Contained()) { SetAction("Idle"); return; }
  Sound("Catapult");
  SetXDir((GetXDir() ?? 0)*3);
  SetYDir(-50);
  SetRDir((GetXDir() ?? 0)*2);
  iXDir=(((GetXDir() ?? 0)>0) ?? 0)*2-1;
  iYDir=20;
}

protected func Hit()
{
  Sound("BmHit");
  if(!Random(5)) return true;
  if(Random(3)) SetRDir(0-(GetRDir() ?? 0));
  if(!GBackSolid(nil,-10)) SetYDir(-8);
  if(!ActIdle()) NormalFlight();
  if(GBackSolid(-10,nil)) return(SetXDir(+15));
  if(GBackSolid(10,nil))  return(SetXDir(-15));
  SetXDir((GetXDir() ?? 0)/2);
}

protected func FlyCheck()
{
  if(S42MigrationStep(iYDir, -1) < 6) return(NormalFlight());
  SetXDir((GetXDir() ?? 0)-(iXDir ?? 0)*10);
  if((iYDir ?? 0)>17) SetYDir((GetYDir() ?? 0)+12);
  if((iYDir ?? 0)<16) SetYDir((GetYDir() ?? 0)-10);
}

func NormalFlight()
{
  SetRDir((GetRDir() ?? 0)/4);
  SetYDir((GetYDir() ?? 0)/2);
  SetAction("Idle");
  iXDir=nil;
  iYDir=1;
}

public func IsAnvilProduct() { return(1); }