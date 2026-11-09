/*--- Waggon ---*/

#strict 3
#include DOOR

local fSignalState;	// 1=Load, 0=Unload

/* Türsteuerung */

protected func SoundOpenDoor()  { Sound("SteelGate2"); }
protected func SoundCloseDoor() { Sound("SteelGate2"); }

protected func AfterDoorClose()
{
	//  if(fSignalState) SetAction("SignalArrive",fSignalState);
	//  fSignalState=0;
}

/* Steuerung */

protected func ContainedLeft(object clonk)  { [$TxtLeft$|Image=STMS:1]  return(ControlLeft(clonk));       }
protected func ContainedRight(object clonk) { [$TxtRight$|Image=STMS:2] return(ControlRight(clonk));      }
protected func ContainedLeftDouble(object clonk) { [$TxtLeftDouble$|Image=STMS:3] return(ControlLeftDouble(clonk)); }
protected func ContainedRightDouble(object clonk) { [$TxtRightDouble$|Image=STMS:4] return(ControlRightDouble(clonk)); }
protected func ContainedDown(object clonk)  { [$TxtStop$|Method=None]  return(ControlDownSingle(clonk)); }
protected func ContainedUpdate(object clonk, int comdir, bool dig, bool throw) { comdir ??= 0;
    dig ??= false;
    throw ??= false;
    return(ControlUpdate(clonk, comdir, dig, throw)); }

protected func ControlLeft(object clonk)
{
  [$TxtLeft$|Image=STMS:1]
  var pEngine = FindObject(nil,nil,nil,nil,nil,OCF_Grab,"Push",this());
  if(!pEngine) return(nil);
  pEngine->~ControlLeft(clonk);
  return(1);
}

protected func ControlRight(object clonk)
{
  [$TxtRight$|Image=STMS:2]
  var pEngine = FindObject(nil,nil,nil,nil,nil,OCF_Grab,"Push",this());
  if(!pEngine) return(nil);
  pEngine->~ControlRight(clonk);
  return(1);
}

protected func ControlLeftDouble(object clonk)
{
  [$TxtLeftDouble$|Image=STMS:3]
  var pEngine = FindObject(nil, nil,nil,nil,nil, OCF_Grab, "Push",this());
  if(!pEngine) return(nil);
  pEngine->~ControlLeftDouble(clonk);
  return(1);
}

protected func ControlRightDouble(object clonk)
{
  [$TxtRightDouble$|Image=STMS:4]
  var pEngine = FindObject(nil, nil,nil,nil,nil, OCF_Grab, "Push",this());
  if(!pEngine) return(nil);
  pEngine->~ControlRightDouble(clonk);
  return(1);
}

protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  comdir ??= 0;
  dig ??= false;
  throw ??= false;
  var pEngine = FindObject(nil, nil,nil,nil,nil, OCF_Grab, "Push",this());
  if(pEngine) pEngine->~ControlUpdate(clonk, comdir, dig, throw);
}

protected func ControlDownSingle(object pCaller)
{
  [$TxtStop$|Method=None]
  var pEngine = FindObject(nil,nil,nil,nil,nil,OCF_Grab,"Push",this());
  // No steam engine or engine has already stopped
  if (Or(!pEngine, Equal(GetComDir(pEngine), COMD_Stop)))
  {
    // Caller is inside us: exit
    if (Equal(Contained(pCaller), this()))
    {
      ActivateEntrance();
      AddCommand(pCaller, "Exit");
    }
  } 
  // Forward control to steam engine
  else 
  {
    pEngine->~ControlDownSingle(pCaller);
    return(1);
  }
}
  
/* Signalsteuerung */

public func SignalCall(object pSignal,bool fLoad)
{
  // Bei fLoad = 1 einladen, Status sichern
  fLoad ??= false;
  if(fSignalState = fLoad) return(SignalLoad());
  // sonst ausladen
  else return(SignalUnload());
}

// Einladen
private func SignalLoad()
{
  var pObj;
  // Gegenstände, Fahrzeuge suchen...
  while(pObj = FindObject(nil,-11,-6,20,20,(OCF_Collectible ?? 0)|(OCF_Grab ?? 0),nil,nil,NoContainer(),pObj))
    // ...Zugmaschine und Waggon nicht...
    if(And(!Equal(GetID(pObj), STMG), !Equal(GetID(pObj), WAGN)))
      // ...einladen
      SetCommand(pObj,"Enter",this());
  return(1);
}

// Ausladen
private func SignalUnload()
{
  // Nur bei Inhalt
  if(!ContentsCount()) return;
  // Alles raus
  for(var pContents,i=nil; pContents = Contents(i); S42MigrationStep(i, 1))
    SetCommand(pContents,"Exit");
  return(1);
}

public func SignalDepart(object pSignal)
{
  // Beim Ausladen nix
  if(!fSignalState) return(1);
  // Einladen stoppen
  var pObj;
  while(pObj = FindObject(nil,-20,-20,40,40,(OCF_Collectible ?? 0)|(OCF_Grab ?? 0)|(OCF_CrewMember ?? 0),nil,nil,NoContainer(),pObj))
    if(!!SEqual(GetCommand(pObj), "Enter"))
      SetCommand(pObj, "None");
  fSignalState = nil;
  return(1);
}

/* Forschung */

public func GetResearchBase() { return(STMG); }
