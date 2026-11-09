#strict 3
#appendto CAN_
#appendto CCD_
#appendto TCN_
#appendto CGC_
#appendto MAM_
#appendto TSP_

func IsTeleporterTarget(){return(true);}

func OnTeleportIn(object pTarget)
{
  pTarget->Enter(this());
  pTarget->Sound("Electric");
  pTarget->Sound("Fire1");
  return(true);
}