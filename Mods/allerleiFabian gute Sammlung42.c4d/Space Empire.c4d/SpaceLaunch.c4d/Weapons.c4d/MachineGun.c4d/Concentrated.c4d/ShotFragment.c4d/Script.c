/*-- Schussfragment --*/

#strict 3

local counter;

public func SetCounter(value)
{
  counter = value;
  SetAction("Count");
  return(1);
}

private func Count()
{
  if (S42MigrationStep(counter, -1)) return(1);
  Explode(4);
  return(1);
}

