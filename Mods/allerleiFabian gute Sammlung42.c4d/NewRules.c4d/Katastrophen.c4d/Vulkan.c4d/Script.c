/*-- Vulkane --*/

#strict 3

protected func Activate(iPlr)
{
  CreateMenu(FXV1,GetCursor(iPlr), this());
  AddMenuItem("Vulkane +1", "Add", FXV1, GetCursor(iPlr), nil, iPlr);
  AddMenuItem("Vulkane -1", "Sub", _NOU, GetCursor(iPlr), nil, iPlr);
}

func Add(dummy, iPlr)
{
  Local(nil)=(Local(nil) ?? 0)+1;
  if(Equal(Local(nil), 4)) Local(nil)=3;
  if(Equal(Local(nil), nil)) Message("Keine Vulkane!");
  if(Equal(Local(nil), 1)) Message("Wenige Vulkane!");
  if(Equal(Local(nil), 2)) Message("Mehrere Vulkane!");
  if(Equal(Local(nil), 3)) Message("Viele Vulkane!");
Activate(iPlr);
  return(Check());
}

func Sub(dummy, iPlr)
{
  Local(nil)=(Local(nil) ?? 0)-1;
  if(Equal(Local(nil), -1)) Local(nil)=nil;
  if(Equal(Local(nil), nil)) Message("Keine Vulkane!");
  if(Equal(Local(nil), 1)) Message("Wenige Vulkane!");
  if(Equal(Local(nil), 2)) Message("Mehrere Vulkane!");
  if(Equal(Local(nil), 3)) Message("Viele Vulkane!");
Activate(iPlr);
  return(Check());
}

func Check()
{
  if(Equal(Local(nil), nil)) return(1);
  if(Equal(Local(nil), 1)) return(SetAction("Vul1"));
  if(Equal(Local(nil), 2)) return(SetAction("Vul2"));
  if(Equal(Local(nil), 3)) return(SetAction("Vul3"));
}

func Ausbruch()
{
  LaunchVolcano(Random(LandscapeWidth())); 
  return(Check());
}