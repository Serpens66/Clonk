/*-- Lore3 --*/

#strict

/* Status */

func Initialize(){
SetList();
Local(10)=CreateObject(LOR2);
Local(6)=((GetY()+10)/11)*11; //Sorgt dafür, dass alle Freigegrabenen stellen zusammenpassen.
Local(1)=(GetX()/11)*11;
Local(2)=11;
Local(3)=11;

Local(0,Local(10))=this();
Local(6,Local(10))=((GetY()+10)/11)*11;
Local(1,Local(10))=(GetX()/11)*11;
Local(2,Local(10))=11;
Local(3,Local(10))=11;
return(1);}

func Destruction()
{
if(Local(10)) RemoveObject(Local(10));
}

public func IsLorry() { return(1); }

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
CreateMenu(GetID(),pClonk,this(),0,0,0,3,1);
AddMenuItem("Links","Ecke",SY1M,pClonk,0,1,"Verschiebt die Linke Obere Ecke nach Links",2,0,0);
AddMenuItem("Rechts","Ecke",SY1M,pClonk,0,2,"Verschiebt die Linke Obere Ecke nach Rechts",2,1,0);

AddMenuItem("Hoch","Ecke",SY1M,pClonk,0,3,"Verschiebt die Linke Obere Ecke nach Links",2,8,0);
AddMenuItem("Runter","Ecke",SY1M,pClonk,0,4,"Verschiebt die Linke Obere Ecke nach Rechts",2,9,0);


AddMenuItem("Breiter","Breite",SY1M,pClonk,0,1,"Macht den Bereich breiter",2,2,0);
AddMenuItem("Schmaler","Breite",SY1M,pClonk,0,2,"Macht den Bereich schmaler",2,3,0);

AddMenuItem("Tiefer","Tiefe",SY1M,pClonk,0,1,"Macht den Bereich tiefer",2,4,0);
AddMenuItem("Flacher","Tiefe",SY1M,pClonk,0,2,"Macht den Bereich flacher",2,5,0);

AddMenuItem("Unter Lore Plazieren","Ausrichten",SY1M,pClonk,0,0,"Setzt den Bereich unter die Lohre",2,10,0);
AddMenuItem("Rahmen An/Aus","Rahmen",LOR2,pClonk,0,0,"Schaltet den Rahmen ein und aus",2,0,0);

AddMenuItem("(Re)Start","Start",SY1M,pClonk,0,1,"Startet das Ausheben des Lochs",2,6,0);
AddMenuItem("Stop","Start",SY1M,pClonk,0,2,"Stopt das Ausheben des Lochs",2,7,0);

AddMenuItem("Inhaltsinfo","Inhalt",GetID(this()),pClonk,0,pClonk,"Zeigt die Mangen der enthaltenen Materialien",2,0,0);
AddMenuItem("Nur %-angaben anzeigen EIN/AUS","SetMes",GetID(this()),pClonk,0,pClonk,"Schaltet den angezeigten Text um.",2,0,0);

AddMenuItem("Modus wechseln","Weechsel",LOR1,pClonk,0,pClonk,"Wechselt zum Materialsauger");

// Selektion wiederherstellen... funktioniert iwie nicht...
  SelectMenuItem(zahl,pClonk);

return(1);}



