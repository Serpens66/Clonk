/*-- Lore3 --*/

#strict 3

/* Status */

func Initialize(){
SetList();
Local(10)=CreateObject(LOR2);
Local(6)=(((GetY() ?? 0)+10)/11)*11; //Sorgt dafür, dass alle Freigegrabenen stellen zusammenpassen.
Local(1)=((GetX() ?? 0)/11)*11;
Local(2)=11;
Local(3)=11;

Local(nil,Local(10))=this();
Local(6,Local(10))=(((GetY() ?? 0)+10)/11)*11;
Local(1,Local(10))=((GetX() ?? 0)/11)*11;
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
  if(And(Stuck(), !Random(5))) SetRDir(RandomX(-7, +7));
}

protected func ContactRight() {
  if(And(Stuck(), !Random(5))) SetRDir(RandomX(-7, +7));
}

private func ControlElevator(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(nil, +1,+1,nil,nil, OCF_Grab, nil,nil, NoContainer(), pElev))
    // Im Fahrstuhlkorb
    if(pElev->~IsElevator())
      // Steuerung an Fahrstuhl weiterleiten
      return(ObjectCall(pElev,szCommand,pObject));
  return(nil);
}

private func ControlElevatorMovement(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(nil, +1,+1,nil,nil, OCF_Grab, nil,nil, NoContainer(), pElev))
    // Fahrstuhlkorb gefunden
    if(pElev->~IsElevator())
      // Lore ist angehalten
      if (Equal(GetComDir(), COMD_Stop))
        // Steuerung an Fahrstuhl weiterleiten
        return(ObjectCall(pElev,szCommand,pObject));
  return(nil);
}

private func ControlElevatorStop(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(nil, +1,+1,nil,nil, OCF_Grab, nil,nil, NoContainer(), pElev))
    // Fahrstuhlkorb gefunden
    if(pElev->~IsElevator())
      // Fahrstuhlkorb bewegt sich
      if (!Equal(GetComDir(pElev), COMD_Stop))
      {
        // Fahrstuhlkorb anhalten
        ObjectCall(pElev,szCommand,pObject); 
        // Noch nicht aussteigen
        SetComDir(COMD_Stop, pObject);
        SetComDir(COMD_Stop);
        // Bewegunsbefehl abbrechen
        return(1);
      }
  return(nil);
}

public func ControlDigDouble(pClonk,zahl) {
CreateMenu(GetID(),pClonk,this(),nil,nil,nil,3,1);
AddMenuItem("Links","Ecke",SY1M,pClonk,nil,1,"Verschiebt die Linke Obere Ecke nach Links",2,nil,nil);
AddMenuItem("Rechts","Ecke",SY1M,pClonk,nil,2,"Verschiebt die Linke Obere Ecke nach Rechts",2,1,nil);

AddMenuItem("Hoch","Ecke",SY1M,pClonk,nil,3,"Verschiebt die Linke Obere Ecke nach Links",2,8,nil);
AddMenuItem("Runter","Ecke",SY1M,pClonk,nil,4,"Verschiebt die Linke Obere Ecke nach Rechts",2,9,nil);


AddMenuItem("Breiter","Breite",SY1M,pClonk,nil,1,"Macht den Bereich breiter",2,2,nil);
AddMenuItem("Schmaler","Breite",SY1M,pClonk,nil,2,"Macht den Bereich schmaler",2,3,nil);

AddMenuItem("Tiefer","Tiefe",SY1M,pClonk,nil,1,"Macht den Bereich tiefer",2,4,nil);
AddMenuItem("Flacher","Tiefe",SY1M,pClonk,nil,2,"Macht den Bereich flacher",2,5,nil);

AddMenuItem("Unter Lore Plazieren","Ausrichten",SY1M,pClonk,nil,nil,"Setzt den Bereich unter die Lohre",2,10,nil);
AddMenuItem("Rahmen An/Aus","Rahmen",LOR2,pClonk,nil,nil,"Schaltet den Rahmen ein und aus",2,nil,nil);

AddMenuItem("(Re)Start","Start",SY1M,pClonk,nil,1,"Startet das Ausheben des Lochs",2,6,nil);
AddMenuItem("Stop","Start",SY1M,pClonk,nil,2,"Stopt das Ausheben des Lochs",2,7,nil);

AddMenuItem("Inhaltsinfo","Inhalt",GetID(this()),pClonk,nil,pClonk,"Zeigt die Mangen der enthaltenen Materialien",2,nil,nil);
AddMenuItem("Nur %-angaben anzeigen EIN/AUS","SetMes",GetID(this()),pClonk,nil,pClonk,"Schaltet den angezeigten Text um.",2,nil,nil);

AddMenuItem("Modus wechseln","Weechsel",LOR1,pClonk,nil,pClonk,"Wechselt zum Materialsauger");

// Selektion wiederherstellen... funktioniert iwie nicht...
  SelectMenuItem(zahl,pClonk);

return(1);}



