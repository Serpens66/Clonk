/*-- Regen --*/

#strict

protected func Activate(iPlr)
{
  CreateMenu(FXL1,GetCursor(iPlr), this());
  AddMenuItem("Regen +1", "Add", FXP1, GetCursor(iPlr), 0, iPlr);
  AddMenuItem("Regen -1", "Sub", _NO1, GetCursor(iPlr), 0, 0);
}

func Add()
{
  Local(0)=Local(0)+1;
  if(Local(0)==4) Local(0)=3;
  if(Local(0)==0) Message("Kein Regen!");
  if(Local(0)==1) Message("Leichter Regen!");
  if(Local(0)==2) Message("Regen!");
  if(Local(0)==3) Message("Starker Regen!");
  return(Check());
}

func Sub()
{
  Local(0)=Local(0)-1;
  if(Local(0)==-1) Local(0)=0;
  if(Local(0)==0) Message("Kein Regen!");
  if(Local(0)==1) Message("Leichter Regen!");
  if(Local(0)==2) Message("Regen!");
  if(Local(0)==3) Message("Starker Regen!");
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
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  return(Check());
}

func DoIt2()
{
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  return(Check());
}

func DoIt3()
{
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()>0) CastPXS("Water",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  if(GetTemperature()<1) CastPXS("Snow",1,0,Random(LandscapeWidth()),0);
  return(Check());
}