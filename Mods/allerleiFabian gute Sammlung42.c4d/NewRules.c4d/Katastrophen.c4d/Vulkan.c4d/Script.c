/*-- Vulkane --*/

#strict

protected func Activate(iPlr)
{
  CreateMenu(FXV1,GetCursor(iPlr), this());
  AddMenuItem("Vulkane +1", "Add", FXV1, GetCursor(iPlr), 0, iPlr);
  AddMenuItem("Vulkane -1", "Sub", _NOU, GetCursor(iPlr), 0, iPlr);
}

func Add(dummy, iPlr)
{
  Local(0)=Local(0)+1;
  if(Local(0)==4) Local(0)=3;
  if(Local(0)==0) Message("Keine Vulkane!");
  if(Local(0)==1) Message("Wenige Vulkane!");
  if(Local(0)==2) Message("Mehrere Vulkane!");
  if(Local(0)==3) Message("Viele Vulkane!");
Activate(iPlr);
  return(Check());
}

func Sub(dummy, iPlr)
{
  Local(0)=Local(0)-1;
  if(Local(0)==-1) Local(0)=0;
  if(Local(0)==0) Message("Keine Vulkane!");
  if(Local(0)==1) Message("Wenige Vulkane!");
  if(Local(0)==2) Message("Mehrere Vulkane!");
  if(Local(0)==3) Message("Viele Vulkane!");
Activate(iPlr);
  return(Check());
}

func Check()
{
  if(Local(0)==0) return(1);
  if(Local(0)==1) return(SetAction("Vul1"));
  if(Local(0)==2) return(SetAction("Vul2"));
  if(Local(0)==3) return(SetAction("Vul3"));
}

func Ausbruch()
{
  LaunchVolcano(Random(LandscapeWidth())); 
  return(Check());
}