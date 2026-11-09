/*-- Living-Sensor --*/

#strict
#include SENS

local animal, feindlicherclonk, feindlichesfahrzeug,projektil,freundlichesfahrzeug, freundlicherclonk,zap;
local distance;

//func ExtraSetup()
//{
//if(distance<=500)
//      AddMenuItem ("Suchradius um 10Meter erweitern","Suchradius",GetID(),pUser,0,1,"Der Suchradius wird um 10 erweitert, bis auf ein Maximum von 500Meter");
//if(distance>=20)
//      AddMenuItem ("Suchradius um 10Meter verringern","Suchradius",GetID(),pUser,0,2,"Der Suchradius wird um 10 verringert, bis auf ein Minimum von 20Meter");
//}

local Mark_LO,Mark_LU,Mark_RO,Mark_RU;



private func Markierungen()
{
  // Linksoben,Linksunten,Rechtsoben,Rechtsunten eine Markierung erstellen
  Mark_LO= CreateObject(FM9I,-distance,-distance);
  Mark_LU= CreateObject(FM9I,-distance,+distance);
  Mark_RO= CreateObject(FM9I,+distance,-distance);
  Mark_RU= CreateObject(FM9I,+distance,+distance);
  Mark_LO-> Init(this(),1);
  Mark_LU-> Init(this(),2);
  Mark_RO-> Init(this(),3);
  Mark_RU-> Init(this(),4);
}


private func ExtraSetup(idItem,pClonk,iRepeat)
{
  RemoveMarks();
  Markierungen();
  if(!iRepeat)
    {
    SetOwner(GetOwner(pClonk),Mark_LO); SetVisibility(VIS_Owner(),Mark_LO);
    SetOwner(GetOwner(pClonk),Mark_LU); SetVisibility(VIS_Owner(),Mark_LU);
    SetOwner(GetOwner(pClonk),Mark_RO); SetVisibility(VIS_Owner(),Mark_RO);
    SetOwner(GetOwner(pClonk),Mark_RU); SetVisibility(VIS_Owner(),Mark_RU);
    }
  //CreateMenu(LS4K,pClonk);
  AddMenuItem("Suchradius um 5 Meter erweitern","SetRadius",LMM3,pClonk,0,pClonk); 
  AddMenuItem("Suchradius um 5 Meter verringern","SetRadius",LMM7,pClonk,0,pClonk);  
}

private func SetRadius(idItem,pClonk)
{  
  var iChange,iSMI;
  if(idItem== LMM3) {iChange= (+5); iSMI= 3; }
  if(idItem== LMM7) { iChange= (-5); iSMI= 4; }
  distance= distance+ iChange;
  if(!Inside(distance,10,500))
    {
    if(distance> 500) distance= 500;
    if(distance< 10) distance= 10;
    }
  else
    {
    Mark_LO-> Move(iChange);
    Mark_LU-> Move(iChange);
    Mark_RO-> Move(iChange);
    Mark_RU-> Move(iChange);
    }
  PlayerMessage(GetOwner(),Format("Radius beträgt nun %v Meter",distance),pClonk);
  SelectMenuItem(iSMI,pClonk);
}

protected func MenuQueryCancel()
{
  return(0,RemoveMarks());
}

protected func Destruction(a,b,c)
{
  RemoveMarks();
  return(_inherited(a,b,c)); 
}

private func RemoveMarks()
{
  if(Mark_LO) RemoveObject(Mark_LO);
  if(Mark_LU) RemoveObject(Mark_LU);
  if(Mark_RO) RemoveObject(Mark_RO);
  if(Mark_RU) RemoveObject(Mark_RU);
}



func Suchradius(ID,zahl)
{
if(zahl==1 && distance<=490) {distance+=10; PlayerMessage(GetOwner(),Format("Radius beträgt nun %v Meter",distance),0);return(1);}
if(zahl==2 && distance>=30) {distance-=10; PlayerMessage(GetOwner(),Format("Radius beträgt nun %v Meter",distance),0);return(1);}
Sound("Error");PlayerMessage(GetOwner(),"Radius am Maximum/Minimum",0);return(1);
}



