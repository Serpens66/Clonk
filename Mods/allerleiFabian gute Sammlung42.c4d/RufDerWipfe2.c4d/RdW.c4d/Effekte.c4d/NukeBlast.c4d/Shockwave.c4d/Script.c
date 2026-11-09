/*-- Schockwelle --*/

#strict

local Size,TargetSize,Speed,iX,iY;

public func Set(iTargetSize,iSpeed,iColor,iColor2)
{
  TargetSize=iTargetSize;
  if(iSpeed)
   Speed=iSpeed;
  else
   Speed=iTargetSize/20;
  Size=1;
  if(iColor2)
  {
   SetGraphics("",0,GetID(),GFX_Overlay,GFXOV_MODE_ExtraGraphics);
   SetLayerTransform(GFX_Overlay,0,105,105,0,0,0);
   SetClrModulation(iColor2,0,GFX_Overlay);
  }
  if(iColor)
   SetClrModulation(iColor,0);
  iX=GetX();
  iY=GetY();
}

public func Shock()
{
  Size+=Speed;
  if(GetPercentOfSize()>50)
   SetObjAlpha(((GetPercentOfSize()-50)*5)+5,this());
  SetCon(Size);
  SetPosition(iX,iY);
  // Extreme Zerstörung

  // Ein Inferno
  for(var obj in FindObjects(Find_Distance(Size),Find_Not(Find_Distance(Size-Speed*2)),Find_OCF(OCF_Inflammable)))
   if(GetDefContactIncinerate(GetID(obj)))
    if(!Random(GetDefContactIncinerate(GetID(obj))+1))
     Incinerate();

  // Zerstörung
  for(var i,x,y; i <= 360; i+=RandomX(10,20))
  {
   if(!Random(5))/*WUSH*/
   {
    BlastObjects(GetX()+Sin(i+RandomX(-5,5),Size+RandomX(-Speed*2,0)),
                GetY()-Cos(i+RandomX(-5,5),Size+RandomX(-Speed*2,0)),
                RandomX(25,70));//Stärke: Flint bis Terra-Flint
   }
   if(!Random(100))/*RÜTTEL*/
   {
    LaunchEarthquake(GetX()+Sin(i+RandomX(-5,5),Size+RandomX(-Speed*2,0)),
                     GetY()-Cos(i+RandomX(-5,5),Size+RandomX(-Speed*2,0)));
   }
   if(!Random(15))/*BOOM*/
    if(!GBackSolid(Sin(i,Size),-Cos(i,Size)))
    {
    CreateObject(ROCK,Sin(i+RandomX(-5,5),Size+RandomX(-Speed*2,0))
                    ,-Cos(i+RandomX(-5,5),Size+RandomX(-Speed*2,0)))->Explode(RandomX(25,70));
    }
   if(!Random(25))/*BRENN*/
    if(!GBackSolid(Sin(i,Size),-Cos(i,Size)))
     {
     CreateObject(FLAM,Sin(i+RandomX(-5,5),Size+RandomX(-Speed*2,0))
                    ,-Cos(i+RandomX(-5,5),Size+RandomX(-Speed*2,0)));
     }
   if(!Random(5))/*PARTIKEL*/
    if(!GBackSolid(Sin(i,Size),-Cos(i,Size)))
     CreateParticle("PSpark",Sin(i+RandomX(-5,5),Size+RandomX(-Speed*2,0)),-Cos(i+RandomX(-5,5),Size+RandomX(-Speed*2,0)),0,0,RandomX(50,100),RGBa(0,255,0,Random(255)));
  }
  ScreenGamma();
  if(Size >= TargetSize)
   Remove();
}

public func Incineration()
{
  Extinguish();
}

protected func Remove()
{
  ResetGamma(4);
  RemoveObject();
}

private func ScreenGamma()
{
  // kurze grüne Bildschirmfärbung
  SetGamma(RGB(  0, 100-GetPercentOfSize(),  0),
           RGB(128, 228-GetPercentOfSize(),128),
           RGB(255, 255,                   255), 4);
  // Lichtblitze
  CreateParticle("PSpark",0,0,0,0,RandomX(1000,1500),RGBa(180,255,127,Random(55)));
}

private func GetPercentOfSize()
{
  return BoundBy(100*Size/TargetSize,0,100);
}
