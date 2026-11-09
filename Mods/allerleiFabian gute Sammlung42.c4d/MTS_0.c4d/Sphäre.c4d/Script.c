/*-- Magnetsphäre --*/

#strict

local yrichtung, xrichtung, leitx, leity, gleis, symbol, blockiert, treibstoff,
      insasse, tor, haltezeit, urhaltezeit, ladbar, flugsound, bwert,tank,lasthalt,bluba,nexthalt;

public func IsVehicleProduct()   { return(true); }
      
func Initialize() 
{
  //Schweben einleiten
  SetAction("Halt");
  leitx = 0;
  leity = 0;
  //Anfangsstrecke=1
  symbol = CreateObject(SY_1,0,0,GetOwner());
  ObjectSetAction(symbol,"Attaching",this());
  gleis = 1;
  bwert = 35; 
  if(FindObject(NOME))tank=0;
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
 var tproz = treibstoff*100 / 800;
 //Drinnen?
 //if(GetAction() S= "Schweben")ControlDig();
 if(Contained(this()) != 0)
   { 
    treibstoff = 0;
    if(symbol) return(RemoveObject(symbol));
    return(1);
   }
 //Keine Aktion?
 if(GetAction(this()) eq "Idle") SetAction("Halt");
 //Tankanzeige
 if(!FindObject(NOME))if (tank == 1) ControlDig();
 //Symbol überm Kopp?
 if(symbol) return(1);
 //Sonst Neues erstellen
 if(gleis == 1) symbol = CreateObject(SY_1,0,0,GetOwner());
 if(gleis == 2) symbol = CreateObject(SY_2,0,0,GetOwner());
 if(gleis == 3) symbol = CreateObject(SY_3,0,0,GetOwner());
 if(gleis == 4) symbol = CreateObject(SY12,0,0,GetOwner());
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
   
  treibstoff = 0;
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
  CreateMenu(ST_1, PController, this(), 0, "Strecken konfigurieren", 0, 0, 0);
    AddMenuItem("Notbremse ziehen", "Notstop", SYCC, PController, 0, 0, " ");
    AddMenuItem("Sofort losfahren", "Los", SY_5, PController, 0, 0, " ");
    AddMenuItem("Strecke 1 wählen", "SetStrecke", SY_1, PController, 0, 1, " ");
    AddMenuItem("Strecke 2 wählen", "SetStrecke", SY_2, PController, 0, 2, " ");
    AddMenuItem("Strecke 3 wählen", "SetStrecke", SY_3, PController, 0, 3, " ");
    AddMenuItem("Strecke 4 wählen", "SetStrecke", SY12, PController, 0, 4, " ");
    if(!FindObject(NOME))AddMenuItem("Tankanzeige an/aus", "Atank", SY_4, PController, 0, 3, " ");
  return(1);
}

////Tank
func Atank()
{
  if (tank == 1) return(tank = 0);
  if (tank == 0) return(tank = 1);
  return(1);
}

////Notbremsung
func Notstop()
{
  blockiert = 120;
  SetAction("Halt");
  treibstoff = 0;
  return(1);
}
////Direkt fahren
func Los()
{
  haltezeit = 0;
  return(1);
}

////Tür öffnen/schließen
func ActivateEntrance(peinsteiger)
{
  var max = GetMaxDamage();
   if(!FindObject(NOME))ShowInfoBar(IBRE, this(), treibstoff, 800, 2);
  ShowInfoBar(IBRS, this(), max-GetDamage(),max, 2);
  
  //Verfeindet?
  if(Hostile(GetOwner(this()),GetOwner(peinsteiger))) return(Message("Eingang versperrt!",this()));
  
  //schon besetzt?
  if (insasse != 0) 
    {
     if (GetOCF(peinsteiger) & OCF_CrewMember())
       {
        if (peinsteiger != insasse) return(Message("Magnetsphäre besetzt!",this()));
       }
    }
    
  //Aussteigen erlaubt?
  if (tor == 0) 
    {  
     Sound("mts_tor"); 
     //Aussteigen am Bahnhof
     if (treibstoff > 0) SetAction("Auf");
     //Aussteigen bei Notausstieg
     if (treibstoff <= 0) SetAction("NotAuf");
     tor = 1;
     insasse = peinsteiger;
    }
    
  return(1);
}



////Aussteigen
func Ejection()
  {
   //Passagiere verabschieden
   if (GetOCF(insasse) & OCF_CrewMember())
     Message("Auf Wiedersehn.",this());
   insasse = 0;
   tor = 1;
   return(1);
  }
  
////Türauf  
func Torauf()
{
  //Passagiere Begrüßen
  if (GetOCF(insasse) & OCF_CrewMember())
    Message("Wilkommen an Bord!",this());
  SetEntrance(1);
  return(1);
}
////Türzu
func Torzu()
{
  SetEntrance(0);
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
  if (pnummer == 1)
  {
  symbol = CreateObject(SY_1,0,0,GetOwner());
  ObjectSetAction(symbol,"Attaching",this());
  //falls im Bahnhof, neue Werte abfragen
  SetAction("Schweben");
  gleis = pnummer;
  }
  if (pnummer == 2)
  {
  symbol = CreateObject(SY_2,0,0,GetOwner());
  ObjectSetAction(symbol,"Attaching",this());
  SetAction("Schweben");
  gleis = pnummer;
  } 
  if (pnummer == 3)
  {
  symbol = CreateObject(SY_3,0,0,GetOwner());
  ObjectSetAction(symbol,"Attaching",this());
  SetAction("Schweben");
  gleis = pnummer;
  }  
  if (pnummer == 4)
  {
  symbol = CreateObject(SY12,0,0,GetOwner());
  ObjectSetAction(symbol,"Attaching",this());
  SetAction("Schweben");
  gleis = pnummer;
  }  
  return(1);
}



