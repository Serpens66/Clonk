/*-- Wetterkontrollstation V.1.2 --*/

#strict 3

#include DOOR
#include WBAS

protected func Initialize(...) {
  SetLocal(90,nil);
  SetLocal(91,nil);
  SetLocal(92,Div(LandscapeWidth(),4));
  SetLocal(93,Mul(GetY(),-1));
  SetLocal(94,GetX());
  SetLocal(1,nil);
  return(1);
}

/* Wetterkontrolle-Menü */

protected func ContainedUp(...) {
  Message("Anzahl der verfügbaren Metalleinheiten: %d |Anzahl der verfügbaren Kristalleinheiten: %d",this(),ContentsCount(METL),ContentsCount(CRYS));
  CreateMenu(ME1Z,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("WindLinks 1M / 1K","WindLeft",WILZ,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("WindRechts 1M / 1K","WindRight",WIRZ,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Regen 5M / 8K","Rain",REGZ,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Schnee 4M / 6K","Snow",SCHZ,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Gewitter 7M / 10K","Storm",GEWZ,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Vulkan 12M /17K","Volcano",VULZ,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Erdbeben 4M / 6K","Earthquake",ERDZ,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Meteoriten Schauer 8M / 11K","MeteorShower",METZ,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Flammenregen 10M / 14K","FlameShower",FLAZ,Par(nil));
  return(1);
}

protected func ContainedDig(...) {
  CreateMenu(ME2Z,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Sektor1","Sector1",SE1Z,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Sektor2","Sector2",SE2Z,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Sektor3","Sector3",SE3Z,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Sektor4","Sector4",SE4Z,Par(nil));
  return(1);
}

protected func ContainedRight(...) {
  CreateMenu(ME3Z,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Kraft der Kristalle nutzen","Kristall",KRIS,Par(nil));
  if (Equal(Local(1), nil) ) AddMenuItem("Kraft der Metalle nutzen","Metall",META,Par(nil));
  return(1);
}

protected func Kristall(...) {
  Message("Station bezieht Kraft aus Kristallen",this());
  SetLocal(88,1);
  return(1);
}

protected func Metall(...) {
  Message("Station bezieht Kraft aus Metallen",this());
  SetLocal(88,nil);
  return(1);
}

/* PhaseCall */

public func Wheathermaker(...) {
  SetLocal(90,Sum(Local(90),1));
  if (Equal(Local(90), 30) ) LaunchLightning(Sum(GetX(),-6),Sum(GetY(),-53),nil,nil,-80,nil);
  if (Equal(Local(90), 30) ) SetLocal(90,nil);
  if (Equal(Local(1), 1)) return(Call("QWindLeft"));
  if (Equal(Local(1), 2)) return(Call("QWindRight"));
  if (Equal(Local(1), 3)) return(Call("QRain"));
  if (Equal(Local(1), 4)) return(Call("QSnow"));
  if (Equal(Local(1), 5)) return(Call("QStorm"));
  if (Equal(Local(1), 6)) return(Call("QVolcano"));
  if (Equal(Local(1), 7)) return(Call("QEarthquake"));
  if (Equal(Local(1), 8)) return(Call("QMeteorShower"));
  if (Equal(Local(1), 9)) return(Call("QFlameShower"));
  return(1);
}

public func EndPhase(...) {
  if (Equal(Local(1), 1)) return(Call("EWindLeft"));
  if (Equal(Local(1), 2)) return(Call("EWindRight"));
  if (Equal(Local(1), 3)) return(Call("ERain"));
  if (Equal(Local(1), 4)) return(Call("ESnow"));
  if (Equal(Local(1), 5)) return(Call("EStorm"));
  if (Equal(Local(1), 6)) return(Call("EVolcano"));
  if (Equal(Local(1), 7)) return(Call("EEarthquake"));
  if (Equal(Local(1), 8)) return(Call("EMeteorShower"));
  if (Equal(Local(1), 9)) return(Call("EFlameShower"));
  return(1);
}

/* Wind-Kontrolle */

public func QWindLeft(...) {
  SetLocal(91,Sum(Local(91),1));
  if (Equal(Local(91), 30) ) SetWind(-100);
  if (Equal(Local(91), 30) ) SetLocal(91,nil);
  return(1);
}

public func QWindRight(...) {
  SetLocal(91,Sum(Local(91),1));
  if (Equal(Local(91), 30) ) SetWind(100);
  if (Equal(Local(91), 30) ) SetLocal(91,nil);
  return(1);
}

public func EWindLeft(...) {
  SetLocal(1,nil);
  SetLocal(91,nil);
  return(1);
}

public func EWindRight(...) {
  SetLocal(1,nil);
  SetLocal(91,nil);
  return(1);
}

/* Ausrichtung */

protected func Sector1(...) {
  SetLocal(2,1);
  return(1);
}

protected func Sector2(...) {
  SetLocal(2,2);
  return(1);
}

protected func Sector3(...) {
  SetLocal(2,3);
  return(1);
}

protected func Sector4(...) {
  SetLocal(2,4);
  return(1);
}

/* Katastrophen */

public func WindLeft(...) {
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Equal(Local(88), nil) , Not((ContentsCount(METL,this()) ?? 0) >= 1 ))) return(1);
  if (And(Equal(Local(88), 1) , Not((ContentsCount(CRYS,this()) ?? 0) >= 1 ))) return(1);
  SetLocal(1,1);
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,nil,-75));
  ObjectCall(Local(89),"Start");
  return(1);
}

