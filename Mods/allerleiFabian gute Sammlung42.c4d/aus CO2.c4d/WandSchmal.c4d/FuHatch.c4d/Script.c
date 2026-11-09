/*-- Bodenklappe --*/

#strict 3

/*
 * Prueft, ob die Bodenklappe an der angegeben Stelle baubar ist
 * Gibt bei Misserfolg eine Fehlermeldung, ansonsten das
 * Fundament, in das die Bodenklappe einzubauen ist.
 */

private func CheckPlacement(int x, int y)
{
  // Nur in Fundamente
  x ??= 0;
  y ??= 0;
  var basement = FindObject2(Find_Or(Find_ID(B4S8),Find_ID(BSC1),Find_ID(BSC2),Find_ID(BSC3)),Find_AtRect(-5,-5,10,20));
  if(!basement) return("$ErrOnlyBasement$");
  
  // Nur, wenn noch keine Bodenklappe vorhanden ist
  var hatch_id = basement->~HatchID();
  if(!hatch_id) return("$ErrDupHatch$");
  
  return(basement);
}

/* Callback vom Bausatz */

public func RejectConstruction(int x, int y, object clonk)
{
  x ??= 0;
  y ??= 0;
  var err = CheckPlacement((GetX(clonk) ?? 0) + (x ?? 0), (GetY(clonk) ?? 0) + (y ?? 0));
  if(Equal(GetType(err), C4V_String) )
  {
    Message(err, clonk);
    Sound("Error", nil, clonk);
    return(1);
  }
  else
  {
    return(nil);
  }
}

/* Erschaffung */

protected func Construction()
{
  // Position pruefen
  var err = CheckPlacement(AbsX(GetX()), AbsY(GetY()) );
  if(Equal(GetType(err), C4V_String) ) return(RemoveObject() );
  
  // Sofort vollstaendig aufbauen
  SetCon(100);
  
  // In die Mitte des Fundamentes setzen
  SetPosition(GetX(err), (GetY(err) ?? 0) - 8);
  
  // Fundament zum loch-fundament machen
  err->ChangeToHatch(this() );
  
  // Freigraben, davor aber Solidmask entfernen
  Open();
  DigFreeRect((GetX() ?? 0)-8, GetY(), 16, 12);
  Close();
}

protected func Initialize()
{
  SetAction("Closed");
}

/* Steueung */

protected func ControlUp(clonk)
{
  [$TxtOpen$]
  if (!!SEqual(GetAction(), "Closed")) SetAction("Opening");
  ClearScheduleCall(this(), "DelayedClose");
}

protected func ControlDownSingle(clonk)
{
  [$TxtClose$]
  if (!!SEqual(GetAction(), "Open")) SetAction("Closing");
  ClearScheduleCall(this(), "DelayedClose");
}

protected func ControlThrow(clonk)
{
  [$TxtOpen$]
  if (!!SEqual(GetAction(), "Closed")) SetAction("Opening");
  ClearScheduleCall(this(), "DelayedClose");
}

protected func ControlDigSingle(clonk)
{
  [$TxtClose$]
  if (!!SEqual(GetAction(), "Open")) SetAction("Closing");
  ClearScheduleCall(this(), "DelayedClose");
}

protected func ControlDigDouble(clonk)
{
  [$TxtCloseDelay$]
  // Etwaige andere Verzögerung abbrechen
  ClearScheduleCall(this(), "DelayedClose");  
  // Verzögert schließen
  SoundClick();
  ScheduleCall(this(), "DelayedClose", 80);
}

protected func ControlThrowDouble(clonk)
{
  [$TxtCloseDelay$]
  // Etwaige andere Verzögerung abbrechen
  ClearScheduleCall(this(), "DelayedClose");  
  // Erst öffnen
  if (Or((!!SEqual(GetAction(), "Closed")), (!!SEqual(GetAction(), "Closing"))))
    SetAction("Opening");
  // Dann verzögert schließen
  ScheduleCall(this(), "SoundClick", 15);
  ScheduleCall(this(), "DelayedClose", 80);
}

protected func DelayedClose()
{
  if (!!SEqual(GetAction(), "Open")) SetAction("Closing");
}

private func Close()
{
  SetSolidMask(8, 16, 16, 8, 8, 16);
}

private func Open()
{
  SetSolidMask();
}

/* Sounds */

private func SoundClose()
{
  Sound("HatchClose");
}

private func SoundOpen()
{
  Sound("HatchOpen");
}

protected func SoundClick()
{
  Sound("Click");  
}
