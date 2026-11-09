/*-- Lavaregen --*/

#strict 3

protected func Activate(iPlr)
{
  CreateMenu(FXL1,GetCursor(iPlr), this());
  AddMenuItem("Lavaregen +1", "Add", LAVA, GetCursor(iPlr), nil, iPlr);
  AddMenuItem("Lavaregen -1", "Sub", _NO1, GetCursor(iPlr), nil, nil);
}

func Add()
{
  Local(nil)=(Local(nil) ?? 0)+1;
  if(Equal(Local(nil), 4)) Local(nil)=3;
  if(Equal(Local(nil), nil)) Message("Kein Lavaregen!");
  if(Equal(Local(nil), 1)) Message("Leichter Lavaregen!");
  if(Equal(Local(nil), 2)) Message("Lavaregen!");
  if(Equal(Local(nil), 3)) Message("Starker Lavaregen!");
  return(Check());
}

func Sub()
{
  Local(nil)=(Local(nil) ?? 0)-1;
  if(Equal(Local(nil), -1)) Local(nil)=nil;
  if(Equal(Local(nil), nil)) Message("Kein Lavaregen!");
  if(Equal(Local(nil), 1)) Message("Leichter Lavaregen!");
  if(Equal(Local(nil), 2)) Message("Lavaregen!");
  if(Equal(Local(nil), 3)) Message("Starker Lavaregen!");
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
  SetVar(nil,RandomX(nil,19));
  if(Equal(Var(nil),2)) SetVar(nil,1);
  if(Equal(Var(nil),3)) SetVar(nil,1);
  if(Equal(Var(nil),4)) SetVar(nil,1);
  if(Equal(Var(nil),5)) SetVar(nil,1);
  if(Equal(Var(nil),6)) SetVar(nil,1);
  if(Equal(Var(nil),7)) SetVar(nil,1);
  if(Equal(Var(nil),8)) SetVar(nil,1);
  if(Equal(Var(nil),9)) SetVar(nil,1);
  if(Equal(Var(nil),10)) SetVar(nil,1);
  if(Equal(Var(nil),11)) SetVar(nil,1);
  if(Equal(Var(nil),12)) SetVar(nil,1);
  if(Equal(Var(nil),13)) SetVar(nil,1);
  if(Equal(Var(nil),14)) SetVar(nil,1);
  if(Equal(Var(nil),15)) SetVar(nil,1);
  if(Equal(Var(nil),16)) SetVar(nil,1);
  if(Equal(Var(nil),17)) SetVar(nil,1);
  if(Equal(Var(nil),18)) SetVar(nil,1);
  if(Equal(Var(nil),19)) SetVar(nil,1);
  if(Equal(Var(nil),nil)) {
   CastObjects(LAVA,1,100,Random(LandscapeWidth()),nil);
   return(SetAction("Wait"));
  }
  if(Equal(Var(nil),1)) CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
  return(Check());
}

func DoIt2()
{
  SetVar(nil,RandomX(nil,9));
  if(Equal(Var(nil),2)) SetVar(nil,1);
  if(Equal(Var(nil),3)) SetVar(nil,1);
  if(Equal(Var(nil),4)) SetVar(nil,1);
  if(Equal(Var(nil),5)) SetVar(nil,1);
  if(Equal(Var(nil),6)) SetVar(nil,1);
  if(Equal(Var(nil),7)) SetVar(nil,1);
  if(Equal(Var(nil),8)) SetVar(nil,1);
  if(Equal(Var(nil),9)) SetVar(nil,1);
  if(Equal(Var(nil),nil)) {
   CastObjects(LAVA,1,100,Random(LandscapeWidth()),nil);
  }
  if(Equal(Var(nil),1)) {
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
  }
  return(Check());
}

func DoIt3()
{
  SetVar(nil,RandomX(nil,4));
  if(Equal(Var(nil),2)) SetVar(nil,1);
  if(Equal(Var(nil),3)) SetVar(nil,1);
  if(Equal(Var(nil),4)) SetVar(nil,1);
  if(Equal(Var(nil),nil)) {
   CastObjects(LAVA,1,100,Random(LandscapeWidth()),nil);
  }
  if(Equal(Var(nil),1)) {
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
   CastPXS("Lava",1,nil,Random(LandscapeWidth()),nil);
  }
  return(Check());
}