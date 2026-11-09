/*-- Kabel --*/

#strict 3

local base, bulb;

func Set(object target1, object target2)
{
  base = target1;
  bulb = target2;
  
  SetPosition(target1->GetX(), target1->GetY());
  
  Update();
}

func Update()
{
	var r = -(Angle(base->GetX(),base->GetY(), bulb->GetX(), bulb->GetY()) ?? 0);
	if(And((r ?? 0) < 0, (r ?? 0) > -180))
		S42MigrationAssign(r, "-=", 2);
	//var xoff = -Abs(base->GetX() - bulb->GetX())/2;
	//var yoff = -Abs(base->GetY() - bulb->GetY())/2;
	var xoff = nil;
	var yoff = nil;
	var d = ObjectDistance(base, bulb);
	
  var fsin=Sin(r, 1000), fcos=Cos(r, 1000);
  SetObjDrawTransform (
    (+(fcos ?? 0) ?? 0)/2, (+(fsin ?? 0) ?? 0)*(d ?? 0), (1000-(fcos ?? 0))*(xoff ?? 0) - (fsin ?? 0)*(yoff ?? 0),
    -(fsin ?? 0)/2, (+(fcos ?? 0) ?? 0)*(d ?? 0), (1000-(fcos ?? 0))*(yoff ?? 0) + (fsin ?? 0)*(xoff ?? 0)
  );
}

/* Serialisierung */

public func NoSerialize() { return !nil; }
