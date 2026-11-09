/*-- Geldbeutel --*/

#strict

local Money;

public func Activate(pClonk)
{
  SetWealth(GetOwner(pClonk),GetWealth(GetOwner(pClonk))+ Money);
  if(Money)
    {
    Message("$ShowMoney$",pClonk,Money);
    Sound("Cash");
    }
  else
    {
    Message("$IsEmpty$",pClonk);
    Sound("CommandFailure1");
    }
  RemoveObject();
}

protected func Hit() { Sound("WoodHit*"); }