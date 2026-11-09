/*-- Globale Funktionen --*/

#strict 3

// Globale NaturFunktionen. Regenwolke: iStrength bestimmt die Stärke und iEnde bestimmt, wann die Wolke wieder entfernt wird. iBlitze von 0 bis 100 regelt die Häufigkeit der Blitze.


global func gChangeMeeresspiegel(value,set)  // ändert den Y Meeresspiegel um value, bzw. wenn set wahr ist, dann setzt es den Spiegel auf den die Y Position value.
{ 
    value ??= 0;
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
  var breite = LandscapeWidth();
  var xRand = !x;
  if (xRand) x = Random(breite);
  if (y == "Zufall")
  {
      if (x < 0 || x >= breite) return;
      for (var versuch = 0; versuch < 2000; versuch++)
      {
          y = Random(LandscapeHeight());
          if (versuch >= 1000 && xRand) x = Random(breite); // Bei Bedarf auch die Spalte wechseln.
          // Die Pruefung ohne Objektkontext verwendet wie LaunchVolcano Kartenkoordinaten.
          // y=0 bezeichnet im Vulkanhelfer den Kartenboden. FXV1 prueft beim Start auch
          // den Untergrund 8 Pixel ueber seiner Position.
          if (y >= 8 && global->GBackSolid(x, y) && global->GBackSolid(x, y - 8))
          {
              LaunchVolcano(x, y, strength, mat); // Fester Untergrund, keine Fluessigkeit.
              return;
          }
      }
      Log(Format("Keine gueltige Ausbruchsstelle fuer x: %v und y: %v", x, y));
      return;
  }
  LaunchVolcano(x, y, strength, mat);
}


global func Erdbebenxyz(x,y) // ein erdbeben
{
  if(!x) x = Random(LandscapeWidth());
  if(!y)
  {
      y = 0;
      while(!GBackSolid(x,y) && y+(GetY() ?? 0)<=LandscapeHeight()) y+=5;
  }
  LaunchEarthquake((GetX() ?? 0)+x,(GetY() ?? 0)+y);
}


global func Blitzexyz(x) // ein Blitz
{
  if(!x) x = Random(LandscapeWidth());
  LaunchLightning(x, 0, -20, 41, +5, 15);
}

global func Eismeteoritenxyz(izahl, ignoretemp, dauer)
{
    var regel = CreateObject(_ICR, 1, 1, -1); // Erstellt eine Eismeteoritenregel. izahl bestimmt die Wahrscheinlichkeit.
    if (!regel) return;
    regel->Einstellen(izahl, ignoretemp); // Mit ignoretemp kann die Regel auch bei milder Temperatur ausloesen.
    if ((dauer ?? 0) > 0) Schedule("RemoveObject()", dauer, 1, regel); // Nach Ablauf nur die Regel dieses Eissturms entfernen.
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
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER);
  if(!cloud) return;
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
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER);
  if(!cloud) return;
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
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER);
  if(!cloud) return;
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
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER);
  if(!cloud) return;
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);
}


global func Regenxyz(izahl, x) // Ein Partikel je Einheit, als Regen, Schnee oder Eisregen.
{
    if (!izahl) izahl = 1; // Ohne Mengenangabe einen Partikel erzeugen.
    var temperatur = GetTemperature();
    if (temperatur >= 80) return; // Ab 80 verdunstet der Niederschlag.

    var material;
    if (temperatur > 0) material = "Water";
    else if (temperatur >= -60) material = "Snow";
    else material = "Ice";

    var zufaelligesX = !x; // Ohne x die Position fuer jeden Partikel neu ziehen.
    while (izahl > 0)
    {
        if (zufaelligesX) x = Random(LandscapeWidth());
        CastPXS(material, 1, 0, x, 0);
        izahl--;
    }
    if (temperatur >= -60) return;
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
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER);
  if(!cloud) return;
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);}


  if(GetTemperature()>0 && GetTemperature() < 80) 
  {
var blitze=iBlitze;
var ende = iEnde;
 var strenght = iStrenght;
 var iMat= Material("Water");
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER);
  if(!cloud) return;
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);}

  if(GetTemperature()<=0 && GetTemperature() >= -60) 
{
var blitze=iBlitze;
var ende = iEnde;
 var strenght = iStrenght;
var iMat= Material("Snow");
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER);
  if(!cloud) return;
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);}
  
if(GetTemperature() < -60)
{ 
var blitze=iBlitze;
var ende = iEnde;
 var strenght = iStrenght;
 var iMat= Material("Ice");
 var cloud= CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER);
  if(!cloud) return;
  cloud-> Activate(iMat,LandscapeWidth(),strenght,blitze);
  Schedule("RemoveObject()",ende,0,cloud);}
  else {return;}   // bei größer 80, verdunstet alles und bei kleiner -60 gibts Eisregen.
}