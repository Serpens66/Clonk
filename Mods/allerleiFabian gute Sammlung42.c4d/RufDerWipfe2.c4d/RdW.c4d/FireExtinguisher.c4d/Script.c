/*-- Feuerlöscher --*/

#strict 3

local iFill,iAmount;

static const RdW_Y3B_MaxFill=5;

public func Initialize()
{
  iFill=RdW_Y3B_MaxFill;
}

public func CheckRefill()
{
  if((iFill ?? 0)>=(RdW_Y3B_MaxFill ?? 0))
    return nil;
  S42MigrationAssign(iAmount, "+=", (ExtractMaterialAmount(nil,nil,Material("Water"),25) ?? 0));
  if((iAmount ?? 0)>=25)
  {
    S42MigrationStep(iFill, 1, true);
    S42MigrationAssign(iAmount, "-=", 25);
    SetGraphics(Format("%d",(RdW_Y3B_MaxFill ?? 0)-(iFill ?? 0)));
  }
}

/* Aktivieren */

public func Activate()
{
  // noch beim Nachladen
  if(!SEqual(GetAction(), "Idle")) return(nil);
  // benötigt einen Container
  if(!Contained()) return(nil);
  // leer?
  if((iFill ?? 0)<=0)  return Message("Leer...",this);
  Extinguish(Contained());
  SetAction("Firing");
  // Sound
  Sound("BlowTorch");
  // Zeichnen
  AddEffect("IntDraw_Y3B",Contained(),1,5,this,GetID());
  return(1);
}

func FxIntDraw_Y3BStart(pTarget)
{
  SetGraphics("",pTarget,_Y3B,4,GFXOV_MODE_Object,nil,nil,this);
  SetLayerTransform(4,nil,((GetDir() ?? 0)*2-1)*100,100,nil,nil,pTarget);
  SetPhysical("Walk",1000,PHYS_StackTemporary,pTarget);
}

func FxIntDraw_Y3BTimer(pTarget)
{
  SetLayerTransform(4,nil,((GetDir() ?? 0)*2-1)*100,100,nil,nil,pTarget);
  if(Or(!Contained(), !SEqual(GetAction(), "Firing")))
    return -1;
}

func FxIntDraw_Y3BStop(pTarget)
{
  SetGraphics(nil,pTarget,nil,4);
  ResetPhysical(pTarget, "Walk");
}

private func Firing()
{
  // Wasser abfeuern
  var dir = ((GetDir(Contained()) ?? 0)*2-1);
  var ammo = CreateContents(SFES);
  Exit(ammo, (dir ?? 0)*10, Random(2), Random(360), (GetXDir() ?? 0)/8+(dir ?? 0)*(Random(6) ?? 0)+(dir ?? 0)*2, (Random(6) ?? 0)-2, (Random(100) ?? 0)-50);
  InsertMaterial(Material("Water"), (dir ?? 0)*10, Random(2), (GetXDir() ?? 0)+(dir ?? 0)*(Random(60) ?? 0)+(dir ?? 0)*20, (Random(60) ?? 0)-20);
  // Nachladen nicht vergessen
  if(Or(!Contained(), (GetActTime() ?? 0) >= 25))
  {
    SetAction("Reload");
    S42MigrationStep(iFill, -1, true);
    SetGraphics(Format("%d",(RdW_Y3B_MaxFill ?? 0)-(iFill ?? 0)));
  }
  return(1);
}


func IsAnvilProduct() {return(1);}