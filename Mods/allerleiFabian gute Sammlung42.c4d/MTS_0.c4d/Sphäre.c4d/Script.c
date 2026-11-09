/*-- Magnetsphäre --*/

#strict 3

local yrichtung, xrichtung, leitx, leity, gleis, symbol, blockiert, treibstoff,
      insasse, tor, haltezeit, urhaltezeit, ladbar, flugsound, bwert,tank,lasthalt,bluba,nexthalt;

public func IsVehicleProduct()   { return(true); }
      
func Initialize() 
{
  //Schweben einleiten
  SetAction("Halt");
  leitx = nil;
  leity = nil;
  //Anfangsstrecke=1
  symbol = CreateObject(SY_1,nil,nil,GetOwner());
  ObjectSetAction(symbol,"Attaching",this());
  gleis = 1;
  bwert = 35; 
  if(FindObject(NOME))tank=nil;
  else tank=1;
  return(1);
}

func NextDock(obj)
{
nexthalt=obj;
}

//DRINNEN ODER DRAUßEN?
func FullCheck() 
{
 var tproz = (treibstoff ?? 0)*100 / 800;
 //Drinnen?
 //if(GetAction() S= "Schweben")ControlDig();
 if(!Equal(Contained(this()), nil))
   { 
    treibstoff = nil;
    if(symbol) return(RemoveObject(symbol));
    return(1);
   }
 //Keine Aktion?
 if(!!SEqual(GetAction(this()), "Idle")) SetAction("Halt");
 //Tankanzeige
 if(!FindObject(NOME))if (Equal(tank, 1)) ControlDig();
 //Symbol überm Kopp?
 if(symbol) return(1);
 //Sonst Neues erstellen
 if(Equal(gleis, 1)) symbol = CreateObject(SY_1,nil,nil,GetOwner());
 if(Equal(gleis, 2)) symbol = CreateObject(SY_2,nil,nil,GetOwner());
 if(Equal(gleis, 3)) symbol = CreateObject(SY_3,nil,nil,GetOwner());
 if(Equal(gleis, 4)) symbol = CreateObject(SY12,nil,nil,GetOwner());
 ObjectSetAction(symbol,"Attaching",this());
 
  return(1);
}



//STATUS
func ControlDig(PController)
{
  [Status|Image=ST_3|Condition=IsBuilt]
  //Statusbalken zeigen
  //Verfeindet?
 // if(Hostile(GetOwner(this()),GetOwner(PController))) return(Message("Feind an der Sphäre!",this()));
  var max = GetMaxDamage();
  //ShowInfoBar(SBQ0, this(), max-GetDamage(), max, 5, 1);
  if(!FindObject(NOME))ShowInfoBar(IBRE, this(), treibstoff, 800, 2);
  //ShowInfoBar(IBRS, this(), max-GetDamage(),max, 2);
  return(1);
}

//NOTSTOP
func ControlDigDouble(PController)
{
  [Nothalt|Image=SYCC|Condition=IsBuilt]
  //Verfeindet?
  PController;
  if(Hostile(GetOwner(this()),GetOwner(PController))) return(Message("Feind an der Sphäre!",this()));
   
  treibstoff = nil;
  SetAction("Halt");
  blockiert = 120;
  return(1);
}


//STRECKENAUSWAHL
func ContainedDig(PController)
{
  [Steuerung|Image=ST_2|Condition=IsBuilt]
  //Verfeindet?
   if(Hostile(GetOwner(this()),GetOwner(PController))) return(Message("Feind in der Sphäre!",this()));
  
  //Streckenmenü öffnen
  CreateMenu(ST_1, PController, this(), nil, "Strecken konfigurieren", nil, nil, nil);
    AddMenuItem("Notbremse ziehen", "Notstop", SYCC, PController, nil, nil, " ");
    AddMenuItem("Sofort losfahren", "Los", SY_5, PController, nil, nil, " ");
    AddMenuItem("Strecke 1 wählen", "SetStrecke", SY_1, PController, nil, 1, " ");
    AddMenuItem("Strecke 2 wählen", "SetStrecke", SY_2, PController, nil, 2, " ");
    AddMenuItem("Strecke 3 wählen", "SetStrecke", SY_3, PController, nil, 3, " ");
    AddMenuItem("Strecke 4 wählen", "SetStrecke", SY12, PController, nil, 4, " ");
    if(!FindObject(NOME))AddMenuItem("Tankanzeige an/aus", "Atank", SY_4, PController, nil, 3, " ");
  return(1);
}

////Tank
func Atank()
{
  if (Equal(tank, 1)) return(tank = nil);
  if (Equal(tank, nil)) return(tank = 1);
  return(1);
}

////Notbremsung
func Notstop()
{
  blockiert = 120;
  SetAction("Halt");
  treibstoff = nil;
  return(1);
}
////Direkt fahren
func Los()
{
  haltezeit = nil;
  return(1);
}

