/*-- Drachenhorst --*/

#strict 3

func IsBulletTarget() { return(1); }

func ProduceEgg()
{
 CreateObject(SEGG,-15,nil);
 Sound("Splat");
 return(1);
}

func Timer()
{
 if (And(!Random(80), !SEqual(GetAction(), "Produce"))) SetAction("Produce");
 return(1);
}

func Death()
{
 Incinerate();
 return(1);
}