/*-- Funktionen --*/

#strict


/* exaktere Sin und Cos durch Interpolation */

global func Sin10(angle, radius)     // angle in 1/10 Grad (0 bis 3600)
{
  var k = angle % 10;
  var s = Sin(angle/10, radius*10) * (10-k);
  s += Sin(angle/10+1, radius*10) * k;
  return((s+50) / 100);
}

global func Cos10(angle, radius)
{
  var k = angle % 10;
  var s = Cos(angle/10, radius*10) * (10-k);
  s += Cos(angle/10+1, radius*10) * k;
  return((s+50) / 100);
}


/* Mehrere Baupläne hinzufügen */

global func AddSomeKnowledges(player, plan1, plan2, plan3, plan4, plan5, plan6, plan7, plan8)
{
  if (plan1)  SetPlrKnowledge(player, plan1);
  if (plan2)  SetPlrKnowledge(player, plan2);
  if (plan3)  SetPlrKnowledge(player, plan3);
  if (plan4)  SetPlrKnowledge(player, plan4);
  if (plan5)  SetPlrKnowledge(player, plan5);
  if (plan6)  SetPlrKnowledge(player, plan6);
  if (plan7)  SetPlrKnowledge(player, plan7);
  if (plan8)  SetPlrKnowledge(player, plan8);
}


/* Erweiterung von ObjectInsertMaterial:
   Direktes auffüllen ohne Fass möglich (z.B. GKW) */

global func ObjectInsertMaterial(imat, Target)
{
  if (imat == -1) return(0);   // kein Material?
  if (!Target) return(0);      // kein Zielobjekt?

//Tank aus Landwirtschaft:
	if (Target->~IsLiquidContainer())
	{
		if (Target->~LiquidInput(imat, 1, this))
			return 1;
	}

  // direktes Auffüllen probieren
  if (Target->~FillWithPipe(imat,1))
    return(1);
  // Fasstyp ermitteln
  var type = GetBarrelType(imat);
  // Fass suchen & auffüllen
  if (type)
  {
    var barrel = FindFillBarrel(Target, type);
    if (barrel)
      return(barrel->BarrelDoFill(1));
  }
  // kein Fass? Dann Objekt überlaufen lassen
  InsertMaterial(imat, GetX(Target)-GetX(), GetY(Target)-GetY());
  return(1);
}

private func FindFillBarrel(InObj, type)
{
  // auffüllbares Fass im Objekt suchen
  var obj;
  var i = -1;
  while (obj = Contents(++i, InObj))
    if (GetID(obj) == type)
      if (!obj->BarrelIsFull())
        return(obj);
  // Nix? Dann halt ein leeres Fass suchen und füllen
  obj = FindContents(EmptyBarrelID(), InObj);
  if (!obj) return(0);
  ChangeDef(type, obj);
  return(obj);
}


/* Verschiedene Ziele finden */

global func GetTargetProjectile(Range, posX, posY, pathclosed)     // Geschosse
{
  var obj;
  while (1)
  {
    obj = FindObject(0, posX,posY,-1,-1, OCF_HitSpeed2(),0,0,NoContainer(),obj);
    if (!obj) break;
    if (Distance(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)) > Range) return(0);
    if (obj->~IsProjectile())
      if (GetOwner(obj) != GetOwner())
       if (Hostile(GetOwner(),GetOwner(obj)) || GetOwner(obj)==-1) // nur feindliche und ohne besitzer
        if ((GetXDir(obj)**2 + GetYDir(obj)**2) > 2500)
          if(pathclosed) 
           return(obj);
     if (obj->~IsProjectile())
      if (GetOwner(obj) != GetOwner())
       if (Hostile(GetOwner(),GetOwner(obj)) || GetOwner(obj)==-1) // nur feindliche und ohne besitzer
        if ((GetXDir(obj)**2 + GetYDir(obj)**2) > 2500)
          if(!pathclosed)
          if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
            return(obj);
      return(0);
  }
  return(0);
}


global func GetTargetZapGlobal(Range, posX, posY, pathclosed)     // Zaps auch angreifen
{
  var obj;
  while (1)
  {
    obj = FindObject(ZAP1, posX, posY,-1,-1, 0,0,0,NoContainer(),obj);
    if (!obj) break;
    if (Distance(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)) > Range) return(0);
    if(pathclosed) 
           return(obj);
    if(!pathclosed)
    if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
      return(obj);
      return(0);
  }
  return(0);
}


global func GetTargetDetonation(Range,ID , posX, posY,pathclosed)     // alle Sprengsätze, Minen
{
  var obj;
  for ( obj in FindObjects(Find_Distance(Range),Find_NoContainer(),Find_And(Find_Func("IsMine") ,Find_Or(Find_Action("Active"),Find_Action("Ready"),Find_Action("ActiveDetonation")))) )
  {
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
  }
return(0);
}

global func GetTargetHostileDetonation(Range, ID, posX, posY,pathclosed)     // feindliche/besitzerlose Sprengsätze, Minen
{
  var obj;
  for ( obj in FindObjects(Find_Distance(Range),Find_NoContainer(),Find_And(Find_Func("IsMine"), Find_Or(Find_Owner(-1),Find_Hostile(GetOwner())) ,Find_Or(Find_Action("Active"),Find_Action("Ready"),Find_Action("ActiveDetonation")))) )
  {
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
  }
return(0);
}

