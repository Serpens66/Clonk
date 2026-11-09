/*-- Licht --*/

#strict

local color;

public func Init(target, vx, vy, size, clr)
{
  if (size) SetCon(size);
  if (clr)  color = clr;
       else color = RGB(255,255,255);
  SetVertex(0,0,vx);
  SetVertex(0,1,vy);
  SetAction("Attach",target);
  SetPosition(GetX(target)+GetVertex(0,0,target)-vx,
              GetY(target)+GetVertex(0,1,target)-vy);
  ResetModulation();
}

private func ResetModulation()
{
//  SetClrModulation(color + ((63-DayDarkness/4) << 24));
SetClrModulation(color + Random(24));
}

