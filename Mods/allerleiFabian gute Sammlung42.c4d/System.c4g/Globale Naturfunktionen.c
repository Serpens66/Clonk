/*-- Globale Funktionen --*/

#strict

// Globale NaturFunktionen. Regenwolke: iStrength bestimmt die Stärke und iEnde bestimmt, wann die Wolke wieder entfernt wird. iBlitze von 0 bis 100 regelt die Häufigkeit der Blitze.


global func gChangeMeeresspiegel(value,set)  // ändert den Y Meeresspiegel um value, bzw. wenn set wahr ist, dann setzt es den Spiegel auf den die Y Position value.
{ 
    for(var object in FindObjects(Find_ID(DRAI)))  // funnktioniert also nur, wenn das Environment Meeresspiegel aktiv ist.
    {
        var newY;
        if(!set) newY = GetY(object)-value;
        else newY = value;
        
        if(newY < 0 || newY > LandscapeHeight()) Log("Neuer Meeresspiegel wuerde außerhalb der Karte liegen");
        else  SetPosition(GetX(object),newY,object); // Y = Y - value  == erhöhung des spiegels bei positivem value
    }   
}

global func ShowMeeresspiegel() // zeigt die existierenden DRAI objekte in der Log an
{
      for(var obj in FindObjects(Find_ID(DRAI)))  
    {
      Log(Format("Drai Position: %v/%v",GetX(obj),GetY(obj)));
    }
    for(var obj in FindObjects(Find_ID(DRA2)))  
    {
      Log(Format("Dra2 Position: %v/%v",GetX(obj),GetY(obj)));
    }
}

global func Meteoritenxyz(x) // ein Meteorit
{
  if(!x) x = Random(LandscapeWidth());
  CastObjects(METO,1,100,x,0);
}

global func Vulcanxyz(int x, y, int strength, string mat)  
{
  var xRand = 0;
  if(!x) {x = Random(LandscapeWidth());xRand=1;}
  if(y=="Zufall") 
  {
      y = Random(LandscapeHeight()); 
      var timer=0; 
      while(!GBackSolid(x,y) && timer<2000)  // muss in erde und nicht in flüssigkeit sein
      {
          y = Random(LandscapeHeight());timer++;
          if(timer>=1000 && xRand) x = Random(LandscapeWidth());// wenn nach 1000 veruschen nichts gefunden, dann auch x verändern, wenn zufällig erlaubt
      }
  } // eine stelle in der Erde suchen
  
  if(timer>=1999) Log(Format("Keine gültige Ausbruchsstelle fuer die Koordinaten x: %v und y: %v",x,y));
  LaunchVolcano(x,y,strength,mat);
}


global func Erdbebenxyz(x,y) // ein erdbeben
{
  if(!x) x = Random(LandscapeWidth());
  if(!y)
  {
      while(!GBackSolid(x,y) && y+GetY()<=LandscapeHeight()) y+=5;
  }
  LaunchEarthquake(GetX()+x,GetY()+y);
}


global func Blitzexyz(x) // ein Blitz
{
  if(!x) x = Random(LandscapeWidth());
  LaunchLightning(x, 0, -20, 41, +5, 15);
}

global func Eismeteoritenxyz(izahl, ignoretemp, dauer)
{
    var regel = CreateObject(_ICR, 1, 1, -1);
    if (!regel) return(0);
    regel->Einstellen(izahl, ignoretemp);
    if (dauer > 0) Schedule("RemoveObject()", dauer, 1, regel);
    return(1);
}

global func Entferneeismeteoritenxyz()
{
RemoveAll(_ICR);
}


global func Sandregenxyz(izahl,x)
{
    var xRand = 0; 
    if(!x) xRand = 1; // gibt an, ob x zufällig sein soll, oder obs der uebergebene wert sein soll
    if(!izahl) izahl = 1;
    while(izahl>0) {if(xRand) x = Random(LandscapeWidth());CastPXS("Sand",1,0,x,0);izahl--;}
}


global func Sandregenwolkexyz(iStrenght,iEnde,iBlitze)
{
var blitze=iBlitze;
var ende = iEnde;
 var strenght = iStrenght;
  var iMat= Material("Sand");
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER());
  if(!cloud) return(0);
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);
}


