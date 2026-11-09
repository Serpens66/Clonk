/* Rakete */

#strict 3
#include MISS

local pTarget;

public func Acceleration() { return(5); }
public func MaxTime() { return(250+(Random(100) ?? 0)); }
public func MaxSpeed() { return(150); }

public func HomingBegin() { return(15); }

/* Timer */

private func Traveling()
{
  // kommt nich weiter
  if((GetActTime() ?? 0) >= (MaxTime() ?? 0)) return(Hit());
  
  // Effekte
  Smoking();

  // beschleunigen
  Accelerate();

  // nur lenken wenn schon etwas fliegend
  if((GetActTime() ?? 0) < (HomingBegin() ?? 0)) return;

  if((GetR() ?? 0)<0)   SetRDir(-10,this(),100);
  if((GetR() ?? 0)>0)   SetRDir(+10,this(),100);
}

global func Normalize(int iAngle, int iStart) {
  iAngle ??= 0;
  iStart ??= 0;
  var iEnd = 360+(iStart ?? 0);
  
  while((iAngle ?? 0)<(iStart ?? 0)) S42MigrationAssign(iAngle, "+=", 360);
  while((iAngle ?? 0)>=(iEnd ?? 0)) S42MigrationAssign(iAngle, "-=", 360);

  return(iAngle);
}



public func NoDecoDamage() { return(true); }
