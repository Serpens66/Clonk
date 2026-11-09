/*-- Geldbeutel --*/

#strict 3

local Money;

public func Activate(pClonk)
{
  SetWealth(GetOwner(pClonk),(GetWealth(GetOwner(pClonk)) ?? 0)+ (Money ?? 0));
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