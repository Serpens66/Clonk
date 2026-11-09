/*-- Drachenhorst --*/

#strict

func IsBulletTarget() { return(1); }

func ProduceEgg()
{
 CreateObject(SEGG,-15,0);
 Sound("Splat");
 return(1);
}

func Timer()
{
 if (!Random(80) && GetAction() ne "Produce") SetAction("Produce");
 return(1);
}

func Death()
{
 Incinerate();
 return(1);
}