/* --- Fass --- */

#strict 3

#appendto BARL

//Besser fürs suchen
public func IsBarrelMaterial(int dnMaterial)
{
	dnMaterial ??= 0;
	return S42MigrationEqual(dnMaterial, BarrelMaterial());
}