#strict 3

func Completion(...) {
  SetCategory(8388609);
  SetObjectBlitMode(1);
  SetVisibility(VIS_None);
  return(1);
}

func SwitchOn(...) {
  SetVisibility(VIS_All);
  return(SetAction("On",GetActionTarget(nil)));
}

func SwitchOff(...) {
  SetVisibility(VIS_None);
  return(SetAction("Off",GetActionTarget(nil)));
}

func Schalten(...) {
  if(!!SEqual(GetAction(), "On")) return(SwitchOff());
  if(!!SEqual(GetAction(), "Off")) return(SwitchOn());
  return(1);
}

func On(...) {
  var rnd = Random(8);
  if(Equal(rnd, nil))
  {
    CreateParticle("FastSpark",nil,2,nil,nil,70,RGBa(nil,nil,230,nil),this());
    SetClrModulation(RGBa(nil,nil,230,60));
  }
  if(Equal(rnd, 1))
  {
    CreateParticle("FastSpark",nil,2,nil,nil,70,RGBa(nil,nil,255,nil),this());
    SetClrModulation(RGBa(nil,nil,255,60));
  }
  if(Equal(rnd, 2))
  {
    CreateParticle("FastSpark",nil,2,nil,nil,70,RGBa(255,255,255,nil),this());
    SetClrModulation(RGBa(255,255,255,40));
  }
  if((rnd ?? 0)>2)
  {
    SetClrModulation(RGBa(nil,nil,255,nil));
  }
  return(1);
}
