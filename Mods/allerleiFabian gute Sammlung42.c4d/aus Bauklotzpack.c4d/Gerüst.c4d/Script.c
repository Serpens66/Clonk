/*-- Gerüst --*/

#strict 3

local pOverlay;

func Initialize() {
   SetAction("Normal");
  return(1);
}


func IsRepa(){return(1);}

public func ControlDig(pClonk) 
{
  if(Hostile(GetOwner(),GetOwner(pClonk))) return(1);
  if(pOverlay) RemoveObject(pOverlay);
   if(S42MigrationEqual(GetAction(), "Schieflinks"))
 {
   SetPosition(GetX(),GetY(),pClonk);
   SetAction("Rueckschieflinks");
   return(1);
  }

if(S42MigrationEqual(GetAction(), "Schiefrechts"))
 {
   SetPosition(GetX(),GetY(),pClonk);
   SetAction("Rueckschiefrechts");
   return(1);
  }

if(S42MigrationEqual(GetAction(), "Linksschief"))
 {
   SetPosition(GetX(),GetY(),pClonk);
   SetAction("Ruecklinksschief");
   return(1);
  }

if(S42MigrationEqual(GetAction(), "Rechtsschief"))
 {
   SetPosition(GetX(),GetY(),pClonk);
   SetAction("Rueckrechtsschief");
   return(1);
  }

if(S42MigrationEqual(GetAction(), "Rechtsod"))
 {
   SetAction("Rueckrechtsod");
   return(1);
  }

if(S42MigrationEqual(GetAction(), "Linksod"))
 {
   SetAction("Ruecklinksod");
   return(1);
  }

if(S42MigrationEqual(GetAction(), "Rechts"))
 {
   SetAction("Rueckrechts");
   return(1);
  }

if(S42MigrationEqual(GetAction(), "Links"))
 {
   SetAction("Ruecklinks");
   return(1);
  }

if(S42MigrationEqual(GetAction(), "Beide"))
 {
   SetAction("Rueckbeide");
   return(1);
  }

if(S42MigrationEqual(GetAction(), "Oben"))
 {
    SetAction("Rueckoben");
    return(1);
  }

if(S42MigrationEqual(GetAction(), "Normal"))
 {
  return(Menu(pClonk));
  }
}

func Oben() {
   SetAction("Zuoben");
  return(1);
}
func SolidOben() 
{
  if(GetName(nil, HZCK))
  {
   pOverlay = CreateObject(2X2H,nil,12,GetOwner());
   Local(nil,pOverlay)=this();
  }
  SetSolidMask(22,44,22,22,nil,nil);
 return(1);
}

func Schieflinks() {
    SetAction("Zuschieflinks");
   return(1);
}

func Schiefrechts() {
    SetAction("Zuschiefrechts");
   return(1);
}

func Rechts() {
    SetAction("Zurechts");
   return(1);
}

func Links() {
    SetAction("Zulinks");
   return(1);
}

func Beide() {
    SetAction("Zubeide");
   return(1);
}

func Schieflinks2() {
  if(GetName(nil, HZCK))
  {
   pOverlay = CreateObject(2X2H,nil,12,GetOwner());
   Local(nil,pOverlay)=this();
  }
    SetSolidMask(110,44,22,22,nil,nil);
   return(1);
}

func Schiefrechts2() {
  if(GetName(nil, HZCK))
  {
   pOverlay = CreateObject(2X2H,nil,12,GetOwner());
   Local(nil,pOverlay)=this();
  }
    SetSolidMask(176,44,22,22,nil,nil);
   return(1);
}

func Rechts2() {
  if(GetName(nil, HZCK))
  {
   pOverlay = CreateObject(2X2H,nil,12,GetOwner());
   Local(nil,pOverlay)=this();
  }
    SetSolidMask(66,44,22,22,nil,nil);
   return(1);
}

func Links2() {
  if(GetName(nil, HZCK))
  {
   pOverlay = CreateObject(2X2H,nil,12,GetOwner());
   Local(nil,pOverlay)=this();
  }
    SetSolidMask(44,44,22,22,nil,nil);
   return(1);
}

func Beide2() {
  if(GetName(nil, HZCK))
  {
   pOverlay = CreateObject(2X2H,nil,12,GetOwner());
   Local(nil,pOverlay)=this();
  }
    SetSolidMask(88,44,22,22,nil,nil);
   return(1);
}

func Normal() {
    SetSolidMask(nil,22,22,22,nil,nil);
   return(1);
}

//func Aufzug() {
//    DoDamage(GetDamage(),CreateObject(KLAU,0,11));
//    RemoveObject();
//   return(1);
//}

func Rechtsod() {
    SetAction("Zurechtsod");
   return(1);
}

func Rechtsod2() {
  if(GetName(nil, HZCK))
  {
   pOverlay = CreateObject(2X2H,nil,12,GetOwner());
   Local(nil,pOverlay)=this();
  }
    SetSolidMask(132,44,22,22,nil,nil);
   return(1);
}

func Linksod() {
    SetAction("Zulinksod");
   return(1);
}

