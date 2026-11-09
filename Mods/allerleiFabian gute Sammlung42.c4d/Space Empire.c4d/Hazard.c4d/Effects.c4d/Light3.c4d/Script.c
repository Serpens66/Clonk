/*-- Licht --*/

#strict 3

local iColor, bAmbience, iF;

func Initialize()
{
TurnOff();  // dadurch werden die lichter erst sichtbar, wenn sie an die gewünschte Stelle verfrachtet wurden
}

//Licht initialisieren, Parameter setzen, etc.
protected func Init(int iSize, int iColor, object pTarget, string cGraphics) {
  iSize ??= 0;
  iColor ??= 0;
  if(And(bAmbience, !IsDark()))
    Schedule("RemoveObject()",1,nil,this());
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
// nur einschalten, wenn lichter erlaubt
if(!ObjectCount(NOLI))  TurnOn();
}

//Wenn das Licht kein Ziel mehr hat -> weg damit.
protected func AttachTargetLost() {
  RemoveObject();
  return;
}

func CheckTarget()
{
    if(!GetActionTarget()) {RemoveObject();return;}
}

//Licht ausschalten
public func TurnOff() { SetVisibility(VIS_None); }

//Licht einschalten
public func TurnOn() { SetVisibility(VIS_All);  }

// größe des Lichts ändern
public func ChangeSize(int iNewSize) { 
  iNewSize ??= 0;
  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);

  iF = iNewSize;
  // SetCon(iF*sizemod/100);
  SetObjDrawTransform((iF ?? 0)*(sizemod ?? 0)/10,nil,nil,nil,(iF ?? 0)*(sizemod ?? 0)/10);
}

// Licht updaten... verändert keine Werte sondern
// passt die Anzeige nur an
public func Update() {
  ChangeSize(iF);
  ChangeColor(iColor);
}

//Farbe des Lichts ändern
public func ChangeColor(int iNewColor) {
  
  iNewColor ??= 0;
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
  iNewColor = RGBa(r,g,b,Min((a ?? 0)+60+(alphamod ?? 0),255));
  
  SetClrModulation(iNewColor);
  
  return(iNewColor);
}

//Grafik ändern
public func ChangeGraphics(string cNewGraphics) {
  SetGraphics(cNewGraphics, nil, GetID());
}

//festlegen, dass es ein Licht ist.
public func IsLight() { return(1); }
public func NoWarp() { return(true); }

/*--  Globale Funktionen zur Lichterzeugung  --*/


//erzeugt ein Licht mit Größe und Farbe und hängt es an pTarget
global func AddLight3(int iSize, int iColor, object pTarget) {
  iSize ??= 0;
  iColor ??= 0;
  if(!pTarget)
    if(!(pTarget = this()))
      return;
  return(CreateLight(LIG3, iSize, iColor, pTarget));
}


global func AddLightHalf3(int iSize, int iColor, object pTarget) {
  iSize ??= 0;
  iColor ??= 0;
  if(!pTarget)
    if(!(pTarget = this()))
      return;
  return(CreateLight(LIG3, iSize, iColor, pTarget, "Half"));
}

global func AddLightCone3(int iSize, int iColor, object pTarget) {
  iSize ??= 0;
  iColor ??= 0;
  if(!pTarget)
    if(!(pTarget = this()))
      return;
  return(CreateLight(LGC3, iSize, iColor, pTarget));
}

//Ambience-Light, Umgebungslicht, nicht sichtbar wenn keine Dunkelheit herrscht.
global func AddLightAmbience3(int iSize, object pTarget, string cGraphics) {
  iSize ??= 0;
  if(!pTarget)
    if(!(pTarget = this()))
      return;
  var light = CreateObject(LIG3,nil,nil,GetController(pTarget));
  light->LocalN("bAmbience") = true;
  light->Init((iSize ?? 0)*5, RGBa(255,255,255,50), pTarget, cGraphics);
  
  return(light);
}


