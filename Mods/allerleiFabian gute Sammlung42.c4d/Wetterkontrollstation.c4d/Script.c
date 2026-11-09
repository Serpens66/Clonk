/*-- Wetterkontrollstation V.1.2 --*/

#strict

#include DOOR
#include WBAS

protected Initialize:
  SetLocal(90,0);
  SetLocal(91,0);
  SetLocal(92,Div(LandscapeWidth(),4));
  SetLocal(93,Mul(GetY(),-1));
  SetLocal(94,GetX());
  SetLocal(1,0);
  return(1);

/* Wetterkontrolle-Menü */

protected ContainedUp:
  Message("Anzahl der verfügbaren Metalleinheiten: %d |Anzahl der verfügbaren Kristalleinheiten: %d",this(),ContentsCount(METL),ContentsCount(CRYS));
  CreateMenu(ME1Z,Par(0));
  if (Local(1) == 0 ) AddMenuItem("WindLinks 1M / 1K","WindLeft",WILZ,Par(0));
  if (Local(1) == 0 ) AddMenuItem("WindRechts 1M / 1K","WindRight",WIRZ,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Regen 5M / 8K","Rain",REGZ,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Schnee 4M / 6K","Snow",SCHZ,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Gewitter 7M / 10K","Storm",GEWZ,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Vulkan 12M /17K","Volcano",VULZ,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Erdbeben 4M / 6K","Earthquake",ERDZ,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Meteoriten Schauer 8M / 11K","MeteorShower",METZ,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Flammenregen 10M / 14K","FlameShower",FLAZ,Par(0));
  return(1);

protected ContainedDig:
  CreateMenu(ME2Z,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Sektor1","Sector1",SE1Z,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Sektor2","Sector2",SE2Z,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Sektor3","Sector3",SE3Z,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Sektor4","Sector4",SE4Z,Par(0));
  return(1);

protected ContainedRight:
  CreateMenu(ME3Z,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Kraft der Kristalle nutzen","Kristall",KRIS,Par(0));
  if (Local(1) == 0 ) AddMenuItem("Kraft der Metalle nutzen","Metall",META,Par(0));
  return(1);

protected Kristall:
  Message("Station bezieht Kraft aus Kristallen",this());
  SetLocal(88,1);
  return(1);

protected Metall:
  Message("Station bezieht Kraft aus Metallen",this());
  SetLocal(88,0);
  return(1);

/* PhaseCall */

public Wheathermaker:
  SetLocal(90,Sum(Local(90),1));
  if (Local(90) == 30 ) LaunchLightning(Sum(GetX(),-6),Sum(GetY(),-53),0,0,-80,0);
  if (Local(90) == 30 ) SetLocal(90,0);
  if (Local(1) == 1) return(Call("QWindLeft"));
  if (Local(1) == 2) return(Call("QWindRight"));
  if (Local(1) == 3) return(Call("QRain"));
  if (Local(1) == 4) return(Call("QSnow"));
  if (Local(1) == 5) return(Call("QStorm"));
  if (Local(1) == 6) return(Call("QVolcano"));
  if (Local(1) == 7) return(Call("QEarthquake"));
  if (Local(1) == 8) return(Call("QMeteorShower"));
  if (Local(1) == 9) return(Call("QFlameShower"));
  return(1);

public EndPhase:
  if (Local(1) == 1) return(Call("EWindLeft"));
  if (Local(1) == 2) return(Call("EWindRight"));
  if (Local(1) == 3) return(Call("ERain"));
  if (Local(1) == 4) return(Call("ESnow"));
  if (Local(1) == 5) return(Call("EStorm"));
  if (Local(1) == 6) return(Call("EVolcano"));
  if (Local(1) == 7) return(Call("EEarthquake"));
  if (Local(1) == 8) return(Call("EMeteorShower"));
  if (Local(1) == 9) return(Call("EFlameShower"));
  return(1);

/* Wind-Kontrolle */

public QWindLeft:
  SetLocal(91,Sum(Local(91),1));
  if (Local(91) == 30 ) SetWind(-100);
  if (Local(91) == 30 ) SetLocal(91,0);
  return(1);

public QWindRight:
  SetLocal(91,Sum(Local(91),1));
  if (Local(91) == 30 ) SetWind(100);
  if (Local(91) == 30 ) SetLocal(91,0);
  return(1);

public EWindLeft:
  SetLocal(1,0);
  SetLocal(91,0);
  return(1);

public EWindRight:
  SetLocal(1,0);
  SetLocal(91,0);
  return(1);

/* Ausrichtung */

protected Sector1:
  SetLocal(2,1);
  return(1);

protected Sector2:
  SetLocal(2,2);
  return(1);

protected Sector3:
  SetLocal(2,3);
  return(1);

protected Sector4:
  SetLocal(2,4);
  return(1);

/* Katastrophen */

public WindLeft:
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Local(88) == 0 , Not(ContentsCount(METL,this()) >= 1 ))) return(1);
  if (And(Local(88) == 1 , Not(ContentsCount(CRYS,this()) >= 1 ))) return(1);
  SetLocal(1,1);
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,0,-75));
  ObjectCall(Local(89),"Start");
  return(1);

