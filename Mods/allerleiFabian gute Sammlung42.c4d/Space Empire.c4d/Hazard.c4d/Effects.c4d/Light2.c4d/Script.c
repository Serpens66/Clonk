/*-- Licht --*/

#strict

local iColor, bAmbience, iF;

func Initialize()
{
TurnOff();  // dadurch werden die lichter erst sichtbar, wenn sie an die gewünschte Stelle verfrachtet wurden
}

//Licht initialisieren, Parameter setzen, etc.
protected func Init(int iSize, int iColor, object pTarget, string cGraphics) {
  if(bAmbience && !IsDark())
    Schedule("RemoveObject()",1,0,this());
//Werte
  if(!iSize)
    iSize = GetCon();  
  if(!pTarget)
    pTarget = GetActionTarget();

  SetOwner(GetController(pTarget));

//Setzen und so
  ChangeColor(iColor);
  ChangeSize(iSize);
  ChangeGraphics(cGraphics);
  SetPosition(GetX(pTarget),GetY(pTarget));
  SetAction("Attach",pTarget);
  Schedule("SetPlrViewRange(FogViewRange(),this(),1)",2,0,0);  // gibt den Sichtradius erst 2Frames später, so wird das nervige flackern beim schießen mit leuchtenden Kugeln verhindert (weil aus i-einen Grund das Licht dort ganz kurz über dem clonk erstellt wird).. problem nun auch in den Kugeln mit schedule gelöst, aber hier schadets ja nicht
// nur einschalten, wenn lichter erlaubt
if(!ObjectCount(NOLI))  TurnOn();
}

func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    return(500);
}

//Wenn das Licht kein Ziel mehr hat -> weg damit.
protected func AttachTargetLost() {
  RemoveObject();
  return();
}

func CheckTarget()
{
    if(!GetActionTarget()) {RemoveObject();return();}
}

//Licht ausschalten
public func TurnOff() { SetVisibility(VIS_None); }

//Licht einschalten
public func TurnOn() { SetVisibility(VIS_All);  }

// größe des Lichts ändern
public func ChangeSize(int iNewSize) { 
  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);

  iF = iNewSize;
  // SetCon(iF*sizemod/100);
  SetObjDrawTransform(iF*sizemod/10,0,0,0,iF*sizemod/10);
}

// Licht updaten... verändert keine Werte sondern
// passt die Anzeige nur an
public func Update() {
  ChangeSize(iF);
  ChangeColor(iColor);
}

//Farbe des Lichts ändern
public func ChangeColor(int iNewColor) {
  
  iColor = iNewColor;
  
  //Wenn keine dunkelheit ist
  if(!IsDark()) {
    //Hintergrundlichter ausblenden
    if(bAmbience)
      iNewColor = RGBa(255,255,255,255);
  }  
  
  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);
  
  var r,g,b,a;
  SplitRGBaValue(iColor,r,g,b,a);
  iNewColor = RGBa(r,g,b,Min(a+60+alphamod,255));
  
  SetClrModulation(iNewColor);
  
  return(iNewColor);
}

//Grafik ändern
public func ChangeGraphics(string cNewGraphics) {
  SetGraphics(cNewGraphics, 0, GetID());
}

//festlegen, dass es ein Licht ist.
public func IsLight() { return(1); }
public func NoWarp() { return(true); }

/*--  Globale Funktionen zur Lichterzeugung  --*/

//erzeugt ein Licht mit Größe und Farbe und hängt es an pTarget
global func AddLight2(int iSize, int iColor, object pTarget) {
  if(!pTarget)
    if(!(pTarget = this()))
      return();
//if(FindObject(KOHE) && (!(FindObject(TS9Z)))) return(); // kein 400er Licht, wenn Helligkeitswechsel deaktiviert und kein Kriegsnebel vorhanden
  
  return(CreateLight(LIG2, iSize, iColor, pTarget));
}


global func AddLightHalf2(int iSize, int iColor, object pTarget) {
  if(!pTarget)
    if(!(pTarget = this()))
      return();
//if(FindObject(KOHE) && (!(FindObject(TS9Z)))) return(); // kein 400er Licht, wenn Helligkeitswechsel deaktiviert und kein Kriegsnebel vorhanden
  
  return(CreateLight(LIG2, iSize, iColor, pTarget, "Half"));
}

global func AddLightCone2(int iSize, int iColor, object pTarget) {
  if(!pTarget)
    if(!(pTarget = this()))
      return();
//if(FindObject(KOHE) && (!(FindObject(TS9Z)))) return(); // kein 400er Licht, wenn Helligkeitswechsel deaktiviert und kein Kriegsnebel vorhanden
  
  return(CreateLight(LGC2, iSize, iColor, pTarget));
}

//Ambience-Light, Umgebungslicht, nicht sichtbar wenn keine Dunkelheit herrscht.
global func AddLightAmbience2(int iSize, object pTarget, string cGraphics) {
  if(!pTarget)
    if(!(pTarget = this()))
      return();
//if(FindObject(KOHE) && (!(FindObject(TS9Z)))) return();  // kein 400er Licht, wenn Helligkeitswechsel deaktiviert und kein Kriegsnebel vorhanden
      
  var light = CreateObject(LIG2,0,0,GetController(pTarget));
  light->LocalN("bAmbience") = true;
  light->Init(iSize*5, RGBa(255,255,255,50), pTarget, cGraphics);
  
  return(light);
}
