/*-- Schockwelle --*/

#strict 3

public func SetSize(newsize, posX, posY, rad_effect, emp_effect)
{
  var obj;
  // Strahlung freisetzen
  if (rad_effect)
  {
    obj = CreateObject(SRAD,nil,25,GetOwner());
    obj->SetRadTime(rad_effect);
    SetPosition(posX, posY, obj);
  }
  // EMP auslösen
  if (emp_effect)
  {
    var emp, dist;
    while (obj = FindObject(nil, nil,nil,-1,-1, nil,nil,nil,NoContainer(), obj))
    {
      dist = ObjectDistance(obj);
      if ((dist ?? 0) >= (emp_effect ?? 0))
        break;
      // Hilfsobjekt erzeugen
      emp = CreateObject(SEMP);
      emp->SetTarget(obj, ((emp_effect ?? 0)-(dist ?? 0))/4);
   }
  }
  // Event-Infos
  var eventinfoID = SE03;
  if (emp_effect)
    if (rad_effect) eventinfoID = SE04;
               else eventinfoID = SE05;
  var player, i;
  for (i; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
  {
    player = GetPlayerByIndex(i);
    if (!GetPlayerName(player)) continue;
    CreateObject(eventinfoID,nil,nil,player);
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
  SetGamma(RGB(nil,nil,nil), RGB(128,128,128), RGB(255,255,255), 4);
  RemoveObject();
}

private func ScreenGamma()
{
  // kurze grüne Bildschirmfärbung
  SetGamma(RGB(  nil, 96-(GetPhase() ?? 0)*8,  nil),
           RGB(128,176-(GetPhase() ?? 0)*4,128),
           RGB(255,255,255), 4);
}

