/*-- Monster erzeugen --*/

#strict 3

protected func Activate(iPlr)
{
  CreateMenu(MONS,GetCursor(iPlr), this());
  AddMenuItem("Monster erzeugen", "Create", MONS, GetCursor(iPlr), nil, iPlr);
  AddMenuItem("Abbruch", "Abort", _NO1, GetCursor(iPlr), nil, iPlr);
}

public func Create(dummy, iPlr)
{
  SetVar(nil,RandomX(nil,1));
  if(Equal(Var(nil),nil))  SetVar(1,MONS);
  if(Equal(Var(nil),1))  SetVar(1,FMNS);
  CreateObject(Var(1),Random(LandscapeWidth()),nil,-1);
  Activate(iPlr);
}

public func Abort()
{
  return(1);
}