public WindRight:
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Local(88) == 0 , Not(ContentsCount(METL,this()) >= 1 ))) return(1);
  if (And(Local(88) == 1 , Not(ContentsCount(CRYS,this()) >= 1 ))) return(1);
  SetLocal(1,2);
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,0,-75));
  ObjectCall(Local(89),"Start");
  return(1);

public Rain:
  if (Local(2) == 0) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Local(88) == 0 , Not(ContentsCount(METL,this()) >= 5 ))) return(1);
  if (And(Local(88) == 1 , Not(ContentsCount(CRYS,this()) >= 8 ))) return(1);
  SetLocal(1,3);
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,0,-75));
  ObjectCall(Local(89),"Start");
  SetLocal(3,GetTemperature());
  return(1);

public QRain:
  SetWind(0);
  if (Local(2) == 1) Call("Rain1");
  if (Local(2) == 2) Call("Rain2");
  if (Local(2) == 3) Call("Rain3");
  if (Local(2) == 4) Call("Rain4");
  return(1);

public Rain1:
  CastPXS("Water",4,0,Sub(RandomX(0,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(0,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(0,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(0,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(0,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(0,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(0,Local(92)),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(0,Local(92)),Local(94)),Local(93));
  if (Local(90) == 29 ) SetTemperature(20);
  return(1);

public Rain2:
  CastPXS("Water",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  if (Local(90) == 29 ) SetTemperature(20);
  return(1);

public Rain3:
  CastPXS("Water",4,0,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(2,Local(92)),Mul(3,Local(92))),Local(94)),Local(93));
  if (Local(90) == 29 ) SetTemperature(20);
  return(1);

public Rain4:
  CastPXS("Water",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Water",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  if (Local(90) == 29 ) SetTemperature(20);
  return(1);

public ERain:
  SetTemperature(Local(3));
  SetLocal(1,0);
  SetLocal(2,0);
  return(1);

public Snow:
  if (Local(2) == 0) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Local(88) == 0 , Not(ContentsCount(METL,this()) >= 4 ))) return(1);
  if (And(Local(88) == 1 , Not(ContentsCount(CRYS,this()) >= 6 ))) return(1);
  SetLocal(1,4);
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,0,-75));
  ObjectCall(Local(89),"Start");
  SetLocal(3,GetTemperature());
  return(1);

public QSnow:
  SetWind(0);
  if (Local(2) == 1) Call("Snow1");
  if (Local(2) == 2) Call("Snow2");
  if (Local(2) == 3) Call("Snow3");
  if (Local(2) == 4) Call("Snow4");
  return(1);

public Snow1:
  CastPXS("Snow",4,0,RandomX(0,Local(92)),Local(93));
  CastPXS("Snow",4,0,RandomX(0,Local(92)),Local(93));
  CastPXS("Snow",4,0,RandomX(0,Local(92)),Local(93));
  CastPXS("Snow",4,0,RandomX(0,Local(92)),Local(93));
  CastPXS("Snow",4,0,RandomX(0,Local(92)),Local(93));
  CastPXS("Snow",4,0,RandomX(0,Local(92)),Local(93));
  CastPXS("Snow",4,0,RandomX(0,Local(92)),Local(93));
  CastPXS("Snow",4,0,RandomX(0,Local(92)),Local(93));
  if (Local(90) == 29 ) SetTemperature(-20);
  return(1);

public Snow2:
  CastPXS("Snow",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Local(92),Mul(2,Local(92))),Local(94)),Local(93));
  if (Local(90) == 29 ) SetTemperature(-20);
  return(1);

public Snow3:
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  if (Local(90) == 29 ) SetTemperature(-20);
  return(1);

