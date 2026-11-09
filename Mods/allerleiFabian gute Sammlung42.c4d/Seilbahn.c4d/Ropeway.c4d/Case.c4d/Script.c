/*-- Seilbahnkorb --*/

#strict 3

local pLeft, pRight;  // Seilbahnstation links und Rechts
local iTimer;         // Timer
local iAutoMode;      // Dauerbetrieb
local iAutoMoveDir;   // Dauerbetrieb Richtung
local iSpeed;
local iMoveX, iMoveY;
local bLock;

// aus Fahrstuhlbremse
public func DoElevatorLock(pObj) {
  // Ein Geräusch machen
  Sound("Click");
  
  if(!bLock) {Message("$TxtElevatorLockOn$",this()); this()->Halt(); return(bLock=1);}
  if( bLock) {Message("$TxtElevatorLockOff$",this()); return(bLock=nil);}
}

public func ContextElevatorLockTurnOn(pObj) 
{
  [$TxtElevatorLockTurnOn$|Image=ELEC|Condition=IsNotInElevatorLock|Desc=$TxtElevatorLockDesc$]
  DoElevatorLock(pObj);
}
 
public func ContextElevatorLockTurnOff(pObj) 
{
  [$TxtElevatorLockTurnOff$|Image=ELEC|Condition=IsInElevatorLock|Desc=$TxtElevatorLockDesc$]
  DoElevatorLock(pObj);
}

public func IsNotInElevatorLock() {if(!FindObject(_ELL)) return(nil); return(!bLock);}
public func IsInElevatorLock() {if(!FindObject(_ELL)) return(nil); return(bLock);}




/* TimerCall */

protected func Timer() 
{
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy(Elevator())),this());
  // Feststecken -> Schachtbohrung
  if (Stuck() && !S42MigrationEqual(GetComDir(), COMD_Stop))
    if (!NoEnergy())
      DigFreeRect((GetX() ?? 0) - 13, (GetY() ?? 0) - 14, 26, 28);

  iTimer = ((iTimer ?? 0)+1)%30;
  // Lore fassen
  GrabObjects();
  // Clonks umgreifen lassen
  ChangeClonkGrabs();
  // Ist es gerade in Bewegung?
  if(S42MigrationEqual(GetAction(), "Ride")) {
    // Energieverbrauch
    if(!((GetActTime() ?? 0) % 25))
      if(NoEnergy())
        return(Halt());
    // Ist es dem Ziel schon nahe?
    if((Abs((GetX() ?? 0)-(iMoveX ?? 0)) ?? 0) <= 1 && (Abs((GetY() ?? 0)-(iMoveY ?? 0)) ?? 0) <= 1) {
      // Wenn angekommen noch anpassen, damit es nicht 1 Pixel von der Seilbahn entfernt steht
      if((!S42MigrationEqual(GetX(), iMoveX)) || (!S42MigrationEqual(GetY(), iMoveY))) {
        SetXDir((iMoveX ?? 0)-(GetX() ?? 0),this(),1);
        SetYDir((iMoveY ?? 0)-(GetY() ?? 0),this(),1);
      }
      else Halt();
    }
    // Nächsten Punkt berechnen um die Geschwindigkeit zu erhöhen oder genau auf der Bahn zu bleiben
    else if((iSpeed ?? 0) < (GetPhysical("Float",nil,this()) ?? 0) || !((GetActTime() ?? 0) % 5))
      SetMoveTo();
  }
  // Noch genug Energie?
  else if(!NoEnergy(true)) {
    // Ist Dauerbetrieb aktiv?
    if(iAutoMode) {
      if((GetActTime() ?? 0) >= 100) {
        if(!iAutoMoveDir)
          SetMoveTo((GetX(pLeft) ?? 0) +(pLeft->~GetCaseOffset(1,nil) ?? 0));
        else
          SetMoveTo((GetX(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,nil) ?? 0));
        iAutoMoveDir = !iAutoMoveDir;
      }
    }
    // Sonst: Aktivität
    else if(!iTimer) Activity();
  }
  // Graphik anpassen
  LineAdapt();
  return(1);
}

