#strict

local iMat, iLength, iStrength, iHeight, iTargetStrength, iTargetLength,blitze,mlight,blitzemodifier;

func Precipitation()
{
    if(!blitzemodifier || !Random(370)) // ab und zu aktualsisieren
    {
        if(GetLength(hashSpielRegeln))
            blitzemodifier=HashGet(hashSpielRegeln, "BlitzeModifier");
        else
            blitzemodifier=100;
    }
//am besten noch iwie dafür sorgen, dass bei mehreren Wolken nur eine dafür sorgt, dass die Partikel entfernt werden, da sie sonst garnicht mehr auftauchen.. anderes objekt macht es nun
if(!blitze) blitze= GetScenLightning();
  // Blitze am besten an Blitzeinstellungen orientieren.. mit GetScenLightning()
  if(!Random(35)){ 
  // Log(Format("blitzemodifier2: %v",blitzemodifier));
  if (!((Random(1000)) >= (blitze*blitzemodifier)/100)) { 
 //sollte mit Chance von  blitze zu 1000 dann Blitzen.  ---> bei eingestellten Blitzen von 10/100 und blitzemodifier von 100%, heißt das ~ alle 200 sekunden ein Blitz pro Wolke
      LaunchLightning(RandomX(GetX() + iLength / 3, GetX() + iLength * 2 / 3), GetY()+RandomX(-25,25), RandomX(+10,-10), 20, 20, 10);
      // Log("BLITZ 2");
      var iColor;// Lichtblitze in Wolke
      iColor = RGBa(170,222,253,140);
     if(!Random(2)) CreateParticle("PSpark",RandomX(AbsX(GetX()) + iLength / 3, AbsX(GetX()) + iLength * 2 / 3),AbsY(GetY())+RandomX(-50,30),0,0,RandomX(1000,4000),iColor,this(),0 );
     else CreateParticle("PSpark",RandomX(AbsX(GetX()) + iLength / 3, AbsX(GetX()) + iLength * 2 / 3),AbsY(GetY())+RandomX(-50,30),0,0,RandomX(1000,4000),iColor,this(),1 );
     bliitzliiicht=1; // wird glaube im umweltschutz objekt dann genutzt, um alle von wolken erstellen lichtobjekte zu entfernen.
}}

  
  if(!Random(30)){ // noch eine chance fuer Lichtblitze in Wolke
  if (!((Random(1000)) >= (blitze*blitzemodifier)/100)){ 
  var iColor;
  iColor = RGBa(170,222,253,140);
 if(!Random(2)) CreateParticle("PSpark",RandomX(AbsX(GetX()) + iLength / 3, AbsX(GetX()) + iLength * 2 / 3),AbsY(GetY())+RandomX(-50,30),0,0,RandomX(1000,4000),iColor,this(),0 );
 else CreateParticle("PSpark",RandomX(AbsX(GetX()) + iLength / 3, AbsX(GetX()) + iLength * 2 / 3),AbsY(GetY())+RandomX(-50,30),0,0,RandomX(1000,4000),iColor,this(),1 );
bliitzliiicht=1; // wird glaube im umweltschutz objekt dann genutzt, um alle von wolken erstellen lichtobjekte zu entfernen.
}}

  if (10+Random(50) > iStrength) return(0);
  
  if(FindObject(SNOR) && GetTemperature()<=0) // wenn SchneeEffekt Regel aktiv und es Schnee Temperatur ist, dann kein Schnee Material, sondern nur Partikel fallen lassen.
  {
      var frequency = 10;
      var grav = GetGravity();
      var speed = 25;
      var dir = BoundBy(GetWind(),-1,1);
      PushParticles("Snow",dir*Random(4));
      for(var x; x < frequency; x++) 
      {
        var size = RandomX(10,100);
        var xdir = BoundBy(GetWind(),-speed,speed)+RandomX(-5,5),
            ydir = (size/2*grav)/100;
        var offset = LandscapeHeight()*2/3;
        CreateParticle("Snow",Random(LandscapeWidth()+offset)-offset/2-offset/2*dir,0,xdir,ydir,size,0);
      }
      return();
  }
  
  
  var vx = GetWind(0, 3) / 10, vy = 30;             
  InsertMaterial(GetPrecipitationMat(), RandomX(GetX(), GetX() + iLength) % LandscapeWidth() - GetX(), RandomX(-25,25), vx, vy);
  InsertMaterial(GetPrecipitationMat(), RandomX(GetX() + iLength / 3, GetX() + iLength * 2 / 3) % LandscapeWidth() - GetX(),RandomX(-25,25), vx, vy);
  InsertMaterial(GetPrecipitationMat(), RandomX(GetX() - iLength / 3, GetX() + iLength * 4 / 3) % LandscapeWidth() - GetX(),RandomX(-25,25), vx, vy);
  var rand;
  while(85 < Random(iStrength))// Extreme Regenfälle
  {
    rand=Random(2);
    if(rand==0)
      InsertMaterial(GetPrecipitationMat(), RandomX(GetX(), GetX() + iLength) % LandscapeWidth() - GetX(), RandomX(-25,25), vx, vy);
    if(rand==1)
      InsertMaterial(GetPrecipitationMat(), RandomX(GetX() + iLength / 3, GetX() + iLength * 2 / 3) % LandscapeWidth() - GetX(),RandomX(-25,25), vx, vy);
    if(rand==2)
      InsertMaterial(GetPrecipitationMat(), RandomX(GetX() - iLength / 3, GetX() + iLength * 4 / 3) % LandscapeWidth() - GetX(),RandomX(-25,25), vx, vy);
  }
  
}

