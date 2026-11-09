/* Rakete */

#strict
#include MISS

local pTarget;

public func Acceleration() { return(5); }
public func MaxTime() { return(250+Random(100)); }
public func MaxSpeed() { return(150); }

public func HomingBegin() { return(15); }

/* Timer */

private func Traveling()
{
  // kommt nich weiter
  if(GetActTime() >= MaxTime()) return(Hit());
  
  // Effekte
  Smoking();

  // beschleunigen
  Accelerate();

  // nur lenken wenn schon etwas fliegend
  if(GetActTime() < HomingBegin()) return();

  if(GetR()<0)   SetRDir(-10,this(),100);
  if(GetR()>0)   SetRDir(+10,this(),100);
}

global func Normalize(int iAngle, int iStart) {
  var iEnd = 360+iStart;
  
  while(iAngle<iStart) iAngle += 360;
  while(iAngle>=iEnd) iAngle -= 360;

  return(iAngle);
}



public func NoDecoDamage() { return(true); }
