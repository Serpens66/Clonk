/*-- Bahnhof --*/

#strict
#include BOB1

local basement, manager, rohr, cont, zeit1, zeit2, zeit3, zeit4,strecke, hrad,dot,circle,hlp1,hlp2;


/* Hilfs-Objekt und Fundament erzeugen bzw. löschen */

func Initialize()
{
  //Weiche erstellen
  rohr = CreateObject(ST_5,0,0,GetOwner());
  ObjectCall(rohr,"SetErzeuger",this());
  ObjectSetAction(rohr,"Attaching",this());
  SetSolidMask(0,91,50,10,-4,3,0);
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
   
  CreateMenu(ST_4, PController, this(), 0, "Strecken konfigurieren", 0, 0, 0);
    AddMenuItem("Status von Strecke 1 abfragen", "Wahl", SY_1, PController, 0, 1, " ");
    AddMenuItem("Status von Strecke 2 abfragen", "Wahl", SY_2, PController, 0, 2, " ");
    AddMenuItem("Status von Strecke 3 abfragen", "Wahl", SY_3, PController, 0, 3, " ");
    AddMenuItem("Status von Strecke 4 abfragen", "Wahl", SY12, PController, 0, 4, " ");
  ObjectCall(rohr,"GetRichtung");
  return(1);
}
//STATUSANFRAGE ÜBERTRAGEN
func Wahl()
{
  if(Par(1) == 1) ObjectCall(rohr,"GetRichtung1");
  if(Par(1) == 2) ObjectCall(rohr,"GetRichtung2");
  if(Par(1) == 3) ObjectCall(rohr,"GetRichtung3"); 
  if(Par(1) == 4) ObjectCall(rohr,"GetRichtung4"); 
  return(1);
}

//ENERGIE-TIMER
func Energie()
{
if(GetCon(this()) < 100) return(0);
if(GetEnergy(this()) <= 0) LocalN("power",rohr) = 0;
if(GetEnergy(this()) > 0) LocalN("power",rohr) = 1;
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
  var mc=CreateObject(SMEC,0,0,-1);
  CreateMenu(ST_4, PController, this(), 0, "Strecken konfigurieren", 0, 0, 0);
  mc->Init(cont,this(),"ContainedDigDouble2");
    AddMenuItem("Strecke 1 wählen", "Auswahl", SY_1, PController, 0, 1, " ");
    AddMenuItem("Strecke 2 wählen", "Auswahl", SY_2, PController, 0, 2, " ");
    AddMenuItem("Strecke 3 wählen", "Auswahl", SY_3, PController, 0, 3, " ");
    AddMenuItem("Strecke 4 wählen", "Auswahl", SY12, PController, 0, 4, " ");
    AddMenuItem("Neue Feldweichen", "NewWeiche", ST_1, PController, 0, PController, " ");
    AddMenuItem("Weichen ändern", "ChangeWeiche", ST_1, PController, 0, PController, " ");
    AddMenuItem("Weichen zerstören", "DestroyWeiche", ST_1, PController, 0, PController, " ");
  return(1);
}

func ContainedDigDouble2_Changed(selection,info)
{
if(selection==0)PlayerMessage(GetOwner(cont),"@<c fbfbfb>Ändert die Andock-|optionen für Strecke 1.",this());
if(selection==1)PlayerMessage(GetOwner(cont),"@<c fbfbfb>Ändert die Andock-|optionen für Strecke 2.",this());
if(selection==2)PlayerMessage(GetOwner(cont),"@<c fbfbfb>Ändert die Andock-|optionen für Strecke 3.",this());
if(selection==3)PlayerMessage(GetOwner(cont),"@<c fbfbfb>Ändert die Andock-|optionen für Strecke 4.",this());
if(selection==4)PlayerMessage(GetOwner(cont),"@<c fbfbfb>Installiert neue Weichen.",this());
if(selection==5)PlayerMessage(GetOwner(cont),"@<c fbfbfb>Ändert Weichenausrichtungen.",this());
if(selection==6)PlayerMessage(GetOwner(cont),"@<c fbfbfb>Deinstallisiert Weichen.",this());
Sound("menuclick",1,0,0,GetOwner(cont));
}

func ContainedDigDouble2_Closed()
{
if(!GetMenu(cont))cont=0;
PlayerMessage(GetOwner(cont)," ",this());
}

func ContainedDigDouble_Changed()
{
Sound("menuclick",1,0,0,GetOwner(cont));
}

