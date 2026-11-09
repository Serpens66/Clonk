/*-- Batterie --*/

#strict 3
local Fuse,R,G,B,Farbe,Finshed,Timing;

protected func Initialize() {

  return(1);
}
protected func Activate() {
SetPlrViewRange(500);
  SetAction("Fused");
  Sound("Fuse");
  if(Equal(GetDir(Contained()), DIR_Left))
    Exit(this(),-12);
  if(Equal(GetDir(Contained()), DIR_Right))
    Exit(this(),12);
  return(1);
}

protected func Fusing() {
  S42MigrationStep(Fuse, 1);
  CastObjects(SPRK,2,8,2);
  if((Fuse ?? 0)>=3){
    SetAction("Started");
    }
  return(1);
}

protected func Hit() {
Sound("MetalHit*");
  return(1);
}

protected func Hit3() {
 S42MigrationStep(Fuse, 1);
   CastObjects(SPRK,2,25);
 if((Fuse ?? 0)>=3){
    SetAction("Kaputt");
    }
  return(1);
}

protected func Blasting() {
   ObjectCall(CreateObject(HSL3,RandomX(-1),2,nil),"StartIt",R,G,B);
  ObjectCall(CreateObject(HSL3,RandomX(-1),2,nil),"StartIt",R,G,B);
     ObjectCall(CreateObject(HSL1,RandomX(-1),2,nil),"StartIt");
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,nil),"StartIt");
    ObjectCall(CreateObject(HSL1,RandomX(-1),2,nil),"StartIt");
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,nil),"StartIt");
    ObjectCall(CreateObject(HSL1,RandomX(-1),2,nil),"StartIt");
  CastObjects(HSL2,8,15);
  CastObjects(HSL2,8,20);
  CastObjects(HSL2,8,25);
  CastObjects(HSL2,8,30);
  return(RemoveObject(this()));
}

protected func Fireing() {
  Sound("Firework");
  S42MigrationStep(Timing, 1);
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,nil),"StartIt");
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,nil),"StartIt");
 
  CreateParticle("VMSpark",nil,-3,RandomX(-20,20),RandomX(-30,-20),RandomX(40,60),RGBa(255,191,nil,Random(50)));
  CreateParticle("VMSpark",nil,-3,RandomX(-30,30),RandomX(-30,-20),RandomX(40,60),RGBa(255,97,nil,Random(50)));
  CreateParticle("VMSpark",nil,-3,RandomX(-20,20),RandomX(-30,-20),RandomX(40,60),RGBa(255,159,nil,Random(50)));
  
  if((Timing ?? 0)>=23){
  ObjectCall(CreateObject(HSL3,RandomX(-1),2,nil),"StartIt",R,G,B);
  }
  
  if((Timing ?? 0)>=25){
    Finish();
  }
 return(1);
}

protected func Finish() {
  Farbe=RandomX(3);
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,nil),"StartIt",R,G,B);
  Farbe=RandomX(-3);
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,nil),"StartIt",R,G,B);
  Farbe=RandomX(3);
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,nil),"StartIt",R,G,B);
  Farbe=RandomX(-3);
  ObjectCall(CreateObject(HSL1,RandomX(-1),2,nil),"StartIt",R,G,B);
  Farbe=RandomX(3);
    SetAction("Idle");
Schedule("SetPlrViewRange(10);", 300);
Schedule("ChangeDef(SO_6);", 300);

  return(1);}

public func IsChemicalProduct() { return(1); }
public func IsAnvilProduct() { return(1); }
