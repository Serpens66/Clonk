/*-- Radioaktivität --*/
#strict

local iStrength;

public func Radiate()
{
  var object;
  iStrength--;
  if(iStrength<30) return(RemoveObject());
  for(var pVictim in FindObjects(Find_Distance(iStrength),Find_OCF(OCF_Alive)))
    PoorVictim(pVictim);
  if(!Random(100) && iStrength>800)// Umweltpestwolken
  {
    var iAngle=Random(360);
    var iX=Sin(iAngle,Random(iStrength/2));
    var iY=-Cos(iAngle,Random(iStrength/2));
    CreateParticle("Cloud",iX,iY,RandomX(-3,3),RandomX(-3,3),RandomX(300,900),RGB(Random(127),255,Random(127)));
  }
  return(1);
}

public func GetStrength(){ return(iStrength);}

private func PoorVictim(pVictim)
{
  var Value=Prozent(iStrength*5/3,ProzentVon(ObjectDistance(pVictim),iStrength));
  if(Value>=iStrength*4/3)
    Value=iStrength*4/3;
  // Objektpezifische Verstrahlung
  if(pVictim->~Irradiation(Value,this()))
    return 1;
  if(pVictim->~IsProtected()) Value/=4;
  if(!PathFree(GetX(),GetY(),GetX(pVictim),GetY(pVictim))) Value=Value*2/3;
  if(!Value)  return 1;
  if(pVictim->~IsClonk())// Clonks
  {
    pVictim->~GetSick(Random(Value/400)/2);
    pVictim->~CheckHealth(-Random(Value/56));
    DoEnergy(-Random(Value/1400),pVictim);
  }
  else// andere Lebewesen
   DoEnergy(-Random(Value/400),pVictim);
  return(1);
}

public func Activate(staerke)
{
  SetAction("Radiate");
  iStrength=staerke;
  for(var RadObj in FindObjects(Find_ID(GetID()),Find_Distance(250,0,0),Find_Exclude()))
  {
   iStrength+=RadObj->~GetStrength();
   RemoveObject(RadObj);
  }
  return(1);
}

/* Globale Verstrahl-Funktion */
global func Radiation2(staerke,x,y)
{
  CreateObject(_Y4D,x,y,-1)->~Activate(staerke);
  return(1);
}
