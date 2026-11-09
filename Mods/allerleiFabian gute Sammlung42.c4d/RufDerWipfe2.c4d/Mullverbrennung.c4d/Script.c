#strict

#include B116
#include _ZF1
//#include BAS9



  local ofen,ofen2,wasteamount,prodval;
local daueranzeige,basement;

// Temperatur und Umweltverschm.anzeige
func ContainedDig()
{
if(daueranzeige==0) {daueranzeige=1;ShowTemp(); return();}
if(daueranzeige==1) {daueranzeige=0;ShowTemp(); return();}
}

//TimerCall
func ShowTemp()
{
  if(zuu==1) SetEntrance(0);
  if(zuu==0) SetEntrance(1);
if(daueranzeige==0) return();
var temperat;
temperat = GetTemperature() / 2;  // geteilt durch 2 weil -50 bis 50°C realisterische Werte sind
Message(Format("Temperatur:%d°C Umweltverschmutzung/ Umweltbelastbarkeit: |%d/%d           ",temperat,globalpollutionanzeige, globalnature),this());
}


Completion:
  SetActIdle();
Pollute();
  return(1);

DoSmoke:
  Smoke(20,-23,12);
  Smoke(44,-23,12);
  return(1);

Ofen:
  ofen=CreateObject(_U2X,13,22);
  ofen2=CreateObject(_U2Y,38,22);
  return(1);

RemoveOfen:
  if(ofen) RemoveObject(ofen);
  if(ofen2) RemoveObject(ofen2);
  return(1);

Collection:
  if(!ActIdle()) return(0);
  var waste;
  if(waste=FindWaste()) Entsorge(waste,waste->~IsWaste()); 
  return(0);

FindWaste:
  var object;
  for(var i=ContentsCount(0,Par(0)); i>=0; i--)
   if(ObjectCall((object=Contents(i,Par(0))),"IsWaste"))
    return(object);
  return(0);

Entsorge:
  if(Par(0)->~Shredder()) return 0;
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(Par(0)),Par(1));
  RemoveObject(Par(0));
  wasteamount=Par(1);
  SetAction("Process");
  Ofen();
  return(1);

Wasting: 
  DoSmoke();
  if(prodval++%10) return(1);
//Pollute(10);
  if(wasteamount--) return(0);
  SetActIdle();
  RemoveOfen();
  Collection();
  return(1);

func BasicalConstruction(){ return true;}

ContextWaste:
  [Müll verbrennen|Image=DFLM|Condition]
  BringWaste(Par(0));
  return(1);

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

NoGet: return(1);

public func Dawn(phase)
{
  AddWindowLight(_U2Z,-16,-6,phase);
  AddWindowLight(_FN3,-48,9,phase);
  return(1);
}

/* ----- KI -----*/

public ArtificalFactoryOf_ZKM: return(1); // Kann _ZKM (Müllabfuhr) herstellen

public func ArtificalProduce_ZKM(arbeiter) // Produziert _ZKM
{
  // Gugge, was benötigt wird
  if(BringWaste(arbeiter)) return(1);
  return(0);
}
