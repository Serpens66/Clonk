#strict 3

local iMat, iLength, iStrength, iHeight, iTargetStrength, iTargetLength,blitze,blitzemodifier;

func Precipitation() // wird alle 2 Frames aufgerufen. 
{
    if(Or(!blitzemodifier, !Random(370))) // ab und zu aktualsisieren
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
  // Log(Format("blitzemodifier: %v",blitzemodifier));
  if (!(((Random(1000)) ?? 0) >= ((blitze ?? 0)*(blitzemodifier ?? 0))/100)) {
 //sollte mit Chance von  blitze zu 1000 dann Blitzen.  ---> bei eingestellten Blitzen von 10/100 und blitzemodifier von 100%, heißt das ~ alle 200 sekunden ein Blitz pro Wolke
      LaunchLightning(RandomX((GetX() ?? 0) + (iLength ?? 0) / 3, (GetX() ?? 0) + (iLength ?? 0) * 2 / 3), (GetY() ?? 0)+(RandomX(-25,25) ?? 0), RandomX(+10,-10), 20, 20, 10);
      // Log("BLITZ");
      var iColor;// Lichtblitze in Wolke
      iColor = RGBa(170,222,253,140);
     if(!Random(2)) CreateParticle("PSpark",RandomX((AbsX(GetX()) ?? 0) + (iLength ?? 0) / 3, (AbsX(GetX()) ?? 0) + (iLength ?? 0) * 2 / 3),(AbsY(GetY()) ?? 0)+(RandomX(-50,30) ?? 0),nil,nil,RandomX(1000,4000),iColor,this(),nil );
     else CreateParticle("PSpark",RandomX((AbsX(GetX()) ?? 0) + (iLength ?? 0) / 3, (AbsX(GetX()) ?? 0) + (iLength ?? 0) * 2 / 3),(AbsY(GetY()) ?? 0)+(RandomX(-50,30) ?? 0),nil,nil,RandomX(1000,4000),iColor,this(),1 );
     bliitzliiicht=1; // wird glaube im umweltschutz objekt dann genutzt, um alle von wolken erstellen lichtobjekte zu entfernen.
}}

  
  if(!Random(30)){ // noch eine chance fuer Lichtblitze in Wolke
  if (!(((Random(1000)) ?? 0) >= ((blitze ?? 0)*(blitzemodifier ?? 0))/100)){
  var iColor;
  iColor = RGBa(170,222,253,140);
 if(!Random(2)) CreateParticle("PSpark",RandomX((AbsX(GetX()) ?? 0) + (iLength ?? 0) / 3, (AbsX(GetX()) ?? 0) + (iLength ?? 0) * 2 / 3),(AbsY(GetY()) ?? 0)+(RandomX(-50,30) ?? 0),nil,nil,RandomX(1000,4000),iColor,this(),nil );
 else CreateParticle("PSpark",RandomX((AbsX(GetX()) ?? 0) + (iLength ?? 0) / 3, (AbsX(GetX()) ?? 0) + (iLength ?? 0) * 2 / 3),(AbsY(GetY()) ?? 0)+(RandomX(-50,30) ?? 0),nil,nil,RandomX(1000,4000),iColor,this(),1 );
bliitzliiicht=1; // wird glaube im umweltschutz objekt dann genutzt, um alle von wolken erstellen lichtobjekte zu entfernen.
}}

  if (10+(Random(50) ?? 0) > (iStrength ?? 0)) return(nil);

  if(And(FindObject(SNOR), (GetTemperature() ?? 0)<=0)) // wenn SchneeEffekt Regel aktiv und es Schnee Temperatur ist, dann kein Schnee Material, sondern nur Partikel fallen lassen.
  {
      var frequency = 10;
      var grav = GetGravity();
      var speed = 25;
      var dir = BoundBy(GetWind(),-1,1);
      PushParticles("Snow",(dir ?? 0)*(Random(4) ?? 0));
      for(var x; (x ?? 0) < (frequency ?? 0); S42MigrationStep(x, 1, true))
      {
        var size = RandomX(10,100);
        var xdir = (BoundBy(GetWind(),-(speed ?? 0),speed) ?? 0)+(RandomX(-5,5) ?? 0),
            ydir = ((size ?? 0)/2*(grav ?? 0))/100;
        var offset = (LandscapeHeight() ?? 0)*2/3;
        CreateParticle("Snow",(Random((LandscapeWidth() ?? 0)+(offset ?? 0)) ?? 0)-(offset ?? 0)/2-(offset ?? 0)/2*(dir ?? 0),nil,xdir,ydir,size,nil);
      }
      return;
  }
      
  
  var vx = (GetWind(nil, 3) ?? 0) / 10, vy = 30;
  InsertMaterial(GetPrecipitationMat(), (RandomX(GetX(), (GetX() ?? 0) + (iLength ?? 0)) ?? 0) % (LandscapeWidth() ?? 0) - (GetX() ?? 0), RandomX(-25,25), vx, vy);
  InsertMaterial(GetPrecipitationMat(), (RandomX((GetX() ?? 0) + (iLength ?? 0) / 3, (GetX() ?? 0) + (iLength ?? 0) * 2 / 3) ?? 0) % (LandscapeWidth() ?? 0) - (GetX() ?? 0),RandomX(-25,25), vx, vy);
  InsertMaterial(GetPrecipitationMat(), (RandomX((GetX() ?? 0) - (iLength ?? 0) / 3, (GetX() ?? 0) + (iLength ?? 0) * 4 / 3) ?? 0) % (LandscapeWidth() ?? 0) - (GetX() ?? 0),RandomX(-25,25), vx, vy);
  var rand;
  while(85 < (Random(iStrength) ?? 0))// Extreme Regenfälle
  {
    rand=Random(2);
    if(Equal(rand, nil))
      InsertMaterial(GetPrecipitationMat(), (RandomX(GetX(), (GetX() ?? 0) + (iLength ?? 0)) ?? 0) % (LandscapeWidth() ?? 0) - (GetX() ?? 0), RandomX(-25,25), vx, vy);
    if(Equal(rand, 1))
      InsertMaterial(GetPrecipitationMat(), (RandomX((GetX() ?? 0) + (iLength ?? 0) / 3, (GetX() ?? 0) + (iLength ?? 0) * 2 / 3) ?? 0) % (LandscapeWidth() ?? 0) - (GetX() ?? 0),RandomX(-25,25), vx, vy);
    if(Equal(rand, 2))
      InsertMaterial(GetPrecipitationMat(), (RandomX((GetX() ?? 0) - (iLength ?? 0) / 3, (GetX() ?? 0) + (iLength ?? 0) * 4 / 3) ?? 0) % (LandscapeWidth() ?? 0) - (GetX() ?? 0),RandomX(-25,25), vx, vy);
  }

}

