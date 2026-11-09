/*-- Lore2 --*/

#strict

/* Status */

local zeehler, materialaarthalb,materialaartfest,materialaartwasser;

func Initialize(){
materialaarthalb=1;
Local(10)=CreateObject(LOR2);
Local(6)=((GetY()+10)/11)*11;
Local(1)=(GetX()/11)*11;
Local(2)=11;
Local(3)=11;

Local(0,Local(10))=this();
Local(6,Local(10))=((GetY()+10)/11)*11;
Local(1,Local(10))=(GetX()/11)*11;
Local(2,Local(10))=11;
Local(3,Local(10))=11;
return(1);}

public func IsLorry() { return(1); }


func Destruction()
{
if(Local(10)) RemoveObject(Local(10));
}

/* Steuerung */

protected func ContactLeft() {
  if(Stuck() && !Random(5)) SetRDir(RandomX(-7, +7));
}

protected func ContactRight() {
  if(Stuck() && !Random(5)) SetRDir(RandomX(-7, +7));
}

private func ControlElevator(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(0, +1,+1,0,0, OCF_Grab(), 0,0, NoContainer(), pElev))
    // Im Fahrstuhlkorb
    if(pElev->~IsElevator())
      // Steuerung an Fahrstuhl weiterleiten
      return(ObjectCall(pElev,szCommand,pObject));
  return(0);
}

private func ControlElevatorMovement(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(0, +1,+1,0,0, OCF_Grab(), 0,0, NoContainer(), pElev))
    // Fahrstuhlkorb gefunden
    if(pElev->~IsElevator())
      // Lore ist angehalten
      if (GetComDir() == COMD_Stop())
        // Steuerung an Fahrstuhl weiterleiten
        return(ObjectCall(pElev,szCommand,pObject));
  return(0);
}

private func ControlElevatorStop(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(0, +1,+1,0,0, OCF_Grab(), 0,0, NoContainer(), pElev))
    // Fahrstuhlkorb gefunden
    if(pElev->~IsElevator())
      // Fahrstuhlkorb bewegt sich
      if (GetComDir(pElev) != COMD_Stop())
      {
        // Fahrstuhlkorb anhalten
        ObjectCall(pElev,szCommand,pObject); 
        // Noch nicht aussteigen
        SetComDir(COMD_Stop(), pObject);
        SetComDir(COMD_Stop());
        // Bewegunsbefehl abbrechen
        return(1);
      }
  return(0);
}

public func ControlDigDouble(pClonk,zahl) {
Saugermenu(pClonk,zahl);
}


func Saugermenu(pClonk,zahl)
{
CreateMenu(GetID(),pClonk,this(),0,0,0,1,1);

AddMenuItem("Links","Ecke",SY2M,pClonk,0,1,"Verschiebt die Linke Obere Ecke nach Links",2,0,0);
AddMenuItem("Rechts","Ecke",SY2M,pClonk,0,2,"Verschiebt die Linke Obere Ecke nach Rechts",2,1,0);

AddMenuItem("Hoch","Ecke",SY2M,pClonk,0,3,"Verschiebt die Linke Obere Ecke nach Links",2,8,0);
AddMenuItem("Runter","Ecke",SY2M,pClonk,0,4,"Verschiebt die Linke Obere Ecke nach Rechts",2,9,0);


AddMenuItem("Breiter","Breite",SY2M,pClonk,0,1,"Macht den Bereich breiter",2,2,0);
AddMenuItem("Schmaler","Breite",SY2M,pClonk,0,2,"Macht den Bereich schmaler",2,3,0);

AddMenuItem("Tiefer","Tiefe",SY2M,pClonk,0,1,"Macht den Bereich tiefer",2,4,0);
AddMenuItem("Flacher","Tiefe",SY2M,pClonk,0,2,"Macht den Bereich flacher",2,5,0);

AddMenuItem("Unter Lore Plazieren","Ausrichten",SY2M,pClonk,0,0,"Setzt den Bereich unter die Lohre",2,10,0);
AddMenuItem("Rahmen An/Aus","Rahmen",LOR2,pClonk,0,0,"Schaltet den Rahmen ein und aus",2,0,0);

AddMenuItem("(Re)Start","Start",SY2M,pClonk,0,1,"Startet das Ausheben des Lochs",2,6,0);
AddMenuItem("Stop","Start",SY2M,pClonk,0,2,"Stopt das Ausheben des Lochs",2,7,0);
AddMenuItem("Nur %-angaben anzeigen EIN/AUS","SetMes",GetID(this()),pClonk,0,pClonk,"Schaltet den angezeigten Text um.",2,0,0);

if(GetPlrKnowledge(GetOwner(),LO33)) // wenn erforscht, dann materialien einstellbar
{
AddMenuItem("Halbfestes Material","Materialwechsel",ERTH,pClonk,materialaarthalb,pClonk,"Wenn aktiviert, wird grabbares Material eingesaugt");
AddMenuItem("Festes Material","Materialwechsel",ROCK,pClonk,materialaartfest,pClonk,"Wenn aktiviert, wird nichtgrabbares Material eingesaugt");
AddMenuItem("Flüssiges Material","Materialwechsel",WBRL,pClonk,materialaartwasser,pClonk,"Wenn aktiviert, wird flüssiges Material eingesaugt, zum lagern werden Fässer im Sauger benötigt");
}
AddMenuItem("Modus wechseln","Weechsel",LOR3,pClonk,0,pClonk,"Wechselt zum Materialformer");
// Selektion wiederherstellen... funktioniert iwie nicht...
  SelectMenuItem(zahl,pClonk);
return(1);}

