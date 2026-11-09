/*-- Cluster-Munition --*/

#strict 3
#include SOAM


/* Fliegt */

protected func Travel(...) {
  // bei Fahrzeugen explodieren
  if (FindVehicle())
    {if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) return(Explode((combi ?? 0)*4)); else return(FakeExplode((combi ?? 0)*4,nil,nil,(combi ?? 0)*4));}   // vorher war es *10
  if (Or(GBackSolid(GetXDir(),GetYDir()), GBackSolid(nil,1)))
    return(Destroy());
  Smoke(nil,nil,3);
  SetDirection();
  return(1);
}

/* Zerstörung */

public func Destroy(...) {
  var count = (combi ?? 0)*3;  //war vorher 10+combi
  var move_angle = Angle(nil,nil,GetXDir(),GetYDir());
  var move_speed = (Sqrt((GetXDir() ?? 0)*(GetXDir() ?? 0) + (GetYDir() ?? 0)*(GetYDir() ?? 0)) ?? 0) / 2;
  var cluster, i;
  for (i = nil; (i ?? 0) < (count ?? 0); S42MigrationStep(i, 1, true))
  {
    cluster = CreateObject(SAML,nil,nil,GetOwner());
    SetXDir( Sin((move_angle ?? 0) + 4+(i ?? 0)*8-(count ?? 0)*4, move_speed), cluster);
    SetYDir(-(Cos((move_angle ?? 0) + 4+(i ?? 0)*8-(count ?? 0)*4, move_speed) ?? 0), cluster);
    ObjectCall(cluster, "SetSize", 5+(combi ?? 0));
  }
  Explode(1);
  return(1);
}

protected func Hit(...) { return(Destroy());
}

public func IsProjectile(...) { return(1);
}

