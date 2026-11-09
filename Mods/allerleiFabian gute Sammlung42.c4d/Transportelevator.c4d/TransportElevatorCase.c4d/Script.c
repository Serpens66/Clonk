/*-- Fahrstuhlkorb --*/

#strict 3
#include ELEC

protected func TransportTimer() 
{
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy(Elevator())),this());
  timer = (S42MigrationStep(timer, 1)) % 30;
  // Lore fassen
  GrabObjects();
  // Clonks umgreifen lassen
  ChangeClonkGrabs();
  // Feststecken -> Schachtbohrung
  if (And(And(Stuck(), !Equal(GetComDir(), COMD_Stop)), !NoEnergy()))
    DigFreeRect((GetX() ?? 0) - 40, (GetY() ?? 0) - 13, 80, 26);
  if (!timer) Activity();
}

//Clonks sollen auch bei dem großen Fahrstuhlschacht gefunden werden!
private func FindWaitingClonk()
{
  var clnk, best;
  while(clnk = FindObject(nil, -56, (RangeTop ?? 0) - (GetY() ?? 0), 112, 6000, OCF_CrewMember, nil, nil, NoContainer(), clnk) )
    if(Or(Equal(GetComDir(clnk), COMD_Stop), (And(!!SEqual(GetAction(clnk), "Swim"), Inside(GetXDir(clnk), -5, 5)))) )
//(GetComDir(clnk) == COMD_Down() || GetComDir(clnk) == COMD_Up() )) )
      if(Or(Or(Or(Or(Or(Or(!!SEqual(GetAction(clnk), "Walk"), !!SEqual(GetAction(clnk), "Push")), !!SEqual(GetAction(clnk), "Scale")), !!SEqual(GetAction(clnk), "Hangle")), !!SEqual(GetAction(clnk), "Swim")), !(!!SEqual(GetAction(clnk), "ArmedWalk"))), !(!!SEqual(GetAction(clnk), "HoldWalk"))))
        if(!Hostile(GetController(clnk), GetOwner()) )
        {
          // Nicht erreichbar?
          if((GetY(clnk) ?? 0) < (GetY() ?? 0) - 7) if(!PathFreeFahrstuhl(GetX(), GetY(), GetX(), GetY(clnk),1) ) continue;
          if((GetY(clnk) ?? 0) > (GetY() ?? 0) + 7) if(!PathFreeFahrstuhl(GetX(), (GetY() ?? 0) + 16, GetX(), GetY(clnk)) ) continue;
          // Priorität prüfen (Cursor hat Priorität, sonst nächster Clonk)
          if(!best)
            best = clnk;
          else if(Equal(GetCursor(GetController(clnk)), clnk))
            best = clnk;
          else if(!Equal(GetCursor(GetController(best)), best))
            if((ObjectDistance(clnk) ?? 0) < (ObjectDistance(best) ?? 0))
              best = clnk;
        }
  return(best);
}

private func GrabObjects()
{
  var pObject, did_grab;
  
  // Fahrzeuge
  while (pObject = FindObject(nil, -36, -10, 72, 30, OCF_Grab, nil, nil, NoContainer(), pObject))
  {
    // Objekt passt in den Fahrstuhlkorb
    if (FitsInElevator(pObject)) 
    {
      if (!((GetCategory(pObject) ?? 0) & (C4D_Vehicle ?? 0))) continue;
      if (!Inside(GetXDir(pObject, 100), -1, +1)) continue;
      if (And(pObject->~IsTree(), (GetCon(pObject) ?? 0) >= 50)) continue;
      if (And(Equal(GetX(pObject), GetX()), Equal(GetY(pObject), (GetY() ?? 0) + 1))) continue;
      if (pObject->~IsElevator() ) continue;
      if (!!SEqual(GetProcedure(pObject), "FLOAT")) continue;
      did_grab = true;
      
      GrabAdjustPosition(pObject);
    }
  }
  
  return(did_grab);
}