func Materialwechsel(ID,pClonk)
{

if(ID==ERTH)
 {
 if(materialaarthalb==0)
{materialaarthalb=1;
CloseMenu(pClonk);
Saugermenu(pClonk,13);
return();}
 if(materialaarthalb==1)
{materialaarthalb=0;
CloseMenu(pClonk);
Saugermenu(pClonk,13);
return();}
 }

if(ID==ROCK)
 {
 if(materialaartfest==0)
{materialaartfest=1;
CloseMenu(pClonk);
Saugermenu(pClonk,14);
return();}
 if(materialaartfest==1)
{materialaartfest=0;
CloseMenu(pClonk);
Saugermenu(pClonk,14);
return();}
 }

if(ID==WBRL)
 {
 if(materialaartwasser==0)
{materialaartwasser=1;
CloseMenu(pClonk);
Saugermenu(pClonk,15);
return();}
 if(materialaartwasser==1)
{materialaartwasser=0;
CloseMenu(pClonk);
Saugermenu(pClonk,15);
return();}
 }
}

func Weechsel(par1,pClonk)
{
var zahahl = ContentsCount(0,this());
var neueLore = CreateObject(LOR3);
while (0<zahahl)
 {
var inhaalt = Contents(0,0);
if(!inhaalt) break;
Enter(neueLore,inhaalt);
zahahl = ContentsCount(0,this());
 }
CloseMenu(pClonk);
//pClonk->~ControlDownDouble();
//SetCommand(pClonk, "Grab", neueLore);
RemoveObject();
return();
}



func SetMes(){
if(Local(9)==0)Local(9)=1; else Local(9)=0;
return(1);}

func Ecke(id idItem, iParameter,bRight){
if(Local(0)==1){
Message("Vorgang läuft...|Änderungen|nicht|möglich.",this());
return(Rahmen(1));}
if(iParameter==1){
Local(1)-=11;
if(bRight==1){Local(1)-=11;}}
if(iParameter==2){
Local(1)+=11;
if(bRight==1){Local(1)+=11;}}
if(iParameter==3){
Local(6)-=11;
if(bRight==1){Local(6)-=11;}}
if(iParameter==4){
Local(6)+=11;
if(bRight==1){Local(6)+=11;}}
UpdateHelper();
Rahmen(1);
return(1);}

func Breite(id idItem, iParameter,bool bRight){
if(Local(0)==1){
Message("Vorgang läuft...|Änderungen|nicht|möglich.",this());
return(Rahmen(1));}
if(iParameter==1){
Local(2)+=11;
if(bRight==1){Local(2)+=11;}}
if(iParameter==2){
Local(2)-=11;
if(bRight==1){Local(2)-=11;}}
if(Local(2)<11){Local(2)=11;}
UpdateHelper();
Rahmen(1);
return(1);}

func Tiefe(id idItem, iParameter,bool bRight){
if(Local(0)==1){
Message("Vorgang läuft...|Änderungen|nicht|möglich.",this());
return(Rahmen(1));}
if(iParameter==1){
Local(3)+=11;}
if(iParameter==2){
Local(3)-=11;}
if(Local(3)<11){Local(3)=11;}
UpdateHelper();
Rahmen(1);
return(1);}

func Ausrichten(){
if(Local(0)==1){
Message("Vorgang läuft...|Änderungen|nicht|möglich.",this());
return(Rahmen(1));}
Local(6)=((GetY()+10)/11)*11;
Local(1)=(GetX()/11)*11;
Local(2)=11;
Local(3)=11;
UpdateHelper();
Rahmen(1);
return(1);}

