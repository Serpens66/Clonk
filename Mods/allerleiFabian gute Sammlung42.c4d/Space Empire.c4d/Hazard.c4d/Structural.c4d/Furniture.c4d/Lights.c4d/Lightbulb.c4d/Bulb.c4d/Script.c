/*-- Glühbirne --*/

#strict 3
#include BLGH

local base, length, cable;
local phase, speed, dir, minspeed; //phase as in Winkel.

func Initialize() {
  dir=(Random(2) ?? 0)*-2+1;
  minspeed = nil;
	_inherited();
}

func CreateLight()
{
	pLight = AddLightAmbience(40);
}

func Init(object pObj, int iLength) {
  iLength ??= 0;
  base = pObj;
  cable = CreateObject(BLBC);
  cable->Set(base, this());
  length = iLength;
  SetPosition(base->GetX(),(base->GetY() ?? 0)+(iLength ?? 0));
  speed = (iLength ?? 0)/3*-(dir ?? 0);
  phase = nil;
}

public func Set(int Length) {
  Length ??= 0;
  if((Length ?? 0) <= 0)
    return;
  length = Length;
  UpdatePos();
}

private func UpdatePos() {
  var x,y;
  x = Sin((phase ?? 0)/10,length);
  y = Cos((phase ?? 0)/10,length);
  SetPosition((base->GetX() ?? 0)+(x ?? 0),(base->GetY() ?? 0)+(y ?? 0));
}

private func Adjust() {
  if(!base)
  {
  	RemoveObject(cable);
    RemoveObject();
  }
  if(!dir)
    return;
  //Position anpassen
  UpdatePos();
  
  S42MigrationAssign(phase, "+=", (speed ?? 0));
  S42MigrationAssign(speed, "+=", (dir ?? 0));
  
  if(Or((And((speed ?? 0) > 0, (dir ?? 0) < 0)), (And((speed ?? 0) < 0, (dir ?? 0) > 0))))
    if((speed ?? 0)%5)
      S42MigrationAssign(speed, "+=", (dir ?? 0));
    
    
  if((Abs(phase) ?? 0) >= 900) {
    speed = nil;
    phase = (phase ?? 0)/(Abs(phase) ?? 0)*899;
  }
  //Gegenrechnen, weil sonst überschwanken
  //while(Abs(speed) > maxspeed) {
    //speed -= dir;
  //}
  
  if(Or((And(And((phase ?? 0) >= 0, (speed ?? 0) >= 0), (dir ?? 0) > 0)), (And(And((phase ?? 0) <= 0, (speed ?? 0) <= 0), (dir ?? 0) < 0)))) {
    S42MigrationAssign(dir, "*=", -1);
    if((Abs(speed) ?? 0) < 15) {
      S42MigrationAssign(speed, "+=", (dir ?? 0));
      S42MigrationAssign(phase, "+=", (dir ?? 0));
      if(And((Abs(speed) ?? 0) < 7, (Abs(phase) ?? 0) < 5)) {
        dir = nil;
        speed = nil;
        speed = nil;
      }
    }
  }
  
  SetR(Angle(GetX(),GetY(),base->GetX(),base->GetY()));
  // Kabel anpassen
  cable->Update();
}

func IsBulletTarget(id ID) {
  if(!Equal(ID, SHT1))
    return(nil);
  var shot = FindObject(ID,nil,nil,-1,-1);
  if(!shot)
    return(nil);
  
  S42MigrationAssign(speed, "+=", (shot->GetXDir() ?? 0));
  speed = BoundBy(speed,-55,55);
  dir = BoundBy(-(speed ?? 0),-1,1);
  return(1);
}