public func WindRight(...) {
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Equal(Local(88), nil) , Not((ContentsCount(METL,this()) ?? 0) >= 1 ))) return(1);
  if (And(Equal(Local(88), 1) , Not((ContentsCount(CRYS,this()) ?? 0) >= 1 ))) return(1);
  SetLocal(1,2);
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,nil,-75));
  ObjectCall(Local(89),"Start");
  return(1);
}

public func Rain(...) {
  if (Equal(Local(2), nil)) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Equal(Local(88), nil) , Not((ContentsCount(METL,this()) ?? 0) >= 5 ))) return(1);
  if (And(Equal(Local(88), 1) , Not((ContentsCount(CRYS,this()) ?? 0) >= 8 ))) return(1);
  SetLocal(1,3);
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,nil,-75));
  ObjectCall(Local(89),"Start");
  SetLocal(3,GetTemperature());
  return(1);
}

public func QRain(...) {
  SetWind(nil);
  if (Equal(Local(2), 1)) Call("Rain1");
  if (Equal(Local(2), 2)) Call("Rain2");
  if (Equal(Local(2), 3)) Call("Rain3");
  if (Equal(Local(2), 4)) Call("Rain4");
  return(1);
}

public func Rain1(...) {
  CastPXS("Water",4,nil,Sub(RandomX(nil,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(nil,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(nil,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(nil,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(nil,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(nil,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(nil,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(nil,Local(92)),Local(94)),Local(93));
  if (Equal(Local(90), 29) ) SetTemperature(20);
  return(1);
}

public func Rain2(...) {
  CastPXS("Water",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  if (Equal(Local(90), 29) ) SetTemperature(20);
  return(1);
}

public func Rain3(...) {
  CastPXS("Water",4,nil,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  if (Equal(Local(90), 29) ) SetTemperature(20);
  return(1);
}

public func Rain4(...) {
  CastPXS("Water",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  if (Equal(Local(90), 29) ) SetTemperature(20);
  return(1);
}

public func ERain(...) {
  SetTemperature(Local(3));
  SetLocal(1,nil);
  SetLocal(2,nil);
  return(1);
}

public func Snow(...) {
  if (Equal(Local(2), nil)) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Equal(Local(88), nil) , Not((ContentsCount(METL,this()) ?? 0) >= 4 ))) return(1);
  if (And(Equal(Local(88), 1) , Not((ContentsCount(CRYS,this()) ?? 0) >= 6 ))) return(1);
  SetLocal(1,4);
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,nil,-75));
  ObjectCall(Local(89),"Start");
  SetLocal(3,GetTemperature());
  return(1);
}

public func QSnow(...) {
  SetWind(nil);
  if (Equal(Local(2), 1)) Call("Snow1");
  if (Equal(Local(2), 2)) Call("Snow2");
  if (Equal(Local(2), 3)) Call("Snow3");
  if (Equal(Local(2), 4)) Call("Snow4");
  return(1);
}

public func Snow1(...) {
  CastPXS("Snow",4,nil,RandomX(nil,Local(92)),Local(93));
  CastPXS("Snow",4,nil,RandomX(nil,Local(92)),Local(93));
  CastPXS("Snow",4,nil,RandomX(nil,Local(92)),Local(93));
  CastPXS("Snow",4,nil,RandomX(nil,Local(92)),Local(93));
  CastPXS("Snow",4,nil,RandomX(nil,Local(92)),Local(93));
  CastPXS("Snow",4,nil,RandomX(nil,Local(92)),Local(93));
  CastPXS("Snow",4,nil,RandomX(nil,Local(92)),Local(93));
  CastPXS("Snow",4,nil,RandomX(nil,Local(92)),Local(93));
  if (Equal(Local(90), 29) ) SetTemperature(-20);
  return(1);
}

public func Snow2(...) {
  CastPXS("Snow",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  if (Equal(Local(90), 29) ) SetTemperature(-20);
  return(1);
}

public func Snow3(...) {
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  if (Equal(Local(90), 29) ) SetTemperature(-20);
  return(1);
}

public func Snow4(...) {
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,nil,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  if (Equal(Local(90), 29) ) SetTemperature(-20);
  return(1);
}

public func ESnow(...) {
  SetTemperature(Local(3));
  SetLocal(1,nil);
  SetLocal(2,nil);
  return(1);
}

public func Storm(...) {
  if (Equal(Local(2), nil)) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Equal(Local(88), nil) , Not((ContentsCount(METL,this()) ?? 0) >= 7 ))) return(1);
  if (And(Equal(Local(88), 1) , Not((ContentsCount(CRYS,this()) ?? 0) >= 10))) return(1);
  SetLocal(1,5);
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,nil,-75));
  ObjectCall(Local(89),"Start");
  return(1);
}

public func QStorm(...) {
  if (Equal(Local(2), 1)) Call("Storm1");
  if (Equal(Local(2), 2)) Call("Storm2");
  if (Equal(Local(2), 3)) Call("Storm3");
  if (Equal(Local(2), 4)) Call("Storm4");
  return(1);
}

public func Storm1(...) {
  if (Not(Random(120))) LaunchLightning(RandomX(nil,Div(LandscapeWidth(),4)), nil, -20, 41, +5, 15);
  return(1);
}

public func Storm2(...) {
  if (Not(Random(120))) LaunchLightning(RandomX(Div(LandscapeWidth(),4),Div(LandscapeWidth(),2)), nil, -20, 41, +5, 15);
  return(1);
}

public func Storm3(...) {
  if (Not(Random(120))) LaunchLightning(RandomX(Div(LandscapeWidth(),2),Mul(3,Div(LandscapeWidth(),4))), nil, -20, 41, +5, 15);
  return(1);
}

public func Storm4(...) {
  if (Not(Random(120))) LaunchLightning(RandomX(Mul(3,Div(LandscapeWidth(),4)),Div(LandscapeWidth(),4)), nil, -20, 41, +5, 15);
  return(1);
}

public func EStorm(...) {
  SetLocal(1,nil);
  SetLocal(2,nil);
  return(1);
}

public func Volcano(...) {
  if (Equal(Local(2), nil)) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Equal(Local(88), nil) , Not((ContentsCount(METL,this()) ?? 0) >= 12 ))) return(1);
  if (And(Equal(Local(88), 1) , Not((ContentsCount(CRYS,this()) ?? 0) >= 17 ))) return(1);
  SetLocal(1,6);
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,nil,-75));
  ObjectCall(Local(89),"Start");
  return(1);
}

