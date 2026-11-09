/*-- Brain --*/

#strict 3

  //Lokale Variablen
  local vessel,ammobag,weapon,type,speed,angle,srange,movpriority,movtimer,rtarget,target,tartimer,turnrate,firemode,firestate,splitfire,firerange,missletimer,kamikaze,saboteur;

public func ReadyToFire()   { return(true); }

func Initialize() {
  //Startvertögerung beginnen
  SetAction("Startdelay"); 
  ammobag = CreateObject(ABAG);
  return(1);
}

func Starter()  {
  //Schiff festlegen
  vessel=Contained();
  //Besitz festlegen
  SetOwner(GetOwner(vessel));
  //Schiffstyp feststellen
  type=vessel->Typ();
  saboteur=vessel->~Saboteurjaeger(); // Predator und Scorpion greifen dann Militärgebäude an
//1=Predator
  //2=Scorpion
  //3=Predator
  //Brain als Pilot festlegen
  vessel->Brain(this());
  //Schiffsabhängige Daten Setzen
  Shipdata();
  //Movement-Priority auf 5 "unwichtig" Setzen
  movpriority=5;
  //Movement-Timer auf Null setzen
  movtimer=nil;
  //Ziel auf Null setzen
  target=nil;
  //Target-Timer auf Null setzen
  tartimer=nil;
  //Waffensystem aktivieren & Durchladen
  vessel->ContainedThrow();
  weapon=vessel->Waffe();
  firemode=1;
  //Starten
  vessel->Triebwerk();
  SetAction("Control");
  }
  
  func Shipdata()  {
  //Predator
  if(Equal(type, 1))
  {
  //Sensorreichweite
  srange=5;
  //Drehgeschwindigkeit
  turnrate=4;
  //Feuerbereich
  splitfire=20;
  //Reichweite
  firerange=500;
  //Munition
  DoAmmo(STAM,100,this());
  DoAmmo(MIAM,2,this());
  if(saboteur) DoAmmo(MIAM,8,this());
  }
  
  //Scorpion
  if(Equal(type, 2))
  {
  vessel->TurretOwner(GetOwner());
  srange=5;
  turnrate=5;
  splitfire=10;
  firerange=500;
  DoAmmo(ENAM,200,this());
  }
  
  //Moloch
  if(Equal(type, 3))
  {
  srange=6;
  turnrate=4;
  splitfire=4;
  firerange=500;
  DoAmmo(STAM,200,this());
  DoAmmo(MIAM,2,this());
  }
  return(1);
  }
  
  func Check() {
  //Bewegungsgrößen beziehen
  speed=vessel->Speed();
  angle=GetR(vessel);
  //Kollisions-Check
  var sum;
  sum=nil;

  if(SensorMMR()) S42MigrationAssign(sum, "+=", 2);
  if(SensorMML()) S42MigrationAssign(sum, "-=", 2);
  if(SensorMR()) S42MigrationAssign(sum, "+=", 2);
  if(SensorML()) S42MigrationAssign(sum, "-=", 2);
  if(SensorR()) S42MigrationAssign(sum, "+=", 1);
  if(SensorR()) S42MigrationAssign(sum, "-=", 1);
  if(And(!sum, SensorM())) SteerRandom(2);
  if((sum ?? 0)<0) SteerRight(2);
  if((sum ?? 0)>0) SteerLeft(2);
  
  if(And(And(IdentifyShip(GetID(target)), !IdentifyFighter(GetID(target))), (Distance(GetX(),GetY(),GetX(target),GetY(target)) ?? 0)<250)) SteerRandom(3);
  
  //Kartengrenzen-Check
  //Oben
  if(And((GetY() ?? 0)<=((speed ?? 0)*(srange ?? 0)), (And((angle ?? 0)>-90, (angle ?? 0)<0)))) SteerLeft(1);
  if(And((GetY() ?? 0)<=((speed ?? 0)*(srange ?? 0)), (And((angle ?? 0)>-0, (angle ?? 0)<90)))) SteerRight(1);
  if(And((GetY() ?? 0)<=((speed ?? 0)*(srange ?? 0)), (!angle))) SteerRandom(1);
  //Unten
  if(And((GetY() ?? 0)>=((LandscapeHeight() ?? 0)-(speed ?? 0)*3), (And((angle ?? 0)>90, (angle ?? 0)<180)))) SteerLeft(1);
  if(And((GetY() ?? 0)>=((LandscapeHeight() ?? 0)-(speed ?? 0)*3), (And((angle ?? 0)>-180, (angle ?? 0)<-90)))) SteerRight(1);
  if(And((GetY() ?? 0)>=((LandscapeHeight() ?? 0)-(speed ?? 0)*3), (Equal(angle, 180)))) SteerRandom(1);
  //Links
  if(And((GetX() ?? 0)<=((speed ?? 0)*(srange ?? 0)), (And((angle ?? 0)>-180, (angle ?? 0)<-90)))) SteerLeft(1);
  if(And((GetX() ?? 0)<=((speed ?? 0)*(srange ?? 0)), (And((angle ?? 0)>-90, (angle ?? 0)<0))))   SteerRight(1);
  if(And((GetX() ?? 0)<=((speed ?? 0)*(srange ?? 0)), (Equal(angle, -90)))) SteerRandom(1);
  //Rechts
  if(And((GetX() ?? 0)>=((LandscapeWidth() ?? 0)-(speed ?? 0)*3), (And((angle ?? 0)>0, (angle ?? 0)<90)))) SteerLeft(1);
  if(And((GetX() ?? 0)>=((LandscapeWidth() ?? 0)-(speed ?? 0)*3), (And((angle ?? 0)>90, (angle ?? 0)<180)))) SteerRight(1);
  if(And((GetX() ?? 0)>=((LandscapeWidth() ?? 0)-(speed ?? 0)*3), (Equal(angle, 90)))) SteerRandom(1);
  //Ecken
  if(Equal(SensorCorner(), 1)) SteerLeft(-1);
  if(Equal(SensorCorner(), 2)) SteerRight(-1);
  
  /*
	Maneuverprioritäten:
	-1 -- Ecke...ausweichen
	0  -- Startpriorität...sorgt für kleines delay
	1  -- Kartenrand...ausweichen
	2 -- Hindernis...ausweichen
	3 -- Zufallsbewegungen
	4 -- Zielverfolgung
	5 -- Kein Maneuver gestartet
     */
	 
  //Maneuver beendet.
  S42MigrationStep(movtimer, 1, true);
  if (And(And((movtimer ?? 0)>30, !SensorCorner()), !sum))
  {
  movtimer=nil;
  movpriority=5;
  SetRDir(nil,vessel);
  }
  
  //Zielfindung
  //Feind aller Spieler
  S42MigrationStep(tartimer, 1, true);
  if ((tartimer ?? 0)>80)
  {
  tartimer=nil;
  target=FindTarget();
  }
  
  //Faktor X
  
  if(And(And(And(!Random(250), (Or(Equal(type, 1), Equal(type, 3)))), (movpriority ?? 0)>3), !sum)) SteerRandom(3);
  if(And(!Random(100), Equal(type, 2)))
  {
  var tarned, energized;
  vessel->Tarned(tarned,energized);
  if(And(!tarned, energized))  vessel->Tarnung();
  }
  
  // Zur Zielverfolgung übergehen
  var iDiff = Normalize((rtarget ?? 0) - (angle ?? 0),-180);
  if(And(Equal(movpriority, 5), target))
  {
  rtarget=Angle(GetX(),GetY(),GetX(target),GetY(target));
  var iTurn = Min(Abs(iDiff),turnrate);
  if(And(Inside(angle,-5,+5), (iDiff ?? 0)>0))
  {
  SteerRight(4);
  movtimer=25;
  }
  if(And(Inside(angle,-5,+5), (iDiff ?? 0)<0))
  {
  SteerLeft(4);
  movtimer=25;
  }
  if(!Inside(angle,-5,+5)) SetR((angle ?? 0)+(iTurn ?? 0)*((((iDiff ?? 0) > 0) ?? 0)*2-1),vessel);
  }
  
  //Waffenwahl
  //Predator
  if(Equal(type, 1))
  {
  var ammo1=(GetAmmo(STAM) ?? 0)+(GetAmmo(STAM,weapon) ?? 0);
  var ammo2=(GetAmmo(MIAM) ?? 0)+(GetAmmo(MIAM,weapon) ?? 0);
  var ammoweapon=(GetAmmo(STAM,weapon) ?? 0)+(GetAmmo(MIAM,weapon) ?? 0);
  if(!ammo2) {saboteur=nil; if(!Equal(firemode, 1)) Mode(1);} // wenn keine raketen mehr, dann keine gebädue mehr angreifen
  if(!saboteur)
    {
  if(And(Equal(firemode, 1), ammo2)) if(IdentifyShip(GetID(target)))  Mode(5); // normale raketen auf Ships feuern
  if(And(Equal(firemode, 5), ammo1)) if(!IdentifyShip(GetID(target))) Mode(1);
  if(And(And(Equal(firemode, 5), !ammo2), ammo1)) Mode(1);
  if(And(And(Equal(firemode, 1), !ammo1), ammo2)) Mode(5);
    }
  if(saboteur)
      {
      if(!Equal(firemode, 4)) Mode(4);  // verschießt Raketen auf Militärgebäude
      }
  }
  
  //Scorpion
  if(Equal(type, 2))
  {
  var ammo1=(GetAmmo(ENAM) ?? 0)+(GetAmmo(ENAM,weapon) ?? 0);
  var ammo2=1;
  var ammoweapon=GetAmmo(ENAM,weapon);
  if(!ammo1) ammo2=nil;
  if(!saboteur)
    {
  if(Equal(firemode, 2)) if(IdentifyShip(GetID(target)))  {  Mode(1);  splitfire=10;  }
  if(Equal(firemode, 1)) if(!IdentifyShip(GetID(target)))  {  Mode(2);  splitfire=150;  }
    }
  if(saboteur)
      {
      if(!Equal(firemode, 4)) {Mode(4); splitfire=150;}
      }
  }
  
  //Moloch
  if(Equal(type, 3))
  {
  var ammo1=(GetAmmo(STAM) ?? 0)+(GetAmmo(STAM,weapon) ?? 0);
  var ammo2=(GetAmmo(MIAM) ?? 0)+(GetAmmo(MIAM,weapon) ?? 0);
  var ammoweapon=(GetAmmo(STAM,weapon) ?? 0)+(GetAmmo(MIAM,weapon) ?? 0);
  if(And(Equal(firemode, 1), ammo2)) Mode(2);
  if(And(Equal(firemode, 2), !ammo2)) Mode(1);
  }
  
  //Beschuss einleiten
  //Keine Muni mehr...dann eben rammen...
  if(!((ammo1 ?? 0)+(ammo2 ?? 0)))
  {
  kamikaze=1;
  return(1);
  }  
  if(!ammoweapon)
  {
  firestate=nil;
  vessel->ContainedThrow();
  return(1);
  }
  if(And(And(And(target, (Distance(GetX(),GetY(),GetX(target),GetY(target)) ?? 0)<(firerange ?? 0)), (Or(PathFree(GetX(),GetY(),GetX(target),GetY(target)), IdentifyShip(GetID(target))))), Inside(iDiff,-(splitfire ?? 0),+(splitfire ?? 0))))
  {
  Fire();
  }
  else
  {
  Ceasefire();
  }
  return(1);
  }
  
  //Zielsuche
  
