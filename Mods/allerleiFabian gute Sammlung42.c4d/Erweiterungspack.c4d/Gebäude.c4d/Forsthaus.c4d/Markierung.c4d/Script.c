#strict 3

local Number,Forsthaus;

protected func Init(pHut,iNum)
{
  // 1: Lo; | 2: Lu; | 3: Ro; | 4: Ru;
  Number= iNum;
  Forsthaus= pHut;
  SetAction("A1");
}

public func Move(iChange)
{
  if(Equal(Number, 1)) SetPosition((GetX() ?? 0)-(iChange ?? 0),(GetY() ?? 0)-(iChange ?? 0));
  if(Equal(Number, 2)) SetPosition((GetX() ?? 0)-(iChange ?? 0),(GetY() ?? 0)+(iChange ?? 0));
  if(Equal(Number, 3)) SetPosition((GetX() ?? 0)+(iChange ?? 0),(GetY() ?? 0)-(iChange ?? 0));
  if(Equal(Number, 4)) SetPosition((GetX() ?? 0)+(iChange ?? 0),(GetY() ?? 0)+(iChange ?? 0));
}