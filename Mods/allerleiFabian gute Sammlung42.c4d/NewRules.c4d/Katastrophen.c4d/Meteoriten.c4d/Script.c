/*-- Meteore --*/

#strict 3

protected func Activate(iPlr,iSelection)
{
  CreateMenu(METO,GetCursor(iPlr), this());
  AddMenuItem("Meteore +1", "Add", METO, GetCursor(iPlr), nil, iPlr);
  AddMenuItem("Meteore -1", "Sub", _NOM, GetCursor(iPlr), nil, iPlr);
  SelectMenuItem(iSelection);
}

func Add(dummy, iPlr)
{
  Local(nil)=(Local(nil) ?? 0)+1;
  if(Equal(Local(nil), 4)) Local(nil)=3;
  if(Equal(Local(nil), nil)) Message("Keine Meteore!");
  if(Equal(Local(nil), 1)) Message("Wenige Meteore!");
  if(Equal(Local(nil), 2)) Message("Viele Meteore!");
  if(Equal(Local(nil), 3)) Message("Unmengen Meteore!");
  var iSelection;
  if(Equal(dummy, METO)) iSelection=nil;
  if(Equal(dummy, _NOM)) iSelection=1;
  Activate(iPlr,iSelection);
  return(Check());
}

func Sub(dummy,iPlr)
{
  Local(nil)=(Local(nil) ?? 0)-1;
  if(Equal(Local(nil), -1)) Local(nil)=nil;
  if(Equal(Local(nil), nil)) Message("Keine Meteore!");
  if(Equal(Local(nil), 1)) Message("Wenige Meteore!");
  if(Equal(Local(nil), 2)) Message("Viele Meteore!");
  if(Equal(Local(nil), 3)) Message("Unmengen Meteore!");
  var iSelection;
  if(Equal(dummy, METO)) iSelection=nil;
  if(Equal(dummy, _NOM)) iSelection=1;
  Activate(iPlr,iSelection);
  return(Check());
}

func Check()
{
  if(Equal(Local(nil), nil)) return(1);
  if(Equal(Local(nil), 1)) return(SetAction("Met1"));
  if(Equal(Local(nil), 2)) return(SetAction("Met2"));
  if(Equal(Local(nil), 3)) return(SetAction("Met3"));
}

func Fall()
{
  CreateObject(METO,Random(LandscapeWidth()),nil,-1);
  return(Check());
}