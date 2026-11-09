/*-- Blitze --*/

#strict

protected func Activate(iPlr)
{
  CreateMenu(FXL1,GetCursor(iPlr), this());
  AddMenuItem("Blitze +1", "Add", FXL1, GetCursor(iPlr), 0, iPlr);
  AddMenuItem("Blitze -1", "Sub", _NOG, GetCursor(iPlr), 0, iPlr);
}

func Add(dummy,iPlr)
{
  Local(0)=Local(0)+1;
  if(Local(0)==4) Local(0)=3;
  if(Local(0)==0) Message("Keine Blitze!");
  if(Local(0)==1) Message("Wenige Blitze!");
  if(Local(0)==2) Message("Viele Blitze!");
  if(Local(0)==3) Message("Unmengen Blitze!");
  Activate(iPlr);
  return(Check());
}

func Sub(dummy,iPlr)
{
  Local(0)=Local(0)-1;
  if(Local(0)==-1) Local(0)=0;
  if(Local(0)==0) Message("Keine Blitze!");
  if(Local(0)==1) Message("Wenige Blitze!");
  if(Local(0)==2) Message("Viele Blitze!");
  if(Local(0)==3) Message("Unmengen Blitze!");
  Activate(iPlr);
  return(Check());
}

func Check()
{
  if(Local(0)==0) return(1);
  if(Local(0)==1) return(SetAction("Blitz1"));
  if(Local(0)==2) return(SetAction("Blitz2"));
  if(Local(0)==3) return(SetAction("Blitz3"));
}

func Blitzen()
{
  LaunchLightning(Random(LandscapeWidth()), 0, -20, 41, +5, 15);
  return(Check());
}