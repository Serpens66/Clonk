/*-- Eisregen --*/

#strict 3

protected func Activate(iPlr)
{
  CreateMenu(FXL1,GetCursor(iPlr), this());
  AddMenuItem("Eisregen +1", "Add", ICE1, GetCursor(iPlr), nil, iPlr);
  AddMenuItem("Eisregen -1", "Sub", _NO1, GetCursor(iPlr), nil, nil);
}

func Add()
{
  Local(nil)=(Local(nil) ?? 0)+1;
  if(Equal(Local(nil), 4)) Local(nil)=3;
  if(Equal(Local(nil), nil)) Message("Kein Eisregen!");
  if(Equal(Local(nil), 1)) Message("Leichter Eisregen!");
  if(Equal(Local(nil), 2)) Message("Eisregen!");
  if(Equal(Local(nil), 3)) Message("Starker Eisregen!");
  return(Check());
}

func Sub()
{
  Local(nil)=(Local(nil) ?? 0)-1;
  if(Equal(Local(nil), -1)) Local(nil)=nil;
  if(Equal(Local(nil), nil)) Message("Kein Eisregen!");
  if(Equal(Local(nil), 1)) Message("Leichter Eisregen!");
  if(Equal(Local(nil), 2)) Message("Eisregen!");
  if(Equal(Local(nil), 3)) Message("Starker Eisregen!");
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
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  return(Check());
}

func DoIt2()
{
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  return(Check());
}

func DoIt3()
{
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  CastPXS("Ice",1,nil,Random(LandscapeWidth()),nil);
  return(Check());
}