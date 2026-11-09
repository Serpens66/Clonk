/*-- Auswahlobjekt --*/

#strict 3

local alpha;


protected func Initialize()
{
  // nur für den Besitzer zu sehen
  SetVisibility(VIS_Owner);
  alpha = 255;
}

public func Init(side, selection)
{
  // einfaden lassen
  SetAction("Appear", selection);
  if (Equal(side, 1))
    SetVertex(nil,nil, -80);
  else
  {
    SetGraphics("2");
    SetVertex(nil,nil, +80);
  }
  // alle anderen Objekte dieser Sorte ausfaden lassen
  var obj;
  while (obj = FindObject(SEL2,nil,nil,nil,nil,nil,nil,selection))
    ObjectSetAction(obj, "Disappear");
}

private func Appear()
{
  S42MigrationAssign(alpha, "-=", 30);
  if ((alpha ?? 0) <= 0)
  {
    alpha = nil;
    SetAction("Show");
  }
  SetClrModulation(RGBa(255,255,255,alpha));
}

private func Disappear()
{
  S42MigrationAssign(alpha, "+=", 30);
  if ((alpha ?? 0) >= 255)
    return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,alpha));
}

