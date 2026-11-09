/* Display Upgradet */

#strict

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
  alpha += 10;

  if (alpha >= 255)
  {
    SetAction("Search");
    alpha = 255;
  }
  SetClrModulation(RGBa(255, 255, 255, 255 - alpha));
  return(1);
}

func Searching()
{
  var owner = GetOwner();
  var center = GetCursor(owner);
  
  var d = 900;
  var x = GetX(center) - GetX();
  var y = GetY(center) - GetY();

  for (var creature in FindObjects(Find_Distance(d, x, y), Find_OCF(OCF_Alive), Find_Exclude(center)))
  {   
    var xd = (GetX(creature) - GetX(center)) / (d / 30);
    var yd = (GetY(creature) - GetY(center)) / (d / 30);
    var marker = CreateObject(LZD2, xd, yd + 6, owner);
    marker->SetCon(GetEnergy(creature)/2);
  }
  
  Trekking();
  return(1);
}

func Trekking()
{
  var owner = GetOwner();
  var center = GetCursor(owner);
  
  var d = 900;
  var x = GetX(center) - GetX();
  var y = GetY(center) - GetY();

  if(starget)
  {
    if(ObjectDistance(center,starget) >= 900)
    {
     return(1);
    }
    var xd = (GetX(starget) - GetX(center)) / (d / 30);
    var yd = (GetY(starget) - GetY(center)) / (d / 30);
    var marke = CreateObject(LZD3, xd, yd + 6, owner);
    
    if(GetOCF(marke) & OCF_Alive())
    {
     marke->SetClrModulation(RGB(255,0,0));
    }
    
    if(GetOCF(marke) & OCF_Collectible())
    {
     marke->SetClrModulation(RGB(0,255,0));
    }
    
    if(GetCategory(marke) & C4D_Vehicle())
    {
     marke->SetClrModulation(RGB(0,0,255));
    }
    LocalN("name",marke) = starget;
  }
  
  if(jumper)
  {
    if(ObjectDistance(center,jumper) >= 900)
    {
     return(1);
    }
    var xd = (GetX(jumper) - GetX(center)) / (d / 30);
    var yd = (GetY(jumper) - GetY(center)) / (d / 30);
    var marke = CreateObject(LZD5, xd, yd + 6, owner);
  }
  return(1);
}