private func FindTarget() {
  var pT;
  if(!saboteur)
  for(pT in FindObjects( Find_NoContainer(),Find_Or(Find_OCF(OCF_Prey),Find_Func("IsHazardShip")) )) 
  {
	// Verfeindet
        if(And((GetOCF(pT) ?? 0) & (OCF_Living ?? 0), !((GetOCF(pT) ?? 0) & (OCF_Alive ?? 0)))) continue; //wenn totes Lebewesen, kein Ziel
	if(And(!(Equal(GetOwner(this()), -1)), (And(!Hostile(GetOwner(this()),GetOwner(pT)), !(Equal(GetOwner(pT), -1)))))) continue;
	if(And(Equal(GetOwner(this()), -1), Equal(GetOwner(pT), -1))) continue;
	if(IdentifyFighter(GetID(pT))) if(pT->Tarned()) continue;
	//if(Contained(pT)) pT=Contained(pT);
    return(pT);
  }
  if(saboteur)
  for(pT in FindObjects( Find_NoContainer(),Find_Func("IsMilitaryBuilding") )) 
  {
	// Verfeindet
	if(And(!(Equal(GetOwner(this()), -1)), (And(!Hostile(GetOwner(this()),GetOwner(pT)), !(Equal(GetOwner(pT), -1)))))) continue;
	if(And(Equal(GetOwner(this()), -1), Equal(GetOwner(pT), -1))) continue;
	if(IdentifyFighter(GetID(pT))) if(pT->Tarned()) continue;
    return(pT);
  }
}
  
  //Waffen
  func WeaponBegin() { return(nil); }
  func WeaponEnd() { return(nil); }
  
  func Fire()
  {
  if(!firestate)
  {
  firestate=1;
  vessel->ContainedThrow();   
  }
  }
  
  func Ceasefire()
  {
  if(firestate)
  {
  firestate=nil;
  vessel->ContainedThrow(); 
  }
  }
 
  func Mode(mode)  {
  if(firestate)
  {
  vessel->ContainedThrow();
  firestate=nil;
  }
  weapon->SetFireMode(mode);
  firemode=mode;
  return(1);
  }
  
  //Kollisions-Sensoren
  func SensorM()  {
  var x,y;
  x=Sin(angle,(srange ?? 0)*(speed ?? 0));
  y=Cos(angle,(srange ?? 0)*(speed ?? 0));
  if (!PathFree(GetX(), GetY(), (GetX() ?? 0)-(x ?? 0), (GetY() ?? 0)-(y ?? 0))) return(1);
  if (And(Equal(type, 1), GBackLiquid(Sin(angle,-(srange ?? 0)*(speed ?? 0)), Cos(angle,-(srange ?? 0)*(speed ?? 0))))) return(1);
  return(nil);
  }
 
  func SensorMML()  {
  var x,y;
  x=Sin((angle ?? 0)-10,(srange ?? 0)*(speed ?? 0));
  y=Cos((angle ?? 0)-10,(srange ?? 0)*(speed ?? 0));
  if (!PathFree(GetX(), GetY(), (GetX() ?? 0)-(x ?? 0), (GetY() ?? 0)-(y ?? 0))) return(1);
  if (And(Equal(type, 1), GBackLiquid(Sin(angle,-(srange ?? 0)*(speed ?? 0)), Cos(angle,-(srange ?? 0)*(speed ?? 0))))) return(1);
  return(nil);
  }
 
  func SensorML()  {
  var x,y;
  x=Sin((angle ?? 0)-30,(srange ?? 0)*(speed ?? 0));
  y=Cos((angle ?? 0)-30,(srange ?? 0)*(speed ?? 0));
  if (!PathFree(GetX(), GetY(), (GetX() ?? 0)-(x ?? 0), (GetY() ?? 0)-(y ?? 0))) return(1);
  if (And(Equal(type, 1), GBackLiquid(Sin(angle,-(srange ?? 0)*(speed ?? 0)), Cos(angle,-(srange ?? 0)*(speed ?? 0))))) return(1);
  return(nil);
  }
  
  func SensorL()  {
  var x,y;
  x=Sin((angle ?? 0)-60,(srange ?? 0)*(speed ?? 0));
  y=Cos((angle ?? 0)-60,(srange ?? 0)*(speed ?? 0));
  if (!PathFree(GetX(), GetY(), (GetX() ?? 0)-(x ?? 0), (GetY() ?? 0)-(y ?? 0))) return(1);
  if (And(Equal(type, 1), GBackLiquid(Sin(angle,-(srange ?? 0)*(speed ?? 0)), Cos(angle,-(srange ?? 0)*(speed ?? 0))))) return(1);
  return(nil);
  }
  
  func SensorMMR()  {
  var x,y;
  x=Sin((angle ?? 0)+10,(srange ?? 0)*(speed ?? 0));
  y=Cos((angle ?? 0)+10,(srange ?? 0)*(speed ?? 0));
  if (!PathFree(GetX(), GetY(), (GetX() ?? 0)-(x ?? 0), (GetY() ?? 0)-(y ?? 0))) return(1);
  if (And(Equal(type, 1), GBackLiquid(Sin(angle,-(srange ?? 0)*(speed ?? 0)), Cos(angle,-(srange ?? 0)*(speed ?? 0))))) return(1);
  return(nil);
  }
  
  func SensorMR()  {
  var x,y;
  x=Sin((angle ?? 0)+30,(srange ?? 0)*(speed ?? 0));
  y=Cos((angle ?? 0)+30,(srange ?? 0)*(speed ?? 0));
  if (!PathFree(GetX(), GetY(), (GetX() ?? 0)-(x ?? 0), (GetY() ?? 0)-(y ?? 0))) return(1);
  if (And(Equal(type, 1), GBackLiquid(Sin(angle,-(srange ?? 0)*(speed ?? 0)), Cos(angle,-(srange ?? 0)*(speed ?? 0))))) return(1);
  return(nil);
  }
  
  func SensorR()  {
  var x,y;
  x=Sin((angle ?? 0)+60,(srange ?? 0)*(speed ?? 0));
  y=Cos((angle ?? 0)+60,(srange ?? 0)*(speed ?? 0));
  if (!PathFree(GetX(), GetY(), (GetX() ?? 0)-(x ?? 0), (GetY() ?? 0)-(y ?? 0))) return(1);
  if (And(Equal(type, 1), GBackLiquid(Sin(angle,-(srange ?? 0)*(speed ?? 0)), Cos(angle,-(srange ?? 0)*(speed ?? 0))))) return(1);
  return(nil);
  }
  
  //Ecksensor
  func SensorCorner()   {
  //1=Links
  //2=Rechts
  if ((Distance(nil,nil,GetX(),GetY()) ?? 0) < (srange ?? 0)*(speed ?? 0))
  {
  if((GetX() ?? 0)<=(GetY() ?? 0)) return(1);
  return(2);
  }
  if ((Distance(nil,LandscapeHeight(),GetX(),GetY()) ?? 0) < (srange ?? 0)*(speed ?? 0))
  {
  if((GetX() ?? 0)>=(LandscapeHeight() ?? 0)-(GetY() ?? 0)) return(2);
  return(1);
  }
  if ((Distance(LandscapeWidth(),nil,GetX(),GetY()) ?? 0) < (srange ?? 0)*(speed ?? 0))
  {
  if((LandscapeWidth() ?? 0)-(GetX() ?? 0)<=(GetY() ?? 0)) return(2);
  return(1);
  }
  if ((Distance(LandscapeWidth(),LandscapeHeight(),GetX(),GetY()) ?? 0) < (srange ?? 0)*(speed ?? 0))
  {
  if((LandscapeWidth() ?? 0)-(GetX() ?? 0)<=(LandscapeHeight() ?? 0)-(GetY() ?? 0)) return(1);
  return(2);
  }
  return(nil);
  }
 
  //Steuermethoden
  func SteerRandom(_priority)  {
  if ((movpriority ?? 0)<=(_priority ?? 0)) return(nil);
  if (And(kamikaze, Equal(_priority, 2))) return(nil);
  movpriority=_priority;
  if (Random(1)) return(SteerLeft());
  return(SteerRight());
  }
  
  func SteerRight(_priority)  {
  if ((movpriority ?? 0)<=(_priority ?? 0)) return(nil);
  if (And(kamikaze, Equal(_priority, 2))) return(nil);
  movpriority=_priority;
  vessel->ContainedRight();
  return(1);
  }
  
  func SteerLeft(_priority)  {
  if ((movpriority ?? 0)<=(_priority ?? 0)) return(nil);
  if (And(kamikaze, Equal(_priority, 2))) return(nil);
  movpriority=_priority;
  vessel->ContainedLeft();
  return(1);
  }
  
  //Winkelarithmetik
  global func Normalize(int iAngle, int iStart) {
  iAngle ??= 0;
  iStart ??= 0;
  var iEnd = 360+(iStart ?? 0);
  
  while((iAngle ?? 0)<(iStart ?? 0)) S42MigrationAssign(iAngle, "+=", 360);
  while((iAngle ?? 0)>=(iEnd ?? 0)) S42MigrationAssign(iAngle, "-=", 360);

  return(iAngle);
}

  //Das Ende
  func Killed()  {
  GameCall("Braindead");
  RemoveObject();
  }