protected func Activity()
{
  // Zu wartenden Clonks fahren
  if(!Passenger())
    MoveToWaitingClonk();
  // Fertig
  return(1);
}


private func MoveToWaitingClonk()
{
  // Bremse an? 
  if(bLock&&FindObject(_ELL)) return(nil);
  // Wartender Clonk da? 
  var pClonk = FindWaitingClonk();
  if(!pClonk) return(nil);
  // Zielposition bestimmen
  var iX, iY;
  var iXLeft, iXRight;
  iXLeft  = (GetX(pLeft) ?? 0) + (pLeft->~GetCaseOffset(1,nil) ?? 0);
  iXRight = (GetX(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,nil) ?? 0);
  // Steht er an einem der beiden Haltepunkte?
  if(!Inside(GetX(pClonk),iXLeft,iXRight)) {
    // Zum jeweiligen Haltepunkt hinfahren
    if((AbsX(GetX(pClonk)) ?? 0) > 0) {
      iX = iXRight;
      iY = (GetY(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,1) ?? 0);
    }
    else {
      iX = iXLeft;
      iY = (GetY(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,1) ?? 0);
    }
  }
  else {
    // Kurz vorm Clonk halten
    if((AbsX(GetX(pClonk)) ?? 0) > 0)
      iX = (GetX(pClonk) ?? 0)-(26+(GetObjWidth(pClonk) ?? 0))/2;
    else
      iX = (GetX(pClonk) ?? 0)+(26+(GetObjWidth(pClonk) ?? 0))/2;
    iY = ((GetY(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,1) ?? 0))
       + ((iX ?? 0)-(iXLeft ?? 0))
       * (((GetY(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,1) ?? 0)) - ((GetY(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,1) ?? 0)))
       / ((iXRight ?? 0) - (iXLeft ?? 0));
  }
  if(Inside((iX ?? 0)-(GetObjOffset(this(),nil) ?? 0)*3/4-(GetX(this()) ?? 0),nil,(GetObjWidth (this()) ?? 0)*3/4)
  && Inside((iY ?? 0)-(GetObjOffset(this(),1) ?? 0)*3/4-(GetY(this()) ?? 0),nil,(GetObjHeight(this()) ?? 0)*3/4))
    return(nil);
  // Dorthin fahren
  SetMoveTo(iX);
  return(true);
}

