/*-- Status-Balken --*/

#strict 3

local size, timer, alpha;


/* Globale Funktionen zum Aufruf */

global func ShowInfoBar(ID, target, fill, maxfill, timer)
{
  // Anzeige aus?
  if (Equal(GetPlrExtraData(GetOwner(target), "SE_infobars"), 2))
    return(1);
  // noch ein Balken da?
  if (!target)
    target = this();
  var bar = FindObject(ID, nil,nil,nil,nil, nil,nil,target);
  // neuen Balken erstellen
  if (!bar)
  {
    if (!timer) return(1);   // nicht, wenn timer=0
    bar = CreateObject(ID, nil,nil, GetOwner(target));
    bar->Init(target);
  }
  // Balken-Einstellungen
  bar->Settings(fill, maxfill, timer);
}

global func DisappearInfoBar(ID, target)
{
  // Info-Balken finden
  if (!target)
    target = this();
  var bar = FindObject(ID, nil,nil,nil,nil, nil,nil,target);
  // Balken ausblenden
  if (!bar) return(nil);
  bar->Disappear();
  return(1);
}


/* Initialisierung */

protected func Initialize()
{
  // anfangs ausgeblendet
  SetClrModulation(RGBa(255,255,255,255));
  // einblenden
  alpha = 255;
}

protected func Destruction()
{
  var obj;
  if (obj = FindObject(IBRS, -1,-33,3,27, nil,nil,GetActionTarget()))  SetDown(obj);
  if (obj = FindObject(IBRE, -1,-33,3,27, nil,nil,GetActionTarget()))  SetDown(obj);
  if (obj = FindObject(IBRP, -1,-33,3,27, nil,nil,GetActionTarget()))  SetDown(obj);
}

private func SetDown(obj)
{
  var dy = 8;
  if (UseTextInfo()) dy = 16;
  SetVertexXY(nil, GetVertex(nil,nil,obj), (GetVertex(nil,1,obj) ?? 0)-(dy ?? 0), obj);
  SetPosition(GetX(obj), (GetY(obj) ?? 0)+(dy ?? 0), obj);
}


/* Zielobjekt zerstört */

protected func AttachTargetLost()
{
  return(RemoveObject());
}


/* Objekt-Calls */

public func Init(target)
{
  // unsichtbar für gegnerische Spieler
  SetVisibility((VIS_Owner ?? 0) | (VIS_Allies ?? 0));
  size = target->~InfoBarSize();
  if (!Inside(size, 1, 5))
    size = 3;
  // Vertex setzen
  var vx = GetVertex(nil,nil,target);
  var vy = (GetVertex(nil,1,target) ?? 0) + (GetObjHeight(target) ?? 0)/2 + (target->~InfoBarPos() ?? 0);
  if (UseTextInfo())
  {
    SetVertexXY(nil, vx, (vy ?? 0)-6);
    SetPosition((GetX(target) ?? 0) + (GetVertex(nil,nil,target) ?? 0) - (vx ?? 0),
                (GetY(target) ?? 0) + (GetVertex(nil,1,target) ?? 0) - (vy ?? 0) + 6);
    // Action & Zielobjekt
    SetAction("ShowText", target);
    CheckPosition(target, 16);
    return(1);
  }
  SetVertexXY(nil, vx, (vy ?? 0)+8);
  SetPosition((GetX(target) ?? 0) + (GetVertex(nil,nil,target) ?? 0) - (vx ?? 0),
              (GetY(target) ?? 0) + (GetVertex(nil,1,target) ?? 0) - (vy ?? 0) - 8);
  // Action & Zielobjekt
  SetAction("Appear", target);
  CheckPosition(target, 8);
}

public func Settings(fill, maxfill, timervalue)
{
  // Wert korrigieren
  if ((fill ?? 0) < 0)       fill = nil;
  if ((fill ?? 0) > (maxfill ?? 0)) fill = maxfill;
  // Anzeigewert
  var start, steps;
  if (Equal(size, 5))  { start = 00; steps = 24; }
  if (Equal(size, 4))  { start = 25; steps = 20; }
  if (Equal(size, 3))  { start = 46; steps = 16; }
  if (Equal(size, 2))  { start = 63; steps = 12; }
  if (Equal(size, 1))  { start = 76; steps = 08; }
  if (!maxfill) SetDir(start);
           else SetDir((start ?? 0) + (steps ?? 0) - (fill ?? 0)*(steps ?? 0)/(maxfill ?? 0));
  // Textmeldung neu
  if (UseTextInfo())
  {
    var text = GetActionTarget()->~InfoBarText(GetID(), fill, maxfill);
    if (!text)
      text = StdInfoBarText(fill, maxfill);
    Message(text, this());
  }
  // Anzeigedauer
  if (!timervalue) return(1);
  if (Equal(timervalue, -1)) timer = 1000000000;   // lange warten...
                   else timer = timervalue;
  // Textanzeige zusätzlich länger
  if (!Equal(text, -1))
    return(S42MigrationAssign(timer, "+=", 2));
  // wieder einblenden, wenn nötig
  if (!SEqual(GetAction(), "Disappear")) return(1);
  SetAction("Appear");
}

public func Disappear()
{
  if (!!SEqual(GetAction(), "Disappear"))
    return(nil);
  SetAction("Disappear");
  return(DisappearStep());
}

private func UseTextInfo()
{
  return(GetPlrExtraData(GetOwner(), "SE_infobars"));
}


/* Ein- und Ausblenden */

private func AppearStep()
{
  // auftauchen lassen
  alpha = Max((alpha ?? 0)-51, nil);
  SetClrModulation(RGBa(255,255,255,alpha));
  if (!alpha) SetAction("ShowBar");
}

private func DisappearStep()
{
  // verschwinden lassen
  alpha = Min((alpha ?? 0)+51, 255);
  if (Equal(alpha, 255)) return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,alpha));
}


/* Action-Calls */

private func CheckTimer()   // für grafische Anzeige
{
  // keine Zeitsteuerung?
  if (!timer) return(1);
  // herunterzählen
  if (S42MigrationStep(timer, -1)) return(1);
  // Zeit abgelaufen
  SetAction("Disappear");
  return(DisappearStep());
}

private func CheckTimer2()   // für Textanzeige
{
  // keine Zeitsteuerung?
  if (!timer) return(1);
  // herunterzählen
  if (S42MigrationStep(timer, -1)) return(1);
  return(RemoveObject());
}


/* Timer-Call */

protected func CheckAction()
{
  if (Or(ActIdle(), !GetActionTarget()))
    return(RemoveObject());
}


/* Anordnung */

public func CheckPosition(target, dy)
{
  if (FindLowerBar(target))
  {
    // sich selbst eine Position hoch
    SetVertexXY(nil, GetVertex(nil,nil), (GetVertex(nil,1) ?? 0)+(dy ?? 0));
    SetPosition(GetX(), (GetY() ?? 0)-(dy ?? 0));
    // neue Position überprüfen
    return(CheckPosition(target, dy));
  }
  var obj = FindHigherBar(target);
  if (obj)
    // anderer Balken soll mal seine Position prüfen
    obj->CheckPosition(target, dy);
}

private func FindLowerBar(...) {
  var obj = FindObject(IBRE, -1,-1,3,3, nil,nil,Par(nil));
  if (!obj)
      obj = FindObject(IBRP, -1,-1,3,3, nil,nil,Par(nil));
  return(obj);
}

private func FindHigherBar(...) {
  return(nil);
}


/* Textausgabe */

private func StdInfoBarText(v1, v2)
{
  return(Format("@Status:  %d / %d HP", v1, v2));
}

