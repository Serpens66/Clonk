/*-- Schockwelle --*/

#strict

public func SetSize(newsize, posX, posY, rad_effect, emp_effect)
{
  var obj;
  // Strahlung freisetzen
  if (rad_effect)
  {
    obj = CreateObject(SRAD,0,25,GetOwner());
    obj->SetRadTime(rad_effect);
    SetPosition(posX, posY, obj);
  }
  // EMP auslösen
  if (emp_effect)
  {
    var emp, dist;
    while (obj = FindObject(0, 0,0,-1,-1, 0,0,0,NoContainer(), obj))
    {
      dist = ObjectDistance(obj);
      if (dist >= emp_effect)
        break;
      // Hilfsobjekt erzeugen
      emp = CreateObject(SEMP);
      emp->SetTarget(obj, (emp_effect-dist)/4);
   }
  }
  // Event-Infos
  var eventinfoID = SE03;
  if (emp_effect)
    if (rad_effect) eventinfoID = SE04;
               else eventinfoID = SE05;
  var player, i;
  for (i; i < GetPlayerCount(); i++)
  {
    player = GetPlayerByIndex(i);
    if (!GetPlayerName(player)) continue;
    CreateObject(eventinfoID,0,0,player);
  }
  // keine Druckwelle?
  if (!newsize) return(RemoveObject());
  // Größe und Position setzen
  SetPosition(posX, posY);
  SetAction("Blast");
  SetCon(newsize);
}

protected func Remove()
{
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 4);
  RemoveObject();
}

private func ScreenGamma()
{
  // kurze grüne Bildschirmfärbung
  SetGamma(RGB(  0, 96-GetPhase()*8,  0),
           RGB(128,176-GetPhase()*4,128),
           RGB(255,255,255), 4);
}

