/*-- Glüheffekt --*/

#strict 3


local burntime, building, switch;


public func IsWaste(){ return(10);}


protected func Initialize()
{
  switch = 1;
  return(1);
}

public func Init()
{
  building = Par(nil);
  SetAction("waiting",building);
  return(1);
}

public func Switch()
{
  if (!switch) Message("eingeschaltet", building); 
          else Message("ausgeschaltet", building); 
  switch = !switch;
  return(1);
}

protected func StartWorking()
{
  if (!!SEqual(GetAction(), "waiting")) return(SetAction("startworking",building));
  if (!!SEqual(GetAction(), "endworking")) {
    var ph=GetPhase();
    SetAction("startworking",building);
    SetPhase(8-(ph ?? 0));
    return(1);
  } 
  return(SetAction("working",building));
}

protected func StopWorking()
{
  if (!!SEqual(GetAction(), "working")) return(SetAction("endworking",building));
  if (!!SEqual(GetAction(), "startworking")) {
    var ph=GetPhase();
    SetAction("endworking",building);
    SetPhase(ph);
    return(1);
  }
  return(SetAction("waiting",building));
}

// Brennstoff suchen

private func GetFuel()
{
  var obj = FindContents(WOOD,building);
  if (!obj) obj = FindContents(COAL,building);
  if (!obj) obj = FindContents(CGBF,building);
  if (!obj) obj = FindContents(OBRL,building);

  if (!obj) obj = FindContents(CORP,building);
  if (!obj) return(nil);
  return(obj);
}

// Action-Call

protected func CheckStatus()
{
  if (!switch) return(StopWorking());
  if (HasEMP(building)) return(StopWorking());
  if (!FindObject(ENRG)) {
    if (!SEqual(GetAction(), "working")) StartWorking();
    building->DoSmoke();
    return(1);
  }
  
  if ((GetEnergy(building) ?? 0) >= 80) return(1);
        
  // Action checken und Rauch erzeugen
  if (!!SEqual(GetAction(), "working")) {
    building->DoSmoke();
    S42MigrationStep(burntime, -1, true);
  }
  
  // noch genug Brennstoff ?
  if ((burntime ?? 0) > 100) return(StartWorking());
  
  // ansonsten neuen Brennstoff einfüllen
  var fuel = GetFuel(); 
  if (!fuel) 
    if ((burntime ?? 0) <= 0) { burntime = nil; return(StopWorking()); }
      else return(StartWorking()); 
  
  // Brennstoff verarbeiten
  if (Equal(GetID(fuel), OBRL))
    {
     S42MigrationAssign(burntime, "+=", (LocalN("iFillLevel",fuel) ?? 0)*15);  // max. 3000
     ChangeDef(BARL,fuel);
     }
  if (Equal(GetID(fuel), CGBF))
    {
     S42MigrationAssign(burntime, "+=", 500);
     fuel->Empty(); 
    }
  if (Equal(GetID(fuel), WOOD)) { S42MigrationAssign(burntime, "+=", 300); RemoveObject(fuel); }
  if (Equal(GetID(fuel), COAL)) { S42MigrationAssign(burntime, "+=", 400); RemoveObject(fuel); }
  if (Equal(GetID(fuel), CORP)) { S42MigrationAssign(burntime, "+=", 400); var verbrannt; verbrannt=CreateObject(VBWS); verbrannt->Verbrannt(CORP,fuel->~IsWaste());RemoveObject(fuel); }
  return(1);
}

public func GetRemaining()
{
  return(burntime);
}
