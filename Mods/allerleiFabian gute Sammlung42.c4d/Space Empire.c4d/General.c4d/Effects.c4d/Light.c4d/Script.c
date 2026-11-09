/*-- Licht --*/

#strict 3

local color;

public func Init(target, vx, vy, size, clr)
{
  if (size) SetCon(size);
  if (clr)  color = clr;
       else color = RGB(255,255,255);
  SetVertex(nil,nil,vx);
  SetVertex(nil,1,vy);
  SetAction("Attach",target);
  SetPosition((GetX(target) ?? 0)+(GetVertex(nil,nil,target) ?? 0)-(vx ?? 0),
              (GetY(target) ?? 0)+(GetVertex(nil,1,target) ?? 0)-(vy ?? 0));
  ResetModulation();
}

private func ResetModulation()
{
//  SetClrModulation(color + ((63-DayDarkness/4) << 24));
SetClrModulation((color ?? 0) + (Random(24) ?? 0));
}

