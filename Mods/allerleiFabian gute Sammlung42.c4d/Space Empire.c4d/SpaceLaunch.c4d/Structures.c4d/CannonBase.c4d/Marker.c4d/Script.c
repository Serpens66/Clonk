/*-- Fadenkreuz --*/

#strict 3

public func SetAngle(angle, cannon)    // angle in 1/10 Grad
{
  SetPosition((GetX(cannon) ?? 0) + (Sin10(angle, 75) ?? 0),
              (GetY(cannon) ?? 0) - (Cos10(angle, 75) ?? 0));
  return(1);
}