global func GetTargetCrew(Range, posX, posY,pathclosed,container)     // verfeindetes Crewmitglied
{
  var obj;
  if(!container)
  for ( obj in FindObjects(Find_Distance(Range),Find_NoContainer(0),Find_OCF(OCF_Alive),Find_OCF(OCF_CrewMember), Find_Or(Find_Hostile(GetOwner(0)),Find_Owner(-1)) ) )
  {
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
  }
  if(container)
for ( obj in FindObjects(Find_Distance(Range),Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive), Find_Or(Find_Hostile(GetOwner(0)),Find_Owner(-1)) ) )
  {
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
  }
return(0);
}

global func GetTargetCrewAllied(Range, posX, posY,pathclosed,container)     // verbündetes Crewmitglied
{
  var obj;
  if(!container)
  for ( obj in FindObjects(Find_Distance(Range),Find_NoContainer(),Find_OCF(OCF_Alive),Find_OCF(OCF_CrewMember), Find_Allied(GetOwner())) )
  {
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
  }
if(container)
  for ( obj in FindObjects(Find_Distance(Range),Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive), Find_Allied(GetOwner())) )
  {
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
  }
return(0);
}


global func GetTargetVehicle(Range, posX, posY,pathclosed,container)     // gegnerisches Fahrzeug
{
  var obj;
  if(!container)
  for ( obj in FindObjects(Find_Distance(Range),Find_Or(Find_Func("IsVehicleProduct"),Find_OCF(OCF_Entrance),Find_Func("IsHazardShip")), Find_NoContainer(),Find_Hostile(GetOwner())) )
  {
  if (GetCategory(obj) & C4D_Vehicle())
     {
    if(!Hostile(GetOwner(),GetController(obj))) continue; // nur, wenn der Controller feindlich ist 
    if(obj->~IsHazardShip()) return(obj); // da diese aus vielen bauteilen bestehen, ist selten pathfree
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
     }
  }
if(container) // findet auch Objekte in Gebäuden/Fahrzeugen
  for ( obj in FindObjects(Find_Distance(Range),Find_Or(Find_Func("IsVehicleProduct"),Find_OCF(OCF_Entrance),Find_Func("IsHazardShip")),Find_Hostile(GetOwner())) )
  {
  if (GetCategory(obj) & C4D_Vehicle())
     {
    if(!Hostile(GetOwner(),GetController(obj))) continue; // nur, wenn der Controller feindlich ist 
    if(obj->~IsHazardShip()) return(obj); // da diese aus vielen bauteilen bestehen, ist selten pathfree
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
     }
  }
return(0);
}


global func GetTargetVehicleAllied(Range, posX, posY,pathclosed,container)     // verbündetes Fahrzeug
{
  var obj;
  if(!container)
  for ( obj in FindObjects(Find_Distance(Range),Find_Or(Find_Func("IsVehicleProduct"),Find_OCF(OCF_Entrance),Find_Func("IsHazardShip")), Find_NoContainer(),Find_Allied(GetController())) )
  {
  if (GetCategory(obj) & C4D_Vehicle())
     {
    if(obj->~IsHazardShip()) return(obj); // da diese aus vielen bauteilen bestehen, ist selten pathfree
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
     }
  }

  if(container) // findet auch Objekte in Gebäuden/Fahrzeugen
  for ( obj in FindObjects(Find_Distance(Range),Find_Or(Find_Func("IsVehicleProduct"),Find_OCF(OCF_Entrance),Find_Func("IsHazardShip")),Find_Allied(GetController())) )
  {
  if (GetCategory(obj) & C4D_Vehicle())
     {
    if(obj->~IsHazardShip()) return(obj); // da diese aus vielen bauteilen bestehen, ist selten pathfree
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
     }
  }
return(0);
}


global func GetTargetAnimal(Range, posX, posY,pathclosed)     // alle Tiere
{
  var obj;
  while (1)
  {
    obj = FindObject(0, posX, posY,-1,-1, OCF_Alive(),0,0,NoContainer(),obj);
    if (!obj) break;
    if (Distance(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)) > Range) return(0);
    if (obj->~IsAnimalAim() || obj->~IsAnimal() )
     {
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
     }
      return(0);
  }
  return(0);
}

global func GetOtherThreats(Range, posX, posY,pathclosed,container) // findet Objekte mit "IsThreat" funktion(also bedrohung), sofern sie keinen Besitzer haben (findet also auch scorpion angriffe. Wenn der scorpion einem menschen gehört und dieser aussteigt, wird nur der kontroller auf kein"besitzer" umgestellt, der besitzer bleibt der spieler)
{  // sind also gefährliche tiere, aliens und auch scorpions/predator
  var obj;
  if(!container)
  for ( obj in FindObjects(Find_Distance(Range),Find_Func("IsThreat"), Find_NoContainer(),Find_Owner(-1) ))
  {
    if(obj->~IsHazardShip()) return(obj); // da diese aus vielen bauteilen bestehen, ist selten pathfree
    if(GetOCF(obj) & OCF_Living && !(GetOCF(obj) & OCF_Alive)) // wenn totes Lebewesen, dann continue
    continue;
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
  }

  if(container)
  for ( obj in FindObjects(Find_Distance(Range),Find_Func("IsThreat"),Find_Owner(-1) ))
  {
    if(obj->~IsHazardShip()) return(obj); // da diese aus vielen bauteilen bestehen, ist selten pathfree
    if(GetOCF(obj) & OCF_Living && !(GetOCF(obj) & OCF_Alive)) // wenn totes Lebewesen, dann continue
    continue;
    if(pathclosed)
           return(obj);
    if(!pathclosed)
        if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
           return(obj);
  }
}


/* prüfen, ob Stromleitungen angeschlossen sind */

global func CheckPWRL(caller) 
{
  var line;
  while (line = FindObject(PWRL,0,0,0,0, 0,"Connect",caller,0, line))
    if (GetActionTarget(0,line) == caller) return(1);
  return(0);
}

