#strict 3

local eat;
local start;

protected func Initialize() {
  start = nil;
  SetAction("StartUp");
  eat = nil;
  return( 1);
}

func Activity() {
  if(!!SEqual(GetAction(), "Sleep"))
    return(1);
  if((GetCon() ?? 0) < 70)
    Death();
  if( !SEqual(GetAction(), "Wait"))
   return( 1);
  if( FindObject2(Find_Category(C4D_Living),Find_Distance(8),Find_NoContainer())) 
   Hamm();
  while(Contents(nil))
    Digest(Contents(nil));
  return( 1); 
}

protected func Hamm() {
  if( !SEqual(GetAction(), "Schnapp")) SetAction( "ShowTeeth");
  Sound( "Dig*");
  return( 1);
}

protected func RejectCollect(idObj, pObj)
{
	if(!!SEqual(GetAction(), "Sleep"))
	  return(1);
	if(Equal(idObj, GOLD))
		start = true;
        // wenn explosives Objekt, tschüss ^^ 
        if (pObj->~ExplodeSize())
	{
                Sound("elbom");
                Explode((pObj->~ExplodeSize() ?? 0)/2);
		Death();		
	}
        if(Equal(start, nil))
	  return(1);
	Sound("namnam");
	SetAction("ShowTeeth");
	Digest(pObj);
	return(nil);
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
		SetClrModulation(nil);
		SetAction("wait");
	}	
}

protected func Digest(pObj)
{
	var die = nil;
	if (pObj->~ExplodeSize())
		die = true;
	if (Equal(GetID(pObj), EJ8Z))	{
		GrabContents(pObj);
	}
	if (Equal(GetID(pObj), 938Z))
	{
		if (LocalN("item1",pObj)->~ExplodeSize()) die = true;
		if (LocalN("item2",pObj)->~ExplodeSize()) die = true;
		if (LocalN("item3",pObj)->~ExplodeSize()) die = true;
	}
	if (OnFire(pObj)) Incinerate(this);

        if (pObj->~IsWaste())
        {
        if (Or(Equal(GetID(pObj), SHLK), Equal(GetID(pObj), _SKA))) {die = true;}  // bei Schlacke
        if (Equal(GetID(pObj), MTWS)) {die = true;}  // bei Metallschrott
        if (Or(Equal(GetID(pObj), CHMU), Equal(GetID(pObj), _CHF))) {die = true;}  // bei Chemieabfall
        if (Equal(GetID(pObj), CORP)) {Log("Leiche gefressen");die = true;}  // bei Leiche... frisst er aber glaub ich nicht, kA warum
        if (Or(Equal(GetID(pObj), HMWS), Equal(GetID(pObj), _MSK))) {die = true;}  // bei Hausmüll
        if (Equal(GetID(pObj), AMWS)) {die = true;}  // bei Atommüll
        if (Equal(GetID(pObj), YECK)) {die = true;}   // bei Yellow Cake
        }

	RemoveObject(pObj);

	if ( Equal(die, true) )
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
     Exit(find,nil,15);
   }
   if (eat)
   {
	SetAction("Sleep");
	eat = nil;
   }
  return( 1);
}

protected func Death() {
  Sound("elbom");
  RemoveObject();
  return(1);
}