////Tür öffnen/schließen
func ActivateEntrance(peinsteiger)
{
  var max = GetMaxDamage();
   if(!FindObject(NOME))ShowInfoBar(IBRE, this(), treibstoff, 800, 2);
  ShowInfoBar(IBRS, this(), (max ?? 0)-(GetDamage() ?? 0),max, 2);
  
  //Verfeindet?
  if(Hostile(GetOwner(this()),GetOwner(peinsteiger))) return(Message("Eingang versperrt!",this()));
  
  //schon besetzt?
  if (!Equal(insasse, nil))
    {
     if ((GetOCF(peinsteiger) ?? 0) & (OCF_CrewMember ?? 0))
       {
        if (!Equal(peinsteiger, insasse)) return(Message("Magnetsphäre besetzt!",this()));
       }
    }
    
  //Aussteigen erlaubt?
  if (Equal(tor, nil))
    {  
     Sound("mts_tor"); 
     //Aussteigen am Bahnhof
     if ((treibstoff ?? 0) > 0) SetAction("Auf");
     //Aussteigen bei Notausstieg
     if ((treibstoff ?? 0) <= 0) SetAction("NotAuf");
     tor = 1;
     insasse = peinsteiger;
    }
    
  return(1);
}



////Aussteigen
func Ejection()
  {
   //Passagiere verabschieden
   if ((GetOCF(insasse) ?? 0) & (OCF_CrewMember ?? 0))
     Message("Auf Wiedersehn.",this());
   insasse = nil;
   tor = 1;
   return(1);
  }
  
////Türauf  
func Torauf()
{
  //Passagiere Begrüßen
  if ((GetOCF(insasse) ?? 0) & (OCF_CrewMember ?? 0))
    Message("Wilkommen an Bord!",this());
  SetEntrance(1);
  return(1);
}
////Türzu
func Torzu()
{
  SetEntrance(nil);
  return(1);
}
////Türzu, daher nicht be/entladen
func NichtLaden(x)
{
  ladbar = x;
  return(1);
}

//// Streckensymbol festlegen
func SetStrecke(nutzlos,pnummer) 
{ 
  RemoveObject(symbol);
  if (Equal(pnummer, 1))
  {
  symbol = CreateObject(SY_1,nil,nil,GetOwner());
  ObjectSetAction(symbol,"Attaching",this());
  //falls im Bahnhof, neue Werte abfragen
  SetAction("Schweben");
  gleis = pnummer;
  }
  if (Equal(pnummer, 2))
  {
  symbol = CreateObject(SY_2,nil,nil,GetOwner());
  ObjectSetAction(symbol,"Attaching",this());
  SetAction("Schweben");
  gleis = pnummer;
  } 
  if (Equal(pnummer, 3))
  {
  symbol = CreateObject(SY_3,nil,nil,GetOwner());
  ObjectSetAction(symbol,"Attaching",this());
  SetAction("Schweben");
  gleis = pnummer;
  }  
  if (Equal(pnummer, 4))
  {
  symbol = CreateObject(SY12,nil,nil,GetOwner());
  ObjectSetAction(symbol,"Attaching",this());
  SetAction("Schweben");
  gleis = pnummer;
  }  
  return(1);
}



////FLUGFUNKTIONEN
func Flug() 
{ 
  if(Equal(nexthalt, lasthalt))nexthalt=nil;
  //nur zum testen da Message("%d",this(),blockiert);
  if((treibstoff ?? 0) < 0)
  return(SetAction("Halt"));
  
  //Sound
  if (Equal(flugsound, nil)) flugsound = GetTime ();
  if (((flugsound ?? 0) + 122) <= (GetTime() ?? 0))
    {
     Sound("mts_flug");
    } 
    //neue Richtungsgeschw.
  if(nexthalt)
  {
  //leitx = Sin(Angle(0,0,GetX(nexthalt),GetY(nexthalt)),3);
  //leity = -Cos(Angle(0,0,GetX(nexthalt),GetY(nexthalt)),3);
  Kurs(Sin(Angle(GetX(),GetY(),GetX(nexthalt),GetY(nexthalt)),3),-(Cos(Angle(GetX(),GetY(),GetX(nexthalt),GetY(nexthalt)),3) ?? 0));
  }
  
  //Richtwerte einlesen
  yrichtung = leity;
  xrichtung = leitx;
  tor = 1;

  //bewegen
  //if(bluba%2){
  SetYDir((yrichtung ?? 0)*10,nil,10);
  SetXDir((xrichtung ?? 0)*10,nil,10);
  //Log("%d,%d",GetXDir(),GetYDir());
  //}
  if(!FindObject(NOME))S42MigrationStep(treibstoff, -1, true);
  if((GetY() ?? 0)<0)S42MigrationAssign(treibstoff, "-=", 20);
  //bluba=(bluba+1)%2;
  S42MigrationStep(blockiert, -1, true);
  
  //Station Suchen
  //drinnen?
  var buka;
  if(!Equal(Contained(this()), nil)) return(nil);
  if(buka=FindObject(ST_1,-20,-20,40,40))
    {
     if(Equal(buka, lasthalt))return(nil);
     //Noch zu nah?
     if((blockiert ?? 0) > 0) return(S42MigrationStep(blockiert, -1));
     //Vergleichswert für Bahnhofsblockade setzen
     bwert = 20;
     ObjectCall(buka,"Andocken",this(),gleis);
     DrawLightningx(nil,nil,(GetX(buka) ?? 0)-(GetX() ?? 0),(GetY(buka) ?? 0)-(GetY() ?? 0));
     lasthalt=buka;
    }
     
    //BAHNHOF SUCHEN
  //drinnen?
  if(!Equal(Contained(this()), nil)) return(nil);
  if(buka=FindObject(ST_5,-20,-20,40,40))
    {
         if(Equal(buka, lasthalt))return(nil);
     //Noch zu nah?
     if((blockiert ?? 0) > (bwert ?? 0)) return(S42MigrationStep(blockiert, -1));
     flugsound = nil;
     Sound("mts_leer");
     blockiert = nil;
     //Sphäre darf be/entladen werden
     ladbar = 1;
     //Vergleichswert für Bahnhofsblockade setzen
     bwert = nil;
     ObjectCall(FindObject(ST_5,-20,-20,40,40),"Andocken",this(),gleis);
          DrawLightningx(nil,nil,(GetX(buka) ?? 0)-(GetX() ?? 0),(GetY(buka) ?? 0)-(GetY() ?? 0));
     lasthalt=buka;
    }
  return(1);
}

