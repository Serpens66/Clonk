/*-- Status-Balken --*/

#strict

local size, timer, alpha;


/* Globale Funktionen zum Aufruf */

global func ShowInfoBar(ID, target, fill, maxfill, timer)
{
  // Anzeige aus?
  if (GetPlrExtraData(GetOwner(target), "SE_infobars") == 2)
    return(1);
  // noch ein Balken da?
  if (!target)
    target = this();
  var bar = FindObject(ID, 0,0,0,0, 0,0,target);
  // neuen Balken erstellen
  if (!bar)
  {
    if (!timer) return(1);   // nicht, wenn timer=0
    bar = CreateObject(ID, 0,0, GetOwner(target));
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
  var bar = FindObject(ID, 0,0,0,0, 0,0,target);
  // Balken ausblenden
  if (!bar) return(0);
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
  if (obj = FindObject(IBRS, -1,-33,3,27, 0,0,GetActionTarget()))  SetDown(obj);
  if (obj = FindObject(IBRE, -1,-33,3,27, 0,0,GetActionTarget()))  SetDown(obj);
  if (obj = FindObject(IBRP, -1,-33,3,27, 0,0,GetActionTarget()))  SetDown(obj);
}

private func SetDown(obj)
{
  var dy = 8;
  if (UseTextInfo()) dy = 16;
  SetVertexXY(0, GetVertex(0,0,obj), GetVertex(0,1,obj)-dy, obj);
  SetPosition(GetX(obj), GetY(obj)+dy, obj);
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
  SetVisibility(VIS_Owner() | VIS_Allies());
  size = target->~InfoBarSize();
  if (!Inside(size, 1, 5))
    size = 3;
  // Vertex setzen
  var vx = GetVertex(0,0,target);
  var vy = GetVertex(0,1,target) + GetObjHeight(target)/2 + target->~InfoBarPos();
  if (UseTextInfo())
  {
    SetVertexXY(0, vx, vy-6);
    SetPosition(GetX(target) + GetVertex(0,0,target) - vx,
                GetY(target) + GetVertex(0,1,target) - vy + 6);
    // Action & Zielobjekt
    SetAction("ShowText", target);
    CheckPosition(target, 16);
    return(1);
  }
  SetVertexXY(0, vx, vy+8);
  SetPosition(GetX(target) + GetVertex(0,0,target) - vx,
              GetY(target) + GetVertex(0,1,target) - vy - 8);
  // Action & Zielobjekt
  SetAction("Appear", target);
  CheckPosition(target, 8);
}

public func Settings(fill, maxfill, timervalue)
{
  // Wert korrigieren
  if (fill < 0)       fill = 0;
  if (fill > maxfill) fill = maxfill;
  // Anzeigewert
  var start, steps;
  if (size==5)  { start = 00; steps = 24; }
  if (size==4)  { start = 25; steps = 20; }
  if (size==3)  { start = 46; steps = 16; }
  if (size==2)  { start = 63; steps = 12; }
  if (size==1)  { start = 76; steps = 08; }
  if (!maxfill) SetDir(start);
           else SetDir(start + steps - fill*steps/maxfill);
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
  if (timervalue == -1) timer = 1000000000;   // lange warten...
                   else timer = timervalue;
  // Textanzeige zusätzlich länger
  if (text != -1)
    return(timer += 2);
  // wieder einblenden, wenn nötig
  if (GetAction() ne "Disappear") return(1);
  SetAction("Appear");
}

public func Disappear()
{
  if (GetAction() eq "Disappear")
    return(0);
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
  alpha = Max(alpha-51, 0);
  SetClrModulation(RGBa(255,255,255,alpha));
  if (!alpha) SetAction("ShowBar");
}

private func DisappearStep()
{
  // verschwinden lassen
  alpha = Min(alpha+51, 255);
  if (alpha == 255) return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,alpha));
}


/* Action-Calls */

private func CheckTimer()   // für grafische Anzeige
{
  // keine Zeitsteuerung?
  if (!timer) return(1);
  // herunterzählen
  if (--timer) return(1);
  // Zeit abgelaufen
  SetAction("Disappear");
  return(DisappearStep());
}

private func CheckTimer2()   // für Textanzeige
{
  // keine Zeitsteuerung?
  if (!timer) return(1);
  // herunterzählen
  if (--timer) return(1);
  return(RemoveObject());
}


/* Timer-Call */

protected func CheckAction()
{
  if (ActIdle() || !GetActionTarget())
    return(RemoveObject());
}


/* Anordnung */

public func CheckPosition(target, dy)
{
  if (FindLowerBar(target))
  {
    // sich selbst eine Position hoch
    SetVertexXY(0, GetVertex(0,0), GetVertex(0,1)+dy);
    SetPosition(GetX(), GetY()-dy);
    // neue Position überprüfen
    return(CheckPosition(target, dy));
  }
  var obj = FindHigherBar(target);
  if (obj)
    // anderer Balken soll mal seine Position prüfen
    obj->CheckPosition(target, dy);
}

private FindLowerBar:
  var obj = FindObject(IBRE, -1,-1,3,3, 0,0,Par(0));
  if (!obj)
      obj = FindObject(IBRP, -1,-1,3,3, 0,0,Par(0));
  return(obj);

private FindHigherBar:
  return(0);


/* Textausgabe */

private func StdInfoBarText(v1, v2)
{
  return(Format("@Status:  %d / %d HP", v1, v2));
}