func Linksod2() {
  if(GetName(nil, HZCK))
  {
   pOverlay = CreateObject(2X2H,nil,12,GetOwner());
   Local(nil,pOverlay)=this();
  }
    SetSolidMask(198,44,22,22,nil,nil);
   return(1);
}

func Rechtsschief() {
    SetAction("Zurechtsschief");
   return(1);
}

func Rechtsschief2() {
  if(GetName(nil, HZCK))
  {
   pOverlay = CreateObject(2X2H,nil,12,GetOwner());
   Local(nil,pOverlay)=this();
  }
    SetSolidMask(154,44,22,22,nil,nil);
   return(1);
}
func Linksschief() {
    SetAction("Zulinksschief");
   return(1);
}

func Linksschief2() {
  if(GetName(nil, HZCK))
  {
   pOverlay = CreateObject(2X2H,nil,12,GetOwner());
   Local(nil,pOverlay)=this();
  }
    SetSolidMask(220,44,22,22,nil,nil);
   return(1);
}

public func Menu(pClonk, object pObj) {

  if(pObj = FindObject2(Find_AtPoint(nil,12),Find_Exclude(),Find_NoContainer(),Find_ID(GRUS),Find_Allied(GetOwner()), ))
  {
   if((GetX() ?? 0) - (GetX(pObj) ?? 0) >= (-5) && (GetX() ?? 0) - (GetX(pObj) ?? 0) <= (5))
    {
     SetPosition(GetX(pObj),(GetY(pObj) ?? 0)-22,nil);
    }
   if((GetX() ?? 0) - (GetX(pObj) ?? 0) < (-5))
    {
     SetPosition((GetX(pObj) ?? 0)-11,(GetY(pObj) ?? 0)-22,nil);
    }
   if((GetX() ?? 0) - (GetX(pObj) ?? 0) > 5)
    {
     SetPosition((GetX(pObj) ?? 0)+11,(GetY(pObj) ?? 0)-22,nil);
    }
  }
  if(pObj = FindObject2(Find_AtPoint(nil,12),Find_Exclude(),Find_NoContainer(),Find_ID(KLZ1),Find_Allied(GetOwner()), ))
  {
   if((GetX() ?? 0) - (GetX(pObj) ?? 0) >= (-5) && (GetX() ?? 0) - (GetX(pObj) ?? 0) <= (5))
    {
     SetPosition(GetX(pObj),(GetY(pObj) ?? 0)-16,nil);
    }
   if((GetX() ?? 0) - (GetX(pObj) ?? 0) < (-5))
    {
     SetPosition((GetX(pObj) ?? 0)-11,(GetY(pObj) ?? 0)-16,nil);
    }
   if((GetX() ?? 0) - (GetX(pObj) ?? 0) > 5)
    {
     SetPosition((GetX(pObj) ?? 0)+11,(GetY(pObj) ?? 0)-16,nil);
    }
  }
  if(pObj = FindObject2(Find_AtPoint(nil,12),Find_Exclude(),Find_NoContainer(),Find_ID(KLZ2),Find_Allied(GetOwner()), ))
  {
   if((GetX() ?? 0) - (GetX(pObj) ?? 0) < 0)
    {
     SetPosition((GetX(pObj) ?? 0)-5,(GetY(pObj) ?? 0)-16,nil);
    }
   if((GetX() ?? 0) - (GetX(pObj) ?? 0) >= 0)
    {
     SetPosition((GetX(pObj) ?? 0)+6,(GetY(pObj) ?? 0)-16,nil);
    }
  }
   CreateMenu(GetID(),pClonk);
   AddMenuItem("Decke verfestigen","Oben",GetID(),pClonk,nil,nil,nil,2,1);
   AddMenuItem("schiefe Ebene nach Links","Schieflinks",GetID(),pClonk,nil,nil,nil,2,5);
   AddMenuItem("schiefe Ebene nach Rechts","Schiefrechts",GetID(),pClonk,nil,nil,nil,2,8);
   AddMenuItem("rechte Außenwand und Decke","Rechts",GetID(),pClonk,nil,nil,nil,2,3);
   AddMenuItem("linke Außenwand und Decke","Links",GetID(),pClonk,nil,nil,nil,2,2);
   //AddMenuItem("beide Außenwände","Beide",GetID(),pClonk,0,0,0,2,4);

   AddMenuItem("rechte Außenwand","Rechtsod",GetID(),pClonk,nil,nil,nil,2,6);
   AddMenuItem("linke Außenwand","Linksod",GetID(),pClonk,nil,nil,nil,2,9);
   AddMenuItem("rechte Außenwand mit schiefer Ebene","Rechtsschief",GetID(),pClonk,nil,nil,nil,2,7);
   AddMenuItem("linke Außenwand mit schiefer Ebene","Linksschief",GetID(),pClonk,nil,nil,nil,2,10);

   //AddMenuItem("Aufzug","Aufzug",KLAU,pClonk);
}

public func MaxDamage() {
  return(100);
}

protected func Damage() {
  // Bei mehr als maximalem Schaden
  if ((GetDamage() ?? 0) >= (MaxDamage() ?? 0))
  {
   if(pOverlay) RemoveObject(pOverlay);
   Incinerate();
   return(1);
  }
}