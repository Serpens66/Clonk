/*-- Feuerlöscher --*/

#strict

local iFill,iAmount;

static const RdW_Y3B_MaxFill=5;

public func Initialize()
{
  iFill=RdW_Y3B_MaxFill;
}

public func CheckRefill()
{
  if(iFill>=RdW_Y3B_MaxFill)
    return 0;
  iAmount+=ExtractMaterialAmount(0,0,Material("Water"),25);
  if(iAmount>=25)
  {
    iFill++;
    iAmount-=25;
    SetGraphics(Format("%d",RdW_Y3B_MaxFill-iFill));
  }
}

/* Aktivieren */

public func Activate()
{
  // noch beim Nachladen
  if(GetAction() ne "Idle") return(0);
  // benötigt einen Container
  if(!Contained()) return(0);
  // leer?
  if(iFill<=0)  return Message("Leer...",this);
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
  SetGraphics("",pTarget,_Y3B,4,GFXOV_MODE_Object,0,0,this);
  SetLayerTransform(4,0,(GetDir()*2-1)*100,100,0,0,pTarget);
  SetPhysical("Walk",1000,PHYS_StackTemporary,pTarget);
}

func FxIntDraw_Y3BTimer(pTarget)
{
  SetLayerTransform(4,0,(GetDir()*2-1)*100,100,0,0,pTarget);
  if(!Contained()||GetAction() ne "Firing")
    return -1;
}

func FxIntDraw_Y3BStop(pTarget)
{
  SetGraphics(0,pTarget,0,4);
  ResetPhysical(pTarget, "Walk");
}

private func Firing()
{
  // Wasser abfeuern
  var dir = (GetDir(Contained())*2-1);
  var ammo = CreateContents(SFES);
  Exit(ammo, dir*10, Random(2), Random(360), GetXDir()/8+dir*Random(6)+dir*2, Random(6)-2, Random(100)-50);
  InsertMaterial(Material("Water"), dir*10, Random(2), GetXDir()+dir*Random(60)+dir*20, Random(60)-20);
  // Nachladen nicht vergessen
  if(!Contained()||GetActTime() >= 25)
  {
    SetAction("Reload");
    iFill--;
    SetGraphics(Format("%d",RdW_Y3B_MaxFill-iFill));
  }
  return(1);
}


func IsAnvilProduct() {return(1);}