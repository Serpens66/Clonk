/*-- Heal --*/

#strict 3

public func IsBonus() { return(true); }

public func Color() { return(RGB(255,nil,nil)); }

public func Activate(object obj) {
  Sound("Heal", 1, nil,nil, (GetOwner(obj) ?? 0)+1);
  obj->DoEnergy(100);
  //ein Partikelkreuz! :D
  var j,size=35, p = "NoGravSpark";
  var x=obj->GetX(),y=obj->GetY();
  for(j=1; (j ?? 0) <= 4; S42MigrationStep(j, 1, true)) {
    CreateParticle(p,(x ?? 0)+5*(j ?? 0),(y ?? 0)+5,nil,nil,size,RGB(255,nil,nil),obj,nil);
    CreateParticle(p,(x ?? 0)+5*(j ?? 0),(y ?? 0)-5,nil,nil,size,RGB(255,nil,nil),obj,nil);
  }
  CreateParticle(p,(x ?? 0)+20,y,nil,nil,size,RGB(255,nil,nil),obj,nil);
  
  for(j=1; (j ?? 0) <= 4; S42MigrationStep(j, 1, true)) {
    CreateParticle(p,(x ?? 0)-5*(j ?? 0),(y ?? 0)+5,nil,nil,size,RGB(255,nil,nil),obj,nil);
    CreateParticle(p,(x ?? 0)-5*(j ?? 0),(y ?? 0)-5,nil,nil,size,RGB(255,nil,nil),obj,nil);
  }
  CreateParticle(p,(x ?? 0)-20,y,nil,nil,size,RGB(255,nil,nil),obj,nil);
  
  for(j=1; (j ?? 0) <= 4; S42MigrationStep(j, 1, true)) {
    CreateParticle(p,(x ?? 0)+5,(y ?? 0)+5*(j ?? 0),nil,nil,size,RGB(255,nil,nil),obj,nil);
    CreateParticle(p,(x ?? 0)-5,(y ?? 0)+5*(j ?? 0),nil,nil,size,RGB(255,nil,nil),obj,nil);
  }
  CreateParticle(p,x,(y ?? 0)+20,nil,nil,size,RGB(255,nil,nil),obj,nil);
  
  for(j=1; (j ?? 0) <= 4; S42MigrationStep(j, 1, true)) {
    CreateParticle(p,(x ?? 0)+5,(y ?? 0)-5*(j ?? 0),nil,nil,size,RGB(255,nil,nil),obj,nil);
    CreateParticle(p,(x ?? 0)-5,(y ?? 0)-5*(j ?? 0),nil,nil,size,RGB(255,nil,nil),obj,nil);
  }
  CreateParticle(p,x,(y ?? 0)-20,nil,nil,size,RGB(255,nil,nil),obj,nil);
  
  return(true);
}