func Start(id idItem, iParameter,bool bRight){
Local(4)=Local(1);
Local(5)=Local(6);
Local(7)=Local(2)*Local(3);
Local(8)=0;
if(iParameter==1){Local(0)=1;}
if(iParameter==2){Local(0)=0;}
return(1);}

func UpdateHelper(){
Local(6,Local(10))=Local(6);
Local(1,Local(10))=Local(1);
Local(2,Local(10))=Local(2);
Local(3,Local(10))=Local(3);
return(1);}

func Rahmen(Temp){
if(Temp==1){
Local(11,Local(10))=50;
}
else
{
if(Local(10,Local(10))==1){Local(10,Local(10))=0;}
else
{Local(10,Local(10))=1;}
}
return(1);}
/*
Local
0 An/Aus
1 X-Pos
6 Y-Pos
2 Breite
3 Tiefe
4 Aktuell X
5 Aktuell Y

7 Gesammtmenge
8 Bereits freigegraben
10 helper

Mengen
10 Ashes (200)
11 Coal (300)
12 Earth (200)
13 FlyAshes (200)
14 FlySand (200)
15 Ice (400)
16 Sand (200)
17 Snow (400)
18 Sulphur (200)
*/

func timer(){
var obj;
var pro;
var col;
var batt;


if(Local(0)==0)
SetAction("Idle");

if(Local(0)==1)
if(FindObject(ENRG)){
batt=FindObject(ADM3,0,0,-1,-1,0,0,0,this());
if(batt!=0){
if(GetEnergy() < 1 && GetEnergy(batt)>0){
DoEnergy(GetEnergy(batt));
DoEnergy(-GetEnergy(batt),batt); // Batterie entleeren
}}}




if(!Local(10)){Local(10)=CreateObject(LOR2);
Local(0,Local(10))=this();}
if(Local(0)==1){

if(FindObject(ENRG)){
if(GetEnergy() < 1){
SetAction("NeedEnergy");
return(Message("Nicht genügend Energie",this()));}}

Local(11)++;
if(Local(11)>=100){DoEnergy(-1); Local(11)=0;}

Local(8)++;

pro=Local(8)*100/Local(7);
col=RGB(255-(pro*255/100),(pro*255/100),0);

	if(Local(9)==0)
		{
		Message("Fertigstellung:| <c %x> %d % </c> |(%d von %d Pixeln)|Noch %d Pixel", this(),col,pro,Local(8),Local(7),Local(7)-Local(8));
		}
		else
		{
		Message("Fertigstellung:| <c %x> %d % </c>", this(),col,pro);
		}

CreateParticle("Mark2",Local(4)-GetX(),Local(5)-GetY(),0,0,10,RGB(255,0,255));

if(materialaarthalb==1)
if(Digable(Local(4),Local(5))==1)
{
LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(GetMaterialColor(GetMaterial(Local(4)-GetX(),Local(5)-GetY()),0,0),GetMaterialColor(GetMaterial(Local(4)-GetX(),Local(5)-GetY()),0,1),GetMaterialColor(GetMaterial(Local(4)-GetX(),Local(5)-GetY()),0,2)));
MatAdd(GetMaterial(Local(4)-GetX(),Local(5)-GetY()));
obj=CreateObject(ROCK,Local(4),Local(5),NO_OWNER());
obj->SetVisibility(VIS_None());
obj->DigFreeRect(Local(4),Local(5),1,1); 
RemoveObject(obj);
}

if(materialaartfest==1)
if(Digable(Local(4),Local(5))==2)   // wenn nicht grabbar
{
LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(GetMaterialColor(GetMaterial(Local(4)-GetX(),Local(5)-GetY()),0,0),GetMaterialColor(GetMaterial(Local(4)-GetX(),Local(5)-GetY()),0,1),GetMaterialColor(GetMaterial(Local(4)-GetX(),Local(5)-GetY()),0,2)));
MatAdd(GetMaterial(Local(4)-GetX(),Local(5)-GetY()));
obj=CreateObject(ROCK,Local(4),Local(5),NO_OWNER());
obj->SetVisibility(VIS_None());
//obj->DigFreeRect(Local(4),Local(5),1,1); 
obj->FreeRect(Local(4),Local(5),1,1); 
RemoveObject(obj);
}

