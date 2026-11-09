/*-- Knochen --*/

#strict

protected func Hit()
{
  Sound("WoodHit*");
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(120); }

func IsManaContainer() { return(true); }
func ManaProcessTime() { if(GetAction()S="BBON") return(280); return(240); }