//SPHÄRE FÜLLEN
func Giver(obj,caller)
  {
   if(Equal(obj, nil)) return(nil);
   Exit(obj);
   Enter(this(),obj);
   return(1);
  }

////Haltezeit einstellen
func SetHaltezeit(x) 
  {
  //Einmal eingestellt, dann Ruhe
  if (Equal(x, urhaltezeit)) return(1);
   urhaltezeit = x;
   haltezeit = x;
   SetAction("Bahnhof");
  }
  

////Weiterfliegen
func Weiter() 
  {
   blockiert = 35;
   SetAction("Schweben");
   return(1);
  }

////Kurs einstellen
func Kurs(x,y) 
{
  leity=y;
  leitx=x;
  return(1);
}

////Im Bahnhof
func Fahrplan() 
{
  if (Equal(haltezeit, 3030)) return(tor = nil);
  while ((haltezeit ?? 0) > 0) return(S42MigrationFirst(S42MigrationStep(haltezeit , -1, true), tor = nil));
  urhaltezeit = nil;
  tor = 1;
  blockiert = 15;
  bwert = nil;
  treibstoff = 800;
  SetAction("Schweben");
  return(1);
}


////Wenn tank leer...
func Tanken()
{ 
  //aussteigbar
  tor = nil;
  
  //Noch zu nah?
  if((blockiert ?? 0) > 0)
     return(S42MigrationAssign(blockiert, "-=", 1));
     
  //Station Suchen
  //drinnen?
  if(!Equal(Contained(this()), nil)) return(nil);
  if(FindObject(ST_1,-20,-20,40,40))
    {
     ObjectCall(FindObject(ST_1,-20,-20,40,40),"Andocken",this(),gleis);
    }
    //BAHNHOF SUCHEN
  //drinnen?
  if(!Equal(Contained(this()), nil)) return(nil);
  if(FindObject(ST_5,-20,-20,40,40))
    {
     //Sphäre darf NICHT be/entladen werden
     ladbar = nil;
     flugsound = nil;
     Sound("mts_leer");
     ObjectCall(FindObject(ST_5,-20,-20,40,40),"Andocken",this(),gleis);
    }
  return(1);
}


/* Schaden */

protected func Damage(...) {
  // zu viel Schaden?
  if ((GetDamage() ?? 0) >= (GetMaxDamage() ?? 0))
    return(Destroy());
  // Anzeige
  ControlDig();
  return(1);
}

public func Destroy(...) {
  // Inhalt rausschleudern
  var obj;
  RemoveObject(symbol);
  while (obj = Contents())
    Exit(obj,nil,nil,Random(360), (Random(9) ?? 0)-4, -(Random(5) ?? 0), (Random(51) ?? 0)-25);
  // Zerstörung durch Explosion
  var i = (GetMaxDamage() ?? 0) / 8;
  while (i)
  {
    obj = CreateObject(SFRX, (Random((GetObjWidth() ?? 0) * 2/3) ?? 0) - (GetObjWidth() ?? 0) / 3,
                             (Random((GetObjHeight() ?? 0) * 2/3) ?? 0) - (GetObjHeight() ?? 0) / 3, GetOwner());
    ObjectCall(obj, "Settings", (i ?? 0)*3/2, 20, 1);
    S42MigrationStep(i, -1);
  }
  // Explosion
  obj = CreateObject(SFRX, nil,nil,GetOwner());
  ObjectCall(obj, "Settings", (GetMaxDamage() ?? 0)/10, 50);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(20);
  else FakeExplode(20,nil,nil,20);
  return(1);
}
  
public func GetMaxDamage(...) { return(40);
}
public func StatusBarPos(...) { return(2);
}
public func InfoBarSize(...) {  return(1);
}


