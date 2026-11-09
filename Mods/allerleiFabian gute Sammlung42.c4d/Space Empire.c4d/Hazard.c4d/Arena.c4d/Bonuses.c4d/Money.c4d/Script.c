/*-- Money --*/

#strict 3

public func IsBonus() { return(true); }

public func Color() { return(RGB(255,255,255)); }

public func Activate(object obj) {
  Sound("Money", 1, nil,nil, (GetOwner(obj) ?? 0)+1);
  var plr = GetController(obj);
  //ka-ching!
  SetWealth(plr,(GetWealth(plr) ?? 0)+100);
  
  //Effekt! :O
  var x = GetX(obj), y = GetY(obj);
  var p = "NoGravSpark", s = 35, c = RGB(255,255,nil);
  CreateParticle(p,(x ?? 0)+10,(y ?? 0)-13,nil,nil,s,c,obj);
  CreateParticle(p,(x ?? 0)+5,(y ?? 0)-15,nil,nil,s,c,obj);
  CreateParticle(p,nil,  (y ?? 0)-15,nil,nil,s,c,obj);
  CreateParticle(p,(x ?? 0)-5,(y ?? 0)-13,nil,nil,s,c,obj);
  CreateParticle(p,(x ?? 0)-10,(y ?? 0)-9,nil,nil,s,c,obj);
  CreateParticle(p,(x ?? 0)-9,(y ?? 0)-4,nil,nil,s,c,obj);
  CreateParticle(p,(x ?? 0)-3,(y ?? 0)-1,nil,nil,s,c,obj);
  
  CreateParticle(p,(x ?? 0)+3,(y ?? 0)+1,nil,nil,s,c,obj);
  CreateParticle(p,(x ?? 0)+9,(y ?? 0)+4,nil,nil,s,c,obj);
  CreateParticle(p,(x ?? 0)+10,(y ?? 0)+9,nil,nil,s,c,obj);
  CreateParticle(p,(x ?? 0)+5,(y ?? 0)+13,nil,nil,s,c,obj);
  CreateParticle(p,x,  (y ?? 0)+15,nil,nil,s,c,obj);
  CreateParticle(p,(x ?? 0)-5,(y ?? 0)+15,nil,nil,s,c,obj);
  CreateParticle(p,(x ?? 0)-10,(y ?? 0)+13,nil,nil,s,c,obj);
  
  for(var j; (j ?? 0) <= 8; S42MigrationStep(j, 1, true))
    CreateParticle(p,x,(y ?? 0)-20+5*(j ?? 0),nil,nil,s,c,obj);
  return(true);
}
