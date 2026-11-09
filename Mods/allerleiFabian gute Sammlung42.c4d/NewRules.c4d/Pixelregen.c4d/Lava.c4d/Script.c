/*-- Lavaregen --*/

#strict

protected func Activate(iPlr)
{
  CreateMenu(FXL1,GetCursor(iPlr), this());
  AddMenuItem("Lavaregen +1", "Add", LAVA, GetCursor(iPlr), 0, iPlr);
  AddMenuItem("Lavaregen -1", "Sub", _NO1, GetCursor(iPlr), 0, 0);
}

func Add()
{
  Local(0)=Local(0)+1;
  if(Local(0)==4) Local(0)=3;
  if(Local(0)==0) Message("Kein Lavaregen!");
  if(Local(0)==1) Message("Leichter Lavaregen!");
  if(Local(0)==2) Message("Lavaregen!");
  if(Local(0)==3) Message("Starker Lavaregen!");
  return(Check());
}

func Sub()
{
  Local(0)=Local(0)-1;
  if(Local(0)==-1) Local(0)=0;
  if(Local(0)==0) Message("Kein Lavaregen!");
  if(Local(0)==1) Message("Leichter Lavaregen!");
  if(Local(0)==2) Message("Lavaregen!");
  if(Local(0)==3) Message("Starker Lavaregen!");
  return(Check());
}

func Check()
{
  if(Local(0)==0) return(1);
  if(Local(0)==1) return(SetAction("Do1"));
  if(Local(0)==2) return(SetAction("Do2"));
  if(Local(0)==3) return(SetAction("Do3"));
}

func DoIt1()
{
  SetVar(0,RandomX(0,19));
  if(Equal(Var(0),2)) SetVar(0,1);
  if(Equal(Var(0),3)) SetVar(0,1);
  if(Equal(Var(0),4)) SetVar(0,1);
  if(Equal(Var(0),5)) SetVar(0,1);
  if(Equal(Var(0),6)) SetVar(0,1);
  if(Equal(Var(0),7)) SetVar(0,1);
  if(Equal(Var(0),8)) SetVar(0,1);
  if(Equal(Var(0),9)) SetVar(0,1);
  if(Equal(Var(0),10)) SetVar(0,1);
  if(Equal(Var(0),11)) SetVar(0,1);
  if(Equal(Var(0),12)) SetVar(0,1);
  if(Equal(Var(0),13)) SetVar(0,1);
  if(Equal(Var(0),14)) SetVar(0,1);
  if(Equal(Var(0),15)) SetVar(0,1);
  if(Equal(Var(0),16)) SetVar(0,1);
  if(Equal(Var(0),17)) SetVar(0,1);
  if(Equal(Var(0),18)) SetVar(0,1);
  if(Equal(Var(0),19)) SetVar(0,1);
  if(Equal(Var(0),0)) {
   CastObjects(LAVA,1,100,Random(LandscapeWidth()),0);
   return(SetAction("Wait"));
  }
  if(Equal(Var(0),1)) CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
  return(Check());
}

func DoIt2()
{
  SetVar(0,RandomX(0,9));
  if(Equal(Var(0),2)) SetVar(0,1);
  if(Equal(Var(0),3)) SetVar(0,1);
  if(Equal(Var(0),4)) SetVar(0,1);
  if(Equal(Var(0),5)) SetVar(0,1);
  if(Equal(Var(0),6)) SetVar(0,1);
  if(Equal(Var(0),7)) SetVar(0,1);
  if(Equal(Var(0),8)) SetVar(0,1);
  if(Equal(Var(0),9)) SetVar(0,1);
  if(Equal(Var(0),0)) {
   CastObjects(LAVA,1,100,Random(LandscapeWidth()),0);
  }
  if(Equal(Var(0),1)) {
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
  }
  return(Check());
}

func DoIt3()
{
  SetVar(0,RandomX(0,4));
  if(Equal(Var(0),2)) SetVar(0,1);
  if(Equal(Var(0),3)) SetVar(0,1);
  if(Equal(Var(0),4)) SetVar(0,1);
  if(Equal(Var(0),0)) {
   CastObjects(LAVA,1,100,Random(LandscapeWidth()),0);
  }
  if(Equal(Var(0),1)) {
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
   CastPXS("Lava",1,0,Random(LandscapeWidth()),0);
  }
  return(Check());
}