func ContainedDigDouble_Closed()
{
if(!GetMenu(cont))cont=0;
PlayerMessage(GetOwner(cont)," ",this());
}

func DestroyWeiche(ID,controller)
{
var mc=CreateObject(SMEC,0,0,-1);
CreateMenu(ID,controller,this(),0,0,0,1);
mc->Init(controller,this(),"NewWeiche");
var a=0;var i=-1;
while(a=FindFriendlyObject(ST_1,GetOwner(controller),a)) {i++;mc->AddInfo(a);AddMenuItem(Format("%d. Weiche",i+1),"WeicheTot",ST_1,Par(1),0,a);}
}

func WeicheTot(ID,targetobj)
{
DoDamage(1000,targetobj);
}



func ChangeWeiche(ID,controller)
{
if(!FindFriendlyObject(ST_1,GetOwner(controller)))return(PlayerMessage(GetOwner(controller),"Keine befreundete Weiche gefunden",controller));
var mc=CreateObject(SMEC,0,0,-1);
CreateMenu(ID,controller,this(),0,0,0,1);
mc->Init(controller,this(),"NewWeiche");
var a=0;var i=-1;
while(a=FindFriendlyObject(ST_1,GetOwner(controller),a)) {i++;mc->AddInfo(a);AddMenuItem(Format("%d. Weiche",i+1),"WegSetWeich",ST_1,Par(1),0,a);}
}

