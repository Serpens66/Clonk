/*-- Bahnhof --*/

#strict 3
#include BOB1

local basement, manager, rohr, cont, zeit1, zeit2, zeit3, zeit4,strecke, hrad,dot,circle,hlp1,hlp2;


/* Hilfs-Objekt und Fundament erzeugen bzw. löschen */

func Initialize()
{
  //Weiche erstellen
  rohr = CreateObject(ST_5,nil,nil,GetOwner());
  ObjectCall(rohr,"SetErzeuger",this());
  ObjectSetAction(rohr,"Attaching",this());
  SetSolidMask(nil,91,50,10,-4,3,nil);
  zeit1 = 3030;
  zeit2 = 3030;
  zeit3 = 3030;
  zeit4 = 3030;
  hrad = LocalN("rad",rohr);
  DoEnergy(100);
  return(1);
}

/* Energie? */
func EnoughEnergy()
{ 
  return(GetEnergy());
}

func Construction()
{
  basement = CreateObject(BS16,-2,+6,GetOwner());
  return(1);
}
  
  
func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
  if (rohr) RemoveObject(rohr);
  if (hrad) RemoveObject(hrad);
  if (manager) RemoveObject(manager);
  return(1);
}

/* Steuerung */

func ContainedUp(PController)
{
  [Streckenstatus]  
  //Verfeindet?
   if(Hostile(GetOwner(this()),GetOwner(PController))) return(Message("<c fbfbfb>Feind im Bahnhof!",this()));
   
  CreateMenu(ST_4, PController, this(), nil, "Strecken konfigurieren", nil, nil, nil);
    AddMenuItem("Status von Strecke 1 abfragen", "Wahl", SY_1, PController, nil, 1, " ");
    AddMenuItem("Status von Strecke 2 abfragen", "Wahl", SY_2, PController, nil, 2, " ");
    AddMenuItem("Status von Strecke 3 abfragen", "Wahl", SY_3, PController, nil, 3, " ");
    AddMenuItem("Status von Strecke 4 abfragen", "Wahl", SY12, PController, nil, 4, " ");
  ObjectCall(rohr,"GetRichtung");
  return(1);
}
//STATUSANFRAGE ÜBERTRAGEN
func Wahl()
{
  if(Equal(Par(1), 1)) ObjectCall(rohr,"GetRichtung1");
  if(Equal(Par(1), 2)) ObjectCall(rohr,"GetRichtung2");
  if(Equal(Par(1), 3)) ObjectCall(rohr,"GetRichtung3");
  if(Equal(Par(1), 4)) ObjectCall(rohr,"GetRichtung4");
  return(1);
}

//ENERGIE-TIMER
func Energie()
{
if((GetCon(this()) ?? 0) < 100) return(nil);
if((GetEnergy(this()) ?? 0) <= 0) LocalN("power",rohr) = nil;
if((GetEnergy(this()) ?? 0) > 0) LocalN("power",rohr) = 1;
  return(1);
}


//Halteeinstellungen verändern
func ContainedDigDouble(PController)//einstellen sodass cont 0 wird bei close
{
  [Strecken einstellen]
  //Fremder?
  //if(GetOwner(this()) != GetOwner(PController)) return(Message("Nur dem Besitzer zugänglich!",this()));
  if(cont)return(Message("<c fbfbfb>Jmd anderes benutzt|die Konsole!",PController));
  //Streckenmenü öffnen
  cont = PController;
  var mc=CreateObject(SMEC,nil,nil,-1);
  CreateMenu(ST_4, PController, this(), nil, "Strecken konfigurieren", nil, nil, nil);
  mc->Init(cont,this(),"ContainedDigDouble2");
    AddMenuItem("Strecke 1 wählen", "Auswahl", SY_1, PController, nil, 1, " ");
    AddMenuItem("Strecke 2 wählen", "Auswahl", SY_2, PController, nil, 2, " ");
    AddMenuItem("Strecke 3 wählen", "Auswahl", SY_3, PController, nil, 3, " ");
    AddMenuItem("Strecke 4 wählen", "Auswahl", SY12, PController, nil, 4, " ");
    AddMenuItem("Neue Feldweichen", "NewWeiche", ST_1, PController, nil, PController, " ");
    AddMenuItem("Weichen ändern", "ChangeWeiche", ST_1, PController, nil, PController, " ");
    AddMenuItem("Weichen zerstören", "DestroyWeiche", ST_1, PController, nil, PController, " ");
  return(1);
}