global func Ascheregenxyz(izahl,x)
{
    var xRand = 0; 
    if(!x) xRand = 1; // gibt an, ob x zufällig sein soll, oder obs der uebergebene wert sein soll
    if(!izahl) izahl = 1;
    while(izahl>0) {if(xRand) x = Random(LandscapeWidth());CastPXS("Ashes",1,0,x,0);izahl--;}
}


global func Ascheregenwolkexyz(iStrenght,iEnde,iBlitze)
{
var blitze=iBlitze;
var ende = iEnde;
 var strenght = iStrenght;
  var iMat= Material("Ashes");
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER());
  if(!cloud) return(0);
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);
}


global func Lavaregenxyz(izahl,x)
{  
    var xRand = 0; 
    if(!x) xRand = 1; // gibt an, ob x zufällig sein soll, oder obs der uebergebene wert sein soll
    if(!izahl) izahl = 1;
    while(izahl>0) {if(xRand) x = Random(LandscapeWidth());CastPXS("Lava",1,0,x,0);izahl--;}
}



global func Lavaregenwolkexyz(iStrenght,iEnde,iBlitze)
{
var blitze=iBlitze;
var ende = iEnde;
 var strenght = iStrenght;
  var iMat= Material("Lava");
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER());
  if(!cloud) return(0);
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);
}


global func Saureregenxyz(izahl,x)
{ 
    var xRand = 0; 
    if(!x) xRand = 1; // gibt an, ob x zufällig sein soll, oder obs der uebergebene wert sein soll
    if(!izahl) izahl = 1;
    while(izahl>0) {if(xRand) x = Random(LandscapeWidth());CastPXS("Acid",1,0,x,0);izahl--;}
}


global func Saureregenwolkexyz(iStrenght,iEnde,iBlitze)
{
var blitze=iBlitze;
var ende = iEnde;
 var strenght = iStrenght;
  var iMat= Material("Acid");
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER());
  if(!cloud) return(0);
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);
}


global func Regenxyz(izahl, x)
{
    if (!izahl) izahl = 1;
    var temperatur = GetTemperature();
    if (temperatur >= 80) return(0);

    var material;
    if (temperatur > 0) material = "Water";
    else if (temperatur >= -60) material = "Snow";
    else material = "Ice";

    var zufaelligesX = !x;
    while (izahl > 0)
    {
        if (zufaelligesX) x = Random(LandscapeWidth());
        CastPXS(material, 1, 0, x, 0);
        izahl--;
    }
    if (temperatur >= -60) return(0);
}

global func Regenwolkexyz(iStrenght,iEnde,iBlitze)  // bzw. Schnee  bzw Eisregen. iStrenght von 0 bis 100, wie stark es regnen soll. iEnde die Zeit in Frames, bis Wolke verschwindet. iBlitze, Chance von  iBlitze zu 200, dass es blitzt
{
//wenn heißer als 40°C, dann regnet es Sand
  if(GetTemperature() >= 80) 
  {
var blitze=iBlitze;
var ende = iEnde;
 var strenght = iStrenght;
 var iMat= Material("Sand");
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER());
  if(!cloud) return(0);
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);}


  if(GetTemperature()>0 && GetTemperature() < 80) 
  {
var blitze=iBlitze;
var ende = iEnde;
 var strenght = iStrenght;
 var iMat= Material("Water");
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER());
  if(!cloud) return(0);
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);}

  if(GetTemperature()<=0 && GetTemperature() >= -60) 
{
var blitze=iBlitze;
var ende = iEnde;
 var strenght = iStrenght;
var iMat= Material("Snow");
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER());
  if(!cloud) return(0);
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);}
  
if(GetTemperature() < -60)
{ 
var blitze=iBlitze;
var ende = iEnde;
 var strenght = iStrenght;
 var iMat= Material("Ice");
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER());
  if(!cloud) return(0);
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);}
  else {return(0);}   // bei größer 80, verdunstet alles und bei kleiner -60 gibts Eisregen.
}