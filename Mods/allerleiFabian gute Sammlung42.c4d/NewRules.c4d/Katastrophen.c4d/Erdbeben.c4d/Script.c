/*-- Erdbeben --*/

#strict

protected func Activate(iPlr)
{
  CreateMenu(FXQ1,GetCursor(iPlr), this());
  AddMenuItem("Erdbeben +1", "Add", FXQ1, GetCursor(iPlr), 0, iPlr);
  AddMenuItem("Erdbeben -1", "Sub", _NOQ, GetCursor(iPlr), 0, iPlr);
}

func Add(dummy,iPlr)
{
  Local(0)=Local(0)+1;
  if(Local(0)==4) Local(0)=3;
  if(Local(0)==0) Message("Keine Erdbeben!");
  if(Local(0)==1) Message("Wenige Erdbeben!");
  if(Local(0)==2) Message("Mehrere Erdbeben!");
  if(Local(0)==3) Message("Viele Erdbeben!");
  Activate(iPlr);
  return(Check());
}

func Sub(dummy,iPlr)
{
  Local(0)=Local(0)-1;
  if(Local(0)==-1) Local(0)=0;
  if(Local(0)==0) Message("Keine Erdbeben!");
  if(Local(0)==1) Message("Wenige Erdbeben!");
  if(Local(0)==2) Message("Mehrere Erdbeben!");
  if(Local(0)==3) Message("Viele Erdbeben!");
  Activate(iPlr);
  return(Check());
}

func Check()
{
  if(Local(0)==0) return(1);
  if(Local(0)==1) return(SetAction("Erd1"));
  if(Local(0)==2) return(SetAction("Erd2"));
  if(Local(0)==3) return(SetAction("Erd3"));
}

func Beben()
{
  var iX=Random(LandscapeWidth());
  var iDY=0;
  while(!GBackSolid(iX,iDY) && iDY+GetY()<=LandscapeHeight()) iDY+=5;
  LaunchEarthquake(GetX()+iX,GetY()+iDY);
  return(Check());
}