func ContainedDigDouble2_Changed(selection,info)
{
if(Equal(selection, nil))PlayerMessage(GetOwner(cont),"@<c fbfbfb>Ändert die Andock-|optionen für Strecke 1.",this());
if(Equal(selection, 1))PlayerMessage(GetOwner(cont),"@<c fbfbfb>Ändert die Andock-|optionen für Strecke 2.",this());
if(Equal(selection, 2))PlayerMessage(GetOwner(cont),"@<c fbfbfb>Ändert die Andock-|optionen für Strecke 3.",this());
if(Equal(selection, 3))PlayerMessage(GetOwner(cont),"@<c fbfbfb>Ändert die Andock-|optionen für Strecke 4.",this());
if(Equal(selection, 4))PlayerMessage(GetOwner(cont),"@<c fbfbfb>Installiert neue Weichen.",this());
if(Equal(selection, 5))PlayerMessage(GetOwner(cont),"@<c fbfbfb>Ändert Weichenausrichtungen.",this());
if(Equal(selection, 6))PlayerMessage(GetOwner(cont),"@<c fbfbfb>Deinstallisiert Weichen.",this());
Sound("menuclick",1,nil,nil,GetOwner(cont));
}

func ContainedDigDouble2_Closed()
{
if(!GetMenu(cont))cont=nil;
PlayerMessage(GetOwner(cont)," ",this());
}

func ContainedDigDouble_Changed()
{
Sound("menuclick",1,nil,nil,GetOwner(cont));
}

func ContainedDigDouble_Closed()
{
if(!GetMenu(cont))cont=nil;
PlayerMessage(GetOwner(cont)," ",this());
}

func DestroyWeiche(ID,controller)
{
var mc=CreateObject(SMEC,nil,nil,-1);
CreateMenu(ID,controller,this(),nil,nil,nil,1);
mc->Init(controller,this(),"NewWeiche");
var a=nil;var i=-1;
while(a=FindFriendlyObject(ST_1,GetOwner(controller),a)) {S42MigrationStep(i, 1, true);mc->AddInfo(a);AddMenuItem(Format("%d. Weiche",(i ?? 0)+1),"WeicheTot",ST_1,Par(1),nil,a);}
}

func WeicheTot(ID,targetobj)
{
DoDamage(1000,targetobj);
}



func ChangeWeiche(ID,controller)
{
if(!FindFriendlyObject(ST_1,GetOwner(controller)))return(PlayerMessage(GetOwner(controller),"Keine befreundete Weiche gefunden",controller));
var mc=CreateObject(SMEC,nil,nil,-1);
CreateMenu(ID,controller,this(),nil,nil,nil,1);
mc->Init(controller,this(),"NewWeiche");
var a=nil;var i=-1;
while(a=FindFriendlyObject(ST_1,GetOwner(controller),a)) {S42MigrationStep(i, 1, true);mc->AddInfo(a);AddMenuItem(Format("%d. Weiche",(i ?? 0)+1),"WegSetWeich",ST_1,Par(1),nil,a);}
}

