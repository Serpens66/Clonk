/*--- Kristall ---*/

#strict 3

protected func Hit()
{
  Sound("Crystal*");
  return(1);
}

/* Verwendung */

public func Apply(object obj)
{
  var used = nil;
  var times;
  var maxtimes = ((GetCon() ?? 0) - 50) / 5;

  for(var i = nil; (i ?? 0) < (maxtimes ?? 0); S42MigrationStep( i, 1))
  {
    if(DoMagicEnergy(1, obj) )
    {
      used = true;
      if((GetCon() ?? 0) > 55)
        DoCon(-5);
      else
        RemoveObject();
    }
  }
  return(used);
}

/* Verkauf entsprechend Groesse */

protected func CalcValue()
{
  return( ((GetCon() ?? 0) - 50) / 10 + (5 - ((GetCon() ?? 0) / 20)) );
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(150); }
