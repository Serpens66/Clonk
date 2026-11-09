#strict 3

// MSC-Script //

func Hit(...) {
  Sound("Thunder*");
    LaunchLightning(Sum(GetX(),Random(150)),nil, -20,35, +4,14 );
    LaunchLightning(Sum(GetX(),Random(150)),nil, -20,35, +4,14 );
    LaunchLightning(Sum(GetX(),Random(150)),nil, -20,41, +4,14 );
    LaunchLightning(Sum(GetX(),Random(150)),nil, -21,41, +5,15 );
    LaunchLightning(Sum(GetX(),Random(150)),nil, -21,37, +5,15 );
    RemoveObject();
  return(1);
}

func Activate(...) {
  Sound("Thunder*");
  AssignVar( nil, Sum( GetX(Par(nil)), GetVertex(nil,nil,Par(nil)) ) );
  AssignVar( 1, Sum( GetY(Par(nil)), GetVertex(nil,1,Par(nil)) ) );
  AssignVar( 2, Sum( -20, Mul(25,GetDir(Par(nil))) ) );
  AssignVar( 3, 15 );
  LaunchLightning( Var(nil), Var(1), Var(2), Var(3), -4,-4 );
  LaunchLightning( Var(nil), Var(1), Var(2), Var(3), -3,-3 );
  LaunchLightning( Var(nil), Var(1), Var(2), Var(3), -2,-2 );
  LaunchLightning( Var(nil), Var(1), Var(2), Var(3), -1,-1 );
  LaunchLightning( Var(nil), Var(1), Var(2), Var(3), nil,nil );
  LaunchLightning( Var(nil), Var(1), Var(2), Var(3), nil,nil );
  LaunchLightning( Var(nil), Var(1), Var(2), Var(3), 1,1 );
  LaunchLightning( Var(nil), Var(1), Var(2), Var(3), 2,2 );
  LaunchLightning( Var(nil), Var(1), Var(2), Var(3), 3,3 );
  LaunchLightning( Var(nil), Var(1), Var(2), Var(3), 4,4 );
  RemoveObject();
  return(1);
}

/* Kann chemisch werden */
func IsChemicalProduct(...) { return(1);
}

/* Forschung */

public func GetResearchBase(...) { return(EFLN);
}


