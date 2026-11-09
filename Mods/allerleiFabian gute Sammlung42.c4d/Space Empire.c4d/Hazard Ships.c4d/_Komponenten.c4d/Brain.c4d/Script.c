/*-- Brain --*/

#strict

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
  movtimer=0;
  //Ziel auf Null setzen
  target=0;  
  //Target-Timer auf Null setzen
  tartimer=0;
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
  if(type==1)
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
  if(type==2)
  {
  vessel->TurretOwner(GetOwner());
  srange=5;
  turnrate=5;
  splitfire=10;
  firerange=500;
  DoAmmo(ENAM,200,this());
  }
  
  //Moloch
  if(type==3)
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
  sum=0;

  if(SensorMMR()) sum+=2;
  if(SensorMML()) sum-=2;
  if(SensorMR()) sum+=2;
  if(SensorML()) sum-=2;
  if(SensorR()) sum+=1;
  if(SensorR()) sum-=1;
  if(!sum && SensorM()) SteerRandom(2);
  if(sum<0) SteerRight(2);
  if(sum>0) SteerLeft(2);
  
  if(IdentifyShip(GetID(target)) && !IdentifyFighter(GetID(target)) && Distance(GetX(),GetY(),GetX(target),GetY(target))<250) SteerRandom(3);
  
  //Kartengrenzen-Check
  //Oben
  if(GetY()<=(speed*srange) && (angle>-90 && angle<0)) SteerLeft(1);
  if(GetY()<=(speed*srange) && (angle>-0 && angle<90)) SteerRight(1);
  if(GetY()<=(speed*srange) && (!angle)) SteerRandom(1);
  //Unten
  if(GetY()>=(LandscapeHeight()-speed*3) && (angle>90 && angle<180)) SteerLeft(1);
  if(GetY()>=(LandscapeHeight()-speed*3) && (angle>-180 && angle<-90)) SteerRight(1);
  if(GetY()>=(LandscapeHeight()-speed*3) && (angle==180)) SteerRandom(1);
  //Links
  if(GetX()<=(speed*srange) && (angle>-180 && angle<-90)) SteerLeft(1);
  if(GetX()<=(speed*srange) && (angle>-90 && angle<0))   SteerRight(1);
  if(GetX()<=(speed*srange) && (angle==-90)) SteerRandom(1);
  //Rechts
  if(GetX()>=(LandscapeWidth()-speed*3) && (angle>0 && angle<90)) SteerLeft(1);
  if(GetX()>=(LandscapeWidth()-speed*3) && (angle>90 && angle<180)) SteerRight(1);
  if(GetX()>=(LandscapeWidth()-speed*3) && (angle==90)) SteerRandom(1);
  //Ecken
  if(SensorCorner() ==1) SteerLeft(-1); 
  if(SensorCorner() ==2) SteerRight(-1); 
  
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
  movtimer++;
  if (movtimer>30 && !SensorCorner() && !sum)
  {
  movtimer=0;
  movpriority=5;
  SetRDir(0,vessel);
  }
  
  //Zielfindung
  //Feind aller Spieler
  tartimer++;
  if (tartimer>80)
  {
  tartimer=0;
  target=FindTarget();
  }
  
  //Faktor X
  
  if(!Random(250) && (type==1 || type==3) && movpriority>3 && !sum) SteerRandom(3);
  if(!Random(100) && type==2)
  {
  var tarned, energized;
  vessel->Tarned(tarned,energized);
  if(!tarned && energized)  vessel->Tarnung();
  }
  
  // Zur Zielverfolgung übergehen
  var iDiff = Normalize(rtarget - angle,-180);
  if(movpriority==5 && target)
  {
  rtarget=Angle(GetX(),GetY(),GetX(target),GetY(target));
  var iTurn = Min(Abs(iDiff),turnrate);
  if(Inside(angle,-5,+5) && iDiff>0)
  {
  SteerRight(4);
  movtimer=25;
  }
  if(Inside(angle,-5,+5) && iDiff<0)
  {
  SteerLeft(4);
  movtimer=25;
  }
  if(!Inside(angle,-5,+5)) SetR(angle+iTurn*((iDiff > 0)*2-1),vessel);
  }
  
  //Waffenwahl
  //Predator
  if(type==1)
  {
  var ammo1=GetAmmo(STAM)+GetAmmo(STAM,weapon);
  var ammo2=GetAmmo(MIAM)+GetAmmo(MIAM,weapon);
  var ammoweapon=GetAmmo(STAM,weapon)+GetAmmo(MIAM,weapon);
  if(!ammo2) {saboteur=0; if(firemode!=1) Mode(1);} // wenn keine raketen mehr, dann keine gebädue mehr angreifen
  if(!saboteur)
    {
  if(firemode==1 && ammo2) if(IdentifyShip(GetID(target)))  Mode(5); // normale raketen auf Ships feuern
  if(firemode==5 && ammo1) if(!IdentifyShip(GetID(target))) Mode(1);
  if(firemode==5 && !ammo2 && ammo1) Mode(1);
  if(firemode==1 && !ammo1 && ammo2) Mode(5);
    }
  if(saboteur)
      {
      if(firemode!=4) Mode(4);  // verschießt Raketen auf Militärgebäude
      }
  }
  
  //Scorpion
  if(type==2)
  {
  var ammo1=GetAmmo(ENAM)+GetAmmo(ENAM,weapon);
  var ammo2=1;
  var ammoweapon=GetAmmo(ENAM,weapon);
  if(!ammo1) ammo2=0;
  if(!saboteur)
    {
  if(firemode==2) if(IdentifyShip(GetID(target)))  {  Mode(1);  splitfire=10;  }
  if(firemode==1) if(!IdentifyShip(GetID(target)))  {  Mode(2);  splitfire=150;  }
    }
  if(saboteur)
      {
      if(firemode!=4) {Mode(4); splitfire=150;}
      }
  }
  
  //Moloch
  if(type==3)
  {
  var ammo1=GetAmmo(STAM)+GetAmmo(STAM,weapon);
  var ammo2=GetAmmo(MIAM)+GetAmmo(MIAM,weapon);
  var ammoweapon=GetAmmo(STAM,weapon)+GetAmmo(MIAM,weapon);
  if(firemode==1 && ammo2) Mode(2);
  if(firemode==2 && !ammo2) Mode(1);
  }
  
  //Beschuss einleiten
  //Keine Muni mehr...dann eben rammen...
  if(!(ammo1+ammo2))
  {
  kamikaze=1;
  return(1);
  }  
  if(!ammoweapon)
  {
  firestate=0;
  vessel->ContainedThrow();
  return(1);
  }
  if(target && Distance(GetX(),GetY(),GetX(target),GetY(target))<firerange && (PathFree(GetX(),GetY(),GetX(target),GetY(target)) || IdentifyShip(GetID(target))) && Inside(iDiff,-splitfire,+splitfire))
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
        if(GetOCF(pT) & OCF_Living && !(GetOCF(pT) & OCF_Alive)) continue; //wenn totes Lebewesen, kein Ziel
	if(!(GetOwner(this())==-1) && (!Hostile(GetOwner(this()),GetOwner(pT)) && !(GetOwner(pT)==-1))) continue;
	if(GetOwner(this())==-1 && GetOwner(pT)==-1) continue;
	if(IdentifyFighter(GetID(pT))) if(pT->Tarned()) continue;
	//if(Contained(pT)) pT=Contained(pT);
    return(pT);
  }
  if(saboteur)
  for(pT in FindObjects( Find_NoContainer(),Find_Func("IsMilitaryBuilding") )) 
  {
	// Verfeindet
	if(!(GetOwner(this())==-1) && (!Hostile(GetOwner(this()),GetOwner(pT)) && !(GetOwner(pT)==-1))) continue;
	if(GetOwner(this())==-1 && GetOwner(pT)==-1) continue;
	if(IdentifyFighter(GetID(pT))) if(pT->Tarned()) continue;
    return(pT);
  }
}
  
  //Waffen
  func WeaponBegin() { return(0); }
  func WeaponEnd() { return(0); }
  
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
  firestate=0;
  vessel->ContainedThrow(); 
  }
  }
 
  func Mode(mode)  {
  if(firestate)
  {
  vessel->ContainedThrow();
  firestate=0;
  }
  weapon->SetFireMode(mode);
  firemode=mode;
  return(1);
  }
  
  //Kollisions-Sensoren
  func SensorM()  {
  var x,y;
  x=Sin(angle,srange*speed);
  y=Cos(angle,srange*speed);
  if (!PathFree(GetX(), GetY(), GetX()-x, GetY()-y)) return(1);
  if (type==1 && GBackLiquid(Sin(angle,-srange*speed), Cos(angle,-srange*speed))) return(1);
  return(0);
  }
 
  func SensorMML()  {
  var x,y;
  x=Sin(angle-10,srange*speed);
  y=Cos(angle-10,srange*speed);
  if (!PathFree(GetX(), GetY(), GetX()-x, GetY()-y)) return(1);
  if (type==1 && GBackLiquid(Sin(angle,-srange*speed), Cos(angle,-srange*speed))) return(1);
  return(0);
  }
 
  func SensorML()  {
  var x,y;
  x=Sin(angle-30,srange*speed);
  y=Cos(angle-30,srange*speed);
  if (!PathFree(GetX(), GetY(), GetX()-x, GetY()-y)) return(1);
  if (type==1 && GBackLiquid(Sin(angle,-srange*speed), Cos(angle,-srange*speed))) return(1);
  return(0);
  }
  
  func SensorL()  {
  var x,y;
  x=Sin(angle-60,srange*speed);
  y=Cos(angle-60,srange*speed);
  if (!PathFree(GetX(), GetY(), GetX()-x, GetY()-y)) return(1);
  if (type==1 && GBackLiquid(Sin(angle,-srange*speed), Cos(angle,-srange*speed))) return(1);
  return(0);
  }
  
  func SensorMMR()  {
  var x,y;
  x=Sin(angle+10,srange*speed);
  y=Cos(angle+10,srange*speed);
  if (!PathFree(GetX(), GetY(), GetX()-x, GetY()-y)) return(1);
  if (type==1 && GBackLiquid(Sin(angle,-srange*speed), Cos(angle,-srange*speed))) return(1);
  return(0);
  }
  
  func SensorMR()  {
  var x,y;
  x=Sin(angle+30,srange*speed);
  y=Cos(angle+30,srange*speed);
  if (!PathFree(GetX(), GetY(), GetX()-x, GetY()-y)) return(1);
  if (type==1 && GBackLiquid(Sin(angle,-srange*speed), Cos(angle,-srange*speed))) return(1);
  return(0);
  }
  
  func SensorR()  {
  var x,y;
  x=Sin(angle+60,srange*speed);
  y=Cos(angle+60,srange*speed);
  if (!PathFree(GetX(), GetY(), GetX()-x, GetY()-y)) return(1);
  if (type==1 && GBackLiquid(Sin(angle,-srange*speed), Cos(angle,-srange*speed))) return(1);
  return(0);
  }
  
  //Ecksensor
  func SensorCorner()   {
  //1=Links
  //2=Rechts
  if (Distance(0,0,GetX(),GetY()) < srange*speed)
  {
  if(GetX()<=GetY()) return(1);
  return(2);
  }
  if (Distance(0,LandscapeHeight(),GetX(),GetY()) < srange*speed)
  {
  if(GetX()>=LandscapeHeight()-GetY()) return(2);
  return(1);
  }
  if (Distance(LandscapeWidth(),0,GetX(),GetY()) < srange*speed)
  {
  if(LandscapeWidth()-GetX()<=GetY()) return(2);
  return(1);
  }
  if (Distance(LandscapeWidth(),LandscapeHeight(),GetX(),GetY()) < srange*speed)
  {
  if(LandscapeWidth()-GetX()<=LandscapeHeight()-GetY()) return(1);
  return(2);
  }
  return(0);
  }
 
  //Steuermethoden
  func SteerRandom(_priority)  {
  if (movpriority<=_priority) return(0);
  if (kamikaze && _priority==2) return(0);
  movpriority=_priority;
  if (Random(1)) return(SteerLeft());
  return(SteerRight());
  }
  
  func SteerRight(_priority)  {
  if (movpriority<=_priority) return(0);
  if (kamikaze && _priority==2) return(0);
  movpriority=_priority;
  vessel->ContainedRight();
  return(1);
  }
  
  func SteerLeft(_priority)  {
  if (movpriority<=_priority) return(0);
  if (kamikaze && _priority==2) return(0);
  movpriority=_priority;
  vessel->ContainedLeft();
  return(1);
  }
  
  //Winkelarithmetik
  global func Normalize(int iAngle, int iStart) {
  var iEnd = 360+iStart;
  
  while(iAngle<iStart) iAngle += 360;
  while(iAngle>=iEnd) iAngle -= 360;

  return(iAngle);
}

  //Das Ende
  func Killed()  {
  GameCall("Braindead");
  RemoveObject();
  }