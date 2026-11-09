/*-- zu weapons --*/

#strict 3

#appendto WEPN

// wenn der Waffennutzer ein Arbeiter ohne Munitionsgürtel ist, dann kann er keine Waffen nutzen
public func ControlThrow(caller)
{
if(!(caller->~IstBauarbeiter()))
return(_inherited(caller));

// wenn Bauarbeiter und keiner der Guertel angelegt, dann Waffen nicht nutzbar
  if(caller->~IstBauarbeiter()) // wenn Arbeiter... wenn mind. stufe 15 mit gürtel, dann waffe nutzbar
if(And((caller->~AktuelleStufe() ?? 0)>=15, (Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or((caller->~MoreAmmo1()), (caller->~MoreAmmo2())), (caller->~MoreAmmo3())), (caller->~MoreAmmo4())), (caller->~MoreAmmo5())), (caller->~MoreAmmo6())), (caller->~MoreAmmo7())), (caller->~MoreAmmo8())), (caller->~MoreAmmo9())), (caller->~MoreAmmo10())), (caller->~MoreAmmo11())), (caller->~MoreAmmo12())), (caller->~MoreAmmo13())), (caller->~MoreAmmo14())),
(caller->~MoreAmmo15())), (caller->~MoreAmmo16())), (caller->~MoreAmmo17())), (caller->~MoreAmmo18())), (caller->~MoreAmmoAllrounder1())), (caller->~MoreAmmoAllrounder2())) )) )
return(_inherited(caller)); 
else {return(PlayerMessage(GetOwner(caller), "Arbeiter koennen ohne Munitionsguertel und unter Stufe 15 keine Waffen nutzen oder Munition aufnehmen" , caller));} 
}

