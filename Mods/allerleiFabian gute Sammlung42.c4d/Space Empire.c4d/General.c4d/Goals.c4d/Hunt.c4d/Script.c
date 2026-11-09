/*-- Jagd --*/

#strict 3
#include GOAL

public func IsFulfilled()
{
  // jede Gattung von Viechzeug überprüfen
  if (FindObject(BIRD)) return(nil);
  if (FindObject(FMNS)) return(nil);
  if (FindObject(FISH)) return(nil);
  if (FindObject(MONS)) return(nil);
  if (FindObject(SHRK)) return(nil);
  if (FindObject(SNKE)) return(nil);
  if (FindObject(WIPF)) return(nil);
  if (FindObject(BEAR)) return(nil);
  if (FindObject(QFSH)) return(nil);
  if (FindObject(_CHP)) return(nil);
  if (FindObject(BBRD)) return(nil);
  if (FindObject(DRGN)) return(nil);
  if (FindObject(HICP)) return(nil);
  if (FindObject(PIRA)) return(nil);
  if (FindObject(SEAL)) return(nil);
  if (FindObject(SPFL)) return(nil);
  if (FindObject(HORS)) return(nil);
  if (FindObject(_IOZ)) return(nil);
  if (FindObject(ICCR)) return(nil);
  if (FindObject(FSKE)) return(nil);
  if (FindObject(SARN)) return(nil);
  if (FindObject(PHNX)) return(nil);
  if (FindObject(GRFN)) return(nil);
  if (FindObject(BAT1)) return(nil);
  return(1);
}

protected func Activate(player)
{
  // schon fertig
  if (IsFulfilled())
    return(MessageWindow("Mission completed, you are victorious!", player));
  // Nachricht ausgeben, was noch zu tun ist
  return(MessageWindow(Get2DoList(), player));
}

private func Get2DoList()
{
  // Liste aufstellen
  var list = "Creatures left:";
  var i;
  // Monster & Eier
  if (i = ObjectCount(BIRD)) 
               list = Format("Birds Left: %d", i);
  if (i = ObjectCount(FMNS)) 
               list = Format("Fire Mosters Left: %d", i);
  if (i = ObjectCount(FISH)) 
               list = Format("Fishes Left: %d", i);
  if (i = ObjectCount(MONS)) 
               list = Format("Monsters Left: %d", i);
  if (i = ObjectCount(SHRK)) 
               list = Format("Sharks Left: %d", i);
  if (i = ObjectCount(SNKE)) 
               list = Format("Snakes Left: %d", i);
  if (i = ObjectCount(WIPF)) 
               list = Format("Wipfs Left: %d", i);
  if (i = ObjectCount(BEAR)) 
               list = Format("Bears Left: %d", i);
  if (i = ObjectCount(QFSH)) 
               list = Format("Black Fishes Left: %d", i);
  if (i = ObjectCount(_CHP)) 
               list = Format("Chappies Left: %d", i);
  if (i = ObjectCount(BBRD)) 
               list = Format("Death Crowns Left: %d", i);
  if (i = ObjectCount(DRGN)) 
               list = Format("Dragons Left: %d", i);
  if (i = ObjectCount(HICP)) 
               list = Format("Seahorses Left: %d", i);
  if (i = ObjectCount(PIRA)) 
               list = Format("Phiranias Left: %d", i);
  if (i = ObjectCount(SEAL)) 
               list = Format("Seals Left: %d", i);
  if (i = ObjectCount(SPFL)) 
               list = Format("Swamp Flies Left: %d", i);
  if (i = ObjectCount(HORS)) 
               list = Format("Horses Left: %d", i);
  if (i = ObjectCount(_IOZ)) 
               list = Format("Ice Ozarks Left: %d", i);
  if (i = ObjectCount(ICCR)) 
               list = Format("Ice Crowns Left: %d", i);
  if (i = ObjectCount(FSKE)) 
               list = Format("Fire Snakes Left: %d", i);
  if (i = ObjectCount(SARN)) 
               list = Format("Saurians Left: %d", i);
  if (i = ObjectCount(PHNX)) 
               list = Format("Phoenixes Left: %d", i);
  if (i = ObjectCount(GRFN)) 
               list = Format("Griffons Left: %d", i);
  if (i = ObjectCount(BAT1)) 
               list = Format("Bats Left: %d", i);
  return(list);
}