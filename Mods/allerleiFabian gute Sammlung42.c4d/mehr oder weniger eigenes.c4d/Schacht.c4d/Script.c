/*-- Schacht --*/

#strict
#include BAS2

local pLink,istausgang, ausgang,ausgangerstellt;

protected func Initialize()
{ 
  SetEntrance(1); 
}

func Ausgangerzeugen()
{
if(istausgang==1)
return(1);
ausgang = CreateObject(CAVE,AbsX(GetX()),AbsY(GetY())+60,GetOwner());
FreeRect((GetX(ausgang))-7,(GetY(ausgang))-7,18,22);
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
if(GetCon(this())!=100)
{return(1);}

//fürs abschließen
  if(zuu==1) SetEntrance(0);
  if(zuu==0) SetEntrance(1);

 // erstelle zweite höhle direkt darunter,sobald jemand eintritt
if(ausgangerstellt==0 && Contents())
{
Ausgangerzeugen();ausgangerstellt=1;
}

// wenn ausgang nicht mehr da, entfernen.
if(ausgangerstellt==1 && !FindObject2(Find_Exclude(),Find_ID(CAVE),Find_AtRect(GetX(pLink)-GetX(),GetY(pLink)-GetY(),2,2)))
{RemoveObject();return();}

//nicht weitermachen, wenn noch kein Ausgang
if(ausgangerstellt==0)
{return(1);}

if(GetCon(pLink)!=100)
return();

// ausgang feststellen
  var pExit;
  if(GetType(pLink) == C4V_Array)
  {
    pExit = pLink[Random(GetLength(pLink))];
  }
  else pExit = pLink;
  if(!pExit) pExit = FindOtherExit();
  if(!pExit) {return();}

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
     Exit(Contents(),0,15);  
    }
  else 
     {
     Exit(Contents(), GetX(pExit)-GetX(),GetY(pExit)-GetY()+15,0, 0);
     }
  return(1);
 }
 return(1);
}

private func FindOtherExit()
{
  var obj, count;
  while(obj = FindObject(GetID(), 0, 0, 0, 0, OCF_Entrance, 0, 0, 0, obj)) 
    if(obj != this()) // Nicht sich selbst
      if(!LocalN("pLink", obj)) // Keines, das schon einen Ausgang hat
        { Var(count++) = obj; }
  return(Var(Random(count)));
}

func IsDicht()
{
return(zuu);
}
