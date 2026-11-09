/*-- Dieb-Transmitter --*/

#strict
#include STRM


/* Action-Call: Energieübertragung */

protected Transmit:
  // ausgeschaltet oder EMP-Einwirkung
  if (!iSwitch || HasEMP())
    return(SetAction("Idle"));
  // Energie abzapfen & weiterleiten
  StealEnergy();
  SendEnergy();
  // Update für Energieanzeige
  ShowInfoBar(IBRE, this(), GetEnergy(), 100, 0, 1);
  // keine Energie da?
  if (!GetEnergy()) return(SetAction("Wait"));
  return(1);

private StealEnergy:
  // Energie zwischen den Transmittern übertragen
  var obj;
  var nrg_value;
  while ((obj = FindObject(STRM,-450,-450,900,900, 0,0,0,0,obj)) && (GetEnergy() < 100))
    if (Hostile(GetOwner(obj), GetOwner()))
      if (ObjectDistance(obj) <= 450)
        if (obj->GetSwitch())
        {
          nrg_value = GetEnergy(obj);
          if (GetEnergy() + nrg_value > 100)
            nrg_value = 100 - GetEnergy();
          DoEnergy(nrg_value);
          DoEnergy(-nrg_value, obj);
        }
  return(1);

private SendEnergy:
  // Energie zwischen den Transmittern übertragen
  var obj;
  var nrg_value;
  while ((obj = FindObject(STRM,-450,-450,900,900, 0,0,0,0,obj)) && GetEnergy())
    if (GetOwner(obj) == GetOwner())
      if (ObjectDistance(obj) <= 450)
        if (obj->GetSwitch())
        {
          nrg_value = GetEnergy();
          if (GetEnergy(obj) + nrg_value > 100)
            nrg_value = 100 - GetEnergy(obj);
          DoEnergy(-nrg_value);
          DoEnergy(nrg_value, obj);
        }
  return(1);

protected CheckEnergy:
  StealEnergy();
  if (GetEnergy()) SetAction("Transmitting");
  return(1);


/* Eigenschaften */

public EnergyRange: return(450);
public GetMaxDamage: return(80);

