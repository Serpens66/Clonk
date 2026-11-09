/*-- Flower --*/

#strict 2
#include GRA1

private func ReproductionAreaSize() { return(500); } // Größe des Bereichs, in dem neue Bäume dieser Art entstehen können
private func ReproductionRate()     { return(20); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxTreeCount()         { return(10); } // Maximale Baumanzahl im Umkreis
private func ForestChance()         { return(1);}  // Chance und Grösse eines Waldes


protected func Initialize(obj) { 
	SetAction("Flower");
	SetPhase(Random(3));
	MoveBehindTrees();
}

public func Seed()
{
 if(GetMaterial(0,5) != Material("Earth"))  //nur auf Erde wachsen, zb wenn erde mittlerweile weggegraben, soll gras nicht in der luft hängen
    {
      RemoveObject();
    	return(1);
    }
 var pReproduct;
  if(GetCon()<80)
   return(1);
 if(GetTemperature()<0)
   return(1);
 if(!(GetSeason()>=12 && GetSeason()<62)) // wenn weder Frühling noch Sommer, nicht pflanzen.
  return(1);
  // Je nach Fortpflanzungsrate mehr oder weniger oft...
//  if(!Random(ReproductionRate()))
    // ...reproduzieren
   if(pReproduct=Reproduction())
	     pReproduct->~NewTree();
}