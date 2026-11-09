#strict

protected func Check()
{
  Smoke(0,0,Random(20)+5,RGB(Random(255),Random(255),Random(255)));
  if(GetYDir() > 0 && CheckBase()) MakeBase();

}

protected func CheckBase() {
  if((GetX() < 30) || (GetX() > LandscapeWidth() - 30) || (GetY() < 60)) return(false);
  var mat = GetMaterial();
  if(mat != -1 && mat != 18) return(false);
  if(FindObject2(Find_Category(C4D_Structure),Find_Distance(60))) return(false);
  return(true);
}

protected func Entrance(pClonk) {
  SetOwner(GetOwner(pClonk));
}

protected func MakeBase() {
  MakeGround();
  CreateObject(ELEF,0,0,GetOwner());
  RemoveObject();
}

protected func MakeGround() {
  Sound("Blast1");
  Smoke(0,0,Random(30)+10,RGB(Random(255),Random(255),Random(255)));
  Smoke(0,0,Random(30)+10,RGB(Random(255),Random(255),Random(255)));
  Smoke(0,0,Random(30)+10,RGB(Random(255),Random(255),Random(255)));
  Smoke(0,0,Random(30)+10,RGB(Random(255),Random(255),Random(255)));
  Smoke(0,0,Random(30)+10,RGB(Random(255),Random(255),Random(255)));
  DrawMaterialQuad("Earth",-38+GetX(),GetY(),-14+GetX(),GetY(),-14+GetX(),4+GetY(),-38+GetX(),4+GetY());
  DrawMaterialQuad("Earth",14+GetX(),GetY(),38+GetX(),GetY(),38+GetX(),4+GetY(),14+GetX(),4+GetY());
}