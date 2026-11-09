/*-- Spind --*/

#strict 3
local fLocked;

func Completion() {
SetCategory(C4D_Structure);
  SetAction("Closed");
}

/*-- Truhe --*/

protected func Open()
{
  if (!SEqual(GetAction(), "Closed")) return;
  // Truhe ist abgeschlossen
  if (fLocked)
  {
    Message("$MsgLocked$", this());
    Sound("Click");
    return;
  }
  SetAction("Opening");
}
  
protected func Close()
{
  if (!SEqual(GetAction(), "Open")) return;
  SetAction("Closing");	    
}

protected func Grabbed(object pClonk, bool fGrab)
{
  // Die Truhe wurde gerade angefasst oder losgelassen
  fGrab ??= false;
  CheckGrab();
}

protected func GrabLost(object pClonk)
{
  // Ein anfassender Clonk hat seinen Halt verloren
  CheckGrab();
}

// Status überprüfen, wenn die Truhe...
// * angefasst oder losgelassen wird
// * sich fertig geöffnet oder geschlossen hat
// * auf- oder zugeschlossen wurde
// * ein anfassender Clonk seinen Halt verloren hat
// * bekanntes Problem: wird ein Clonk durch Schaden weggeschleudert, bekommt die Truhe das nicht mit (es gibt kein GrabLost)

private func CheckGrab()
{
  // Die Truhe soll sich selbst öffnen, wenn (mindestens) ein Clonk sie anfasst...
  if (FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,"Push",this()))
    Open();
  // ...und schließen, wenn nicht
  else
    Close();
}

// Auf- und zuschließen

public func Lock()
{
  // Bereits abgeschlossen
  if (fLocked) return(1);
  // Abschließen    
  fLocked = true;
  Sound("Click");  
  CheckGrab();
  return(1);
}

public func Unlock()
{
  // Nicht abgeschlossen
  if (!fLocked) return(1);
  // Aufschließen
  fLocked = nil;
  Sound("Click");  
  CheckGrab();
  return(1);
}

protected func RejectCollect(id idObj, object pObj)
{
  // Abgeschlossen: Hineinlegen nicht möglich
  if (fLocked)
  {
    Message("$MsgLocked$", this());
    Sound("Error");
    return(true);
  }
  // Nicht abgeschlossen: Hineinlegen okay
  return(nil);
}

protected func RejectContents()
{
  // Abgeschlossen: Herausnehmen nicht möglich
  if (fLocked)
  {
    Message("$MsgLocked$", this());
    Sound("Error");
    return(true);
  }
  // Nicht abgeschlossen: Herausnehmen okay
  return(nil);
}

// Sound-Effekte

protected func Opening() 
{
  Sound("ChestOpen"); 
}
  
protected func Closing() 
{
  Sound("ChestClose");
}

// Steuerung

protected func ControlDig(object pClonk)
{
  // Herausnehmen per Graben: Holen-Menü öffnen
  pClonk->SetCommand(nil, "Get", this(), nil,nil, nil, 1);
}




//spint:


func Damage(int iChange) {
  iChange ??= 0;
  if(!SEqual(GetAction(), "Open"))
    SetAction("Open");
    
  AddEffect("Opening",this(),50,1,this(),GetID(),Abs(iChange));
}

func FxOpeningStart(object pTarget, int iEffectNumber, int iTemp, int speed) {
  iEffectNumber ??= 0;
  iTemp ??= 0;
  speed ??= 0;
  EffectVar(nil,pTarget,iEffectNumber) = speed;
  EffectVar(1,pTarget,iEffectNumber) = 1;
}

func FxOpeningTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if(And((EffectVar(nil,pTarget,iEffectNumber) ?? 0) < 15, (iEffectTime ?? 0)%(15-(EffectVar(nil,pTarget,iEffectNumber) ?? 0))))
    return;
  var phase = pTarget->GetPhase();
  S42MigrationAssign(phase, "+=", (Max(1,(EffectVar(nil,pTarget,iEffectNumber) ?? 0)/15) ?? 0)*(EffectVar(1,pTarget,iEffectNumber) ?? 0));
  if((phase ?? 0) > 5) {
    EffectVar(1,pTarget,iEffectNumber) = -1;
    phase = 5-(phase ?? 0)%5;
  }
  if((phase ?? 0) < 0) {
    EffectVar(1,pTarget,iEffectNumber) = 1;
    phase = (phase ?? 0)%5;
  }
  pTarget->SetPhase(phase);
  if(S42MigrationStep(EffectVar(nil,pTarget,iEffectNumber), -1) <= 0)
    return(-1);
}

func Hit()
{
  // Bumpf
  Sound("ClonkHit*");
}

public func IsCraneGrabable() { return(1); }
