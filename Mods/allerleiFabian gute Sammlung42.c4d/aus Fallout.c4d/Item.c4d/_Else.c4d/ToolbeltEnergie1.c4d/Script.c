/*-- Gürtel klein Munition --*/

#strict
#include GEAR





/* Status */

public func GetGearType()	{ return(GEAR_Boots); }
protected func Hit()		{ Sound("Munch1"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
pClonk->GetMoreAmmo(3);
}

public func GearEffectReset(object pClonk)
{
pClonk->GetNormalAmmo();
Ammomengekorrigieren(pClonk);
}

func Ammomengekorrigieren(pClonk)
{

//Dafür sorgen, dass beim ablegen des Gürtels das Maximum nicht überschritten ist und Munitionskisten erzeugen

var ammo_id = ENAM;
var pack_id = ENAP;
var in_ammobag= GetAmmo(ammo_id,pClonk);
var maximum = ammo_id->MaxAmmo(pClonk);
var packed = pack_id->AmmoCount();
if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);

if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);

if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);

if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);

if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);

if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);
if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);
if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);
if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);
if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);
if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);
if(in_ammobag>maximum)
{ DoAmmo(ammo_id,-packed,pClonk); CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());}
in_ammobag= GetAmmo(ammo_id,pClonk);


}



public func IsRefineryProduct() { return(1); }
