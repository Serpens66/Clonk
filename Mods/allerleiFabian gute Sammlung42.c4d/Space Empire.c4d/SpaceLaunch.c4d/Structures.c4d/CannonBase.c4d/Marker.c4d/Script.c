/*-- Fadenkreuz --*/

#strict

public func SetAngle(angle, cannon)    // angle in 1/10 Grad
{
  SetPosition(GetX(cannon) + Sin10(angle, 75),
              GetY(cannon) - Cos10(angle, 75));
  return(1);
}

