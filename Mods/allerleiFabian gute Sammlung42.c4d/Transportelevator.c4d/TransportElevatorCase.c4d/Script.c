/*-- Fahrstuhlkorb --*/

#strict
#include ELEC

protected func TransportTimer() 
{
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy(Elevator())),this());
  timer = (++timer) % 30;
  // Lore fassen
  GrabObjects();
  // Clonks umgreifen lassen
  ChangeClonkGrabs();
  // Feststecken -> Schachtbohrung
  if (Stuck() && GetComDir() != COMD_Stop() && !NoEnergy())
    DigFreeRect(GetX() - 40, GetY() - 13, 80, 26);
  if (!timer) Activity();
}

//Clonks sollen auch bei dem großen Fahrstuhlschacht gefunden werden!
private func FindWaitingClonk()
{
  var clnk, best;
  while(clnk = FindObject(0, -56, RangeTop - GetY(), 112, 6000, OCF_CrewMember(), 0, 0, NoContainer(), clnk) )
    if(GetComDir(clnk) == COMD_Stop() || (GetAction(clnk) S= "Swim" && Inside(GetXDir(clnk), -5, 5)) ) 
//(GetComDir(clnk) == COMD_Down() || GetComDir(clnk) == COMD_Up() )) )
      if(GetAction(clnk) S= "Walk" || GetAction(clnk) S= "Push" || GetAction(clnk) S= "Scale" || GetAction(clnk) S= "Hangle" || GetAction(clnk) S= "Swim" || !(GetAction(clnk) S= "ArmedWalk") || !(GetAction(clnk) S= "HoldWalk"))
        if(!Hostile(GetController(clnk), GetOwner()) )
        {
          // Nicht erreichbar?
          if(GetY(clnk) < GetY() - 7) if(!PathFreeFahrstuhl(GetX(), GetY(), GetX(), GetY(clnk),1) ) continue;
          if(GetY(clnk) > GetY() + 7) if(!PathFreeFahrstuhl(GetX(), GetY() + 16, GetX(), GetY(clnk)) ) continue;
          // Priorität prüfen (Cursor hat Priorität, sonst nächster Clonk)
          if(!best)
            best = clnk;
          else if(GetCursor(GetController(clnk)) == clnk)
            best = clnk;
          else if(GetCursor(GetController(best)) != best)
            if(ObjectDistance(clnk) < ObjectDistance(best))
              best = clnk;
        }
  return(best);
}

private func GrabObjects()
{
  var pObject, did_grab;
  
  // Fahrzeuge
  while (pObject = FindObject(0, -36, -10, 72, 30, OCF_Grab(), 0, 0, NoContainer(), pObject)) 
  {
    // Objekt passt in den Fahrstuhlkorb
    if (FitsInElevator(pObject)) 
    {
      if (!(GetCategory(pObject) & C4D_Vehicle())) continue;
      if (!Inside(GetXDir(pObject, 100), -1, +1)) continue;
      if (pObject->~IsTree() && GetCon(pObject) >= 50) continue;
      if (GetX(pObject) == GetX() && GetY(pObject) == GetY() + 1) continue;
      if (pObject->~IsElevator() ) continue;
      if (GetProcedure(pObject) eq "FLOAT") continue;
      did_grab = true;
      
      GrabAdjustPosition(pObject);
    }
  }
  
  return(did_grab);
}

private func ChangeClonkGrabs()
{
  var clonk;
  var clonk_ocf = OCF_Living() | OCF_NotContained();
  // Schiebende Clonks suchen 80/2=40; 40-6=34
  while (clonk = FindObject(0, -34, -10, 68, 20, clonk_ocf, "Push", 0, 0, clonk)) 
  {
    if (GetComDir(clonk) != COMD_Stop() && GetComDir(clonk) != COMD_Up()) continue;
    if (!Inside(GetXDir(clonk), -2, +2)) continue;
    if (GetCommand(clonk) && GetCommand(clonk) ne "None") continue;
    if (GetMenu(clonk)) continue;
      
    // Geschobenes Objekt prüfen
    var target = GetActionTarget(0, clonk);
    if (!target) continue;
    if (target->~IsElevator() )
    {
      if(target == this())
      {
				// In den Fahrstuhlkorb laufen, wenn der Clonk am Rand steht
        if(!Inside(GetX(clonk) - GetX(), -36, +36))
        {
					SetCommand(clonk, "Grab", this());
          AddCommand(clonk, "MoveTo", 0, BoundBy(clonk->GetX(), GetX()-5, GetX()+5), GetY());
          AddCommand(clonk, "UnGrab");
        }
      }

			continue;
    }

    if (!Inside(GetX(target), GetX() - 8, GetX() + 8)) continue;
    if (ObjectDistance(this(), target) > 15) continue;
    if (!Inside(GetXDir(target), -2, +2)) continue;
    if (!PathFreeFahrstuhl(GetX(this()), GetY(this()), GetX(target), GetY(target))) continue;
    if (!FitsInElevator(target)) continue;
      
    // Geschobenes Objekt zentrieren
    GrabAdjustPosition(target);
    // Der Clonk soll nun uns anfassen
    if (!Inside(GetX(clonk) - GetX(), -5, +5)) 
    {
      //SetCommand(clonk, "UnGrab"); unnötig, wenn wir unten SetCommand benutzen...
      //AppendCommand(clonk, "MoveTo", 0, GetX()); wtf sollte denn dieses MoveTo-Kommando zur oberen Bildschirmkante...?
      SetCommand(clonk, "Grab", this());

      // Clonk soll in die Mitte laufen, wenn das Target gerade erst
      // reingeschoben wurde, sonst steht der Clonk so blöd am Rand des
      // Fahrstuhls und lässt beim Runterfahren den Korb los.
      if(!Inside(GetX(clonk) - GetX(), -36, +36))
      {
        AddCommand(clonk, "MoveTo", 0, BoundBy(clonk->GetX(), GetX()-5, GetX()+5), GetY());
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
      SetActionTargets(this(), 0, clonk);
    }
  }
}

private func FindVehicle(vehicle) 
{
  return(FindObject(0, -36, -10, 72, 30, OCF_Grab(), 0, 0, NoContainer(), vehicle));
}

private func FitsInElevator(pObject)
{
  // Alle (ungedrehten) Vertices des Objekts prüfen
  var vertexCount = GetDefCoreVal("Vertices", "DefCore", pObject->GetID());
  for (var i = 0; i < vertexCount; i++) 
  {
    var vertexX = GetDefCoreVal("VertexX", "DefCore", pObject->GetID(), i);
    var vertexY = GetDefCoreVal("VertexY", "DefCore", pObject->GetID(), i);
    // Passt nicht
    if (!Inside(vertexX, -40, 39) || !Inside(vertexY, -20, 15)) return(0);
  }
  // Passt
  return(1);  
}

private func Passenger()
{
  // Clonk finden
  var clnk = FindObject(0, -40,-13,40,20, OCF_CrewMember(),0,0,NoContainer());
  if(!clnk) return (0);
  // steht wirklich auf dem Fahrstuhl?
  if(GetProcedure(clnk) ne "WALK") return (0);
  return (clnk);
}

protected func UpdateTransferZone()
{         
  if (!Elevator()) return(SetTransferZone());
  SetTransferZone(-40, GetY(oElev) - GetY(), 40, RangeBottom - GetY(oElev) + 12);
  return(1);
}

public func PathFreeFahrstuhl(int iX1, int iY1, int iX2, int iY2)	
{
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
