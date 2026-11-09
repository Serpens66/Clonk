/*-- Weichenkonsole --*/

#strict 3
#include BOB2
local rad, gleis1, gleis2, gleis3, gleis4,cont, leitx, leity,step1, step2, step3, step4,basement, manager, obj,hlper;


func Construction()
{
  //basement = CreateObject(BS04,0,+6,GetOwner());
  SetClrModulation(RGBa(nil,nil,nil,255));
  SetCategory(1);
  return(1);
}

func Initialize() 
{
  //Weiche erstellen
  rad = CreateObject(ST_2,nil,nil,GetOwner());
  ObjectSetAction(rad,"Attaching",this());
  return(1);
}



//STRECKENSTATUS
func ControlUp(PController)
{
  [Streckenrichtung anzeigen|Image=ST_2|Condition=IsBuilt]

  //Verfeindet?
   if(Hostile(GetOwner(this()),GetOwner(PController))) return(Message("Feind an der Weiche!",this()));
  
  if ((gleis1 ?? 0) > 180) gleis1 = ((gleis1 ?? 0) - 180) * -1;
  if ((gleis2 ?? 0) > 180) gleis2 = ((gleis2 ?? 0) - 180) * -1;
  if ((gleis3 ?? 0) > 180) gleis3 = ((gleis3 ?? 0) - 180) * -1;
  if ((gleis4 ?? 0) > 180) gleis4 = ((gleis4 ?? 0) - 180) * -1;
  if (Equal(gleis1, 360)) gleis1 = (nil);
  if (Equal(gleis2, 360)) gleis2 = (nil);
  if (Equal(gleis3, 360)) gleis3 = (nil);
  if (Equal(gleis4, 360)) gleis4 = (nil);

  Message("<c fbfbfb>Strecke 1: %d°|Strecke 2: %d°|Strecke 3: %d°|Strecke 4: %d°",this(),gleis1,gleis2,gleis3,gleis4);
  return(1);
}



//STRECKENAUSWAHL
func ControlDigDouble(PController)
{
  [Streckenrichtung einstellen|Image=SY_5|Condition=IsBuilt]
  //Fremder?
  if(!Equal(GetOwner(this()), GetOwner(PController))) return(Message("Nur dem Besitzer zugänglich!",this()));
   var max = GetMaxDamage();
  //ShowInfoBar(SBQ0, this(), max-GetDamage(), max, 5, 1);
  ShowInfoBar(IBRS, this(), (max ?? 0)-(GetDamage() ?? 0),max, 2);
  
  //Streckenmenü öffnen
  cont = PController;
    var mc=CreateObject(SMEC,nil,nil,GetOwner(cont));
  CreateMenu(ST_1, PController, this(), nil, "Strecken konfigurieren", nil, nil, nil);
  mc->Init(cont,this(),"ControlDigDouble");
    AddMenuItem("Richtung von Strecke 1 einstellen", "Strecke", SY_1, PController, nil, 1, " ");
    AddMenuItem("Richtung von Strecke 2 einstellen", "Strecke", SY_2, PController, nil, 2, " ");
    AddMenuItem("Richtung von Strecke 3 einstellen", "Strecke", SY_3, PController, nil, 3, " ");
    AddMenuItem("Richtung von Strecke 4 einstellen", "Strecke", SY12, PController, nil, 4, " ");
    
  return(1);
}

func ControlDigDouble_Closed()
{
cont=nil;
}


//WINKELMENÜ (WEICHE WIRD AUF DIE ENTSPRECHENDE POSITION GEDREHT)
func Strecke() 
{
    hlper=CreateObject(NFWH,nil,nil,GetOwner(cont));
  //Strecke auswählen
  if (Equal(Par(1), 1)) {SetR(gleis1,rad);LocalN("r",hlper)=gleis1;}
  if (Equal(Par(1), 2)) {SetR(gleis2,rad);LocalN("r",hlper)=gleis2;}
  if (Equal(Par(1), 3)) {SetR(gleis3,rad);LocalN("r",hlper)=gleis3;}
  if (Equal(Par(1), 4)) {SetR(gleis4,rad);LocalN("r",hlper)=gleis4;}
  LocalN("mother",hlper)=this();
  LocalN("radius",hlper)=200;
  hlper->DoIt();
  Message("<c fbfbfb>Nach Strecke %d ausgerichtet",this(),Par(1),GetR(rad));
 
  //Winkel einstellen
  CreateMenu(ST_1, cont, this(), nil, "Richtung", nil, 1, 1);
    var mc=CreateObject(SMEC,nil,nil,GetOwner(cont));
    mc->Init(cont,this(),"Strecke");
    AddMenuItem("  nach rechts", "NGradp", SY_6,  cont, nil, Par(1), " ");
    AddMenuItem("  nach links", "NGradm", SY_7,  cont, nil, Par(1), " ");
  return(hlper);
}

func Strecke_Closed()
{
cont=nil;
RemoveObject(hlper);
}