func WegSetWeich(ID,tobj)
{
dot=tobj->Strecke();
var mc=CreateObject(SMEC,0,0,-1);
CreateMenu(ST_3,cont,Par(1),0,0,0,0,1);
mc->Init(cont,this(),"WegSetWeich");
var newa;
newa=CreateObject(CCSY,0,0,-1);
SetGraphics(0,newa,SY_1,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:1 CCW","NGradm",0,cont,0,1," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CCSY,0,0,-1);
SetGraphics(0,newa,SY_2,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:2 CCW","NGradm",0,cont,0,2," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CCSY,0,0,-1);
SetGraphics(0,newa,SY_3,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:3 CCW","NGradm",0,cont,0,3," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CCSY,0,0,-1);
SetGraphics(0,newa,SY12,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:4 CCW","NGradm",0,cont,0,4," ",4,newa);
RemoveObject(newa);

//hälfte

newa=CreateObject(CWSY,0,0,-1);
SetGraphics(0,newa,SY_1,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:1 CW","NGradp",0,cont,0,1," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CWSY,0,0,-1);
SetGraphics(0,newa,SY_2,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:2 CW","NGradp",0,cont,0,2," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CWSY,0,0,-1);
SetGraphics(0,newa,SY_3,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:3 CW","NGradp",0,cont,0,3," ",4,newa);
RemoveObject(newa);
newa=CreateObject(CWSY,0,0,-1);
SetGraphics(0,newa,SY12,1,GFXOV_MODE_Picture);
AddMenuItem("Weiche:4 CW","NGradp",0,cont,0,4," ",4,newa);
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
a=a%4;
a++;
if(! (a==hlp2))Sound("menuclick",1,0,0,GetOwner(cont));
hlp2=a;
dot=info->Strecke(0,a);
}

func WegSetWeich_Closed()
{
if(dot){dot->DelOldLine();RemoveObject(dot);}
dot=0;
var c=CenterWeich(ST_1,hlp1);
c->ShowSomething(0,hlp2);
hlp1=0;
if(!GetMenu(cont))cont=0;
}


func NewWeiche()
{
var a=0;var i=-1;
CreateMenu(QQQQ,Par(1),0,0,0,0,1);
var mc=CreateObject(SMEC,0,0,-1);
mc-> Init(Par(1),this(),"NewWeiche");
mc->AddInfo(this());
i++;
AddMenuItem(Format("%d. Banhof",i+1),"CenterBanhof",ST_4,Par(1),0,this());
while(a=FindFriendlyObject(ST_4,GetOwner(Par(1)),a)) {i++;mc->AddInfo(a);AddMenuItem(Format("%d. Banhof",i+1),"CenterBanhof",ST_4,Par(1),0,a);}
while(a=FindFriendlyObject(ST_1,GetOwner(Par(1)),a)) {i++;mc->AddInfo(a);AddMenuItem(Format("%d. Weiche",i+1),"CenterWeich",ST_1,Par(1),0,a);}
}

func NewWeiche_Changed(selection,info)
{
if(circle)RemoveObject(circle);
SetPlrView(GetOwner(cont),info);
var a=CreateObject(HCIR,0,0,GetOwner(cont));
SetPosition(GetX(info),GetY(info),a);
circle=a;
Sound("menuclick",1,0,0,GetOwner(cont));
}

func NewWeiche_Closed()
{
if(circle)RemoveObject(circle);
if(!GetMenu(cont))cont=0;
}

func CenterBanhof(ID,obj)
{
var a=CreateObject(NFWH,GetX(LocalN("rad",LocalN("rohr",obj)))-GetX(),GetY(LocalN("rad",LocalN("rohr",obj)))-GetY(),GetOwner(cont));
var mc=CreateObject(SMEC,0,0,-1);
LocalN("mother",a)=LocalN("rad",LocalN("rohr",obj));
LocalN("r",a)=GetR(LocalN("rad",rohr));
a->DoIt();
dot=a;
CreateMenu(ST_2,cont,a,0,0,0,0,1);
mc->Init(cont,this(),"CenterBanhof");
AddMenuItem("Links","SetDreh2",SY_8,cont,0,-90," ");
AddMenuItem("Rechts","SetDreh2",SY_9,cont,0,90," ");
AddMenuItem("Radius -10","Radius",SY_7,cont,10,-10," ");
AddMenuItem("Radius +10","Radius",SY_6,cont,10,+10," ");
AddMenuItem("Feldweiche platzieren","Feldweicheplatz",ST_1,cont,0,cont," ");
AddLeerItem(1,cont);
Schedule("SetMenuSize(4,2,cont)",10);
SetPlrView(GetOwner(cont),obj);
}

func CenterBanhof_Changed()
{
Sound("menuclick",1,0,0,GetOwner(cont));
}

func CenterBanhof_Closed()
{
if(dot){dot->DelOldLine();RemoveObject(dot);}
dot=0;
if(!GetMenu(cont))cont=0;
}

func CenterWeich(ID,obj)
{
var a=CreateObject(NFWH,GetX(obj)-GetX(),GetY(obj)-GetY(),GetOwner(cont));
LocalN("mother",a)=obj;
dot=a;
a->DoIt(0);
var mc=CreateObject(SMEC,0,0,-1);
CreateMenu(ID,cont,a,0,0,0,0,1);
mc->Init(cont,this(),"CenterWeich");
AddMenuItem("-1°","Dreh",SY_8,cont,1,-1," ");
AddMenuItem("+1°","Dreh",SY_9,cont,1,1," ");
AddMenuItem("-10°","Dreh",SY_8,cont,10,-10," ");
AddMenuItem("+10°","Dreh",SY_9,cont,10,10," ");
AddMenuItem("Radius -10","Radius",SY_7,cont,10,-10," ");
AddMenuItem("Radius +10","Radius",SY_6,cont,10,+10," ");
AddMenuItem("Feldweiche platzieren","Feldweicheplatz",ST_1,cont,0,cont," ");
AddLeerItem(1,cont);
AddMenuItem("Richtung Nr1","ShowSomething",SY_1,cont,0,1," ");
AddMenuItem("Richtung Nr2","ShowSomething",SY_2,cont,0,2," ");
AddMenuItem("Richtung Nr3","ShowSomething",SY_3,cont,0,3," ");
AddMenuItem("Richtung Nr4","ShowSomething",SY12,cont,0,4," ");
SetPlrView(GetOwner(cont),a);
//AddMenuItem("Strecke aufleuchten","Magicway",0,cont,0,0," ");
Schedule("SetMenuSize(4,3,cont)",10);
return(a);
}

func CenterWeich_Changed()
{
Sound("menuclick",1,0,0,GetOwner(cont));
}

func CenterWeich_Closed()
{
if(dot){dot->DelOldLine();RemoveObject(dot);}
dot=0;
if(!GetMenu(cont))cont=0;
}

func FindFriendlyObject(ID,player,lastobj)
{
var a;
if(a=FindObject(ID,0,0,0,0,0,0,0,0,lastobj)) { if(!Hostile(player,GetOwner(a))) return(a); }
else return(0);
}


//STRECKENRICHTUNG/ HALTEZEIT/ LADEOPTIONEN
func Auswahl()
{
  strecke = Par(1);
  //Streckenmenü öffnen
  ObjectCall(rohr,"SetStrecke",0,Par(1));
  var mc=CreateObject(SMEC,0,0,-1);
  CreateMenu(ST_1, cont, this(), 0, "Strecken konfigurieren", 0, 0, 0);
    mc->Init(cont,this(),"ContainedDigDouble");
    AddMenuItem("Richtung ändern", "SetRichtung", SY_5, cont, 0, Par(1), " ");
    AddMenuItem("Haltezeit einstellen", "SetZeit", SY_4, cont, 0,Par(1), " ");
    AddMenuItem("be/entladen", "SetLaden", SY10, cont, 0, Par(1), " ");
    AddMenuItem("Streckenwechsel", "SetNewStrecke", SY11, cont, 0, Par(1), " ");
  return(1);
}


//STRECKENRICHTUNG
func SetRichtung()
{
  //ObjectCall(rohr,"SetStrecke",0,Par(1));
  CreateMenu(ST_1, cont, this(), 0, "Strecken konfigurieren", 0, 1, 1);
    AddMenuItem("links", "ObjRichtung", SY_8, cont, 0, 1, " ");
    AddMenuItem("rechts", "ObjRichtung", SY_9, cont, 0, 2, " ");
  return(1);
}
//RICHTUNG ÜBERTRAGEN
func ObjRichtung()
{
  if(Par(1) == 1) ObjectCall(rohr,"Links");
  if(Par(1) == 2) ObjectCall(rohr,"Rechts");
  return(1);
}


//HALTEZEIT
func SetZeit()
{
  ObjectCall(rohr,"GetZeit",0,Par(1));
  CreateMenu(ST_1, cont, this(), 0, "Haltezeit einstellen", 0, 1, 1);
    AddMenuItem("+30 Frames", "ObjZeitPlus", SY_6, cont, 0, Par(1), " ");
    AddMenuItem("-30 Frames", "ObjZeitMinus", SY_7, cont, 0, Par(1), " ");
    AddMenuItem("30 Frames", "ObjZeit30", SYNU, cont, 0, Par(1), " ", 0,10);
    AddMenuItem("Unbegrenzter Aufenthalt", "ObjZeitUnend", SYNU, cont, 0, Par(1), " ", 0,0);
  return(1);
}
//ZEIT ÜBERTRAGEN +
func ObjZeitPlus()
{ 
  if(Par(1) == 1 && zeit1 < 3030 ) ObjectCall(rohr,"Zeit",1,zeit1 += 30);
  if(Par(1) == 2 && zeit2 < 3030 ) ObjectCall(rohr,"Zeit",2,zeit2 += 30);
  if(Par(1) == 3 && zeit3 < 3030 ) ObjectCall(rohr,"Zeit",3,zeit3 += 30);
  if(Par(1) == 4 && zeit4 < 3030 ) ObjectCall(rohr,"Zeit",4,zeit4 += 30);
  return(1);
}
//ZEIT UNENDLICH
func ObjZeitUnend()
{ 
  if(Par(1) == 1) ObjectCall(rohr,"Zeit",1,zeit1=3030);
  if(Par(1) == 2) ObjectCall(rohr,"Zeit",2,zeit2=3030);
  if(Par(1) == 3) ObjectCall(rohr,"Zeit",3,zeit3=3030);
  if(Par(1) == 4) ObjectCall(rohr,"Zeit",4,zeit4=3030);
  return(1);
}
//ZEIT 30
func ObjZeit30()
{ 
  if(Par(1) == 1) ObjectCall(rohr,"Zeit",1,zeit1=30);
  if(Par(1) == 2) ObjectCall(rohr,"Zeit",2,zeit2=30);
  if(Par(1) == 3) ObjectCall(rohr,"Zeit",3,zeit3=30);
  if(Par(1) == 4) ObjectCall(rohr,"Zeit",4,zeit4=30);
  return(1);
}
//ZEIT ÜBERTRAGEN -
func ObjZeitMinus()
{
  if(Par(1) == 1 && zeit1 > 30 ) ObjectCall(rohr,"Zeit",1,zeit1 -= 30);
  if(Par(1) == 2 && zeit2 > 30 ) ObjectCall(rohr,"Zeit",2,zeit2 -= 30);
  if(Par(1) == 3 && zeit3 > 30 ) ObjectCall(rohr,"Zeit",3,zeit3 -= 30);
  if(Par(1) == 4 && zeit4 > 30 ) ObjectCall(rohr,"Zeit",4,zeit4 -= 30);
  return(1);
}


//BE/ENTLADEN
func SetLaden()
{
  var x;
  strecke = Par(1);
  if(strecke == 1 ) x = LocalN("laden1",rohr);
  if(strecke == 2 ) x = LocalN("laden2",rohr);
  if(strecke == 3 ) x = LocalN("laden3",rohr);
  if(strecke == 4 ) x = LocalN("laden4",rohr);
  Message("<c fbfbfb>Strecke %d %s.",this(),strecke,ObjectCall(rohr,"GetAusdruck",x));
  CreateMenu(ST_1, cont, this(), 0, "Ladeoptionen", 0, 1, 1);
    AddMenuItem(" Sphäre beladen", "Laden", SY_6, cont, 0, 1, " ");
    AddMenuItem("Sphäre entladen", "Laden", SY_7, cont, 0, 2, " ");
    AddMenuItem("Keine Aktion", "Laden", SYCC, cont, 0, 0, " ");
  return(1);
} 
//LADEOPTIONEN WEITERLEITEN
func Laden()
{
  if(strecke == 1 ) LocalN("laden1",rohr) = Par(1);
  if(strecke == 2 ) LocalN("laden2",rohr) = Par(1);
  if(strecke == 3 ) LocalN("laden3",rohr) = Par(1);
  if(strecke == 4 ) LocalN("laden4",rohr) = Par(1);
  Message("<c fbfbfb>Strecke %d %s.",this(),strecke,ObjectCall(rohr,"GetAusdruck",(Par(1))));
  return(1);
 }


//Streckenwechsel
func SetNewStrecke()
{
var x;
 if(strecke == 1 ) x = LocalN("wechsel1",rohr);
 if(strecke == 2 ) x = LocalN("wechsel2",rohr);
 if(strecke == 3 ) x = LocalN("wechsel3",rohr);
 if(strecke == 4 ) x = LocalN("wechsel4",rohr);
 if(strecke == x) Message("<c fbfbfb>Strecke %d: kein Wechsel",this(),strecke);
 else(Message("<c fbfbfb>Strecke %d: Wechsel auf Strecke %d.",this(),strecke,x));
  CreateMenu(ST_1, cont, this(), 0, "Ladeoptionen", 0, 0, 1);
    AddMenuItem("Auf Strecke 1 wechseln. ACHTUNG!:Der Streckenwechsel findet VOR dem Andocken statt, daher gelten am Bahnhof Richtungs,Abfarts-und Ladeeinstellungen der NEUEN Strecke !!!", "NewStrecke", SY_1, cont, 0, 1, " ");
    AddMenuItem("Auf Strecke 2 wechseln. ACHTUNG!:Der Streckenwechsel findet VOR dem Andocken statt, daher gelten am Bahnhof Richtungs,Abfarts-und Ladeeinstellungen der NEUEN Strecke !!!", "NewStrecke", SY_2, cont, 0, 2, " ");
    AddMenuItem("Auf Strecke 3 wechseln. ACHTUNG!:Der Streckenwechsel findet VOR dem Andocken statt, daher gelten am Bahnhof Richtungs,Abfarts-und Ladeeinstellungen der NEUEN Strecke !!!", "NewStrecke", SY_3, cont, 0, 3, " ");
    AddMenuItem("Auf Strecke 4 wechseln. ACHTUNG!:Der Streckenwechsel findet VOR dem Andocken statt, daher gelten am Bahnhof Richtungs,Abfarts-und Ladeeinstellungen der NEUEN Strecke !!!", "NewStrecke", SY12, cont, 0, 4, " ");
  return(1);
 } 
//Streckenwechsel weitergeben
func NewStrecke()
{
  if(strecke == 1 ) LocalN("wechsel1",rohr) = Par(1);
  if(strecke == 2 ) LocalN("wechsel2",rohr) = Par(1);
  if(strecke == 3 ) LocalN("wechsel3",rohr) = Par(1);
  if(strecke == 4 ) LocalN("wechsel4",rohr) = Par(1);
  if(strecke == Par(1)) return(Message("<c fbfbfb>Strecke %d: kein Wechsel",this(),strecke));
  Message("<c fbfbfb>Strecke %d: Wechsel auf Strecke %d.",this(),strecke,(Par(1)));
  return(1);
 }

 
/* Inhalt von Loren übernehmen */

func DoorClosed()
{
  var num = -1;
  var obj;
  while (obj = Contents(++num))
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

public StatusBarID: return(SBR0);
public StatusBarPos: return(2);
public GetMaxDamage: return(75);


