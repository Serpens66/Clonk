#strict

public func IsWaste(){ return 120;}

local zerfall;

func Initialize()
{
    if(FindObject(NOWS)) {RemoveObject();return();}
    zerfall=0;
}

public func RadioActiv()
{
 // if(!Random(5))
  //  return Radiation2(RandomX(25,50));
++zerfall;

if(!Random(10))
{
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", 0,GetX(),GetY(), Random(100)+50);
}

if(!Random(3)) CastPXS("Acid", 20, 10, 0, 0);

if (zerfall==31)
{    
CreateObject(YECK,0,0,GetOwner());
CreateObject(YECK,0,0,GetOwner());
ChangeDef(ABRL);
    CastPXS("Acid", 20, 10, 0, 0);
    CastPXS("Acid", 20, 10, 0, 0);
    CastPXS("Acid", 20, 10, 0, 0);
    CastPXS("Acid", 20, 10, 0, 0);
    CastPXS("Acid", 20, 10, 0, 0);
}

}

public func Hit()
{
  Sound("ClonkHit*");
if(!Random(2))     CastPXS("Acid", 20, 10, 0, 0);
 // if(!Random(15))
 //   return Radiation2(RandomX(50,100));
}

public func Shredder()
{
  Sound("ClonkHit*");
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", 0,GetX(),GetY(), Random(200)+100);
  Explode(50);
    CastPXS("Acid", 20, 10, 0, 0);
    CastPXS("Acid", 20, 10, 0, 0);
    CastPXS("Acid", 20, 10, 0, 0);
    CastPXS("Acid", 20, 10, 0, 0);
    CastPXS("Acid", 20, 10, 0, 0);
 // return Radiation2(RandomX(200,350));
}
