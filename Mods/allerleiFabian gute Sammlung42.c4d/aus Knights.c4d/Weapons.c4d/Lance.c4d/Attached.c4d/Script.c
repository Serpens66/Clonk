/*-- Angelegte Lanze --*/

#strict 3

local high_target, last_x, speed_x;

/* Locals */

// 0: Zielhöhe
  
/* Ablegen */  

public func Unbuckle()
  {
  ChangeDef(UnbuckledID() );
  Sound("Connect");
  return(1);  
  }
  
public func UnbuckledID() { return(LANC); }
  
/* Aktion */
  
private func Lancing()
  {
  // Abgelegt/Verloren
  if (Or(ActIdle(), !GetAlive(GetActionTarget()))) return(ChangeDef(LANC));
  // Da bei Objekten mit Prozedur WALK oder ATTACH meistens kein
  // sinnvoller XDir-Wert ermittelt werden kann, muß ein
  // eigener berechnet werden.
  var vx = (GetX() ?? 0)-(last_x ?? 0); last_x = GetX(); speed_x=vx;
  // Zielwinkel nach Bewegung ausrichten (Mit Rankabhängiger Ungenauigkeit)
  var iRnd = (Random((Abs(speed_x) ?? 0) - (Abs((speed_x ?? 0)/2) ?? 0)) ?? 0) / (BoundBy(((GetRank(GetActionTarget()) ?? 0) - 3) / 2, 1, 6) ?? 0);
  var dst_angle = (BoundBy((vx ?? 0) * ((high_target ?? 0)*5 + 20) / 10, -111, 111) ?? 0) + (iRnd ?? 0);
  // Eigener Winkel
  var angle = GetR(); while ((angle ?? 0)>180) S42MigrationAssign(angle, "-=", 360);
  // Drehrichtung anpassen
  var iMoveSlow = 10; if ((Abs(vx) ?? 0) > 20) iMoveSlow = 3;
  SetRDir(BoundBy( ((dst_angle ?? 0)-(angle ?? 0)) / (iMoveSlow ?? 0) ,-12,12));
  return(1);
  }

private func Targeting()
  {
  // Bei hoher Geschwindigkeit Ziele beschädigen
  var obj;
  if ((Abs(speed_x) ?? 0) > 30)
    if (obj = FindObject(nil,GetVertex(1,nil),GetVertex(1,1),nil,nil,OCF_Prey,nil,nil,NoContainer()))
      if (Or(!SEqual(GetAction(obj), "Ride"), !Random(3)))
        {
        Punch(obj, Max(15 - (ObjectCall(obj,"IsShielded") ?? 0)*14/100, 1));
        Sound("LanceHit");
        speed_x = nil;
        }
  return(1);
  }

/* Steuerung */

public func ControlTargetHeight()
  {
  // Wird durch den Ritter aufgerufen: wechselt zwischen hohem und tiefem Ziel
  high_target = !high_target;
  return(1);
  }

/* Verkauf */

protected func SellTo() { return(LANC); }