private func ChangeClonkGrabs()
{
  var clonk;
  var clonk_ocf = (OCF_Living ?? 0) | (OCF_NotContained ?? 0);
  // Schiebende Clonks suchen 80/2=40; 40-6=34
  while (clonk = FindObject(nil, -34, -10, 68, 20, clonk_ocf, "Push", nil, nil, clonk))
  {
    if (And(!Equal(GetComDir(clonk), COMD_Stop), !Equal(GetComDir(clonk), COMD_Up))) continue;
    if (!Inside(GetXDir(clonk), -2, +2)) continue;
    if (And(GetCommand(clonk), !SEqual(GetCommand(clonk), "None"))) continue;
    if (GetMenu(clonk)) continue;
      
    // Geschobenes Objekt prüfen
    var target = GetActionTarget(nil, clonk);
    if (!target) continue;
    if (target->~IsElevator() )
    {
      if(Equal(target, this()))
      {
				// In den Fahrstuhlkorb laufen, wenn der Clonk am Rand steht
        if(!Inside((GetX(clonk) ?? 0) - (GetX() ?? 0), -36, +36))
        {
					SetCommand(clonk, "Grab", this());
          AddCommand(clonk, "MoveTo", nil, BoundBy(clonk->GetX(), (GetX() ?? 0)-5, (GetX() ?? 0)+5), GetY());
          AddCommand(clonk, "UnGrab");
        }
      }

			continue;
    }

    if (!Inside(GetX(target), (GetX() ?? 0) - 8, (GetX() ?? 0) + 8)) continue;
    if ((ObjectDistance(this(), target) ?? 0) > 15) continue;
    if (!Inside(GetXDir(target), -2, +2)) continue;
    if (!PathFreeFahrstuhl(GetX(this()), GetY(this()), GetX(target), GetY(target))) continue;
    if (!FitsInElevator(target)) continue;
      
    // Geschobenes Objekt zentrieren
    GrabAdjustPosition(target);
    // Der Clonk soll nun uns anfassen
    if (!Inside((GetX(clonk) ?? 0) - (GetX() ?? 0), -5, +5))
    {
      //SetCommand(clonk, "UnGrab"); unnötig, wenn wir unten SetCommand benutzen...
      //AppendCommand(clonk, "MoveTo", 0, GetX()); wtf sollte denn dieses MoveTo-Kommando zur oberen Bildschirmkante...?
      SetCommand(clonk, "Grab", this());

      // Clonk soll in die Mitte laufen, wenn das Target gerade erst
      // reingeschoben wurde, sonst steht der Clonk so blöd am Rand des
      // Fahrstuhls und lässt beim Runterfahren den Korb los.
      if(!Inside((GetX(clonk) ?? 0) - (GetX() ?? 0), -36, +36))
      {
        AddCommand(clonk, "MoveTo", nil, BoundBy(clonk->GetX(), (GetX() ?? 0)-5, (GetX() ?? 0)+5), GetY());
        AddCommand(clonk, "UnGrab");
      }
     // else
     // {
        // Hack: der Clonk hat wahrscheinlich soeben erst mit ControlDown angehalten.
        // Das darauf folgende ControlDownSingle sollte nicht als Kommando 'Runter'
        // für den Fahrstuhl ausgewertet werden!
   //     pIgnoreClonkDown = clonk;
     // }
    } 
    else 
    {
      SetActionTargets(this(), nil, clonk);
    }
  }
}

private func FindVehicle(vehicle) 
{
  return(FindObject(nil, -36, -10, 72, 30, OCF_Grab, nil, nil, NoContainer(), vehicle));
}

private func FitsInElevator(pObject)
{
  // Alle (ungedrehten) Vertices des Objekts prüfen
  var vertexCount = GetDefCoreVal("Vertices", "DefCore", pObject->GetID());
  for (var i = nil; (i ?? 0) < (vertexCount ?? 0); S42MigrationStep(i, 1, true))
  {
    var vertexX = GetDefCoreVal("VertexX", "DefCore", pObject->GetID(), i);
    var vertexY = GetDefCoreVal("VertexY", "DefCore", pObject->GetID(), i);
    // Passt nicht
    if (Or(!Inside(vertexX, -40, 39), !Inside(vertexY, -20, 15))) return(nil);
  }
  // Passt
  return(1);  
}

private func Passenger()
{
  // Clonk finden
  var clnk = FindObject(nil, -40,-13,40,20, OCF_CrewMember,nil,nil,NoContainer());
  if(!clnk) return (nil);
  // steht wirklich auf dem Fahrstuhl?
  if(!SEqual(GetProcedure(clnk), "WALK")) return (nil);
  return (clnk);
}

protected func UpdateTransferZone()
{         
  if (!Elevator()) return(SetTransferZone());
  SetTransferZone(-40, (GetY(oElev) ?? 0) - (GetY() ?? 0), 40, (RangeBottom ?? 0) - (GetY(oElev) ?? 0) + 12);
  return(1);
}

public func PathFreeFahrstuhl(int iX1, int iY1, int iX2, int iY2)	
{
	iX1 ??= 0;
	iY1 ??= 0;
	iX2 ??= 0;
	iY2 ??= 0;
	for(var pTemp in FindObjects(Find_ID(BEKP)))
		{
		pTemp->EndMaskr();
		}

var wegfrei= PathFree(iX1, iY1, iX2, iY2);

	for(var pTemp in FindObjects(Find_ID(BEKP)))
		{
		pTemp->BeginMaskr();
		}
return(wegfrei);
}