func GetPrecipitationMat()
{
  var mat=iMat;
  return mat;
}

func Movement()
{
  if(!IsAntiLag() && !FindObject(WOL_))
  {
    iHeight = SinX(dayhelligkeit,25,LandscapeHeight()/10);
    if (GetX() > LandscapeWidth()-20)
      SetPosition(25,iHeight);
    if (GetX() < 20)
      SetPosition(LandscapeWidth()-25,iHeight);
    if(iHeight!=GetY())
    {
      if(iHeight>GetY())
        SetYDir(10);
      else
        SetYDir(-10);
    }
    else
     SetYDir(0);
  }
  Schedule(Format("SetXDir(%d))",BoundBy(GetWind(0, 3)*75/iStrength,-100,100)),2,1);
  iStrength+=GetSign(iTargetStrength-iStrength)*Random(2);
  iLength  +=GetSign(iTargetLength  -iLength  )*Random(10);
  if(iStrength==iTargetStrength)
   if(!Random(300))
    iTargetStrength=0;
  if(iTargetStrength==0)
   if(iStrength==0)
    {
    CreateObject(FX22,AbsX(Random(LandscapeWidth())),0,-1)->~Activate(iMat, iTargetLength, BoundBy(RandomX(GetScenarioVal("Precipitation", "Weather",1),GetScenarioVal("Precipitation", "Weather",2))+RandomX(-20,20),0,200));
    RemoveObject();
    }
  SetShape(-iLength/2,-25,iLength,50);
  /*if(iLength  ==iTargetLength  &&(!Random(300)))
   iTargetLength  =0;*/
}

func Activate(inMat, inLength, inStrength, iBlitze)
{
  blitze = iBlitze;
  iMat = inMat; iLength = inLength/2;
  iHeight = SinX(dayhelligkeit,25,LandscapeHeight()/10);
  if(IsAntiLag() || FindObject(WOL_)) iHeight = -1;
  SetPosition(GetX(),iHeight);
  iTargetStrength = inStrength+Random(RandomX(1,3)*inStrength)+5;
  iTargetLength   = inLength+RandomX(-inLength/5,+inLength/5);
  iStrength = 1;
  // if(FrameCounter()<10)  // deaktiviert, weil iStrength sonst immer 1 wäre.. keine ahnung warum dies so gemacht wurde
    iStrength=iTargetStrength;
  CreateVisisble();
  SetAction("Process");
  return(1);                                      
}

func CreateVisisble()
{
  ClearScheduleCall(0,"CreateVisisble");
  if(IsAntiLag() || FindObject(WOL_)){ iHeight=-1; SetPosition(GetX(),-1); return 0;}
  for(var i = iLength; i >= 0; i-=RandomX(10,40))
  {
    if(!Random(79))
      CreateParticle("Cloud",i,RandomX(-iLength/7,iLength/7),0,0,RandomX(1500,2750),HSL(0,0,BoundBy(140-iStrength/2,30,140)+Random(70)),this());
  }
  ScheduleCall(0,"CreateVisisble",70);
}

private func GetRandomSign()
{
  var r=Random(1);
  if(r)
   return -1;
  return 1;
}