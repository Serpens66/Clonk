/*-- Umspannwerk --*/

#strict 3
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
        if(And((GetEnergy() ?? 0)>90, (GetEnergy(batt) ?? 0)<100)) // wenn mehr als 90 Energie vorhanden, dann evlt was in unaufgeladenen Batterien zwischenlagern
        {
            var differenz= Min(GetEnergy(),100-(GetEnergy(batt) ?? 0));
            DoEnergy(differenz,batt); // Batterie aufladen
            DoEnergy(-(differenz ?? 0));
        }
        else if(And((GetEnergy() ?? 0)<=30, (GetEnergy(batt) ?? 0)>0)) // wenn wenig Energie, evlt aus Batterien saugen
        {
            var differenz= Min(90-(GetEnergy() ?? 0),GetEnergy(batt)); // nur bis 90 auffüllen, da sonst im nächsten durchlauf wieder geleert wird.
            DoEnergy(differenz); 
            DoEnergy(-(differenz ?? 0),batt);// Batterie entladen
        }
    }
    
}

// außer Batterie nichts aufnehmen
protected func RejectCollect(def,pObj) { if(Or(!Equal(def, ADM3), (ObjectCount2(Find_ID(ADM3),Find_Container(this())) ?? 0)>=20)) return(1); } // maximal 20 Stück

func OnMenuSelection(iItemIndex, pMenuObject) 
{
  if(!!SEqual(Format("%i",GetMenu(pMenuObject)), "MID1"))
  {
  // Log(Format("OnMenuSel MID1 aufgerufen",iItemIndex));
  var iCorrect = -1;
  var pSelectedObject = aFrom[((GetLength(aFrom) ?? 0)-(iItemIndex ?? 0))+(iCorrect ?? 0)];
  //while(!pSelectedObject)  {iCorrect--; pSelectedObject = aFrom[(GetLength(aFrom)-iItemIndex)+iCorrect];}
  
  SetPlrView(GetOwner(),pSelectedObject);
  if(pMarker) RemoveObject(pMarker);
  if(pSelectedObject)
    pMarker = CreateObject(_SM4,(GetX(pSelectedObject) ?? 0)-(GetX() ?? 0),(GetY(pSelectedObject) ?? 0)-(GetY() ?? 0)-30,GetOwner());
  }
  
  if(!!SEqual(Format("%i",GetMenu(pMenuObject)), "MID2"))
  {
  // Log(Format("OnMenuSel MID2 aufgerufen",iItemIndex));
  //Explode(50,aTo[(GetLength(aTo)-iItemIndex)-1]);
  var iCorrect = -1;
  var pSelectedObject = aTo[((GetLength(aTo) ?? 0)-(iItemIndex ?? 0))+(iCorrect ?? 0)];
  //while(!pSelectedObject)  {iCorrect--; pSelectedObject = aTo[(GetLength(aTo)-iItemIndex)+iCorrect];}
  
  SetPlrView(GetOwner(),pSelectedObject);
  if(pMarker) RemoveObject(pMarker);
  if(pSelectedObject)
    pMarker = CreateObject(_SM4,(GetX(pSelectedObject) ?? 0)-(GetX() ?? 0),(GetY(pSelectedObject) ?? 0)-(GetY() ?? 0)-30,GetOwner());
  }
  return(_inherited(iItemIndex, pMenuObject));
}

func MenuQueryCancel() {if(pMarker) RemoveObject(pMarker);}

func ControlUp(pByObj) 
{
CreateMenu(_USW,pByObj,this(),nil,"$TxtNoEntry$",nil,1,nil,ROCK);
AddMenuItem("$TxtFrom$","OpenFromMenu",DPIP,pByObj,nil,pByObj);
AddMenuItem("$TxtTo$","OpenToMenu",SPIP,pByObj,nil,pByObj);
if(FindObject2(Find_ID(ADM3),Find_Container(this()))) // wenn Batterien drin sind, auch den Status von diesen Anzeigen lassen
    AddMenuItem("Status Batterien","OpenBatterieMenu",ADM3,pByObj,nil,pByObj);
}

func OpenBatterieMenu(idItem, pByObj)
{
CreateMenu(_USW,pByObj,this(),nil,"$TxtNoEntry$",nil,1,nil,ADM3); // ein Menu erzeugen, in dem alle Batterien mit ihrer aktuellen Energie gelistet sind
var zehler=1;
for(var batt in FindObjects(Find_ID(ADM3),Find_Container(this())))
    {
        AddMenuItem(Format("%v. Batterie Energie: %v",zehler,GetEnergy(batt)),"OpenBatterieMenu",ADM3,pByObj,nil,pByObj,"Hier siehst du den Aufladestatus der enthaltenen Batterien"); // wird ein Eintrag ausgewählt, wird das Menu nochmal aufgerufen, also die Werte aktualisiert
        S42MigrationStep(zehler, 1, true);
    }
}