if(materialaartwasser==1)
if(Digable(Local(4),Local(5))==3)   // wenn flüssig
{
LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(GetMaterialColor(GetMaterial(Local(4)-GetX(),Local(5)-GetY()),0,0),GetMaterialColor(GetMaterial(Local(4)-GetX(),Local(5)-GetY()),0,1),GetMaterialColor(GetMaterial(Local(4)-GetX(),Local(5)-GetY()),0,2)));
MatAdd(GetMaterial(Local(4)-GetX(),Local(5)-GetY()));
obj=CreateObject(ROCK,Local(4),Local(5),NO_OWNER());
obj->SetVisibility(VIS_None());
//obj->DigFreeRect(Local(4),Local(5),1,1); 
obj->FreeRect(Local(4),Local(5),1,1); 
RemoveObject(obj);
}


/*
if(GetMaterial(Local(4)-GetX(),Local(5)-GetY())==Material("Ashes")){Local(10)++; LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(50,50,50));}
if(GetMaterial(Local(4)-GetX(),Local(5)-GetY())==Material("Coal")){Local(11)++; LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(10,10,10));}
if(GetMaterial(Local(4)-GetX(),Local(5)-GetY())==Material("Earth")){Local(12)++; LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(200,160,10));}
if(GetMaterial(Local(4)-GetX(),Local(5)-GetY())==Material("FlyAshes")){Local(13)++; LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(50,50,50));}
if(GetMaterial(Local(4)-GetX(),Local(5)-GetY())==Material("FlySand")){Local(14)++; LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(225,210,35));}
if(GetMaterial(Local(4)-GetX(),Local(5)-GetY())==Material("Ice")){Local(15)++; LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(102,196,255));}
if(GetMaterial(Local(4)-GetX(),Local(5)-GetY())==Material("Sand")){Local(16)++; LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(225,210,35));}
if(GetMaterial(Local(4)-GetX(),Local(5)-GetY())==Material("Snow")){Local(17)++; LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(255,255,255));}
if(GetMaterial(Local(4)-GetX(),Local(5)-GetY())==Material("Sulphur")){Local(18)++; LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(240,210,0));}
obj=CreateObject(ROCK,Local(4),Local(5),NO_OWNER());
obj->SetVisibility(VIS_None());
obj->DigFreeRect(Local(4),Local(5),1,1); 
RemoveObject(obj);
*/


Local(4)++;
if(Local(4)==Local(1)+Local(2)){
Local(4)=Local(1);
Local(5)++;
                               }

if(Local(5)==Local(3)+Local(6)){Start(0,2,0);}
}
if(ContentsCount()>89){Message("!!Warnung!!|Lagerkapazität erreicht",this());}
if(ContentsCount()>99){Exit(Contents(0,this()),0,5); }
if(Contained()){Contained()->GrabContents(this());}
return(1);
}

func LineTo(x,y,c){
DrawParticleLine("Mark2",x,y,0,0,5,10,c,c,0);
return(1);}

func MatAdd(int Mat){
var max;
var ID;
max=GetMaterialVal("Dig2ObjectRatio","Material",Mat);
if(max)
 {
Local(100+Mat)++;
if(Local(100+Mat)==max){
ID=GetMaterialVal("Dig2Object","Material",Mat); 
if(ID!=0){CreateContents(ID);}
Local(100+Mat)=0;
                       }
  }
if(!max)
  {
max=GetMaterialVal("Blast2ObjectRatio","Material",Mat);
Local(100+Mat)++;
if(Local(100+Mat)==max){
ID=GetMaterialVal("Blast2Object","Material",Mat); 
if(ID!=0){CreateContents(ID);}
Local(100+Mat)=0;
                       }
  }

if(!max)
if(Mat==Material("Oil") || Mat==Material("Water") || Mat==Material("Acid") || Mat==Material("Lava") || Mat==Material("DuroLava"))
   {
max=3;
Local(100+Mat)++;
if(Local(100+Mat)==max){
var fasss = FindContents(BARL);
if(fasss) ObjectInsertMaterial(Mat, this());
Local(100+Mat)=0;
                       }
   }

return(1);}

func Digable(int x,int y){
var mat;
mat=GetMaterial(x-GetX(),y-GetY());
if(GetPlrKnowledge(GetOwner(),LO33))   // auch festes material abbaubar
  {
if(mat==Material("Oil") || mat==Material("Water") || mat==Material("Acid") || mat==Material("Lava") || mat==Material("DuroLava")) return(3); // flüssiges
if(GetMaterialVal("DigFree","Material",mat)) return(1);   // wenn grabbar
if(!GetMaterialVal("DigFree","Material",mat)) return(2);  //wenn nicht grabbar
  }
return(GetMaterialVal("DigFree","Material",mat));
}