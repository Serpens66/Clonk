/*-- RepairBot --*/

#strict

private FlyProcess:
  // Gebäude zum Reparieren da?
  var target;
  while (target = FindObject(0, 0,1,0,0, 0,0,0, NoContainer(), target))
    if (DefinitionCall(GetID(target),"Repairable") && GetDamage(target))
    {
      // Objekt reparieren
      SetAction("Repair");
      DoDamage(-1,target);
      Sound("Repair");
      // Funken versprühen
      if (Random(2))
        CastParticles("MSpark", 2+Random(4),20, 0,0, 20,30, RGBa(128,192,255,64), RGBa(255,255,255,64));
    }
  return(1);

protected Activity:
  // nix machen
  if (!Random(2)) return(1);
  // verschüttet: sterben
  if (GBackSolid()) return(Death());
  // einfach so sterben
  if (!Random(30)) return(Death());
  // Richtung ändern
  if (!Random(2)) SetComDir(Random(9)+1);
  // Verfolgung aufgeben
  if (!Random(4)) SetCommand(this(),"None");
  // neues Ziel verfolgen
  if (!Random(5))
  {
    var obj = GetObject2Repair();
    if (obj) SetCommand(this(),"Follow",obj);
  }
  return(1);
  
private GetObject2Repair:
  var obj;
  while (obj = FindObject(0, 0,0,-1,-1, 0,0,0, NoContainer(),obj))
    if (!InLiquid(obj))
      if (GetDamage(obj))
        if (DefinitionCall(GetID(obj),"Repairable"))
          return(obj);
  return(0);

protected Initialize:
  SetAction("Fly");
  SetComDir(Random(9)+1);
  return(1);

protected Damage: return(RemoveObject());
protected Death:  return(RemoveObject());

