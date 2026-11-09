#strict 3

protected func Check()
{
  Smoke(nil,nil,(Random(20) ?? 0)+5,RGB(Random(255),Random(255),Random(255)));
  if(And((GetYDir() ?? 0) > 0, CheckBase())) MakeBase();

}

protected func CheckBase() {
  if(Or(Or(((GetX() ?? 0) < 30), ((GetX() ?? 0) > (LandscapeWidth() ?? 0) - 30)), ((GetY() ?? 0) < 60))) return(nil);
  var mat = GetMaterial();
  if(And(!Equal(mat, -1), !Equal(mat, 18))) return(nil);
  if(FindObject2(Find_Category(C4D_Structure),Find_Distance(60))) return(nil);
  return(true);
}

protected func Entrance(pClonk) {
  SetOwner(GetOwner(pClonk));
}

protected func MakeBase() {
  MakeGround();
  CreateObject(ELEF,nil,nil,GetOwner());
  RemoveObject();
}

protected func MakeGround() {
  Sound("Blast1");
  Smoke(nil,nil,(Random(30) ?? 0)+10,RGB(Random(255),Random(255),Random(255)));
  Smoke(nil,nil,(Random(30) ?? 0)+10,RGB(Random(255),Random(255),Random(255)));
  Smoke(nil,nil,(Random(30) ?? 0)+10,RGB(Random(255),Random(255),Random(255)));
  Smoke(nil,nil,(Random(30) ?? 0)+10,RGB(Random(255),Random(255),Random(255)));
  Smoke(nil,nil,(Random(30) ?? 0)+10,RGB(Random(255),Random(255),Random(255)));
  DrawMaterialQuad("Earth",-38+(GetX() ?? 0),GetY(),-14+(GetX() ?? 0),GetY(),-14+(GetX() ?? 0),4+(GetY() ?? 0),-38+(GetX() ?? 0),4+(GetY() ?? 0));
  DrawMaterialQuad("Earth",14+(GetX() ?? 0),GetY(),38+(GetX() ?? 0),GetY(),38+(GetX() ?? 0),4+(GetY() ?? 0),14+(GetX() ?? 0),4+(GetY() ?? 0));
}