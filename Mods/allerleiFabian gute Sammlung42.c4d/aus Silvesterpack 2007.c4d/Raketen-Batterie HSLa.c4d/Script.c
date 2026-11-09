/*-- Batterie --*/

#strict
local Fuse,R,G,B,Farbe,Finshed,Timing;

protected func Initialize() {

  return(1);
}
protected func Activate() {
SetPlrViewRange(500);
  SetAction("Fused");
  Sound("Fuse");
  if(GetDir(Contained())==DIR_Left())
    Exit(this(),-12);
  if(GetDir(Contained())==DIR_Right())
    Exit(this(),12);
  return(1);
}

protected func Fusing() {
  ++Fuse;
  CastObjects(SPRK,2,8,2);
  if(Fuse>=3){
    SetAction("Started");
    }
  return(1);
}

protected func Hit() {
Sound("MetalHit*");
  return(1);
}

protected func Hit3() {
 ++Fuse;
   CastObjects(SPRK,2,25);
 if(Fuse>=3){
    SetAction("Kaputt");
    }
  return(1);
}

protected func Blasting() {
   ObjectCall(CreateObject(HSL3,RandomX(-1),2,0),"StartIt",R,G,B);
  ObjectCall(CreateObject(HSL3,RandomX(-1),2,0),"StartIt",R,G,B);
     ObjectCall(CreateObject(HSL1,RandomX(-1),2,0),"StartIt");
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,0),"StartIt");
    ObjectCall(CreateObject(HSL1,RandomX(-1),2,0),"StartIt");
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,0),"StartIt");
    ObjectCall(CreateObject(HSL1,RandomX(-1),2,0),"StartIt");
  CastObjects(HSL2,8,15);
  CastObjects(HSL2,8,20);
  CastObjects(HSL2,8,25);
  CastObjects(HSL2,8,30);
  return(RemoveObject(this()));
}

protected func Fireing() {
  Sound("Firework");
  ++Timing;
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,0),"StartIt");
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,0),"StartIt");
 
  CreateParticle("VMSpark",0,-3,RandomX(-20,20),RandomX(-30,-20),RandomX(40,60),RGBa(255,191,0,Random(50)));
  CreateParticle("VMSpark",0,-3,RandomX(-30,30),RandomX(-30,-20),RandomX(40,60),RGBa(255,97,0,Random(50)));
  CreateParticle("VMSpark",0,-3,RandomX(-20,20),RandomX(-30,-20),RandomX(40,60),RGBa(255,159,0,Random(50)));
  
  if(Timing>=23){
  ObjectCall(CreateObject(HSL3,RandomX(-1),2,0),"StartIt",R,G,B);
  }
  
  if(Timing>=25){
    Finish();
  }
 return(1);
}

protected func Finish() {
  Farbe=RandomX(3);
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,0),"StartIt",R,G,B);
  Farbe=RandomX(-3);
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,0),"StartIt",R,G,B);
  Farbe=RandomX(3);
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,0),"StartIt",R,G,B);
  Farbe=RandomX(-3);
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,0),"StartIt",R,G,B);
  Farbe=RandomX(3);
    SetAction("Idle");
Schedule("SetPlrViewRange(10);", 300);
Schedule("ChangeDef(SO_6);", 300);

  return(1);}

public func IsChemicalProduct() { return(1); }
public func IsAnvilProduct() { return(1); }
