/*-- Lore2 --*/

#strict 3

/* Status */

local zeehler, materialaarthalb,materialaartfest,materialaartwasser;

func Initialize(){
materialaarthalb=1;
Local(10)=CreateObject(LOR2);
Local(6)=(((GetY() ?? 0)+10)/11)*11;
Local(1)=((GetX() ?? 0)/11)*11;
Local(2)=11;
Local(3)=11;

Local(nil,Local(10))=this();
Local(6,Local(10))=(((GetY() ?? 0)+10)/11)*11;
Local(1,Local(10))=((GetX() ?? 0)/11)*11;
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
Saugermenu(pClonk,zahl);
}


func Saugermenu(pClonk,zahl)
{
CreateMenu(GetID(),pClonk,this(),nil,nil,nil,1,1);

AddMenuItem("Links","Ecke",SY2M,pClonk,nil,1,"Verschiebt die Linke Obere Ecke nach Links",2,nil,nil);
AddMenuItem("Rechts","Ecke",SY2M,pClonk,nil,2,"Verschiebt die Linke Obere Ecke nach Rechts",2,1,nil);

AddMenuItem("Hoch","Ecke",SY2M,pClonk,nil,3,"Verschiebt die Linke Obere Ecke nach Links",2,8,nil);
AddMenuItem("Runter","Ecke",SY2M,pClonk,nil,4,"Verschiebt die Linke Obere Ecke nach Rechts",2,9,nil);


AddMenuItem("Breiter","Breite",SY2M,pClonk,nil,1,"Macht den Bereich breiter",2,2,nil);
AddMenuItem("Schmaler","Breite",SY2M,pClonk,nil,2,"Macht den Bereich schmaler",2,3,nil);

AddMenuItem("Tiefer","Tiefe",SY2M,pClonk,nil,1,"Macht den Bereich tiefer",2,4,nil);
AddMenuItem("Flacher","Tiefe",SY2M,pClonk,nil,2,"Macht den Bereich flacher",2,5,nil);

AddMenuItem("Unter Lore Plazieren","Ausrichten",SY2M,pClonk,nil,nil,"Setzt den Bereich unter die Lohre",2,10,nil);
AddMenuItem("Rahmen An/Aus","Rahmen",LOR2,pClonk,nil,nil,"Schaltet den Rahmen ein und aus",2,nil,nil);

AddMenuItem("(Re)Start","Start",SY2M,pClonk,nil,1,"Startet das Ausheben des Lochs",2,6,nil);
AddMenuItem("Stop","Start",SY2M,pClonk,nil,2,"Stopt das Ausheben des Lochs",2,7,nil);
AddMenuItem("Nur %-angaben anzeigen EIN/AUS","SetMes",GetID(this()),pClonk,nil,pClonk,"Schaltet den angezeigten Text um.",2,nil,nil);

if(GetPlrKnowledge(GetOwner(),LO33)) // wenn erforscht, dann materialien einstellbar
{
AddMenuItem("Halbfestes Material","Materialwechsel",ERTH,pClonk,materialaarthalb,pClonk,"Wenn aktiviert, wird grabbares Material eingesaugt");
AddMenuItem("Festes Material","Materialwechsel",ROCK,pClonk,materialaartfest,pClonk,"Wenn aktiviert, wird nichtgrabbares Material eingesaugt");
AddMenuItem("Flüssiges Material","Materialwechsel",WBRL,pClonk,materialaartwasser,pClonk,"Wenn aktiviert, wird flüssiges Material eingesaugt, zum lagern werden Fässer im Sauger benötigt");
}
AddMenuItem("Modus wechseln","Weechsel",LOR3,pClonk,nil,pClonk,"Wechselt zum Materialformer");
// Selektion wiederherstellen... funktioniert iwie nicht...
  SelectMenuItem(zahl,pClonk);
return(1);}

func Materialwechsel(ID,pClonk)
{

if(Equal(ID, ERTH))
 {
 if(Equal(materialaarthalb, nil))
{materialaarthalb=1;
CloseMenu(pClonk);
Saugermenu(pClonk,13);
return;}
 if(Equal(materialaarthalb, 1))
{materialaarthalb=nil;
CloseMenu(pClonk);
Saugermenu(pClonk,13);
return;}
 }

if(Equal(ID, ROCK))
 {
 if(Equal(materialaartfest, nil))
{materialaartfest=1;
CloseMenu(pClonk);
Saugermenu(pClonk,14);
return;}
 if(Equal(materialaartfest, 1))
{materialaartfest=nil;
CloseMenu(pClonk);
Saugermenu(pClonk,14);
return;}
 }

if(Equal(ID, WBRL))
 {
 if(Equal(materialaartwasser, nil))
{materialaartwasser=1;
CloseMenu(pClonk);
Saugermenu(pClonk,15);
return;}
 if(Equal(materialaartwasser, 1))
{materialaartwasser=nil;
CloseMenu(pClonk);
Saugermenu(pClonk,15);
return;}
 }
}