func Initialize()
{
  _inherited();
  
  
    //Die einzelnen Stadien.
    //Alle Stadien können gleichzeitig aktiv sein und sind immer 0 oder 1!
  
  AddState("$ENEMYS$"); //0 <- Kennnummer.
  AddState("$ALLIED$"); //1
  AddState("$ANIMALS$");//2
  AddState("feindliches Fahrzeug"); //3
  AddState("befreundetes Fahrzeug"); //4
  AddState("Projektile"); //5
  AddState("Zaps"); //6
  RemoveMarks();
  distance = 50;
}


/*  Deaktiviert, stattdessen neue Funktion mit globalen Funktionen aus Space Empire
func Timer()
{
	Reset();
	living = FindObject2(Find_Distance (distance),Find_OCF (OCF_Alive));
	if(living)
	{
		if(GetOwner(living)==-1)
    {
      
       // Status setzen.
       // SetState(state,<Kennnummer>,<Wert>); (Wert kann nur 1 oder 0 sein.)
      
			SetState(state,2,1);//Aha! Ein Tier(zumindest ein besitzerloses Wesen)!
    }
		else
		{
			if(Hostile (GetOwner(this()),GetOwner(living)))	
				SetState(state,0,1);//Der Feind naht!
			else
				SetState(state,1,1);//Aber ein Freund!
		}
	}
  
  
   // Aktionen auslösen.
  
	TriggerActions();
}
*/



// Status setzen. SetState(state,<Kennnummer>,<Wert>); (Wert kann nur 1 oder 0 sein.)
func Timer()
{
	Reset();
        feindlicherclonk = GetTargetCrew(distance,0,0,1,1);
        freundlicherclonk = GetTargetCrewAllied(distance,0,0,1,1);
	animal = GetTargetAnimal(distance,0,0,1);
        feindlichesfahrzeug = GetTargetVehicle(distance,0,0,1,1);
        freundlichesfahrzeug = GetTargetVehicleAllied(distance,0,0,1,1);
        projektil = GetTargetProjectile(distance,0,0,1);
        zap = GetTargetZapGlobal(distance,0,0,1);

        if(feindlicherclonk) 
        SetState(state,0,1); //Der Feind naht!

	if(freundlicherclonk)
        SetState(state,1,1);//Ein Freund!

	if(animal)
	SetState(state,2,1);//Aha! Ein Tier!

        if(feindlichesfahrzeug)
        SetState(state,3,1); //Ein feindliches Fahrzeug

        if(freundlichesfahrzeug)
        SetState(state,4,1); //Ein freundliches Fahrzeug

        if(projektil)
        SetState(state,5,1); //Ein Projektil

        if(zap)
        SetState(state,6,1); //Eine Biene!       
        	  
  
   // Aktionen auslösen.
  
	TriggerActions();
}




func Reset()//Hilfsfunktion für den Timer. -> alles nochmal neu suchen
{
	SetState(state,0,2);//Kein Feind!
	SetState(state,1,2);//Kein Freund!
	SetState(state,2,2);//Kein Tier!
	SetState(state,3,2);//Kein FeindFahrzeug
	SetState(state,4,2);//Kein FreundFahrzeug
	SetState(state,5,2);//Kein Projektil
	SetState(state,6,2);//Keine Biene
}

public func GetAdditionalInfo()//Spezialinformationen für Sensor-Ziele.
{
// if beachten, was im Sensor aktiviert ist, sonst wird ständig animal bei tieranwesendheit returnt, obwohl auf feinde geschossen werden soll
//außerdem klappts auch nicht, wenn auf tier geschossen werden soll und tier returnt wird

if(animal) {Log("Animal wird returnt");return(animal);}
if(feindlicherclonk) return(feindlicherclonk);
if(freundlicherclonk) return(freundlicherclonk);
if(feindlichesfahrzeug) return(feindlichesfahrzeug);
if(freundlichesfahrzeug) return(freundlichesfahrzeug);
if(projektil) return(projektil);
if(zap) return(zap);
return(0);
}