/*-- Erdbeben --*/

#strict 3

protected func Activate(iPlr)
{
  CreateMenu(FXQ1,GetCursor(iPlr), this());
  AddMenuItem("Erdbeben +1", "Add", FXQ1, GetCursor(iPlr), nil, iPlr);
  AddMenuItem("Erdbeben -1", "Sub", _NOQ, GetCursor(iPlr), nil, iPlr);
}

func Add(dummy,iPlr)
{
  Local(nil)=(Local(nil) ?? 0)+1;
  if(Equal(Local(nil), 4)) Local(nil)=3;
  if(Equal(Local(nil), nil)) Message("Keine Erdbeben!");
  if(Equal(Local(nil), 1)) Message("Wenige Erdbeben!");
  if(Equal(Local(nil), 2)) Message("Mehrere Erdbeben!");
  if(Equal(Local(nil), 3)) Message("Viele Erdbeben!");
  Activate(iPlr);
  return(Check());
}

func Sub(dummy,iPlr)
{
  Local(nil)=(Local(nil) ?? 0)-1;
  if(Equal(Local(nil), -1)) Local(nil)=nil;
  if(Equal(Local(nil), nil)) Message("Keine Erdbeben!");
  if(Equal(Local(nil), 1)) Message("Wenige Erdbeben!");
  if(Equal(Local(nil), 2)) Message("Mehrere Erdbeben!");
  if(Equal(Local(nil), 3)) Message("Viele Erdbeben!");
  Activate(iPlr);
  return(Check());
}

func Check()
{
  if(Equal(Local(nil), nil)) return(1);
  if(Equal(Local(nil), 1)) return(SetAction("Erd1"));
  if(Equal(Local(nil), 2)) return(SetAction("Erd2"));
  if(Equal(Local(nil), 3)) return(SetAction("Erd3"));
}

func Beben()
{
  var iX=Random(LandscapeWidth());
  var iDY=nil;
  while(And(!GBackSolid(iX,iDY), (iDY ?? 0)+(GetY() ?? 0)<=(LandscapeHeight() ?? 0))) S42MigrationAssign(iDY, "+=", 5);
  LaunchEarthquake((GetX() ?? 0)+(iX ?? 0),(GetY() ?? 0)+(iDY ?? 0));
  return(Check());
}