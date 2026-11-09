/*-- Säureregen --*/

#strict 3

protected func Activate(iPlr)
{
  CreateMenu(FXL1,GetCursor(iPlr), this());
  AddMenuItem("Säureregen +1", "Add", ABRL, GetCursor(iPlr), nil, iPlr);
  AddMenuItem("Säureregen -1", "Sub", _NO1, GetCursor(iPlr), nil, nil);
}

func Add()
{
  Local(nil)=(Local(nil) ?? 0)+1;
  if(Equal(Local(nil), 4)) Local(nil)=3;
  if(Equal(Local(nil), nil)) Message("Kein Säureregen!");
  if(Equal(Local(nil), 1)) Message("Leichter Säureregen!");
  if(Equal(Local(nil), 2)) Message("Säureregen!");
  if(Equal(Local(nil), 3)) Message("Starker Säureregen!");
  return(Check());
}

func Sub()
{
  Local(nil)=(Local(nil) ?? 0)-1;
  if(Equal(Local(nil), -1)) Local(nil)=nil;
  if(Equal(Local(nil), nil)) Message("Kein Säureregen!");
  if(Equal(Local(nil), 1)) Message("Leichter Säureregen!");
  if(Equal(Local(nil), 2)) Message("Säureregen!");
  if(Equal(Local(nil), 3)) Message("Starker Säureregen!");
  return(Check());
}

func Check()
{
  if(Equal(Local(nil), nil)) return(1);
  if(Equal(Local(nil), 1)) return(SetAction("Do1"));
  if(Equal(Local(nil), 2)) return(SetAction("Do2"));
  if(Equal(Local(nil), 3)) return(SetAction("Do3"));
}

func DoIt1()
{
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  return(Check());
}

func DoIt2()
{
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  return(Check());
}

func DoIt3()
{
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Acid",1,nil,Random(LandscapeWidth()),nil);
  return(Check());
}