/*-- Wipfjagd --*/

#strict 3
#include OREM

// Anzahl Wipfe die Sterben dürfen
local AllowedDeaths;
// Anzahl Toter Wipfe
local DeadWipfs;

func Initialize() {
  
  // 1x WPHT = 0 AllowedDeaths
  AllowedDeaths = nil;
  
  var pGoal;
  if (pGoal = FindObject(WPHT)) 
  {
    // 2x WPHT = 2 AllowedDeaths
    if (Equal(LocalN("AllowedDeaths", pGoal), nil))
      LocalN("AllowedDeaths", pGoal) = 1;
    // 3x WPHT = 3 AllowedDeaths usw.  
    S42MigrationStep(LocalN("AllowedDeaths", pGoal), 1);
    // Nicht genug Wipfe da?
    var i = (LocalN("AllowedDeaths", pGoal) ?? 0) - (ObjectCount(WIPF) ?? 0) + 1 + (Random(3) ?? 0);
    // Noch welche erzeugen
    while ((i ?? 0) > 0)
    {
      PlaceAnimal(WIPF);
      S42MigrationStep(i, -1, true);
    }    
    // Kontrolle an das erste Spielzielobjekt übergeben
    return(RemoveObject());
  }  
       
  return(inherited());
}  
    
public func IsFulfilled() 
{
  //Ob es wohl besser wäre, wenn das Wipf seinen Tod per Scriptfunktion an das Spielziel meldete?
  var pWipf;
  while (pWipf = FindObject(DWPF,nil,nil,nil,nil,OCF_Fullcon,nil,nil,nil,pWipf))
    if (!Local(nil, pWipf))
    {
      SetLocal(nil, 1, pWipf);
      S42MigrationStep(DeadWipfs, 1);
    }

  if ((DeadWipfs ?? 0) > (AllowedDeaths ?? 0))
  {
    var i = GetPlayerCount(); 
    while (S42MigrationStep(i, -1, true))
      EliminatePlayer(GetPlayerByIndex(i));
    return (nil);
  }
  return(!FindObject(WIPF));
}

public func SellID() { return (WIPF); }

protected func Activate(plr) 
{
  if ((DeadWipfs ?? 0)>(AllowedDeaths ?? 0)) return(MessageWindow(Format("$MsgFailed$"),plr));
  if (IsFulfilled()) return(MessageWindow(Format("$MsgSuccess$"),plr));
  MessageWindow(Format("$MsgTodo$",ObjectCount(WIPF),DeadWipfs,AllowedDeaths),plr);
}