public Snow4:
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  CastPXS("Snow",4,0,Sub(RandomX(Mul(3,Local(92)),Mul(4,Local(92))),Local(94)),Local(93));
  if (Local(90) == 29 ) SetTemperature(-20);
  return(1);

public ESnow:
  SetTemperature(Local(3));
  SetLocal(1,0);
  SetLocal(2,0);
  return(1);

public Storm:
  if (Local(2) == 0) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Local(88) == 0 , Not(ContentsCount(METL,this()) >= 7 ))) return(1);
  if (And(Local(88) == 1 , Not(ContentsCount(CRYS,this()) >= 10))) return(1);
  SetLocal(1,5);
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,0,-75));
  ObjectCall(Local(89),"Start");
  return(1);

public QStorm:
  if (Local(2) == 1) Call("Storm1");
  if (Local(2) == 2) Call("Storm2");
  if (Local(2) == 3) Call("Storm3");
  if (Local(2) == 4) Call("Storm4");
  return(1);

public Storm1:
  if (Not(Random(120))) LaunchLightning(RandomX(0,Div(LandscapeWidth(),4)), 0, -20, 41, +5, 15);
  return(1);

public Storm2:
  if (Not(Random(120))) LaunchLightning(RandomX(Div(LandscapeWidth(),4),Div(LandscapeWidth(),2)), 0, -20, 41, +5, 15);
  return(1);

public Storm3:
  if (Not(Random(120))) LaunchLightning(RandomX(Div(LandscapeWidth(),2),Mul(3,Div(LandscapeWidth(),4))), 0, -20, 41, +5, 15);
  return(1);

public Storm4:
  if (Not(Random(120))) LaunchLightning(RandomX(Mul(3,Div(LandscapeWidth(),4)),Div(LandscapeWidth(),4)), 0, -20, 41, +5, 15);
  return(1);

public EStorm:
  SetLocal(1,0);
  SetLocal(2,0);
  return(1);

public Volcano:
  if (Local(2) == 0) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Local(88) == 0 , Not(ContentsCount(METL,this()) >= 12 ))) return(1);
  if (And(Local(88) == 1 , Not(ContentsCount(CRYS,this()) >= 17 ))) return(1);
  SetLocal(1,6);
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,0,-75));
  ObjectCall(Local(89),"Start");
  return(1);

public QVolcano:
  if (Local(2) == 1) Call("Volcano1");
  if (Local(2) == 2) Call("Volcano2");
  if (Local(2) == 3) Call("Volcano3");
  if (Local(2) == 4) Call("Volcano4");
  return(1);

public Volcano1:
  if (Not(Random(400))) LaunchVolcano(RandomX(0,Div(LandscapeWidth(),4)));
  return(1);

public Volcano2:
  if (Not(Random(400))) LaunchVolcano(RandomX(Div(LandscapeWidth(),4),Div(LandscapeWidth(),2)));
  return(1);

public Volcano3:
  if (Not(Random(400))) LaunchVolcano(RandomX(Div(LandscapeWidth(),2),Mul(3,Div(LandscapeWidth(),4))));
  return(1);

public Volcano4:
  if (Not(Random(400))) LaunchVolcano(RandomX(Mul(3,Div(LandscapeWidth(),4)),LandscapeWidth()));
  return(1);

public EVolcano:
  SetLocal(1,0);
  SetLocal(2,0);
  return(1);

public Earthquake:
  if (Local(2) == 0) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Local(88) == 0 , Not(ContentsCount(METL,this()) >= 4 ))) return(1);
  if (And(Local(88) == 1 , Not(ContentsCount(CRYS,this()) >= 6 ))) return(1);
  SetLocal(1,7);
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,0,-75));
  ObjectCall(Local(89),"Start");
  return(1);

public QEarthquake:
  if (Local(2) == 1) Call("Earthquake1");
  if (Local(2) == 2) Call("Earthquake2");
  if (Local(2) == 3) Call("Earthquake3");
  if (Local(2) == 4) Call("Earthquake4");
  return(1);

public Earthquake1:
  if (Not(Random(120))) LaunchEarthquake(RandomX(0,Div(LandscapeWidth(),4)),Random(LandscapeHeight()));
  return(1);

public Earthquake2:
  if (Not(Random(120))) LaunchEarthquake(RandomX(Div(LandscapeWidth(),4),Div(LandscapeWidth(),2)),Random(LandscapeHeight()));
  return(1);

