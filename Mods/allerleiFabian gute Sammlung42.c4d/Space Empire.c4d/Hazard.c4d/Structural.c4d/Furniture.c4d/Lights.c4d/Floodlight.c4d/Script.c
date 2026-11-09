/*-- Scheinwerfer --*/

#strict 3

local pLight;

protected func Initialize()
{
  pLight = CreateObject(FLHO, nil,nil, -1);
	pLight -> Activate(this());
}

protected func ControlLeft()
{
  pLight->TurnLeft();
}

protected func ControlRight()
{
  pLight->TurnRight();
}

protected func ControlUp()
{
  pLight->Stop();
}

public func ControlDigDouble()
{
  pLight->Switch();
}

// Scriptaufruf

public func SetRotation(int iR, int iOwn)
{
  iR ??= 0;
  iOwn ??= 0;
  iR = BoundBy(iR, -90, 90);
  SetR(iOwn);

  pLight->SetR((iR ?? 0)+(GetR() ?? 0));
  pLight->RotateLight();
}

// Kein Kran darf mich schnappen!
public func NoCrane() { return(1); }
