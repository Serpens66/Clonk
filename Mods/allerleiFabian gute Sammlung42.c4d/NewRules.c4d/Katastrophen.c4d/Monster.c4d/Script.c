/*-- Monster erzeugen --*/

#strict

protected func Activate(iPlr)
{
  CreateMenu(MONS,GetCursor(iPlr), this());
  AddMenuItem("Monster erzeugen", "Create", MONS, GetCursor(iPlr), 0, iPlr);
  AddMenuItem("Abbruch", "Abort", _NO1, GetCursor(iPlr), 0, iPlr);
}

public func Create(dummy, iPlr)
{
  SetVar(0,RandomX(0,1));
  if(Equal(Var(0),0))  SetVar(1,MONS);
  if(Equal(Var(0),1))  SetVar(1,FMNS);
  CreateObject(Var(1),Random(LandscapeWidth()),0,-1);
  Activate(iPlr);
}

public func Abort()
{
  return(1);
}