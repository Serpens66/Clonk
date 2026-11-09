/*-- Gerüst --*/

#strict 2

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
   if(GetAction() == "Schieflinks") 
 {
   SetPosition(GetX(),GetY(),pClonk);
   SetAction("Rueckschieflinks");
   return(1);
  }

if(GetAction() == "Schiefrechts") 
 {
   SetPosition(GetX(),GetY(),pClonk);
   SetAction("Rueckschiefrechts");
   return(1);
  }

if(GetAction() == "Linksschief") 
 {
   SetPosition(GetX(),GetY(),pClonk);
   SetAction("Ruecklinksschief");
   return(1);
  }

if(GetAction() == "Rechtsschief") 
 {
   SetPosition(GetX(),GetY(),pClonk);
   SetAction("Rueckrechtsschief");
   return(1);
  }

if(GetAction() == "Rechtsod") 
 {
   SetAction("Rueckrechtsod");
   return(1);
  }

if(GetAction() == "Linksod") 
 {
   SetAction("Ruecklinksod");
   return(1);
  }

if(GetAction() == "Rechts") 
 {
   SetAction("Rueckrechts");
   return(1);
  }

if(GetAction() == "Links") 
 {
   SetAction("Ruecklinks");
   return(1);
  }

if(GetAction() == "Beide") 
 {
   SetAction("Rueckbeide");
   return(1);
  }

if(GetAction() == "Oben") 
 {
    SetAction("Rueckoben");
    return(1);
  }

if(GetAction() == "Normal")
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
  if(GetName(0, HZCK))
  {
   pOverlay = CreateObject(2X2H,0,12,GetOwner());
   Local(0,pOverlay)=this();
  }
  SetSolidMask(22,44,22,22,0,0);
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
  if(GetName(0, HZCK))
  {
   pOverlay = CreateObject(2X2H,0,12,GetOwner());
   Local(0,pOverlay)=this();
  }
    SetSolidMask(110,44,22,22,0,0);
   return(1);
}

func Schiefrechts2() {
  if(GetName(0, HZCK))
  {
   pOverlay = CreateObject(2X2H,0,12,GetOwner());
   Local(0,pOverlay)=this();
  }
    SetSolidMask(176,44,22,22,0,0);
   return(1);
}

func Rechts2() {
  if(GetName(0, HZCK))
  {
   pOverlay = CreateObject(2X2H,0,12,GetOwner());
   Local(0,pOverlay)=this();
  }
    SetSolidMask(66,44,22,22,0,0);
   return(1);
}

func Links2() {
  if(GetName(0, HZCK))
  {
   pOverlay = CreateObject(2X2H,0,12,GetOwner());
   Local(0,pOverlay)=this();
  }
    SetSolidMask(44,44,22,22,0,0);
   return(1);
}

func Beide2() {
  if(GetName(0, HZCK))
  {
   pOverlay = CreateObject(2X2H,0,12,GetOwner());
   Local(0,pOverlay)=this();
  }
    SetSolidMask(88,44,22,22,0,0);
   return(1);
}

func Normal() {
    SetSolidMask(0,22,22,22,0,0);
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
  if(GetName(0, HZCK))
  {
   pOverlay = CreateObject(2X2H,0,12,GetOwner());
   Local(0,pOverlay)=this();
  }
    SetSolidMask(132,44,22,22,0,0);
   return(1);
}

func Linksod() {
    SetAction("Zulinksod");
   return(1);
}

func Linksod2() {
  if(GetName(0, HZCK))
  {
   pOverlay = CreateObject(2X2H,0,12,GetOwner());
   Local(0,pOverlay)=this();
  }
    SetSolidMask(198,44,22,22,0,0);
   return(1);
}

func Rechtsschief() {
    SetAction("Zurechtsschief");
   return(1);
}

func Rechtsschief2() {
  if(GetName(0, HZCK))
  {
   pOverlay = CreateObject(2X2H,0,12,GetOwner());
   Local(0,pOverlay)=this();
  }
    SetSolidMask(154,44,22,22,0,0);
   return(1);
}
func Linksschief() {
    SetAction("Zulinksschief");
   return(1);
}