func GetPrecipitationMat()
{
  var mat=iMat;
  if(Equal(iMat, Material("Water")))
  {
   if((GetTemperature() ?? 0)<(RandomX(-4,4) ?? 0))
   {
    mat=Material("Ice");
    if(Random(2))
     mat=Material("Snow");
    if((GetTemperature() ?? 0)<(RandomX(-5,-8) ?? 0))
     mat=Material("Snow");
   }
   //if(ProzentVon(globalnature,globalpollution)>RandomX(50,70))
    //if(Random(2))
     //mat=Material("Acid");
  }
  return mat;
}

func Movement()
{
  if(And(!IsAntiLag(), !FindObject(WOL_)))
  {
    iHeight = SinX(dayhelligkeit,25,(LandscapeHeight() ?? 0)/10);
    if ((GetX() ?? 0) > (LandscapeWidth() ?? 0)-20)
      SetPosition(25,iHeight);
    if ((GetX() ?? 0) < 20)
      SetPosition((LandscapeWidth() ?? 0)-25,iHeight);
    if(!Equal(iHeight, GetY()))
    {
      if((iHeight ?? 0)>(GetY() ?? 0))
        SetYDir(10);
      else
        SetYDir(-10);
    }
    else
     SetYDir(nil);
  }
  Schedule(Format("SetXDir(%d))",BoundBy((GetWind(nil, 3) ?? 0)*75/(iStrength ?? 0),-100,100)),2,1);
  S42MigrationAssign(iStrength, "+=", (GetSign((iTargetStrength ?? 0)-(iStrength ?? 0)) ?? 0)*(Random(2) ?? 0));
  S42MigrationAssign(iLength, "+=", (GetSign((iTargetLength ?? 0)  -(iLength ?? 0)  ) ?? 0)*(Random(10) ?? 0));
  if(Equal(iStrength, iTargetStrength))
   if(!Random(300))
    iTargetStrength=nil;
  if(Equal(iTargetStrength, nil))
   if(Equal(iStrength, nil))
    {
    CreateObject(FXP1,AbsX(Random(LandscapeWidth())),nil,-1)->~Activate(iMat, iTargetLength, BoundBy((RandomX(GetScenarioVal("Precipitation", "Weather",1),GetScenarioVal("Precipitation", "Weather",2)) ?? 0)+(RandomX(-20,20) ?? 0),nil,200));
    RemoveObject();
    }
  SetShape(-(iLength ?? 0)/2,-25,iLength,50);
  /*if(iLength  ==iTargetLength  &&(!Random(300)))
   iTargetLength  =0;*/
}

func Activate(inMat, inLength, inStrength)
{
  iMat = inMat; iLength = (inLength ?? 0)/2;
  iHeight = SinX(dayhelligkeit,25,(LandscapeHeight() ?? 0)/10);
  if(Or(IsAntiLag(), FindObject(WOL_))) iHeight = -1;
  SetPosition(GetX(),iHeight);
  iTargetStrength = (inStrength ?? 0)+(Random((RandomX(1,3) ?? 0)*(inStrength ?? 0)) ?? 0)+5;
  iTargetLength   = (inLength ?? 0)+(RandomX(-(inLength ?? 0)/5,(+(inLength ?? 0) ?? 0)/5) ?? 0);
  iStrength = 1;
  if((FrameCounter() ?? 0)<10)
    iStrength=iTargetStrength;
  CreateVisisble();
  SetAction("Process");
  return(1);                                      
}

func CreateVisisble()
{
  ClearScheduleCall(nil,"CreateVisisble");
  if(Or(IsAntiLag(), FindObject(WOL_))){ iHeight=-1; SetPosition(GetX(),-1); return nil;}
  for(var i = iLength; (i ?? 0) >= 0; S42MigrationAssign(i, "-=", (RandomX(10,40) ?? 0)))
  {
    if(!Random(79))
      CreateParticle("Cloud",i,RandomX(-(iLength ?? 0)/7,(iLength ?? 0)/7),nil,nil,RandomX(1500,2750),HSL(nil,nil,(BoundBy(140-(iStrength ?? 0)/2,30,140) ?? 0)+(Random(70) ?? 0)),this());
  }
  ScheduleCall(nil,"CreateVisisble",70);
}

private func GetRandomSign()
{
  var r=Random(1);
  if(r)
   return -1;
  return 1;
}