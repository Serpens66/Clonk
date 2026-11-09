#strict 3

#include B116
#include _ZF1
//#include BAS9



  local ofen,ofen2,wasteamount,prodval;
local daueranzeige,basement;

// Temperatur und Umweltverschm.anzeige
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


func Completion(...) {
  SetActIdle();
Pollute();
  return(1);
}

func DoSmoke(...) {
  Smoke(20,-23,12);
  Smoke(44,-23,12);
  return(1);
}

func Ofen(...) {
  ofen=CreateObject(_U2X,13,22);
  ofen2=CreateObject(_U2Y,38,22);
  return(1);
}

func RemoveOfen(...) {
  if(ofen) RemoveObject(ofen);
  if(ofen2) RemoveObject(ofen2);
  return(1);
}

func Collection(...) {
  if(!ActIdle()) return(nil);
  var waste;
  if(waste=FindWaste()) Entsorge(waste,waste->~IsWaste()); 
  return(nil);
}

func FindWaste(...) {
  var object;
  for(var i=ContentsCount(nil,Par(nil)); (i ?? 0)>=0; S42MigrationStep(i, -1, true))
   if(ObjectCall((object=Contents(i,Par(nil))),"IsWaste"))
    return(object);
  return(nil);
}

func Entsorge(...) {
  if(Par(nil)->~Shredder()) return nil;
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(Par(nil)),Par(1));
  RemoveObject(Par(nil));
  wasteamount=Par(1);
  SetAction("Process");
  Ofen();
  return(1);
}

func Wasting(...) {
  DoSmoke();
  if(S42MigrationStep(prodval, 1, true)%10) return(1);
//Pollute(10);
  if(S42MigrationStep(wasteamount, -1, true)) return(nil);
  SetActIdle();
  RemoveOfen();
  Collection();
  return(1);
}

func BasicalConstruction(){ return true;}

func ContextWaste(...) {
  [Müll verbrennen|Image=DFLM|Condition]
  BringWaste(Par(nil));
  return(1);
}

private func BringWaste(arbeiter)
{
  if(!arbeiter->~ControlCommandAcquire(this(),SHLK))
   if(!arbeiter->~ControlCommandAcquire(this(),CHWS))
    if(!arbeiter->~ControlCommandAcquire(this(),HMWS))
     if(!arbeiter->~ControlCommandAcquire(this(),AMWS))
      if(!arbeiter->~ControlCommandAcquire(this(),_MSK))
       if(!arbeiter->~ControlCommandAcquire(this(),_SKA))
        if(!arbeiter->~ControlCommandAcquire(this(),_CHF))
         return(arbeiter->~ControlCommandAcquire(this(),CORP));
  return(1);
}

func NoGet(...) { return(1);
}

public func Dawn(phase)
{
  AddWindowLight(_U2Z,-16,-6,phase);
  AddWindowLight(_FN3,-48,9,phase);
  return(1);
}

/* ----- KI -----*/

public func ArtificalFactoryOf_ZKM(...) { return(1); // Kann _ZKM (Müllabfuhr) herstellen
}

public func ArtificalProduce_ZKM(arbeiter) // Produziert _ZKM
{
  // Gugge, was benötigt wird
  if(BringWaste(arbeiter)) return(1);
  return(nil);
}
