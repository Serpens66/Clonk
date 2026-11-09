/*-- Schockwelle --*/

#strict 3

local Size,TargetSize,Speed,iX,iY;

public func Set(iTargetSize,iSpeed,iColor,iColor2)
{
  TargetSize=iTargetSize;
  if(iSpeed)
   Speed=iSpeed;
  else
   Speed=(iTargetSize ?? 0)/20;
  Size=1;
  if(iColor2)
  {
   SetGraphics("",nil,GetID(),GFX_Overlay,GFXOV_MODE_ExtraGraphics);
   SetLayerTransform(GFX_Overlay,nil,105,105,nil,nil,nil);
   SetClrModulation(iColor2,nil,GFX_Overlay);
  }
  if(iColor)
   SetClrModulation(iColor,nil);
  iX=GetX();
  iY=GetY();
}

public func Shock()
{
  S42MigrationAssign(Size, "+=", (Speed ?? 0));
  if((GetPercentOfSize() ?? 0)>50)
   SetObjAlpha((((GetPercentOfSize() ?? 0)-50)*5)+5,this());
  SetCon(Size);
  SetPosition(iX,iY);
  // Extreme Zerstörung

  // Ein Inferno
  for(var obj in FindObjects(Find_Distance(Size),Find_Not(Find_Distance((Size ?? 0)-(Speed ?? 0)*2)),Find_OCF(OCF_Inflammable)))
   if(GetDefContactIncinerate(GetID(obj)))
    if(!Random((GetDefContactIncinerate(GetID(obj)) ?? 0)+1))
     Incinerate();

  // Zerstörung
  for(var i,x,y; (i ?? 0) <= 360; S42MigrationAssign(i, "+=", (RandomX(10,20) ?? 0)))
  {
   if(!Random(5))/*WUSH*/
   {
    BlastObjects((GetX() ?? 0)+(Sin((i ?? 0)+(RandomX(-5,5) ?? 0),(Size ?? 0)+(RandomX(-(Speed ?? 0)*2,nil) ?? 0)) ?? 0),
                (GetY() ?? 0)-(Cos((i ?? 0)+(RandomX(-5,5) ?? 0),(Size ?? 0)+(RandomX(-(Speed ?? 0)*2,nil) ?? 0)) ?? 0),
                RandomX(25,70));//Stärke: Flint bis Terra-Flint
   }
   if(!Random(100))/*RÜTTEL*/
   {
    LaunchEarthquake((GetX() ?? 0)+(Sin((i ?? 0)+(RandomX(-5,5) ?? 0),(Size ?? 0)+(RandomX(-(Speed ?? 0)*2,nil) ?? 0)) ?? 0),
                     (GetY() ?? 0)-(Cos((i ?? 0)+(RandomX(-5,5) ?? 0),(Size ?? 0)+(RandomX(-(Speed ?? 0)*2,nil) ?? 0)) ?? 0));
   }
   if(!Random(15))/*BOOM*/
    if(!GBackSolid(Sin(i,Size),-(Cos(i,Size) ?? 0)))
    {
    CreateObject(ROCK,Sin((i ?? 0)+(RandomX(-5,5) ?? 0),(Size ?? 0)+(RandomX(-(Speed ?? 0)*2,nil) ?? 0))
                    ,-(Cos((i ?? 0)+(RandomX(-5,5) ?? 0),(Size ?? 0)+(RandomX(-(Speed ?? 0)*2,nil) ?? 0)) ?? 0))->Explode(RandomX(25,70));
    }
   if(!Random(25))/*BRENN*/
    if(!GBackSolid(Sin(i,Size),-(Cos(i,Size) ?? 0)))
     {
     CreateObject(FLAM,Sin((i ?? 0)+(RandomX(-5,5) ?? 0),(Size ?? 0)+(RandomX(-(Speed ?? 0)*2,nil) ?? 0))
                    ,-(Cos((i ?? 0)+(RandomX(-5,5) ?? 0),(Size ?? 0)+(RandomX(-(Speed ?? 0)*2,nil) ?? 0)) ?? 0));
     }
   if(!Random(5))/*PARTIKEL*/
    if(!GBackSolid(Sin(i,Size),-(Cos(i,Size) ?? 0)))
     CreateParticle("PSpark",Sin((i ?? 0)+(RandomX(-5,5) ?? 0),(Size ?? 0)+(RandomX(-(Speed ?? 0)*2,nil) ?? 0)),-(Cos((i ?? 0)+(RandomX(-5,5) ?? 0),(Size ?? 0)+(RandomX(-(Speed ?? 0)*2,nil) ?? 0)) ?? 0),nil,nil,RandomX(50,100),RGBa(nil,255,nil,Random(255)));
  }
  ScreenGamma();
  if((Size ?? 0) >= (TargetSize ?? 0))
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
  SetGamma(RGB(  nil, 100-(GetPercentOfSize() ?? 0),  nil),
           RGB(128, 228-(GetPercentOfSize() ?? 0),128),
           RGB(255, 255,                   255), 4);
  // Lichtblitze
  CreateParticle("PSpark",nil,nil,nil,nil,RandomX(1000,1500),RGBa(180,255,127,Random(55)));
}

private func GetPercentOfSize()
{
  return BoundBy(100*(Size ?? 0)/(TargetSize ?? 0),nil,100);
}