//NORMALSTRECKE +
func NGradp() 
{
  //Variablen an Winkelzahlen angleichen
  if (Equal(gleis1, 360)) gleis1 = nil;
  if (Equal(gleis2, 360)) gleis2 = nil;
  if (Equal(gleis3, 360)) gleis3 = nil;
  if (Equal(gleis4, 360)) gleis4 = nil;

  //Winkel einstellen, welche Stufe?
  if (Equal(Par(1), 1))
    {
    S42MigrationAssign(step1, "+=", 1);
    if (Equal(step1, 36)) step1 = nil;
    gleis1 = SetDrehung(rad,step1);
    }
  if (Equal(Par(1), 2))
    {
    S42MigrationAssign(step2, "+=", 1);
    if (Equal(step2, 36)) step2 = nil;
    gleis2 = SetDrehung(rad,step2);
    }
  if (Equal(Par(1), 3))
    {
    S42MigrationAssign(step3, "+=", 1);
    if (Equal(step3, 36)) step3 = nil;
    gleis3 = SetDrehung(rad,step3);
    }
  if (Equal(Par(1), 4))
    {
    S42MigrationAssign(step4, "+=", 1);
    if (Equal(step4, 36)) step4 = nil;
    gleis4 = SetDrehung(rad,step4);
    }

  Message("<c fbfbfb>Strecke %d: %d°",this(),Par(1),GetR(rad));
  return(1);
}



//NORMALSTRECKE -
func NGradm() 
{
  //Variablen an Winkelzahlen angleichen
  if (Equal(gleis1, 360)) gleis1 = nil;
  if (Equal(gleis2, 360)) gleis2 = nil;
  if (Equal(gleis3, 360)) gleis3 = nil;
  if (Equal(gleis4, 360)) gleis4 = nil;
  
  //Winkel einstellen, welche Stufe?
  if (Equal(Par(1), 1))
    {
    S42MigrationAssign(step1, "-=", 1);
    if (Equal(step1, -1)) step1 = 35;
    gleis1 = SetDrehung(rad,step1);
    }
  if (Equal(Par(1), 2))
    {
    S42MigrationAssign(step2, "-=", 1);
    if (Equal(step2, -1)) step2 = 35;
    gleis2 = SetDrehung(rad,step2);
    }
  if (Equal(Par(1), 3))
    {
    S42MigrationAssign(step3, "-=", 1);
    if (Equal(step3, -1)) step3 = 35;
    gleis3 = SetDrehung(rad,step3);
    }
  if (Equal(Par(1), 4))
    {
    S42MigrationAssign(step4, "-=", 1);
    if (Equal(step4, -1)) step4 = 35;
    gleis4 = SetDrehung(rad,step4);
    }
    
  Message("<c fbfbfb>Strecke %d: %d°",this(),Par(1),GetR(rad));

  return(1);
}


//SPHÄRE ANDOCKEN
func Andocken(pkugel,pnummer) 
  {
  if((LocalN("treibstoff",pkugel) ?? 0) <= 0) return(Message("Sphäre ist nicht magnetisiert!",this()));
  Sound("mts_halt");
 
  if (Equal(pnummer, nil))
    return(Message("Magnetsphäre hat keine Strecke zugewiesen!",this()));   
  if (Equal(pnummer, 1))
    SetR(gleis1,rad);
  if (Equal(pnummer, 2))
    SetR(gleis2,rad);
  if (Equal(pnummer, 3))
    SetR(gleis3,rad);
  if (Equal(pnummer, 4))
    SetR(gleis4,rad);
    
  leitx = Sin((GetR(rad) ?? 0)*1000,3,1000);
  leity = -(Cos((GetR(rad) ?? 0)*1000,3,1000) ?? 0);
  var ffs=FindObjectCircleDiagonal(GetR(rad),100,5000,this(),1,GetOwner(),ST_1,ST_5,nil);
  ObjectCall(pkugel,"NextDock",ffs);
  //CastParticles("PxSpark",200,200,GetX(ffs)-GetX(),GetY(ffs)-GetY(),50,50,RGBa(200,200,200,10),RGBa(200,200,200,10));
  ObjectCall(pkugel,"Kurs",leitx,leity);
  ObjectSetAction(pkugel,"Attaching",this());
  return(1);
  }



/*ZERSTÖRUNG*/
func Destruction()
  {
  var obj;
  RemoveObject(rad);
  while (obj = Contents()) RemoveObject(obj);
  //SL-Objekte entfernen
  if (basement) RemoveObject(basement);
  if (manager) RemoveObject(manager);
  return(1);
  }


/* EIGENSCHAFTEN 
*/

func EnoughEnergy()
{ 
  return(GetEnergy());
}


/*FUNKTIONEN*/
func SetDrehung(prad,pstep)
{
 var winkel;
 var a;
 a=(pstep ?? 0)*10;
 winkel=a;
 if(Equal(winkel, 10))S42MigrationStep(winkel, 1, true);
 if(Equal(winkel, 350))S42MigrationStep(winkel, -1, true);
 //Stufen in Winkel umrechnen

 //Winkel einstellen 
 SetR(winkel,prad);
 LocalN("r",hlper)=winkel;
 hlper->DoIt();
 return(winkel);
 }
 
 public func InfoBarPos(...) {   return(1);
}
public func InfoBarSize(...) {  return(1);
}
public func Repairable(...) {   return(1);
}
public func GetMaxDamage(...) { return(60);
}
 
 



