/*-- Sternenhimmel --*/

#strict

local phase;

protected func Initialize()
{
  // erstmal unsichtbar
  SetClrModulation(RGBa(255,255,255,255));
  SetAction("Shine");
  // Phase setzen
  var bool, obj;
  while (!bool)
  {
    phase = Random(8);
    // andere Phase als die umliegenden Sternenhimmelobjekte benutzen
    bool = 1;
    if (obj = FindObject(GetID(), -305,-305, 10,10))   // links oben
      if (obj->GetStarPhase() == phase)
        bool = 0;
    if (obj = FindObject(GetID(), -5,-305, 10,10))     // oben
      if (obj->GetStarPhase() == phase)
        bool = 0;
    if (obj = FindObject(GetID(), -305,-5, 10,10))     // links
      if (obj->GetStarPhase() == phase)
        bool = 0;
    if (obj = FindObject(GetID(), -5,-5, 10,10))       // an dieser Stelle (falls mehr als 1 Schicht Sterne verwendet werden)
      if (obj->GetStarPhase() == phase)
        bool = 0;
  }
  SetPhase(phase);
  // Parallaxität: 20%
  Local(0) = 20;
  Local(1) = 20;
}

public GetStarPhase: return(phase);

public func SetStarPhase(newphase)
{
  phase = newphase;
  return(ResetPhase());
}

private func ResetPhase()
{
  SetPhase(phase);
}

public func SetLightness(value)
{
  // Transparenz neu setzen
  SetClrModulation(RGBa(255,255,255, 255-value));
}

