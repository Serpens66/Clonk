/*-- Regenerator --*/

#strict 3
#include B116
#include _ZF1
//#include BAS9

local tempchange,aktuell,wert;
local daueranzeige,basement;


public func IsWaste(){ return(wert);}

func Initialize()
{
wert=nil;
}

protected func Construction()
{
  if(Equal(GetID(), BasementID()) ) return(nil);
  // Nur wenn noch kein Fundament vorhanden: Damit bei Überladung von
  // Fundamentobjekten nicht Fundamente mehrfach erstellt werden.
  if(!basement) basement = CreateObject(BasementID(), nil, 8, GetOwner() );
  return(_inherited() );
}
  
public func Destruction()
{
  Predestruction();
  return(_inherited());
}

func Predestruction()
{
  if(Equal(GetID(), BasementID()) ) return(nil);
  if(basement) RemoveObject(basement);
}


func ContainedDig()
{
if(Equal(daueranzeige, nil)) {daueranzeige=1;ShowTemp(); return;}
if(Equal(daueranzeige, 1)) {daueranzeige=nil;ShowTemp(); return;}
}

//TimerCall
func ShowTemp()
{
  if(Equal(zuu, 1)) SetEntrance(nil);
  if(Equal(zuu, nil)) SetEntrance(1);
if(Equal(daueranzeige, nil)) return;
var temperat;
temperat = (GetTemperature() ?? 0) / 2;  // geteilt durch 2 weil -50 bis 50°C realisterische Werte sind
Message(Format("Temperatur:%d°C Umweltverschmutzung/ Umweltbelastbarkeit: |%d/%d           ",temperat,globalpollutionanzeige, globalnature),this());
}


//BasementID: return(B116);
func BasicalConstruction(){ return true;}

func ContextTerra(...) {
  [Terraforming|Image=_X1C|Condition=FriendlyClonk]
  var clonk=Par(nil);
  if(!Equal(Contained(clonk), this())) {
	SetCommand(clonk,"Enter",this());
	AppendCommand(clonk,"Call",this(),clonk,nil,nil,nil,"Menu");
	return(1);}
  Menu(nil,clonk);
  return(1);
}

func ContainedUp(...) {
  [Terraforming|Image=_X1C]
  Menu(nil,Par(nil));
  return(1);
}

func Menu(...) {
  var clonk=Par(1);
  CreateMenu(_X1C,clonk,this());
  AddMenuItem("Schadstoffe filtern","Filter",_Z2A,clonk);
  if(GetPlrKnowledge(TMST,GetOwner())) // um Temp zu ändern, ist Forschung noetig
  {
    AddMenuItem("Temperatur senken","Tempe",ICE1,clonk);
    AddMenuItem("Temperatur erhöhen","Tempe",DFLM,clonk);
    AddMenuItem("Temperatur halten","Tempe",_X1A,clonk);
  }
  AddMenuItem("Arbeit einstellen","Stop",_X1A,clonk);
  AddMenuItem("Infos über Temperatur","Descr",RMMG,clonk,nil,clonk);
  return(1);
}

func Stop()
{
wert=nil;
SetAction("Idle");
}


private func Descr(idItem,pClonk)
{
  MessageWindow("Ab -30°C gefrieren Partikel in der Luft und regnen als Eismeteoriten, falls Eishagel aktiviert, auf die Erde. Außerdem ist es so kalt, dass die Clonks ohne Schutzanzug festfrieren können und langsam erfrieren. Ab 40°C vertrocknen die Bäume, außerdem besteht hohe Waldbrandgefahr. Auch dem Clonk ist es ab 30°C zu heiß und bekommt ohne Schutzanzug Schaden, noch mehr ab 40°C. (Sofern toedliche Temp aktiviert). Je nach Temperatur verdunstet Wasser, sofern aktiviert. Außerdem solltest du darauf achten die Temperatur nicht zu schnell zu ändern, da sich sonst heftige Gewitter bilden!", GetOwner(pClonk));
}



protected func Tempe(select)
{
  SetAction("Process2");
  tempchange=nil;
  aktuell=GetTemperature();
  if(Equal(select, ICE1)) return(tempchange=-1);
  if(Equal(select, DFLM)) return(tempchange=+1);
  if(Equal(select, _X1A)) return(tempchange=1000);
  return("*mecker*");
}

func Temping(...) {
wert=1000; 
if(Equal(tempchange, 1000)) {SetTemperature(aktuell);}
else {SetTemperature((GetTemperature() ?? 0)+(tempchange ?? 0));}

//  Pollute(30);
  return(1);  
}

func Filter(...) {
  SetAction("LuftFiltern");
  return(1);
}

func Filtering(...) {
//  Pollute(-10);
wert=-200;  
  return(1);
}

public func Dawn(phase)
{
  AddWindowLight(_FN1,-1,18,phase);
  AddWindowLight(_FN3,-38,40,phase);
  return(1);
}
