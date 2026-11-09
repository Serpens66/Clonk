/*-- Regen --*/

#strict

local warten, regnet,regnenlassen,wolke;


// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden, noch nicht fertig, daher normal choosable
// public func IsVariabelChooseable2() { return(1); }
public func IsChooseable2() { return(1); }

func Initialize()
{
SetPosition(1,1);
regnet=0;
regnenlassen=0;
warten=0;
}

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Es soll häufiger schneien als regnen.

func Regen()
{

if(regnet==0)
{
  //Regen
  if(GetTemperature()>0 && GetTemperature() < 80) {warten+=1;}

  //Schnee
  if(GetTemperature()<1 && GetTemperature() > -60) {warten+=3;}

  // Eisregen
  if(GetTemperature() < -60)   {warten+=3;}
}

if(!Random(4000))  // dieser if befehl steuert das beenden des regens. nur mit gewisser chance dies ausführen, hiermit wird regen mehr oder weniger uzfällig gestartet und auch beendet
{
    if(warten>=12000)
    {
      regnenlassen+=1;
      regnet=1;

    // folgendes ist eig überflüssig, weil nun mit Wolke geregelt.

    //wenn es mehr als 40°C ist, dann Sandregen
    if(GetTemperature() > 80){  if(regnenlassen>=2000) {regnet=0;warten=0;regnenlassen=0;wolke=0;}}   // dieses aufhören hat keine auswirkung mehr, da die dauer der wolke in der fkt Regenwolkexyz geergelt wird. Dennoch dies stehen lassen, da wenn wolken deaktiviert, regen wieder so gemacht wird.

    //wenn es regentemperatur ist, dann dauerts mind. 2000 Frames ,bis es aufhört
    if(GetTemperature()>0 && GetTemperature() < 80){  if(regnenlassen>=1000) {regnet=0;warten=0;regnenlassen=0;wolke=0;}}

    // wenn es schneetemperatur ist, dann dauerts mind. 4000Frames, bis es aufhört.
    if(GetTemperature()<1 && GetTemperature() > -60){ if(regnenlassen>=2000) {regnet=0;warten=0;regnenlassen=0;wolke=0;}}

    // wenn es eistemp ist, dann mindestens 3000 Frames
    if(GetTemperature() < -60){  if(regnenlassen>=2000) {regnet=0;warten=0;regnenlassen=0;wolke=0;}}
    }
}


// sobald einmal gestartet, werden einmal wolken erstellt.
// zwei Regenwolken werden erstellt, mit der stärke zwischen 50 und 100, die 2000 bis 4000 Frames halten.
if(warten>=12000 && regnet==1 && wolke==0) {
if(!(ObjectCount(WOL_)))  // dies objekt heißt "keine wolken"
{
Regenwolkexyz(RandomX(30,100),RandomX(1000,2000),GetScenLightning());Regenwolkexyz(RandomX(30,100),RandomX(1000,2000),GetScenLightning());wolke=1;  // wenn wolken aktiv, wird wolke erstellt und hier steht auch wie lange diese aktiv sein sollen
}
else {if(GetTemperature() < 80) Regenxyz();if(GetTemperature() >= 80) Sandregenxyz();}
}
// -> es werden 2 regenwolken mit stärke 30 bis 100 erstellt und bleiben 2000 bis 4000frames, mit Blitzen je nach szenarion einstellung.

// kann noch regenxyz dazu, dann regnets überall ztusätzlich

}


// -> bei über 0Grad wird nach 12000frames alle 1000Frames mit der Chance von 1 zu 4 (=oder jeden frame mit chance 1 zu 4000) der Regen gestartet. Dieser hält dann mindestens 1000 Frames , aber durch Random evtl auch 4 mal länger.
// -> bei Schneetemp wird schon nach 4000Frames (12000/3) die Möglichkeit für Schnee gegeben. dieser hält auch doppelt solange.
