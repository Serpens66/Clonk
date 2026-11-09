/*-- Cluster-Munition --*/

#strict
#include SOAM


/* Fliegt */

protected Travel:
  // bei Fahrzeugen explodieren
  if (FindVehicle())
    {if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) return(Explode(combi*4)); else return(FakeExplode(combi*4,0,0,combi*4));}   // vorher war es *10
  if (GBackSolid(GetXDir(),GetYDir()) || GBackSolid(0,1))
    return(Destroy());
  Smoke(0,0,3);
  SetDirection();
  return(1);

/* Zerstörung */

public Destroy:
  var count = combi*3;  //war vorher 10+combi
  var move_angle = Angle(0,0,GetXDir(),GetYDir());
  var move_speed = Sqrt(GetXDir()*GetXDir() + GetYDir()*GetYDir()) / 2;
  var cluster, i;
  for (i = 0; i < count; i++)
  {
    cluster = CreateObject(SAML,0,0,GetOwner());
    SetXDir( Sin(move_angle + 4+i*8-count*4, move_speed), cluster);
    SetYDir(-Cos(move_angle + 4+i*8-count*4, move_speed), cluster);
    ObjectCall(cluster, "SetSize", 5+combi);
  }
  Explode(1);
  return(1);

protected Hit: return(Destroy());

public IsProjectile: return(1);