func Weechsel(par1,pClonk)
{
var zahahl = ContentsCount(nil,this());
var neueLore = CreateObject(LOR1);
while (0<(zahahl ?? 0))
 {
var inhaalt = Contents(nil,nil);
if(!inhaalt) break;
Enter(neueLore,inhaalt);
zahahl = ContentsCount(nil,this());
 }
CloseMenu(pClonk);
//pClonk->~ControlDownDouble();
//SetCommand(pClonk, "Grab", neueLore);
RemoveObject();
return;
}


func SetMes(){
if(Equal(Local(9), nil))Local(9)=1; else Local(9)=nil;
return(1);}

func Ecke(id idItem, iParameter,bRight){
if(Equal(Local(nil), 1)){
Message("Vorgang läuft...|Änderungen|nicht|möglich.",this());
return(Rahmen(1));}
if(Equal(iParameter, 1)){
S42MigrationAssign(Local(1), "-=", 11);
if(Equal(bRight, 1)){S42MigrationAssign(Local(1), "-=", 11);}}
if(Equal(iParameter, 2)){
S42MigrationAssign(Local(1), "+=", 11);
if(Equal(bRight, 1)){S42MigrationAssign(Local(1), "+=", 11);}}
if(Equal(iParameter, 3)){
S42MigrationAssign(Local(6), "-=", 11);
if(Equal(bRight, 1)){S42MigrationAssign(Local(6), "-=", 11);}}
if(Equal(iParameter, 4)){
S42MigrationAssign(Local(6), "+=", 11);
if(Equal(bRight, 1)){S42MigrationAssign(Local(6), "+=", 11);}}
UpdateHelper();
Rahmen(1);
return(1);}

func Breite(id idItem, iParameter,bool bRight){
bRight ??= false;
if(Equal(Local(nil), 1)){
Message("Vorgang läuft...|Änderungen|nicht|möglich.",this());
return(Rahmen(1));}
if(Equal(iParameter, 1)){
S42MigrationAssign(Local(2), "+=", 11);
if(Equal(bRight, 1)){S42MigrationAssign(Local(2), "+=", 11);}}
if(Equal(iParameter, 2)){
S42MigrationAssign(Local(2), "-=", 11);
if(Equal(bRight, 1)){S42MigrationAssign(Local(2), "-=", 11);}}
if((Local(2) ?? 0)<11){Local(2)=11;}
UpdateHelper();
Rahmen(1);
return(1);}

func Tiefe(id idItem, iParameter,bool bRight){
bRight ??= false;
if(Equal(Local(nil), 1)){
Message("Vorgang läuft...|Änderungen|nicht|möglich.",this());
return(Rahmen(1));}
if(Equal(iParameter, 1)){
S42MigrationAssign(Local(3), "+=", 11);}
if(Equal(iParameter, 2)){
S42MigrationAssign(Local(3), "-=", 11);}
if((Local(3) ?? 0)<11){Local(3)=11;}
UpdateHelper();
Rahmen(1);
return(1);}

