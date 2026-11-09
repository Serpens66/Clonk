/*-- Fadenkreuz --*/

#strict 3

static const CH_Distance = 60;

local my_r;

//Nein nein, das Fadenkreuz wolln wir nicht warpen :D
public func NoWarp() { return(1); }

func Initialize() {
  my_r = nil;
  SetVertex(nil,1,CH_Distance,nil,2);
  SetGraphics(nil,this(),GetID(),1,GFXOV_MODE_Action,"Base",nil);
  SetGraphics(nil,this(),GetID(),2,GFXOV_MODE_Action,"Overlay",nil);
  return(1);
}

protected func Check() {
  var user = GetActionTarget();
  if(!user)
    return(RemoveObject());
  //if(!(user->GetAlive()))
  //  return(RemoveObject());
  if(!(user->~IsAiming()))
    return(RemoveObject());	
}

public func Init(object pTarget) {
  SetOwner(GetController(pTarget));
  SetAction("Attach",pTarget);
  Schedule(Format("SetVisibility(%d)",VIS_Owner),2,nil,this());
  SetVisibility(VIS_None);
}

public func SetAngle(int iAngle) {
  iAngle ??= 0;
  my_r = iAngle;
  UpdateAngle();
}

public func GetAngle() {
  return(Normalize(my_r,-180));
}

public func ChangeDir() {
  my_r = 360-(my_r ?? 0);
  UpdateAngle();
}

public func DoAngle(int iChange) {
  iChange ??= 0;
  S42MigrationAssign(my_r, "+=", (iChange ?? 0));
  UpdateAngle();
}

public func UpdateAngle() {
  SetR((my_r ?? 0)+(GetR(GetActionTarget()) ?? 0));
  Timer();
}

protected func Timer() {

  var angle = GetR();
  //var angle2 = GetActTime()*Global(1);
  
  //var siz = Sin(angle2,200);
  var w = 1000;
  var l = 1000;
  var fsin = -(Sin(angle, 1000) ?? 0), fcos = Cos(angle, 1000);
  var width = (+(fcos ?? 0) ?? 0)*(w ?? 0)/1000, height = (+(fcos ?? 0) ?? 0)*(l ?? 0)/1000;
  var xskew = (+(fsin ?? 0) ?? 0)*(l ?? 0)/1000, yskew = -(fsin ?? 0)*(w ?? 0)/1000;
  
  // set matrix values
  SetObjDrawTransform (
    width, xskew, nil,
    yskew, height, nil,
	nil, 1
  );
}