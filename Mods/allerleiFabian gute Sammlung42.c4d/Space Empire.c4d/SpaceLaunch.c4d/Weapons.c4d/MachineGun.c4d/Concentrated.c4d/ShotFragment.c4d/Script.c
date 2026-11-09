/*-- Schussfragment --*/

#strict

local counter;

public func SetCounter(value)
{
  counter = value;
  SetAction("Count");
  return(1);
}

private func Count()
{
  if (--counter) return(1);
  Explode(4);
  return(1);
}

