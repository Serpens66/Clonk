/*-- Meteore --*/

#strict

protected func Activate(iPlr,iSelection)
{
  CreateMenu(METO,GetCursor(iPlr), this());
  AddMenuItem("Meteore +1", "Add", METO, GetCursor(iPlr), 0, iPlr);
  AddMenuItem("Meteore -1", "Sub", _NOM, GetCursor(iPlr), 0, iPlr);
  SelectMenuItem(iSelection);
}

func Add(dummy, iPlr)
{
  Local(0)=Local(0)+1;
  if(Local(0)==4) Local(0)=3;
  if(Local(0)==0) Message("Keine Meteore!");
  if(Local(0)==1) Message("Wenige Meteore!");
  if(Local(0)==2) Message("Viele Meteore!");
  if(Local(0)==3) Message("Unmengen Meteore!");
  var iSelection;
  if(dummy==METO) iSelection=0;
  if(dummy==_NOM) iSelection=1;
  Activate(iPlr,iSelection);
  return(Check());
}

func Sub(dummy,iPlr)
{
  Local(0)=Local(0)-1;
  if(Local(0)==-1) Local(0)=0;
  if(Local(0)==0) Message("Keine Meteore!");
  if(Local(0)==1) Message("Wenige Meteore!");
  if(Local(0)==2) Message("Viele Meteore!");
  if(Local(0)==3) Message("Unmengen Meteore!");
  var iSelection;
  if(dummy==METO) iSelection=0;
  if(dummy==_NOM) iSelection=1;
  Activate(iPlr,iSelection);
  return(Check());
}

func Check()
{
  if(Local(0)==0) return(1);
  if(Local(0)==1) return(SetAction("Met1"));
  if(Local(0)==2) return(SetAction("Met2"));
  if(Local(0)==3) return(SetAction("Met3"));
}

func Fall()
{
  CreateObject(METO,Random(LandscapeWidth()),0,-1);
  return(Check());
}