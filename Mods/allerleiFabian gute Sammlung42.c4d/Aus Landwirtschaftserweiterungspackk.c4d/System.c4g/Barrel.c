/* --- Fass --- */

#strict 2

#appendto BARL

//Besser fürs suchen
public func IsBarrelMaterial(int dnMaterial)
{
	return dnMaterial == BarrelMaterial();
}