func Ausrichten(){
if(Equal(Local(nil), 1)){
Message("Vorgang läuft...|Änderungen|nicht|möglich.",this());
return(Rahmen(1));}
Local(6)=(((GetY() ?? 0)+10)/11)*11;
Local(1)=((GetX() ?? 0)/11)*11;
Local(2)=11;
Local(3)=11;
UpdateHelper();
Rahmen(1);
return(1);}

func Start(id idItem, iParameter,bool bRight){
bRight ??= false;
Local(4)=Local(1);
Local(5)=Local(6);
Local(7)=(Local(2) ?? 0)*(Local(3) ?? 0);
Local(8)=nil;
if(Equal(iParameter, 1)){Local(nil)=1;}
if(Equal(iParameter, 2)){Local(nil)=nil;}
return(1);}

func UpdateHelper(){
Local(6,Local(10))=Local(6);
Local(1,Local(10))=Local(1);
Local(2,Local(10))=Local(2);
Local(3,Local(10))=Local(3);
return(1);}

func Rahmen(Temp){
if(Equal(Temp, 1)){
Local(11,Local(10))=50;
}
else
{
if(Equal(Local(10,Local(10)), 1)){Local(10,Local(10))=nil;}
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
Local(20)=nil;

Local(100)="Ashes";
Local(200)=nil;
Local(300)=C4Id("ASHS");

Local(101)="Rock";
Local(201)=nil;
Local(301)=C4Id("CNCR");

Local(102)="Earth";
Local(202)=nil;
Local(302)=C4Id("ERTH");

Local(103)="Ice";
Local(203)=nil;
Local(303)=C4Id("ICE1");

Local(104)="Granite";
Local(204)=nil;
Local(304)=C4Id("LIQG");

Local(105)="FlySand";
Local(205)=nil;
Local(305)=C4Id("SAND");

Local(106)="Sand";
Local(206)=nil;
Local(306)=C4Id("SAND");

Local(107)="Snow";
Local(207)=nil;
Local(307)=C4Id("SNWB");

return(1);}

func Inhalt(ID,par,right){
CreateMenu(GetID(this()),par,this(),nil,nil,nil,1,nil);
AddMenuItem("Menu aktualisieren","Select",GetID(this()),par,nil,par,nil,128,nil,-1);
for(var i=nil;(i ?? 0)<(Local(25) ?? 0);S42MigrationStep(i, 1, true))
	{
	if(Equal(i, Local(20))){
	AddMenuItem(Format("%10s  +",Local(100+(i ?? 0))),"Select",Local(300+(i ?? 0)),par,Local(200+(i ?? 0)),par,nil,128,nil,i);}
	else{
	AddMenuItem(Format("%10s  -",Local(100+(i ?? 0))),"Select",Local(300+(i ?? 0)),par,Local(200+(i ?? 0)),par,nil,128,nil,i);}
	}
AddMenuItem("Menü schließen           ","menuclose",GetID(this()),par,nil,par,nil,nil,nil,nil);

return(1);}

func Select(ID,par,right,val){
if(Equal(val, -1)){
Inhalt(nil,par,nil);
SelectMenuItem(nil);
}else{
Local(20)=val;
Inhalt(nil,par,nil);
SelectMenuItem((val ?? 0)+1,par);}
return(1);}

func menuclose(ID,par,right,val){
CloseMenu(par);
return(1);}

func CheckContent(){
var mat;
var mat1;
for(var i=nil;(i ?? 0)<(ContentsCount() ?? 0);S42MigrationStep(i, 1, true))
	{
	for(var x=nil;(x ?? 0)<(Local(25) ?? 0);S42MigrationStep(x, 1, true))
		{
		mat=Local(100+(x ?? 0));
		mat1=Contents(i)-> ~ MaterialNameString();
		if(!!SEqual(mat, mat1))
			{
			if((Local(200+(x ?? 0)) ?? 0)<1000){
			S42MigrationAssign(Local(200+(x ?? 0)), "+=", (Contents(i)-> ~ MaterialMange() ?? 0)); //Screibfehler(bleibt aber so) :)
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

if(Equal(Local(nil), nil))
SetAction("Idle");

if(Equal(Local(nil), 1))
if(FindObject(ENRG)){
batt=FindObject(ADM3,nil,nil,-1,-1,nil,nil,nil,this());
if(!Equal(batt, nil)){
if(And((GetEnergy() ?? 0) < 1, (GetEnergy(batt) ?? 0)>0)){
DoEnergy(GetEnergy(batt));
DoEnergy(-(GetEnergy(batt) ?? 0),batt); // Batterie entleeren
}}}

CheckContent();
if(!Local(10)){Local(10)=CreateObject(LOR2);
	Local(nil,Local(10))=this();}
if(Equal(Local(nil), 1)){

if(FindObject(ENRG)){
if((GetEnergy() ?? 0) < 1){
SetAction("NeedEnergy");
return(Message("Nicht genügend Energie",this()));}}

S42MigrationStep(Local(11), 1, true);
if((Local(11) ?? 0)>=100){DoEnergy(-1); Local(11)=nil;}

	if((Materialvorrat() ?? 0)>0){

		S42MigrationStep(Local(8), 1, true);

	pro=(Local(8) ?? 0)*100/(Local(7) ?? 0);
	col=RGB(255-((pro ?? 0)*255/100),((pro ?? 0)*255/100),nil);
	if(Equal(Local(9), nil))
		{
		Message("Fertigstellung:| <c %x> %d % </c> |(%d von %d Pixeln)|Noch %d Pixel", this(),col,pro,Local(8),Local(7),(Local(7) ?? 0)-(Local(8) ?? 0));
		}
		else
		{
		Message("Fertigstellung:| <c %x> %d % </c>", this(),col,pro);
		}

	CreateParticle("Mark2",(Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0),nil,nil,10,RGB(255,nil,255));
	if(Equal(Free(Local(4),Local(5)), 1))
		{
		LineTo((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0),RGB(GetMaterialColor(Material(DefMat()),nil,nil),GetMaterialColor(Material(DefMat()),nil,1),GetMaterialColor(Material(DefMat()),nil,2)));

		MatSub();
		tunnel=1;
		if(Equal(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)), -1)){tunnel=nil;}

		DrawMaterialQuad(Format("%s-SMOOTH",DefMat()),Local(4),Local(5),(Local(4) ?? 0)+1,Local(5),Local(4),Local(5),Local(4),(Local(5) ?? 0)+1,tunnel);

		}

	S42MigrationStep(Local(4), 1, true);
	if(Equal(Local(4), (Local(1) ?? 0)+(Local(2) ?? 0))){
		Local(4)=Local(1);
		S42MigrationStep(Local(5), 1, true);
		}

	if(Equal(Local(5), (Local(3) ?? 0)+(Local(6) ?? 0))){Start(nil,2,nil);}
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
DrawParticleLine("Mark2",x,y,nil,nil,5,10,c,c,nil);
return(1);}

func MatSub(){
S42MigrationStep(Local(200+(Local(20) ?? 0)), -1, true);
return(1);}

func Materialvorrat(){
return(Local(200+(Local(20) ?? 0)));}

func Free(int x,int y){
x ??= 0;
y ??= 0;
var frei;
frei=1;
if(GBackSemiSolid((x ?? 0)-(GetX() ?? 0),(y ?? 0)-(GetY() ?? 0))){frei=nil;}
return(frei);}

func DefMat() {
return(Local(100+(Local(20) ?? 0)));
}
