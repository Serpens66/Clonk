#strict

local eat;
local start;

protected func Initialize() {
  start = false;
  SetAction("StartUp");
  eat = false;
  return( 1);
}

func Activity() {
  if(GetAction() eq "Sleep")
    return(1);
  if(GetCon() < 70) 
    Death();
  if( GetAction() ne "Wait") 
   return( 1);
  if( FindObject2(Find_Category(C4D_Living),Find_Distance(8),Find_NoContainer())) 
   Hamm();
  while(Contents(0))
    Digest(Contents(0));
  return( 1); 
}

protected func Hamm() {
  if( GetAction() ne "Schnapp") SetAction( "ShowTeeth");
  Sound( "Dig*");
  return( 1);
}

protected func RejectCollect(idObj, pObj)
{
	if(GetAction() eq "Sleep")
	  return(1);
	if(idObj == GOLD)
		start = true;
        // wenn explosives Objekt, tschüss ^^ 
        if (pObj->~ExplodeSize())
	{
                Sound("elbom");
                Explode(pObj->~ExplodeSize()/2);
		Death();		
	}
        if(start == false)
	  return(1);
	Sound("namnam");
	SetAction("ShowTeeth");
	Digest(pObj);
	return(0);
}



protected func Sleep()
{
	SetClrModulation(RGB(70,100,150));		
}

protected func StartUp()
{
	SetClrModulation(RGB(140,60,60));		
}

protected func Wake()
{
	if (start) 
	{
		SetClrModulation(0);
		SetAction("wait");
	}	
}

protected func Digest(pObj)
{
	var die = false;
	if (pObj->~ExplodeSize())
		die = true;
	if (GetID(pObj) == EJ8Z)	{
		GrabContents(pObj);
	}
	if (GetID(pObj) == 938Z)
	{
		if (LocalN("item1",pObj)->~ExplodeSize()) die = true;
		if (LocalN("item2",pObj)->~ExplodeSize()) die = true;
		if (LocalN("item3",pObj)->~ExplodeSize()) die = true;
	}
	if (OnFire(pObj)) Incinerate(this);

        if (pObj->~IsWaste())
        {
        if (GetID(pObj)==SHLK || GetID(pObj)==_SKA) {die = true;}  // bei Schlacke
        if (GetID(pObj)==MTWS) {die = true;}  // bei Metallschrott
        if (GetID(pObj)==CHMU || GetID(pObj)==_CHF) {die = true;}  // bei Chemieabfall
        if (GetID(pObj)==CORP) {Log("Leiche gefressen");die = true;}  // bei Leiche... frisst er aber glaub ich nicht, kA warum
        if (GetID(pObj)==HMWS || GetID(pObj)==_MSK) {die = true;}  // bei Hausmüll
        if (GetID(pObj)==AMWS) {die = true;}  // bei Atommüll
        if (GetID(pObj)==YECK) {die = true;}   // bei Yellow Cake
        }

	RemoveObject(pObj);

	if ( die == true )
	{
		Sound("Blast1");
		Death();		
	}
	eat = true;
}

protected func SchnappEnd() {
   for(var find in FindObjects(Find_Category(C4D_Living),Find_Distance(8),Find_NoContainer())) 
   {
     Sound("namnam");
     GrabContents(find);
     Enter(this(),find);
     DoEnergy(-30000,find,1);
     Exit(find,0,15);
   }
   if (eat)
   {
	SetAction("Sleep");
	eat = false;
   }
  return( 1);
}

protected func Death() {
  Sound("elbom");
  RemoveObject();
  return(1);
}