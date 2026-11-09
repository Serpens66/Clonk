#strict 3
#appendto HZCK

local ammobag;

protected func ContextAmmobag(object pCaller)
{
  [$AmmoBag$|Image=STAP|Condition=AmmoBagContextCheck]
  var ammo, x;
  //Menü erzeugen
  CreateMenu(STAP,this(),nil,nil,GetName(nil,ABAG),nil,1,1);
  //Munition finden und hinzufügen
  while(ammo = Contents(x,ammobag))
  {
    // nur Ammo
    if(!(ammo ->~IsAmmo())) continue;
    //hinzufügen
    AddMenuItem(GetName(ammo),Format("PackAmmo(%i);",GetID(ammo)), GetID(ammo), this(), GetAmmo(GetID(ammo)),nil,GetDesc(ammo));
    //Zählervariable erhöhen
    S42MigrationStep(x, 1, true);
  }
  return(1);
}

protected func PackAmmo(ammo)
{
	var packed,pack_id,in_ammobag;
	if(Equal(ammo, STAM)){ pack_id=STAP; packed=pack_id->AmmoCount();}
	if(Equal(ammo, ENAM)){ pack_id=ENAP; packed=pack_id->AmmoCount();}
	if(Equal(ammo, GRAM)){ pack_id=GRAP; packed=pack_id->AmmoCount();}
	if(Equal(ammo, GSAM)){ pack_id=GSAP; packed=pack_id->AmmoCount();}
	if(Equal(ammo, MIAM)){ pack_id=MIAP; packed=pack_id->AmmoCount();}
	if(Equal(ammo, SSTM)){ pack_id=STLM; packed=pack_id->AmmoCount();}
	if(Equal(ammo, SHTM)){ pack_id=SHMU; packed=pack_id->AmmoCount();}
	if(Equal(ammo, SPBT)){ pack_id=SNI_; packed=pack_id->AmmoCount();}
	if(Equal(ammo, PTAM)){ pack_id=PTAP; packed=pack_id->AmmoCount();}
	in_ammobag=GetAmmo(ammo);
	if((packed ?? 0)<=(in_ammobag ?? 0))
	{
		Sound("Click");
		DoAmmo(ammo,-(packed ?? 0));
		CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());
		ContextAmmobag(this());
		return(1);
	}
        if(And((packed ?? 0)>(in_ammobag ?? 0), 0<(in_ammobag ?? 0)))
        {
	var muni=CreateObject(pack_id,AbsX(GetX()),AbsY(GetY()),GetOwner());
        muni ->~ RestAmmoreduzieren((packed ?? 0)-(in_ammobag ?? 0)); // muni im paket wird um diese Zahl reduziert
	Sound("Click");
        DoAmmo(ammo,-(in_ammobag ?? 0));
        ContextAmmobag(this());
	return(1);
        }
	Sound("CommandFailure1");
	return(nil);
}
