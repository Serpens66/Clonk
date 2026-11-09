#strict
#appendto HZCK

local ammobag;

protected func ContextAmmobag(object pCaller)
{
  [$AmmoBag$|Image=STAP|Condition=AmmoBagContextCheck]
  var ammo, x;
  //Menü erzeugen
  CreateMenu(STAP,this(),0,0,GetName(0,ABAG),0,1,1);
  //Munition finden und hinzufügen
  while(ammo = Contents(x,ammobag))
  {
    // nur Ammo
    if(!(ammo ->~IsAmmo())) continue;
    //hinzufügen
    AddMenuItem(GetName(ammo),Format("PackAmmo(%i);",GetID(ammo)), GetID(ammo), this(), GetAmmo(GetID(ammo)),0,GetDesc(ammo));
    //Zählervariable erhöhen
    x++;
  }
  return(1);
}

protected func PackAmmo(ammo)
{
	var packed,pack_id,in_ammobag;
	if(ammo==STAM){ pack_id=STAP; packed=pack_id->AmmoCount();}
	if(ammo==ENAM){ pack_id=ENAP; packed=pack_id->AmmoCount();}
	if(ammo==GRAM){ pack_id=GRAP; packed=pack_id->AmmoCount();}
	if(ammo==GSAM){ pack_id=GSAP; packed=pack_id->AmmoCount();}
	if(ammo==MIAM){ pack_id=MIAP; packed=pack_id->AmmoCount();}
	if(ammo==SSTM){ pack_id=STLM; packed=pack_id->AmmoCount();}
	if(ammo==SHTM){ pack_id=SHMU; packed=pack_id->AmmoCount();}
	if(ammo==SPBT){ pack_id=SNI_; packed=pack_id->AmmoCount();}
	if(ammo==PTAM){ pack_id=PTAP; packed=pack_id->AmmoCount();}
	in_ammobag=GetAmmo(ammo);
	if(packed<=in_ammobag)
	{
		Sound("Click");
		DoAmmo(ammo,-packed);
		CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());
		ContextAmmobag(this());
		return(1);
	}
        if(packed>in_ammobag && 0<in_ammobag)
        {
	var muni=CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());
        muni ->~ RestAmmoreduzieren(packed-in_ammobag); // muni im paket wird um diese Zahl reduziert
	Sound("Click");
        DoAmmo(ammo,-in_ammobag);
        ContextAmmobag(this());
	return(1);
        }
	Sound("CommandFailure1");
	return(0);
}
