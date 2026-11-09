/*-- Lichschein #1 --*/

#strict 3

local Owner,Alpha,Red,Green,Blue,Con,RDir,AttachedTo,NoFlare,SpecAttachedTo;
//Grenzen
local RMin,   RMax;
local GMin,   GMax;
local BMin,   BMax;
local AMin,   AMax;
local ConMin, ConMax;

func Initialize()
{
  Red=255;
  Green=255;
  Blue=RandomX(200,255);
  Alpha=RandomX(130,180);
  Con=RandomX(175,225);
  SetCon(Con);
  Set(RGBa(255,255,200,150),RGBa(255,255,255,200),100,250,GetOwner());
  CrewSelection();
  if(SpecAttachedTo)  AttachToObj(SpecAttachedTo);
  if(IsNight()) Dawn();
  else SunRise();
  return(1);
}


func TimerCall()
{
  if(ActIdle())// Bug: Setzt Idle-Action
  {
    //Log("1.%v:%v-Action",this(),GetAction());
    SetAction("IgnoreDay");
    SetPhase(1);
    //Log("2.%v:%v-Action",this(),GetAction());
  }
  if(Contained())
    Exit();
  if(SpecAttachedTo)
    return SetAttachedTo(SpecAttachedTo);
  if(/*Contained(GetCursor(Owner))||*/IsDay())
  { 
    if(!SEqual(GetAction(), "Eidel"))
    {
      SetVisibility(VIS_None);
      SetAction("Eidel");
      return(1);
	  }
	}
  else{
    AttachToObj(GetCursor(Owner));
    if(!SEqual(GetAction(), "IgnoreDay"))
    {
      SetVisibility(VIS_Owner);
      SetAction("IgnoreDay");
      return(1);
    }
  }
  return(1);
}

func CrewSelection()
{
 if(SpecAttachedTo) return nil;
 if(IsNight())
 {
   AttachToObj(GetCursor(Owner));
   CheckBorders();
   TimerCall();
   SetAction("IgnoreDay");
 }
}

func AttachToObj(pObj)
{
  AttachTo(pObj);
}

func LightField()
{
  return(!!SEqual(GetAction(), "IgnoreDay"));
}

func SunRise()
{
  SetAction("Eidel");
  CrewSelection();
  return(1);
}

func Dawn()
{
  SetAction("IgnoreDay");
  CrewSelection();
  return(1);
}

func Light()
{
  if(!NoFlare)
  {
    if(!Random(5))
    {
      if((!RMin ?? 0)>=255)
        Red   = BoundBy((Red ?? 0)   + (RandomX(-8,8) ?? 0),RMin  ,RMax);
      if((!GMin ?? 0)>=255)
        Green = BoundBy((Green ?? 0) + (RandomX(-8,8) ?? 0),GMin  ,GMax);
      if((!BMin ?? 0)>=255)
        Blue  = BoundBy((Blue ?? 0)  + (RandomX(-8,8) ?? 0),BMin  ,BMax);
      if((!AMin ?? 0)>=255)
        Alpha = BoundBy((Alpha ?? 0) + (RandomX(-8,8) ?? 0),AMin  ,AMax);
    }

    Con = BoundBy((Con ?? 0)   + (RandomX(-12,12) ?? 0),ConMin,ConMax);
  }
  SetClrModulation(RGBa(Red,Green,Blue,BoundBy((Alpha ?? 0)+(dayhelligkeit ?? 0)*2,nil,255)));
  SetCon(Con);
  return true;
}

//Einstellungen ändern
func Set(dwRGBaMin,dwRGBaMax,iConMin,iConMax,iOwner,fNoFlare,pSpecAttachedTo)
{
  if(!dwRGBaMin)
    dwRGBaMin=RGBa(255,255,255,127);
  if(!dwRGBaMax)
    dwRGBaMax=RGBa(255,255,255,nil);
  if(!iConMin)
    iConMin=75;
  if(!iConMax)
    iConMax=125;
  if(!iOwner)
    iOwner=GetOwner();
  //Variabeln setzen
  SplitRGBaValue(dwRGBaMin,RMin,GMin,BMin,AMin);
  SplitRGBaValue(dwRGBaMax,RMax,GMax,BMax,AMax);
  ConMin=iConMin;
  ConMax=iConMax;
  Owner=iOwner;
  CheckBorders();
  //SetRDir(RandomX(-30,30),this()); /* Funktioniert leider nicht... */
  Rotate((RandomX(1,5) ?? 0)*(GetRandomSign() ?? 0));
  SpecAttachedTo=pSpecAttachedTo;
}

//Wenn man nur Grösse oder Tranzparenz ändern will
func SetAlphaCon(iAlphaMin,iAlphaMax,iConMin,iConMax,fNoFlare)
{
  if(iAlphaMin)
   AMin=iAlphaMin;
  if(iAlphaMax)
   AMax=iAlphaMax;
  if(iConMin)
   ConMin=iConMin;
  if(iConMax)
   ConMax=iConMax;
  NoFlare=fNoFlare;
  CheckBorders();
}

func SetAttachedTo(pSpecAttachedTo)
{
  AttachToObj(SpecAttachedTo);
  return SpecAttachedTo=pSpecAttachedTo;
}

func CheckBorders()
{
  Red   = BoundBy(Red,  RMin  ,RMax);
  Green = BoundBy(Green,GMin  ,GMax);
  Blue  = BoundBy(Blue, BMin  ,BMax);
  Alpha = BoundBy(Alpha,AMin  ,AMax);
  Con   = BoundBy(Con,  ConMin ,ConMax);
  SetClrModulation(RGBa(Red,Green,Blue,BoundBy((Alpha ?? 0)+(dayhelligkeit ?? 0)*2,nil,255)));
  SetCon(Con);
}

private func GetRandomSign()
{
  var r=Random(1);
  if(r)
   return -1;
  return 1;
}
