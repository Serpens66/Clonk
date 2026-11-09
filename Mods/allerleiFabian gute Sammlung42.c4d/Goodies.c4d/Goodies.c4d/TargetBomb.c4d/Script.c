#strict 3

//Anmk: Script leicht geändern (Message) (MichaelK)

func Collection(...) { return(SetOwner(GetOwner(Par(nil)))); // by ArneB // return by flgr
}

func Hit(...) {
  if( Not( ActIdle() ) ) return(1);
  SetYDir(-50);
  SetAction("JustHitted");
  return(1);
}

func ScanningArea(...) {
  SetXDir(nil);
  SetYDir(nil);
  AssignVar(nil, FindObject2(Find_Distance(200),Find_OCF(OCF_Living),Find_NoContainer()) );
  if( Not( Hostile( GetOwner(), GetOwner(Var(nil)))))  return(1);

  SetAction( "FollowTheEnemy",Var(nil) );
  SetCommand(this(),"Follow",Var(nil));
  Message("*- Verfolger-Bombe verfolgt Gegner -*", this());

  return(1);
}

func DoFiring(...) {
  Call("ContCheck");
  SetVar(nil,GetActionTarget(nil));
  if( Contained( Var(nil) ) )  return(Call("SwitchOut"));

  if( Equal(Random(10),nil)) Call("YepShot");

  AssignVar(1,FindObject(nil,-10,-10,20,20,OCF_Living));
  if( Not( Hostile( GetOwner(), GetOwner(Var(1)))))  return(1);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(20);
  else FakeExplode(20,nil,nil,20);
  return(1);
}

func ContCheck(...) {
  if( Not( Contained() ) ) return(1);
  SetAction("Idle");
  if( Hostile( GetOwner(), GetOwner( Contained() ) )) if(!keinerdschaden) return( Explode(20) ); else FakeExplode(20,nil,nil,20);
  Message("*- Verfolger-Bombe ausgeschaltet -*", this());
  return(1);
}

func YepShot(...) {
  SetVar(nil,GetActionTarget(nil,this()));
  SetVar(1,Sum( Mul( GetX(),-1 ),GetX( Var(nil) ) ));
  SetVar(2,Sum( Mul( GetY(),-1 ),GetY( Var(nil) ) ));
  SetVar(3,CreateObject( SP52, nil, nil, GetOwner() ));
  ObjectSetAction(Var(3),"MovingAround");
  SetXDir(Var(1),Var(3));
  SetYDir(Var(2),Var(3));
  return(1);
}

func SwitchOut(...) {
  SetYDir(-50);
  SetAction("JustHitted");
  SetCommand(this(),"None");
  return(1);
}