func Weechsel(par1,pClonk)
{
var zahahl = ContentsCount(0,this());
var neueLore = CreateObject(LOR1);
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

9 Nur Prozent anzeigen

25 maximale materialienzahl
20 Zu benutzendes Material (int)

100-199 MaterialNamen
200-299 Materialmengen
300-399 ID-Materialklumpen

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

func SetList(){
Local(25)=8;
Local(20)=0;

Local(100)="Ashes";
Local(200)=0;
Local(300)=C4Id("ASHS");

Local(101)="Rock";
Local(201)=0;
Local(301)=C4Id("CNCR");

Local(102)="Earth";
Local(202)=0;
Local(302)=C4Id("ERTH");

Local(103)="Ice";
Local(203)=0;
Local(303)=C4Id("ICE1");

Local(104)="Granite";
Local(204)=0;
Local(304)=C4Id("LIQG");

Local(105)="FlySand";
Local(205)=0;
Local(305)=C4Id("SAND");

Local(106)="Sand";
Local(206)=0;
Local(306)=C4Id("SAND");

Local(107)="Snow";
Local(207)=0;
Local(307)=C4Id("SNWB");

return(1);}

func Inhalt(ID,par,right){
CreateMenu(GetID(this()),par,this(),0,0,0,1,0);
AddMenuItem("Menu aktualisieren","Select",GetID(this()),par,0,par,0,128,0,-1);
for(var i=0;i<Local(25);i++)
	{
	if(i==Local(20)){
	AddMenuItem(Format("%10s  +",Local(100+i)),"Select",Local(300+i),par,Local(200+i),par,0,128,0,i);}
	else{
	AddMenuItem(Format("%10s  -",Local(100+i)),"Select",Local(300+i),par,Local(200+i),par,0,128,0,i);}
	}
AddMenuItem("Menü schließen           ","menuclose",GetID(this()),par,0,par,0,0,0,0);

return(1);}

func Select(ID,par,right,val){
if(val==-1){
Inhalt(0,par,0);
SelectMenuItem(0);
}else{
Local(20)=val;
Inhalt(0,par,0);
SelectMenuItem(val+1,par);}
return(1);}

func menuclose(ID,par,right,val){
CloseMenu(par);
return(1);}

func CheckContent(){
var mat;
var mat1;
for(var i=0;i<ContentsCount();i++)
	{
	for(var x=0;x<Local(25);x++)
		{
		mat=Local(100+x);
		mat1=Contents(i)-> ~ MaterialNameString();
		if(mat S= mat1)
			{
			if(Local(200+x)<1000){
			Local(200+x)+=Contents(i)-> ~ MaterialMange(); //Screibfehler(bleibt aber so) :)
			RemoveObject(Contents(i));
			return(1);
			}}
		}
	}
return(1);}


func timer(){
var obj;
var pro;
var col;
var tunnel;
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

CheckContent();
if(!Local(10)){Local(10)=CreateObject(LOR2);
	Local(0,Local(10))=this();}
if(Local(0)==1){

if(FindObject(ENRG)){
if(GetEnergy() < 1){
SetAction("NeedEnergy");
return(Message("Nicht genügend Energie",this()));}}

Local(11)++;
if(Local(11)>=100){DoEnergy(-1); Local(11)=0;}

	if(Materialvorrat()>0){

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
	if(Free(Local(4),Local(5))==1)
		{
		LineTo(Local(4)-GetX(),Local(5)-GetY(),RGB(GetMaterialColor(Material(DefMat()),0,0),GetMaterialColor(Material(DefMat()),0,1),GetMaterialColor(Material(DefMat()),0,2)));

		MatSub();
		tunnel=1;
		if(GetMaterial(Local(4)-GetX(),Local(5)-GetY())==-1){tunnel=0;}

		DrawMaterialQuad(Format("%s-SMOOTH",DefMat()),Local(4),Local(5),Local(4)+1,Local(5),Local(4),Local(5),Local(4),Local(5)+1,tunnel);

		}

	Local(4)++;
	if(Local(4)==Local(1)+Local(2)){
		Local(4)=Local(1);
		Local(5)++;
		}

	if(Local(5)==Local(3)+Local(6)){Start(0,2,0);}
	}
	else
		{
		Message("Bitte Material nachfüllen oder anderes Material auswählen", this());		 
		}

}
//if(ContentsCount()>89){Message("!!Warnung!!|Lagerkapazität erreicht",this());}
//if(ContentsCount()>99){Exit(Contents(0,this()),0,5); }
if(Contained()){Contained()->GrabContents(this());}
return(1);}

func LineTo(x,y,c){
DrawParticleLine("Mark2",x,y,0,0,5,10,c,c,0);
return(1);}

func MatSub(){
Local(200+Local(20))--;
return(1);}

func Materialvorrat(){
return(Local(200+Local(20)));}

func Free(int x,int y){
var frei;
frei=1;
if(GBackSemiSolid(x-GetX(),y-GetY())){frei=0;}
return(frei);}

func DefMat() {
return(Local(100+Local(20)));
}