public func QVolcano(...) {
  if (Equal(Local(2), 1)) Call("Volcano1");
  if (Equal(Local(2), 2)) Call("Volcano2");
  if (Equal(Local(2), 3)) Call("Volcano3");
  if (Equal(Local(2), 4)) Call("Volcano4");
  return(1);
}

public func Volcano1(...) {
  if (Not(Random(400))) LaunchVolcano(RandomX(nil,Div(LandscapeWidth(),4)));
  return(1);
}

public func Volcano2(...) {
  if (Not(Random(400))) LaunchVolcano(RandomX(Div(LandscapeWidth(),4),Div(LandscapeWidth(),2)));
  return(1);
}

public func Volcano3(...) {
  if (Not(Random(400))) LaunchVolcano(RandomX(Div(LandscapeWidth(),2),Mul(3,Div(LandscapeWidth(),4))));
  return(1);
}

public func Volcano4(...) {
  if (Not(Random(400))) LaunchVolcano(RandomX(Mul(3,Div(LandscapeWidth(),4)),LandscapeWidth()));
  return(1);
}

public func EVolcano(...) {
  SetLocal(1,nil);
  SetLocal(2,nil);
  return(1);
}

public func Earthquake(...) {
  if (Equal(Local(2), nil)) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Equal(Local(88), nil) , Not((ContentsCount(METL,this()) ?? 0) >= 4 ))) return(1);
  if (And(Equal(Local(88), 1) , Not((ContentsCount(CRYS,this()) ?? 0) >= 6 ))) return(1);
  SetLocal(1,7);
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,nil,-75));
  ObjectCall(Local(89),"Start");
  return(1);
}

public func QEarthquake(...) {
  if (Equal(Local(2), 1)) Call("Earthquake1");
  if (Equal(Local(2), 2)) Call("Earthquake2");
  if (Equal(Local(2), 3)) Call("Earthquake3");
  if (Equal(Local(2), 4)) Call("Earthquake4");
  return(1);
}

public func Earthquake1(...) {
  if (Not(Random(120))) LaunchEarthquake(RandomX(nil,Div(LandscapeWidth(),4)),Random(LandscapeHeight()));
  return(1);
}

