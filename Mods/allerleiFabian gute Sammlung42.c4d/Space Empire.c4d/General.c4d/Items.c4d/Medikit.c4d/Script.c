/*-- Medikit --*/

#strict 3

protected func Hit()
{
  Sound("RockHit");
}

protected func Activate(caller)
{
  if (OnFire(caller)) { 
    Sound("ExHeal"); 
    Extinguish(caller); 
    return(RemoveObject());
  } 
  Sound("Heal");
  DoEnergy(+100,caller);  
  RemoveObject();
}