func Weechsel(par1,pClonk)
{
var zahahl = ContentsCount(nil,this());
var neueLore = CreateObject(LOR3);
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




if(!Local(10)){Local(10)=CreateObject(LOR2);
Local(nil,Local(10))=this();}
if(Equal(Local(nil), 1)){

if(FindObject(ENRG)){
if((GetEnergy() ?? 0) < 1){
SetAction("NeedEnergy");
return(Message("Nicht genügend Energie",this()));}}

S42MigrationStep(Local(11), 1, true);
if((Local(11) ?? 0)>=100){DoEnergy(-1); Local(11)=nil;}

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

if(Equal(materialaarthalb, 1))
if(Equal(Digable(Local(4),Local(5)), 1))
{
LineTo((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0),RGB(GetMaterialColor(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)),nil,nil),GetMaterialColor(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)),nil,1),GetMaterialColor(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)),nil,2)));
MatAdd(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)));
obj=CreateObject(ROCK,Local(4),Local(5),NO_OWNER);
obj->SetVisibility(VIS_None);
obj->DigFreeRect(Local(4),Local(5),1,1); 
RemoveObject(obj);
}

if(Equal(materialaartfest, 1))
if(Equal(Digable(Local(4),Local(5)), 2))   // wenn nicht grabbar
{
LineTo((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0),RGB(GetMaterialColor(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)),nil,nil),GetMaterialColor(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)),nil,1),GetMaterialColor(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)),nil,2)));
MatAdd(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)));
obj=CreateObject(ROCK,Local(4),Local(5),NO_OWNER);
obj->SetVisibility(VIS_None);
//obj->DigFreeRect(Local(4),Local(5),1,1); 
obj->FreeRect(Local(4),Local(5),1,1); 
RemoveObject(obj);
}

if(Equal(materialaartwasser, 1))
if(Equal(Digable(Local(4),Local(5)), 3))   // wenn flüssig
{
LineTo((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0),RGB(GetMaterialColor(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)),nil,nil),GetMaterialColor(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)),nil,1),GetMaterialColor(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)),nil,2)));
MatAdd(GetMaterial((Local(4) ?? 0)-(GetX() ?? 0),(Local(5) ?? 0)-(GetY() ?? 0)));
obj=CreateObject(ROCK,Local(4),Local(5),NO_OWNER);
obj->SetVisibility(VIS_None);
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


S42MigrationStep(Local(4), 1, true);
if(Equal(Local(4), (Local(1) ?? 0)+(Local(2) ?? 0))){
Local(4)=Local(1);
S42MigrationStep(Local(5), 1, true);
                               }

if(Equal(Local(5), (Local(3) ?? 0)+(Local(6) ?? 0))){Start(nil,2,nil);}
}
if((ContentsCount() ?? 0)>89){Message("!!Warnung!!|Lagerkapazität erreicht",this());}
if((ContentsCount() ?? 0)>99){Exit(Contents(nil,this()),nil,5); }
if(Contained()){Contained()->GrabContents(this());}
return(1);
}

func LineTo(x,y,c){
DrawParticleLine("Mark2",x,y,nil,nil,5,10,c,c,nil);
return(1);}

func MatAdd(int Mat){
Mat ??= 0;
var max;
var ID;
max=GetMaterialVal("Dig2ObjectRatio","Material",Mat);
if(max)
 {
S42MigrationStep(Local(100+(Mat ?? 0)), 1, true);
if(Equal(Local(100+(Mat ?? 0)), max)){
ID=GetMaterialVal("Dig2Object","Material",Mat); 
if(!Equal(ID, nil)){CreateContents(ID);}
Local(100+(Mat ?? 0))=nil;
                       }
  }
if(!max)
  {
max=GetMaterialVal("Blast2ObjectRatio","Material",Mat);
S42MigrationStep(Local(100+(Mat ?? 0)), 1, true);
if(Equal(Local(100+(Mat ?? 0)), max)){
ID=GetMaterialVal("Blast2Object","Material",Mat); 
if(!Equal(ID, nil)){CreateContents(ID);}
Local(100+(Mat ?? 0))=nil;
                       }
  }

if(!max)
if(Or(Or(Or(Or(Equal(Mat, Material("Oil")), Equal(Mat, Material("Water"))), Equal(Mat, Material("Acid"))), Equal(Mat, Material("Lava"))), Equal(Mat, Material("DuroLava"))))
   {
max=3;
S42MigrationStep(Local(100+(Mat ?? 0)), 1, true);
if(Equal(Local(100+(Mat ?? 0)), max)){
var fasss = FindContents(BARL);
if(fasss) ObjectInsertMaterial(Mat, this());
Local(100+(Mat ?? 0))=nil;
                       }
   }

return(1);}

func Digable(int x,int y){
x ??= 0;
y ??= 0;
var mat;
mat=GetMaterial((x ?? 0)-(GetX() ?? 0),(y ?? 0)-(GetY() ?? 0));
if(GetPlrKnowledge(GetOwner(),LO33))   // auch festes material abbaubar
  {
if(Or(Or(Or(Or(Equal(mat, Material("Oil")), Equal(mat, Material("Water"))), Equal(mat, Material("Acid"))), Equal(mat, Material("Lava"))), Equal(mat, Material("DuroLava")))) return(3); // flüssiges
if(GetMaterialVal("DigFree","Material",mat)) return(1);   // wenn grabbar
if(!GetMaterialVal("DigFree","Material",mat)) return(2);  //wenn nicht grabbar
  }
return(GetMaterialVal("DigFree","Material",mat));
}