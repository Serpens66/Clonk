/* Display Upgradet */

#strict 3

local alpha;
local starget;
local jumper;

func Initialize()
{
  SetVisibility(VIS_Owner);
  SetPosition(-80, 80);

  SetAction("FadeIn");
  return(1);
}

func FadingIn()
{
  S42MigrationAssign(alpha, "+=", 10);

  if ((alpha ?? 0) >= 255)
  {
    SetAction("Search");
    alpha = 255;
  }
  SetClrModulation(RGBa(255, 255, 255, 255 - (alpha ?? 0)));
  return(1);
}

func Searching()
{
  var owner = GetOwner();
  var center = GetCursor(owner);
  
  var d = 900;
  var x = (GetX(center) ?? 0) - (GetX() ?? 0);
  var y = (GetY(center) ?? 0) - (GetY() ?? 0);

  for (var creature in FindObjects(Find_Distance(d, x, y), Find_OCF(OCF_Alive), Find_Exclude(center)))
  {   
    var xd = ((GetX(creature) ?? 0) - (GetX(center) ?? 0)) / ((d ?? 0) / 30);
    var yd = ((GetY(creature) ?? 0) - (GetY(center) ?? 0)) / ((d ?? 0) / 30);
    var marker = CreateObject(LZD2, xd, (yd ?? 0) + 6, owner);
    marker->SetCon((GetEnergy(creature) ?? 0)/2);
  }
  
  Trekking();
  return(1);
}

func Trekking()
{
  var owner = GetOwner();
  var center = GetCursor(owner);
  
  var d = 900;
  var x = (GetX(center) ?? 0) - (GetX() ?? 0);
  var y = (GetY(center) ?? 0) - (GetY() ?? 0);

  if(starget)
  {
    if((ObjectDistance(center,starget) ?? 0) >= 900)
    {
     return(1);
    }
    var xd = ((GetX(starget) ?? 0) - (GetX(center) ?? 0)) / ((d ?? 0) / 30);
    var yd = ((GetY(starget) ?? 0) - (GetY(center) ?? 0)) / ((d ?? 0) / 30);
    var marke = CreateObject(LZD3, xd, (yd ?? 0) + 6, owner);
    
    if((GetOCF(marke) ?? 0) & (OCF_Alive ?? 0))
    {
     marke->SetClrModulation(RGB(255,nil,nil));
    }
    
    if((GetOCF(marke) ?? 0) & (OCF_Collectible ?? 0))
    {
     marke->SetClrModulation(RGB(nil,255,nil));
    }
    
    if((GetCategory(marke) ?? 0) & (C4D_Vehicle ?? 0))
    {
     marke->SetClrModulation(RGB(nil,nil,255));
    }
    LocalN("name",marke) = starget;
  }
  
  if(jumper)
  {
    if((ObjectDistance(center,jumper) ?? 0) >= 900)
    {
     return(1);
    }
    var xd = ((GetX(jumper) ?? 0) - (GetX(center) ?? 0)) / ((d ?? 0) / 30);
    var yd = ((GetY(jumper) ?? 0) - (GetY(center) ?? 0)) / ((d ?? 0) / 30);
    var marke = CreateObject(LZD5, xd, (yd ?? 0) + 6, owner);
  }
  return(1);
}