public Earthquake3:
  if (Not(Random(120))) LaunchEarthquake(RandomX(Div(LandscapeWidth(),2),Mul(3,Div(LandscapeWidth(),4))),Random(LandscapeHeight()));
  return(1);

public Earthquake4:
  if (Not(Random(120))) LaunchEarthquake(RandomX(Mul(3,Div(LandscapeWidth(),4)),LandscapeWidth()),Random(LandscapeHeight()));
  return(1);

public EEarthquake:
  SetLocal(1,0);
  SetLocal(2,0);
  return(1);

public MeteorShower:
  if (Local(2) == 0) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Local(88) == 0 , Not(ContentsCount(METL,this()) >= 8 ))) return(1);
  if (And(Local(88) == 1 , Not(ContentsCount(CRYS,this()) >= 11))) return(1);
  SetLocal(1,8);
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,0,-75));
  ObjectCall(Local(89),"Start");
  return(1);

public QMeteorShower:
  if (Local(2) == 1) Call("MeteorShower1");
  if (Local(2) == 2) Call("MeteorShower2");
  if (Local(2) == 3) Call("MeteorShower3");
  if (Local(2) == 4) Call("MeteorShower4");
  return(1);

public MeteorShower1:
  if (Not(Random(90))) CastObjects(METO,1,20,Sub(RandomX(0,Div(LandscapeWidth(),4)),GetX()),Sub(-1,GetY()));
  return(1);

public MeteorShower2:
  if (Not(Random(90))) CastObjects(METO,1,20,Sub(RandomX(Div(LandscapeWidth(),4),Div(LandscapeWidth(),2)),GetX()),Sub(-1,GetY()));
  return(1);

public MeteorShower3:
  if (Not(Random(90))) CastObjects(METO,1,20,Sub(RandomX(Div(LandscapeWidth(),2),Mul(3,Div(LandscapeWidth(),4))),GetX()),Sub(-1,GetY()));
  return(1);

public MeteorShower4:
  if (Not(Random(90))) CastObjects(METO,1,20,Sub(RandomX(Mul(3,Div(LandscapeWidth(),4)),LandscapeWidth()),GetX()),Sub(-1,GetY()));
  return(1);

public EMeteorShower:
  SetLocal(1,0);
  SetLocal(2,0);
  return(1);

public FlameShower:
  if (Local(2) == 0) return(Message("Erst Aktionsgebiet auswählen"));
  if (Not(SEqual(GetAction(),"Idle"))) return(1);
  if (And(Local(88) == 0 , Not(ContentsCount(METL,this()) >= 10 ))) return(1);
  if (And(Local(88) == 1 , Not(ContentsCount(CRYS,this()) >= 14 ))) return(1);
  SetLocal(1,9);
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 1 ) RemoveObject(FindContents(CRYS));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  if (Local(88) == 0 ) RemoveObject(FindContents(METL));
  SetAction("StartSequence");
  SetLocal(89,CreateObject(STAT,0,-75));
  ObjectCall(Local(89),"Start");
  return(1);

public QFlameShower:
  if (Local(2) == 1) Call("FlameShower1");
  if (Local(2) == 2) Call("FlameShower2");
  if (Local(2) == 3) Call("FlameShower3");
  if (Local(2) == 4) Call("FlameShower4");
  return(1);

protected FlameShower1:
  if (Not(Random(8))) CastObjects(DFLM,1,0,Sub(RandomX(0,Div(LandscapeWidth(),4)),GetX()),Sub(-1,GetY()));
  return(1);

public FlameShower2:
  if (Not(Random(8))) CastObjects(DFLM,1,0,Sub(RandomX(Div(LandscapeWidth(),4),Div(LandscapeWidth(),2)),GetX()),Sub(-1,GetY()));
  return(1);

public FlameShower3:
  if (Not(Random(8))) CastObjects(DFLM,1,0,Sub(RandomX(Div(LandscapeWidth(),2),Mul(3,Div(LandscapeWidth(),4))),GetX()),Sub(-1,GetY()));
  return(1);

public FlameShower4:
  if (Not(Random(8))) CastObjects(DFLM,1,0,Sub(RandomX(Mul(3,Div(LandscapeWidth(),4)),LandscapeWidth()),GetX()),Sub(-1,GetY()));
  return(1);

public EFlameShower:
  SetLocal(1,0);
  SetLocal(2,0);
  return(1);