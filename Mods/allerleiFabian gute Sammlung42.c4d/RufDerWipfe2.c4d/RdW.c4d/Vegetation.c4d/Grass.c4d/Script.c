/*-- Grass --*/
#strict
#include GRAS

private func ReproductionAreaSize() { return(800); } // Größe des Bereichs, in dem neue Bäume dieser Art entstehen können
private func ReproductionRate()     { return(10); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxTreeCount()         { return(70); } // Maximale Baumanzahl im Umkreis
private func ForestChance()         { return(1);}  // Chance und Grösse eines Waldes

protected func Initialize()
{
  // Zufällige Größe
 if (Random(5))
    DoCon(-Random(20) - 40);
  else
    DoCon(-Random(60) - 20);
  // Zufällige Form
  SetAction("Grass");
  SetPhase(Random(2));
  if (!Random(8)) SetPhase(2);
  // if(!Random(5)) SetAction("GrassTF");  // vordergrund.. funktioniert nicht, daher nur defcore
  // Zufällige Richtung
 if (Random(2)) SetDir(DIR_Right);
  // Drehung nach Erdoberfläche
  var x_off = 18 * GetCon() / 100;
  var y_off = 15 * GetCon() / 100;
  var slope = GetSolidOffset(-x_off, y_off) - GetSolidOffset(x_off, y_off);
  SetR(slope);
  // Höhe anpassen
  while (!GBackSolid(0, 5))
    SetPosition(GetX(), GetY() + 1);
  // Gras bleibt hinter Bäumen
 MoveBehindTrees();
 Inited();
}

func Inited() // nicht sicher wann construction aufgerufen wird, daher auch hier der material check nach init
{
    /* Wüste? */
  if(GetMaterial(0,3) != Material("Earth"))  //nur auf Erde wachsen
    {
      RemoveObject();
    	return(1);
    }
  // if(GetMaterial(AbsX(GetX(pClonk)),AbsY(GetY(pClonk))) == Material("Tunnel")) // auch nicht unter der Erde ... erstmal deaktiveren, da unter der erde eig doch okay, nur nicht auf fahrstühlen oderso, und das ist mit Erdekontrolle eigentlich gegeben
  // if(Random(4)) // 0,1,2 oder 3 -> nur in 1/4 der Fälle stehen lassen 
  // {
      // RemoveObject();
    	// return(1);
  // }
  // Waldbildung
}


func Incineration()
{
  SetClrModulation(RGB(127,100,127));
  Schedule("DoCon(-1)",1,50);
  SetGraphics("");
}

func Construction()
{
  ScheduleCall(0,"Constructed",1);
  return(1);
}

func Constructed()
{
 var pReproduct;
  /* Wüste? */
  if(GetMaterial(0,3) != Material("Earth"))  //nur auf Erde wachsen
    {
      RemoveObject();
    	return(1);
    }
  // if(GetMaterial(AbsX(GetX(pClonk)),AbsY(GetY(pClonk))) == Material("Tunnel")) // auch nicht unter der Erde ... erstmal deaktiveren, da unter der erde eig doch okay, nur nicht auf fahrstühlen oderso, und das ist mit Erdekontrolle eigentlich gegeben
  // if(Random(4)) // 0,1,2 oder 3 -> nur in 1/4 der Fälle stehen lassen 
  // {
      // RemoveObject();
    	// return(1);
  // }
  // Waldbildung
  if(FrameCounter()<5)
    if(Random(ForestChance()))
      if(pReproduct=Reproduction())
	     pReproduct->~SetCon(BoundBy(RandomX(10,150),10,100));
	//ResortObject("ResortForeground");
  return(1);
}

/* Fortpflanzung */

public func Seed()
{
 if(GetMaterial(0,3) != Material("Earth"))  //nur auf Erde wachsen, zb wenn erde mittlerweile weggegraben, soll gras nicht in der luft hängen
    {
      RemoveObject();
    	return(1);
    }
 var pReproduct;
  if(GetCon()<80)
   return(1);
 if(GetTemperature()<0)
   return(1);
  // Je nach Fortpflanzungsrate mehr oder weniger oft...
//  if(!Random(ReproductionRate()))
    // ...reproduzieren
   if(pReproduct=Reproduction())
	     pReproduct->~NewTree();
}
public func Reproduction()
{
 // Ist noch Platz für einen Baum?
  var iSize = ReproductionAreaSize();
  var iOffset = iSize / -2;
//  if (ObjectCount(0, iOffset, iOffset, iSize, iSize, 0,"Grass")<MaxTreeCount())
if(ObjectCount2(Find_Distance(iSize),Find_ID(GetID(this())))<MaxTreeCount())
    // OK, hin damit
{ 
// Log("Grasgeplanzt");  
return(PlaceVegetation(GetID(this()), iOffset, iOffset, iSize, iSize, 10000) );};
  // Kein Platz ;'(
 return(0);
}

public func ForceGraphChange(){return true;} // Erzwingt Grafikwandlung
public func UpdateGraphic(sGraph){ if(IsAntiLag()) { SetGraphics(sGraph); return true;}}

public func IsBulletGrasTarget(){ return(true); }  //für Flammenwerfer



/*


// fürs ernten mit der Sense... ist in sense ausreichend geregelt

public func Ernte(object pByObj)
{
        var HarvestProductID = GetID();
        var HarvestDeathID = GetID();
	var dcon = GetDefCoreVal("Width", "DefCore", GetID()) * GetCon() / 200;
	var caColor = DefinitionCall(GetID(), "GetStrawColor");
	for (var cnt = 0; cnt < 15 + Random(10); cnt++)
	{
		CreateParticle("GrassBlade", RandomX(-dcon, dcon), -1, RandomX(-20, 20), RandomX(-40, -20), 30 + Random(30), RGB(caColor[0], caColor[1], caColor[2]), 0, 0);
	}
	var idtProduct, dtIndex = 0;
	while (idtProduct = DefinitionCall(GetID(), Format("HarvestProductID%d", ++dtIndex)))
		CreateContents(idtProduct, pByObj, DefinitionCall(GetID(), Format("HarvestProductCount%d", dtIndex)));
	dtIndex = DefinitionCall(GetID(), "HarvestStrawAmount");
	while (dtIndex--)
		CreateContents(DTFH, pByObj)->~SetType(GetID());
	if (HarvestDeathID())
		CreateObject(HarvestDeathID(), 0, +GetDefCoreVal("Height", "DefCore", GetID()) * GetCon() / 200);
	RemoveObject();
	return 1;
} //ernten


func IsField() {return(1);}
*/