////FLUGFUNKTIONEN
func Flug() 
{ 
  if(nexthalt==lasthalt)nexthalt=0;
  //nur zum testen da Message("%d",this(),blockiert);
  if(treibstoff < 0)
  return(SetAction("Halt"));
  
  //Sound
  if (flugsound == 0) flugsound = GetTime ();
  if ((flugsound + 122) <= GetTime()) 
    {
     Sound("mts_flug");
    } 
    //neue Richtungsgeschw.
  if(nexthalt)
  {
  //leitx = Sin(Angle(0,0,GetX(nexthalt),GetY(nexthalt)),3);
  //leity = -Cos(Angle(0,0,GetX(nexthalt),GetY(nexthalt)),3);
  Kurs(Sin(Angle(GetX(),GetY(),GetX(nexthalt),GetY(nexthalt)),3),-Cos(Angle(GetX(),GetY(),GetX(nexthalt),GetY(nexthalt)),3));
  }
  
  //Richtwerte einlesen
  yrichtung = leity;
  xrichtung = leitx;
  tor = 1;

  //bewegen
  //if(bluba%2){
  SetYDir(yrichtung*10,0,10);
  SetXDir(xrichtung*10,0,10);
  //Log("%d,%d",GetXDir(),GetYDir());
  //}
  if(!FindObject(NOME))treibstoff--;
  if(GetY()<0)treibstoff-=20;
  //bluba=(bluba+1)%2;
  blockiert--;
  
  //Station Suchen
  //drinnen?
  var buka;
  if(Contained(this()) != 0) return(0);
  if(buka=FindObject(ST_1,-20,-20,40,40))
    {
     if(buka==lasthalt)return(0);
     //Noch zu nah?
     if(blockiert > 0) return(--blockiert);
     //Vergleichswert für Bahnhofsblockade setzen
     bwert = 20;
     ObjectCall(buka,"Andocken",this(),gleis);
     DrawLightningx(0,0,GetX(buka)-GetX(),GetY(buka)-GetY());
     lasthalt=buka;
    }
     
    //BAHNHOF SUCHEN
  //drinnen?
  if(Contained(this()) != 0) return(0);
  if(buka=FindObject(ST_5,-20,-20,40,40))
    {
         if(buka==lasthalt)return(0);
     //Noch zu nah?
     if(blockiert > bwert) return(--blockiert);
     flugsound = 0;
     Sound("mts_leer");
     blockiert = 0;
     //Sphäre darf be/entladen werden
     ladbar = 1;
     //Vergleichswert für Bahnhofsblockade setzen
     bwert = 0;
     ObjectCall(FindObject(ST_5,-20,-20,40,40),"Andocken",this(),gleis);
          DrawLightningx(0,0,GetX(buka)-GetX(),GetY(buka)-GetY());
     lasthalt=buka;
    }
  return(1);
}

//SPHÄRE FÜLLEN
func Giver(obj,caller)
  {
   if(obj == 0) return(0);
   Exit(obj);
   Enter(this(),obj);
   return(1);
  }

////Haltezeit einstellen
func SetHaltezeit(x) 
  {
  //Einmal eingestellt, dann Ruhe
  if (x == urhaltezeit) return(1);
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
  if (haltezeit == 3030) return(tor = 0);
  while (haltezeit > 0) return(haltezeit --, tor = 0);
  urhaltezeit = 0;
  tor = 1;
  blockiert = 15;
  bwert = 0;
  treibstoff = 800;
  SetAction("Schweben");
  return(1);
}


////Wenn tank leer...
func Tanken()
{ 
  //aussteigbar
  tor = 0;
  
  //Noch zu nah?
  if(blockiert > 0)
     return(blockiert -= 1);
     
  //Station Suchen
  //drinnen?
  if(Contained(this()) != 0) return(0);
  if(FindObject(ST_1,-20,-20,40,40))
    {
     ObjectCall(FindObject(ST_1,-20,-20,40,40),"Andocken",this(),gleis);
    }
    //BAHNHOF SUCHEN
  //drinnen?
  if(Contained(this()) != 0) return(0);
  if(FindObject(ST_5,-20,-20,40,40))
    {
     //Sphäre darf NICHT be/entladen werden
     ladbar = 0;
     flugsound = 0;
     Sound("mts_leer");
     ObjectCall(FindObject(ST_5,-20,-20,40,40),"Andocken",this(),gleis);
    }
  return(1);
}


/* Schaden */

protected Damage:
  // zu viel Schaden?
  if (GetDamage() >= GetMaxDamage())
    return(Destroy());
  // Anzeige
  ControlDig();
  return(1);

public Destroy:
  // Inhalt rausschleudern
  var obj;
  RemoveObject(symbol);
  while (obj = Contents())
    Exit(obj,0,0,Random(360), Random(9)-4, -Random(5), Random(51)-25);
  // Zerstörung durch Explosion
  var i = GetMaxDamage() / 8;
  while (i)
  {
    obj = CreateObject(SFRX, Random(GetObjWidth() * 2/3) - GetObjWidth() / 3,
                             Random(GetObjHeight() * 2/3) - GetObjHeight() / 3, GetOwner());
    ObjectCall(obj, "Settings", i*3/2, 20, 1);
    --i;
  }
  // Explosion
  obj = CreateObject(SFRX, 0,0,GetOwner());
  ObjectCall(obj, "Settings", GetMaxDamage()/10, 50);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(20);
  else FakeExplode(20,0,0,20);
  return(1);
  
public GetMaxDamage: return(40);
public StatusBarPos: return(2);
public InfoBarSize:  return(1);


