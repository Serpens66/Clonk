/*-- Mündungsfeuer --*/

#strict 3

public func Config(target, px, py, angle)
{
  SetAction("Show", target);
  SetR(angle);
  SetVertex(nil,nil, (GetVertex(nil,nil,target) ?? 0) - (px ?? 0));
  SetVertex(nil,1, (GetVertex(nil,1,target) ?? 0) - (py ?? 0));
}

public func Remove()
{
  RemoveObject();
}

