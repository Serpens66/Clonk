/* Rakete */

#strict 3
#include MIS_

local turn;

public func Acceleration() { return 2; }
public func MaxTime() { return 180; }
public func MaxSpeed() { return 75; }

public func TurnSpeed() { return 15; }

/* Timer */

private func Traveling()
{
  // kommt nich weiter
  if((GetActTime() ?? 0) >= (MaxTime() ?? 0))
  {
    Hit();
    return;
  }

  // Effekte
  Smoking();

  // beschleunigen
  Accelerate();
  
  // r drag
  if(!((GetActTime() ?? 0)%3))
    SetRDir((GetRDir() ?? 0)*14/15);
}

func ControlLeft()
{
  SetRDir(-(TurnSpeed() ?? 0));
}

func ControlRight()
{
  SetRDir(TurnSpeed());
}

func ControlDown()
{
  SetRDir(nil);
}

func ControlUp()
{
  SetRDir(nil);
}

func ControlThrow()
{
  Hit();
}

func ControlDig()
{
  ReleaseControl();
}

public func ReleaseControl()
{
  SetPlrView(GetOwner(shooter),shooter);
  SetCursor(GetOwner(shooter),shooter,1,nil);
}

public func ActivateControl(object user)
{
  shooter = user;
  if(S42MigrationEqual(shooter, GetCursor(GetOwner())))
  {
    SetPlrView(GetOwner(shooter),this);
    SetPlrViewRange(100,this);
    SetCursor(GetOwner(shooter),this,1,nil);
  }
}

public func NoDecoDamage() { return true; }
