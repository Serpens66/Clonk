#strict

//Anmk: Script leicht geändern (Message) (MichaelK)

Collection: return(SetOwner(GetOwner(Par(0)))); // by ArneB // return by flgr

Hit:  
  if( Not( ActIdle() ) ) return(1);
  SetYDir(-50);
  SetAction("JustHitted");
  return(1);

ScanningArea:
  SetXDir(0);
  SetYDir(0);
  AssignVar(0, FindObject2(Find_Distance(200),Find_OCF(OCF_Living),Find_NoContainer()) );
  if( Not( Hostile( GetOwner(), GetOwner(Var(0)))))  return(1);

  SetAction( "FollowTheEnemy",Var(0) ); 
  SetCommand(this(),"Follow",Var(0));
  Message("*- Verfolger-Bombe verfolgt Gegner -*", this());

  return(1);

DoFiring:
  Call("ContCheck");
  SetVar(0,GetActionTarget(0));
  if( Contained( Var(0) ) )  return(Call("SwitchOut"));

  if( Equal(Random(10),0)) Call("YepShot");

  AssignVar(1,FindObject(0,-10,-10,20,20,OCF_Living()));
  if( Not( Hostile( GetOwner(), GetOwner(Var(1)))))  return(1);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(20);
  else FakeExplode(20,0,0,20);
  return(1);

ContCheck:
  if( Not( Contained() ) ) return(1);
  SetAction("Idle");
  if( Hostile( GetOwner(), GetOwner( Contained() ) )) if(!keinerdschaden) return( Explode(20) ); else FakeExplode(20,0,0,20);
  Message("*- Verfolger-Bombe ausgeschaltet -*", this());
  return(1);

YepShot:
  SetVar(0,GetActionTarget(0,this()));
  SetVar(1,Sum( Mul( GetX(),-1 ),GetX( Var(0) ) ));
  SetVar(2,Sum( Mul( GetY(),-1 ),GetY( Var(0) ) ));
  SetVar(3,CreateObject( SP52, 0, 0, GetOwner() ));
  ObjectSetAction(Var(3),"MovingAround");
  SetXDir(Var(1),Var(3));
  SetYDir(Var(2),Var(3));
  return(1);

SwitchOut:
  SetYDir(-50);
  SetAction("JustHitted");
  SetCommand(this(),"None");
  return(1);
