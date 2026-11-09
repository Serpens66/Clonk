/*-- Burgfahrstuhl --*/

#strict 3
#appendto F_EL // metall
#appendto ELEF // normal
// #appendto CPEL // burg
#appendto BE5C // bunker, achtung korb hat selbe ID wie burgkorb CPEC

local sperre;

func addsperre()
{
sperre=1;
return(1);
}

func hassperre()
{
return(sperre);
}
