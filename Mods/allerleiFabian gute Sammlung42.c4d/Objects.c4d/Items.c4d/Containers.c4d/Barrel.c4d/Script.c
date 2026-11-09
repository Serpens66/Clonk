/*-- Fass --*/

#strict 3

// Anzahl Materialpixel, die in ein Fass passen
static const BARL_MaxFill = 200;

// Füllstand
local iFillLevel;

// Füllmaterial+1
// Benutzt nur vom Lavafass, um zwischen Lava und DuroLava zu unterscheiden
// Kann für Fässer aus alten Spielständen 0 sein!
local iFillMaterial;

// Gegenwärtiges Material, mit dem nicht aufgefüllt werden konnte
local iBackMaterial;

// ID des leeren Fasses
global func EmptyBarrelID() { return(BARL); }

// Verkauf
protected func SellTo()
{
  // Fasskonfiguration: Gegebenenfalls Verkauf zum gefüllten Fass
  if ((BarrelConfiguration() ?? 0) & 2) return(GetID());
  // Ansonsten beim Verkauf entleeren
  return(EmptyBarrelID());
}

/* TimerCall */

private func FillCheck()
{
  // Wenn das normale Fass per ChangeDef zurueckgeaendert wurde
  // koennte die Fuellvariable noch voll sein
  if(Equal(BarrelMaterial(), -1))
  {
  	if(iFillLevel)
                iFillLevel=nil;
  }
  
  // Material an aktueller Position überprüfen
  var iMaterial;
  if ( Or((Equal((iMaterial = GetMaterial()), -1)), (Equal(iMaterial, iBackMaterial))) ) return(nil);
  // Fass für dieses Material suchen
  var idBarrel;
  if (!(idBarrel = GetBarrelType(iMaterial))) return(iBackMaterial = iMaterial);
  // Fass füllen wenn entweder leer oder mit dem gleichen Material aufgefüllt werden kann
  if(And(!Equal(BarrelMaterial(), -1), !Equal(BarrelMaterial(), iMaterial))) return(nil);
  // Wenn nicht bereits voll
  if((iFillLevel ?? 0) >= (BarrelMaxFill() ?? 0)) return(nil);
  // Fass füllen
  S42MigrationAssign(iFillLevel, "+=", (ExtractMaterialAmount(nil, nil, iMaterial, (BarrelMaxFill() ?? 0) - (iFillLevel ?? 0)) ?? 0));
  // Zum vollen Fass werden, wenn wir es nicht bereits sind
  if(Equal(BarrelMaterial(), -1))
  {
    iFillMaterial = (iMaterial ?? 0)+1;
    ChangeDef(idBarrel);
  }
  return(1);
}

/* Füllung */

// Aktuelle Füllung
public func GetAmount() { return(iFillLevel); }

// Füllmaterial
public func BarrelMaterial() { return(-1); }
public func BarrelMaterialName() { return; }

// Maximale Füllung
public func BarrelMaxFill() { return(BARL_MaxFill); }

// Füllung erhöhen/verringern
public func BarrelDoFill(int iChange, int iMat)
{
  iChange ??= 0;
  iMat ??= 0;
  var iNewFill = BoundBy((iFillLevel ?? 0) + (iChange ?? 0), nil, BarrelMaxFill());
  if (Equal(iNewFill, iFillLevel)) return;
  S42MigrationAssign(iNewFill, "-=", (iFillLevel ?? 0)); S42MigrationAssign(iFillLevel, "+=", (iNewFill ?? 0));
  // Aktuelles Füllmaterial ggf. überschreiben
  if (And(iMat, (iChange ?? 0)>0)) iFillMaterial = iMat;
  // Tatsächliche Änderung des Füllstandes zurückgeben
  return(iNewFill);
}

// Fass voll?
public func BarrelIsFull() 
{ 
  return(Equal(iFillLevel, BarrelMaxFill()));
}

// Ein Pixel Füllung entfernen
public func BarrelExtractLiquid()
{
  // Füllung verringern
  if (!BarrelDoFill(-1)) 
    // Konnte Füllung nicht verringern: Fass ist leer/kein Material
    return(-1);
  // Füllmaterial speichern    
  var iMaterial = BarrelMaterial();
  // Jetzt leer?
  if (!iFillLevel) 
    ChangeDef(EmptyBarrelID());
  // Füllmaterial zurückliefern
  return(iMaterial);
}

/* Wert nach Füllung */

// Wert = Wert(Leeres Fass) + (Wert(Volles Fass) - Wert(Leeres Fass)) * (Füllung / MaxFüllung)
protected func CalcValue()
{
  var iValueEmpty = GetValue(nil, EmptyBarrelID());
  var iValueFull = GetValue(nil, GetID());
  return((iValueEmpty ?? 0) + (((iValueFull ?? 0) - (iValueEmpty ?? 0)) * (iFillLevel ?? 0)) / (BarrelMaxFill() ?? 0));
}

/* Aufschlag */ 
  
protected func Hit()
{
  Sound("ClonkHit*");
  BarrelEject();
  return(1);
}

private func BarrelEject()
{
  var strMaterial;
  if (!(strMaterial = BarrelMaterialName())) return(1);
  Sound("Splash1");
  CastPXS(strMaterial, iFillLevel, 20);
  iFillLevel = nil;
  ChangeDef(EmptyBarrelID());
  return(1);  
}

/* Initialisierung */

protected func Initialize()
{
  // Kein Material im Hintergrund
  iBackMaterial = -1;
  // Materialfass? Füllen
  if (BarrelMaterialName()) 
    iFillLevel = BarrelMaxFill();
  return(1);
}


/* Aktivierung auf Doppel-Graben */

protected func Activate()
{
  [$TxtEject$]
  // Ausleeren
  return(BarrelEject());
}
  
/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }
