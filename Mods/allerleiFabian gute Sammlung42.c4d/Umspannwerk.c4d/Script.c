/*-- Umspannwerk --*/

#strict
#include BAS5

local aFrom, aTo, aLines, pMarker,Local2,Local3;

func Initialize() {
  Local(2)=1;
  Local(3)=1;
  Local2=1;
  Local3=1;
  aFrom  = CreateArray(1); 
  aTo    = CreateArray(1); 
  aLines = CreateArray(1); 
  return(1);
}

func BatterieEnergy()
{
    if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
    
    for(var batt in FindObjects(Find_ID(ADM3),Find_Container(this())))
    {
        if(GetEnergy()>90 && GetEnergy(batt)<100) // wenn mehr als 90 Energie vorhanden, dann evlt was in unaufgeladenen Batterien zwischenlagern
        {
            var differenz= Min(GetEnergy(),100-GetEnergy(batt));
            DoEnergy(differenz,batt); // Batterie aufladen
            DoEnergy(-differenz);
        }
        else if(GetEnergy()<=30 && GetEnergy(batt)>0) // wenn wenig Energie, evlt aus Batterien saugen   
        {
            var differenz= Min(90-GetEnergy(),GetEnergy(batt)); // nur bis 90 auffüllen, da sonst im nächsten durchlauf wieder geleert wird.
            DoEnergy(differenz); 
            DoEnergy(-differenz,batt);// Batterie entladen
        }
    }
    
}

// außer Batterie nichts aufnehmen
protected func RejectCollect(def,pObj) { if(def!=ADM3 || ObjectCount2(Find_ID(ADM3),Find_Container(this()))>=20) return(1); } // maximal 20 Stück

func OnMenuSelection(iItemIndex, pMenuObject) 
{
  if(Format("%i",GetMenu(pMenuObject)) eq "MID1")
  {
  // Log(Format("OnMenuSel MID1 aufgerufen",iItemIndex));
  var iCorrect = -1;
  var pSelectedObject = aFrom[(GetLength(aFrom)-iItemIndex)+iCorrect];
  //while(!pSelectedObject)  {iCorrect--; pSelectedObject = aFrom[(GetLength(aFrom)-iItemIndex)+iCorrect];}
  
  SetPlrView(GetOwner(),pSelectedObject);
  if(pMarker) RemoveObject(pMarker);
  if(pSelectedObject)
    pMarker = CreateObject(_SM4,GetX(pSelectedObject)-GetX(),GetY(pSelectedObject)-GetY()-30,GetOwner());
  }
  
  if(Format("%i",GetMenu(pMenuObject)) eq "MID2")
  {
  // Log(Format("OnMenuSel MID2 aufgerufen",iItemIndex));
  //Explode(50,aTo[(GetLength(aTo)-iItemIndex)-1]);
  var iCorrect = -1;
  var pSelectedObject = aTo[(GetLength(aTo)-iItemIndex)+iCorrect];
  //while(!pSelectedObject)  {iCorrect--; pSelectedObject = aTo[(GetLength(aTo)-iItemIndex)+iCorrect];}
  
  SetPlrView(GetOwner(),pSelectedObject);
  if(pMarker) RemoveObject(pMarker);
  if(pSelectedObject)
    pMarker = CreateObject(_SM4,GetX(pSelectedObject)-GetX(),GetY(pSelectedObject)-GetY()-30,GetOwner());
  }
  return(_inherited(iItemIndex, pMenuObject));
}

func MenuQueryCancel() {if(pMarker) RemoveObject(pMarker);}

func ControlUp(pByObj) 
{
CreateMenu(_USW,pByObj,this(),0,"$TxtNoEntry$",0,1,0,ROCK);
AddMenuItem("$TxtFrom$","OpenFromMenu",DPIP,pByObj,0,pByObj);
AddMenuItem("$TxtTo$","OpenToMenu",SPIP,pByObj,0,pByObj);
if(FindObject2(Find_ID(ADM3),Find_Container(this()))) // wenn Batterien drin sind, auch den Status von diesen Anzeigen lassen
    AddMenuItem("Status Batterien","OpenBatterieMenu",ADM3,pByObj,0,pByObj);
}

func OpenBatterieMenu(idItem, pByObj)
{
CreateMenu(_USW,pByObj,this(),0,"$TxtNoEntry$",0,1,0,ADM3); // ein Menu erzeugen, in dem alle Batterien mit ihrer aktuellen Energie gelistet sind
var zehler=1;
for(var batt in FindObjects(Find_ID(ADM3),Find_Container(this())))
    {
        AddMenuItem(Format("%v. Batterie Energie: %v",zehler,GetEnergy(batt)),"OpenBatterieMenu",ADM3,pByObj,0,pByObj,"Hier siehst du den Aufladestatus der enthaltenen Batterien"); // wird ein Eintrag ausgewählt, wird das Menu nochmal aufgerufen, also die Werte aktualisiert
        zehler++;
    }
}



func OpenFromMenu(idItem, pByObj)
{
CreateMenu(_USW,pByObj,this(),0,"$TxtNoEntry$",0,1,0,MID1);

var iLengthFrom = GetLength(aFrom);
  while(iLengthFrom)
  {
    iLengthFrom--;
    if(aFrom[iLengthFrom])
    {
    AddMenuItem(Format("%s {{%i}}",GetName(aFrom[iLengthFrom]),GetStatus(aFrom[iLengthFrom])),"ChangeStatus",GetID(aFrom[iLengthFrom]),pByObj,0,aFrom[iLengthFrom]);
    }
    if(!aFrom[iLengthFrom]) AddMenuItem("Nicht belegt", "Returning", _DEN, pByObj);
  }
  // if(iLengthFrom>=1) 
    OnMenuSelection(0, pByObj); // direkt einmal aufrufen, damit Gebäude direkt markiert wird
}

