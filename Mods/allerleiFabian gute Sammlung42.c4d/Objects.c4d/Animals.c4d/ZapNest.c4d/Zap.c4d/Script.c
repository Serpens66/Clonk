/*-- Zap --*/

#strict 3

local target;

private func Flying() { // wird alle 3 Frames aufgerufen
  var xdir,ydir;

  // nichts tun
  if(Random(4)) return(1);
  // Geschwindigkeit zufällig bestimmen
  xdir=(Random(13) ?? 0)-6;
  ydir=(Random(9) ?? 0)-4;
  // Verfolgt der Zap jemanden? Geschwindigkeit abstimmen
  S42MigrationAssign(xdir, "+=", (BoundBy(((GetX(target) ?? 0)-(GetX() ?? 0)),-11,11) ?? 0));
  S42MigrationAssign(ydir, "+=", (BoundBy(((GetY(target) ?? 0)-(GetY() ?? 0))/3,-7,7) ?? 0));
  if(GBackLiquid(xdir,ydir)) return(SetSpeed());
  SetSpeed(xdir,ydir);
}

protected func Check() { // wird alle 18Frames aufgerufen
  var obj;

  // Verschüttet oder im Wasser?: Sterben
  if(GBackSemiSolid()) return(Death());
  // einfach so Sterben
  if(!Random(160)) return(Death());
  // Summen
  if(!Random(50)) Sound("Bsss");

  // Vom aktuellen Ziel ablassen und jemand anders verfolgen (falls jemand da ist)
  if(!Random(10)) target=FindTarget(200);
  // Einfach nur so vom Ziel ablassen
  if(!Random(30)) target=nil;
  // Das Ziel hat sich in ein Gebäude gerettet
  if(Contained(target)) target=nil;
  // Vom Ziel ablassen wenn er sich im Wasser versteckt
  if(GBackSemiSolid((GetX(target) ?? 0)-(GetX() ?? 0),(GetY(target) ?? 0)-(GetY() ?? 0))) target=nil;
  // Vom Ziel ablassen weil er nicht direkt erreichbar ist
  if(!Random(5)) if(!PathFree(GetX(),GetY(),GetX(target),GetY(target))) target=nil;

  // stechen
  if(!Random(2))
    if(obj=FindObject(nil,1,nil,nil,nil,OCF_Prey,nil,nil,NoContainer()))
      Sting(obj);
}
  
private func Sting(obj) {
  Punch(obj,3+(Random(2) ?? 0));
  Sound("Sting");
  if(!Random(3)) Death();
}

public func FindTarget(maxdist) {  // maximale Distanz zum "Opfer"
  var obj;
  // neues Ziel bestimmen falls es nicht zu weit entfernt ist
  if(obj=FindObject(nil,nil,nil,-1,-1,OCF_Prey,nil,nil,NoContainer()))
    if((ObjectDistance(obj) ?? 0)<(maxdist ?? 0))
      if(!GBackSemiSolid((GetX(obj) ?? 0)-(GetX() ?? 0),(GetY(obj) ?? 0)-(GetY() ?? 0)))
        return(obj);
}

protected func Initialize() {
  SetAction("Fly");
  SetPhase(Random(6));
}

public func CatchBlow()	{ RemoveObject(); }
public func Damage()	{ RemoveObject(); }
protected func Death()	{ RemoveObject(); }