func OpenFromMenu(idItem, pByObj)
{
CreateMenu(_USW,pByObj,this(),nil,"$TxtNoEntry$",nil,1,nil,MID1);

var iLengthFrom = GetLength(aFrom);
  while(iLengthFrom)
  {
    S42MigrationStep(iLengthFrom, -1, true);
    if(aFrom[(iLengthFrom ?? 0)])
    {
    AddMenuItem(Format("%s {{%i}}",GetName(aFrom[(iLengthFrom ?? 0)]),GetStatus(aFrom[(iLengthFrom ?? 0)])),"ChangeStatus",GetID(aFrom[(iLengthFrom ?? 0)]),pByObj,nil,aFrom[(iLengthFrom ?? 0)]);
    }
    if(!aFrom[(iLengthFrom ?? 0)]) AddMenuItem("Nicht belegt", "Returning", _DEN, pByObj);
  }
  // if(iLengthFrom>=1) 
    OnMenuSelection(nil, pByObj); // direkt einmal aufrufen, damit Gebäude direkt markiert wird
}

func OpenToMenu(idItem, pByObj)
{
CreateMenu(_USW,pByObj,this(),nil,"$TxtNoEntry$",nil,1,nil,MID2);

var iLengthTo = GetLength(aTo);
  while(S42MigrationStep(iLengthTo, -1, true))
  {
    if(aTo[(iLengthTo ?? 0)])
    {
    AddMenuItem(Format("%s {{%i}}",GetName(aTo[(iLengthTo ?? 0)]),GetStatus(aTo[(iLengthTo ?? 0)])),"ChangeStatus",GetID(aTo[(iLengthTo ?? 0)]),pByObj,nil,aTo[(iLengthTo ?? 0)]);
    }
    if(!aTo[(iLengthTo ?? 0)]) AddMenuItem("Nicht belegt", "Returning", _DEN, pByObj);
  }
  // if(iLengthFrom>=1) 
    OnMenuSelection(nil, pByObj); // direkt einmal aufrufen, damit Gebäude direkt markiert wird
}

func Returning() {return;}

func ChangeStatus(idItem, pObject) 
{
if(pMarker) RemoveObject(pMarker);
var iCount = nil;
var pLines;
//Explode(50,pObject);
  while(pLines=FindObject(PWRL,nil,nil,nil,nil,nil,"Connect",this(),nil,pLines))
  {
  aLines[(iCount ?? 0)] = pLines;
  if(pObject)
    {
    if(And(Equal(ObjectNumber(GetActionTarget(nil,pLines)), ObjectNumber(pObject)), Equal(ObjectNumber(GetActionTarget(1,pLines)), ObjectNumber(this())))) {pLines->ChangeTransfer(); return(1);}
    if(And(Equal(ObjectNumber(GetActionTarget(1,pLines)), ObjectNumber(pObject)), Equal(ObjectNumber(GetActionTarget(nil,pLines)), ObjectNumber(this())))) {pLines->ChangeTransfer(); return(1);}
    }
  S42MigrationStep(iCount, 1, true);
  //Message("%d",this(),iCount);
  }
}

func GetStatus(pObject) 
{
var iCount = nil;
var pLines;
//Explode(50,pObject);
  while(pLines=FindObject(PWRL,nil,nil,nil,nil,nil,"Connect",this(),nil,pLines))
  {
  aLines[(iCount ?? 0)] = pLines;
  if(pObject)
    {
    if(And(Equal(ObjectNumber(GetActionTarget(nil,pLines)), ObjectNumber(pObject)), Equal(ObjectNumber(GetActionTarget(1,pLines)), ObjectNumber(this()))))
      {
      if( pLines->IsStopped())
        return(_DEN);
      if(!pLines->IsStopped())
        return(_ENG);
      }
    if(And(Equal(ObjectNumber(GetActionTarget(1,pLines)), ObjectNumber(pObject)), Equal(ObjectNumber(GetActionTarget(nil,pLines)), ObjectNumber(this()))))
      {
      if( pLines->IsStopped())
        return(_DEN);
      if(!pLines->IsStopped())
        return(_ENG);
      }
    }
  S42MigrationStep(iCount, 1, true);
  //Message("%d",this(),iCount);
  }
}

func ConnectionTarget(pTarget, fOrigin) // bei fahrstuhl ist es hier das fahrstuhlgebäude selbst, aber bei lost ist es der korb!
{
    if(Equal(GetID(pTarget), LNKT)) return; // es kommt vor, dass leitungsbausatz ubergeben wird.

    // Log(Format("Connected %v , Umspannwerk als Quelle?: %v. ArrayFrom: %v, ArrayTo: %v",pTarget,fOrigin,aFrom,aTo));
    if(fOrigin)
      {
      aTo[(GetFreeField(aTo) ?? 0)] = pTarget;
      }

    if(!fOrigin)
      {
      aFrom[(GetFreeField(aFrom) ?? 0)] = pTarget;
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
      while(S42MigrationStep(iLengthFrom, -1, true))
      {
      if(Equal(ObjectNumber(aFrom[(iLengthFrom ?? 0)]), ObjectNumber(pLostObject))) aFrom[(iLengthFrom ?? 0)]=nil;
      }
  
    var iLengthTo = GetLength(aTo);
      if(fOrigin)
      while(S42MigrationStep(iLengthTo, -1, true))
      {
      if(Equal(ObjectNumber(aTo[(iLengthTo ?? 0)]), ObjectNumber(pLostObject))) aTo[(iLengthTo ?? 0)]=nil;
      }
    // Log(Format("Hinterher: ArrayFrom: %v, ArrayTo: %v",aFrom,aTo));
}

func GetFreeField(aArray) 
{
    var iCount = nil;
    while(aArray[(iCount ?? 0)])
      {
      S42MigrationStep(iCount, 1, true);
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
