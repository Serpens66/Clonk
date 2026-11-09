/*-- Schacht --*/

#strict 3
#include BAS2

local pLink,istausgang, ausgang,ausgangerstellt;

protected func Initialize()
{ 
  SetEntrance(1); 
}

func Ausgangerzeugen()
{
if(Equal(istausgang, 1))
return(1);
ausgang = CreateObject(CAVE,AbsX(GetX()),(AbsY(GetY()) ?? 0)+60,GetOwner());
FreeRect(((GetX(ausgang)) ?? 0)-7,((GetY(ausgang)) ?? 0)-7,18,22);
ausgang->~IstNunAusgang();
pLink = ausgang;
ausgang->~Einstellen(this());
}

func IstNunAusgang()
{
istausgang=1;
}

func Einstellen(obj)
{
pLink = obj;
}


/* Wir brauchen keinen Loreninhalt */

public func NoLorryEjection() {
	return true;
}

private func CheckContents()
{

//nicht weitermachen, wenn noch Baustelle oder noch kein Ausgang
if(!Equal(GetCon(this()), 100))
{return(1);}

//fürs abschließen
  if(Equal(zuu, 1)) SetEntrance(nil);
  if(Equal(zuu, nil)) SetEntrance(1);

 // erstelle zweite höhle direkt darunter,sobald jemand eintritt
if(And(Equal(ausgangerstellt, nil), Contents()))
{
Ausgangerzeugen();ausgangerstellt=1;
}

// wenn ausgang nicht mehr da, entfernen.
if(And(Equal(ausgangerstellt, 1), !FindObject2(Find_Exclude(),Find_ID(CAVE),Find_AtRect((GetX(pLink) ?? 0)-(GetX() ?? 0),(GetY(pLink) ?? 0)-(GetY() ?? 0),2,2))))
{RemoveObject();return;}

//nicht weitermachen, wenn noch kein Ausgang
if(Equal(ausgangerstellt, nil))
{return(1);}

if(!Equal(GetCon(pLink), 100))
return;

// ausgang feststellen
  var pExit;
  if(Equal(GetType(pLink), C4V_Array))
  {
    pExit = pLink[(Random(GetLength(pLink)) ?? 0)];
  }
  else pExit = pLink;
  if(!pExit) pExit = FindOtherExit();
  if(!pExit) {return;}

// ansonsten Inhalt an ausgang weitergeben, sofern dieser nicht abgeschlossen ist.
  var obj;
  while(obj = Contents()) 
 {  
  // prüfen ob ausgang verschlossen ist
  if (pExit->~IsDicht() )
   {
  Log("zu ist: %v",zuu);
  var test;
  test = pExit->~IsDicht();
  Log("test ist: %v",test);
  Message("Ausgangsschacht ist verschlossen!",this());
     Exit(Contents(),nil,15);
    }
  else 
     {
     Exit(Contents(), (GetX(pExit) ?? 0)-(GetX() ?? 0),(GetY(pExit) ?? 0)-(GetY() ?? 0)+15,nil, nil);
     }
  return(1);
 }
 return(1);
}

private func FindOtherExit()
{
  var obj, count;
  while(obj = FindObject(GetID(), nil, nil, nil, nil, OCF_Entrance, nil, nil, nil, obj))
    if(!Equal(obj, this())) // Nicht sich selbst
      if(!LocalN("pLink", obj)) // Keines, das schon einen Ausgang hat
        { Var(S42MigrationStep(count, 1, true)) = obj; }
  return(Var(Random(count)));
}

func IsDicht()
{
return(zuu);
}
