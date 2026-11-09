/*-- Glüheffekt --*/

#strict


local burntime, building, switch;


public func IsWaste(){ return(10);}


protected func Initialize()
{
  switch = 1;
  return(1);
}

public func Init()
{
  building = Par(0);
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
  if (GetAction() eq "waiting") return(SetAction("startworking",building));
  if (GetAction() eq "endworking") {
    var ph=GetPhase();
    SetAction("startworking",building);
    SetPhase(8-ph);
    return(1);
  } 
  return(SetAction("working",building));
}

protected func StopWorking()
{
  if (GetAction() eq "working") return(SetAction("endworking",building));
  if (GetAction() eq "startworking") {
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
  if (!obj) return(0);
  return(obj);
}

// Action-Call

protected func CheckStatus()
{
  if (!switch) return(StopWorking());
  if (HasEMP(building)) return(StopWorking());
  if (!FindObject(ENRG)) {
    if (GetAction() ne "working") StartWorking();
    building->DoSmoke();
    return(1);
  }
  
  if (GetEnergy(building) >= 80) return(1);
        
  // Action checken und Rauch erzeugen
  if (GetAction() eq "working") {
    building->DoSmoke();
    burntime--;
  }
  
  // noch genug Brennstoff ?
  if (burntime > 100) return(StartWorking());
  
  // ansonsten neuen Brennstoff einfüllen
  var fuel = GetFuel(); 
  if (!fuel) 
    if (burntime <= 0) { burntime = 0; return(StopWorking()); }
      else return(StartWorking()); 
  
  // Brennstoff verarbeiten
  if (GetID(fuel) == OBRL)
    {
     burntime += LocalN("iFillLevel",fuel)*15;  // max. 3000 
     ChangeDef(BARL,fuel);
     }
  if (GetID(fuel) == CGBF)
    {
     burntime += 500;
     fuel->Empty(); 
    }
  if (GetID(fuel) == WOOD) { burntime += 300; RemoveObject(fuel); }
  if (GetID(fuel) == COAL) { burntime += 400; RemoveObject(fuel); }  
  if (GetID(fuel) == CORP) { burntime += 400; var verbrannt; verbrannt=CreateObject(VBWS); verbrannt->Verbrannt(CORP,fuel->~IsWaste());RemoveObject(fuel); }  
  return(1);
}

public func GetRemaining()
{
  return(burntime);
}