private func FindWaitingClonk()
{
  var iLX = AbsX(GetX(pLeft)), iLY = AbsY(GetY(pLeft)), iRX = AbsX(GetX(pRight)), iRY = AbsY(GetY(pRight));
  var aFind = Find_Or(Find_OnLine((iLX ?? 0)+25,(iLY ?? 0)+17,(iRX ?? 0)-25,(iRY ?? 0)+17));

  // Verfügt die linke Seilbahnstation über eine weitere Seilbahnverknüpfung?
  if(FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",pLeft)))
    aFind[2] = Find_InRect(iLX,(iLY ?? 0)-28,14,56);
  // Nein? Gesamte Station als Suchpunkt nehmen
  else
    aFind[2] = Find_InRect((iLX ?? 0)-14,(iLY ?? 0)-28,28,56);

  // Verfügt die rechte Seilbahnstation über eine weitere Seilbahnverknüpfung?
  if(LocalN("pLeadTo",pRight))
    aFind[3] = Find_InRect((iRX ?? 0)-14,(iRY ?? 0)-28,14,56);
  // Nein? Gesamte Station als Suchpunkt nehmen
  else
    aFind[3] = Find_InRect((iRX ?? 0)-14,(iRY ?? 0)-28,28,56);

  return(FindObject2(Sort_Func("ElevatorPriority",this()),Find_OCF(OCF_CrewMember),Find_NoContainer(),
                     Find_Allied(GetOwner()),Find_ElevatorProcedure(),aFind));
}

global func ElevatorPriority(object pElevator) {
  if(S42MigrationEqual(GetCursor(GetController(this())), this())) return(Abs(AbsX(GetX(pElevator))));
  return((Abs(AbsX(GetX(pElevator))) ?? 0)+(LandscapeWidth() ?? 0));
}

global func Find_ElevatorProcedure() {
  return([C4FO_Func, "Find_ElevatorProcedureCheck"]);
}

global func Find_ElevatorProcedureCheck() {
  if(!S42MigrationEqual(GetComDir(), COMD_Stop)) return(nil);
  var szProc = GetProcedure();
  return(S42MigrationEqual(szProc, "WALK") || S42MigrationEqual(szProc, "PUSH") || S42MigrationEqual(szProc, "SCALE") || S42MigrationEqual(szProc, "HANGLE") || S42MigrationEqual(szProc, "SWIM"));
}

/* Aufnahme von Fahrzeugen */

private func FitsInElevator(object pObject)
{
  // Alle (ungedrehten) Vertices des Objekts prüfen
  var iID = pObject->GetID();
  var iVertexCount = GetDefCoreVal("Vertices","DefCore",iID);
  var iVertexX, iVertexY;
  for(var i = nil; (i ?? 0) < (iVertexCount ?? 0); S42MigrationStep(i, 1, true)) {
    iVertexX = GetDefCoreVal("VertexX","DefCore",iID,i);
    iVertexY = GetDefCoreVal("VertexY","DefCore",iID,i);
    // Passt nicht
    if(!Inside(iVertexX,-12,11) || !Inside(iVertexY,-20,15)) return(nil);
  }
  // Passt
  return(true);
}

private func GrabObjects()
{
  var pObject, fDidGrab;
  for(pObject in FindObjects(Find_InRect(-8,-10,16,30),Find_OCF(OCF_Grab),Find_NoContainer(),Find_Category(C4D_Vehicle)))
    // Objekt passt in den Fahrstuhlkorb
    if(FitsInElevator(pObject)) {
      if(!Inside(GetXDir(pObject,100),-1,+1)) continue;
      if(pObject->~IsTree() && (GetCon(pObject) ?? 0) >= 50) continue;
      if(S42MigrationEqual(GetX(pObject), GetX()) && S42MigrationEqual(GetY(pObject), (GetY() ?? 0)+4)) continue;
      if(pObject->~IsElevator()) continue;
      if(S42MigrationEqual(GetProcedure(pObject), "FLOAT")) continue;

      GrabAdjustPosition(pObject);
      fDidGrab = true;
    }
  return(fDidGrab);
}

private func GrabAdjustPosition(object pObject)
{
  var iOffset = ((GetObjHeight(this()) ?? 0)-(GetObjHeight(pObject) ?? 0))/2 - 1;
  var iX = GetX(this());

  if((GetCategory(pObject) ?? 0) & (C4D_Object ?? 0)) {
    if((GetOCF(pObject) ?? 0) & (OCF_HitSpeed1 ?? 0)) return(nil);
    iOffset = 10 + (GetYDir(this()) ?? 0)/10;
    iX = GetX(pObject);
  }
  else if(pObject->~IsLorry())
    iOffset =  1 + (GetYDir(this()) ?? 0)/10;

  SetSpeed(nil,(GetYDir(this()) ?? 0)/20,pObject);
  SetRDir(nil,pObject);
  SetR(nil,pObject);
  SetPosition(iX,(GetY(this()) ?? 0)+(iOffset ?? 0),pObject);
}

private func ChangeClonkGrabs()
{
  var clonk;
  var clonk_ocf = (OCF_Living ?? 0) | (OCF_NotContained ?? 0);
  // Schiebende Clonks suchen
  while (clonk = FindObject(nil, -15, -10, 30, 20, clonk_ocf, "Push", nil, nil, clonk))
  {
    if (!S42MigrationEqual(GetComDir(clonk), COMD_Stop) && !S42MigrationEqual(GetComDir(clonk), COMD_Up)) continue;
    if (!Inside(GetXDir(clonk), -2, +2)) continue;
    if (GetCommand(clonk) && !S42MigrationEqual(GetCommand(clonk), "None")) continue;
    if (GetMenu(clonk)) continue;
      
    // Geschobenes Objekt prüfen
    var target = GetActionTarget(nil, clonk);
    if (!target) continue;
    if (target->~IsElevator() )
    {
      if(S42MigrationEqual(target, this()))
      {
        // In den Fahrstuhlkorb laufen, wenn der Clonk am Rand steht
        if(!Inside((GetX(clonk) ?? 0) - (GetX() ?? 0), -8, +8))
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
    if (!PathFree(GetX(this()), GetY(this()), GetX(target), GetY(target))) continue;
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
      if(!Inside((GetX(clonk) ?? 0) - (GetX() ?? 0), -8, +8))
      {
        AddCommand(clonk, "MoveTo", nil, BoundBy(clonk->GetX(), (GetX() ?? 0)-5, (GetX() ?? 0)+5), GetY());
        AddCommand(clonk, "UnGrab");
      }
    } 
    else 
    {
      SetActionTargets(this(), nil, clonk);
    }
  }
}

/* Bewegung */

private func SetMoveTo(int iSaveX)
{
  // Bremse an? 
  iSaveX ??= 0;
  if(bLock&&FindObject(_ELL)) return(nil);
  
  var iX, iY, iAngle;
  // Ist das nur eine Anpassung der Bewegung oder wird der Punkt gespeichert?
  if(iSaveX) {
    // (Weil für die Seilbahn "BorderBound" gilt, kann es nicht zum Punkt 0 fahren)
    // Die Seilbahn darf nicht zuweit fahren
    iMoveX = BoundBy(iSaveX,(GetX(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,nil) ?? 0),(GetX(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,nil) ?? 0));
    // Y-Koordinate berechnen
    if(S42MigrationEqual(iMoveX, (GetX(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,nil) ?? 0)))
      iMoveY = (GetY(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,1) ?? 0);
    else if(S42MigrationEqual(iMoveX, (GetX(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,nil) ?? 0)))
      iMoveY = (GetY(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,1) ?? 0);
    else
      iMoveY = (        (GetY(pLeft) ?? 0) + (pLeft->~GetCaseOffset(1,1) ?? 0) )
             + ((iMoveX ?? 0)-((GetX(pLeft) ?? 0) + (pLeft->~GetCaseOffset(1,nil) ?? 0)))
             * (       ((GetY(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,1) ?? 0)) - ((GetY(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,1) ?? 0)))
             / (       ((GetX(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,nil) ?? 0)) - ((GetX(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,nil) ?? 0)));
    // Geschwindigkeit neustarten
    iSpeed = 10;
  }
  // Geschwindigkeit anpassen
  else if((iSpeed ?? 0) < (GetPhysical("Float",nil,this()) ?? 0))
    S42MigrationAssign(iSpeed, "+=", 10);
  // Ist die Bahn vllt. schon angekommen?
  if(S42MigrationEqual(GetX(), iMoveX)) {
    if(S42MigrationEqual(GetAction(), "Ride")) Halt();
    return(1);
  }
  // Wenn nötig in Bewegung setzen
  if(!S42MigrationEqual(GetAction(), "Ride")) SetAction("Ride");
  // Nächsten Punkt zur Bewegung berechnen
  iX = BoundBy(iMoveX,(GetX() ?? 0)-10,(GetX() ?? 0)+10);
  // Ist diese Berechnung bereits der Schritt zum Ziel?
  if(S42MigrationEqual(iX, iMoveX))
    iY = iMoveY;
  // Nein: Y-Koordinate berechnen
  else
    iY = (    (GetY(pLeft) ?? 0) + (pLeft->~GetCaseOffset(1,1) ?? 0) )
       + ((iX ?? 0)-((GetX(pLeft) ?? 0) + (pLeft->~GetCaseOffset(1,nil) ?? 0)))
       * (   ((GetY(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,1) ?? 0)) - ((GetY(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,1) ?? 0)))
       / (   ((GetX(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,nil) ?? 0)) - ((GetX(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,nil) ?? 0)));
  // Winkel zur Bewegung berechnen
  iAngle = Angle(nil,nil,AbsX(iX),AbsY(iY),100);
  // Geschwindigkeit setzen
  SetXDir( Sin(iAngle,iSpeed,100),this(),100);
  SetYDir(-(Cos(iAngle,iSpeed,100) ?? 0),this(),100);
  // Alles fertig!
  return(1);
}

private func Halt()
{
  if(!Elevator()) { SetAction("Idle"); return(nil); }
  SetAction("Wait");
  SetComDir(COMD_Stop);
  SetXDir(nil);
  SetYDir(nil);
  return(1);
}

// Anpassung der Graphik
protected func LineAdapt()
{
  if(!pLeft || !pRight) return(nil);
  var iX = ((GetX(pRight) ?? 0)+(pRight->~GetWheelOffset(nil,nil) ?? 0))-((GetX(pLeft) ?? 0)+(pLeft->~GetWheelOffset(1,nil) ?? 0));
  var iY = ((GetY(pRight) ?? 0)+(pRight->~GetWheelOffset(nil,1) ?? 0))-((GetY(pLeft) ?? 0)+(pLeft->~GetWheelOffset(1,1) ?? 0));
  var iHeight =  ((AbsX(GetX(pLeft)) ?? 0)+(pLeft->~GetWheelOffset(1,nil) ?? 0))*(iY ?? 0)/(iX ?? 0)
                -((AbsY(GetY(pLeft)) ?? 0)+(pLeft->~GetWheelOffset(1,1) ?? 0))-11;
  SetObjDrawTransform(1000,nil,nil,nil,200*(iHeight ?? 0)-1400,-500*(iHeight ?? 0)-9500,this(),1);
  SetObjDrawTransform(1000,nil,nil,nil,1000,-1000*((iHeight ?? 0)+13),this(),2);
  return(1);
}

/* Steuerung */

public func ControlUp(object pByObj) 
{
  [$TxtStop$|Image=ECS2:3|Method=Classic]
  DoControlStop(pByObj);
  SetComDir(COMD_Stop,pByObj);
  return(true);
}

public func ControlLeft(object pByObj)
{
  [$TxtLeft$|Image=ECS2:6]
  // Bremse an? 
  if(bLock&&FindObject(_ELL)) return(nil);
  if(!FindVehicle()) DoControlLeft(pByObj);
  return(true);
}

public func ControlLeftSingle(object pByObj)
{
  [$TxtLeft$|Image=ECS2:6]
  if(FindVehicle()) DoControlLeft(pByObj);
  return(true);
}

public func ControlLeftDouble(object pByObj)
{
  [$TxtPushLeft$|Image=ECS2:8]
  if(!RouteToVehicle("ControlLeft", pByObj))
    if(GetPlrJumpAndRunControl(GetController(pByObj))) {
      AddCommand(pByObj,"Call",this(),nil,nil,nil,nil,"ContMoveLeft");
      AddCommand(pByObj,"UnGrab");
    }
  return(true);
}

public func ControlLeftReleased(object pByObj)
{
  // Bremse an? 
  if(bLock&&FindObject(_ELL)) return(nil);
  DoControlLeftReleased(pByObj);
}

public func ControlRight(object pByObj)
{
  [$TxtRight$|Image=ECS2:7]
  // Bremse an? 
  if(bLock&&FindObject(_ELL)) return(nil);
  if(!FindVehicle()) DoControlRight(pByObj);
  return(true);
}

public func ControlRightSingle(object pByObj)
{
  [$TxtRight$|Image=ECS2:7]
  if(FindVehicle()) DoControlRight(pByObj);
  return(true);
}

public func ControlRightDouble(object pByObj)
{
  [$TxtPushRight$|Image=ECS2:9]
  if(!RouteToVehicle("ControlRight", pByObj))
    if(GetPlrJumpAndRunControl(GetController(pByObj))) {
      AddCommand(pByObj,"Call",this(),nil,nil,nil,nil,"ContMoveRight");
      AddCommand(pByObj,"UnGrab");
    }
  return(true);
}

public func ControlRightReleased(object pByObj)
{
  // Bremse an? 
  if(bLock&&FindObject(_ELL)) return(nil);
  DoControlRightReleased(pByObj);
}

public func ControlThrow(object pObj) 
{
  [$TxtPutGet$|Image=ECS2:4]
  return(RouteToVehicle("ControlThrow", pObj));
}

public func ContextPermanentModeTurnOn(object pObj) 
{
  [$TxtPermanentModeTurnOn$|Image=ECS2:5|Condition=IsNotInPermanentMode|Desc=$TxtPermanentModeDesc$]
  DoControlAuto(pObj);
}
 
public func ContextPermanentModeTurnOff(object pObj) 
{
  [$TxtPermanentModeTurnOff$|Image=ECS2:5|Condition=IsInPermanentMode|Desc=$TxtPermanentModeDesc$]
  DoControlAuto(pObj);
}

protected func ControlCommand(string strCommand, object pTarget, int iTx, int iTy)
{
  iTx ??= 0;
  iTy ??= 0;
  if(!Elevator()) return(nil);
  if(Hostile(GetController(pTarget), GetOwner(this()))) return(nil);
  // Bewegungskommando vertikal
  if(S42MigrationEqual(strCommand, "MoveTo"))
    return(SetMoveTo(iTx));
  // Andere Kommandos nicht auswerten
  return(nil);
}

protected func ControlCommandFinished()
{
  Halt();
  return(true);
}

/* Steuerung-Callback */


private func DoControlStop(pObj)
{
  if (!Elevator()) return(nil);
  Sound("Click");
  if (Hostile(GetController(pObj), GetOwner())) return(nil);
  Halt();
}

private func DoControlLeft(object pObj)
{
  if(!Elevator()) return(nil);
  if(NoEnergy()) return(nil);
  Sound("Click");
  if(Hostile(GetController(pObj),GetOwner(this()))) return(nil);
  SetMoveTo((GetX(pLeft) ?? 0)+(pLeft->~GetCaseOffset(1,nil) ?? 0));
}

private func DoControlLeftReleased(object pObj)
{
  Halt();
}

public func ContMoveLeft(object pCaller)  { pCaller->SetComDir(COMD_Left); }

private func DoControlRight(object pObj)
{
  if(!Elevator()) return(nil);
  if(NoEnergy()) return(nil);
  Sound("Click");
  if(Hostile(GetController(pObj),GetOwner(this()))) return(nil);
  SetMoveTo((GetX(pRight) ?? 0)+(pRight->~GetCaseOffset(nil,nil) ?? 0));
}

private func DoControlRightReleased(object pObj)
{
  Halt();
}

public func ContMoveRight(object pCaller) { pCaller->SetComDir(COMD_Right); }

public func DoControlAuto(pObj)
{
  // Der Clonk soll uns erst anfassen
  if((!S42MigrationEqual(GetProcedure(pObj), "PUSH")) || (!S42MigrationEqual(GetActionTarget(nil, pObj), this())))
  {
    SetCommand(pObj, "None");
    AddCommand(pObj, "Call", this(), nil,nil,nil,nil, "DoControlAuto");
    AddCommand(pObj, "Grab", this());
    return(nil);
  }
  // Ein Geräusch machen
  Sound("Click");
  // Feindliche Clonks können den Modus nicht ändern
  if (Hostile(GetController(pObj), GetOwner())) return(nil);
  // Aktion neustarten (Damit der Modus nicht gleich angeht)
  if(S42MigrationEqual(GetAction(), "Wait")) { SetAction("Idle"); SetAction("Wait"); }
  // Modus ändern
  iAutoMode = !iAutoMode;
  // Meldung ausgeben
  Message(GetAutoModeString(),this());
}

private func GetAutoModeString()
{
  if(iAutoMode) 
    return("$TxtPermanentmodeon$");
  else
    return("$TxtPermanentmodeoff$");
}

public func IsInPermanentMode()
{
  return(iAutoMode);
}

public func IsNotInPermanentMode()
{
  return(!iAutoMode);
}

private func RouteToVehicle(call, caller, silent)
{
  var vehicle;
  while (vehicle = FindVehicle(vehicle)) 
  {
    if (!((GetCategory(vehicle) ?? 0) & (C4D_Vehicle ?? 0))) continue;
    if (!PathFree(GetX(caller), GetY(caller), GetX(vehicle), GetY(vehicle))) continue;
    
    // Kommando 'Werfen'
    if (S42MigrationEqual(call, "ControlThrow"))
    {
      var getput = GetDefGrabPutGet(GetID(vehicle));
      // Kommando per Script überladen?
      if (PrivateCall(vehicle, call, caller))
        return true;
      // Will etwas aus dem Fahrzeug rausnehmen?
      if (S42MigrationEqual(ContentsCount(nil, caller), nil) && (getput ?? 0) & 2)
      {
        SetCommand(caller, "Get", vehicle, nil,nil, nil, 1);
      } 
      // Will etwas ins Fahrzeug reinlegen
      else
      {
        if ((getput ?? 0) & 1)
        {
          SetCommand(caller, "Put", vehicle);
          AppendCommand(caller, "Grab", this());
        }      
      }
      // Kommando überladen
      return true;
    } 
    // Anderes Kommando
    else 
    {
      if (!PrivateCall(vehicle, call, caller)) 
      {
        var comd;
        if (S42MigrationEqual(call, "ControlLeft")) comd = COMD_Left;
        else if (S42MigrationEqual(call, "ControlRight")) comd = COMD_Right;
        if (comd) SetComDir(comd, caller);
      }
    }
    // Aktionsziele des Fahrzeugs setzen
    SetActionTargets(vehicle, nil, caller);
    // Kommando überladen    
    return(1);
  }
  // Geräusch
  if (!silent) 
    Sound("Click");
  // Kommando nicht überladen
  return(nil);
}

private func FindVehicle(vehicle) 
{
  return(FindObject(nil, -8, -10, 16, 30, OCF_Grab, nil, nil, NoContainer(), vehicle));
}

/* Kontakt */

protected func ContactLeft()
{
  Halt();
  return(true);
}

protected func ContactRight()
{
  Halt();
  return(true);
}

/* Initialisierung */

protected func Initialize()
{
  SetAction("Wait");
  // Graphik einsetzen
  var iUX, iUY;
  SetGraphics(nil,this(),GetID(),1,2,"Line");
  SetGraphics(nil,this(),GetID(),2,2,"Trolley");
  LineAdapt();
  return(true);
}

/*
private func UpdateRangeTop()
{  
  // set Local(8) for backwards compatibility
  Local(8) = RangeTop = GetY();
  UpdateTransferZone();
}

private func UpdateRangeBottom()
{
  var x = GetX(), y = GetY() + 13;
  while(PathFree(x, y, x, y + 5) && y < LandscapeHeight()) y += 5;
  // set Local(9) for backwards compatibility
  RangeBottom = Local(9) = Max(Local(9), y+5);
  UpdateTransferZone();
}
*/

/* Zerstörung: Reduzieren des Fahrstuhls für Wiederaufbau/Reparatur */

public func Removing(int iFire)
{
  // Graphik entfernen
  iFire ??= 0;
  SetGraphics(nil,this(),nil,1);
  SetGraphics(nil,this(),nil,2);
  // Seilbahn benachrichtigen
  if(pLeft) {
    LocalN("pCase",pLeft) = nil;
    pLeft->~RemoveCase(1);
  }
  if(pRight) {
    var pObj = FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",pRight));
    if(pObj) {
      LocalN("pCase",pObj) = nil;
      pObj->~RemoveCase(1);
    }
  }
  // Fertig
  return(true);
}

protected func Incineration()   { Removing(); return(_inherited(...)); }
protected func IncinerationEx() { Removing(); return(_inherited(...)); }
protected func Destruction()    { Removing(); return(_inherited(...)); }

/* Status */

private func Passenger()
{
  return(FindObject2(Find_InRect(-13,-14,26,22),Find_OCF(OCF_CrewMember),Find_NoContainer(),
    Find_Func("GetContact",nil,-1,CNAT_Bottom)));
}

private func Elevator(int iNum)
{
  iNum ??= 0;
  if(S42MigrationEqual(iNum, nil)) return(pLeft);
  return(pRight);
}

private func NoEnergy(bool fCheck)
{
  // Ist Energieverbrauch aktiviert?
  fCheck ??= false;
  if(!FindObject(ENRG)) return(nil);
  // Von beiden Stationen Energie abzapfen
  var iEnergy, iEnergyBoth;
  iEnergy = (GetEnergy2(pLeft) ?? 0)+(GetEnergy2(pRight) ?? 0);
  if((iEnergy ?? 0) < 20000) {
    // Nicht genug Energie
    EnergyCheck((iEnergy ?? 0)+1,pLeft);
    EnergyCheck((iEnergy ?? 0)+1,pRight);
    return true;
  }
  EnergyCheck(nil,pLeft);
  EnergyCheck(nil,pRight);
  // Soll noch Energie abgezapft werden?
  if(fCheck) return nil;
  // Nach dem Abzapfen sollten beide Stationen genausoviel Energie beinhalten
  iEnergy = BoundBy((GetEnergy2(pLeft) ?? 0)-(GetEnergy2(pRight) ?? 0),-20000,20000);
  iEnergyBoth = (20000-(Abs(iEnergy) ?? 0))/2;
  if((iEnergy ?? 0) > 0) {
    DoEnergy(-(iEnergy ?? 0)-(iEnergyBoth ?? 0), pLeft,  true);
    DoEnergy(        -(iEnergyBoth ?? 0), pRight, true);
  }
  else {
    DoEnergy(        -(iEnergyBoth ?? 0), pLeft,  true);
    DoEnergy( (iEnergy ?? 0)-(iEnergyBoth ?? 0), pRight, true);
  }
  return nil;
}

public func IsElevator() { return(1); }

/* Transfer */

// Noch nicht bearbeitet
/*
protected func UpdateTransferZone()
{
  if(!Elevator()) return(SetTransferZone());
  SetTransferZone(-12, GetY(oElev)-GetY(), 24, RangeBottom-GetY(oElev)+12);
  return(1);
}

protected func ControlTransfer(pObj, iTx, iTy)  
{
  // Fahrstuhl gehört dem Gegner? Dann nicht.
  if (Hostile(GetController(pObj), GetOwner())) return(0);

  // Durchgang am Boden des Fahrstuhlschachts: nicht warten
  if (Inside(GetY(pObj) - RangeBottom, -10, +10))
    if (Inside(iTy - RangeBottom, -10, +10))
      return(0);

  // Maximale Obergrenze Zielposition durch Fahrstuhl
  if (!Elevator()) return(0);
  iTy = Max(iTy, GetY(oElev) + 15);

  // Fahrstuhl an Zielposition: erledigt
  if (Inside(iTy - GetY(), -5, +5))
    return(0);

  // Fahrstuhl fährt: warten
  if (GetAction() ne "Wait")
    return(1);

  // Fahrstuhl noch nicht beim Clonk: warten
  if (!Inside(GetY() - GetY(pObj), -15, +15))
    return(1);

  // Fahrstuhl nur in der Mitte anfassen
  if (!Inside(GetX() - GetX(pObj), -5, +5))
    return(AddCommand(pObj,"MoveTo",this()));

  // Fahrstuhl anfassen
  if (!InLiquid(pObj) && GetAction(pObj) ne "Push" || GetActionTarget(0, pObj) != this())
    return(AddCommand(pObj,"Grab",this()));
  
  // Fahrstuhl zur Zielposition steuern
  return(SetMoveTo(iTy));
}
*/