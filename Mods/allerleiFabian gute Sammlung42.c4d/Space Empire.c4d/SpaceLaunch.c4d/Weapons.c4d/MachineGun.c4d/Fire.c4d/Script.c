/*-- Mündungsfeuer --*/

#strict

public func Config(target, px, py, angle)
{
  SetAction("Show", target);
  SetR(angle);
  SetVertex(0,0, GetVertex(0,0,target) - px);
  SetVertex(0,1, GetVertex(0,1,target) - py);
}

public func Remove()
{
  RemoveObject();
}