func Linksschief2() {
  if(GetName(0, HZCK))
  {
   pOverlay = CreateObject(2X2H,0,12,GetOwner());
   Local(0,pOverlay)=this();
  }
    SetSolidMask(220,44,22,22,0,0);
   return(1);
}

public func Menu(pClonk, object pObj) {

  if(pObj = FindObject2(Find_AtPoint(0,12),Find_Exclude(),Find_NoContainer(),Find_ID(GRUS),Find_Allied(GetOwner()), )) 
  {
   if(GetX() - GetX(pObj) >= (-5) && GetX() - GetX(pObj) <= (5))
    {
     SetPosition(GetX(pObj),GetY(pObj)-22,0);
    }
   if(GetX() - GetX(pObj) < (-5))
    {
     SetPosition(GetX(pObj)-11,GetY(pObj)-22,0);
    }
   if(GetX() - GetX(pObj) > 5)
    {
     SetPosition(GetX(pObj)+11,GetY(pObj)-22,0);
    }
  }
  if(pObj = FindObject2(Find_AtPoint(0,12),Find_Exclude(),Find_NoContainer(),Find_ID(KLZ1),Find_Allied(GetOwner()), )) 
  {
   if(GetX() - GetX(pObj) >= (-5) && GetX() - GetX(pObj) <= (5))
    {
     SetPosition(GetX(pObj),GetY(pObj)-16,0);
    }
   if(GetX() - GetX(pObj) < (-5))
    {
     SetPosition(GetX(pObj)-11,GetY(pObj)-16,0);
    }
   if(GetX() - GetX(pObj) > 5)
    {
     SetPosition(GetX(pObj)+11,GetY(pObj)-16,0);
    }
  }
  if(pObj = FindObject2(Find_AtPoint(0,12),Find_Exclude(),Find_NoContainer(),Find_ID(KLZ2),Find_Allied(GetOwner()), )) 
  {
   if(GetX() - GetX(pObj) < 0)
    {
     SetPosition(GetX(pObj)-5,GetY(pObj)-16,0);
    }
   if(GetX() - GetX(pObj) >= 0)
    {
     SetPosition(GetX(pObj)+6,GetY(pObj)-16,0);
    }
  }
   CreateMenu(GetID(),pClonk);
   AddMenuItem("Decke verfestigen","Oben",GetID(),pClonk,0,0,0,2,1);
   AddMenuItem("schiefe Ebene nach Links","Schieflinks",GetID(),pClonk,0,0,0,2,5);
   AddMenuItem("schiefe Ebene nach Rechts","Schiefrechts",GetID(),pClonk,0,0,0,2,8);
   AddMenuItem("rechte Außenwand und Decke","Rechts",GetID(),pClonk,0,0,0,2,3);
   AddMenuItem("linke Außenwand und Decke","Links",GetID(),pClonk,0,0,0,2,2);
   //AddMenuItem("beide Außenwände","Beide",GetID(),pClonk,0,0,0,2,4);

   AddMenuItem("rechte Außenwand","Rechtsod",GetID(),pClonk,0,0,0,2,6);
   AddMenuItem("linke Außenwand","Linksod",GetID(),pClonk,0,0,0,2,9);
   AddMenuItem("rechte Außenwand mit schiefer Ebene","Rechtsschief",GetID(),pClonk,0,0,0,2,7);
   AddMenuItem("linke Außenwand mit schiefer Ebene","Linksschief",GetID(),pClonk,0,0,0,2,10);

   //AddMenuItem("Aufzug","Aufzug",KLAU,pClonk);
}

public func MaxDamage() {
  return(100);
}

protected func Damage() {
  // Bei mehr als maximalem Schaden
  if (GetDamage() >= MaxDamage()) 
  {
   if(pOverlay) RemoveObject(pOverlay);
   Incinerate();
   return(1);
  }
}