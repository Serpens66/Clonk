#strict

Completion:
  SetCategory(8388609);
  SetObjectBlitMode(1);
  SetVisibility(VIS_None);
  return(1);

SwitchOn:
  SetVisibility(VIS_All);
  return(SetAction("On",GetActionTarget(0)));

SwitchOff:
  SetVisibility(VIS_None);
  return(SetAction("Off",GetActionTarget(0)));

Schalten:
  if(GetAction()S="On") return(SwitchOff());
  if(GetAction()S="Off") return(SwitchOn());
  return(1);

On:
  var rnd = Random(8);
  if(rnd==0)
  {
    CreateParticle("FastSpark",0,2,0,0,70,RGBa(0,0,230,0),this());
    SetClrModulation(RGBa(0,0,230,60));
  }
  if(rnd==1)
  {
    CreateParticle("FastSpark",0,2,0,0,70,RGBa(0,0,255,0),this());
    SetClrModulation(RGBa(0,0,255,60));
  }
  if(rnd==2)
  {
    CreateParticle("FastSpark",0,2,0,0,70,RGBa(255,255,255,0),this());
    SetClrModulation(RGBa(255,255,255,40));
  }
  if(rnd>2)
  {
    SetClrModulation(RGBa(0,0,255,0));
  }
  return(1);
