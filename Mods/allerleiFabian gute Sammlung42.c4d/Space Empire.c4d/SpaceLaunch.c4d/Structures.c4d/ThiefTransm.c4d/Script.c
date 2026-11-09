/*-- Dieb-Transmitter --*/

#strict 3
#include STRM


/* Action-Call: Energieübertragung */

protected func Transmit(...) {
  // ausgeschaltet oder EMP-Einwirkung
  if (Or(!iSwitch, HasEMP()))
    return(SetAction("Idle"));
  // Energie abzapfen & weiterleiten
  StealEnergy();
  SendEnergy();
  // Update für Energieanzeige
  ShowInfoBar(IBRE, this(), GetEnergy(), 100, nil, 1);
  // keine Energie da?
  if (!GetEnergy()) return(SetAction("Wait"));
  return(1);
}

private func StealEnergy(...) {
  // Energie zwischen den Transmittern übertragen
  var obj;
  var nrg_value;
  while (And((obj = FindObject(STRM,-450,-450,900,900, nil,nil,nil,nil,obj)), ((GetEnergy() ?? 0) < 100)))
    if (Hostile(GetOwner(obj), GetOwner()))
      if ((ObjectDistance(obj) ?? 0) <= 450)
        if (obj->GetSwitch())
        {
          nrg_value = GetEnergy(obj);
          if ((GetEnergy() ?? 0) + (nrg_value ?? 0) > 100)
            nrg_value = 100 - (GetEnergy() ?? 0);
          DoEnergy(nrg_value);
          DoEnergy(-(nrg_value ?? 0), obj);
        }
  return(1);
}

private func SendEnergy(...) {
  // Energie zwischen den Transmittern übertragen
  var obj;
  var nrg_value;
  while (And((obj = FindObject(STRM,-450,-450,900,900, nil,nil,nil,nil,obj)), GetEnergy()))
    if (Equal(GetOwner(obj), GetOwner()))
      if ((ObjectDistance(obj) ?? 0) <= 450)
        if (obj->GetSwitch())
        {
          nrg_value = GetEnergy();
          if ((GetEnergy(obj) ?? 0) + (nrg_value ?? 0) > 100)
            nrg_value = 100 - (GetEnergy(obj) ?? 0);
          DoEnergy(-(nrg_value ?? 0));
          DoEnergy(nrg_value, obj);
        }
  return(1);
}

protected func CheckEnergy(...) {
  StealEnergy();
  if (GetEnergy()) SetAction("Transmitting");
  return(1);
}


/* Eigenschaften */

public func EnergyRange(...) { return(450);
}
public func GetMaxDamage(...) { return(80);
}

