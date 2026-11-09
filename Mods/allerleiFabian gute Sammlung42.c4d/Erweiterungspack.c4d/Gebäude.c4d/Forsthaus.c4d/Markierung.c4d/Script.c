#strict

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
  if(Number== 1) SetPosition(GetX()-iChange,GetY()-iChange);
  if(Number== 2) SetPosition(GetX()-iChange,GetY()+iChange);
  if(Number== 3) SetPosition(GetX()+iChange,GetY()-iChange);
  if(Number== 4) SetPosition(GetX()+iChange,GetY()+iChange);
}