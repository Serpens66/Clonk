#strict 3

public func IsWaste(){ return 120;}

local zerfall;

func Initialize()
{
    if(FindObject(NOWS)) {RemoveObject();return;}
    zerfall=nil;
}

public func RadioActiv()
{
 // if(!Random(5))
  //  return Radiation2(RandomX(25,50));
S42MigrationStep(zerfall, 1);

if(!Random(10))
{
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", nil,GetX(),GetY(), (Random(100) ?? 0)+50);
}

if(!Random(3)) CastPXS("Acid", 20, 10, nil, nil);

if (Equal(zerfall, 31))
{    
CreateObject(YECK,nil,nil,GetOwner());
CreateObject(YECK,nil,nil,GetOwner());
ChangeDef(ABRL);
    CastPXS("Acid", 20, 10, nil, nil);
    CastPXS("Acid", 20, 10, nil, nil);
    CastPXS("Acid", 20, 10, nil, nil);
    CastPXS("Acid", 20, 10, nil, nil);
    CastPXS("Acid", 20, 10, nil, nil);
}

}

public func Hit()
{
  Sound("ClonkHit*");
if(!Random(2))     CastPXS("Acid", 20, 10, nil, nil);
 // if(!Random(15))
 //   return Radiation2(RandomX(50,100));
}

public func Shredder()
{
  Sound("ClonkHit*");
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", nil,GetX(),GetY(), (Random(200) ?? 0)+100);
  Explode(50);
    CastPXS("Acid", 20, 10, nil, nil);
    CastPXS("Acid", 20, 10, nil, nil);
    CastPXS("Acid", 20, 10, nil, nil);
    CastPXS("Acid", 20, 10, nil, nil);
    CastPXS("Acid", 20, 10, nil, nil);
 // return Radiation2(RandomX(200,350));
}
