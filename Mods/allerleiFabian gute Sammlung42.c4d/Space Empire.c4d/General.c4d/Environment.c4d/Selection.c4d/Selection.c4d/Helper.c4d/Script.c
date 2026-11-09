/*-- Auswahlobjekt --*/

#strict

local alpha;


protected func Initialize()
{
  // nur für den Besitzer zu sehen
  SetVisibility(VIS_Owner());
  alpha = 255;
}

public func Init(side, selection)
{
  // einfaden lassen
  SetAction("Appear", selection);
  if (side == 1)
    SetVertex(0,0, -80);
  else
  {
    SetGraphics("2");
    SetVertex(0,0, +80);
  }
  // alle anderen Objekte dieser Sorte ausfaden lassen
  var obj;
  while (obj = FindObject(SEL2,0,0,0,0,0,0,selection))
    ObjectSetAction(obj, "Disappear");
}

private func Appear()
{
  alpha -= 30;
  if (alpha <= 0)
  {
    alpha = 0;
    SetAction("Show");
  }
  SetClrModulation(RGBa(255,255,255,alpha));
}

private func Disappear()
{
  alpha += 30;
  if (alpha >= 255)
    return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,alpha));
}

