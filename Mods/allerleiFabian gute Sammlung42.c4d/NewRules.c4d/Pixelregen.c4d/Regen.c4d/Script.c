/*-- Regen --*/

#strict 3

protected func Activate(iPlr)
{
  CreateMenu(FXL1,GetCursor(iPlr), this());
  AddMenuItem("Regen +1", "Add", FXP1, GetCursor(iPlr), nil, iPlr);
  AddMenuItem("Regen -1", "Sub", _NO1, GetCursor(iPlr), nil, nil);
}

func Add()
{
  Local(nil)=(Local(nil) ?? 0)+1;
  if(Equal(Local(nil), 4)) Local(nil)=3;
  if(Equal(Local(nil), nil)) Message("Kein Regen!");
  if(Equal(Local(nil), 1)) Message("Leichter Regen!");
  if(Equal(Local(nil), 2)) Message("Regen!");
  if(Equal(Local(nil), 3)) Message("Starker Regen!");
  return(Check());
}

func Sub()
{
  Local(nil)=(Local(nil) ?? 0)-1;
  if(Equal(Local(nil), -1)) Local(nil)=nil;
  if(Equal(Local(nil), nil)) Message("Kein Regen!");
  if(Equal(Local(nil), 1)) Message("Leichter Regen!");
  if(Equal(Local(nil), 2)) Message("Regen!");
  if(Equal(Local(nil), 3)) Message("Starker Regen!");
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
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  return(Check());
}

func DoIt2()
{
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  return(Check());
}

func DoIt3()
{
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)>0) CastPXS("Water",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  if((GetTemperature() ?? 0)<1) CastPXS("Snow",1,nil,Random(LandscapeWidth()),nil);
  return(Check());
}