func OpenToMenu(idItem, pByObj)
{
CreateMenu(_USW,pByObj,this(),0,"$TxtNoEntry$",0,1,0,MID2);

var iLengthTo = GetLength(aTo);
  while(iLengthTo--)
  {
    if(aTo[iLengthTo])
    {
    AddMenuItem(Format("%s {{%i}}",GetName(aTo[iLengthTo]),GetStatus(aTo[iLengthTo])),"ChangeStatus",GetID(aTo[iLengthTo]),pByObj,0,aTo[iLengthTo]);
    }
    if(!aTo[iLengthTo]) AddMenuItem("Nicht belegt", "Returning", _DEN, pByObj);
  }
  // if(iLengthFrom>=1) 
    OnMenuSelection(0, pByObj); // direkt einmal aufrufen, damit Gebäude direkt markiert wird
}

func Returning() {return();}

func ChangeStatus(idItem, pObject) 
{
if(pMarker) RemoveObject(pMarker);
var iCount = 0;
var pLines;
//Explode(50,pObject);
  while(pLines=FindObject(PWRL,0,0,0,0,0,"Connect",this(),0,pLines))
  {
  aLines[iCount] = pLines;
  if(pObject)
    {
    if(ObjectNumber(GetActionTarget(0,pLines)) == ObjectNumber(pObject) && ObjectNumber(GetActionTarget(1,pLines)) == ObjectNumber(this())) {pLines->ChangeTransfer(); return(1);}
    if(ObjectNumber(GetActionTarget(1,pLines)) == ObjectNumber(pObject) && ObjectNumber(GetActionTarget(0,pLines)) == ObjectNumber(this())) {pLines->ChangeTransfer(); return(1);}
    }
  iCount++;
  //Message("%d",this(),iCount);
  }
}

func GetStatus(pObject) 
{
var iCount = 0;
var pLines;
//Explode(50,pObject);
  while(pLines=FindObject(PWRL,0,0,0,0,0,"Connect",this(),0,pLines))
  {
  aLines[iCount] = pLines;
  if(pObject)
    {
    if(ObjectNumber(GetActionTarget(0,pLines)) == ObjectNumber(pObject) && ObjectNumber(GetActionTarget(1,pLines)) == ObjectNumber(this())) 
      {
      if( pLines->IsStopped())
        return(_DEN);
      if(!pLines->IsStopped())
        return(_ENG);
      }
    if(ObjectNumber(GetActionTarget(1,pLines)) == ObjectNumber(pObject) && ObjectNumber(GetActionTarget(0,pLines)) == ObjectNumber(this())) 
      {
      if( pLines->IsStopped())
        return(_DEN);
      if(!pLines->IsStopped())
        return(_ENG);
      }
    }
  iCount++;
  //Message("%d",this(),iCount);
  }
}

func ConnectionTarget(pTarget, fOrigin) // bei fahrstuhl ist es hier das fahrstuhlgebäude selbst, aber bei lost ist es der korb!
{
    if(GetID(pTarget)==LNKT) return(); // es kommt vor, dass leitungsbausatz ubergeben wird.

    // Log(Format("Connected %v , Umspannwerk als Quelle?: %v. ArrayFrom: %v, ArrayTo: %v",pTarget,fOrigin,aFrom,aTo));
    if(fOrigin)
      {
      aTo[GetFreeField(aTo)] = pTarget;
      }

    if(!fOrigin)
      {
      aFrom[GetFreeField(aFrom)] = pTarget;
      }
      // Log(Format("hinterher Connected %v , Umspannwerk als Quelle?: %v. ArrayFrom: %v, ArrayTo: %v",pTarget,fOrigin,aFrom,aTo));
}

func LostTarget(pLostObject, fOrigin) //fOrigin, wenn wahr, dann ist umspannwerk ausgangspunkt der leitung, also strom fließt von umspanngswerk nach object.
{
    if(pLostObject->~IsElevator()) // wenn fahrstuhlkorb, funktioniert so auch bei Seilbahn
        pLostObject = pLostObject->Elevator(); // das dazugeörige Fahrstuhlgebäude
    // Log(Format("LOOOOOOOOOOOOOOOOOST %v , Umspannwerk als Quelle?: %v. ArrayFrom: %v, ArrayTo: %v",pLostObject,fOrigin,aFrom,aTo));

    var iLengthFrom = GetLength(aFrom); // Arrays auf vorhandene Einträge Überprüfen... der eintrag umspannwerk wird mermals vorkommen, daher ist ein einfaches löschen mit nachrücken wohl nicht sinnvoll
      if(!fOrigin)
      while(iLengthFrom--)
      {
      if(ObjectNumber(aFrom[iLengthFrom]) == ObjectNumber(pLostObject)) aFrom[iLengthFrom]=0;  
      }
  
    var iLengthTo = GetLength(aTo);
      if(fOrigin)
      while(iLengthTo--)
      {
      if(ObjectNumber(aTo[iLengthTo]) == ObjectNumber(pLostObject)) aTo[iLengthTo]=0;
      }
    // Log(Format("Hinterher: ArrayFrom: %v, ArrayTo: %v",aFrom,aTo));
}

func GetFreeField(aArray) 
{
    var iCount = 0;
    while(aArray[iCount])
      {
      iCount++;
      }
    return(iCount);
}

func ChangedConnection(fOrigin) 
{
//if(fOrigin)
//Message("ORIGIN LOST",this());
//if(!fOrigin)
//Message("TARGET LOST",this());
//Message("LOOOOOOOOOOOOOOOOOOOOST",this());
}
