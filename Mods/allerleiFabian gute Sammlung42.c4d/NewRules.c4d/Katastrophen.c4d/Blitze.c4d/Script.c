/*-- Blitze --*/

#strict 3

protected func Activate(iPlr)
{
  CreateMenu(FXL1,GetCursor(iPlr), this());
  AddMenuItem("Blitze +1", "Add", FXL1, GetCursor(iPlr), nil, iPlr);
  AddMenuItem("Blitze -1", "Sub", _NOG, GetCursor(iPlr), nil, iPlr);
}

func Add(dummy,iPlr)
{
  Local(nil)=(Local(nil) ?? 0)+1;
  if(Equal(Local(nil), 4)) Local(nil)=3;
  if(Equal(Local(nil), nil)) Message("Keine Blitze!");
  if(Equal(Local(nil), 1)) Message("Wenige Blitze!");
  if(Equal(Local(nil), 2)) Message("Viele Blitze!");
  if(Equal(Local(nil), 3)) Message("Unmengen Blitze!");
  Activate(iPlr);
  return(Check());
}

func Sub(dummy,iPlr)
{
  Local(nil)=(Local(nil) ?? 0)-1;
  if(Equal(Local(nil), -1)) Local(nil)=nil;
  if(Equal(Local(nil), nil)) Message("Keine Blitze!");
  if(Equal(Local(nil), 1)) Message("Wenige Blitze!");
  if(Equal(Local(nil), 2)) Message("Viele Blitze!");
  if(Equal(Local(nil), 3)) Message("Unmengen Blitze!");
  Activate(iPlr);
  return(Check());
}

func Check()
{
  if(Equal(Local(nil), nil)) return(1);
  if(Equal(Local(nil), 1)) return(SetAction("Blitz1"));
  if(Equal(Local(nil), 2)) return(SetAction("Blitz2"));
  if(Equal(Local(nil), 3)) return(SetAction("Blitz3"));
}

func Blitzen()
{
  LaunchLightning(Random(LandscapeWidth()), nil, -20, 41, +5, 15);
  return(Check());
}