public func Earthquake2(...) {
  if (Not(Random(120))) LaunchEarthquake(RandomX(Div(LandscapeWidth(),4),Div(LandscapeWidth(),2)),Random(LandscapeHeight()));
  return(1);
}

public func Earthquake3(...) {
  if (Not(Random(120))) LaunchEarthquake(RandomX(Div(LandscapeWidth(),2),Mul(3,Div(LandscapeWidth(),4))),Random(LandscapeHeight()));
  return(1);
}

public func Earthquake4(...) {
  if (Not(Random(120))) LaunchEarthquake(RandomX(Mul(3,Div(LandscapeWidth(),4)),LandscapeWidth()),Random(LandscapeHeight()));
  return(1);
}

public func EEarthquake(...) {
  SetLocal(1,nil);
  SetLocal(2,nil);
  return(1);
}

public func MeteorShower(...) {
  if (Equal(Local(2), nil)) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Equal(Local(88), nil) , Not((ContentsCount(METL,this()) ?? 0) >= 8 ))) return(1);
  if (And(Equal(Local(88), 1) , Not((ContentsCount(CRYS,this()) ?? 0) >= 11))) return(1);
  SetLocal(1,8);
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,nil,-75));
  ObjectCall(Local(89),"Start");
  return(1);
}

public func QMeteorShower(...) {
  if (Equal(Local(2), 1)) Call("MeteorShower1");
  if (Equal(Local(2), 2)) Call("MeteorShower2");
  if (Equal(Local(2), 3)) Call("MeteorShower3");
  if (Equal(Local(2), 4)) Call("MeteorShower4");
  return(1);
}

public func MeteorShower1(...) {
  if (Not(Random(90))) CastObjects(METO,1,20,Sub(RandomX(nil,Div(LandscapeWidth(),4)),GetX()),Sub(-1,GetY()));
  return(1);
}

public func MeteorShower2(...) {
  if (Not(Random(90))) CastObjects(METO,1,20,Sub(RandomX(Div(LandscapeWidth(),4),Div(LandscapeWidth(),2)),GetX()),Sub(-1,GetY()));
  return(1);
}

public func MeteorShower3(...) {
  if (Not(Random(90))) CastObjects(METO,1,20,Sub(RandomX(Div(LandscapeWidth(),2),Mul(3,Div(LandscapeWidth(),4))),GetX()),Sub(-1,GetY()));
  return(1);
}

public func MeteorShower4(...) {
  if (Not(Random(90))) CastObjects(METO,1,20,Sub(RandomX(Mul(3,Div(LandscapeWidth(),4)),LandscapeWidth()),GetX()),Sub(-1,GetY()));
  return(1);
}

public func EMeteorShower(...) {
  SetLocal(1,nil);
  SetLocal(2,nil);
  return(1);
}

public func FlameShower(...) {
  if (Equal(Local(2), nil)) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Equal(Local(88), nil) , Not((ContentsCount(METL,this()) ?? 0) >= 10 ))) return(1);
  if (And(Equal(Local(88), 1) , Not((ContentsCount(CRYS,this()) ?? 0) >= 14 ))) return(1);
  SetLocal(1,9);
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), 1) ) RemoveObject(FindContents(CRYS));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  if (Equal(Local(88), nil) ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,nil,-75));
  ObjectCall(Local(89),"Start");
  return(1);
}

public func QFlameShower(...) {
  if (Equal(Local(2), 1)) Call("FlameShower1");
  if (Equal(Local(2), 2)) Call("FlameShower2");
  if (Equal(Local(2), 3)) Call("FlameShower3");
  if (Equal(Local(2), 4)) Call("FlameShower4");
  return(1);
}

protected func FlameShower1(...) {
  if (Not(Random(8))) CastObjects(DFLM,1,nil,Sub(RandomX(nil,Div(LandscapeWidth(),4)),GetX()),Sub(-1,GetY()));
  return(1);
}

public func FlameShower2(...) {
  if (Not(Random(8))) CastObjects(DFLM,1,nil,Sub(RandomX(Div(LandscapeWidth(),4),Div(LandscapeWidth(),2)),GetX()),Sub(-1,GetY()));
  return(1);
}

public func FlameShower3(...) {
  if (Not(Random(8))) CastObjects(DFLM,1,nil,Sub(RandomX(Div(LandscapeWidth(),2),Mul(3,Div(LandscapeWidth(),4))),GetX()),Sub(-1,GetY()));
  return(1);
}

public func FlameShower4(...) {
  if (Not(Random(8))) CastObjects(DFLM,1,nil,Sub(RandomX(Mul(3,Div(LandscapeWidth(),4)),LandscapeWidth()),GetX()),Sub(-1,GetY()));
  return(1);
}

public func EFlameShower(...) {
  SetLocal(1,nil);
  SetLocal(2,nil);
  return(1);
}