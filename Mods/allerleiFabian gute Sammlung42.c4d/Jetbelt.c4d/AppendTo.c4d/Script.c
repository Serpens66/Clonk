/*-- Jetbelt --*/

#strict 3
#appendto HZCK

public func ControlUpDouble()
{
  var inh = _inherited(...);
  var jetpack = this()->HasGear(nil,JB4K);
  if(And(!inh, jetpack))
  {
      jetpack->UpDouble();
  }
}

public func ControlDownDouble()
{
  var inh = _inherited(...);
  var jetpack = this()->HasGear(nil,JB4K);
  if(And(!inh, jetpack))
  {
    jetpack->DownDouble();
  }
}

public func ControlLeftDouble()
{
  var inh = _inherited(...);
  var jetpack = this()->HasGear(nil,JB4K);
  if(And(!inh, jetpack))
  {
    jetpack->LeftDouble();
  }
}

public func ControlRightDouble()
{
  var inh = _inherited(...);
  var jetpack = this()->HasGear(nil,JB4K);
  if(And(!inh, jetpack))
  {
    jetpack->RightDouble();
  }
}

public func ControlLeft()
{
  _inherited(...);
  var jetpack = this()->HasGear(nil,JB4K);
  if(jetpack)
  {
    jetpack->Left();
  }
}

public func ControlRight()
{
  _inherited(...);
  var jetpack = this()->HasGear(nil,JB4K);
  if(jetpack)
  {
    jetpack->Right();
  }
}

public func ControlUp()
{
  _inherited(...);
  var jetpack = this()->HasGear(nil,JB4K);
  if(jetpack)
  {
    jetpack->Up();
  }
}

public func ControlDown()
{
  _inherited(...);
  var jetpack = this()->HasGear(nil,JB4K);
  if(jetpack)
  {
    jetpack->Down();
  }
}

/*public func ControlDigSingle()
{
  _inherited(...);
  var jetpack = this()->HasGear(0,JB4K);
  if(jetpack)
  {
    jetpack->Slow();
  }
}*/

public func ControlUpdate(object controller, int comdir)
{
  comdir ??= 0;
  var inh = _inherited(controller,comdir,...);
  var jetpack = this()->HasGear(nil,JB4K);
  if(And(!inh, jetpack))
  {
    jetpack->Update(comdir);
  }
}

//Agility

private func ControlAgility(string strControl)
{
  if(Or(Or((!!SEqual(strControl, "ControlLeftDouble")),
     (!!SEqual(strControl, "ControlRightDouble"))),
     (!!SEqual(strControl, "ControlUp"))))
  {
    var jetpack = this()->HasGear(nil,JB4K);
    if(jetpack)
      if(jetpack->IsReady())
        return(nil);
  }

  return(_inherited(strControl,...));
}