func WegSetWeich(ID,tobj)
{
dot=tobj->Strecke();
var mc=CreateObject(SMEC,nil,nil,-1);
CreateMenu(ST_3,cont,Par(1),nil,nil,nil,nil,1);
mc->Init(cont,this(),"WegSetWeich");
var newa;
newa=CreateObject(CCSY,nil,nil,-1);
SetGraphics(nil,newa,SY_1,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:1 CCW","NGradm",nil,cont,nil,1," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CCSY,nil,nil,-1);
SetGraphics(nil,newa,SY_2,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:2 CCW","NGradm",nil,cont,nil,2," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CCSY,nil,nil,-1);
SetGraphics(nil,newa,SY_3,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:3 CCW","NGradm",nil,cont,nil,3," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CCSY,nil,nil,-1);
SetGraphics(nil,newa,SY12,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:4 CCW","NGradm",nil,cont,nil,4," ",4,newa);
RemoveObject(newa);

//hälfte

newa=CreateObject(CWSY,nil,nil,-1);
SetGraphics(nil,newa,SY_1,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:1 CW","NGradp",nil,cont,nil,1," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CWSY,nil,nil,-1);
SetGraphics(nil,newa,SY_2,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:2 CW","NGradp",nil,cont,nil,2," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CWSY,nil,nil,-1);
SetGraphics(nil,newa,SY_3,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:3 CW","NGradp",nil,cont,nil,3," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CWSY,nil,nil,-1);
SetGraphics(nil,newa,SY12,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:4 CW","NGradp",nil,cont,nil,4," ",4,newa);
RemoveObject(newa);
SetPlrView(GetOwner(cont),tobj);
mc->AddInfo(tobj);
mc->AddInfo(tobj);
mc->AddInfo(tobj);
mc->AddInfo(tobj);
mc->AddInfo(tobj);
mc->AddInfo(tobj);
mc->AddInfo(tobj);
mc->AddInfo(tobj);
hlp1=tobj;
Schedule("SetMenuSize(4,2,cont)",5);
}

func WegSetWeich_Changed(selection,info)
{
if(dot){dot->DelOldLine();RemoveObject(dot);}
var a=selection;
a=(a ?? 0)%4;
S42MigrationStep(a, 1, true);
if(! (Equal(a, hlp2)))Sound("menuclick",1,nil,nil,GetOwner(cont));
hlp2=a;
dot=info->Strecke(nil,a);
}

func WegSetWeich_Closed()
{
if(dot){dot->DelOldLine();RemoveObject(dot);}
dot=nil;
var c=CenterWeich(ST_1,hlp1);
c->ShowSomething(nil,hlp2);
hlp1=nil;
if(!GetMenu(cont))cont=nil;
}


func NewWeiche()
{
var a=nil;var i=-1;
CreateMenu(QQQQ,Par(1),nil,nil,nil,nil,1);
var mc=CreateObject(SMEC,nil,nil,-1);
mc-> Init(Par(1),this(),"NewWeiche");
mc->AddInfo(this());
S42MigrationStep(i, 1, true);
AddMenuItem(Format("%d. Banhof",(i ?? 0)+1),"CenterBanhof",ST_4,Par(1),nil,this());
while(a=FindFriendlyObject(ST_4,GetOwner(Par(1)),a)) {S42MigrationStep(i, 1, true);mc->AddInfo(a);AddMenuItem(Format("%d. Banhof",(i ?? 0)+1),"CenterBanhof",ST_4,Par(1),nil,a);}
while(a=FindFriendlyObject(ST_1,GetOwner(Par(1)),a)) {S42MigrationStep(i, 1, true);mc->AddInfo(a);AddMenuItem(Format("%d. Weiche",(i ?? 0)+1),"CenterWeich",ST_1,Par(1),nil,a);}
}

func NewWeiche_Changed(selection,info)
{
if(circle)RemoveObject(circle);
SetPlrView(GetOwner(cont),info);
var a=CreateObject(HCIR,nil,nil,GetOwner(cont));
SetPosition(GetX(info),GetY(info),a);
circle=a;
Sound("menuclick",1,nil,nil,GetOwner(cont));
}

func NewWeiche_Closed()
{
if(circle)RemoveObject(circle);
if(!GetMenu(cont))cont=nil;
}

func CenterBanhof(ID,obj)
{
var a=CreateObject(NFWH,(GetX(LocalN("rad",LocalN("rohr",obj))) ?? 0)-(GetX() ?? 0),(GetY(LocalN("rad",LocalN("rohr",obj))) ?? 0)-(GetY() ?? 0),GetOwner(cont));
var mc=CreateObject(SMEC,nil,nil,-1);
LocalN("mother",a)=LocalN("rad",LocalN("rohr",obj));
LocalN("r",a)=GetR(LocalN("rad",rohr));
a->DoIt();
dot=a;
CreateMenu(ST_2,cont,a,nil,nil,nil,nil,1);
mc->Init(cont,this(),"CenterBanhof");
AddMenuItem("Links","SetDreh2",SY_8,cont,nil,-90," ");
AddMenuItem("Rechts","SetDreh2",SY_9,cont,nil,90," ");
AddMenuItem("Radius -10","Radius",SY_7,cont,10,-10," ");
AddMenuItem("Radius +10","Radius",SY_6,cont,10,+10," ");
AddMenuItem("Feldweiche platzieren","Feldweicheplatz",ST_1,cont,nil,cont," ");
AddLeerItem(1,cont);
Schedule("SetMenuSize(4,2,cont)",10);
SetPlrView(GetOwner(cont),obj);
}

func CenterBanhof_Changed()
{
Sound("menuclick",1,nil,nil,GetOwner(cont));
}

func CenterBanhof_Closed()
{
if(dot){dot->DelOldLine();RemoveObject(dot);}
dot=nil;
if(!GetMenu(cont))cont=nil;
}

func CenterWeich(ID,obj)
{
var a=CreateObject(NFWH,(GetX(obj) ?? 0)-(GetX() ?? 0),(GetY(obj) ?? 0)-(GetY() ?? 0),GetOwner(cont));
LocalN("mother",a)=obj;
dot=a;
a->DoIt(nil);
var mc=CreateObject(SMEC,nil,nil,-1);
CreateMenu(ID,cont,a,nil,nil,nil,nil,1);
mc->Init(cont,this(),"CenterWeich");
AddMenuItem("-1°","Dreh",SY_8,cont,1,-1," ");
AddMenuItem("+1°","Dreh",SY_9,cont,1,1," ");
AddMenuItem("-10°","Dreh",SY_8,cont,10,-10," ");
AddMenuItem("+10°","Dreh",SY_9,cont,10,10," ");
AddMenuItem("Radius -10","Radius",SY_7,cont,10,-10," ");
AddMenuItem("Radius +10","Radius",SY_6,cont,10,+10," ");
AddMenuItem("Feldweiche platzieren","Feldweicheplatz",ST_1,cont,nil,cont," ");
AddLeerItem(1,cont);
AddMenuItem("Richtung Nr1","ShowSomething",SY_1,cont,nil,1," ");
AddMenuItem("Richtung Nr2","ShowSomething",SY_2,cont,nil,2," ");
AddMenuItem("Richtung Nr3","ShowSomething",SY_3,cont,nil,3," ");
AddMenuItem("Richtung Nr4","ShowSomething",SY12,cont,nil,4," ");
SetPlrView(GetOwner(cont),a);
//AddMenuItem("Strecke aufleuchten","Magicway",0,cont,0,0," ");
Schedule("SetMenuSize(4,3,cont)",10);
return(a);
}

func CenterWeich_Changed()
{
Sound("menuclick",1,nil,nil,GetOwner(cont));
}

func CenterWeich_Closed()
{
if(dot){dot->DelOldLine();RemoveObject(dot);}
dot=nil;
if(!GetMenu(cont))cont=nil;
}

func FindFriendlyObject(ID,player,lastobj)
{
var a;
if(a=FindObject(ID,nil,nil,nil,nil,nil,nil,nil,nil,lastobj)) { if(!Hostile(player,GetOwner(a))) return(a); }
else return(nil);
}


//STRECKENRICHTUNG/ HALTEZEIT/ LADEOPTIONEN
func Auswahl()
{
  strecke = Par(1);
  //Streckenmenü öffnen
  ObjectCall(rohr,"SetStrecke",nil,Par(1));
  var mc=CreateObject(SMEC,nil,nil,-1);
  CreateMenu(ST_1, cont, this(), nil, "Strecken konfigurieren", nil, nil, nil);
    mc->Init(cont,this(),"ContainedDigDouble");
    AddMenuItem("Richtung ändern", "SetRichtung", SY_5, cont, nil, Par(1), " ");
    AddMenuItem("Haltezeit einstellen", "SetZeit", SY_4, cont, nil,Par(1), " ");
    AddMenuItem("be/entladen", "SetLaden", SY10, cont, nil, Par(1), " ");
    AddMenuItem("Streckenwechsel", "SetNewStrecke", SY11, cont, nil, Par(1), " ");
  return(1);
}


//STRECKENRICHTUNG
func SetRichtung()
{
  //ObjectCall(rohr,"SetStrecke",0,Par(1));
  CreateMenu(ST_1, cont, this(), nil, "Strecken konfigurieren", nil, 1, 1);
    AddMenuItem("links", "ObjRichtung", SY_8, cont, nil, 1, " ");
    AddMenuItem("rechts", "ObjRichtung", SY_9, cont, nil, 2, " ");
  return(1);
}
//RICHTUNG ÜBERTRAGEN
func ObjRichtung()
{
  if(Equal(Par(1), 1)) ObjectCall(rohr,"Links");
  if(Equal(Par(1), 2)) ObjectCall(rohr,"Rechts");
  return(1);
}


//HALTEZEIT
func SetZeit()
{
  ObjectCall(rohr,"GetZeit",nil,Par(1));
  CreateMenu(ST_1, cont, this(), nil, "Haltezeit einstellen", nil, 1, 1);
    AddMenuItem("+30 Frames", "ObjZeitPlus", SY_6, cont, nil, Par(1), " ");
    AddMenuItem("-30 Frames", "ObjZeitMinus", SY_7, cont, nil, Par(1), " ");
    AddMenuItem("30 Frames", "ObjZeit30", SYNU, cont, nil, Par(1), " ", nil,10);
    AddMenuItem("Unbegrenzter Aufenthalt", "ObjZeitUnend", SYNU, cont, nil, Par(1), " ", nil,nil);
  return(1);
}
//ZEIT ÜBERTRAGEN +
func ObjZeitPlus()
{ 
  if(And(Equal(Par(1), 1), (zeit1 ?? 0) < 3030) ) ObjectCall(rohr,"Zeit",1,S42MigrationAssign(zeit1, "+=", 30));
  if(And(Equal(Par(1), 2), (zeit2 ?? 0) < 3030) ) ObjectCall(rohr,"Zeit",2,S42MigrationAssign(zeit2, "+=", 30));
  if(And(Equal(Par(1), 3), (zeit3 ?? 0) < 3030) ) ObjectCall(rohr,"Zeit",3,S42MigrationAssign(zeit3, "+=", 30));
  if(And(Equal(Par(1), 4), (zeit4 ?? 0) < 3030) ) ObjectCall(rohr,"Zeit",4,S42MigrationAssign(zeit4, "+=", 30));
  return(1);
}
//ZEIT UNENDLICH
func ObjZeitUnend()
{ 
  if(Equal(Par(1), 1)) ObjectCall(rohr,"Zeit",1,zeit1=3030);
  if(Equal(Par(1), 2)) ObjectCall(rohr,"Zeit",2,zeit2=3030);
  if(Equal(Par(1), 3)) ObjectCall(rohr,"Zeit",3,zeit3=3030);
  if(Equal(Par(1), 4)) ObjectCall(rohr,"Zeit",4,zeit4=3030);
  return(1);
}
//ZEIT 30
func ObjZeit30()
{ 
  if(Equal(Par(1), 1)) ObjectCall(rohr,"Zeit",1,zeit1=30);
  if(Equal(Par(1), 2)) ObjectCall(rohr,"Zeit",2,zeit2=30);
  if(Equal(Par(1), 3)) ObjectCall(rohr,"Zeit",3,zeit3=30);
  if(Equal(Par(1), 4)) ObjectCall(rohr,"Zeit",4,zeit4=30);
  return(1);
}
//ZEIT ÜBERTRAGEN -
func ObjZeitMinus()
{
  if(And(Equal(Par(1), 1), (zeit1 ?? 0) > 30) ) ObjectCall(rohr,"Zeit",1,S42MigrationAssign(zeit1, "-=", 30));
  if(And(Equal(Par(1), 2), (zeit2 ?? 0) > 30) ) ObjectCall(rohr,"Zeit",2,S42MigrationAssign(zeit2, "-=", 30));
  if(And(Equal(Par(1), 3), (zeit3 ?? 0) > 30) ) ObjectCall(rohr,"Zeit",3,S42MigrationAssign(zeit3, "-=", 30));
  if(And(Equal(Par(1), 4), (zeit4 ?? 0) > 30) ) ObjectCall(rohr,"Zeit",4,S42MigrationAssign(zeit4, "-=", 30));
  return(1);
}


//BE/ENTLADEN
func SetLaden()
{
  var x;
  strecke = Par(1);
  if(Equal(strecke, 1) ) x = LocalN("laden1",rohr);
  if(Equal(strecke, 2) ) x = LocalN("laden2",rohr);
  if(Equal(strecke, 3) ) x = LocalN("laden3",rohr);
  if(Equal(strecke, 4) ) x = LocalN("laden4",rohr);
  Message("<c fbfbfb>Strecke %d %s.",this(),strecke,ObjectCall(rohr,"GetAusdruck",x));
  CreateMenu(ST_1, cont, this(), nil, "Ladeoptionen", nil, 1, 1);
    AddMenuItem(" Sphäre beladen", "Laden", SY_6, cont, nil, 1, " ");
    AddMenuItem("Sphäre entladen", "Laden", SY_7, cont, nil, 2, " ");
    AddMenuItem("Keine Aktion", "Laden", SYCC, cont, nil, nil, " ");
  return(1);
} 
//LADEOPTIONEN WEITERLEITEN
func Laden()
{
  if(Equal(strecke, 1) ) LocalN("laden1",rohr) = Par(1);
  if(Equal(strecke, 2) ) LocalN("laden2",rohr) = Par(1);
  if(Equal(strecke, 3) ) LocalN("laden3",rohr) = Par(1);
  if(Equal(strecke, 4) ) LocalN("laden4",rohr) = Par(1);
  Message("<c fbfbfb>Strecke %d %s.",this(),strecke,ObjectCall(rohr,"GetAusdruck",(Par(1))));
  return(1);
 }


//Streckenwechsel
func SetNewStrecke()
{
var x;
 if(Equal(strecke, 1) ) x = LocalN("wechsel1",rohr);
 if(Equal(strecke, 2) ) x = LocalN("wechsel2",rohr);
 if(Equal(strecke, 3) ) x = LocalN("wechsel3",rohr);
 if(Equal(strecke, 4) ) x = LocalN("wechsel4",rohr);
 if(Equal(strecke, x)) Message("<c fbfbfb>Strecke %d: kein Wechsel",this(),strecke);
 else(Message("<c fbfbfb>Strecke %d: Wechsel auf Strecke %d.",this(),strecke,x));
  CreateMenu(ST_1, cont, this(), nil, "Ladeoptionen", nil, nil, 1);
    AddMenuItem("Auf Strecke 1 wechseln. ACHTUNG!:Der Streckenwechsel findet VOR dem Andocken statt, daher gelten am Bahnhof Richtungs,Abfarts-und Ladeeinstellungen der NEUEN Strecke !!!", "NewStrecke", SY_1, cont, nil, 1, " ");
    AddMenuItem("Auf Strecke 2 wechseln. ACHTUNG!:Der Streckenwechsel findet VOR dem Andocken statt, daher gelten am Bahnhof Richtungs,Abfarts-und Ladeeinstellungen der NEUEN Strecke !!!", "NewStrecke", SY_2, cont, nil, 2, " ");
    AddMenuItem("Auf Strecke 3 wechseln. ACHTUNG!:Der Streckenwechsel findet VOR dem Andocken statt, daher gelten am Bahnhof Richtungs,Abfarts-und Ladeeinstellungen der NEUEN Strecke !!!", "NewStrecke", SY_3, cont, nil, 3, " ");
    AddMenuItem("Auf Strecke 4 wechseln. ACHTUNG!:Der Streckenwechsel findet VOR dem Andocken statt, daher gelten am Bahnhof Richtungs,Abfarts-und Ladeeinstellungen der NEUEN Strecke !!!", "NewStrecke", SY12, cont, nil, 4, " ");
  return(1);
 } 
//Streckenwechsel weitergeben
func NewStrecke()
{
  if(Equal(strecke, 1) ) LocalN("wechsel1",rohr) = Par(1);
  if(Equal(strecke, 2) ) LocalN("wechsel2",rohr) = Par(1);
  if(Equal(strecke, 3) ) LocalN("wechsel3",rohr) = Par(1);
  if(Equal(strecke, 4) ) LocalN("wechsel4",rohr) = Par(1);
  if(Equal(strecke, Par(1))) return(Message("<c fbfbfb>Strecke %d: kein Wechsel",this(),strecke));
  Message("<c fbfbfb>Strecke %d: Wechsel auf Strecke %d.",this(),strecke,(Par(1)));
  return(1);
 }

 
/* Inhalt von Loren übernehmen */

func DoorClosed()
{
  var num = -1;
  var obj;
  while (obj = Contents(S42MigrationStep(num, 1)))
    if (ObjectCall(obj, "IsLorry"))
    {
      GrabContents(obj);
      SetCommand(obj,"Exit");
    }
  return(1);
 }
 
 //SPHÄRE AUSRÄUMEN
 func Grabber(obj)
 {
  Exit(obj);
  Enter(this(),obj);
  return(1);
 }
 
/* Eigenschaften */

public func StatusBarID(...) { return(SBR0);
}
public func StatusBarPos(...) { return(2);
}
public func GetMaxDamage(...) { return(75);
}


