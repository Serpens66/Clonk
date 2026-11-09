#strict


// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }

protected func Initialize() {
  SetPosition(1,1);
}

func Destruction()
{
var i = 0;
for (var plrnum = GetPlayerCount (); plrnum; i++)
  if (GetPlayerName (i)) {
    plrnum--;
    SetFoW (0, i);
  }
}

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// hier erstmal alles deaktiviert, weil es jetzt direkt im Clonkskript steht. Klappt besser mit langsamer Anpassung des Sichtradius.
/*

#appendto CLNK
local zielSichtweite,zahler,keineweitereKontrolle, SetSichtweite;

protected func Initialize() {
  if(GetID()!=TS9Z) return(SetPlrViewRange(0),_inherited());
  SetPosition(1,1);

  var id;
  SetSichtweite=450;

zahler=1;
  SetAction("Wait");
keineweitereKontrolle==0;

  var obj = FindObject(GetID());
  if (!obj) return(Fertiog());
  // sich selbst dazuzählen
  obj->AddOne();
  Fertiog();
  RemoveObject();
}

public func AddOne()
{
  ++zahler;
}

func Fertiog()
{
 SetFoW(1,GetOwner());
}

//protected func Initialized() 
//{ FogofWar();}

//FoW aufrechterhalten bei abgespeicherten Spielen
protected func SetTransferZone() {Log("Set"); SetFoW(1,GetOwner());}
protected func UpdateTransferZone() {Log("Update"); SetFoW(1,GetOwner());}

//Alles zusammen nochmal. Und zwar weil beim initialisieren der Startclonks die SetPlrViewRange-Funzion nicht richtig funzt, da zu der Zeit noch garnicht die ganzen TS9Z-Objekte erstellt wurden. Außerdem auch weil man ja die Maussteuerung während des Spiels ausschalten kann (FoW wird dabei auch ausgestellt)




protected func FogofWar() 
{
//if(!Random(500))
//zahler = ObjectCount(TS9Z)+1; //+1 damit sich selbst mitgezählt wird. Ab und zu prüfen ob zahler noch aktuell

if(!Random(50))
for(var i1=0; i1<GetPlayerCount(); i1++) {
  SetFoW(1,i1);}
   

if(zahler==4)
FogOfWar1();

if(zahler==2)
FogOfWar2();

if(zahler==3)
FogOfWar3();
}


func FogOfWar1()
{

for(var i1=0; i1<GetPlayerCount(); i1++) {
// if(!Random(100)) SetFoW(1,i1);
 for(var i2=0; i2<GetCrewCount(i1); i2++) {
  zielSichtweite = 400;  // Standard und am Tag: 100
  if(IsNight()) zielSichtweite = 200; // Nacht? Dann eine Sichtweite von 50 anstreben

var z=0;
while(!GBackSky(GetX(GetCrew(i1,i2)),GetY(GetCrew(i1,i2))-z)) ++z; 
var himmely = GetY(GetCrew(i1,i2))-z; // y koordiante über Clonk, wo das erste mal himmer ist
  if(
//(GBackSolid(GetX(GetCrew(i1,i2)),GetY(GetCrew(i1,i2))-16) && 
(GetCrew(i1,i2)->~GetMaterial() == Material("Tunnel")) && (!PathFree(GetX(GetCrew(i1,i2)),GetY(GetCrew(i1,i2)),GetX(GetCrew(i1,i2)),himmely))  ) zielSichtweite = 120; // Tunnel und keine direkte Verbindung zum Himmel? Dann eine Sichtweite von 25 anstreben

  if(
// deaktiviert, weil es nicht immer wie geüwnscht funktioniert, besser ist pathfree
//(GBackSolid(GetX(GetCrew(i1,i2)),GetY(GetCrew(i1,i2))-16) && 
(GetCrew(i1,i2)->~GetMaterial() == Material("Tunnel")) && IsNight() && (!PathFree(GetX(GetCrew(i1,i2)),GetY(GetCrew(i1,i2)),GetX(GetCrew(i1,i2)),himmely)) ) zielSichtweite = 80; // Tunnel+ fehlende Verbindung zum  Himmel? UND Nacht?! Dann eine Sichtweite von 15 anstreben
     // Werte aktualisieren. Nicht direkt setzen, damit wir ein verlauf haben, funktioniert so nicht
//while (SetSichtweite != zielSichtweite) {  
//  if(SetSichtweite > zielSichtweite) SetSichtweite -= 1;
//  if(SetSichtweite < zielSichtweite) SetSichtweite += 1;
//}
  SetSichtweite = zielSichtweite;

//Message("%d %d",GetCrew(i1,i2),SetSichtweite,zielSichtweite);    //ist nur für Editor sinnvoll, wenn man die genauen Zahlen im Spiel sehen will.

SetPlrViewRange(SetSichtweite,GetCrew(i1,i2),1);  // und die Variabel SetSichtweite setzen
                                          }
                                           }
}

func FogOfWar2()
{

for(var i1=0; i1<GetPlayerCount(); i1++) {
//  if(!Random(100)) SetFoW(1,i1);
 for(var i2=0; i2<GetCrewCount(i1); i2++) {
 //Log("Nebelzähler ist 2");
  zielSichtweite = 400;  // Standard und am Tag: 100
  if(IsNight()) zielSichtweite = 200; // Nacht? Dann eine Sichtweite von 50 anstreben

     // Werte aktualisieren. Nicht direkt setzen, damit wir ein verlauf haben.
//while (SetSichtweite != zielSichtweite) {  
//  if(SetSichtweite > zielSichtweite) SetSichtweite -= 1;
//  if(SetSichtweite < zielSichtweite) SetSichtweite += 1;}
SetSichtweite = zielSichtweite;

//Message("%d %d",GetCrew(i1,i2),SetSichtweite,zielSichtweite);    //ist nur für Editor sinnvoll, wenn man die genauen Zahlen im Spiel sehen will.

SetPlrViewRange(SetSichtweite,GetCrew(i1,i2),1);  // und die Variabel SetSichtweite setzen
                                          }
                                           }
}

func FogOfWar3()
{
if(!Random(100))
 {
  SetSichtweite = 300;  // Standard 

for(var i1=0; i1<GetPlayerCount(); i1++) {
   SetFoW(1,i1);
 for(var i2=0; i2<GetCrewCount(i1); i2++) {

SetPlrViewRange(SetSichtweite,GetCrew(i1,i2),1);  // und die Variabel SetSichtweite setzen
 }
                                          }
                                           }

}

*/




// Mithilfe von einem Effekt hier dann timer aufrufe tätigen und so langsam die Sichtweite anpassen.
//FxAnpassTimer()
//{
//  if(SetSichtweite > zielSichtweite) SetSichtweite -= 1;
//  if(SetSichtweite < zielSichtweite) SetSichtweite += 1;
//}